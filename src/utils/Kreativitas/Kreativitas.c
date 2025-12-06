#include "Kreativitas.h"

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}


void printHorizontalLine(int width, const char* left, const char* middle, const char* right)
{
    printf("%s", left);
    for (int i = 0; i < width - 2; i++)
    {
        printf("%s", middle);
    }
    printf("%s\n", right);
}

void printDoubleLine(int width)
{
    printHorizontalLine(width, DBOX_TL, DBOX_H, DBOX_TR);
}

void printSingleLine(int width)
{
    printHorizontalLine(width, BOX_TL, BOX_H, BOX_TR);
}

void printBoxedText(const char* text, int width)
{
    int textLen = strlen(text);
    int padding = (width - textLen - 2) / 2;
    
    printf("%s", BOX_V);
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s", text);
    for (int i = 0; i < width - textLen - padding - 2; i++) printf(" ");
    printf("%s\n", BOX_V);
}

void printSectionHeader(const char* emoji, const char* title)
{
    printf("\n%s%s %s%s\n", BOLD_CYAN, emoji, title, RESET);
    printf("%s────────────────────────────────────────%s\n", DIM, RESET);
}

void printSectionDivider()
{
    printf("%s", DIM);
    printHorizontalLine(60, BOX_VR, BOX_H, BOX_VL);
    printf("%s", RESET);
}

void printBreadcrumb(const char* path)
{
    printf("%s%s%s\n", BOLD_BLUE, path, RESET);
}

void printStatusBar(const char* username, int karma, const char* location)
{
    printf("\n");
    printHorizontalLine(80, BOX_TL, BOX_H, BOX_TR);
    
    if (username != NULL && strlen(username) > 0)
    {
        printf("%s %s%s%s %s| Karma: %s%d%s %s| %s%s %s\n",
               BOX_V,
               BOLD_YELLOW, username, RESET,
               DIM, YELLOW, karma, RESET,
               DIM, location, BOX_V, RESET);
    }
    else
    {
        printf("%s %sGuest Mode%s %s| %s%s %s\n",
               BOX_V,
               DIM, RESET,
               DIM, location, BOX_V, RESET);
    }
    
    printHorizontalLine(80, BOX_BL, BOX_H, BOX_BR);
}

void printSuccess(const char* message)
{
    printf("\n%s[SUCCESS]%s %s\n", BOLD_GREEN, RESET, message);
}

void printError(const char* message)
{
    printf("\n%s[ERROR]%s %s\n", BOLD_RED, RESET, message);
}

void printWarning(const char* message)
{
    printf("\n%s[WARNING]%s %s\n", BOLD_YELLOW, RESET, message);
}

void printInfo(const char* message)
{
    printf("\n%s[INFO]%s %s\n", BOLD_CYAN, RESET, message);
}

void showProgress(const char* message, int percent)
{
    int barWidth = 40;
    int filled = (barWidth * percent) / 100;
    
    printf("\r%s %s[", message, BOLD_CYAN);
    for (int i = 0; i < barWidth; i++)
    {
        if (i < filled)
            printf("█");
        else
            printf("░");
    }
    printf("]%s %d%%", RESET, percent);
    fflush(stdout);
    
    if (percent >= 100)
        printf("\n");
}

void spinnerAnimation(const char* message, int steps)
{
    const char* spinner[] = {"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"};
    int spinnerSize = 10;
    
    for (int i = 0; i < steps; i++)
    {
        printf("\r%s%s %s%s", BOLD_CYAN, spinner[i % spinnerSize], message, RESET);
        fflush(stdout);
        usleep(100000); // 100ms
    }
    printf("\r%s[DONE] %s%s\n", BOLD_GREEN, message, RESET);
}


void printCommandHelp(const char* command, const char* description, int isActive)
{
    if (isActive)
    {
        printf("   %s%-25s%s %s│%s %s\n", 
               BOLD_WHITE, command, RESET,
               DIM, RESET,
               description);
    }
    else
    {
        printf("   %s%-25s%s %s│%s %s%s\n", 
               DIM, command, RESET,
               DIM, RESET,
               DIM, description);
    }
}

