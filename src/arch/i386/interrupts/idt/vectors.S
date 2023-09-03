/*********************************************************************
*
*   MODULE:
*       vectors.S
*
*   DESCRIPTION:
*       One vector function for each of the 256 IDT entries.
*
*********************************************************************/

/*--------------------------------------------------------------------
                               INCLUDES
--------------------------------------------------------------------*/
#include "memory/mmu.h"

/*--------------------------------------------------------------------
                          LITERAL CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                                TYPES
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                                MACROS
--------------------------------------------------------------------*/


/*----------------------------------------------------------
NOTE: The ; at the end of each line is there to break up
each line as the c preprocessor cannot insert newline chars.
----------------------------------------------------------*/
#define NORMAL_VEC(num)             \
    .globl vector##num##;           \
    .type vector##num##, @function; \
    vector##num##:                  \
    pushl $0;                       \
    pushl $(num);                   \
    jmp vector_common_lo

#define EXCEPT_VEC(num)             \
    .globl vector##num##;           \
    .type vector##num##, @function; \
    vector##num##:                  \
    pushl $(num);                   \
    jmp vector_common_lo

/*--------------------------------------------------------------------
                           MEMORY CONSTANTS
--------------------------------------------------------------------*/

# vector table
.data
.globl vector_table
vector_table:
  .long vector0
  .long vector1
  .long vector2
  .long vector3
  .long vector4
  .long vector5
  .long vector6
  .long vector7
  .long vector8
  .long vector9
  .long vector10
  .long vector11
  .long vector12
  .long vector13
  .long vector14
  .long vector15
  .long vector16
  .long vector17
  .long vector18
  .long vector19
  .long vector20
  .long vector21
  .long vector22
  .long vector23
  .long vector24
  .long vector25
  .long vector26
  .long vector27
  .long vector28
  .long vector29
  .long vector30
  .long vector31
  .long vector32
  .long vector33
  .long vector34
  .long vector35
  .long vector36
  .long vector37
  .long vector38
  .long vector39
  .long vector40
  .long vector41
  .long vector42
  .long vector43
  .long vector44
  .long vector45
  .long vector46
  .long vector47
  .long vector48
  .long vector49
  .long vector50
  .long vector51
  .long vector52
  .long vector53
  .long vector54
  .long vector55
  .long vector56
  .long vector57
  .long vector58
  .long vector59
  .long vector60
  .long vector61
  .long vector62
  .long vector63
  .long vector64
  .long vector65
  .long vector66
  .long vector67
  .long vector68
  .long vector69
  .long vector70
  .long vector71
  .long vector72
  .long vector73
  .long vector74
  .long vector75
  .long vector76
  .long vector77
  .long vector78
  .long vector79
  .long vector80
  .long vector81
  .long vector82
  .long vector83
  .long vector84
  .long vector85
  .long vector86
  .long vector87
  .long vector88
  .long vector89
  .long vector90
  .long vector91
  .long vector92
  .long vector93
  .long vector94
  .long vector95
  .long vector96
  .long vector97
  .long vector98
  .long vector99
  .long vector100
  .long vector101
  .long vector102
  .long vector103
  .long vector104
  .long vector105
  .long vector106
  .long vector107
  .long vector108
  .long vector109
  .long vector110
  .long vector111
  .long vector112
  .long vector113
  .long vector114
  .long vector115
  .long vector116
  .long vector117
  .long vector118
  .long vector119
  .long vector120
  .long vector121
  .long vector122
  .long vector123
  .long vector124
  .long vector125
  .long vector126
  .long vector127
  .long vector128
  .long vector129
  .long vector130
  .long vector131
  .long vector132
  .long vector133
  .long vector134
  .long vector135
  .long vector136
  .long vector137
  .long vector138
  .long vector139
  .long vector140
  .long vector141
  .long vector142
  .long vector143
  .long vector144
  .long vector145
  .long vector146
  .long vector147
  .long vector148
  .long vector149
  .long vector150
  .long vector151
  .long vector152
  .long vector153
  .long vector154
  .long vector155
  .long vector156
  .long vector157
  .long vector158
  .long vector159
  .long vector160
  .long vector161
  .long vector162
  .long vector163
  .long vector164
  .long vector165
  .long vector166
  .long vector167
  .long vector168
  .long vector169
  .long vector170
  .long vector171
  .long vector172
  .long vector173
  .long vector174
  .long vector175
  .long vector176
  .long vector177
  .long vector178
  .long vector179
  .long vector180
  .long vector181
  .long vector182
  .long vector183
  .long vector184
  .long vector185
  .long vector186
  .long vector187
  .long vector188
  .long vector189
  .long vector190
  .long vector191
  .long vector192
  .long vector193
  .long vector194
  .long vector195
  .long vector196
  .long vector197
  .long vector198
  .long vector199
  .long vector200
  .long vector201
  .long vector202
  .long vector203
  .long vector204
  .long vector205
  .long vector206
  .long vector207
  .long vector208
  .long vector209
  .long vector210
  .long vector211
  .long vector212
  .long vector213
  .long vector214
  .long vector215
  .long vector216
  .long vector217
  .long vector218
  .long vector219
  .long vector220
  .long vector221
  .long vector222
  .long vector223
  .long vector224
  .long vector225
  .long vector226
  .long vector227
  .long vector228
  .long vector229
  .long vector230
  .long vector231
  .long vector232
  .long vector233
  .long vector234
  .long vector235
  .long vector236
  .long vector237
  .long vector238
  .long vector239
  .long vector240
  .long vector241
  .long vector242
  .long vector243
  .long vector244
  .long vector245
  .long vector246
  .long vector247
  .long vector248
  .long vector249
  .long vector250
  .long vector251
  .long vector252
  .long vector253
  .long vector254
  .long vector255


