### IF2110 Algorithm and Programming II - Tubes 1
# Groddit - Social Media Platform

> A Reddit-inspired social media platform built in C

Groddit is a terminal based social media application that brings the essence of Reddit to your command line. Create posts, engage in discussions, vote on content, and connect with other users, all through an elegant text-based user interface.

## Team / Developer

**Kelompok K01-I:**

- 13524001 Samuelson D Tanuraharja
- 13524019 Kevin Wirya Valerian
- 13524023 Jonathan Kris Wicaksono
- 13524026 Made Branenda Jordhy
- 13524053 Muhammad Haris Putra Sulastianto

## Features

**Core Functionality:**
- **User Management**: Register accounts, login/logout with secure password hashing
- **Content Creation**: Create posts in subgroddits (topic-based communities) and comment with nested replies
- **Voting System**: Upvote/downvote posts and comments with karma tracking
- **Social Features**: Follow/unfollow users, view followers and following lists
- **Profile System**: View user profiles with activity statistics and recent posts

**Advanced Features:**
- **Smart Feed** (Heap-based): Personalized feed from followed users with latest/newest sorting
- **Trending Analysis**: Discover hot topics in subgroddits with keyword frequency analysis
- **Advanced Search** (Trie-based): Fast prefix search for users, posts, and subgroddits
- **Friend Recommendations** (BFS Graph): Get friend suggestions based on mutual connections
- **Content Moderation**: Automatic filtering of inappropriate content using blacklist
- **Data Security**: Password hashing (FNV-1a) and file encryption (LCG cipher)
- **Professional TUI**: Beautiful terminal interface with ANSI colors, box-drawing characters, and animations

## Getting Started

### Prerequisites

- GCC compiler (or any C compiler)
- Make build tool
- Unix-like environment (Linux, macOS, or Windows with WSL)

### Installation

1. Clone the repository:
```bash
git clone https://github.com/Labpro-22/if2110-tubes-2025-k01-i-1.git
cd if2110-tubes-2025-k01-i-1
```

2. Build the project:
```bash
make
```

3. Run the application:
```bash
./bin/main
```

### Initial Configuration

On first run, you'll be prompted to select a configuration folder:
- `config-1/`: Sample data with basic users and posts
- `PwHashed/`: Sample data with password hashing enabled
- `FileEnc/`: Sample data with file encryption enabled
- `PwHashedAndFileEnc/`: Maximum security configuration

Simply type the folder name (e.g., `config-1`) when prompted.

## Developing

To start developing this project:

```bash
git clone https://github.com/Labpro-22/if2110-tubes-2025-k01-i-1.git
cd if2110-tubes-2025-k01-i-1
```

### Project Structure

```
if2110-tubes-2025-k01-i-1/
├── README.md
├── Makefile                      # Build configuration
├── bin/                          # Compiled executables
│   └── main
├── build/                        # Object files (.o)
├── src/                          # Source files
│   ├── core/
│   │   ├── main.c               # Entry point
│   │   └── main.h
│   ├── adt/                     # Abstract Data Types
│   │   ├── ADTSederhana/        # Basic ADTs (User, Post, Comment, etc.)
│   │   ├── Graph/               # Graph for social network
│   │   ├── Heap/                # Max Heap for feed sorting
│   │   ├── ListBerkait/         # Linked list implementation
│   │   ├── MesinKarakter/       # Character machine
│   │   ├── MesinKata/           # Word machine for parsing
│   │   ├── PostTree/            # Tree for comment hierarchy
│   │   └── Trie/                # Trie for advanced search
│   └── utils/                   # Feature modules
│       ├── AdvancedSearch/      # Trie-based search
│       ├── Comment/             # Comment management
│       ├── ContentModeration/   # Content filtering
│       ├── Feed/                # Personalized feed
│       ├── FollRec/             # Friend recommendations
│       ├── Initialize/          # System initialization
│       ├── Kreativitas/         # TUI components
│       ├── Load/                # Data loading
│       ├── Post/                # Post management
│       ├── Profil/              # User profiles
│       ├── Save/                # Data persistence
│       ├── Security/            # Hashing & encryption
│       ├── Social/              # Follow system
│       ├── Subgroddit/          # Community management
│       ├── Trending/            # Trending analysis
│       ├── User/                # Authentication
│       └── Voting/              # Vote management
├── config/                      # Configuration & data files
│   ├── config-1/               # Sample data
│   ├── PwHashed/               # Hashed password data
│   ├── FileEnc/                # Encrypted file data
│   └── BlacklistWords/         # Moderation wordlist
└── doc/                        # Documentation
```

