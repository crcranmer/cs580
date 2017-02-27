//Corey Cranmer, E725U784, Assignment #1
//File: parts1.c
//This program will keep track of parts for a manufacturer. It will read part data from a file and save each part record to a structure. An array of structures will be used to store all the records.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parts1.h"
#include "utils.h"

void rtrim(char s[]) //function to get rid of any extra space in strings
{
  char *end = s + strlen(s) - 1; //pointer to the end of string
  while (isspace(*end))
  {
    *end = '\0';
    end--;
  }
}

Part read_part_record(FILE *fp) //function to read data from part_data.txt and save in structure array
{
  Part data; //where we store data
  char nl; //for getting rid of newline

  fscanf(fp, "%17c", data.pname);
  data.pname[17] = '\0';
  rtrim(data.pname);

  fscanf(fp, "%8c", data.pnumber);
  data.pnumber[8] = '\0';
  rtrim(data.pnumber);
  
  fscanf(fp, "%6lf", &data.weight);
  //data.weight[7] = '\0';
  //rtrim(data.weight);
  fscanf(fp, "%c", &nl); //ignores empty space after weight

  fscanf(fp, "%5c", data.supplier1);
  data.supplier1[5] = '\0';
  rtrim(data.supplier1);

  fscanf(fp, "%5c", data.supplier2);
  data.supplier2[5] = '\0';
  rtrim(data.supplier2);

  fscanf(fp, "%d", &data.in_stock);
  fscanf(fp, "%c", &nl);                        /* get and ignore the newline */

  return data;
}

void file_output(Part output_part, FILE *output_fp) //prints info to part_matches.txt based on user's input
{
  fprintf(output_fp, "%s %s, %.2lf N, %d in stock\n", output_part.pname, output_part.pnumber, output_part.weight, output_part.in_stock);
}

//takes the user's selected weight and compares it to weights in text file based on how the user wishes to compare
void find_weight(Part data_list[100], int records, double weight, int choice)
{
  int i; //loop purposes
  FILE *out_fp; //file to output info to screen or to file
  double tempWeight; //used to compare user's weight and weights in part_data.txt

  out_fp = fopen("part_matches.txt", "w");

  if(out_fp == NULL) //Check to see if output file was opened, if not, alert user and close program
  {
    printf("\nCould not make a file to write to.\n");
    exit(0);
  }
  else
  {
    for(i = 0; i < records-1; i++) //Compare part weight to user selected weight and sends part info that satisfy comparison type
    {
      tempWeight = data_list[i].weight;

      if (choice == 1) //parts with weights LESS THAN entered weight
      {
        if (tempWeight < weight)
        {
          file_output(data_list[i], out_fp); //send results to part_matches.txt
        }
      }
      else if (choice == 2) //weights EQUAL TO entered weight
      {
        if(tempWeight == weight)
        {
          file_output(data_list[i], out_fp); //send results to part_matches.txt
        }
      }
      else if (choice == 3) //weights GREATER THAN entered weight
      {
        if (tempWeight > weight)
        {
          file_output(data_list[i], out_fp); //send results to part_matches.txt
        }
      }
      else
      {
	printf("Although you entered a number, it was not a valid comparison choice. Please try again.\n");
	exit(0);
      }
    }
  }

  fclose(out_fp);
  printf("\nA list of parts matching your selection has been saved in the file parts_matches.txt.\n");   //Tell user list was successfully created
}

int main(void)
{
  FILE *in_fp; //variable used to read file

  Part data_list[100]; //array that will store list of part data
  int num_records = 0; //loop purposes
  double weight_choice; //user inputed weight for comparison
  int compare_choice; //the way the user wishes to compare weights
  
  in_fp = fopen("part_data.txt", "r");

  if (in_fp == NULL)
  {
    printf("Could not find file.\n");
    return 0;
  }
  else
  {
    while (!feof(in_fp))
    {
      data_list[num_records] = read_part_record(in_fp);
      num_records++;
    }
    
    fclose(in_fp);
  }
  
  printf("This program will find parts in the inventory based on a weight. You will have a choice to get parts whose weight is less than, greater than, or equal to the weight you enter.\n");
  weight_choice = get_double("\nEnter the weight to use for comparing with the parts' weights: ");
 
  printf("Comparison type:\n   1 - Find parts with weights LESS THAN your entered weight\n   2 - Find parts with weights EQUAL TO your entered weight\n   3 - Find parts with weights GREATHER THAN your entered weight\n");
  compare_choice = get_int("\nEnter the comparison type to use: ");
  
  find_weight(data_list, num_records, weight_choice, compare_choice);
  
  return 0;
}
