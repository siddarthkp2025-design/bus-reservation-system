#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <emscripten.h>

#define MAX 200

/* ─── Struct ─── */
typedef struct {
    int    ticketID;
    int    routeNo;
    char   passengerName[100];
    int    seatNo;
    char   status[20];   /* "Booked" or "Cancelled" */
} Ticket;

/* ─── Global state ─── */
Ticket tickets[MAX];
int    count         = 0;
int    nextTicketID  = 1001;

/* State machine */
int step       = 0;
int menuChoice = 0;

/* Temp vars for multi-step input */
int  tempRoute;
char tempName[100];
int  tempSeat;
int  tempTicketID;

/* ─── File Handling ─── */

void saveToFile() {
    FILE *fp = fopen("bookings.dat", "wb");
    if (fp == NULL) {
        printf("Error: could not save records.\n");
        return;
    }
    fwrite(&count,        sizeof(int),    1,     fp);
    fwrite(&nextTicketID, sizeof(int),    1,     fp);
    fwrite(tickets,       sizeof(Ticket), count, fp);
    fclose(fp);
}

void loadFromFile() {
    FILE *fp = fopen("bookings.dat", "rb");
    if (fp == NULL) {
        count        = 0;
        nextTicketID = 1001;
        return;
    }
    fread(&count,        sizeof(int),    1,     fp);
    fread(&nextTicketID, sizeof(int),    1,     fp);
    fread(tickets,       sizeof(Ticket), count, fp);
    fclose(fp);
}

/* ─── Utilities ─── */

int findTicketByID(int id) {
    for (int i = 0; i < count; i++) {
        if (tickets[i].ticketID == id)
            return i;
    }
    return -1;
}

int isSeatTaken(int route, int seat) {
    for (int i = 0; i < count; i++) {
        if (tickets[i].routeNo == route &&
            tickets[i].seatNo  == seat  &&
            strcmp(tickets[i].status, "Booked") == 0)
            return 1;
    }
    return 0;
}

const char* routeLabel(int r) {
    if (r == 1) return "Chennai -> Bangalore  (346km, Rs.250, ~6hrs)";
    if (r == 2) return "Chennai -> Coimbatore (494km, Rs.200, ~8hrs)";
    if (r == 3) return "Chennai -> Madurai    (462km, Rs.150, ~7hrs)";
    return "Unknown Route";
}

const char* routeFare(int r) {
    if (r == 1) return "Rs.250";
    if (r == 2) return "Rs.200";
    if (r == 3) return "Rs.150";
    return "—";
}

/* ─── Core Operations ─── */

void bookTicket() {
    if (count >= MAX) {
        printf("Booking limit reached.\n");
        return;
    }
    if (tempRoute < 1 || tempRoute > 3) {
        printf("Invalid route number.\n");
        return;
    }
    if (tempSeat < 1 || tempSeat > 40) {
        printf("Invalid seat number. Must be 1-40.\n");
        return;
    }
    if (strlen(tempName) == 0) {
        printf("Passenger name cannot be empty.\n");
        return;
    }
    if (isSeatTaken(tempRoute, tempSeat)) {
        printf("Seat %d is already booked on Route %d.\n", tempSeat, tempRoute);
        return;
    }

    tickets[count].ticketID  = nextTicketID;
    tickets[count].routeNo   = tempRoute;
    tickets[count].seatNo    = tempSeat;
    strcpy(tickets[count].passengerName, tempName);
    strcpy(tickets[count].status, "Booked");
    count++;
    nextTicketID++;

    saveToFile();

    printf("Booking confirmed!\n");
    printf("------------------------------\n");
    printf("Ticket ID : %d\n",   tickets[count-1].ticketID);
    printf("Name      : %s\n",   tickets[count-1].passengerName);
    printf("Route     : %s\n",   routeLabel(tempRoute));
    printf("Seat No   : %d\n",   tickets[count-1].seatNo);
    printf("Fare      : %s\n",   routeFare(tempRoute));
    printf("Status    : Booked\n");
}

void cancelTicket(int id) {
    int idx = findTicketByID(id);
    if (idx == -1) {
        printf("Ticket ID %d not found.\n", id);
        return;
    }
    if (strcmp(tickets[idx].status, "Cancelled") == 0) {
        printf("Ticket %d is already cancelled.\n", id);
        return;
    }

    strcpy(tickets[idx].status, "Cancelled");
    saveToFile();

    printf("Ticket cancelled successfully.\n");
    printf("------------------------------\n");
    printf("Ticket ID : %d\n", tickets[idx].ticketID);
    printf("Name      : %s\n", tickets[idx].passengerName);
    printf("Seat No   : %d\n", tickets[idx].seatNo);
    printf("Status    : Cancelled\n");
}

