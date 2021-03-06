// WaterLevel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

/* Input data */
const unsigned int INPUT_DATA[] = {1, 2, 3, 4, 5,6,5,4,3,2,1,2};


/************************************************************
Calculating of water volume
/param: array containing configuration of the borders
/param: number of array elements
/return: calculated volume of water
*************************************************************/
unsigned int  CalculateWaterVolume(const unsigned int prm_arr[],unsigned int prm_number)
{
  long lRetVal = 0;
  unsigned int * const pBorders = new unsigned int[prm_number](); // set all members to 0
  unsigned int  * const pFillLevels = new unsigned int [prm_number](); // set all members to 0
  unsigned int* pCurrentLeftBorder = pBorders;
  bool bPit = false;

  /* Show array members*/
  printf("Number of elements: %lu\n\n",prm_number);
    printf(" ******   Input data ******\n\n|");
  for (unsigned int i=0; i< prm_number; i++)
  {
    printf("%u|",prm_arr[i]);
  }
  printf("\n\n");

  --pCurrentLeftBorder; // Set current border pointer to the point before first member of the array (initial position)

  /* Walk through array from [1] to the end index*/
  for (unsigned int Idx = 1; Idx < prm_number; Idx++ )
  {
      // Check whether a pit begins at current position    
    if (prm_arr[Idx] < prm_arr[Idx-1])
    {
      bPit = true;
      pCurrentLeftBorder++; // Set pointer to the next free position in borders array
      *pCurrentLeftBorder = Idx-1; // and store in it the index of current left border 
    }
    else // Same level or higher
    {
      /* If we are inside a pit */
      if ( bPit == true)
      {
        unsigned int currentFillLevel = 0; // Fill level in the current pit
        bool bPostDecrementCurrentBorder = false; // Is posthandling of the current border index nessesary ?  
        unsigned int uNum = 0; // Amount of affected indexes to fill with the current fill level
        
       /* When the current border (right side of the pit) is greater than the current left border (left side of the pit)
         move the current left border until the current right border is less or equal to it.
         Then these two values ​define the boundaries of the current pit */
        while (prm_arr[Idx] > prm_arr[*pCurrentLeftBorder])
        {
          /* If the left index has reached the initial value, it means that the outer nested pit completed, 
          while loop will be aborted */
          if (  pCurrentLeftBorder == pBorders)
          {
            bPit = false;
            break;
          }
          /* Move to the next valid higher left border*/
          pCurrentLeftBorder--;
        } // end of while

        /* If after adjusting the current wall (right edge of the pit) is still higher than the corresponding left-hand border,
        set the filling level to the left border.
        */
        if (prm_arr[Idx] > prm_arr[*pCurrentLeftBorder])
        {
          currentFillLevel = prm_arr[*pCurrentLeftBorder];
        }
        /* If after adjusting the current wall (right edge of the pit) is still lower than the corresponding left-hand border,
        set the filling level to the right border.
        */
        else if (prm_arr[Idx] < prm_arr[*pCurrentLeftBorder])
        {
           currentFillLevel = prm_arr[Idx];
        }
        else
        {
          /* In case of equality of both boundaries set the filling level to the current wall height */
          currentFillLevel = prm_arr[Idx];
          /* If we are currently in the pit, left border is to move to the next valid higher value afterwards */
          if (true == bPit)
          {
            bPostDecrementCurrentBorder = true;
          }
        }

      /* If within this step the outer nested pit has been completed, left border has to move to the initial position afterwards */
        if (false == bPit)
        {
          bPostDecrementCurrentBorder = true;
        }

        /* Calculate the number of cells in the current pit, which have to be filled with water.*/
        uNum = Idx - *pCurrentLeftBorder - 1;
        /* Define first index of the filling area */
        unsigned int fillIndex = *pCurrentLeftBorder+1;

        /* Fill the current pit with calculated volume of water, taking into account the height of the borders*/
        while (uNum > 0)
        {
          unsigned int newFillLevel = currentFillLevel - prm_arr[fillIndex];
          pFillLevels[fillIndex] = newFillLevel;  
          fillIndex++;
          uNum--;
        }

        /* Adjust the pointer to the left edge of the next hole, if necessary*/
        if (true == bPostDecrementCurrentBorder)
        {
          pCurrentLeftBorder--;
        }
      }
    }
  }
 
  /* Info message with local filling levels of all pits 
   Calculate the total volume of water by adding the partial volumes.*/
  for (long unsigned i=0; i<prm_number; i++ )
  {
    printf("FillLevels[%lu] = %lu\n",i,pFillLevels[i]);
    lRetVal += pFillLevels[i];
  }

  delete [] pFillLevels;
  delete [] pBorders;
  return lRetVal;
}



int _tmain(int argc, _TCHAR* argv[])
{
 
  long unsigned numberOfElements = sizeof(INPUT_DATA)/sizeof(INPUT_DATA[0]);
  long unsigned WaterVolume = 0; 
  printf("Calculating of water volume\n"); 
  WaterVolume = CalculateWaterVolume(INPUT_DATA,numberOfElements);
  printf("\n\n******************************************************************\n"); 
  printf("\n Calculated water volume for given array: %lu\n",WaterVolume); 
  printf("\n******************************************************************\n"); 
  system("pause");
 	return 0;
}

