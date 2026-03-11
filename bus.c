#include <stdio.h>
#include <string.h>

#define MAX_SEATS 40

int seats[MAX_SEATS] = {0};
char passenger[MAX_SEATS][50];

void viewRoutes() {
    printf("\nAvailable Routes:\n");
    printf("1. Chennai -> Bangalore  Fare: ₹250\n");
    printf("2. Chennai -> Coimbatore Fare: ₹200\n");
    printf("3. Chennai -> Madurai    Fare: ₹150\n");
}

void viewSeats() {
    printf("\nSeat Status:\n");

    for(int i=0;i<MAX_SEATS;i++) {
        if(seats[i] == 0)
            printf("Seat %d : Available\n", i+1);
        else
            printf("Seat %d : Booked by %s\n", i+1, passenger[i]);
    }
}

void bookSeat(int seat, char name[]) {

    if(seat < 1 || seat > MAX_SEATS) {
        printf("Invalid seat number\n");
        return;
    }

    if(seats[seat-1] == 1) {
        printf("Seat already booked\n");
        return;
    }

    seats[seat-1] = 1;
    strcpy(passenger[seat-1], name);

    printf("Seat %d successfully booked for %s\n", seat, name);
}

void cancelSeat(int seat) {

    if(seat < 1 || seat > MAX_SEATS) {
        printf("Invalid seat number\n");
        return;
    }

    if(seats[seat-1] == 0) {
        printf("Seat is not booked\n");
        return;
    }

    seats[seat-1] = 0;

    printf("Seat %d booking cancelled\n", seat);
}

void searchSeat(int seat) {

    if(seat < 1 || seat > MAX_SEATS) {
        printf("Invalid seat number\n");
        return;
    }

    if(seats[seat-1] == 1)
        printf("Seat %d booked by %s\n", seat, passenger[seat-1]);
    else
        printf("Seat %d is available\n", seat);
}

int main() {

    int choice;

    printf("BusWay WASM Reservation System Loaded\n");

    while(1) {

        printf("\nMenu\n");
        printf("1 View Routes\n");
        printf("2 Book Ticket\n");
        printf("3 Exit\n");
        printf("4 Cancel Ticket\n");
        printf("5 Search Ticket\n");
        printf("6 View Seats\n");

        scanf("%d",&choice);

        if(choice == 1) {

            viewRoutes();

        }
        else if(choice == 2) {

            int route, seat;
            char name[50];

            scanf("%d",&route);
            scanf("%s",name);
            scanf("%d",&seat);

            bookSeat(seat,name);

        }
        else if(choice == 4) {

            int seat;
            scanf("%d",&seat);

            cancelSeat(seat);
        }
        else if(choice == 5) {

            int seat;
            scanf("%d",&seat);

            searchSeat(seat);
        }
        else if(choice == 6) {

            int route;
            scanf("%d",&route);

            viewSeats();
        }
        else if(choice == 3) {

            printf("Exiting system...\n");
        }
    }

    return 0;
}