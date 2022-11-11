//=====================================================================================================================================

#include "filework.h"

//=====================================================================================================================================

void TextCtor (Text* text, FILE* const source)
{
    ASSERT (source != nullptr);
    ASSERT (text   != nullptr);

    readInBuff (text, source);

    matchLines (text);
}

//=====================================================================================================================================

void ReadInBuf (Text* text, FILE* source)
{
    ASSERT (text   != nullptr);
    ASSERT (source != nullptr);
    
    text->nChar = fileSize (source);

    text->bufffer = (char*) calloc (text->nChar + 1, sizeof (char));
    
    fread (text->buffer, sizeof(char), text->nChar, source);
}

//=====================================================================================================================================

size_t FileSize (FILE* file)
{
    ASSERT (file != nullptr);

	size_t curPos = ftell (stream);

	fseek (stream, 0L, SEEK_END);
	size_t size = ftell (stream);

	if (size == -1)
    {
        printf ("Error in function: %s. Error size = -1!\n", __func__);
        return -1;
    }

	fseek (stream, curPos, SEEK_SET);

	return size;
}

//=====================================================================================================================================

void MatchLines (Text* text)
{
    size_t   line        = 0;
    size_t   lineStart   = 0;
    
    countInText (text->buffer, '\n', &(text->nLines));
    text->lines = (FileLines*) calloc (text->nLines + 1, sizeof(FileLines));
    
    for (size_t index = 1; index < text->nChar; index++)
    {
        if (*(text->buffer + index) == '\n')
        {
            (*(text->buffer + index)= '\0');

            (text->lines + line)->lineStart = text->buffer + lineStart;
            (text->lines + line)->lineLen   = index - lineStart;
            (text->lines + line)->lineIndex = line;

            lineStart = index + 1;

            line = line + 1;
        }

        if (lineStart > text->nChar)
        {
            break;
        }
    }

}

//=====================================================================================================================================
     
void CountInText (char* text, char search, size_t* count)
{
    size_t index = 0;

    while (*(text + index))  
    {
        if (*(text + index) == search)
        {
            ++(*count);
        }
        ++index;
    }
}

//=====================================================================================================================================

void TextDtor(Text* text)
{
    ASSERT (text != nullptr);

    free (text->buffer); 
    free (text->lines);
}

//=====================================================================================================================================