### Building

The project uses a Makefile for compilation:

```bash
# Build the project
make

# Clean build artifacts
make clean

# Rebuild from scratch
make clean && make
```

All object files are placed in `build/` and the final executable in `bin/main`.

## Usage

After launching the application, you'll see a welcome screen. Here are the main commands:

### Authentication
```
REGISTER;           # Create a new account
LOGIN;              # Sign in to your account
LOGOUT;             # Sign out
```

### Content Management
```
POST;               # Create a new post
COMMENT;            # Add a comment to a post
DELETE_POST;        # Delete your post
DELETE_COMMENT;     # Delete your comment
CREATE_SUBGRODDIT;  # Create a new community
```

### Browsing & Discovery
```
SHOW_FEED;                                    # View your personalized feed
VIEW_SUBGRODDIT <name> <HOT|NEW> <INCR|DECR>; # Browse subgroddit posts
LIHAT <post_id>;                              # View post with comments
SEARCH_USER;                                  # Search for users
SEARCH_POST;                                  # Search for posts
SEARCH_SUBGRODDIT;                            # Search for communities
TRENDING <subgroddit> <time> <unit>;          # View trending topics
```

### Social Features
```
FOLLOW <username>;     # Follow a user
UNFOLLOW <username>;   # Unfollow a user
FOLLOWING;             # View who you follow
FOLLOWERS;             # View your followers
FRIEND_REC;            # Get friend recommendations
PROFILE <username>;    # View user profile
```

### Voting
```
UPVOTE_POST;           # Upvote a post
DOWNVOTE_POST;         # Downvote a post
UNDO_VOTE_POST;        # Remove your vote from post
UPVOTE_COMMENT;        # Upvote a comment
DOWNVOTE_COMMENT;      # Downvote a comment
UNDO_VOTE_COMMENT;     # Remove your vote from comment
```

### Data Management
```
LOAD;               # Load data from folder
SAVE;               # Save current state
SET_SECURITY;       # Configure security settings
```

### Help
```
HELP;               # Show all available commands
EXIT; or QUIT;      # Exit the application
```

## Configuration

### Security Settings

Enable password hashing (irreversible):
```
SET_SECURITY PASSWORD;
```

Enable file encryption:
```
SET_SECURITY FILE ON;
```

Set custom encryption seed:
```
SET_SECURITY ENC_SEED 12345;
```

Disable file encryption:
```
SET_SECURITY FILE OFF;
```

### Data Files

Data is stored in CSV format under `config/` folders:
- `user.csv`: User accounts
- `post.csv`: Posts with content
- `comment.csv`: Comments and replies
- `subgroddit.csv`: Communities
- `social.csv`: Follow relationships
- `voting.csv`: Vote records
- `security.conf`: Security configuration

## Contributing

Contributions are welcome! This project is part of IF2110 Algorithm and Programming II course at Institut Teknologi Bandung.

If you'd like to contribute:
1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

Please ensure your code follows the existing style and includes appropriate documentation.

## Links

- **Course**: IF2110 Algorithm and Programming II
- **Institution**: Institut Teknologi Bandung
- **Repository**: https://github.com/Labpro-22/if2110-tubes-2025-k01-i-1
- **Documentation**: See `doc/` folder for detailed specifications

## License

This project is developed as part of academic coursework at Institut Teknologi Bandung. All rights reserved to the course instructors and student contributors.

---