/*--------------------------------------------------------------------
                              VARIABLES
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                              PROCEDURES
--------------------------------------------------------------------*/

.text

/*********************************************************************
*
*   PROCEDURE NAME:
*       vector_common_lo
*
*   DESCRIPTION:
*       builds exception frame and calls vector_common_hi()
*
*********************************************************************/
.global vector_common_lo
.type vector_common_lo, @function
vector_common_lo:

    // Save segment selectors
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    // Save general regs
    pushal

    // Switch to kernel data segment
    movw $(SEG_KDATA<<3), %ax

    // Pass control to high level handler
    pushl %esp
    call vector_common_hi
    addl $4, %esp

vector_ret:    
    popal
    popl %gs
    popl %fs
    popl %es
    popl %ds
    addl $0x8, %esp // vector number and error code.
    iret


NORMAL_VEC(0)
NORMAL_VEC(1)
NORMAL_VEC(2)
NORMAL_VEC(3)
NORMAL_VEC(4)
NORMAL_VEC(5)
NORMAL_VEC(6)
NORMAL_VEC(7)
EXCEPT_VEC(8)
NORMAL_VEC(9)
EXCEPT_VEC(10)
EXCEPT_VEC(11)
EXCEPT_VEC(12)
EXCEPT_VEC(13)
EXCEPT_VEC(14)
NORMAL_VEC(15)
NORMAL_VEC(16)
EXCEPT_VEC(17)
NORMAL_VEC(18)
NORMAL_VEC(19)
NORMAL_VEC(20)
NORMAL_VEC(21)
NORMAL_VEC(22)
NORMAL_VEC(23)
NORMAL_VEC(24)
NORMAL_VEC(25)
NORMAL_VEC(26)
NORMAL_VEC(27)
NORMAL_VEC(28)
NORMAL_VEC(29)
NORMAL_VEC(30)
NORMAL_VEC(31)
NORMAL_VEC(32)
NORMAL_VEC(33)
NORMAL_VEC(34)
NORMAL_VEC(35)
NORMAL_VEC(36)
NORMAL_VEC(37)
NORMAL_VEC(38)
NORMAL_VEC(39)
NORMAL_VEC(40)
NORMAL_VEC(41)
NORMAL_VEC(42)
NORMAL_VEC(43)
NORMAL_VEC(44)
NORMAL_VEC(45)
NORMAL_VEC(46)
NORMAL_VEC(47)
NORMAL_VEC(48)
NORMAL_VEC(49)
NORMAL_VEC(50)
NORMAL_VEC(51)
NORMAL_VEC(52)
NORMAL_VEC(53)
NORMAL_VEC(54)
NORMAL_VEC(55)
NORMAL_VEC(56)
NORMAL_VEC(57)
NORMAL_VEC(58)
NORMAL_VEC(59)
NORMAL_VEC(60)
NORMAL_VEC(61)
NORMAL_VEC(62)
NORMAL_VEC(63)
NORMAL_VEC(64)
NORMAL_VEC(65)
NORMAL_VEC(66)
NORMAL_VEC(67)
NORMAL_VEC(68)
NORMAL_VEC(69)
NORMAL_VEC(70)
NORMAL_VEC(71)
NORMAL_VEC(72)
NORMAL_VEC(73)
NORMAL_VEC(74)
NORMAL_VEC(75)
NORMAL_VEC(76)
NORMAL_VEC(77)
NORMAL_VEC(78)
NORMAL_VEC(79)
NORMAL_VEC(80)
NORMAL_VEC(81)
NORMAL_VEC(82)
NORMAL_VEC(83)
NORMAL_VEC(84)
NORMAL_VEC(85)
NORMAL_VEC(86)
NORMAL_VEC(87)
NORMAL_VEC(88)
NORMAL_VEC(89)
NORMAL_VEC(90)
NORMAL_VEC(91)
NORMAL_VEC(92)
NORMAL_VEC(93)
NORMAL_VEC(94)
NORMAL_VEC(95)
NORMAL_VEC(96)
NORMAL_VEC(97)
NORMAL_VEC(98)
NORMAL_VEC(99)
NORMAL_VEC(100)
NORMAL_VEC(101)
NORMAL_VEC(102)
NORMAL_VEC(103)
NORMAL_VEC(104)
NORMAL_VEC(105)
NORMAL_VEC(106)
NORMAL_VEC(107)
NORMAL_VEC(108)
NORMAL_VEC(109)
NORMAL_VEC(110)
NORMAL_VEC(111)
NORMAL_VEC(112)
NORMAL_VEC(113)
NORMAL_VEC(114)
NORMAL_VEC(115)
NORMAL_VEC(116)
NORMAL_VEC(117)
NORMAL_VEC(118)
NORMAL_VEC(119)
NORMAL_VEC(120)
NORMAL_VEC(121)
NORMAL_VEC(122)
NORMAL_VEC(123)
NORMAL_VEC(124)
NORMAL_VEC(125)
NORMAL_VEC(126)
NORMAL_VEC(127)
NORMAL_VEC(128)
NORMAL_VEC(129)
NORMAL_VEC(130)
NORMAL_VEC(131)
NORMAL_VEC(132)
NORMAL_VEC(133)
NORMAL_VEC(134)
NORMAL_VEC(135)
NORMAL_VEC(136)
NORMAL_VEC(137)
NORMAL_VEC(138)
NORMAL_VEC(139)
NORMAL_VEC(140)
NORMAL_VEC(141)
NORMAL_VEC(142)
NORMAL_VEC(143)
NORMAL_VEC(144)
NORMAL_VEC(145)
NORMAL_VEC(146)
NORMAL_VEC(147)
NORMAL_VEC(148)
NORMAL_VEC(149)
NORMAL_VEC(150)
NORMAL_VEC(151)
NORMAL_VEC(152)
NORMAL_VEC(153)
NORMAL_VEC(154)
NORMAL_VEC(155)
NORMAL_VEC(156)
NORMAL_VEC(157)
NORMAL_VEC(158)
NORMAL_VEC(159)
NORMAL_VEC(160)
NORMAL_VEC(161)
NORMAL_VEC(162)
NORMAL_VEC(163)
NORMAL_VEC(164)
NORMAL_VEC(165)
NORMAL_VEC(166)
NORMAL_VEC(167)
NORMAL_VEC(168)
NORMAL_VEC(169)
NORMAL_VEC(170)
NORMAL_VEC(171)
NORMAL_VEC(172)
NORMAL_VEC(173)
NORMAL_VEC(174)
NORMAL_VEC(175)
NORMAL_VEC(176)
NORMAL_VEC(177)
NORMAL_VEC(178)
NORMAL_VEC(179)
NORMAL_VEC(180)
NORMAL_VEC(181)
NORMAL_VEC(182)
NORMAL_VEC(183)
NORMAL_VEC(184)
NORMAL_VEC(185)
NORMAL_VEC(186)
NORMAL_VEC(187)
NORMAL_VEC(188)
NORMAL_VEC(189)
NORMAL_VEC(190)
NORMAL_VEC(191)
NORMAL_VEC(192)
NORMAL_VEC(193)
NORMAL_VEC(194)
NORMAL_VEC(195)
NORMAL_VEC(196)
NORMAL_VEC(197)
NORMAL_VEC(198)
NORMAL_VEC(199)
NORMAL_VEC(200)
NORMAL_VEC(201)
NORMAL_VEC(202)
NORMAL_VEC(203)
NORMAL_VEC(204)
NORMAL_VEC(205)
NORMAL_VEC(206)
NORMAL_VEC(207)
NORMAL_VEC(208)
NORMAL_VEC(209)
NORMAL_VEC(210)
NORMAL_VEC(211)
NORMAL_VEC(212)
NORMAL_VEC(213)
NORMAL_VEC(214)
NORMAL_VEC(215)
NORMAL_VEC(216)
NORMAL_VEC(217)
NORMAL_VEC(218)
NORMAL_VEC(219)
NORMAL_VEC(220)
NORMAL_VEC(221)
NORMAL_VEC(222)
NORMAL_VEC(223)
NORMAL_VEC(224)
NORMAL_VEC(225)
NORMAL_VEC(226)
NORMAL_VEC(227)
NORMAL_VEC(228)
NORMAL_VEC(229)
NORMAL_VEC(230)
NORMAL_VEC(231)
NORMAL_VEC(232)
NORMAL_VEC(233)
NORMAL_VEC(234)
NORMAL_VEC(235)
NORMAL_VEC(236)
NORMAL_VEC(237)
NORMAL_VEC(238)
NORMAL_VEC(239)
NORMAL_VEC(240)
NORMAL_VEC(241)
NORMAL_VEC(242)
NORMAL_VEC(243)
NORMAL_VEC(244)
NORMAL_VEC(245)
NORMAL_VEC(246)
NORMAL_VEC(247)
NORMAL_VEC(248)
NORMAL_VEC(249)
NORMAL_VEC(250)
NORMAL_VEC(251)
NORMAL_VEC(252)
NORMAL_VEC(253)
NORMAL_VEC(254)
NORMAL_VEC(255)