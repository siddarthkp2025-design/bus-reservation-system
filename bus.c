#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SEATS 40

struct Booking {
    int ticketID;
    int routeID;
    char name[50];
    int seat;
};

int ticketCounter = 1001;

void showMenu() {
    printf("\n===== BUS RESERVATION SYSTEM =====\n");
    printf("1. View Routes\n");
    printf("2. Book Ticket\n");
    printf("3. Exit\n");
}

void showRoutes() {
    printf("\nAvailable Routes:\n");
    printf("1. Chennai -> Bangalore\n");
    printf("2. Chennai -> Coimbatore\n");
    printf("3. Chennai -> Madurai\n");
}

void bookTicket() {

    struct Booking b;

    printf("Enter Route ID (1-3): ");
    fflush(stdout);
    scanf("%d",&b.routeID);

    printf("Enter Passenger Name: ");
    fflush(stdout);
    scanf("%s",b.name);

    printf("Enter Seat Number (1-40): ");
    fflush(stdout);
    scanf("%d",&b.seat);

    if(b.seat < 1 || b.seat > MAX_SEATS){
        printf("Invalid seat number\n");
        return;
    }

    b.ticketID = ticketCounter++;

    FILE *fp = fopen("bookings.txt","a");

    if(fp == NULL){
        printf("File error\n");
        return;
    }

    fprintf(fp,"%d %d %s %d\n",
            b.ticketID,
            b.routeID,
            b.name,
            b.seat);

    fclose(fp);

    printf("Booking Successful!\n");
    printf("Ticket ID: %d\n", b.ticketID);
}

int main() {

    int choice;

    while(1){

        showMenu();

        printf("Enter choice: ");
        fflush(stdout);
        scanf("%d",&choice);

        switch(choice){

            case 1:
                showRoutes();
                break;

            case 2:
                bookTicket();
                break;

            case 3:
                printf("Exiting...\n");
                exit(0);

            default:
                printf("Invalid choice\n");
        }
    }

    return 0;
}