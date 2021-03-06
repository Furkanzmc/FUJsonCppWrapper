#include "JsonCppWrapper.h"
namespace zmc
{
JsonCppWrapper::JsonCppWrapper(std::string filePath)
{
    loadJsonFile(filePath);
}

JsonCppWrapper::JsonCppWrapper()
{
}

JsonCppWrapper::~JsonCppWrapper()
{
}

void JsonCppWrapper::writeStyledJsonFile(Json::Value &jsonValueObject, std::string loadedFilePath)
{
    Json::StyledWriter writer;
    // Make a new JSON document for the configuration. Preserve original comments.
    std::string outputConfig = writer.write(jsonValueObject);
    std::ofstream ostream;
    ostream.open(loadedFilePath);
    ostream << outputConfig << std::endl;
    ostream.close();
}

bool JsonCppWrapper::doesObjectExist(std::string key, std::string rootObjectName)
{
    if (mLoadedJsonFilePath.size() < 1)
        return false;//No file is loaded
    bool exists = false;
    Json::Value &rootValue = rootObjectName.size() > 1 ? mRootValue[rootObjectName][key]
                                                         : mRootValue[key];
    if (rootObjectName.size() < 1) {
        if (mRootValue[key] != Json::nullValue)
            exists = true;
    } else {
        for (Json::Value &value : rootValue) {
            if (value.isObject()) {
                if (value.getMemberNames().at(0) == key) {
                    exists = true;
                    break;
                }
            }
        }
    }
    return exists;
}

bool JsonCppWrapper::loadJsonFile(std::string filePath)
{
    mLoadedJsonFilePath = "";
    Json::Reader reader;
    std::ifstream stream(filePath);
    if (stream.is_open()) {
        std::istream &istream = stream;
        bool parsingSuccessful = reader.parse(istream, mRootValue);
        if (!parsingSuccessful) {
            // report to the user the failure and their locations in the document.
            std::cout << "Failed to parse configuration\n" << reader.getFormatedErrorMessages() << std::endl;
            return false;
        }
        mLoadedJsonFilePath = filePath;
    }
    else if ((stream.rdstate() & std::ifstream::failbit) != 0) {
        std::cerr << "Error opening " << filePath << std::endl;
        return false;
    }
    return true;
}

bool JsonCppWrapper::removeObject(std::string key, std::string rootObjectName)
{
    if (mLoadedJsonFilePath.size() < 1)
        return false;//No file is loaded
    if (rootObjectName.size() > 1)
        mRootValue[rootObjectName].removeMember(key);
    else
        mRootValue.removeMember(key);
    writeStyledJsonFile(mRootValue, mLoadedJsonFilePath);
    return true;
}

Json::Value JsonCppWrapper::getObject(std::string key, std::string rootObjectName)
{
    Json::Value returnValue = Json::nullValue;
    if (rootObjectName.size() > 1) {
        if (mRootValue[rootObjectName][key].isArray())
            return Json::Value::null;
        returnValue = mRootValue[rootObjectName].get(key, Json::Value());
    } else {
        if (mRootValue[key].isArray())
            return Json::Value::null;
        returnValue = mRootValue.get(key, Json::Value());
    }
    return returnValue;
}

Json::Value JsonCppWrapper::getObjectInValue(std::string key, Json::Value *objValue, std::string rootObjectName)
{
    Json::Value returnValue = Json::nullValue;
    Json::Value &objectValue = *objValue;
    if (rootObjectName.size() > 1) {
        if (objectValue.isArray())
            return Json::Value::null;
        returnValue = objectValue[rootObjectName].get(key, Json::Value());
    } else {
        if (objectValue.isArray())
            return Json::Value::null;
        returnValue = objectValue.get(key, Json::Value());
    }
    return returnValue;
}

Json::Value JsonCppWrapper::getObjectInArray(std::string arrayKey, std::string objectKey, std::string rootObjectName)
{
    Json::Value returnValue = Json::nullValue;
    if (rootObjectName.size() > 1) {
        if (!mRootValue[rootObjectName][arrayKey].isArray())
            return Json::nullValue;//It's not an array
        for (Json::Value value : mRootValue[rootObjectName][arrayKey]) {
            if (value.isObject() && value[objectKey] != Json::nullValue) {
                returnValue = value[objectKey];
                break;
            }
        }
    } else {
        if (!mRootValue[arrayKey].isArray())
            return Json::nullValue;
        for (Json::Value value : mRootValue[arrayKey]) {
            if (value.isObject() && value[objectKey] != Json::nullValue) {
                returnValue = value[objectKey];
                break;
            }
        }
    }
    return returnValue;
}

Json::Value JsonCppWrapper::getObjectInArrayValue(Json::Value *objValue, std::string objectKey)
{
    if (objValue == nullptr || !objValue->isArray())
        return Json::nullValue;
    Json::Value returnValue = Json::nullValue;
    if (!objValue->isArray())
        return Json::nullValue;
    for (Json::Value value : *objValue) {
        if (value.isObject() && value[objectKey] != Json::nullValue) {
            returnValue = value[objectKey];
            break;
        }
    }
    return returnValue;
}

std::vector<Json::Value> JsonCppWrapper::getArray(std::string key, std::string rootObjectName)
{
    std::vector<Json::Value> arrayValueVector;
    if (rootObjectName.size() > 1) {
        if (!mRootValue[rootObjectName][key].isArray())
            return arrayValueVector;
        for (Json::Value value : mRootValue[rootObjectName][key]) {
            arrayValueVector.push_back(value);
        }
    } else {
        if (!mRootValue[key].isArray())
            return arrayValueVector;
        for (Json::Value value : mRootValue[key]) {
            arrayValueVector.push_back(value);
        }
    }
    return arrayValueVector;
}

std::vector<Json::Value> JsonCppWrapper::getArrayInValue(Json::Value *objValue)
{
    std::vector<Json::Value> arrayValueVector;
    if (!objValue->isArray())
        return arrayValueVector;
    for (Json::Value value : *objValue) {
        arrayValueVector.push_back(value);
    }
    return arrayValueVector;
}

std::vector<std::string> JsonCppWrapper::getObjectNamesInKey(std::string key)
{
    return mRootValue[key].getMemberNames();
}

std::vector<std::string> JsonCppWrapper::getObjectNames()
{
    return mRootValue.getMemberNames();
}
}
