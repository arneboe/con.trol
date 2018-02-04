#include "ErrorHandler.h"
#include <stdio.h>

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
	printf("error! %s:%d\n", file, line);
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
}
