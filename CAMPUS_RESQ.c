
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ROWS 8
#define COLS 8
#define MAX_RESOURCES 20
#define MAX_INCIDENTS 50

// ============================================================
// 1. STRUCTURES
// Structures group related information into a single data type.
// ============================================================

struct Resource {
    int id;
    char name[40];
    char type[20];
    int x, y;
    int available;       // 1 = available, 0 = busy
};

struct Incident {
    int id;
    char type[20];
    char description[100];
    int x, y;
    int priority;        // 1 = Low, 2 = Medium, 3 = High, 4 = Critical
    int resolved;        // 0 = Active, 1 = Resolved
};

// ============================================================
// 2. GLOBAL VARIABLES
// Arrays store multiple resources, incidents, and map cells.
// ============================================================

struct Resource resources[MAX_RESOURCES];
struct Incident incidents[MAX_INCIDENTS];

int resourceCount = 0;
int incidentCount = 0;
int nextResourceID = 1;
int nextIncidentID = 1;

// 0 represents a walkable cell; 1 represents an obstacle.
int campus[ROWS][COLS] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
};

// Variables used by the recursive route-finding function.
int visited[ROWS][COLS];
int routeX[ROWS * COLS];
int routeY[ROWS * COLS];
int routeLength = 0;

// ============================================================
// 3. FUNCTION DECLARATIONS
// ============================================================

void initializeResources(void);
void displayMenu(void);
void displayCampusMap(void);
int validLocation(int x, int y);
int readLocation(char *message);
void addResource(void);
void displayResources(void);
void reportIncident(void);
void displayIncidents(void);
void sortIncidents(void);
void findNearestResource(void);
void allocateResource(void);
void resolveIncident(void);
void generateReport(void);
void emergencySimulation(void);
void showRoute(int startX, int startY, int endX, int endY);
int findRoute(int x, int y, int targetX, int targetY);
int findResourceIndex(int id);
int findIncidentIndex(int id);
void clearInputBuffer(void);
void readText(char text[], int size);
int readInteger(char *message);

// ============================================================
// 4. INPUT HELPER FUNCTIONS
// These functions make keyboard input easier to manage.
// ============================================================

// Remove leftover characters from the input buffer.
void clearInputBuffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        // Keep reading until the end of the line.
    }
}

// Read an integer safely enough for this beginner project.
int readInteger(char *message) {
    int value;
    int result;

    while (1) {
        printf("%s", message);
        result = scanf("%d", &value);
        clearInputBuffer();

        if (result == 1) {
            return value;
        }

        printf("Invalid input. Please enter a whole number.\n");
    }
}

// Read a line of text, including spaces.
void readText(char text[], int size) {
    if (fgets(text, size, stdin) != NULL) {
        text[strcspn(text, "\n")] = '\0';
    } else {
        text[0] = '\0';
    }
}

// Check whether coordinates are inside the campus and walkable.
int validLocation(int x, int y) {
    if (x < 0 || x >= ROWS || y < 0 || y >= COLS) {
        return 0;
    }

    if (campus[x][y] == 1) {
        return 0;
    }

    return 1;
}

// Ask the user to enter valid campus coordinates.
int readLocation(char *message) {
    int x, y;

    while (1) {
        printf("%s\n", message);

        x = readInteger("Enter row (0-7): ");
        y = readInteger("Enter column (0-7): ");

        if (validLocation(x, y)) {
            return x * COLS + y;
        }

        printf("Invalid location or obstacle. Try again.\n");
    }
}

// ============================================================
// 5. INITIALIZE DEFAULT RESOURCES
// A few resources are added so the project works immediately.
// ============================================================

