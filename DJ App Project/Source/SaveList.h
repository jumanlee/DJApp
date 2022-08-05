
#pragma once

#include <JuceHeader.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
using namespace juce;

class SaveList
{
    //please see .cpp file for all the relevant comments for each method
    public:
        SaveList();
        ~SaveList();
        
        void save(const std::vector<File> &fileArray);
        std::vector<File> load();
        void clearList();
        bool isEmpty();

    private:
        
        std::vector<std::string> savedList;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SaveList)
};
