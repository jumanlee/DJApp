#include <JuceHeader.h>
#include "SaveList.h"


//==============================================================================
SaveList::SaveList()
{
    std::ifstream csvFile{"textFile.csv"};
    
    //if ifstream is unable to access the csv, then just print out the placeholder text.
    if(!csvFile.good())
    {
        for(int i = 0; i < 9; i++)
        {
            savedList.push_back("Empty track: Drag files here to load your songs");
        }
    }
}

SaveList::~SaveList()
{
}

//method to save the music files on the playlist onto a csv file.
void SaveList::save(const std::vector<File> &fileArray)
{
    std::vector<std::string> convertedFileArray;
    
    //convert all file names to std::string
    for(int i = 0; i < fileArray.size(); i++)
    {
        convertedFileArray.push_back(fileArray[i].getFullPathName().toStdString());
        
        std::cout << convertedFileArray[i] <<std::endl;
    }
    
    std::ofstream textFile;
    
    //create the csv file
    textFile.open("textFile.csv");
    
    //convert and output all the files into the csv file.
    for(int i = 0; i < convertedFileArray.size(); i++)
    {
        textFile <<convertedFileArray[i]<<std::endl;
    }
    
    //close the file
    textFile.close();
}

//method to load the saved list of music files from the csv file.
std::vector<File> SaveList::load()
{
    std::vector<File> entries;
    
    std::ifstream csvFile{"textFile.csv"};
    std::string line;
    
    if(csvFile.is_open())
    {
        while(std::getline(csvFile, line)) //gets a line in the csvFile and put it into the line variable
        {
            //this is to catch the bad line throw std::exception{};
            try
            {
                //push all the collected items in the csv file into the entires array
                entries.push_back(File{String(line)});
            }
            catch(const std::exception& e)
            {
                //if it's bad data, then print out the error message
                std::cout <<"CSVReader::readCSV bad data" << std::endl;
            }
        }
    }
    
    return entries;
}

//method to reset the saved list.
void SaveList::clearList()
{
    savedList.clear();
}

//boolean method to check if the csv file is empty or non-existent.
bool SaveList::isEmpty()
{
    //get the csv file named textFile.csv
    std::ifstream csvFile{"textFile.csv"};
    
    //make variable for a line
    std::string line;
    //get the first line in the csv file
    std::getline(csvFile, line);
    
    //if the csvFile does not exist in the folder OR if the csv file exists but the first line in the csv file is an empty string (this means the whole csv contains nothing) then return true because it is empty.
    
    if(!csvFile.good() || line.empty())
    {
        return true;
    }
    
    return false;
}