void initializeResources(void) {
    resources[resourceCount].id = nextResourceID++;
    strcpy(resources[resourceCount].name, "Medical Team");
    strcpy(resources[resourceCount].type, "Medical");
    resources[resourceCount].x = 0;
    resources[resourceCount].y = 0;
    resources[resourceCount].available = 1;
    resourceCount++;

    resources[resourceCount].id = nextResourceID++;
    strcpy(resources[resourceCount].name, "Fire Safety Team");
    strcpy(resources[resourceCount].type, "Fire");
    resources[resourceCount].x = 0;
    resources[resourceCount].y = 7;
    resources[resourceCount].available = 1;
    resourceCount++;

    resources[resourceCount].id = nextResourceID++;
    strcpy(resources[resourceCount].name, "Security Team");
    strcpy(resources[resourceCount].type, "Security");
    resources[resourceCount].x = 7;
    resources[resourceCount].y = 7;
    resources[resourceCount].available = 1;
    resourceCount++;
}

// ============================================================
// 6. DISPLAY CAMPUS MAP
// The map is a 2D array traversed using nested loops.
// ============================================================

void displayCampusMap(void) {
    int i, j;

    printf("\n========== CAMPUS MAP ==========\n");
    printf("    ");

    for (j = 0; j < COLS; j++) {
        printf("%d ", j);
    }

    printf("\n");

    for (i = 0; i < ROWS; i++) {
        printf("%d | ", i);

        for (j = 0; j < COLS; j++) {
            if (campus[i][j] == 1) {
                printf("# ");
            } else {
                printf(". ");
            }
        }

        printf("\n");
    }

    printf("\n. = Walkable location\n");
    printf("# = Obstacle\n");
    printf("Coordinates are written as (row, column).\n");
}

// ============================================================
// 7. RESOURCE MANAGEMENT
// Add and display response teams using a 1D array of structures.
// ============================================================

void addResource(void) {
    int location;

    if (resourceCount >= MAX_RESOURCES) {
        printf("Resource limit reached.\n");
        return;
    }

    printf("\n========== ADD RESOURCE ==========\n");

    resources[resourceCount].id = nextResourceID++;

    printf("Enter resource/team name: ");
    readText(resources[resourceCount].name,
             sizeof(resources[resourceCount].name));

    printf("Enter resource type (Medical/Fire/Security/Other): ");
    readText(resources[resourceCount].type,
             sizeof(resources[resourceCount].type));

    location = readLocation("Enter the resource's campus location");

    resources[resourceCount].x = location / COLS;
    resources[resourceCount].y = location % COLS;
    resources[resourceCount].available = 1;

    printf("Resource registered successfully. ID: %d\n",
           resources[resourceCount].id);

    resourceCount++;
}

void displayResources(void) {
    int i;

    printf("\n========== RESOURCE INVENTORY ==========\n");

    if (resourceCount == 0) {
        printf("No resources registered.\n");
        return;
    }

    printf("%-5s %-25s %-12s %-12s %-12s\n",
           "ID", "Name", "Type", "Location", "Status");

    for (i = 0; i < resourceCount; i++) {
        printf("%-5d %-25s %-12s (%d,%d)        %-12s\n",
               resources[i].id,
               resources[i].name,
               resources[i].type,
               resources[i].x,
               resources[i].y,
               resources[i].available ? "Available" : "Busy");
    }
}

// Search for a resource by its unique ID.
int findResourceIndex(int id) {
    int i;

    for (i = 0; i < resourceCount; i++) {
        if (resources[i].id == id) {
            return i;
        }
    }

    return -1;
}

// ============================================================
// 8. INCIDENT REPORTING
// A structure stores each incident and its current status.
// ============================================================

