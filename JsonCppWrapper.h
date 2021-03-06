#ifndef JSONCPPWRAPPER_H
#define JSONCPPWRAPPER_H
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <json/json.h>

namespace zmc
{
class JsonCppWrapper
{
public:
    JsonCppWrapper(std::string filePath);
    JsonCppWrapper();
    ~JsonCppWrapper();
    bool doesObjectExist(std::string key, std::string rootObjectName = "");
    /**
     * @brief Loads a new *.json file to a Json::Value. Then all the functions use the same Json::Value object.
     * If you want to operate in different files, load them first! It does NOT create a new file if the file does not exist
     * @param filePath -> File path to the *.json file
     */
    bool loadJsonFile(std::string filePath);
    std::string &getLoadedJsonFilePath()
    {
        return mLoadedJsonFilePath;
    }

    /**
     * @brief Adds a new object to the loaded *.json file. Make sure to provide a root object name (e.g Student name) to add it
     * as a sub object of another. Otherwise it's added to the file without any root object, use with caution. Make sure the json file is
     * loaded. If the given key already exists, it overwrites the old value
     * @param rootObjectName -> If you don't have a root object just give it an emty value (e.g: "")
     * @param key
     * @param value
     */
    template<typename T>
    bool addNewObject(std::string key, T value, std::string rootObjectName = "");

    /**
     * @brief Removes an object from the loaded *.json file. Make sure to provide a root object name (e.g Student name) to remove
     * a sub object Otherwise it's removed from the file without any root object, use with caution. Make sure the json file is
     * loaded. Arrays are deleted with this function too.
     * @param rootObjectName -> If you don't have a root object just give it an emty value (e.g: "")
     * @param key
     * @param value
     */
    bool removeObject(std::string key, std::string rootObjectName = "");

    /**
     * @brief Adds a new array object to the loaded *.json file. Make sure to provide a root object name (e.g Student name) to add it
     * as a sub object of another. Otherwise it's added to the file without any root object, use with caution. Make sure the json file is
     * loaded. If the given key already exists, it appends to the old value. It doesn't check for uniqeness in the array.
     * So an item can exist more than once
     * @param key
     * @param Basic types are: Numbers, string, bool
     * @param rootObjectName
     */
    template<typename T>
    bool addNewArray(std::string key, std::vector<T> &items, std::string rootObjectName = "");

    /**
     * @brief Removes an item from an array in the loaded *.json file. Make sure to provide a root object name (e.g Student name)
     * to remove a sub object. Otherwise it's removed from the file without any root object, use with caution. Make sure the json file is
     * loaded. Since there can be same values more than once, all of the same items are deleted. This function is for removal of non-objects
     * in the array
     * @param key --> The key that has the array
     * @param value --> This value must be std:string if nestedArray is true
     * @param rootObjectName
     */
    template<typename T>
    bool removeItemFromArray(std::string key, T value, std::string rootObjectName = "");

    /**
     * @brief Removes an object item from an array in the loaded *.json file. Make sure to provide a root object name (e.g Student name)
     * to remove a sub object. Otherwise it's removed from the file without any root object, use with caution. Make sure the json file is
     * loaded. Since there can be same values more than once, all of the same items are deleted. This function is for removal of objects in
     * the array
     * @param key --> The key that has the array
     * @param value --> This value must be std:string if nestedArray is true
     * @param rootObjectName
     */
    template<typename T>
    bool removeObjectItemFromArray(std::string key, T value, std::string rootObjectName = "");

    /**
     * @brief Adds a new item to the array object in the loaded *.json file. Make sure to provide a root object name (e.g Student name)
     * to add it as a sub object of another. Otherwise it's added to the file without any root object, use with caution. Make sure the
     * json file is loaded. It doesn't check for uniqeness in the array. So an item can exist more than once. If the given key does
     * not exist, a new array object is created.
     * @param key --> The key that contains the array
     * @param value
     * @param rootObjectName
     */
    template<typename T>
    bool addItemToArray(std::string key, T value, std::string rootObjectName = "");

    /**
     * @brief Adds a new object to the array object in the loaded *.json file. Make sure to provide a root object name (e.g Student name)
     * to add it as a sub object of another. Otherwise it's added to the file without any root object, use with caution. Make sure the
     * json file is loaded. It doesn't check for uniqeness in the array. So an item can exist more than once. If the given key does
     * not exist, a new array object is created.
     * @param key --> The key that contains the array
     * @param objectKeys -> The object key array to add the value
     * @param values -- Indexes should contain the related value for the object key
     * @param rootObjectName
     */
    template<typename T>
    bool addObjectToArray(std::string key, std::vector<std::string> objectKeys, std::vector<T> values, std::string rootObjectName = "");
    template<typename T>
    bool addObjectToArray(std::string key, std::string objectKey, T values, std::string rootObjectName = "");

    /**
     * @brief Returns a Json::Value only containing the value for the key making it possible to convert the value between types.
     * @param key
     * @param rootObjectName
     * @return Json::Value if it does not exit Json::nullValue
     */
    Json::Value getObject(std::string key, std::string rootObjectName = "");

    /**
     * @brief Returns a Json::Value only containing the value for the key making it possible to convert the value between types.
     * If the objectValue does not equal to null, objectValue is used instead of mRootValue
     * @param key
     * @param objValue
     * @param rootObjectName
     * @return Json::Value if it does not exit Json::nullValue
     */
    Json::Value getObjectInValue(std::string key, Json::Value *objValue = nullptr, std::string rootObjectName = "");

    /**
     * @brief Returns an object inside an array.
     * @param arrayKey
     * @param objectKey
     * @param rootObjectName
     * @return Json::Value if it does not exit Json::nullValue
     */
    Json::Value getObjectInArray(std::string arrayKey, std::string objectKey, std::string rootObjectName = "");

    /**
     * @brief Returns the value of the key in an array value
     * @param objValue
     * @param objectKey
     * @return
     */
    Json::Value getObjectInArrayValue(Json::Value *objValue, std::string objectKey);

    /**
     * @brief Returns an std::vector<Json::Value>. The type those Json::Value have change depending on the elements of the array.
     * If the array has an object inside, an object will be returned then you can get key-value of that object using
     * JsonCppWrapper::getItemInObject
     * @param key
     * @param rootObjectName
     * @return
     */
    std::vector<Json::Value> getArray(std::string key, std::string rootObjectName = "");

    /**
     * @brief Returns the elements of the array in the given Json::Value. objValue must be an array object.
     * @param objValue
     * @return
     */
    std::vector<Json::Value> getArrayInValue(Json::Value *objValue);

    /**
     * @brief Returns the member names in the given key
     * @param key
     * @return
     */
    std::vector<std::string> getObjectNamesInKey(std::string key);

    /**
     * @brief Returns the member names in the root
     * @return
     */
    std::vector<std::string> getObjectNames();

private:
    Json::Value mRootValue;
    std::string mLoadedJsonFilePath;

private:
    void writeStyledJsonFile(Json::Value &jsonValueObject, std::string loadedFilePath);
};
}
#include "JsonCppWrapper.inl"//Includes the template function definitions
#endif // JSONCPPWRAPPER_H
