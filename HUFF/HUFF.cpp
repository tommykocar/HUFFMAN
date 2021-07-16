




#include "HUFF.h"



HUFF::HUFF()
{

	tree = NULL;

}


void HUFF::EncodeFileWithTree(string inputFile, string TreeFile, string outputFile)
{
    // This Method will encode any file with the name of the inputfile/file you want encoded, the tree builder file you want to use
    // to encode the file with, and the name of the output file name of the file you wanted created and written to. It will read in the
    // builder file and create the tree. Then we would go get character by character and get their paths and create the new bytes to encode
    // the file with. Then we would write the new encoded bytes to the new/output file to be the encoded version of the input file


    clock_t timeRecorded = clock();                     // starts the clock to determine how long the function ran for

    ifstream dataFile(inputFile, ios::binary | ios::in);    
    ifstream builderFile(TreeFile, ios::binary | ios::in);

    // This goes when the output file is null, it searchs for a period in the inputfile and 
    // if so then it goes through and deletes the extension off of it. If no period and just a name then 
    // it doesn't have an extension so it adds huf to the end of it and changes the output file from null to the input file name
    if (outputFile == "")
    {
        unsigned int fileSize = inputFile.size();                    //If output file is null find the size of the input file
        for (unsigned int i = 0; i < fileSize; i++)                  
        {
            if (inputFile.at(i) == '.')                                 // Goes through each chacacter of the input name and looks for a period
            {
                inputFile.erase(inputFile.size() - (fileSize - (unsigned long) i));     // if it finds a period it erases the extension
                break;
            }

        }
        inputFile += ".huf";                                          //adds extenstion htree onto the inputfile name
        outputFile = inputFile;                                         //makes the output file name input file name

    }
    ofstream encodedWithBuilderFile(outputFile, ios::binary | ios::out);

    node* Min1, * Min2;
    unsigned int subscriptNode1 = 0, subscriptNode2 = 0, treeIterations = 0, asciiNumber = 0, countByteSize = 0, bitHolder[8];
    unsigned char data;
    node* Nodes[256];
    string letterPath = "", pathsArray[256];



    //This make an array of all 256 char values as nodes
    for (unsigned int count = 0; count < 256; count++)
    {
        node* newNode = new node;                       //creates new node
        data = count;                                   // data become the char with the int value of count
        newNode->letter = data;                         // puts the new node letter as data
        Nodes[count] = newNode;                         // puts new node into the node array
    }

    // this while loop goes until it hits then end of the file
    // so it will go and get the next character as long as there
    // is a character to get. It then finds it in the array and 
    // increases its weight values so we know how many times it has occured. 
    // Once we hit the end of the file it exits
    while (treeIterations != 255)
    {
        subscriptNode1 = builderFile.get();                                  // the 5 lines get two bytes at a time and then get there two nodes
        Min1 = Nodes[subscriptNode1];                                       // from the array and make a new node with these two nodes as the
        subscriptNode2 = builderFile.get();                                // children. 
        Min2 = Nodes[subscriptNode2];
        node* newNode = new node;

        if (subscriptNode1 < subscriptNode2)                        // if subscript of node 1 is smalled the node's 2 subscript do these lines
        {
            newNode->LChildPointer = Min1;                          // these 4 lines put the nodes into the children and puts
            newNode->RChildPointer = Min2;                          // the new node/subetree back into the smalles subscript spot
            Nodes[subscriptNode1] = newNode;                        // along with making the bigger subscript spot NULL
            Nodes[subscriptNode2] = NULL;
        }
        else
        {
            newNode->LChildPointer = Min2;                          // these 4 lines put the nodes into the children and puts                         
            newNode->RChildPointer = Min1;                          // the new node/subetree back into the smalles subscript spot
            Nodes[subscriptNode2] = newNode;                        // along with making the bigger subscript spot NULL
            Nodes[subscriptNode1] = NULL;
        }

        encodedWithBuilderFile << (unsigned char)subscriptNode1 << (unsigned char)subscriptNode2;           // writes subscriptnode1 and subscriptnode2 to the outputfile

        treeIterations++;
    }

    tree = Nodes[0];            // sets tree as our tree we made at nodes[0] in our node array

    


    dataFile.seekg(0, ios::beg);            // seeks to the beginning of the file

    PathFinder(tree, letterPath, pathsArray);           // finds the paths to all the leaf nodes in the tree

    // This goes through each character and finds their path and then 
    // create bytes with those paths and write them to the file with
    // the created bytes from those characters paths

    while (dataFile.peek() != -1)       // does this function as long as there is a character to get to encode
    {

        data = dataFile.get();                       // gets character
        asciiNumber = data;                         // asciinumber becomes the int value of the char value in data
        letterPath = pathsArray[asciiNumber];       // finds the letter path of that char in the pathsarray
        asciiNumber = 0;                            // sets ascii to 0


        while (asciiNumber != letterPath.size())
        {
            bitHolder[countByteSize] = letterPath.at(asciiNumber);              // goes character by character through the letter path
            countByteSize++;            // adds 1 to byte size
            asciiNumber++;              // adds 1 to ascii

            if (countByteSize == 8)                 // Runs when byte size is 8, which means it has 8 bits which is the size of 1 byte
            {
                data = NULL;
                for (unsigned int j = 0; j <= 7; j++)
                {

                    if (bitHolder[j] == 49)
                    {
                        data |= (1 << (7 - j));         // adds up the binary total and stores the character in data of the binary int value
                    }


                }


                encodedWithBuilderFile << data;         // writes the char to the outputfile
                countByteSize = 0;              // puts countbytesize to 0
                
            }


        }

    }
    // as long as the byte needs padding bits this code is run
    // if the byte is 0 that means there is no bits in the byte 
    // so there is no need for padding as there is no byte needing more bits
    // if needed padding bytes it finds the longest path and adds the needed number of padding bits

    if (countByteSize != 0)
    {
        letterPath = "";
        for (unsigned int i = 0; i < 256; i++)
        {
            if (letterPath == "")
            {
                letterPath = pathsArray[i];                         // puts path in patharray[i] in letter path
            }
            else if (letterPath.size() < pathsArray[i].size())
            {
                letterPath = pathsArray[i];                 // puts new path in letter path as it is bigger then letter path original path
            }
        }

        asciiNumber = 0;

        data = NULL;
        while (countByteSize != 8)
        {
            bitHolder[countByteSize] = letterPath.at(asciiNumber);      // goes character by character through the letter path
            countByteSize++;
            asciiNumber++;
            if (countByteSize == 8)
            {
                for (unsigned int j = 0; j <= 7; j++)
                {

                    if (bitHolder[j] == 49)
                    {
                        data |= (1 << (7 - j));         // adds up the binary total and stores the character in data of the binary int value
                    }


                }


                encodedWithBuilderFile << data;         // writes last byte to file
            }
        }
    }

    timeRecorded = clock() - timeRecorded;     // stops clock and finds the difference from the beginning as the is the time it took to execute the function
           
    dataFile.seekg(0, ios::end);        // seeks the end of the file

    cout.precision(3);          // sets fixed valued to cout
    cout << fixed << "Time: " << (float) timeRecorded / CLOCKS_PER_SEC << " Seconds. " << dataFile.tellg() << " bytes in / " << encodedWithBuilderFile.tellp() << " bytes out";         // This sets the fixed precision to the timeRecorded and prints out the timeRecorded and the bytes read in and out

    dataFile.close();               // closes file
    builderFile.close();            // closes file
    encodedWithBuilderFile.close();      // closes file

}