void reportIncident(void) {
    int choice;
    int location;
    struct Incident *incident;

    if (incidentCount >= MAX_INCIDENTS) {
        printf("Incident limit reached.\n");
        return;
    }

    printf("\n========== REPORT INCIDENT ==========\n");
    printf("1. Medical emergency\n");
    printf("2. Fire emergency\n");
    printf("3. Security issue\n");
    printf("4. Maintenance issue\n");

    choice = readInteger("Select incident type: ");

    incident = &incidents[incidentCount];

    switch (choice) {
        case 1:
            strcpy(incident->type, "Medical");
            incident->priority = 4;
            break;
        case 2:
            strcpy(incident->type, "Fire");
            incident->priority = 4;
            break;
        case 3:
            strcpy(incident->type, "Security");
            incident->priority = 3;
            break;
        case 4:
            strcpy(incident->type, "Maintenance");
            incident->priority = 1;
            break;
        default:
            printf("Invalid incident type.\n");
            return;
    }

    printf("Briefly describe the incident: ");
    readText(incident->description, sizeof(incident->description));

    location = readLocation("Enter the incident location");

    incident->x = location / COLS;
    incident->y = location % COLS;
    incident->id = nextIncidentID++;
    incident->resolved = 0;

    printf("\nIncident reported successfully!\n");
    printf("Incident ID: %d\n", incident->id);
    printf("Priority: %d\n", incident->priority);

    if (incident->priority == 4) {
        printf("CRITICAL: Immediate response is required.\n");
    } else if (incident->priority == 3) {
        printf("HIGH: Respond as soon as possible.\n");
    } else if (incident->priority == 2) {
        printf("MEDIUM: Response is needed.\n");
    } else {
        printf("LOW: Can be handled routinely.\n");
    }

    incidentCount++;
}

// Display all active and resolved incidents.
void displayIncidents(void) {
    int i;

    printf("\n========== INCIDENT REGISTER ==========\n");

    if (incidentCount == 0) {
        printf("No incidents have been reported.\n");
        return;
    }

    printf("%-5s %-15s %-10s %-12s %-12s\n",
           "ID", "Type", "Priority", "Location", "Status");

    for (i = 0; i < incidentCount; i++) {
        printf("%-5d %-15s %-10d (%d,%d)        %-12s\n",
               incidents[i].id,
               incidents[i].type,
               incidents[i].priority,
               incidents[i].x,
               incidents[i].y,
               incidents[i].resolved ? "Resolved" : "Active");

        printf("      Description: %s\n",
               incidents[i].description);
    }
}

// Search for an incident by its unique ID.
int findIncidentIndex(int id) {
    int i;

    for (i = 0; i < incidentCount; i++) {
        if (incidents[i].id == id) {
            return i;
        }
    }

    return -1;
}

// ============================================================
// 9. SORT INCIDENTS BY PRIORITY
// Bubble sort places higher-priority incidents first.
// ============================================================

void sortIncidents(void) {
    int i, j;
    struct Incident temp;

    if (incidentCount == 0) {
        printf("No incidents to sort.\n");
        return;
    }

    for (i = 0; i < incidentCount - 1; i++) {
        for (j = 0; j < incidentCount - i - 1; j++) {
            if (incidents[j].priority < incidents[j + 1].priority) {
                temp = incidents[j];
                incidents[j] = incidents[j + 1];
                incidents[j + 1] = temp;
            }
        }
    }

    printf("Incidents sorted from highest to lowest priority.\n");
    displayIncidents();
}

// ============================================================
// 10. FIND NEAREST AVAILABLE RESOURCE
// Manhattan distance is calculated using coordinates.
// This is a simple estimate, not an actual walking distance.
// ============================================================

void findNearestResource(void) {
    int id, index;
    int i, distance, bestDistance = 9999;
    int bestIndex = -1;

    if (incidentCount == 0) {
        printf("Report an incident first.\n");
        return;
    }

    id = readInteger("Enter incident ID: ");
    index = findIncidentIndex(id);

    if (index == -1 || incidents[index].resolved) {
        printf("Incident not found or already resolved.\n");
        return;
    }

    for (i = 0; i < resourceCount; i++) {
        if (resources[i].available == 1) {
            // Manhattan distance = horizontal + vertical distance.
            distance = abs(resources[i].x - incidents[index].x)
                     + abs(resources[i].y - incidents[index].y);

            if (distance < bestDistance) {
                bestDistance = distance;
                bestIndex = i;
            }
        }
    }

    if (bestIndex == -1) {
        printf("No available resources at the moment.\n");
        return;
    }

    printf("\nNearest available resource:\n");
    printf("ID: %d\n", resources[bestIndex].id);
    printf("Name: %s\n", resources[bestIndex].name);
    printf("Type: %s\n", resources[bestIndex].type);
    printf("Location: (%d,%d)\n",
           resources[bestIndex].x, resources[bestIndex].y);
    printf("Estimated grid distance: %d steps\n", bestDistance);

    printf("\nWould you like to view a route to this resource?\n");
    printf("1. Yes\n2. No\n");

    if (readInteger("Choice: ") == 1) {
        showRoute(incidents[index].x, incidents[index].y,
                  resources[bestIndex].x, resources[bestIndex].y);
    }
}