void searchTicket(int id) {
    int idx = findTicketByID(id);
    if (idx == -1) {
        printf("Ticket ID %d not found.\n", id);
        return;
    }

    printf("Ticket found.\n");
    printf("------------------------------\n");
    printf("Ticket ID : %d\n", tickets[idx].ticketID);
    printf("Name      : %s\n", tickets[idx].passengerName);
    printf("Route     : %s\n", routeLabel(tickets[idx].routeNo));
    printf("Seat No   : %d\n", tickets[idx].seatNo);
    printf("Fare      : %s\n", routeFare(tickets[idx].routeNo));
    printf("Status    : %s\n", tickets[idx].status);
}

void showAllTickets() {
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(tickets[i].status, "Booked") == 0) {
            if (!found) {
                printf("Active bookings:\n");
            }
            found = 1;
            printf("------------------------------\n");
            printf("Ticket ID : %d\n", tickets[i].ticketID);
            printf("Name      : %s\n", tickets[i].passengerName);
            printf("Route     : %s\n", routeLabel(tickets[i].routeNo));
            printf("Seat No   : %d\n", tickets[i].seatNo);
            printf("Fare      : %s\n", routeFare(tickets[i].routeNo));
            printf("Status    : %s\n", tickets[i].status);
        }
    }
    if (!found) {
        printf("No active bookings found.\n");
    }
}

/* ─── State Machine (called from JS via ccall) ─── */

EMSCRIPTEN_KEEPALIVE
void processInput(char *input) {

    /* MAIN MENU */
    if (step == 0) {
        menuChoice = atoi(input);

        if (menuChoice == 1) {
            /* Book ticket: ask route */
            printf("Enter route number (1, 2, or 3):\n");
            printf("  1. Chennai -> Bangalore  Rs.250\n");
            printf("  2. Chennai -> Coimbatore Rs.200\n");
            printf("  3. Chennai -> Madurai    Rs.150\n");
            step = 1;
        }
        else if (menuChoice == 2) {
            printf("Enter Ticket ID to cancel:\n");
            step = 10;
        }
        else if (menuChoice == 3) {
            printf("Enter Ticket ID to search:\n");
            step = 20;
        }
        else if (menuChoice == 4) {
            showAllTickets();
        }
        else {
            printf("Invalid choice. Use 1-4.\n");
        }
        return;
    }

    /* ── BOOK: step 1 = route ── */
    else if (step == 1) {
        tempRoute = atoi(input);
        if (tempRoute < 1 || tempRoute > 3) {
            printf("Invalid route. Enter 1, 2, or 3.\n");
            step = 0;
            return;
        }
        printf("Enter passenger name (no spaces):\n");
        step = 2;
        return;
    }

    /* ── BOOK: step 2 = name ── */
    else if (step == 2) {
        if (strlen(input) == 0) {
            printf("Name cannot be empty.\n");
            step = 0;
            return;
        }
        strcpy(tempName, input);
        printf("Enter seat number (1-40):\n");
        step = 3;
        return;
    }

    /* ── BOOK: step 3 = seat → execute ── */
    else if (step == 3) {
        tempSeat = atoi(input);
        bookTicket();
        step = 0;
        return;
    }

    /* ── CANCEL: step 10 = ticket ID → execute ── */
    else if (step == 10) {
        tempTicketID = atoi(input);
        if (tempTicketID <= 0) {
            printf("Invalid Ticket ID.\n");
            step = 0;
            return;
        }
        cancelTicket(tempTicketID);
        step = 0;
        return;
    }

    /* ── SEARCH: step 20 = ticket ID → execute ── */
    else if (step == 20) {
        tempTicketID = atoi(input);
        if (tempTicketID <= 0) {
            printf("Invalid Ticket ID.\n");
            step = 0;
            return;
        }
        searchTicket(tempTicketID);
        step = 0;
        return;
    }
}

/* ─── Main ─── */

int main() {
    count = 0;
    nextTicketID = 1001;
    printf("BusWay Reservation System loaded.\n");
    return 0;
}
