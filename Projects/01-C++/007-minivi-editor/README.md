# Mini-VI Editor

## Description

- A mini terminal app mimicking the VI editor.
- Create files with buffer content
- Portable on Windows and Linux.

## Structure of the App

- [main.cpp](./src/main.cpp)
    - contains the entry point to the app
    - builds the menu page of the app and controls the flow of the pages
    - builds and maintains the "New File" page
- [terminal.h](./src/terminal.h) & [terminal.cpp](./src/terminal.cpp)
    - contain enum ``InputMode`` with values:
        - ``CMDMODE`` standing for command mode, where user's input is not echoed and canonical mode is off.
        - ``ECHOMODE`` standing for echo mode, where user's input is echoed and in canonical mode.
    - contain enum ``SpecialKey`` which has values for special keys that are used throughout the app.
    - contain struct ``Key`` which is composed of a flag if the key is special and a union between char and ``SpecialKey``, storing the key's value.
    - containstruct ``TerminalSize`` storing the rows and columns count of the terminal window.
    - contain the functions that control the terminal: 
        - ``gotoxy(int x, int y)``
        - ``int getColorCode(const string &color)``
        - ``void setColor(const std::string &textColor, const std::string &bgColor)``
        - ``void resetColor()``
        - ``void clearScreen()``
        - ``void hideCursor(bool hide)``
        - ``void setInputMode(InputMode mode)``
        - ``Key getKeyPress()``
        - ``TerminalSize getTerminalSize()``
        - ``const char* getCurrentUsername()``
        - ``void printAt(const std::string &text, short x, short y, const std::string &textColor, const std::string &bgColor)``
- [validators.h](./src/validators.h) & [validators.cpp](./src/validators.cpp)
    - contain the functions to validate user's input on file name and size:
        - ``bool isFileExist(const std::string *fileName)``
        - ``bool hasInvalidChars(const std::string &fileName)``
        - ``bool isEmpty(const std::string &fileName)``
        - ``bool isInputInteger(const std::string &input)``
        - ``int strToInt(const std ::string &input)``
        - ``std::string getValidFilename()``
        - ``int getValidFileSize()``
- [files.h](./src/files.h) & [files.cpp](./src/files.cpp)
    - contain the functions that handle file management:
        - ``void takeFileInput(int count, char *fileContent,char *fileName)``
        - ``void endFileInput(int count, char *fileContent, char *fileName)``
        - ``void saveFile(char *fileContent, char *fileName)``
        - ``void displayExistingFiles()``
        - ``void viewFileContent(const std::string &filename)``

## Flow of the App

![flow of the app](flow.png)

1. Main Menu Page:
    - The app initally opens on the main menu page with 3 options available to navigate through using the arrow keys.
    - The options available are New File, Display Files, and Exit.
2. New File Page:
    - When the user chooses the New File page from the Menu Page, the user is prompted to enter the name of the file. If the file name inputted is empty, has invalid characters or the file name already exists, the user is reprompted until the user inputs a valid name.
    - After providing a valid name, the user is prompted to enter the wanted count of characters to register for the file size. If the input size is a non integer, non positive or empty string the user is reprompted until the user inputs a valid size.
    - The user will be able to input their characters and fill the input buffer. The user's selected size will be highlighted for showing what space is available to add. The user can input any character in the allocated size.
    - When the user wants to finish, they can press ESC then save the file, which returns to the Main Menu page.
3. Display Files Page:
    - Displays a list of the saved files. User is able to navigate between the available files and choose one to view.
    - Once choosing a file its content will be displayed
    - The user can go back to the display the files page or exit the application.
4. Exit:
    - If the user navigates to the exit option and chooses it or clicks ESC button the app exits.

## External Libraries Used:

- [iostream](https://cplusplus.com/reference/iostream/)
    - stands for Input/Output Stream
    - used to access the objects ``cin`` and ``cout``
- [cstdlib](https://cplusplus.com/reference/cstdlib/)
    - stands for C Standard Library
    - used to access the environment ``system`` to execute system commands (clear terminal in our case) 
- [windows](https://learn.microsoft.com/en-us/windows/win32/api/winbase/)
    - used to access Windows API to use its system calls and objects like ``HANDLE``, ``GetStdHandle()``, ``COORD``, ``SetConsoleCursorPosition()``, and ``SetConsoleTextAttribute()``
- [conio](https://code-reference.com/c/conio.h)
    - stands for Console Input/Output
    - used to access function ``_getch()`` to parse inputted keys in Windows
- [unistd](https://pubs.opengroup.org/onlinepubs/7908799/xsh/unistd.h.html)
    - stands for Unix Standard.
    - used for ```STDIN_FILENO``` constant
- [termios](https://pubs.opengroup.org/onlinepubs/7908799/xsh/termios.h.html)
    - stands for Terminal Input Output Settings
    - used for gaining access to the ``termios`` structure and its methods ```tcgetattr()``` & ```tcsetattr()``` as well as the flag ```c_lflag``` and the attribute ```TCSANOW```.
- [sys/ioctl](https://man7.org/linux/man-pages/man2/ioctl.2.html)
    - stands for Input/Output Control
    - used for gaining access to the ``TIOCGWINSZ`` function which is used to get and set window size
- [pwd](https://pubs.opengroup.org/onlinepubs/009695099/basedefs/pwd.h.html)
    - stands for password
    - used for gaining access to the ``getpwuid`` and ``getuid`` functions to know the username of the current logged in user.

## Resources
- [Windows Handle](https://learn.microsoft.com/en-us/windows/win32/sysinfo/handles-and-objects)
- [Linux Termios](https://pubs.opengroup.org/onlinepubs/7908799/xsh/termios.h.html)
- [Linux terminal escape characters](https://stackoverflow.com/questions/38770996/linux-moving-the-console-cursor-visual)
- [Linux terminal coloring](https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal)
https://www.geeksforgeeks.org/cpp/how-to-change-console-color-in-cpp/

- [Windows coloring](https://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c)
- [Windows set input mode](https://learn.microsoft.com/en-us/windows/console/setconsolemode)
- https://learn.microsoft.com/en-us/windows/console/getconsolemode
- https://www.reddit.com/r/cpp_questions/comments/16r50l4/whats_dword_and_lpword_supposed_to_be_used_for/ 
- https://stackoverflow.com/questions/9292231/unechoed-string-input-in-c
- [Linux arrow codes](https://stackoverflow.com/questions/10463201/getch-and-arrow-codes)
- [Terminal Size](https://stackoverflow.com/questions/23369503/get-size-of-terminal-window-rows-columns)
- [filesystem](https://en.cppreference.com/w/cpp/filesystem.html)