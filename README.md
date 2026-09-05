# 🍄 WonderQuest

**A C++ text-based adventure through Wonderland — built from scratch with object-oriented design.**

![Language](https://img.shields.io/badge/language-C%2B%2B-blue)
![Type](https://img.shields.io/badge/type-Text--Based%20Adventure-9cf)
![Status](https://img.shields.io/badge/status-Complete-brightgreen)

---

## 📖 About

WonderQuest is a text-based adventure game inspired by *Alice's Adventures in Wonderland*, built as a course project (COMP 206) to apply object-oriented programming principles to an interactive, file-driven game world.

Rather than hard-coding the world into the program, WonderQuest reads its **locations, items, and characters** from external text files at runtime and assembles them into a live, explorable map — so the entire game world can be edited or expanded without touching a line of C++.

## ✨ Features

- 🧩 **Object-oriented architecture** — dedicated classes for locations, items, characters, and inventory keep the game logic modular and extensible
- 📂 **File-driven world model** — rooms, items, and characters are parsed from plain text files (`location.txt`, `items.txt`, `characters.txt`) into a dynamic, map-based game state
- 🗺️ **8 interconnected Wonderland locations** to explore, from the Halls of Door to the Queen's Croquet Ground and the Courtroom
- 🎒 **Command parser & action system** supporting movement, and item pickup/drop/use
- 🐇 **Iconic Wonderland characters** — the White Rabbit, the Cheshire Cat, the Caterpillar, the Queen of Hearts, and more, each with their own dialogue
- 📏 **Shrink/grow transformation mechanic** — drink or eat the right item to change size and unlock access to areas you couldn't reach before
- 🏆 **Scoring system** that tracks your progress as you collect items and make your way home

## 🗺️ World Map

```
        Duchess's Kitchen
               |
        Cheshire Cat's Wood ── Pool of Tears
               |                     |
        Halls of Door ────── Caterpillar's Mushroom
               |                     |
        The Beautiful Garden   Queen's Croquet Ground
             (home!)                 |
                              The Courtroom
```

## 🕹️ Sample Playthrough

```
> You've landed in a long, low hall lined with locked doors of every size.
> A glass table holds a bottle labelled DRINK ME and a curiously small key.

> take bottle
You picked up: DRINK ME Bottle

> drink bottle
You feel yourself shrinking...

> go west
You arrive at Caterpillar's Mushroom.
A Caterpillar sits atop it, calmly smoking a hookah.

> talk to caterpillar
Caterpillar: "Who are YOU?" he asks, blowing a slow ring of smoke.
```

## 🛠️ Tech Stack

- **Language:** C++
- **Concepts:** Object-Oriented Programming, File I/O, Dynamic Data Structures, Command Parsing

## 🚀 Getting Started

Clone the repo and compile with any standard C++ compiler:

```bash
git clone https://github.com/jai-12-cpu/wonderquest.git
cd wonderquest
g++ -std=c++17 -o wonderquest game.cpp
./wonderquest
```

## 📁 Project Structure

```
wonderquest/
├── game.cpp        # Main game source code
├── location.txt    # Room/location data
├── items.txt       # Item data
└── characters.txt  # Character data
```

## 👤 Author

**Jai Srinivasan Sivaraj**
Computer Engineering @ University of Waterloo
[Portfolio](https://jai-12-cpu.github.io/portfolio/) · [GitHub](https://github.com/jai-12-cpu) · [LinkedIn](https://linkedin.com/in/jai-srinivasan-sivaraj)
