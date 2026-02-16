# Embedded Tic-Tac-Toe AI

![Arduino](https://img.shields.io/badge/Platform-Arduino-blue.svg)
![Language](https://img.shields.io/badge/Language-C++-orange.svg)
![Display](https://img.shields.io/badge/Display-SSD1306_OLED-lightgrey.svg)
![AI](https://img.shields.io/badge/AI-Minimax-green.svg)

An embedded Tic-Tac-Toe game developed for Arduino using an SSD1306 OLED display.  
The project features a clean `GameBoard` abstraction, button-based navigation, and an AI opponent implemented with the Minimax algorithm.

This project was inspired by the [Tic-Tac-Toe AI project from CS50 AI](https://cs50.harvard.edu/ai/projects/0/tictactoe/), adapted for an embedded environment with physical controls and display.

---

## 🎥 Demonstration

![Demo](assets/demo.gif)

---
## 📦 Requirements

### Hardware
- **Arduino** (Uno, Nano, Mega, or compatible)
- **SSD1306 OLED Display** (128x64, I2C interface)
- **3x Push Buttons**
- **Jumper wires** and **pull-up resistors** (optional if using INPUT_PULLUP)

### Software
- **Arduino IDE** (version 1.8.0+)
- Adafruit Libraries:
  - `Adafruit SSD1306`
  - `Adafruit GFX Library`
  - `Wire` (I2C, usually included)

---

## 🚀 Installation

### 1️⃣ Clone the Repository
```bash
git clone https://github.com/your-username/embedded-tic-tac-toe.git
cd embedded-tic-tac-toe
```

### 2️⃣ Install Dependencies
Open **Arduino IDE** and go to:
```
Sketch → Include Library → Manage Libraries
```

Search and install:
- `Adafruit SSD1306`
- `Adafruit GFX Library`

### 3️⃣ Connect Hardware
Follow the wiring diagram in the [Hardware](#-hardware) section below.

### 4️⃣ Configure Pins (Optional)
If using different pins, edit the `.ino` file:
```cpp
#define BTN_SELECT 2
#define BTN_DOWN   3
#define BTN_RIGHT  4
```

### 5️⃣ Upload
1. Select your board: `Tools → Board → Arduino Uno` (or your board)
2. Select the port: `Tools → Port`
3. Click **Upload** (→)

---


## 🎮 Controls

| Button       | Action                       |
|--------------|------------------------------|
| **Down**     | Move selection down one row  |
| **Right**    | Move selection right one col |
| **Select**   | Confirm selected move        |

> 💡 **Tip**: Selection wraps around when reaching board edges.
---

## 🧠 Intelligence

The embedded AI uses the **Minimax algorithm** to choose optimal moves.  
For Tic-Tac-Toe, Minimax guarantees a **perfect play**, the AI will never lose. The AI evaluates all possible moves and selects the best one according to the Minimax recursion.

```
┌─────────────────────────────────────┐
│       Current Game Position         │
└────────────┬────────────────────────┘
             │
       ┌─────┴─────┐. . .
       │           │
       │           │
   ┌───▼───┐   ┌───▼───┐
   │ Move1 │   │ Move2 │  ... (explore all)
   └───┬───┘   └───┬───┘
       │           │
       │  
       • Recurse until terminal state
       • Backtrack with scores
       • Select best move
```

### Score interpretation:

| Outcome       | Score |
|---------------|-------|
| X wins        | +1    |
| O wins        | -1    |
| Draw / tie    | 0     |

 >  🧠 The algorithm **maximizes** for X (human) and **minimizes** for O (AI), ensuring the best defense.

 > 💡 To optimize performance, the AI's first move (Player O) is randomized instead of using the full Minimax algorithm. This avoids the computational overhead of exploring all 362,880 possible game states from an empty board, which would result in significant latency and repetitive gameplay. After the opening move, Minimax resumes for optimal play

---


## 🔌 Hardware

### 📍 SSD1306 Display (I2C)

Connect the OLED display using the **I2C interface**:

| OLED Pin | Connect To |
|----------|-----------|
| **VCC**  | 5V or 3.3V|
| **GND**  | GND       |
| **SDA**  | SDA (A4 on Uno) |
| **SCL**  | SCL (A5 on Uno) |

> ⚠️ **Note**: Check your display voltage. Some use 3.3V, others 5V.

### 🔘 Buttons

Buttons connected between a **digital pin** and **GND**. The project uses built-in `INPUT_PULLUP`.

| Button  | Arduino Pin | Other Terminal |
|---------|------------|----------------|
| SELECT  | D2         | GND            |
| DOWN    | D3         | GND            |
| RIGHT   | D4         | GND            |

**Logic**:
- **Button released** → HIGH (pull-up)
- **Button pressed** → LOW


---

## 💻 Code Structure

| File | Responsibility |
|------|-----------------|
| `board.h/.cpp` | Manage game state, validate moves, detect winner |
| `actions.h/.cpp` | Represent and enumerate available moves |
| `minimax.h/.cpp` | Calculate best move using Minimax |
| `ttt.ino` | Game loop, OLED rendering, input detection |

---

## 🔧 Troubleshooting

### ❌ Display Not Appearing
- Check I2C connections (SDA/SCL)
- Confirm I2C address is `0x3C` (edit in code if needed)
- Test with Arduino IDE's `Wire` example

### ❌ Buttons Not Responding
- Check connections to GND
- Verify pin is set to `INPUT_PULLUP`
- Use a multimeter to test continuity

### ❌ AI Too Slow
- Minimax is optimal but recursive. For Tic-Tac-Toe (9 cells) it's fairly fast
- For larger games, consider **memoization** or **alpha-beta pruning**

---

## 📈 Potential Improvements

- [ ] **Human match option**: Allow two humans to play.
- [ ] **Configurable Difficulty**: Add difficulty levels (easy, medium, hard)
- [ ] **Game History**: Save games to EEPROM
- [ ] **Sound/Vibration**: Tactile feedback on moves
- [ ] **Scoring**: Track wins/draws/losses
- [ ] **Alpha-Beta Pruning**: Optimize Minimax for larger games

---

## 🤝 Credits

This project was inspired by the Tic-Tac-Toe AI assignment from Harvard’s **CS50 Artificial Intelligence** course.  

Special thanks to the CS50 AI team for the fundamental algorithmic ideas:➡️ 
https://cs50.harvard.edu/ai/projects/0/tictactoe/
https://cs50.harvard.edu/ai/notes/0/

---
**Built with ❤️ by makers and embbeded AI enthusiasts.**


