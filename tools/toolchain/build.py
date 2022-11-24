'''
This script downloads the source for and then builds the toolchain
that we need to build the my_os project.
'''
#*******************************************************************
#                              Imports
#*******************************************************************
import os
import os.path
import requests
import contextlib
import tarfile
import subprocess
import shutil

#*******************************************************************
#                             Constants
#*******************************************************************
SCRIPT_DIR = os.path.dirname(__file__)

DOWNLOAD_DIR = os.path.normpath(SCRIPT_DIR + "/downloads")
shutil.rmtree(DOWNLOAD_DIR, ignore_errors=True)
os.mkdir(DOWNLOAD_DIR)

ARCH = "x86"
TARGET = "i686-elf"
PREFIX = os.path.normpath(SCRIPT_DIR + "/local/" + ARCH)
BUILD_DIR = os.path.normpath(SCRIPT_DIR + "/build/" + ARCH)

GCC_VERSION = "12.2.0"
GCC_BASE_URL = "https://ftp.gnu.org/gnu/gcc/gcc-{}".format(GCC_VERSION)
GCC_NAME = "gcc-{}".format(GCC_VERSION)
GCC_FILE_NAME = "{}.tar.gz".format(GCC_NAME)

BINUTILS_VERSION = "2.39"
BINUTILS_BASE_URL = "https://ftp.gnu.org/gnu/binutils"
BINUTILS_NAME = "binutils-{}".format(BINUTILS_VERSION)
BINUTILS_FILE_NAME = "{}.tar.gz".format(BINUTILS_NAME)

#*******************************************************************
#                             Procedures
#*******************************************************************
def main():

    #-------------------------------------------------------
    # Download Sources
    #-------------------------------------------------------
    with change_working_dir(DOWNLOAD_DIR):
        
        # Download Binutils
        print("Downloading Binutils")
        open(BINUTILS_FILE_NAME, "wb").write(requests.get(BINUTILS_BASE_URL + "/" + BINUTILS_FILE_NAME).content)

        # Download GCC
        print("Downloading GCC")
        open(GCC_FILE_NAME, "wb").write(requests.get(GCC_BASE_URL + "/" + GCC_FILE_NAME).content)
        
        # Extract Binutils
        print("Extracting Binutils")
        tmp_archive = tarfile.open(BINUTILS_FILE_NAME)
        tmp_archive.extractall()
        tmp_archive.close()

        # Extract GCC
        print("Extracting GCC")
        tmp_archive = tarfile.open(GCC_FILE_NAME)
        tmp_archive.extractall()
        tmp_archive.close()

    #-------------------------------------------------------
    # Compile Code
    #-------------------------------------------------------
    if os.path.exists(PREFIX):
        shutil.rmtree(PREFIX)
    
    if os.path.exists(BUILD_DIR):
        shutil.rmtree(BUILD_DIR)

    os.makedirs(PREFIX)
    os.makedirs(BUILD_DIR)

    with change_working_dir(BUILD_DIR):

        #---------------------------------------------------
        # Build Binutils
        #---------------------------------------------------
        os.makedirs("binutils")

        with change_working_dir("binutils"):
            print("Building binutils")

            build_step(
                "Binutils/configure",
                ["{}/downloads/{}/configure".format(SCRIPT_DIR, BINUTILS_NAME),
                "--target={}".format(TARGET),
                '--prefix={}'.format(PREFIX),
                "--with-sysroot",
                "--disable-nls",
                "--disable-werror"]
                )

            build_step(
                "Binutils/make",
                ["make"]
                )

            build_step(
                "Binutils/install",
                ["make", "install"]
                )

        #---------------------------------------------------
        # Build GCC
        #---------------------------------------------------
        with change_working_dir("gcc"):
            print("Building GCC")

            build_step(
                "GCC/configure",
                ["{}/downloads/{}/configure".format(SCRIPT_DIR, GCC_NAME),
                "--target={}".format(TARGET),
                "--prefix={}".format(PREFIX),
                "--disable-nls",
                "--enable-languages=c,c++",
                "--without-headers"
                ])

            build_step(
                "GCC/build gcc",
                ["make", "all-gcc"]
                )

            build_step(
                "GCC/build libgcc",
                ["make", "all-target-libgcc"]
                )

            build_step(
                "GCC/install gcc",
                ["make", "install-gcc"]
                )

            build_step(
                "GCC/install libgcc",
                ["make", "install-libgcc"]
                )


def build_step(message, command):
    with subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True) as process:
        prefix = "\x1b[34m[{}]\x1b[39m ".format(message)

        print(prefix + "Command: " + str(process.args))

        for line in process.stdout:
            print(prefix + line.strip("\n"))

@contextlib.contextmanager
def change_working_dir(new_dir):
    '''
    Emulate bash pushd/popd like behavior in python
    '''
    previous_dir = os.getcwd()
    os.chdir(new_dir)
    try:
        yield
    finally:
        os.chdir(previous_dir)

if __name__=="__main__":
    main()