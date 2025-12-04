#include "Kreativitas.h"

void printBanner() {
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
        "                                  ██████                                    \n"
    );
}


void loadingBar(int length, int duration) {
    for (int i = 0; i < length; i++) {
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


const char *phases[] = { "░", "▒", "▓", "█" };
void loadingBarSmooth(int length, int delayMicroseconds) {
    int totalPhases = 4;  

    for (int i = 0; i < length * totalPhases; i++) {
        int filledBlocks = i / totalPhases;
        int phaseIndex   = i % totalPhases;

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

void printMenu() {
    printf("\n          [ 📘 DAFTAR COMMAND GRODDIT : CREDIT]\n");

    printf("🔐  AKUN\n");
    printf("   • REGISTER;\n");
    printf("   • LOGIN;\n");
    printf("   • LOGOUT;\n");
    printf("   • PROFILE <username>;\n\n");

    printf("🧭  NAVIGASI & PEMBACAAN\n");
    printf("   • SHOW_FEED;                // Lihat feed utama\n");
    printf("   • VIEW_SUBGRODDIT;          // Masuk ke subgroddit\n");
    printf("   • VIEW_POST;                // Lihat detail post\n");
    printf("   • PRINTPOSTS;               // Debug print\n");
    printf("   • PRINTCOMMENTS;\n");
    printf("   • PRINTUSERS;\n");
    printf("   • PRINTSUBGRODDITS;\n");
    printf("   • PRINTSOCIALS;\n");
    printf("   • PRINTVOTINGS;\n\n");

    printf("📝  POSTING\n");
    printf("   • POST;                     // Membuat post baru\n");
    printf("   • DELETE_POST;\n");
    printf("   • CREATE_SUBGRODDIT r/<name>;\n");
    printf("   • COMMENT;                  // Tambah komentar\n");
    printf("   • DELETE_COMMENT;\n\n");

    printf("⬆️  VOTING\n");
    printf("   • UPVOTE_POST;\n");
    printf("   • DOWNVOTE_POST;\n");
    printf("   • UNDO_VOTE_POST;\n");
    printf("   • UPVOTE_COMMENT;\n");
    printf("   • DOWNVOTE_COMMENT;\n");
    printf("   • UNDO_VOTE_COMMENT;\n\n");

    printf("👥  SOSIAL\n");
    printf("   • FOLLOW <username>;\n");
    printf("   • UNFOLLOW <username>;\n");
    printf("   • FOLLOWING;                  // Atau FOLLOWING <username>;\n");
    printf("   • FOLLOWERS;                  // Atau FOLLOWERS <username>;\n");
    printf("   • FRIEND_RECOMMENDATION;\n\n");

    printf("🔎  DISCOVERY\n");
    printf("   • SEARCH_USER;\n");
    printf("   • SEARCH_POST;\n");
    printf("   • SEARCH_SUBGRODDIT;\n");
    printf("   • TRENDING;\n\n");

    printf("💾  DATA MANAGEMENT\n");
    printf("   • LOAD;\n");
    printf("   • SAVE;\n\n");

    printf("🚪  KELUAR PROGRAM\n");
    printf("   • EXIT;\n");
    printf("   • QUIT;\n\n");
}