// ============================================================
// 11. RECURSIVE ROUTE FINDER
// Depth-first search explores neighbouring cells recursively.
// If a direction fails, the function backtracks and tries another.
// ============================================================

int findRoute(int x, int y, int targetX, int targetY) {
    int directions[4][2] = {
        {-1, 0},  // Up
        {1, 0},   // Down
        {0, -1},  // Left
        {0, 1}    // Right
    };

    int i;
    int nextX, nextY;

    // Base case: destination reached.
    if (x == targetX && y == targetY) {
        routeX[routeLength] = x;
        routeY[routeLength] = y;
        routeLength++;
        return 1;
    }

    // Reject cells outside the map, blocked cells, or visited cells.
    if (!validLocation(x, y) || visited[x][y]) {
        return 0;
    }

    visited[x][y] = 1;

    // Try each of the four possible directions.
    for (i = 0; i < 4; i++) {
        nextX = x + directions[i][0];
        nextY = y + directions[i][1];

        if (findRoute(nextX, nextY, targetX, targetY)) {
            // Save the current cell while returning from recursion.
            routeX[routeLength] = x;
            routeY[routeLength] = y;
            routeLength++;
            return 1;
        }
    }

    // No route found through this cell; backtrack.
    return 0;
}

// Prepare the route search and print the discovered path.
void showRoute(int startX, int startY, int endX, int endY) {
    int i;

    memset(visited, 0, sizeof(visited));
    routeLength = 0;

    if (!validLocation(startX, startY) ||
        !validLocation(endX, endY)) {
        printf("Invalid route coordinates.\n");
        return;
    }

    if (!findRoute(startX, startY, endX, endY)) {
        printf("No route could be found.\n");
        return;
    }

    printf("\nRoute found! Follow these coordinates:\n");

    // The recursive function records the route backwards,
    // so print it in reverse order.
    for (i = routeLength - 1; i >= 0; i--) {
        printf("(%d,%d)", routeX[i], routeY[i]);

        if (i > 0) {
            printf(" -> ");
        }
    }

    printf("\nTotal grid moves: %d\n", routeLength - 1);
}

// ============================================================
// 12. ALLOCATE A RESOURCE TO AN INCIDENT
// The selected resource becomes busy until the incident is resolved.
// ============================================================

void allocateResource(void) {
    int incidentID, resourceID;
    int incidentIndex, resourceIndex;

    incidentID = readInteger("Enter incident ID: ");
    incidentIndex = findIncidentIndex(incidentID);

    if (incidentIndex == -1) {
        printf("Incident not found.\n");
        return;
    }

    if (incidents[incidentIndex].resolved) {
        printf("This incident is already resolved.\n");
        return;
    }

    displayResources();

    resourceID = readInteger("Enter resource ID to allocate: ");
    resourceIndex = findResourceIndex(resourceID);

    if (resourceIndex == -1) {
        printf("Resource not found.\n");
        return;
    }

    if (!resources[resourceIndex].available) {
        printf("This resource is currently busy.\n");
        return;
    }

    resources[resourceIndex].available = 0;

    printf("\nResource allocated successfully!\n");
    printf("%s is responding to incident %d.\n",
           resources[resourceIndex].name, incidentID);

    showRoute(incidents[incidentIndex].x,
              incidents[incidentIndex].y,
              resources[resourceIndex].x,
              resources[resourceIndex].y);
}

