<img width="866" height="244" alt="imagen" src="https://github.com/user-attachments/assets/268e0820-a651-4737-904b-f72696e6e8ce" />

# Termidoro

A pomodoro timer that runs in your terminal, written in C.

## Features

- Terminal-based countdown timer.
- Customizable Pomodoro duration.
- Saves your timer between sessions.
- Lightweight ASCII art cuz I like it :b

## Requirements

- GCC or another compatible C compiler.
- A terminal environment.

## Installation

Clone the repository:

```bash
git clone https://github.com/leookernel/termidoro.git
cd termidoro
```

## Compilation

Compile the source code using GCC:

Linux / macOS

```bash
gcc main.c -o termidoro
```

Windows

```bash
gcc main.c -o termidoro.exe
```

## Getting Started

When launching Termidoro for the first time:

1. Select Settings to configure your Pomodoro duration.
2. Start Pomodoro to begin your session.
3. Exit when you're done :D

Your configured duration is automatically saved and reused in future sessions.

## Project structure

```
termidoro/
├── main.c
├── assets/
│   └── treeart.txt
├── data/ # Created automatically
│   └── settings.dat # Generated at runtime
├── README.md
└── .gitignore
```