void HUFF::EncodeFile(string inputFile, string OutputFile)
{
    //This method goes through and takes the inputed file/file wanted to be encoded and writes it to the output file. 
    //It would take the file and go character by character and add 1 to the weight value of each node that matched the letter
    //it got from the file. Once it hit the eof then it would go get their paths based on their spot in the tree and the putting the inputed file
    //back to the begging. Then again we would go get character by character and create the new bytes based of the character's paths. Then we would
    //take those new bytes and write the to the new/output file.

    clock_t timeRecorded = clock();             // starts clock

    ifstream originalFile(inputFile, ios::binary | ios::in);

    // This goes when the output file is null, it searchs for a period in the inputfile and 
    // if so then it goes through and deletes the extension off of it. If no period and just a name then 
    // it doesn't have an extension so it adds huf to the end of it and changes the output file from null to the input file name
    if (OutputFile == "")
    {
        unsigned int fileSize = inputFile.size();                    //If output file is null find the size of the input file
        for (unsigned int i = 0; i < fileSize; i++)
        {
            if (inputFile.at(i) == '.')                                 // Goes through each chacacter of the input name and looks for a period
            {
                inputFile.erase(inputFile.size() - (fileSize - (unsigned long) i));     // if it finds a period it erases the extension
                break;
            }

        }
        inputFile += ".huf";                                          //adds extenstion htree onto the inputfile name
        OutputFile = inputFile;                                         //makes the output file name input file name

    }
    ofstream encodedFile(OutputFile, ios::binary | ios::out);
   


    node* Min1, * Min2, * nodeHolder;
    unsigned int subscriptNode1 = 0, subscriptNode2 = 0, minCount, treeIterations = 0, subscriptHolder, asciiNumber=0, countByteSize=0, bitHolder[8];
    unsigned char data;
    node* Nodes[256];
    string letterPath = "", pathsArray[256];

   
    //This make an array of all 256 char values as nodes
    for (unsigned int count = 0; count < 256; count++)
    {
        node* newNode = new node;               // creates new node
        data = count;                           // puts data as the char that is the letter of count
        newNode->letter = data;                 // makes the letter of the new node the letter of data
        Nodes[count] = newNode;                 // adds new node to the node array
    }

    // this while loop goes until it hits then end of the file
    // so it will go and get the next character as long as there
    // is a character to get. It then finds it in the array and 
    // increases its weight values so we know how many times it has occured. 
    // Once we hit the end of the file it exits
   
    while (originalFile.peek() != -1)
    {
        data = originalFile.get();                      // gets one letter at a time fromt he file until hitting EOF


        for (unsigned int count1 = 0; count1 < 256; count1++)
        {
            if (Nodes[count1]->letter == data)
            {
                Nodes[count1]->weight++;                // finds node in the node array with the same letter and adds one to the weight

                break;
            }

        }
    }


    // This goes through and finds the two smallest values based of their
    // weights first, and then if they have the same weight we base it off
    // their subscript value. If your subscript value is first then you technically
    // hold the lesser value you are smaller. Once we find our two smallest values
    // we then create a subtree with those two nodes as the children. Left one
    // is always the smallest subscript value no matter the weight. It then puts that
    // subtree back into the smallest subscript spot in the array and changes the largest one 
    // to null. We also put the subscript values in an array to be our first 510 bytes 
    // so the reciever can build the tree. 

    while (treeIterations != 255)
    {
        Min1 = NULL;
        Min2 = NULL;
        for (minCount = 0; minCount < 256; minCount++)
        {
            if (Nodes[minCount] == NULL)
            {
                continue;
            }
            else if (Min1 == NULL)
            {
                Min1 = Nodes[minCount];                         // if min1 is null adds node at min count to min1 and the number of mincount to subscriptnode1
                subscriptNode1 = minCount;
            }
            else if (Min2 == NULL)
            {
                Min2 = Nodes[minCount];                         // // if min2 is null adds node at min count to min2 and the number of mincount to subscriptnode2
                subscriptNode2 = minCount;
            }
            else if (Nodes[minCount]->weight < Min1->weight)
            {
                if (Min1->weight < Min2->weight || Min1->weight == Min2->weight && subscriptNode1 < subscriptNode2)
                {
                    Min2 = Min1;                            // if min1 is less then min2 the make min2 the node at min1
                    subscriptNode2 = subscriptNode1;        // make subscript of node 2 the value at subscirptnode1
                }
                Min1 = Nodes[minCount];                     //adds node at min count to min1and the number of mincount to subscriptnode1
                subscriptNode1 = minCount;

            }
            else if (Nodes[minCount]->weight < Min2->weight)
            {
                Min2 = Nodes[minCount];                 //adds node at min count to min2 and the number of mincount to subscriptnode2
                subscriptNode2 = minCount;

            }


        }
        if (subscriptNode2 < subscriptNode1)
        {
            nodeHolder = Min1;                  // if subscript2 is smaller than subscript1 the nodeholder is value at min1
            Min1 = Min2;                        // min1 value becomes value at min2
            Min2 = nodeHolder;                  // min2 values becomes the value at nodeholder
            subscriptHolder = subscriptNode1;   // subscriptholder value is value at min1
            subscriptNode1 = subscriptNode2;    // subscriptnode1 becomes value at subscriptnode2
            subscriptNode2 = subscriptHolder;   // subscriptnode2 becomes value at subscriptnode1

        }
        
        encodedFile << (unsigned char)subscriptNode1 << (unsigned char)subscriptNode2;          // writes the subscripts of node 1 and node 2 to the output file
        treeIterations++;       // adds 1 to tree iterations

        node* newNode = new node;                           // creates new node

        newNode->weight = Min1->weight + Min2->weight;      // adds weight of min1 and min2 and puts the weight in the new node

        newNode->LChildPointer = Min1;                      // makes min1 left child
        newNode->RChildPointer = Min2;                      // makes min2 right child
        Nodes[subscriptNode1] = newNode;                    // puts new node in array where min1 was at
        Nodes[subscriptNode2] = NULL;                       // puts null where min2 was at in the node array

    }

    tree = Nodes[0];                                        // sets tree as the tree made in node[0] of the node array

    originalFile.seekg(0, ios::beg);                        // seeks to the beginning of the inputfile

    PathFinder(tree, letterPath, pathsArray);               // calls path finder function where it finds the string paths of each node in the tree

    // This goes through each character and finds their path and then 
    // create bytes with those paths and write them to the file with
    // the created bytes from those characters paths

    while (originalFile.peek() != -1)
    {
        
        data = originalFile.get();              // goes char by char through the input file until you hit EOF
        asciiNumber = data;                     // makes asiinumber the int from the char data
        letterPath = pathsArray[asciiNumber];   // finds the path of that char value in the paths array
        asciiNumber = 0;                        // sets asciinumber = 0
        
        
        while (asciiNumber != letterPath.size())
        {
            bitHolder[countByteSize] = letterPath.at(asciiNumber);      // goes through the path character by character 
            countByteSize++;                                            // adds 1 to countbytesize
            asciiNumber++;                                              // adds 1 to asciiNumber

            if (countByteSize == 8)                               // if countbytesize equals 8, it means there is 8 bits in the array so it has enough for a byte now
            {
                data = NULL;
                for (unsigned int j = 0; j <= 7; j++)                      // goes through the array one by one and if it equals 1 it adds to it based off its position
                {                                                 // so if its the first bit and it was 1 then we would add 128 to data, if the second to last bit we would add 2
                    
                    if (bitHolder[j] == 49)
                    {
                        data |= (1 << (7 - j));                   // adds up the binary total and stores the character in data of the binary int value
                    }

                   
                }

                
                encodedFile << data;                                // writes that character to the output file
                countByteSize = 0;                                // returns the byte size to zero as it just wrote the byte to the file
                
            }
            

        }
        
    }
    
    // as long as the byte needs padding bits this code is run
     // if the byte is 0 that means there is no bits in the byte 
     // so there is no need for padding as there is no byte needing more bits
     // if needed padding bytes it finds the longest path and adds the needed number of padding bits

    if (countByteSize != 0)
    {
        letterPath = "";
        for (unsigned int i = 0; i < 256; i++)
        {
            if (letterPath == "")
            {
                letterPath = pathsArray[i];                     // stores the first path into letterpath since it is null
            }
            else if (letterPath.size() < pathsArray[i].size())
            {
                letterPath = pathsArray[i];                     // stores new path in letter path as it is bigger in size
            }
        }

        asciiNumber = 0;

        data = NULL;
        while (countByteSize != 8)
        {
            bitHolder[countByteSize] = letterPath.at(asciiNumber);           // goes through the path character by character of the path to add padding bits if need to the last bit
            countByteSize++;
            asciiNumber++;
            if (countByteSize == 8)
            {
                for (unsigned int j = 0; j <= 7; j++)
                {

                    if (bitHolder[j] == 49)
                    {
                        data |= (1 << (7 - j));       // adds up the binary total and stores the character in data of the binary int value          
                    }


                }

                encodedFile << data;                 // writes it to the output file
            }
        }
    }

    timeRecorded = clock() - timeRecorded;             // stops clock and finds the difference from the start time as that is the time it took to execute
    originalFile.seekg(0, ios::end);        //seeks to the end of file
    
    cout.precision(3);          // set fixed value to cout
    cout << fixed << "Time: " <<   (float) timeRecorded / CLOCKS_PER_SEC << " Seconds. " << originalFile.tellg() << " bytes in / " << encodedFile.tellp() << " bytes out";       // This sets the fixed precision to the timeRecorded and prints out the timeRecorded and the bytes read in and out


    originalFile.close();       // both files are being closed
    encodedFile.close();

}


