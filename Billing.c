#include <stdio.h>
#include <stdlib.h>
#define MAX_ITEMS 10
#define MAX_NAME_LENGTH 20
#define MAX_ADDRESS_LENGTH 50

// Function prototypes
void displayMenu(char names[][MAX_NAME_LENGTH], float prices[], int itemCount);
void placeOrder(char names[][MAX_NAME_LENGTH], float prices[], int itemCount, char address[], char contact[]);

int main() {
    // Input menu details
    char names[MAX_ITEMS][MAX_NAME_LENGTH] = {
        "Burger", "Biryani", "Mo:Mo:", "Pasta", "Sandwich",
        "Pizza", "Fries", "Thukpa", "Sizzler", "Chopsuey"
    };
    float prices[MAX_ITEMS] = {150.0, 180.0, 120.0, 130.0, 130.0,
                               300.0, 175.0, 145.0, 350.0, 140.0};
    int itemCount = 10;
    printf("\tFoods and Delights Restaurant \n");
    printf("\tDhapakhel, Lalitpur \n");
    printf("\tContact No.: 55450987 \n\n");
    printf("Welcome to our Restaurant!\n\n");
    printf("Feel free to choose the foods you like!!\n\n");
    // Get user's address and contact
    char name[50];
    char address[50];
    char contact[10];
    printf("Enter Your Name: ");
    scanf("%s", name);
    printf("Enter your address: ");
    scanf("%s", address);
    printf("Please enter your contact number: ");
    scanf("%s", contact);
    // Main loop
    while (1) {
        printf("\n1. Display Menu\n");
        printf("2. Place Order\n");
        printf("3. Exit\n\n");
        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                displayMenu(names, prices, itemCount); // Display the menu
                break;
            case 2:
                placeOrder(names, prices, itemCount, address, contact); // Place an order
                break;
            case 3:
                printf("\nThank you for visiting us.\nWe hope you enjoyed your food. \n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}

// Function to place an order and display the bill
void placeOrder(char names[][MAX_NAME_LENGTH], float prices[], int itemCount, char address[], char contact[]) {
    int order[MAX_ITEMS] = {0};
    int choice, quantity;
    printf("\nPlacing an order:\n");
    displayMenu(names, prices, itemCount); // Display the menu here
    while (1) {
        printf("Enter the item ID to order (0 to finish): ");
        scanf("%d", &choice);
        if (choice == 0) {
            break;
        }
        if (choice < 1 || choice > itemCount) {
            printf("Invalid item ID. Please try again.\n");
            continue;
        }
        printf("Enter the quantity: ");
        scanf("%d", &quantity);
        order[choice - 1] += quantity;
    }
    // Display the final order with address and contact
    printf("\n\n\t\tYour Bill Order\n\n");
    printf("----------------------------------------\n\n");
    printf("Your Name: \t%s\n", address);
    printf("Your Address: \t%s\n", address);
    printf("Contact Number:\t%s\n\n", contact);
    printf("----------------------------------------\n");
    printf("Item\t\tQuantity\tPrice\n");
    printf("----------------------------------------\n");
    float totalBill = 0;
    for (int i = 0; i < itemCount; ++i) {
        if (order[i] > 0) {
            printf("%-20s%d\t\t%.2f\n", names[i], order[i], prices[i] * order[i]);
            totalBill += prices[i] * order[i];
        }
    }
    // Display the total amount of bill and info of the user
    printf("----------------------------------------\n");
    printf("Total Amount\t\t\t%.2f\n\n", totalBill);
}

// Function to display the menu
void displayMenu(char names[][MAX_NAME_LENGTH], float prices[], int itemCount) {
    printf("Menu:\n");
    printf("----------------------------------------\n");
    printf("ID\tItem\t\tPrice\n");
    printf("----------------------------------------\n");
    for (int i = 0; i < itemCount; ++i) {
        printf("%d\t%-20s%.2f\n", i + 1, names[i], prices[i]);
    }
    printf("----------------------------------------\n");
}
