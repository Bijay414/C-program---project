
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the Account structure
struct Account 
{
    int accountNumber;
    char name[100];
    float balance;
};

// Function Declearation
void createAccount(struct Account *account);
void deposit(struct Account *account);
void withdraw(struct Account *account);
void checkBalance(struct Account *account);

int main() {
    struct Account account; // Declare an Account variable
    int choice; // Variable to store user choice

    while (1) { // Infinite loop to keep the program running
        // Display menu options
        printf("\nBank Management System\n");
        printf("1. Create Account\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Check Balance\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice); // Read user choice

        // Perform action based on user choice
        switch (choice) {
            case 1:
                createAccount(&account); // Create a new account
                break;
            case 2:
                deposit(&account); // Deposit money into the account
                break;
            case 3:
                withdraw(&account); // Withdraw money from the account
                break;
            case 4:
                checkBalance(&account); // Check the account balance
                break;
            case 5:
                exit(0); // Exit the program
            default:
                printf("Invalid choice! Please try again.\n"); // Handle invalid choices
        }
    }

    return 0; // Return 0 to indicate successful execution
}

// Function to create a new account
void createAccount(struct Account *account) {
    printf("Enter account number: ");
    scanf("%d", &account->accountNumber); // Read account number
    printf("Enter account holder name: ");
    scanf("%s", account->name); // Read account holder name
    //-> is used to access the structure members through a pointer
    account->balance = 0.0; // Initialize balance to 0
    printf("Account created successfully!\n");
}

// Function to deposit money into the account
void deposit(struct Account *account) {
    float amount;
    printf("Enter amount to deposit: ");
    scanf("%f", &amount); // Read deposit amount
    account->balance += amount; // Add amount to balance
    printf("Amount deposited successfully!\n");
}

// Function to withdraw money from the account
void withdraw(struct Account *account) {
    float amount;
    printf("Enter amount to withdraw: ");
    scanf("%f", &amount); // Read withdrawal amount
    if (amount > account->balance) { // Check if sufficient balance is available
        printf("Insufficient balance!\n");
    } else {
        account->balance -= amount; // Subtract amount from balance
        printf("Amount withdrawn successfully!\n");
    }
}

// Function to check the account balance
void checkBalance(struct Account *account) {
    printf("Account number: %d\n", account->accountNumber); // Display account number
    printf("Account holder name: %s\n", account->name); // Display account holder name
    printf("Account balance: %.2f\n", account->balance); // Display account balance
}