void printInputPrompt(const char* context)
{
    printf("\n%s┌─ %s%s\n", DIM, context, RESET);
    printf("%s│%s\n", DIM, RESET);
    printf("%s└─>%s %s█▒▒░░%s ", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
    fflush(stdout);
}

void printBanner()
{
    printf(
        "    █░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░█         ██░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░\n"
        "  ▒▓▓▓▓▓▓▓▓▓▓▓▓▒██       █▒█▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒█          ██▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒\n"
        "  ▒▓█▓█▓█▓█▓▓▒███        █░▓▓█▓█▓█▓█▓█▓█▓▒██           ███▓█▓█▓█▓█▓█▓█▓█▓█▓█▒\n"
        "  ▒▓█▓▓█▓█▓▓▒██          ██▒▓▓█▓█▓█▓█▓█▓█▒██             ██▒▓█▓▓█▓█▓█▓█▓█▓█▓▒\n"
        "  ▒▓▓█▓▓▓▓▓███            █▒█▓▓▓▓▓▓▓▓▓▓▓▓▒█               ██▒▓█▓▓▓▓▓▓▓▓▓▓▓▓▓▒\n"
        "  ▒▓█▓█▓█▓██              █░▓▓█▓█▓█▓█▓█▓█▒█                ███▓█▓█▓█▓█▓█▓█▓█▒\n"
        "  ▒▓▓▓▓█▓██               ██▒▓█▓▓█▓█▓█▓█▓▒█                  ██▒▓▓█▓█▓█▓█▓█▓▒\n"
        "  ▒▓█▓█▓██                 █▒▓▓█▓▓▓▓▓▓▓▓█▒█                   ██▒▓█▓▓▓▓▓▓▓▓▓▒\n"
        "  ▒▓█▓▓██                  ██▓█▓█▓█▓█▓█▓█░█                    ███▓▓█▓█▓█▓█▓▒\n"
        "  ▒▓█▓██                    █▒▓▓▓█▓█▓█▓██░█                      ██▒▓▓▓█▓█▓█▒\n"
        "  ▒▓▓██                     █▒▓█▓▓▓▓▓▓▓▓█▒█                       ███▓█▓▓▓▓▓▒\n"
        "  ▒▓██                      ██▒▓▒▒▓█▓█▓█▓▒█                         ██▒▓▓█▓█▒\n"
        "  ░▒█                        ██▒██░▓▓▓█▓█░█                          ███▓▓▓▓▒\n"
        "   ██                         █░███▒▓▓▓▓█▒█               █████        ██▓▓█▒\n"
        "  ██                          ██████▒▓█▓█▒█      ███████████      ████  ███▒░\n"
        "  █        █████ █████   █████ ███ ███▓▓▓▒█  ███       █████          █   ██▒\n"
        "        ██       █████         ███   ██▒▓▒███          █████         ██    ██\n"
        "      ██         █████         ███    ███▒░███         █████       ██        \n"
        "      ██         █████      ███  █      ████  ██████   █████ ██████          \n"
        "        ███████████████████               ███                                \n"
        "                                         ██                                 \n"
        "                                  ██████                                    \n");
}

void loadingBar(int length, int duration)
{
    for (int i = 0; i < length; i++)
    {
        printf("\r[");
        for (int j = 0; j < i; j++)
            printf(GREEN "█" RESET);
        for (int j = i; j < length; j++)
            printf(" ");
        printf("] %d%%", (i * 100) / length);
        fflush(stdout);
        usleep(duration);
    }
    printf("\r[");
    for (int j = 0; j < length; j++)
        printf(GREEN "█" RESET);
    printf("] 100%%\n");
}

const char *phases[] = {"░", "▒", "▓", "█"};
void loadingBarSmooth(int length, int delayMicroseconds)
{
    int totalPhases = 4;

    for (int i = 0; i < length * totalPhases; i++)
    {
        int filledBlocks = i / totalPhases;
        int phaseIndex = i % totalPhases;

        printf("\r[");

        for (int j = 0; j < filledBlocks; j++)
            printf("█");

        if (filledBlocks < length)
            printf("%s", phases[phaseIndex]);

        for (int j = filledBlocks + 1; j < length; j++)
            printf(" ");

        printf("]");
        fflush(stdout);
        usleep(delayMicroseconds);
    }
    printf("\n");
}

void printMenu()
{
    clearScreen();
    
    // Breadcrumb
    printBreadcrumb("Main Menu > Help");
    
    printf("\n");
    printf("%s", BOLD_CYAN);
    printHorizontalLine(80, DBOX_TL, DBOX_H, DBOX_TR);
    printf("%s         GRODDIT COMMAND CENTER - Interactive Help System         %s\n", DBOX_V, DBOX_V);
    printHorizontalLine(80, DBOX_BL, DBOX_H, DBOX_BR);
    printf("%s\n", RESET);
    
    printSectionHeader("", "AUTHENTICATION");
    printCommandHelp("REGISTER;", "Create a new account", 1);
    printCommandHelp("LOGIN;", "Sign in to your account", 1);
    printCommandHelp("LOGOUT;", "Sign out from your account", 1);
    printCommandHelp("PROFILE <username>;", "View user profile details", 1);
    
    printSectionDivider();
    
    printSectionHeader("", "NAVIGATION & READING");
    printCommandHelp("SHOW_FEED;", "View main feed posts", 1);
    printCommandHelp("VIEW_SUBGRODDIT;", "Enter subgroddit view", 1);
    printCommandHelp("VIEW_POST;", "Read post with comments", 1);
    printCommandHelp("PRINTPOSTS;", "Debug: Print all posts", 0);
    printCommandHelp("PRINTCOMMENTS;", "Debug: Print all comments", 0);
    printCommandHelp("PRINTUSERS;", "Debug: Print all users", 0);
    printCommandHelp("PRINTSUBGRODDITS;", "Debug: Print all subgroddits", 0);
    printCommandHelp("PRINTSOCIALS;", "Debug: Print social graph", 0);
    printCommandHelp("PRINTVOTINGS;", "Debug: Print voting records", 0);
    
    printSectionDivider();
    
    printSectionHeader("", "CONTENT MANAGEMENT");
    printCommandHelp("POST;", "Create a new post", 1);
    printCommandHelp("DELETE_POST;", "Remove your post", 1);
    printCommandHelp("CREATE_SUBGRODDIT r/<name>;", "Create new subgroddit", 1);
    printCommandHelp("COMMENT;", "Add comment to post", 1);
    printCommandHelp("DELETE_COMMENT;", "Remove your comment", 1);
    
    printSectionDivider();
    
    printSectionHeader("", "VOTING SYSTEM");
    printCommandHelp("UPVOTE_POST;", "Upvote a post", 1);
    printCommandHelp("DOWNVOTE_POST;", "Downvote a post", 1);
    printCommandHelp("UNDO_VOTE_POST;", "Remove your vote from post", 1);
    printCommandHelp("UPVOTE_COMMENT;", "Upvote a comment", 1);
    printCommandHelp("DOWNVOTE_COMMENT;", "Downvote a comment", 1);
    printCommandHelp("UNDO_VOTE_COMMENT;", "Remove your vote from comment", 1);
    
    printSectionDivider();
    
    printSectionHeader("", "SOCIAL FEATURES");
    printCommandHelp("FOLLOW <username>;", "Follow a user", 1);
    printCommandHelp("UNFOLLOW <username>;", "Unfollow a user", 1);
    printCommandHelp("FOLLOWING;", "View users you follow", 1);
    printCommandHelp("FOLLOWING <username>;", "View user's following list", 1);
    printCommandHelp("FOLLOWERS;", "View your followers", 1);
    printCommandHelp("FOLLOWERS <username>;", "View user's followers", 1);
    printCommandHelp("FRIEND_RECOMMENDATION;", "Get friend suggestions", 1);
    
    printSectionDivider();
    
    printSectionHeader("", "DISCOVERY & SEARCH");
    printCommandHelp("SEARCH_USER;", "Find users by username", 1);
    printCommandHelp("SEARCH_POST;", "Search posts by keyword", 1);
    printCommandHelp("SEARCH_SUBGRODDIT;", "Find subgroddits", 1);
    printCommandHelp("TRENDING;", "View trending content", 1);
    
    printSectionDivider();
    
    printSectionHeader("", "DATA MANAGEMENT");
    printCommandHelp("LOAD;", "Load data from files", 1);
    printCommandHelp("SAVE;", "Save current state to files", 1);
    
    printSectionDivider();
    
    printSectionHeader("", "SECURITY & PRIVACY");
    printCommandHelp("SET_SECURITY PASSWORD;", "Enable password hashing (irreversible)", 1);
    printCommandHelp("SET_SECURITY FILE ON;", "Enable file encryption", 1);
    printCommandHelp("SET_SECURITY FILE OFF;", "Disable file encryption", 1);
    
    printSectionDivider();
    
    printSectionHeader("", "SYSTEM");
    printCommandHelp("HELP;", "Show this help menu", 1);
    printCommandHelp("EXIT;", "Exit the application", 1);
    printCommandHelp("QUIT;", "Exit the application", 1);
    
    printf("\n%s", BOLD_CYAN);
    printHorizontalLine(80, BOX_TL, BOX_H, BOX_TR);
    printf("%s  %sTip:%s Type any command followed by %s;%s to execute  %s           %s%s\n", 
           BOX_V, BOLD_YELLOW, RESET, BOLD_WHITE, RESET, BOX_V, " ", BOX_V);
    printHorizontalLine(80, BOX_BL, BOX_H, BOX_BR);
    printf("%s\n\n", RESET);
}
