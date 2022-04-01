#ifndef EE3420_MEGA_H
#define EE3420_MEGA_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "ASCII.h"

#define ERROR_UNKNOWN_COMMAND -1
#define ERROR_INVALID_PARAMETER -2
#define ERROR_INVALID_DATA -3
#define ERROR_UNAVAILABLE -4
#define ERROR_TIMEOUT -5


char serial_receive_buffer[81];
int serial_receive_buffer_pointer=0;
char serial_received_char;
int serial_received_instruction_complete=0;
char serial_buffer[81];

char serial_device_type[40];
char serial_device_name[40];

char my_device_type[]="Mega2560";
char my_device_name[]="Megamouth";

void serial_wait_for_enquire()
{
  while((serial_received_char = Serial.read()) != ENQ) { }
  Serial.write(ACK);
}

void clear_serial_receive_buffer()
{
 serial_receive_buffer[0]='\0';
 serial_received_instruction_complete=0;
} 

void serial_build_command()
{
  char c;
  char buf[3];
  buf[0]=0;
  buf[1]=0;
  buf[2]=0;

  while((Serial.available()>0)&&(!serial_received_instruction_complete))
  {
    buf[0]=c=Serial.read();
    if(c=='\n')
    {
      buf[0]=0;
      serial_received_instruction_complete=1;
    }
    if(c=='\r')
    {
      buf[0]=0;
    }
    if((c==ACK)||(c==NACK)||(c==ENQ))
    {
      buf[0]=0;
    }
    strcat(serial_receive_buffer,buf); 

    /*a carriage return and newline can cause a zero-length command */
    if((serial_received_instruction_complete==1) && (strlen(serial_receive_buffer)==0))
    {serial_received_instruction_complete=0;}
  }
}

#define max_parameter_limit 20
#define max_parameter_length 40
char serial_command_parameters[max_parameter_limit][max_parameter_length];
int serial_command_execution_complete;
int command_parameter_counter;
//char * tok;
char parameter_separators[]=" :,\r\n";

int parse_command_string(char input_string[], char parameters[max_parameter_limit][max_parameter_length], char separators[])
{
  int command_counter;
  char * tok;
  // char token_separators[]=" :,\r\n"
  
  for(command_counter=0; command_counter<max_parameter_limit; command_counter++)
  {
    parameters[command_counter][0]='\0';
  }
  
  command_counter=0;
  tok=strtok(input_string,separators);
  while((tok != NULL)&&(command_counter<max_parameter_limit))
  {
    //printf("TOKEN: %s\n",tok)
    strncpy ( &parameters[command_counter][0],tok, max_parameter_length );
    //printf("PARAMETER: %s\n",&command_parameters[command_counter][0]);
    command_counter++;
    tok=strtok(NULL,separators);    
  }
  command_parameter_counter=command_counter;
  return(command_counter);  
}

void make_command_parameters_uppercase(char parameters[max_parameter_limit][max_parameter_length])
{
  for(int i=0; i< max_parameter_limit; i++)
  {
    for(int j=0; j<strlen(&parameters[i][0]); j++)
    {   
      parameters[i][j]=toupper(parameters[i][j]);
    }
  }
}


#endif //#ifndef EE3420_MEGA_H