void HUFF::DecodeFile(string inputFile, string OutputFile)
{
    //This method goes and takes in the input file and decodes it and writes the new bytes that it decodes to the outputfile.
    //It takes in the encoded file and takes the first 510 bytes and creates the tree based off the first 510 bytes. Then it 
    //went and got the encoded character one by one till eof. It would take the character and take its int value and if it was a
    //1 then it would go down toward the right child and if it was a 0 then it would go toward the left child and it would keep doing
    //this based off their binary value and it would stop either at the end of the byte or even in the middle once it got to a leaf node.
    //It would take that letter and write it to the file and then keep doing this until eof. Finally the file would be decoded

    clock_t timeRecorded = clock();

    ifstream eFile(inputFile, ios::binary | ios::in);
    
    node* Min1, * Min2;
    unsigned int subscriptNode1 = 0, subscriptNode2 = 0, treeCount = 0, asciiNumber = 0;
    unsigned char data;
    node* Nodes[256];
    string bytePath = "", byteReversed;



    //This make an array of all 256 char values as nodes
    for (unsigned int count = 0; count < 256; count++)
    {
        node* newNode = new node;                                           // creates new node 
        data = count;                                                       // creates the char value from the int value
        newNode->letter = data;                                             // puts the letter to the char value found above
        Nodes[count] = newNode;                                             // puts the node in the node array
    }

    // this while loop goes until it hits then end of the file
    // so it will go and get the next two characters as long as there
    // is characters to get. It then goes through and makes subtrees out of those
    // two characters. Once it reads in the first 510 bytes then it stops and has
    // made the whole tree with all the letters

    while(treeCount != 255)
    {
        subscriptNode1= eFile.get();                                // the 5 lines get two bytes at a time and then get there two nodes
        Min1 = Nodes[subscriptNode1];                               // from the array and make a new node with these two nodes as the
        subscriptNode2= eFile.get();                                // children. 
        Min2 = Nodes[subscriptNode2];
        node* newNode = new node;
        
        if (subscriptNode1 < subscriptNode2)                        // if subscript of node 1 is smalled the node's 2 subscript do these lines
        {
            newNode->LChildPointer = Min1;                          // these 4 lines put the nodes into the children and puts
            newNode->RChildPointer = Min2;                          // the new node/subetree back into the smalles subscript spot
            Nodes[subscriptNode1] = newNode;                        // along with making the bigger subscript spot NULL
            Nodes[subscriptNode2] = NULL;
        }
        else
        {
            newNode->LChildPointer = Min2;                          // these 4 lines put the nodes into the children and puts                         
            newNode->RChildPointer = Min1;                          // the new node/subetree back into the smalles subscript spot
            Nodes[subscriptNode2] = newNode;                        // along with making the bigger subscript spot NULL
            Nodes[subscriptNode1] = NULL;
        }
     

        treeCount++;
    }

    tree = Nodes[0];                                                         // sets the tree to the tree we made out of the Nodes array
    ofstream decodedFile(OutputFile, ios::binary | ios::out);

    // this takes in the encoded bytes and goes through one by one until it hits the end of the file
    // it divideds the value we get from the encoded file by 2 until you get to 0. If the value can be divided by
    // two with no remainders put a zero for a bit, if there is remainders put a 1 for a bit. That is the letter path
    // so we followe the bits one by one until we get to a letter. Once we get to a letter we go back to the start of
    // the tree and keep following the bits. It does this until there is no many bytes/bits left in the file. This is
    // where your file should be back to normal

    while (eFile.peek() != -1)
    {
        bytePath = "";
        asciiNumber = eFile.get();                      // gets byte's int value

        while (asciiNumber > 0)
        {
            
            if (asciiNumber % 2)
            {
                bytePath += "1";                    // if number has remainder then we input 1 into the string 
            }
            else
            {
                bytePath += "0";                    // if number has no remainders then input 0 into the string
            }
            asciiNumber = asciiNumber / 2;
        }

       
        while (bytePath.size() != 8)
        {
                bytePath += "0";                       // if length of string isn't 8 long add leading zeros till 8 bits long
        }
        
        byteReversed = "";

        for (unsigned int i = 0; i <= 7; i++)
        {
            byteReversed += bytePath.at(7 - (unsigned long) i);        // reverse the byte path as it is backwards bassed of LSB and MSB
        }

        for (unsigned int j = 0; j < 8; j++)
        {

            
            if (byteReversed.at(j) == '1')
            {
                tree = tree->RChildPointer;             // if bit value is 1 go down the right path
               
            }
            else
            {
                tree = tree->LChildPointer;             // if bit value is 0 go down the left path
                
            }

            if (tree->LChildPointer == NULL)
            {
                decodedFile << tree->letter;            // if the Lchild is NULL after going down the path the node is a leaf
                tree = Nodes[0];                        // so write the letter to the file and put the tree back to the top
            }
            
        }
        

    }

    timeRecorded = clock() - timeRecorded;              // stops clock and takes the difference from the start of the the clock as that is the time it took to execute
    eFile.seekg(0, ios::end);       // seeks to end of file

    cout.precision(3);              // sets the fixed value for the cout
    
    cout << fixed << "Time: " << (float) timeRecorded / CLOCKS_PER_SEC<< " Seconds. " << eFile.tellg() << " bytes in / " << decodedFile.tellp() << " bytes out";          // This sets the fixed precision to the timeRecorded and prints out the timeRecorded and the bytes read in and out

    decodedFile.close();            // close both files
    eFile.close();


}


