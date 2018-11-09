#ifndef FEPSTOOL_KEYBOARDINPUTMANAGER_HPP
#define FEPSTOOL_KEYBOARDINPUTMANAGER_HPP

#include <data/FebsStruture.hpp>
#include <string>

class KeyboardInputManager
{
private:
    ObjectType _type;
    std::map<int, int> _key_map;

public:
    KeyboardInputManager(ObjectType type, std::string const & file_name = "OC_Config");
    ~KeyboardInputManager();

public:
    void load(ObjectType type);

public:
    int getCode(int keycode);


};


#endif //FEPSTOOL_KEYBOARDINPUTMANAGER_HPP
