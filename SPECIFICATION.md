---

## **Project Title: “Starbound Console: A Text-Based Space Simulation Sandbox”**

---

### **Overview**
Create a **real-time space simulation and management game** rendered entirely in ASCII. You control a small ship traveling through a vast galaxy with procedural systems, trading, crew management, diplomacy, and tactical space combat.

Think: *Dwarf Fortress* meets *FTL* meets *Elite Dangerous*, but **in text** and **C++-only**.

---

### **Core Features and Specifications**

#### **1. Galaxy Simulation**
- Procedural generation of a galaxy with:
  - Star systems, planets, asteroid belts
  - Factions, space stations, anomalies
- Represent galaxy as a zoomable 2D map
  - Far zoom: `*` = star, `.` = unexplored
  - Close zoom: full ASCII-rendered solar system

#### **2. Real-Time or Tick-Based Simulation**
- Time advances in ticks
- Player can pause or speed up time
- Events (e.g., fleet movements, station events) are processed per tick

#### **3. Ship Management**
- Internal ship layout (rooms, crew, systems)
- ASCII room view (like a top-down blueprint):
  ```
  +---+---+---+
  | B | C | E |
  +---+---+---+
  | S | H | W |
  +---+---+---+
  ```
  B = Bridge, C = Crew Quarters, E = Engine, S = Storage, H = Hallway, W = Weapons

- Assign crew to stations, repair systems, etc.

#### **4. Space Travel and Navigation**
- Manual navigation or autopilot across star systems
- Hyperjump requires fuel and targeting
- Events during travel (e.g., pirates, anomalies)

#### **5. Resource System**
- Fuel, food, oxygen, energy, credits
- Ship systems consume resources (e.g., life support uses O2 & power)

#### **6. Trading System**
- Dock at stations to buy/sell:
  - Cargo (minerals, tech, black market goods)
  - Crew (hiring/firing)
  - Ship upgrades (shields, weapons, sensors)

#### **7. Tactical Combat System**
- Real-time or turn-based ASCII battles:
  - Enemy ships represented by characters (`A`, `B`, `S`, etc.)
  - Damage to systems, shields, or hull
  - Crew injuries, fires, and system malfunctions

#### **8. Diplomacy & Factions**
- Multiple procedurally generated factions
- Reputation system based on:
  - Missions, piracy, helping factions
- Event-based narrative (e.g., war breaks out, you're targeted)

#### **9. Save/Load System**
- Save galaxy state, ship, crew, and ongoing missions
- Use simple text files for storage

---

### **Stretch Features**
- **Custom Ship Designer**: Build your own ASCII ship layout
- **Alien Species Generator**: Procedural aliens with traits
- **On-Planet ASCII Exploration**: Text-rendered planetary surface missions
- **Radio Transmission System**: ASCII “radio logs” that evolve (like old text adventure flavor)

---

### **Challenges**
- ASCII GUI design for complex data
- Text-based real-time rendering
- Modular ship logic (power routing, oxygen flow, etc.)
- Simulating hundreds of background ships and events
- UI design using just text (windows, toggles, menus, etc.)

---

### **Suggested File Structure**
```
/src
  main.cpp
  galaxy.cpp / .h
  ship.cpp / .h
  crew.cpp / .h
  economy.cpp / .h
  factions.cpp / .h
  combat.cpp / .h
  renderer.cpp / .h
  input.cpp / .h
  save_system.cpp / .h
  events.cpp / .h

/data
  planets.txt
  alien_names.txt
  item_templates.txt

/saves
  universe_001.txt
```

---
