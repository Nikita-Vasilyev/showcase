# Nikita-Vasilyev/showcase

This repository is a collection of diverse projects, each demonstrating different aspects of software and hardware development. Below is a summary of each main project directory and its purpose.

---

## Projects Overview

### 391OS
A simplistic Unix-based operating system written in C and x86 Assembly over 8 weeks.  
**Summary:**  
- Implements fundamental OS features: General Descriptor Table (GDT), Interrupt Descriptor Table (IDT), paging (virtual memory), process management, task scheduling, and device drivers (keyboard, RTC, PIT, terminal).
- Contains file systems and system call code (see `student-distrib/` for core source).
- Collaboratively built by a team of three, featuring process execution, concurrent processes, and shell management.

### AARG (Audio Augmented Reality Glasses)
Senior design capstone project creating Audio Augmented Reality Glasses.  
**Summary:**  
- Wearable glasses capture images and send them for backend recognition, returning an audio explanation.
- Focuses on accessibility (e.g., for visually impaired users), digital tour guides, and other AR applications.
- Features include fast computation, high accuracy, and lightweight construction.
- Final prototype: image capture, backend object identification, and audio feedback implemented.  
[Read more in the directory's README](https://github.com/Nikita-Vasilyev/showcase/blob/main/AARG/README.md).

### Blackjack
A fully functional Blackjack game programmed in Java.  
**Summary:**  
- Implements standard Blackjack rules and logic (player, dealer, deck, hands, and betting logic).
- Features include chip management, hit/stand/double down decisions, and game loop for multiple rounds.
- Includes test classes for hand and player logic.

### bestbuyBot
A bot intended to purchase an out-of-stock item when it comes in stock on BestBuy.com, using Python.  
**Summary:**  
- Uses Selenium and ChromeDriver to automate browser actions.
- Monitors a product page and attempts to purchase the product as soon as it becomes available.
- Automates login, cart management, checkout, and payment entry.

### computerSystemsAndProgramming
Collection of coursework projects from the UIUC Computer Systems and Programming course.  
**Summary:**  
- Contains multiple mini-projects (mp1–mp12) covering assembly, C, and C++.
- Projects likely span systems programming fundamentals, data structures, algorithms, and OS topics.
- [See directory](https://github.com/Nikita-Vasilyev/showcase/tree/main/computerSystemsAndProgramming)

### occupancyDetector
An Internet of Things (IoT) device to determine whether an observed area is occupied.  
**Summary:**  
- Utilizes Python and Wireshark for collecting wireless packet data (MAC address, Rx power).
- Processes and analyzes data using an optimized K-Means algorithm to establish room occupancy.
- Designed for Raspberry Pi 3; includes scripts for network setup, packet capture, and data analysis.

### pizzaTime
A network-based pizza ordering service with a client and server using Java.  
**Summary:**  
- Allows clients to place pizza orders and servers to create pizzas based on requests.
- Features a GUI for topping selection and pizza creation.
- Includes instructions for running server/client and interacting through the application.

### waterandlava
A hardware/software project recreating the flash game "Fireboy and Watergirl" on FPGA hardware.  
**Summary:**  
- Developed for the Urbana Board’s Xilinx Spartan-7 XC7S50-CSGA324 FPGA.
- Features 3 levels with collectible stars, pressure plate puzzles, and water/lava interactions.
- Includes Vivado project files, HDL sources, and a hardware design archive.
- [Read more in the directory's README](https://github.com/Nikita-Vasilyev/showcase/blob/main/waterandlava/README.md).

---

## License

Distributed under the MIT License.