// ============================================================
// 13. RESOLVE AN INCIDENT
// Resolving an incident also frees its assigned resource.
// ============================================================

void resolveIncident(void) {
    int incidentID, incidentIndex;
    int resourceID, resourceIndex;

    incidentID = readInteger("Enter incident ID to resolve: ");
    incidentIndex = findIncidentIndex(incidentID);

    if (incidentIndex == -1) {
        printf("Incident not found.\n");
        return;
    }

    if (incidents[incidentIndex].resolved) {
        printf("Incident is already resolved.\n");
        return;
    }

    incidents[incidentIndex].resolved = 1;

    printf("Incident %d marked as resolved.\n", incidentID);

    printf("Enter the ID of the resource that completed the response.\n");
    printf("Enter 0 if no resource was allocated.\n");
    resourceID = readInteger("Resource ID: ");

    if (resourceID != 0) {
        resourceIndex = findResourceIndex(resourceID);

        if (resourceIndex != -1) {
            resources[resourceIndex].available = 1;
            printf("%s is available again.\n",
                   resources[resourceIndex].name);
        } else {
            printf("Resource ID not found; no resource status was changed.\n");
        }
    }
}

// ============================================================
// 14. CAMPUS STATUS REPORT
// Matrix traversal and counters summarize the current situation.
// ============================================================

void generateReport(void) {
    int i, j;
    int active = 0, resolved = 0;
    int available = 0, busy = 0;
    int priorityCount[5] = {0, 0, 0, 0, 0};
    int obstacleCount = 0;
    int riskScore = 0;

    // Count incident statuses and priorities.
    for (i = 0; i < incidentCount; i++) {
        if (incidents[i].resolved) {
            resolved++;
        } else {
            active++;
        }

        if (incidents[i].priority >= 1 &&
            incidents[i].priority <= 4) {
            priorityCount[incidents[i].priority]++;
        }
    }

    // Count available and busy resources.
    for (i = 0; i < resourceCount; i++) {
        if (resources[i].available) {
            available++;
        } else {
            busy++;
        }
    }

    // Count obstacles by traversing the 2D campus matrix.
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            if (campus[i][j] == 1) {
                obstacleCount++;
            }
        }
    }

    // A simple illustrative risk score based on active incidents.
    // These weights are example values, not real safety standards.
    for (i = 0; i < incidentCount; i++) {
        if (!incidents[i].resolved) {
            riskScore += incidents[i].priority;
        }
    }

    printf("\n========== CAMPUS STATUS REPORT ==========\n");
    printf("Total resources: %d\n", resourceCount);
    printf("Available resources: %d\n", available);
    printf("Busy resources: %d\n", busy);

    printf("\nTotal incidents: %d\n", incidentCount);
    printf("Active incidents: %d\n", active);
    printf("Resolved incidents: %d\n", resolved);

    printf("\nIncidents by priority:\n");
    printf("Low (1): %d\n", priorityCount[1]);
    printf("Medium (2): %d\n", priorityCount[2]);
    printf("High (3): %d\n", priorityCount[3]);
    printf("Critical (4): %d\n", priorityCount[4]);

    printf("\nCampus grid: %d rows x %d columns\n", ROWS, COLS);
    printf("Walkable cells: %d\n", ROWS * COLS - obstacleCount);
    printf("Obstacle cells: %d\n", obstacleCount);
    printf("Illustrative active incident risk score: %d\n", riskScore);

    if (active == 0) {
        printf("Status: No active incidents.\n");
    } else if (priorityCount[4] > 0) {
        printf("Status: CRITICAL - check critical incidents immediately.\n");
    } else if (priorityCount[3] > 0) {
        printf("Status: HIGH PRIORITY - urgent attention needed.\n");
    } else {
        printf("Status: Active incidents require monitoring.\n");
    }
}

// ============================================================
// 15. EMERGENCY SIMULATION
// Demonstrates reporting, finding a resource, and allocating it.
// ============================================================

