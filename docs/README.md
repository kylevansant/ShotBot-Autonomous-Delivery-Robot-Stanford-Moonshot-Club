# ShotBot Project Documentation

This directory contains supporting project-level documentation for **ShotBot**, a stair-climbing delivery robot developed as part of the Stanford Moonshot Club.

Detailed mechanical, electrical, and firmware documentation is maintained in their respective directories. This folder contains broader project records, including procurement information, historical planning material, and technical references that informed the design.

---

## Bill of Materials and Project Costs

[`Shotbot_BOM.xlsx`](Shotbot_BOM.xlsx)

This spreadsheet contains component procurement and cost records accumulated throughout ShotBot development.

It includes:

- Major mechanical and electrical components
- Component quantities and purchase costs
- Supplier and product references
- Prototype cost tracking
- Components used during earlier design iterations
- Components considered for future revisions

Because the spreadsheet was maintained throughout the development process, some listed components were replaced, removed from the final design, or never incorporated into the completed prototype.

The document is retained as a record of the project's hardware selection, procurement, and iterative development process.

---

## Original SCARD Project Pitch

[`scard_original_project_pitch.pdf`](scard_original_project_pitch.pdf)

This presentation reflects the team's original project concept and intended scope during the early development of ShotBot.

The project was initially proposed as **SCARD — Stair Climbing Autonomous Robot Delivery**. The original concept included planned capabilities such as:

- Autonomous navigation
- LiDAR-based environmental sensing
- Camera integration
- Stair climbing
- Tracked locomotion
- Item transportation

As development progressed, the project scope was narrowed to prioritize the mechanical stair-climbing system, electrical integration, and completion of a reliable working prototype.

The completed ShotBot platform therefore used **Bluetooth teleoperation rather than autonomous navigation**.

The presentation is retained as historical documentation of the project's original goals and design direction and should not be interpreted as a description of the final implemented system.

---

## Design References

The following literature was used during development to evaluate existing approaches to stair-climbing robotics and inform the mechanical architecture of ShotBot.

### Stair-Climbing Robot Mechanisms

[`References/stair_climbing_robots_review.pdf`](References/stair_climbing_robots_review.pdf)

**T. Seo, S. Ryu, J. H. Won, Y. Kim, and H. S. Kim,  
"Stair-climbing Robots: a Review on Mechanism, Sensing, and Performance Evaluation,"  
IEEE Access, 2023.  
DOI: 10.1109/ACCESS.2023.3286871**

This review surveys several major classes of stair-climbing robots, including:

- Tracked robots
- Legged robots
- Wheel-linkage robots
- Wheel-legged robots

The paper was particularly relevant to the development of ShotBot's **tracked stair-climbing architecture**.

Its discussion of one-body tracked robots with articulated flippers helped inform the decision to use a front articulated mechanism in conjunction with the main tracks. The reviewed mechanisms demonstrate how flippers can increase track contact with stairs, assist with transitions onto and off of staircases, and reduce the risk of tip-over during stair traversal.

The ShotBot design was subsequently adapted around the project's own mechanical requirements, available actuators, manufacturing methods, and prototype constraints.

This reference is included to document the prior research that informed the team's mechanical design decisions rather than as work produced by the ShotBot team.

The included author version is distributed under the **Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License** and is retained unmodified. :contentReference[oaicite:0]{index=0}

---

## Project Evolution

ShotBot changed significantly between the initial proposal and the completed prototype.

The general project progression was:

```text
Original SCARD Concept
        │
        │  Autonomous stair-climbing delivery robot
        │
        ▼
Mechanical Architecture Development
        │
        │  Tracked locomotion + articulated stair mechanism
        │
        ▼
Electrical Prototyping
        │
        │  Breadboard testing + ESP32 motor control
        │
        ▼
Custom PCB Integration
        │
        │  Altium-designed interconnect PCB
        │
        ▼
Final ShotBot Prototype
        │
        │  Bluetooth teleoperation
        │  Independent motor control
        │  Functional stair climbing
        ▼
Working Prototype
```

Autonomous navigation, LiDAR integration, and camera-based perception remained part of the original design direction but were not implemented in the final version due to project scope.

The project instead focused on successfully demonstrating the mechanical and electrical systems required for a remotely operated stair-climbing delivery platform.

---

