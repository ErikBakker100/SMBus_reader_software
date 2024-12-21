#include "menus.h"

void displayMainmenu() {
    ansi.clearScreen();
    ansi.println("1 = Menu,                   This menu");
    ansi.println("2 = Search address,         Find address, use 2 x x for start and end address.");
    ansi.println("3 = Standard SBS commands,  Reads Registers as defined in rev 1.1 of the smart battery data specification.");
    ansi.println("4 = IC specific commands,   ");
    ansi.println("5 = Unseal Battery,         ");
    ansi.println("6 = Seal Battery,           ");
    ansi.println("7 = Clear Permanent Failure ");
    ansi.println("8 = Specify Command         Use 8 x for command, x can be name or reg, use 8 x x for range in hex.");
}

void displaySmallmenu() {
    ansi.clearScreen();
    ansi.println("1=Menu, 2=Search, 3=Standard, 4=Extended, 5=Unseal, 6=Seal, 7=Clear PF, 8=Specify Command");
}