void HUFF::MakeTreeBuilder(string inputFile, string OutputFile)
{
    // This method goes and takes in the inputed file and creates a tree and puts it into the outputfile so the
    // person I'm sending this too can use this to create the tree. It goes through the inputfile and goes character by character 
    // and adds 1 to weight of the node that matches the letter of the character we got. Once you reach the eof then we got through
    // the node array and make subtrees one by one based on the two smallest weights in the node array. This create the tree from the 
    // bottom up. We then write these two smallest nodes, in terms of weight, to the output file and this will give us the info to make
    // the tree ourselves if we didn't have the weights of the characters. This file will be 510 bytes each time as we right two at a time 
    // to write to the file so since there is 256 different characters it would take 256 times 2 and the subtract 1 as we are left with one node
    // which is the tree. Which would leave us 510 bytes.

    clock_t timeRecorded = clock();                      // starts the clock to determine how long the function ran for

    ifstream readInFile(inputFile, ios::binary | ios::in);

    // This goes when the output file is null, it searchs for a period in the inputfile and 
    // if so then it goes through and deletes the extension off of it. If no period and just a name then 
    // it doesn't have an extension so it adds htree to the end of it and changes the output file from null to the input file name
    if (OutputFile == "")
    {
        unsigned int fileSize = inputFile.size();                    //If output file is null find the size of the input file
        for (unsigned int i = 0; i < fileSize; i++)                  
        {
            if (inputFile.at(i) == '.')                                 // Goes through each chacacter of the input name and looks for a period
            {
                inputFile.erase(inputFile.size() - (fileSize - (unsigned long) i));     // if it finds a period it erases the extension
                break;
            }

        }
        inputFile += ".htree";                                          //adds extenstion htree onto the inputfile name
        OutputFile = inputFile;                                         //makes the output file name input file name

    }
    ofstream writeOutFile(OutputFile, ios::binary | ios::out);

    node* Min1, * Min2, *nodeHolder;
    unsigned int subscriptNode1 = 0, subscriptNode2 = 0, minCount, treeiteration = 0, subscriptHolder;
    unsigned char data;
    node* Nodes[256];
   
    


    //This make an array of all 256 char values as nodes
    for (unsigned int count = 0; count < 256; count++)
    {
        node* newNode = new node;                           // creates new node
        data = count;                                       // based off the int value finds the char value
        newNode->letter = data;                             // puts char letter in the letter spot of the new node
        Nodes[count] = newNode;                             // puts the new node in the node array
    }

    // this while loop goes until it hits then end of the file
    // so it will go and get the next character as long as there
    // is a character to get. It then finds it in the array and 
    // increases its weight values so we know how many times it has occured. 
    // Once we hit the end of the file it exits

    while (readInFile.peek() != -1)
    {
        data = readInFile.get();                      // gets the next byte in the file as long as there is one


        for (unsigned int count1 = 0; count1 < 256; count1++)
        {
            if (Nodes[count1]->letter == data)
            {
                Nodes[count1]->weight++;                // finds the node with the same letter/char and ups their weight by one

                break;
            }

        }
    }


    // This goes through and finds the two smallest values based on their
    // weights first, and then if they have the same weight we base it off
    // their subscript value. If your subscript value is first then you technically
    // hold the lesser value you are smaller. Once we find our two smallest values
    // we then create a subtree with those two nodes as the children. Left one
    // is always the smallest subscript value no matter the weight. It then puts that
    // subtree back into the smallest subscript spot in the array and changes the largest one 
    // to null. We also put the subscript values in an array to be our first 510 bytes 
    // so the reciever can build the tree. 

    while (treeiteration != 255)
    {
        Min1 = NULL;
        Min2 = NULL;
        for (minCount = 0; minCount < 256; minCount++)
        {
            if (Nodes[minCount] == NULL)
            {
                continue;                               // if node is NULL then skip loop
            }
            else if (Min1 == NULL)
            {
                Min1 = Nodes[minCount];                 // puts node into Min1 and subscript in subscript 1
                subscriptNode1 = minCount;
            }
            else if (Min2 == NULL)
            {
                Min2 = Nodes[minCount];                 // puts node into Min2 and subscript in subscript 2
                subscriptNode2 = minCount;
            }
            else if (Nodes[minCount]->weight < Min1->weight)
            {
                if (Min1->weight < Min2->weight || Min1->weight == Min2->weight && subscriptNode1 < subscriptNode2)
                {
                    Min2 = Min1;                                // min1 is smaller weight or has equal weight but is sooner in the array
                    subscriptNode2 = subscriptNode1;            // then min2 so it makes min1 go to Min2
                }
                
                Min1 = Nodes[minCount];                         // makes min1 the node it is on as it is lesser weight, as well as subscript node to the node it is on
                subscriptNode1 = minCount;

            }
            else if (Nodes[minCount]->weight < Min2->weight)
            {
                Min2 = Nodes[minCount];                     // makes node it is on min2 and subscript it is on to subscript2
                subscriptNode2 = minCount;

            }


        }

        //swithes nodes as Min2 comes first/sooner in the array out of the two min nodes

        if (subscriptNode2 < subscriptNode1)
        {
            nodeHolder = Min1;                          // holds node 1                         
            Min1 = Min2;                                // moves node 2 into node 1
            Min2 = nodeHolder;                          // moves nodeholder (node1 old value) into node 2
            subscriptHolder = subscriptNode1;           // holds subscript of node 1
            subscriptNode1 = subscriptNode2;            // moves subscript of node 2 to node 1 subscript
            subscriptNode2 = subscriptHolder;           // makes subscript holder (subscript1 old value) to subscript 2
            
        }
       
        writeOutFile << (unsigned char)subscriptNode1 << (unsigned char)subscriptNode2;           // writes the pair of subscripts to the output file (it always put the lower subscript first)
        
        treeiteration++;


        node* newNode = new node;                                           // creates new node

        newNode->weight = Min1->weight + Min2->weight;                      // adds weight of node 1 and node 2 an puts it in the new node
       
            newNode->LChildPointer = Min1;                                  // makes node 1 left child
            newNode->RChildPointer = Min2;                                  // makes node 2 right child
            Nodes[subscriptNode1] = newNode;                                // puts new node into space of array that node 1 was at
            Nodes[subscriptNode2] = NULL;                                   // makes spot of node 2 in node array to null
        



    }

    timeRecorded = clock() - timeRecorded;                  // stops the clock and takes the difference from the start as that is the time it took to execute
   

    readInFile.seekg(0, ios::end);                        // seeks to the end of a file

    cout.precision(3);                                  // Sets fixed value for cout
    cout << fixed << "Time: " << (float) timeRecorded / CLOCKS_PER_SEC << " Seconds. " << readInFile.tellg() << " bytes in / " << writeOutFile.tellp() << " bytes out";      // This sets the fixed precision to the timeRecorded and prints out the timeRecorded and the bytes read in and out
    
    readInFile.close();                                   // closes the file
    
    writeOutFile.close();                                     // closes the file
   
}


