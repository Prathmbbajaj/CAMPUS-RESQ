# CAMPUS RESQ
### Smart Campus Emergency & Resource Allocation System

## Project Description

**CAMPUS RESQ** is a C-based, command-line application designed to simulate an intelligent emergency response and resource allocation system within a university campus. It provides a structured approach to managing campus emergencies by maintaining incident records, monitoring resource availability, identifying nearby response resources, and coordinating their allocation.

The system represents a campus through a two-dimensional grid, allowing locations and obstacles to be simulated. By combining basic distance calculations and recursive pathfinding, CAMPUS RESQ demonstrates how emergency response coordination can be approached through programming and algorithmic problem-solving.

## Purpose of the Project

The primary purpose of CAMPUS RESQ is to demonstrate how programming concepts can be applied to a practical campus emergency management scenario.

The project aims to:

- Improve the organization and tracking of campus emergency incidents.
- Maintain information about available emergency response resources.
- Identify the nearest available resource for a reported incident.
- Simulate route identification between different campus locations.
- Manage resource allocation and availability.
- Maintain incident records and track their resolution status.
- Demonstrate the application of fundamental C programming concepts to a real-world-inspired problem.

## Core Functionalities

### 1. Resource Management
Maintains records of campus emergency resources, including their identification numbers, names, types, and locations. It also tracks resource availability, distinguishing between available and currently occupied resources.

### 2. Incident Reporting and Management
Allows campus emergencies to be registered with relevant information such as incident ID, emergency type, description, location, and priority. Reported incidents are maintained for tracking and further processing.

### 3. Nearest Resource Identification
Identifies the nearest available resource in relation to an incident using Manhattan distance calculations. This helps simulate the selection of an appropriate response resource based on location.

### 4. Campus Grid and Route Finding
Represents the campus environment using a two-dimensional grid containing accessible locations and obstacles. A recursive pathfinding algorithm is used to search for a possible route between two locations.

### 5. Emergency Resource Allocation
Simulates the assignment of available resources to reported incidents. Resource availability is updated after allocation, ensuring that occupied resources are not treated as available for other emergencies.

### 6. Incident Resolution
Maintains the resolution status of reported emergencies. Once an incident is resolved, its assigned resource is released and marked as available again.

### 7. Emergency Simulation
Provides a simulated emergency response scenario that demonstrates the coordination of incident identification, resource selection, allocation, and response handling.

### 8. Reports and Record Management
Displays information about registered incidents and campus resources, including their current status and availability. It provides an overview of the system's records and resource allocation.

## C Programming Concepts Implemented

The project integrates several fundamental programming concepts:

- **Structures:** For organizing incident and resource information.
- **Arrays:** For storing multiple incident and resource records.
- **Two-Dimensional Arrays:** For representing the campus grid.
- **Functions:** For modular organization of different system operations.
- **Recursion:** For implementing the route-finding algorithm.
- **Searching:** For locating specific incidents and resources.
- **Sorting:** For organizing incident records.
- **Loops and Conditional Statements:** For controlling program flow and implementing decision-making.
- **Input Validation:** For handling user input and maintaining reliable operations.

## Conclusion

CAMPUS RESQ demonstrates how fundamental C programming concepts can be combined to create a structured campus emergency management simulation. Through incident reporting, resource management, distance calculation, recursive pathfinding, and resource allocation, the project illustrates the role of programming in addressing practical coordination challenges.

It serves as an academic demonstration of algorithmic thinking, structured data management, modular programming, and problem-solving through the C programming language.

---

**Project Name:** CAMPUS RESQ  
**Project Domain:** Campus Emergency Management  
**Programming Language:** C  
**Application Type:** Command-Line Application  
**Project Category:** Academic Semester Project
