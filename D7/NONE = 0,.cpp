enum UTokenType
{
    NONE       = 0, // invalid, none
    IDENTIFIER = 1, // func name, var name, var type
    NAMESPACE  = 2, // ::
    DIRECTIVE  = 3, // #  call preprocessor directive                #include
    MACRO      = 4, // __ return preprocessor property how tokens    __LINE__
    MACRO_S    = 5, // ## return preprocessor property as string     ##LINE##

    // const 
    // 100 - 199
    GROUP_CONST = 100,
    INT       = 101, // 12345
    FLOAT     = 102, // 3.14f
    STRING    = 103, // "Hello world"
    CHAR      = 104, // ' '
    BOOLEAN   = 105, // true, false

    // math
    // 200 - 219
    GROUP_MATH = 200,
    ADD = 201, // +
    SUB = 202, // -
    MUL = 203, // *
    DIV = 204, // /
    MOD = 205, // %
    POW = 206, // ^
    INC = 207, // ++
    DEC = 208, // --

    // logical
    // 220 - 239
    GROUP_LOGICAL = 220,
    AND = 221, // &&
    OR  = 222, // ||
    NOT = 223, // !

    // equal operators
    // 240 - 259
    GROUP_EQUAL = 240,
    EQUAL         = 241, // ==
    NOT_EQUAL     = 242, // !=
    LESS          = 243, // <
    GREATER       = 244, // >
    LESS_EQUAL    = 245, // <=
    GREATER_EQUAL = 246, // >=

    // assign
    // 260 - 278
    GROUP_ASSIGN = 260,
    ASSIGN     = 261, // =
    ASSIGN_ADD = 262, // +=
    ASSIGN_SUB = 263, // -=
    ASSIGN_MUL = 264, // *=
    ASSIGN_DIV = 265, // /=
    ASSIGN_MOD = 266, // %=
    ASSIGN_POW = 267, // ^=





    // symbols
    // 300 - 319
    GROUP_SYMBOLS = 300,
    SEMICOLON = 301, // ;
    COLON     = 302, // :
    COMMA     = 303, // ,
    POINT     = 304, // .
    QUESTION  = 305, // ?
    AMPERSAND = 306, // &


    // brackets
    // 320 - 339
    GROUP_BRACKETS = 320,
    LBRA = 321, // {
    RBRA = 322, // }
    LPAR = 323, // (
    RPAR = 324, // )
    LSQR = 325, // [
    RSQR = 326, // ]

    // base func
    // 400 - 499
    GROUP_FUNC = 400,
    LABEL  = 401,
    GOTO   = 402,
    NEW    = 403,
    DELETE = 404,

    // control flow
    // 500 - 599
    GROUP_CONTROL = 500,
    RETURN    = 501,
    IF        = 502,
    ELSE      = 503,
    ELIF      = 504,
    DO_WHILE  = 505,
    WHILE     = 506,
    FOR       = 507,
    BREAK     = 508,
    CONTINUE  = 509,

    SWITCH    = 510,
    CASE      = 511,
    DEFAULT   = 512,

    /*
        EVERYTHING RELATED TO MACROS WILL BE MOVED! 
        IT'S PART OF THE PREPROCESSOR!
    */
#if 0
    // preprocessor func
    // 600 - 699
    GROUP_PREPROCESSOR_FUNC = 600,
    M_DEFINE  = 601,
    M_IFDEF   = 602,
    M_IFNDEF  = 603,
    M_IF      = 604,
    M_ELSE    = 605,
    M_ELIF    = 606,
    M_ENDIF   = 607,
    M_INCLUDE = 608,

    // preprocessor property
    // 700 - 799
    GROUP_PREPROCESSOR_PROPERTY = 700,

    MP_POS    = 701, // (str) 120:0
    MP_LINE   = 702, // (int) 120
    MP_COLUMN = 703, // (int) 0

    MP_FILE      = 710, // (str) C:/Users/Drevnii7/main.cpp
    MP_FILE_PATH = 711, // (str) C:/Users/Drevnii7/
    MP_FILE_NAME = 712, // (str) main.cpp

    MP_DATE         = 720, // (str) 08.05.2026
    MP_DATE_YEAR    = 721, // (int) 2026
    MP_DATE_MONTH   = 722, // (int) 5
    MP_DATE_DAY     = 723, // (int) 8

    MP_TIME          = 730, // (str) 16:08:02
    MP_TIME_HOUR     = 731, // (int) 16
    MP_TIME_MINUTE   = 732, // (int) 8
    MP_TIME_SECOND   = 733, // (int) 26

    MP_FUNC             = 740, // (tokens) void main(int argc, char *argv[])
    MP_FUNC_RETURN      = 741, // (tokens) void
    MP_FUNC_NAME        = 742, // (tokens) main
    MP_FUNC_ARGS        = 743, // (tokens) int argc, char *argv[]
    MP_FUNC_ARGS_TYPES  = 744, // (tokens) int, char *
    MP_FUNC_ARGS_NAMES  = 745, // (tokens) argc, argv
    MP_FUNC_ARGS_COUNT  = 746, // (int) 2
    MP_FUNC_DEFINE_LINE = 747, // (int) function declaration position
    MP_FUNC_LINE        = 748, // (int) position relative to the function declaration

    MP_TOKEN_TYPE       = 760, // (token type) get IDENTIFIER type (static reflection)
    MP_TOKEN_NAME       = 761, // (str) get IDENTIFIER name (static reflection)
#endif
}