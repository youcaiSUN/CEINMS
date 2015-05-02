//__________________________________________________________________________
// Author(s): Elena Ceseracciu - August 2014
// email:  elena.ceseracciu@gmail.com
//
// DO NOT REDISTRIBUTE WITHOUT PERMISSION
//__________________________________________________________________________
//


#include "InputDataXmlReader.h"
#include <string>
using std::string;
#include <map>
using std::map;
#include <iostream>
using std::cout;
using std::endl;
#include "inputData-schema.hxx"

#include "validation.h"
#include "FileUtils.h"

InputDataXmlReader::InputDataXmlReader(const string& filename)
{

    try {
        std::auto_ptr<InputDataType> inputDataPointer(parseAndValidate<InputDataType>(filename, inputData_schema, sizeof(inputData_schema)));
        inputDataPointer_ = inputDataPointer;
    }
    catch (const xml_schema::exception& e) {
        cout << e << endl;
        exit(EXIT_FAILURE);
    }

    // save the location of input filename
    string::size_type dirSep = filename.rfind('/'); // Unix/Mac dir separator

    if (dirSep == string::npos)
        dirSep = filename.rfind('\\'); // DOS dir separator

    if (dirSep != string::npos) // if '_fileName' contains path information...
        filepath_ = filename.substr(0, dirSep + 1); // include trailing slashes

    readXml();
}

void InputDataXmlReader::readXml() {

    try {
        std::string& myLmtFile(*(inputDataPointer_->muscleTendonLengthFile().begin()));
        std::string& myEmgFile(*(inputDataPointer_->excitationsFile().begin()));
       //TODO std::string& myMomentArmDir(*(inputDataPointer_->momentArmsFDirectory().begin()));
        if (inputDataPointer_->externalTorquesFile().size() >0 )
            std::string& myExtTorqueFile(*(inputDataPointer_->externalTorquesFile().begin()));
    }
    catch (const xml_schema::exception& e) {
        cout << e << endl;
        exit(EXIT_FAILURE);
    }
}

std::string InputDataXmlReader::getLmtFile()
{
    std::string fileName(*(inputDataPointer_->muscleTendonLengthFile().begin()));
    FileUtils::trim(fileName);
    if (FileUtils::isAbsolute(fileName.c_str()))
            return fileName;
        else
            return filepath_ + fileName;
}

std::map<std::string, std::string> InputDataXmlReader::getMaFiles()
{
    map<string, string> result;
    for (auto it : inputDataPointer_->momentArmsFiles().begin()->momentArmsFile())
    {
        std::string pathToMaFile = it.c_str();
        FileUtils::trim(pathToMaFile);
        if (!FileUtils::isAbsolute(pathToMaFile.c_str()))
            pathToMaFile = filepath_ + pathToMaFile;
        result.insert(std::pair<string, string>(it.dofName(), pathToMaFile));
    }
    // return filepath_ + *(inputDataPointer_->momentArmsDirectory().begin());
    return result;
}

std::string InputDataXmlReader::getExcitationsFile()
{
    std::string fileName(*(inputDataPointer_->excitationsFile().begin()));
    FileUtils::trim(fileName);
    if (FileUtils::isAbsolute(fileName.c_str()))
        return fileName;
    else
        return filepath_ + fileName;
}

std::string InputDataXmlReader::getExternalTorqueFile()
{
    if (inputDataPointer_->externalTorquesFile().size() < 1)
        return "";
    std::string fileName(*(inputDataPointer_->externalTorquesFile().begin()));
    FileUtils::trim(fileName);
    if (FileUtils::isAbsolute(fileName.c_str()))
        return fileName;
    else
        return filepath_ + fileName;
}

