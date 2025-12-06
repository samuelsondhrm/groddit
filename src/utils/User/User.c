#include "User.h"
#include "../Profil/Profil.h"

int findIdByUsername(const char *username)
{
    for (int i = 0; i < USER_COUNT; i++)
    {
        char currUsername[256];
        wordToString(currUsername, USERS[i].username);
        if (strCmp(currUsername, username) == 0)
            return i;
    }
    return -1;
}

char *findUsernameById(int id)
{
    static char buffer[256];

    if (id < 0 || id >= USER_COUNT)
    {
        buffer[0] = '\0';
        return buffer;
    }

    wordToString(buffer, USERS[id].username);
    return buffer;
}

int findUserIndexById(const char *userId)
{
    if (userId == NULL || userId[0] == '\0')
        return IDX_UNDEF;
    char idStr[16];
    for (int i = 0; i < USER_COUNT; i++)
    {
        wordToString_safe(idStr, sizeof(idStr), USERS[i].user_id);
        if (strCmp(idStr, userId) == 0)
            return i;
    }
    return IDX_UNDEF;
}

void generateUserID(char *id, int num)
{
    id[0] = 'U';
    id[1] = 'S';
    id[2] = 'E';
    id[3] = 'R';
    int hundreds = (num / 100) % 10;
    int tens = (num / 10) % 10;
    int ones = num % 10;
    id[4] = '0' + hundreds;
    id[5] = '0' + tens;
    id[6] = '0' + ones;
    id[7] = '\0';
}

