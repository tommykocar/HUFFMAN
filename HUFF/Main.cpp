





#include "HUFF.h"


int main(int argc,  char* argv[])
{
    HUFF* mytree = new HUFF();
    
    
    
    
    
    if (strcmp(argv[1], "-e") == 0)                     //Compares the value in position one are the argv array to the letter e
    {                                                   //If it does it puts back 0 as it is equal
        if (argc < 4)
        {
          string outputFile = "";                       // if the count is less then 4 there is now 4th file so we put it as null
          mytree->EncodeFile(argv[2], outputFile);      // calls encode function with output file

        }
       mytree->EncodeFile(argv[2], argv[3]);            // if the count is 4 then it calls the ecoded function with argv[3] instead of with output file
        

    }
    else if (strcmp(argv[1], "-d") == 0)                 //Compares the value in position one are the argv array to the letter d
    {    
        
        mytree->DecodeFile(argv[2], argv[3]);            //calls the Decoded function
             
    }
    else if (strcmp(argv[1], "-t") == 0)                   //Compares the value in position one are the argv array to the letter t
    {
        if (argc < 4)
        {
            string outputFile = "";                         // if the count is less then 4 there is no 4th file so we put it as null
            mytree->MakeTreeBuilder(argv[2], outputFile);   // calls the tree builder function with the output file made

        }
        mytree->MakeTreeBuilder(argv[2], argv[3]);          // calls the tree builder function with the output file specified in argv[3]
         
    }
    else if (strcmp(argv[1], "-et") == 0)                       //Compares the value in position one are the argv array to the letter et
    {
        if (argc < 5)
        {
            string outputFile = "";                                         // if the count is less then 5 there is now 4th file so we put it as null
            mytree->EncodeFileWithTree(argv[2], argv[3], outputFile);       // calls the encode file with tree function with the output file made

        }
        mytree->EncodeFileWithTree(argv[2], argv[3], argv[4]);              // calls the encode file with tree function with the file specified in argv[4]
       


    }
    else if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-help") || strcmp(argv[1], "-?"))           //Compares the value in position one are the argv array to the letter h/?/help
    {

        mytree->DisplayHelp();                                                                          //calls the help function
        
    }


    return 0;
}
