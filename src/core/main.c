#include "main.h"

int main() {
    initialize();

    printf("\n============== USERS ==============\n");
    for (int i = 0; i < USER_COUNT; i++) {
        User u = USERS[i];
        printf("UserID: %s | Username: %s | Karma: %d | Created: %s\n",
            u.user_id, u.username, u.karma, u.created_at);
        printf("Password: %s\n\n", u.password);
    }

    printf("\n============== POSTS ==============\n");
    for (int i = 0; i < POST_COUNT; i++) {
        Post p = POSTS[i];
        printf("PostID: %s | Subgroddit: %s | Author: %s\n",
            p.post_id, p.subgroddit_id, p.author_id);
        printf("Title: %s\n", p.title);
        printf("Content: %s\n", p.content);
        printf("Created: %s | Upvotes: %d | Downvotes: %d\n\n",
            p.created_at, p.upvotes, p.downvotes);
    }

    printf("\n============== COMMENTS ==============\n");
    for (int i = 0; i < COMMENT_COUNT; i++) {
        Comment c = COMMENTS[i];
        printf("ID: %d | Post: %s | Author: %s | Parent: %d\n",
            c.comment_id, c.post_id, c.author_id, c.parent_comment_id);
        printf("Content: %s\n", c.content);
        printf("Upvotes: %d | Downvotes: %d\n\n",
            c.upvotes, c.downvotes);
    }

    printf("\n============== SUBGRODDITS ==============\n");
    for (int i = 0; i < SUBGRODDIT_COUNT; i++) {
        Subgroddit s = SUBGRODDITS[i];
        printf("SubgrodditID: %s | Name: %s\n\n",
            s.subgroddit_id, s.name);
    }

    printf("\n============== SOCIAL (FOLLOW) ==============\n");
    for (int i = 0; i < SOCIAL_COUNT; i++) {
        Social s = SOCIALS[i];
        printf("Follower: %s -> Following: %s\n",
            s.follower_id, s.following_id);
    }

    printf("\n============== VOTING ==============\n");
    for (int i = 0; i < VOTING_COUNT; i++) {
        Voting v = VOTINGS[i];
        printf("User: %s | TargetType: %s | TargetID: %s | Vote: %s\n",
            v.user_id, v.target_type, v.target_id, v.vote_type);
    }

    printf("\n=== SELESAI PRINT SEMUA DATA ===\n");

    //
    // FREE MEMORY
    //

    // Free Post content
    for (int i = 0; i < POST_COUNT; i++) {
        if (POSTS[i].content != NULL)
            free(POSTS[i].content);
    }

    // Free Comment content
    for (int i = 0; i < COMMENT_COUNT; i++) {
        if (COMMENTS[i].content != NULL)
            free(COMMENTS[i].content);
    }

    free(USERS);
    free(POSTS);
    free(COMMENTS);
    free(SUBGRODDITS);
    free(SOCIALS);
    free(VOTINGS);

    return 0;
}
