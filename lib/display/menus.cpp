#include "menus.h"

void displayMainmenu() {
    ansi.clearScreen();
    ansi.println("1 = Menu,                   This menu");
    ansi.println("2 = Search address,         Find address, use 2 x x for start and end address. F.e. 2 8 15");
    ansi.println("3 = Command category,       Use 3 x, x= 1=Deviceinfo, 2=Usageinfo, 3=Computedinfo, 4=Status, 5=Atrates");
    ansi.println("4 = Command name            Use 4 x for command, x = name of command. Use 4 ? to list commands.");
    ansi.println("5 = Unseal Battery,         Use 5 a b, a,b decimal or hex : f.e. 5 0x1234 0x5678. None for default values.");
    ansi.println("6 = Seal Battery,           ");
    ansi.println("7 = Clear Permanent Failure Use 7 a b, a,b decimal or hex : f.e. 5 0x1234 0x5678. None for default values.");
    ansi.println("8 = Full Access             Use 8 a b, a,b decimal or hex : f.e. 5 0x1234 0x5678. None for default values.");

}

void displaySmallmenu() {
    ansi.clearScreen();
    ansi.println("1=Menu, 2=Search, 3=Category, 4=Name, 5=Unseal, 6=Seal, 7=Clear PF, 8=Full Access");
    ansi.println();
}
