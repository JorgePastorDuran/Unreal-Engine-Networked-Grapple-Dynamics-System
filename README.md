# Unreal Engine Networked Grapple Dynamics System
 
# Overview

This Unreal Engine 5.3.2 project showcases a custom movement mode implementation that enables a character to grapple and smoothly navigate towards a targeted point. The movement system is designed to allow the player character to gracefully slide off surfaces upon collision, enhancing the gameplay experience with fluid and dynamic movement mechanics.

## Features

- **Custom Grapple Movement**: A C++ extension of the `Character` and `CharacterMovementComponent` classes, introducing a novel movement mode tailored for grappling mechanics.
- **Network Replication**: The custom movement mode is fully networked and replicated, ensuring consistent gameplay experiences across server and client sessions, even under laggy conditions.
- **Client-Side Prediction**: Advanced implementation of `PhysCustom` override for seamless client-side prediction, minimizing the perception of input delays or discrepancies.
- **Seamless Multiplayer**: Whether you're hosting as the server or joining as a client, the gameplay remains unaffected, offering a smooth and uniform experience across different network setups.
- **Bonus Mechanics**: Additional features include the ability to attach to and launch from walls, adding depth and versatility to player movement.

## Installation

1. Clone this repository to your local machine.
2. Open the project in Unreal Engine 5.3.2.
3. Compile the project to ensure all C++ code is properly integrated.
4. Run the project either in the editor or package it for your desired platform.

## Usage

To experience the custom grapple movement:

1. Launch the project within Unreal Engine.
2. Enter Play mode, choosing either a server or client role.
3. Use the designated input (defined within the project settings) to aim and grapple towards points within the game world.
4. Experiment with grappling and moving across various surfaces to observe the smooth sliding mechanics and networked movement replication.

## Networking Tools

For testing under network conditions, it is recommended to use tools like `clumsy` to simulate various network scenarios, including latency and packet loss. This can help in validating the robustness of the custom movement mode under less-than-ideal network conditions.
![Jorge Pastor TEST MOB 2024-02-28 144138](https://github.com/JorgePastorDuran/Unreal-Engine-Networked-Grapple-Dynamics-System/assets/95531829/f6acb88e-8f25-43a1-a324-51ced82248e6)
