//
// Created by simon on 2016-03-13.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_HUDSTRINGS_H
#define SUPER_BUBBA_AWESOME_SPACE_HUDSTRINGS_H

using namespace std;

string hudStringWelcome = string("Welcome pilot")+
        "\r\n\r\n"+
        "Your mission is to destroy 50 of " +
        "the asteroids in this system. Be " +
        "aware that you do not have any "+
        "shields. One hit will destroy you.\r\n\r\n" +
        "Use W and S to thrust, A and D to " +
        "rotate, P and L to pitch and the " +
        "mouse button to fire the laser.\r\n\r\n" +
        "Press ENTER to proceed.";

string hudStringCredits = string("Credits\r\n\r\n") +
                          "This game has been made by \r\n" +
                          "* Viktor Sjolind\r\n" +
                          "* Johan Nilsson Hansen\r\n" +
                          "* Simon Lindhen\r\n" +
                          "* Sebastian Norlin\r\n" +
                          "While creating Bubba-3D\r\n\r\n" +
                          "github.com/Bubbers/Bubba-3D";

string hudStringFailure = string("MISSION\r\nFAILURE");
string hudStringSuccess = string("MISSION\r\nSUCCESS");

#endif //SUPER_BUBBA_AWESOME_SPACE_HUDSTRINGS_H
