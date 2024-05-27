#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max_movies 10
#define max_name_length 50
#define max_phone_length 15
#define max_rows 10
#define max_cols 10

void load_movies();
void display_movies();
void display_seating();
void book_ticket();
void cancel_ticket();
void display_billing(struct booking booking);

struct movie {
    char name[max_name_length];
    char time[10];
    float price;
};

struct booking {
    int booking_id;
    char movie[max_name_length];
    char name[max_name_length];
    char phone[max_phone_length];
    int seat_row;
    int seat_col;
};

struct movie movies[max_movies];
struct booking bookings[max_rows][max_cols];
int num_movies = 0;
float movie_price = 0.0;
char seats[max_rows][max_cols];

void load_movies() {
    FILE *fp = fopen("movies.txt", "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    while (fscanf(fp, "%s %s %f", movies[num_movies].name, movies[num_movies].time, &movies[num_movies].price) ==3) {
        num_movies++;
    }

    fclose(fp);
}

void initialize_seats() {
    for (int i = 0; i < max_rows; i++) {
        for (int j = 0; j < max_cols; j++) {
            seats[i][j] = '-';
        }
    }
}

void display_movies() {
    printf("Available Movies:\n");
    for (int i = 0; i < num_movies; i++) {
        printf("%d. %s\t\t [%s]\t\t- Rs%.2f\n", i + 1, movies[i].name, movies[i].time, movies[i].price);
    }
}

void display_seating() {
    printf("Seating arrangement:\n");
    printf("  ");
    for (int j = 0; j < max_cols; j++) {
        printf("\t%2d", j + 1);
    }
    printf("\n");
    for (int i = 0; i < max_rows; i++) {
        printf(" (ROW)%2d", i + 1);
        for (int j = 0; j < max_cols; j++) {
            printf(" %c\t", seats[i][j]);
        }
        printf("\n");
    }
}

void book_ticket() {
    display_movies();
    int choice;
    printf("Enter movie choice: ");
    scanf("%d", &choice);
    choice--; 
    
    if (choice < 0 || choice >= num_movies) {
        printf("Invalid movie choice!\n");
        return;
    }

    int num_tickets;
    printf("Enter the number of tickets to book: ");
    scanf("%d", &num_tickets);
    struct booking new_booking;
    printf("Enter your name  :");
    scanf("%s", new_booking.name);
    printf("Enter your phone number : ");
    scanf("%s", new_booking.phone);
    
    for (int i = 0; i < num_tickets; i++) {
        display_seating();
        printf("Enter seat row for ticket %d: ", i + 1);
        int row, col;
        scanf("%d", &row);
        printf("Enter seat column for ticket %d: ", i + 1);
        scanf("%d", &col);
        
        if (row < 1 || row > max_rows || col < 1 || col > max_cols) {
            printf("Invalid seat selection. Please choose a seat within the valid range.\n");
            i--; 
            continue;
        }

        if (seats[row - 1][col - 1] == 'x') {
            printf("Seat already booked. Please choose another seat.\n");
            i--; // Decrement i to rebook the same ticket
            continue;
        }
        
        new_booking.booking_id = 1000 + i; // Unique booking ID for each ticket
        strcpy(new_booking.movie, movies[choice].name);
        seats[row - 1][col - 1] = 'x';
        new_booking.seat_row = row;
        new_booking.seat_col = col;
        bookings[row - 1][col - 1] = new_booking;
        display_seating();
        display_billing(new_booking);
        // Placeholder for storing booking details in file
    }
}

void cancel_ticket() {
    int id;
    printf("Enter booking ID to cancel: ");
    scanf("%d", &id);

    // Find the booking with the given ID and mark its seat as available
    for (int i = 0; i < max_rows; i++) {
        for (int j = 0; j < max_cols; j++) {
            if (bookings[i][j].booking_id == id) {
                printf("Ticket with booking ID %d canceled successfully.\n", id);
                seats[i][j] = '-';
                display_seating();
                movie_price = movie_price - movies[j].price;
                return;
            }
        }
    }
    printf("Ticket with booking ID %d not found.\n", id);
}

void display_billing(struct booking booking){
    printf("\n--- Ticket ---\n");
    printf("Booking ID: %d\n", booking.booking_id);
    printf("Movie: %s\n", booking.movie);
    printf("Name: %s\n", booking.name);
    printf("Phone: %s\n", booking.phone);
    printf("Seat: Row %d, Col %d\n", booking.seat_row, booking.seat_col);
    
    for (int i = 0; i < num_movies; i++) {
        if (strcmp(movies[i].name, booking.movie) == 0) {
            movie_price = movie_price + movies[i].price;
            printf("Total Amount: %f\n", movie_price);
        }
    }
    printf("---------------\n");
}

int main() {
    printf("\n\n\n");
    printf("                         WELCOME TO MOVIE TICKET BOOKING \n");
    printf("                                     ONLINE  \n\n");
	
    load_movies();
    initialize_seats();

    int choice;
    do {
        printf("\nMain Menu\n");
        printf("1. Book a ticket\n");
        printf("2. Cancel a ticket\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                book_ticket();
                break;
            case 2:
                cancel_ticket();
                break;
            case 3:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice!\n");
                
        }
    } while (choice != 3);

    return 0;
}