void HUFF::DisplayHelp()
{
    //Displays the functions they are able to call and what they do

    printf( "To Make a tree builder file: Syntax for a tree file is: 'HUFF -t File1 [File2]'\n"
        "HUFF is the name of executable and -t is for the tree builder function. File1 and File2 are there to\n"
        "represent your files. File2 is optional as it is in brackets, if file1 was 'Shakespeare.txt' then\n"
        "we would take .txt off the end of it and make a tree File called 'Shakespeare.htree'. They don't even have\n"
        "to have an extension at the end and it will take 'Shakespeare' to 'Shakespeare.htree'. File1 represents\n"
        "any file you want from a txt file to a bin file, but file2 is the name your giving to the builder file\n"
        "your creating.\n\n"

        "To Make an encoded file: Syntax encode a file is: 'HUFF -e File1 [File2]'\n"
        "HUFF is the name of executable and -e is for the encode function. File1 and File2 are there to\n"
        "represent your files. File2 is optional as it is in brackets, if file1 was 'Shakespeare.txt' then\n"
        "we would take .txt off the end of it and make a File called 'Shakespeare.huf'. They don't even have\n"
        "to have an extension at the end and it will take 'Shakespeare' to 'Shakespeare.huf'. File1 represents\n"
        "any file you want from a txt file to a bin file, but file2 is the name your giving to the builder file\n"
        "your creating.\n\n"

        "To Make a Decoded file: Syntax to decode a file is: 'HUFF -d File1 File2'\n"
        "HUFF is the name of executable and -d is for the decode function. File1 and File2 are there to\n"
        "represent your files. You can tell that file2 in this syntax is not in brackets so it is not optional. File1 represents\n"
        "any file you want from a txt file to a bin file, but file2 is the name your giving to the builder file\n"
        "your creating.\n\n"

        "To Make an encoded file with tree builder File: Syntax to encode a file with a builder file is: 'HUFF -e File1 File2 [File3]'\n"
        "HUFF is the name of executable and -et is for the encode with tree builder function. File1, File2, and File3 are there to\n"
        "represent your files. File3 is optional as it is in brackets, if file1 was 'Shakespeare.txt' and File2 was 'Shakespeare.htree' then\n"
        "we would take .txt off the end of File1 and make a File called 'Shakespeare.huf'. They don't even have to have an extension at the\n"
        "end and it will take 'Shakespeare' to 'Shakespeare.huf'. File1 represents any file you want from a txt file to a bin file, file2 is the\n"
        "tree builder file telling your how to make the tree, and file3 is the name your giving to the builder file your creating.");
    
    
    

}

