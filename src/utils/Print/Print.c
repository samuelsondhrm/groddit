#include "Print.h"

void printComments()
{
    printf("\n============== COMMENTS ==============\n");

    for (int i = 0; i < COMMENT_COUNT; i++)
    {
        Comment c = COMMENTS[i];

        char postId[201];
        char authorId[201];
        char contentStr[201];

        wordToString(postId, c.post_id);
        wordToString(authorId, c.author_id);
        wordToString(contentStr, c.content);

        printf("ID: %d | Post: %s | Author: %s | Parent: %d\n",
               c.comment_id, postId, authorId, c.parent_comment_id);

        printf("Content: %s\n", contentStr);
        printf("Upvotes: %d | Downvotes: %d\n\n",
               c.upvotes, c.downvotes);
    }
}

void printPosts()
{
    printf("\n============== POSTS ==============\n");
    Node *p = POSTS.head;
    while (p != NULL)
    {
        if (p->element.type == TYPE_POST)
        {
            Post post = p->element.data.post;

            char postId[201];
            char subId[201];
            char authorId[201];
            char title[201];
            char content[201];
            char createdAt[201];

            wordToString(postId, post.post_id);
            wordToString(subId, post.subgroddit_id);
            wordToString(authorId, post.author_id);
            wordToString(title, post.title);
            wordToString(content, post.content);
            timeToStr(createdAt, post.created_at);

            printf("ID: %s | SubGroddit: %s | Author: %s\n", postId, subId, authorId);
            printf("Title: %s\n", title);
            printf("Content: %s\n", content);
            printf("Created: %s | Upvotes: %d | Downvotes: %d\n\n",
                   createdAt, post.upvotes, post.downvotes);
        }
        p = p->next;
    }
}

void printUsers()
{
    printf("\n============== USERS ==============\n");

    for (int i = 0; i < USER_COUNT; i++)
    {
        User u = USERS[i];

        char userId[201];
        char username[201];
        char password[201];
        char createdAt[201];

        wordToString(userId, u.user_id);
        wordToString(username, u.username);
        wordToString(password, u.password);
        timeToStr(createdAt, u.created_at);

        printf("ID: %s | Username: %s | Password: %s\n", userId, username, password);
        printf("Karma: %d | Created: %s\n\n", u.karma, createdAt);
    }
}

void printSubGroddits()
{
    printf("\n============== SUBGRODDITS ==============\n");
    Node *p = SUBGRODDITS.head;
    while (p != NULL)
    {
        if (p->element.type == TYPE_SUBGRODDIT)
        {
            SubGroddit s = p->element.data.subgroddit;

            char subId[201];
            char name[201];

            wordToString(subId, s.subgroddit_id);
            wordToString(name, s.name);

            printf("ID: %s | Name: %s\n\n", subId, name);
        }
        p = p->next;
    }
}

void printSocials()
{
    printf("\n============== SOCIALS ==============\n");

    for (int i = 0; i < SOCIAL_COUNT; i++)
    {
        Social so = SOCIALS[i];

        char userId[201];
        char followingId[201];

        wordToString(userId, so.user_id);
        wordToString(followingId, so.following_id);

        printf("%s follows %s\n", userId, followingId);
    }
    printf("\n");
}

void printVotings()
{
    printf("\n============== VOTINGS ==============\n");
    printf("Total votings: %d\n", VOTING_COUNT);

    for (int i = 0; i < VOTING_COUNT; i++)
    {
        Voting v = VOTINGS[i];

        char userId[201];
        char targetType[201];
        char targetId[201];
        char voteType[201];

        wordToString(userId, v.user_id);
        wordToString(targetType, v.target_type);
        wordToString(targetId, v.target_id);
        wordToString(voteType, v.vote_type);

        printf("[%d] %s voted %s on %s %s\n", i, userId, voteType, targetType, targetId);
    }
    printf("\n");
}