void emergencySimulation(void) {
    int location;
    int incidentIndex;
    int i, distance;
    int bestIndex = -1;
    int bestDistance = 9999;

    if (incidentCount >= MAX_INCIDENTS) {
        printf("Incident limit reached. Cannot run simulation.\n");
        return;
    }

    printf("\n========== EMERGENCY SIMULATION ==========\n");
    printf("Scenario: A medical emergency occurs in a campus block.\n");

    location = readLocation("Choose the simulated incident location");

    incidentIndex = incidentCount;

    incidents[incidentIndex].id = nextIncidentID++;
    strcpy(incidents[incidentIndex].type, "Medical");
    strcpy(incidents[incidentIndex].description,
           "Simulated medical emergency");
    incidents[incidentIndex].x = location / COLS;
    incidents[incidentIndex].y = location % COLS;
    incidents[incidentIndex].priority = 4;
    incidents[incidentIndex].resolved = 0;
    incidentCount++;

    printf("\nIncident created with ID %d.\n",
           incidents[incidentIndex].id);

    // Find the nearest available resource.
    for (i = 0; i < resourceCount; i++) {
        if (resources[i].available) {
            distance = abs(resources[i].x - incidents[incidentIndex].x)
                     + abs(resources[i].y - incidents[incidentIndex].y);

            if (distance < bestDistance) {
                bestDistance = distance;
                bestIndex = i;
            }
        }
    }

    if (bestIndex == -1) {
        printf("No resources are available. Incident remains active.\n");
        return;
    }

    printf("Nearest resource: %s\n", resources[bestIndex].name);
    printf("Estimated grid distance: %d\n", bestDistance);

    // Allocate the resource and show a recursive route.
    resources[bestIndex].available = 0;

    printf("Resource allocated automatically for the simulation.\n");

    showRoute(incidents[incidentIndex].x,
              incidents[incidentIndex].y,
              resources[bestIndex].x,
              resources[bestIndex].y);

    printf("\nSimulation response completed.\n");
    printf("The incident remains active until you mark it resolved.\n");
    printf("Assigned resource ID: %d\n", resources[bestIndex].id);
}

// ============================================================
// 16. MAIN MENU
// The do-while loop keeps the program running until Exit is chosen.
// The switch statement calls the selected feature.
// ============================================================

void displayMenu(void) {
    printf("\n============================================\n");
    printf("       CAMPUS RESQ - VERSION 1.0\n");
    printf(" Smart Campus Emergency Response System\n");
    printf("============================================\n");
    printf("1.  Display Campus Map\n");
    printf("2.  Register a New Resource\n");
    printf("3.  View Resource Inventory\n");
    printf("4.  Report an Incident\n");
    printf("5.  View All Incidents\n");
    printf("6.  Sort Incidents by Priority\n");
    printf("7.  Find Nearest Available Resource\n");
    printf("8.  Allocate Resource to Incident\n");
    printf("9.  Resolve an Incident\n");
    printf("10. Generate Campus Status Report\n");
    printf("11. Run Emergency Simulation\n");
    printf("0.  Exit\n");
    printf("============================================\n");
}

int main(void) {
    int choice;

    initializeResources();

    printf("Welcome to CAMPUS RESQ!\n");
    printf("Default medical, fire, and security teams are registered.\n");

    do {
        displayMenu();
        choice = readInteger("Enter your choice: ");

        switch (choice) {
            case 1:
                displayCampusMap();
                break;

            case 2:
                addResource();
                break;

            case 3:
                displayResources();
                break;

            case 4:
                reportIncident();
                break;

            case 5:
                displayIncidents();
                break;

            case 6:
                sortIncidents();
                break;

            case 7:
                findNearestResource();
                break;

            case 8:
                allocateResource();
                break;

            case 9:
                resolveIncident();
                break;

            case 10:
                generateReport();
                break;

            case 11:
                emergencySimulation();
                break;

            case 0:
                printf("Thank you for using CAMPUS RESQ. Goodbye!\n");
                break;

            default:
                printf("Invalid choice. Please select a menu option.\n");
        }

    } while (choice != 0);

    return 0;
}