void registerUser()
{
    if (CURRENT_USER_INDEX != -1)
    {
        char *usn;
        usn = findUsernameById(CURRENT_USER_INDEX);
        printError("Cannot register while logged in");
        printf("You are already logged in as %s. Please logout first.\n", usn);
        return;
    }

    clearScreen();
    printBreadcrumb("Main Menu > Authentication > New Account");
    
    printf("\n");
    printf("%s", BOLD_CYAN);
    printHorizontalLine(80, BOX_TL, BOX_H, BOX_TR);
    printf("%s                    GRODDIT ACCOUNT REGISTRATION                    %s\n", BOX_V, BOX_V);
    printHorizontalLine(80, BOX_BL, BOX_H, BOX_BR);
    printf("%s\n", RESET);

    char username[256];
    char password[25];
    int usernameValid = 0;
    int passwordValid = 0;

    printSectionHeader("", "ACCOUNT INFORMATION");
    printf("\n");
    
    while (!usernameValid)
    {
        printf("%sUsername%s\n", BOLD_WHITE, RESET);
        printf("%s├─%s Enter: ", BOX_V, RESET);
        fflush(stdout);
        STARTWORD_INPUT();
        wordToString(username, currentWord);
        IgnoreNewline();

        printf("%s└─%s ", BOX_V, RESET);
        spinnerAnimation("Checking availability", 5);

        if (findIdByUsername(username) != -1)
        {
            printf("%s└─%s ", BOX_V, RESET);
            printError("Username already taken");
            printf("   Username %s%s%s is already registered. Try another.\n\n", BOLD_YELLOW, username, RESET);
            continue;
        }

        printf("%s└─%s %s[AVAILABLE]%s\n\n", BOX_V, RESET, GREEN, RESET);
        usernameValid = 1;
    }

    while (!passwordValid)
    {
        printf("%sPassword%s %s(8-20 characters)%s\n", BOLD_WHITE, RESET, DIM, RESET);
        printf("%s├─%s Enter: ", BOX_V, RESET);
        fflush(stdout);
        STARTWORD_INPUT();
        wordToString(password, currentWord);

        IgnoreNewline();

        int passLen = 0;
        while (password[passLen] != '\0')
            passLen++;

        if (passLen < 8 || passLen > 20)
        {
            printf("%s└─%s ", BOX_V, RESET);
            printError("Invalid password length");
            printf("   Password must be 8-20 characters. You entered %d characters.\n\n", passLen);
            continue;
        }

        printf("%s└─%s %s[SECURE]%s\n\n", BOX_V, RESET, GREEN, RESET);
        passwordValid = 1;
    }

    printSectionDivider();

    printSectionHeader("⚙️", "CREATING ACCOUNT");
    printf("\n");

    if (!ensureCapacity((void **)&USERS, &USER_CAPACITY, sizeof(User), USER_COUNT + 1))
    {
        printError("Failed to create account");
        printf("System capacity full. Please contact administrator.\n");
        return;
    }

    for (int i = 0; i <= 100; i += 25)
    {
        showProgress("Creating", i);
        usleep(150000);
    }
    printf("\n");

    User *u = &USERS[USER_COUNT];

    char uid[8];
    generateUserID(uid, USER_COUNT + 1);

    Word uidWord;
    stringToWord(&uidWord, uid);
    copyWord(&u->user_id, uidWord);

    Word usn;
    Word pw;
    if (password_hashing_enabled())
    {
        uint32_t h = hash_password(password);
        uint32ToWord(&pw, h);
    }
    else
    {
        stringToWord(&pw, password);
    }
    stringToWord(&usn, username);
    copyWord(&u->username, usn);
    copyWord(&u->password, pw);
    u->karma = 0;
    u->created_at = time(NULL);

    USER_COUNT++;
    addVertex(&SOCIAL_GRAPH);

    printf("\n");
    printSectionHeader("", "REGISTRATION SUCCESSFUL");
    printf("\n");
    
    char timeStr[30];
    timeToStr(timeStr, u->created_at);
    
    printf("%s Account ID:    %s%s%s\n", BOX_V, BOLD_WHITE, uid, RESET);
    printf("%s Username:      %s%s%s\n", BOX_V, BOLD_WHITE, username, RESET);
    printf("%s Created:       %s%s%s\n", BOX_V, BOLD_WHITE, timeStr, RESET);
    printf("%s Initial Karma: %s%d%s\n", BOX_V, BOLD_WHITE, u->karma, RESET);
    
    printSectionDivider();

    printSectionHeader("", "NEXT STEPS");
    printf("  %s1.%s Use %sLOGIN;%s to access your account\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
    printf("  %s2.%s Complete your profile with %sPROFILE;%s\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
    printf("  %s3.%s Explore communities with %sSHOW_FEED;%s\n\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);

    printSuccess("Account registration completed!");
}

void loginUser()
{
    if (CURRENT_USER_INDEX != -1)
    {
        char *usn = findUsernameById(CURRENT_USER_INDEX);
        printError("Already logged in");
        printf("You are currently logged in as %s%s%s.\n", BOLD_WHITE, usn, RESET);
        printf("Please use %sLOGOUT;%s first if you want to switch accounts.\n", BOLD_WHITE, RESET);
        return;
    }

    clearScreen();
    printBreadcrumb("Main Menu > Authentication > Login");
    
    printf("\n");
    printf("%s", BOLD_CYAN);
    printHorizontalLine(80, BOX_TL, BOX_H, BOX_TR);
    printf("%s                     GRODDIT ACCOUNT LOGIN                          %s\n", BOX_V, BOX_V);
    printHorizontalLine(80, BOX_BL, BOX_H, BOX_BR);
    printf("%s\n", RESET);

    printSectionHeader("", "AUTHENTICATION");
    printf("\n");

    char username[256];
    char inputPw[256];

    printf("%sUsername%s\n", BOLD_WHITE, RESET);
    printf("%s├─%s Enter: ", BOX_V, RESET);
    STARTWORD_INPUT();
    wordToString(username, currentWord);
    IgnoreNewline();

    printf("%s└─%s ", BOX_V, RESET);
    spinnerAnimation("Checking account", 5);

    int user_id = findIdByUsername(username);
    if (user_id == -1)
    {
        printf("%s└─%s ", BOX_V, RESET);
        printError("Account not found");
        printf("   Username %s%s%s does not exist. Please register first.\n", BOLD_YELLOW, username, RESET);
        printf("\n   Use %sREGISTER;%s to create a new account.\n", BOLD_WHITE, RESET);
        return;
    }

    printf("%s└─%s %s[ACCOUNT FOUND]%s\n\n", BOX_V, RESET, GREEN, RESET);

    printf("%sPassword%s\n", BOLD_WHITE, RESET);
    printf("%s├─%s Enter: ", BOX_V, RESET);
    STARTWORD_INPUT();
    wordToString(inputPw, currentWord);
    IgnoreNewline();

    int len = 0;
    while (inputPw[len] != '\0')
        len++;

    if (len < 8 || len > 20)
    {
        printf("%s└─%s ", BOX_V, RESET);
        printError("Invalid password length");
        printf("   Password must be 8-20 characters.\n");
        return;
    }

    printf("%s└─%s ", BOX_V, RESET);
    spinnerAnimation("Verifying credentials", 8);

    char storedPw[256];
    wordToString(storedPw, USERS[user_id].password);

    int stored_is_digits = 1;
    for (int i = 0; storedPw[i] != '\0'; i++)
    {
        if (storedPw[i] < '0' || storedPw[i] > '9')
        {
            stored_is_digits = 0;
            break;
        }
    }

    if (password_hashing_enabled())
    {
        uint32_t hashedInput = hash_password(inputPw);

        if (stored_is_digits)
        {
            uint32_t storedHash = (uint32_t)wordToInt(USERS[user_id].password);

            if (hashedInput != storedHash)
            {
                printf("%s└─%s ", BOX_V, RESET);
                printError("Authentication failed");
                printf("   Incorrect password. Please try again.\n");
                return;
            }

            CURRENT_USER_INDEX = user_id;
        }
        else
        {
            if (strcmp(inputPw, storedPw) != 0)
            {
                printf("%s└─%s ", BOX_V, RESET);
                printError("Authentication failed");
                printf("   Incorrect password. Please try again.\n");
                return;
            }

            CURRENT_USER_INDEX = user_id;
        }
    }
    else
    {
        if (strcmp(inputPw, storedPw) != 0)
        {
            printf("%s└─%s ", BOX_V, RESET);
            printError("Authentication failed");
            printf("   Incorrect password. Please try again.\n");
            return;
        }

        CURRENT_USER_INDEX = user_id;
    }

    printf("%s└─%s %s[AUTHENTICATED]%s\n\n", BOX_V, RESET, GREEN, RESET);
    
    printSectionDivider();
    
    printSectionHeader("", "LOGIN SUCCESSFUL");
    printf("\n");
    
    User *u = &USERS[user_id];
    char timeStr[30];
    timeToStr(timeStr, u->created_at);
    
    printf("%s Welcome back, %s%s%s!\n", BOX_V, BOLD_CYAN, username, RESET);
    printf("%s\n", BOX_V);
    
    char userIdStr[50];
    wordToString(userIdStr, u->user_id);
    
    printf("%s Account ID:    %s%s%s\n", BOX_V, BOLD_WHITE, userIdStr, RESET);
    printf("%s Karma:         %s%d%s\n", BOX_V, BOLD_YELLOW, computeUserKarma(user_id), RESET);
    printf("%s Member since:  %s%s%s\n", BOX_V, BOLD_WHITE, timeStr, RESET);
    
    printSectionDivider();

    printSectionHeader("", "QUICK ACTIONS");
    printf("  %s•%s %sSHOW_FEED;%s - View your personalized feed\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
    printf("  %s•%s %sPOST;%s - Create a new post\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
    printf("  %s•%s %sPROFILE;%s - View your profile\n\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);

    printSuccess("You are now logged in!");
}

void logoutUser()
{
    if (CURRENT_USER_INDEX == -1)
    {
        printError("Not logged in");
        printf("You must be logged in to logout.\n");
        printf("\nUse %sLOGIN;%s to sign in to your account.\n", BOLD_WHITE, RESET);
        return;
    }

    char currentUsername[256];
    wordToString(currentUsername, USERS[CURRENT_USER_INDEX].username);

    clearScreen();
    printBreadcrumb("Main Menu > Authentication > Logout");
    
    printf("\n");
    printf("%s", BOLD_CYAN);
    printHorizontalLine(80, BOX_TL, BOX_H, BOX_TR);
    printf("%s                        LOGOUT CONFIRMATION                         %s\n", BOX_V, BOX_V);
    printHorizontalLine(80, BOX_BL, BOX_H, BOX_BR);
    printf("%s\n", RESET);

    printSectionHeader("", "SIGNING OUT");
    printf("\n");
    printf("Currently logged in as: %s%s%s\n\n", BOLD_CYAN, currentUsername, RESET);

    spinnerAnimation("Saving session", 8);
    spinnerAnimation("Signing out", 8);

    CURRENT_USER_INDEX = -1;

    if (CURRENT_USER_INDEX == -1)
    {
        printf("\n");
        printSectionHeader("", "LOGOUT SUCCESSFUL");
        printf("\n");
        printf("%s You have been signed out successfully.\n", BOX_V);
        printf("%s Session ended for: %s%s%s\n", BOX_V, BOLD_WHITE, currentUsername, RESET);
        
        printSectionDivider();

        printSectionHeader("", "SEE YOU AGAIN");
        printf("  %s•%s Use %sLOGIN;%s to sign back in\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        printf("  %s•%s Use %sREGISTER;%s to create a new account\n\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);

        printSuccess("Thank you for using Groddit!");
    }
    else
    {
        printError("Logout failed");
        printf("Unable to sign out. Please try again.\n");
    }

    printf("\n");
    return;
}