'''
This script downloads the source for and then builds the toolchain
that we need to build the my_os project.
'''
#*******************************************************************
#                              Imports
#
# NOTE:
#   When changing the imports remember to update the requirements.txt 
#   file via:
#       "pip install pipreqs"
#       "pipreqs --force --ignore build,CMake,downloads,local ./"
#
#   Pipreqs repo: https://github.com/bndr/pipreqs
#
#*******************************************************************

import os
import os.path
import contextlib
import tarfile
import subprocess
import shutil
import wget

#*******************************************************************
#                             Constants
#*******************************************************************
SCRIPT_DIR = os.path.dirname(__file__)

ARCH = "x86"
TARGET = "i686-elf"

DOWNLOAD_DIR = os.path.normpath(SCRIPT_DIR + "/downloads")
BUILD_DIR = os.path.normpath(SCRIPT_DIR + "/build/" + ARCH)
PREFIX = os.path.normpath(SCRIPT_DIR + "/local/" + ARCH)

GCC_VERSION = "12.2.0"
GCC_BASE_URL = "https://ftp.gnu.org/gnu/gcc/gcc-{}".format(GCC_VERSION)
GCC_NAME = "gcc-{}".format(GCC_VERSION)
GCC_FILE_NAME = "{}.tar.gz".format(GCC_NAME)

BINUTILS_VERSION = "2.39"
BINUTILS_BASE_URL = "https://ftp.gnu.org/gnu/binutils"
BINUTILS_NAME = "binutils-{}".format(BINUTILS_VERSION)
BINUTILS_FILE_NAME = "{}.tar.gz".format(BINUTILS_NAME)

NR_CPUS = str(os.cpu_count())

#*******************************************************************
#                             Procedures
#*******************************************************************
def main():
    #-------------------------------------------------------
    # Set up directories
    #-------------------------------------------------------
    os.makedirs(DOWNLOAD_DIR, exist_ok=True)
    
    shutil.rmtree(BUILD_DIR, ignore_errors=True)
    os.makedirs(BUILD_DIR)

    #-------------------------------------------------------
    # Download Sources
    #-------------------------------------------------------
    with change_working_dir(DOWNLOAD_DIR):
        # TODO: Verify existing downloaded files using their associated pgp signature files or an MD5 hash
        
        # Download Binutils source if not already
        if (not os.path.exists(BINUTILS_FILE_NAME)):
            print("Downloading Binutils")
            # open(BINUTILS_FILE_NAME, "wb").write(requests.get(BINUTILS_BASE_URL + "/" + BINUTILS_FILE_NAME).content)
            wget.download(BINUTILS_BASE_URL + "/" + BINUTILS_FILE_NAME)
        else:
            print("Skipping Binutils download")

        # Download GCC source if not already
        if (not os.path.exists(GCC_FILE_NAME)):
            print("Downloading GCC")
            # open(GCC_FILE_NAME, "wb").write(requests.get(GCC_BASE_URL + "/" + GCC_FILE_NAME).content)
            wget.download(GCC_BASE_URL + "/" + GCC_FILE_NAME)
        else:
            print("Skipping GCC download")

        # Extract Binutils
        print("Extracting Binutils")
        tmp_archive = tarfile.open(BINUTILS_FILE_NAME)
        tmp_archive.extractall() # extracts to current directory
        tmp_archive.close()

        # Extract GCC
        print("Extracting GCC")
        tmp_archive = tarfile.open(GCC_FILE_NAME)
        tmp_archive.extractall() # extracts to current directory
        tmp_archive.close()

    #-------------------------------------------------------
    # Compile Code
    #-------------------------------------------------------
    with change_working_dir(BUILD_DIR):
        
        shutil.rmtree(PREFIX, ignore_errors=True)
        os.makedirs(PREFIX)

        #---------------------------------------------------
        # Build Binutils
        #---------------------------------------------------
        shutil.rmtree("binutils-build", ignore_errors=True)
        os.makedirs("binutils-build")

        with change_working_dir("binutils-build"):
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
                ["make", "-j", NR_CPUS]
                )

            build_step(
                "Binutils/install",
                ["make", "install"]
                )

        #---------------------------------------------------
        # Build GCC
        #---------------------------------------------------
        shutil.rmtree("gcc-build", ignore_errors=True)
        os.makedirs("gcc-build")

        with change_working_dir("gcc-build"):
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
                ["make", "all-gcc", "-j", NR_CPUS]
                )

            build_step(
                "GCC/build libgcc",
                ["make", "all-target-libgcc", "-j", NR_CPUS]
                )

            build_step(
                "GCC/install gcc",
                ["make", "install-gcc"]
                )

            build_step(
                "GCC/install libgcc",
                ["make", "install-target-libgcc"]
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