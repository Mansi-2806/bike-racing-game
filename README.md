# 🏍️ Bike Racing Game (Console-Based in C++)

A classic arcade-style bike racing game built using **C++** for the Windows console. The game offers real-time keyboard interaction, visual bike and obstacle rendering using ASCII characters, and a scoring system that increases difficulty as you progress.

---

## 🎮 Features

- 🏁 Avoid obstacles to score points
- 💥 Collision detection and lives system
- 🔁 Dynamic difficulty with level-ups
- 🚦 Colored ASCII UI with road and divider
- 📊 On-screen display of Score, Lives, and Level
- 🕹️ Real-time controls: A/D or ←/→ to move, P to pause, ESC to quit

---

## 🛠️ Tech Stack

- **Language:** C++
- **Environment:** Windows Console
- **Headers Used:** `<iostream>`, `<conio.h>`, `<windows.h>`, `<time.h>`, `<string>`

---

## 🧠 Key Concepts Used

- Procedural programming in C++
- Real-time user input with `conio.h`
- Console cursor manipulation using `gotoxy()`
- ANSI-style color rendering using `SetConsoleTextAttribute()`
- Modular function design (game loop, rendering, logic)

---

## 💻 Controls

| Key | Action         |
|-----|----------------|
| `A` / `←` | Move Left     |
| `D` / `→` | Move Right    |
| `P`       | Pause Game    |
| `ESC`     | Exit to Menu  |

---

## 🧩 Game Structure

- **`main()`**: Menu and entry point
- **`play()`**: Core game loop
- **`drawBike()`, `drawEnemy()`**: Rendering elements
- **`collision()`**: Detect crashes
- **`levelUp()`**: Speed and level progression
- **`updateScore()`**: UI updates for score/lives/level

---

## 📷 Screenshots

*(Insert screenshots here if available)*

---

## 🚀 Getting Started

### 🖥️ Run on Windows

1. Clone this repository:
   ```bash
   git clone https://github.com/<your-username>/bike-racing-game.git
   cd bike-racing-game
