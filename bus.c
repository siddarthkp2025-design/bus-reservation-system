#include <stdio.h>
#include <string.h>

#define MAX_SEATS 40

int seats[MAX_SEATS] = {0};
char passenger[MAX_SEATS][50];

void viewRoutes() {

    printf("Available Routes:\n");
    printf("1. Chennai -> Bangalore\n");
    printf("2. Chennai -> Coimbatore\n");
    printf("3. Chennai -> Madurai\n");
    printf("4. Chennai -> Trichy\n");
}

void viewSeats() {

    printf("Seat Status:\n");

    for(int i=0;i<MAX_SEATS;i++)
    {
        if(seats[i] == 0)
        {
            printf("Seat %d : Available\n", i+1);
        }
        else
        {
            printf("Seat %d : Booked by %s\n", i+1, passenger[i]);
        }
    }
}

void bookSeat(int seat, const char* name)
{

    if(seat < 1 || seat > MAX_SEATS)
    {
        printf("Invalid seat number\n");
        return;
    }

    if(seats[seat-1] == 1)
    {
        printf("Seat already booked\n");
        return;
    }

    seats[seat-1] = 1;
    strcpy(passenger[seat-1], name);

    printf("Seat %d successfully booked for %s\n", seat, name);
}

void cancelSeat(int seat)
{

    if(seat < 1 || seat > MAX_SEATS)
    {
        printf("Invalid seat number\n");
        return;
    }

    if(seats[seat-1] == 0)
    {
        printf("Seat is not booked\n");
        return;
    }

    seats[seat-1] = 0;

    printf("Seat %d booking cancelled\n", seat);
}

void searchSeat(int seat)
{

    if(seat < 1 || seat > MAX_SEATS)
    {
        printf("Invalid seat number\n");
        return;
    }

    if(seats[seat-1] == 1)
    {
        printf("Seat %d booked by %s\n", seat, passenger[seat-1]);
    }
    else
    {
        printf("Seat %d is available\n", seat);
    }
}

int main()
{
    printf("Bus Reservation System Loaded\n");
    printf("Use the website buttons to operate the system.\n");

    return 0;
}