#include <stdio.h>
#include <string.h>
#include <emscripten.h>
#ifndef EMSCRIPTEN_KEEPALIVE
#define EMSCRIPTEN_KEEPALIVE
#endif
#define MAX_SEATS 40

int seats[MAX_SEATS];
int ticketIDs[MAX_SEATS];
int currentTicket = 1000;


void initSystem() {
    for (int i = 0; i < MAX_SEATS; i++) {
        seats[i] = 0;
        ticketIDs[i] = 0;
    }
}

EMSCRIPTEN_KEEPALIVE
int* getSeats() {
    return seats;
}

EMSCRIPTEN_KEEPALIVE
int bookSeat(int seat, const char* name) {
    if (seat < 1 || seat > 40) return -1;
    if (seats[seat - 1] == 1) return -2;

    seats[seat - 1] = 1;
    ticketIDs[seat - 1] = currentTicket++;

    return ticketIDs[seat - 1];
}

EMSCRIPTEN_KEEPALIVE
int cancelSeat(int ticketID) {
    for (int i = 0; i < MAX_SEATS; i++) {
        if (ticketIDs[i] == ticketID) {
            seats[i] = 0;
            ticketIDs[i] = 0;
            return 1;
        }
    }
    return -1;
}

EMSCRIPTEN_KEEPALIVE
int searchSeat(int ticketID) {
    for (int i = 0; i < MAX_SEATS; i++) {
        if (ticketIDs[i] == ticketID) {
            return i + 1;
        }
    }
    return -1;
}
int main() {
    initSystem();
    return 0;
}