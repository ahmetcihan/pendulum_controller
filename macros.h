#ifndef MACROS_H
#define MACROS_H

typedef char s8;
typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

//#define MATLAB_RECORDINGS

#define TEST_STOPPED            0
#define TEST_RUNNING            1
#define TEST_PAUSED             2
#define TEST_JUST_FINISHED      3

#define RELAY_OFF               48
#define RELAY_ON                49

#define LOAD_PACE_CONTROL       1
#define DISP_PACE_CONTROL       2

#define V_AXIS_LOAD             0
#define V_AXIS_STRESS           1
#define V_AXIS_DISPLACEMENT     2

#define H_AXIS_TIME                 0
#define H_AXIS_DISPLACEMENT         1

#define FIXED_AXES              0
#define AUTO_ZOOM               1

#define CBR                     0
#define MARSHALL                1
#define IDT                     2
#define SCB                     3
#define TRIAXIAL                4
#define UU                      5
#define CU_CD                   6
#define UNIAXIAL                7
#define LOAD_CONTROL            8
#define DISPLACEMENT_CONTROL    9

#define ASTM_STANDARD_CBR       0
#define EN_STANDARD_CBR         1
#define BS_STANDARD_CBR         2

#define ASTM_STANDARD_MARSHALL  0
#define EN_STANDARD_MARSHALL    1

#define FLAT                    0
#define ASCENDING               1
#define DESCENDING              2

#define LOAD_1                  0
#define LOAD_2                  1
#define LOAD_3                  2

#define AUTO                    0
#define MAN                     1

#define PAUSE                   0
#define STOP                    1

#define TAB_MAINSCREEN          0
#define TAB_TEST                1
#define TAB_PARAMETERS          2
#define TAB_TEST_RESULTS        3
#define TAB_SETTINGS            4
#define TAB_CALIBRATION         5
#define TAB_ADMIN               6
#define TAB_RESERVED            7

#define QStringLiteral QString::fromUtf8

#define SCREEN_POS_1            5,141
#define SCREEN_POS_2            5,209
#define SCREEN_POS_3            5,277
#define SCREEN_POS_4            5,345

#define DOUBLE_CLICK_BLOCK_TIME 400

#define KN_MM       0
#define LBF_IN      1
#define KGF_CM      2

#define UP          0
#define DOWN        1

#endif // MACROS_H
