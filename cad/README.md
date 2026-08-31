# ShotBot CAD

This directory contains the mechanical CAD developed for **ShotBot**, a stair-climbing delivery robot built as part of the Stanford Moonshot Club.

The CAD was created primarily in Autodesk Fusion 360. Native Fusion design files and STEP exports are included where appropriate to make the models accessible across different CAD platforms.

## Main Assembly Render

![ShotBot main assembly render](Renders/Main.png)

The render above shows the primary ShotBot mechanical assembly, including the drivetrain, tread system, chassis, and stair-climbing mechanisms.

## Directory Contents

### Main Assembly

The assembly files contain the primary mechanical configuration of ShotBot and are intended to show how the major structural, drivetrain, and stair-climbing components integrate into the complete robot.

### Custom Parts

The `Custom_parts/` directory contains the relevant custom mechanical components developed during the ShotBot project.

This directory includes:

- Parts used in the final or most recent ShotBot configuration
- Earlier design iterations that were later revised or replaced

The older components are retained to document the mechanical development process and provide context for how the design evolved over the course of the project.

Some components may therefore not appear in the final assembly.

### LiDAR Mount

The custom-parts directory also contains a preliminary mount designed for a planned LiDAR sensor.

The LiDAR system was considered as part of a future sensing and navigation architecture, but the mount and associated system were **not fully implemented due to the scope and timeline of the project**. The CAD is included as a record of the intended future development.

### 3D-Printed Tread System

ShotBot's custom 3D-printed tread design was inspired by **James Bruton’s 3D-printed tank tread designs**.

The concept was adapted for ShotBot's mechanical requirements and stair-climbing application. The resulting ShotBot tread components were designed as part of the robot's custom drivetrain and mobility system.

## Renders

The `Renders/` directory contains additional views of the ShotBot assembly and major mechanical systems. These renders are provided to make the design easier to inspect without requiring CAD software.

Available views include:

- `Front.png`
- `Isometric.png`
- `Main.png`
- `Side.png`
- `Top.png`

## Third-Party Components

The original ShotBot development assembly incorporated externally sourced CAD models for commercial off-the-shelf components such as motors, bearings, electronics, and other hardware.

The CAD in this repository is organized to emphasize the components and mechanical systems developed specifically for ShotBot. Third-party component models should not be interpreted as original ShotBot designs.

## Project Status

ShotBot reached the prototype stage as a remotely controlled delivery robot capable of stair-climbing.

Several additional capabilities, including LiDAR-based sensing and more advanced navigation, were explored during development but were not fully implemented within the project's scope.