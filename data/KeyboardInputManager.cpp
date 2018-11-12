//
// Created by bogonets on 2018. 11. 9..
//

#include "KeyboardInputManager.hpp"

KeyboardInputManager::KeyboardInputManager(ObjectType type, std::string const & file_name) : _type(type)
{
    load(type);
}

KeyboardInputManager::~KeyboardInputManager()
{
    /* EMPTY */
}

void KeyboardInputManager::load(ObjectType type)
{

}

// Detection Code List
//  0 ~ 9 : class change
// 10 ~ 19: move to folder
// 20  : confirm box
// 30  : next object
// 31  : previous object
// 32  : delete object
// 50  : nextfile
// 51  : prevfile
// 100 : deletefile
// 101 : cropfile
// 110 : crosshair on/off
// 111 : object name on/off
// 112 : box on/off
// 200 : undo

int KeyboardInputManager::getCode(int keycode)
{
    auto code = _key_map.find(keycode);
    std::cout << code->first << " " << code->second << std::endl;
    if (code != _key_map.end()) {
        return code->second;
    }

    if (_type == ObjectType::DETECTION) {
        switch (keycode) {
            case 13 : return 101;// 'enter', crop image
            case 69 : return 20; // 'e', confirm box

            case 96 : return 30; // '`', next object;
            case 9  : return 31; //'tab', prev object;
            case 84 : return 32; // 't', delete object

            case 81 : return 51; // 'q', previous image
            case 82 : return 50; // 'r', next image
                //case ?? : return 100// delete file!!

            case 49 : return 0;  // '1', class change
            case 50 : return 1;  // '2', class change
            case 51 : return 2;  // '3', class change
            case 52 : return 3;  // '4', class change
            case 53 : return 4;  // '5', class change
            case 54 : return 5;  // '6', class change
            case 55 : return 6;  // '7', class change
            case 56 : return 7;  // '8', class change
            case 57 : return 8;  // '9', class change
            case 48 : return 9;  // '0', class change

            case 90 : return 10; // 'z', image move to folder
            case 88 : return 11; // 'x', image move to folder
            case 67 : return 12; // 'c', image move to folder
            case 86 : return 13; // 'v', image move to folder

            case 72 : return 110;// 'h', crosshair on/off
            case 78 : return 111;// 'n', class name on/off
            case 66 : return 112;// 'b', box on/off

            case 85 : return 200;// 'u', undo
            default : return -1;
        }
    } else if (_type == ObjectType::SEGMENTATION) {
        return -1;
    }
    return -1;
}
