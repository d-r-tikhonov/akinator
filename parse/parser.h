#ifndef PARSE_H
#define PARSE_H

//=====================================================================================================================================

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//=====================================================================================================================================

#ifndef NDEBUG
    #define ASSERT(condition)                                                                   \
        if (!(condition))                                                                       \
        {                                                                                       \
            printf ("Assertion FAILED! Error in %s. FILE: %s. LINE: %d. FUNCTION: %s\n",        \
                    #condition, __FILE__, __LINE__, __PRETTY_FUNCTION__);                       \
        }
#else
    #define ASSERT(condition) ;
#endif

//=====================================================================================================================================

struct fileLines
{
    char* lineStart = nullptr;

    size_t lineLen   = 0;
    size_t lineIndex = 0;
};

struct Text
{
    char* buffer = nullptr;

    fileLines* lines = nullptr;

    size_t nChar  = 0;
    size_t nLines = 0;
};

//=====================================================================================================================================

void        textCtor        (Text * text, FILE* const source);
size_t      fileSize        (FILE* const file);
void        countInText     (char* buf, char search, size_t* count);
void        readInBuf       (Text* text, FILE* source);
void        matchLines      (Text* text);
void        textDtor        (Text* text); 

//=====================================================================================================================================

#endif