void HUFF::PathFinder(node* TreeNode, string paths, string arrayOfPaths[256])
{
    // This method takes in the start of the tree, a null string path and an array of the number of characters
    // that will have a string path to them. It recursively goes through the tree and creates string paths for
    // each character. It would go through the tree and based on the node they were at it would go down the left
    // path and add 0 to the string path if the left child wasn't null. It would go down the right path and add 1
    // to the string path if the right child wasn't null. If the left and right child was null then it was a leaf,
    // and would store the string path value for the letter in the array of paths based off what it was at when it 
    // got to that node. 

    int value = 0;

    
        if (TreeNode->LChildPointer == NULL && TreeNode->RChildPointer == NULL)
        {
            value = TreeNode->letter;          // this is a leaf so it finds its chars int value
            arrayOfPaths[value] = paths;       // puts the path in the array, based on the chars value so its value is 1 then the path of that char is put into spot 1
        }
        else
        {
            paths = paths + "0";                                            // adds 0 to path
            PathFinder(TreeNode->LChildPointer, paths, arrayOfPaths);       // calls this method with the new path and the left child as the new tree node
            paths.erase(paths.size() - 1);                                  // erases the 0 from the size as it came back to its orginal node/path


            paths = paths + "1";                                            // adds 1 to path
            PathFinder(TreeNode->RChildPointer, paths, arrayOfPaths);       // calls this method with the new path and the right child as the new tree node
            paths.erase(paths.size() - 1);                                  // erases the 1 from the size as it came back to its orginal node/path
        }
   
}


