#ifndef _CONSOLEIO_H
#define _CONSOLEIO_H

#include <tice.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>


#include "include/filemanger.h"
#include "include/tistring.h"
#include "include/memory.h"
#include "include/convert&tools.h"

void ti_get_in_d(char* data)
{
    uint16_t i = 0;
    if(data == NULL)
        return;
    for(char get_csc = os_GetCSC();get_csc != sk_Enter;i++)
       data[i] = get_csc;
    return;
}

char* ti_get_in_s()
{
    char* buffer = (char*)malloc(1);
    if(buffer != NULL)
    {
        for(uint16_t i = 0;i < UINT16_MAX;i++)
        {
            char get_char = os_GetCSC();
            char* temp = (char*)realloc(buffer,i+1);
            if(temp != NULL)
            {
                if(get_char == sk_Enter)
                {
                    buffer[i] = 0;
                    return buffer;
                }
                else
                {
                    buffer = temp;
                    buffer[i] = get_char;
                }
            }
            else
                os_ThrowError(-1);
        }
    }
    else
        os_ThrowError(-1);
    return buffer;
}

void ti_out_pf(uint16_t size,char* frmt,...)
{
    if(frmt == NULL)
        return;
    char *string= (char*)malloc(1);
    int24_t svalue,buffer_size;
    uint24_t uvalue;
    float fvalue;

    va_list va;
    va_start(va,frmt);

    for(uint16_t i = 0; ;i++)
    {
        char current = frmt[i];

        if(frmt[i] == '\0')
        {
            string = (char*)allocate_more(string,i+1);
            string[i+1] = 0;
            break;
        }
        else
        {
            if(frmt[i] == '%')
            {
                char next = frmt[i+1];
                char *buffer = (char*)malloc(20);
                switch(next)
                {
                    case 'd':
                        svalue = va_arg(va,int24_t);
                        itoa_32(&buffer,(int32_t)svalue,10,0);
                        buffer_size = strlen(buffer);
                        string = (char*)allocate_more(string,i + buffer_size);
                        if(memcpy(string + i,buffer,buffer_size) == NULL)
                            os_ThrowError(-1);
                        break;
                    case 's':
                        free(buffer);
                        buffer = va_arg(va,char*);
                        buffer_size = strlen(buffer);
                        string = (char*)allocate_more(string,i + buffer_size);
                        if(memcpy(string + i,buffer,buffer_size) == NULL)
                            os_ThrowError(-1);

                        break;
                    case 'p':
                        uvalue = va_arg(va,uint24_t);
                        itoa_32(&buffer,(uint32_t)uvalue,10,0);
                        buffer_size = strlen(buffer);
                        string = (char*)allocate_more(string,i + buffer_size);
                        if(memcpy(string + i,buffer,buffer_size) == NULL)
                            os_ThrowError(-1);
                        break;  
                    case 'f':
                        fvalue = va_arg(va,float);
                        buffer = ftoa_32(buffer,fvalue);
                        buffer_size = strlen(buffer);
                        string = (char*)allocate_more(string,i + buffer_size);
                        if(memcpy(string + i,buffer,buffer_size) == NULL)
                            os_ThrowError(-1);
                        break;
                }
                free(buffer);
                i++;
            }
            else
            {
                allocate_more(string,i+1);
                string[i] = current;
            }
        }
    }

    os_PutStrFull(string);
}

#endif