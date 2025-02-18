#include <stdio.h>
#include <string.h>

// Define the user structure
struct user
 {
    int phone;
    char account[100];
    char password[100];
    float balance;
};

int main() {
    struct user us, t;
    int ch, flag = 0; 
    char password[100], accountno[100];
    int phonenumber, choice;
    float amount;
    FILE *fp, *tempFile;

   
    printf("What do you want to do?\n");
    printf("1. Register An Account\n");
    printf("2. Log In An Account\n");
    printf("Your choice: ");
    scanf("%d", &ch); 

    if (ch == 1) {
        // Register a new account
        fp = fopen("account.txt", "a"); // Open file in append mode
        if (fp == NULL) {
            printf("Error opening file!\n");
            return 1;
        }
        printf("Enter your account number: ");
        scanf("%s", us.account); 
        printf("Enter your phone number: ");
        scanf("%d", &us.phone); 
        printf("Enter your new password: ");
        scanf("%s", us.password);
        us.balance = 0; // Initialize balance to 0
        fprintf(fp, "%s\t%d\t%s\t%.2f\n", us.account, us.phone, us.password, us.balance); // Write account details to file
        fclose(fp); // Close file
        printf("Your account has been successfully created.\n");
    } 
    else if (ch == 2) {
        // Log in to an existing account
        fp = fopen("account.txt", "r"); // Open file in read mode
        tempFile = fopen("temp.txt", "w"); // Open temporary file in write mode
        if (fp == NULL || tempFile == NULL) {
            printf("Error opening file!\n");
            return 1;
        }
        printf("Enter your phone number: ");
        scanf("%d", &phonenumber); 
        printf("Enter your password: ");
        scanf("%s", password); 

        // Read account details from file
        while (fscanf(fp, "%s%d%s%f", us.account, &us.phone, us.password, &us.balance) != EOF) {
            if (us.phone == phonenumber && strcmp(password, us.password) == 0) {
                // If phone number and password match
                printf("\t\tCUSTOMER ACCOUNT BANKING MANAGEMENT SYSTEM\n");
                printf("1. Check Balance\n");
                printf("2. Deposit An Amount\n");
                printf("3. Withdraw\n");
                printf("4. Transfer Balance\n");
                printf("5. Change Password\n");
                printf("Your Choice: ");
                scanf("%d", &choice); 
                switch (choice) {
                    case 1:
                       
                        printf("Your current balance is %.2f\n", us.balance);
                        break;
                    case 2:
                        
                        printf("Enter the amount for deposit: ");
                        scanf("%f", &amount);
                        us.balance += amount;
                        printf("Amount deposited successfully.\n");
                        break;
                    case 3:
                      
                        printf("Enter the amount for withdrawal: ");
                        scanf("%f", &amount);
                        if (amount > us.balance) {
                            printf("Insufficient balance!\n");
                        } else {
                            us.balance -= amount;
                            printf("You have withdrawn Rs. %.2f\n", amount);
                        }
                        break;
                    case 4:
                        // Transfer balance
                        printf("Enter account number to transfer amount: ");
                        scanf("%s", accountno);
                        rewind(fp); // Reset file pointer to the beginning
                        while (fscanf(fp, "%s%d%s%f", t.account, &t.phone, t.password, &t.balance) != EOF) {
                            if (strcmp(accountno, t.account) == 0) {
                                printf("Enter amount to transfer: ");
                                scanf("%f", &amount);
                                if (amount > us.balance) {
                                    printf("Insufficient balance!\n");
                                } else {
                                    t.balance += amount;
                                    us.balance -= amount;
                                    printf("Transfer successful.\n");
                                }
                            }
                            fprintf(tempFile, "%s\t%d\t%s\t%.2f\n", t.account, t.phone, t.password, t.balance); // Write updated account details to temporary file
                        }
                        break;
                    case 5:
                        // Change password
                        printf("Enter your new password: ");
                        scanf("%s", password);
                        strcpy(us.password, password);
                        printf("Password changed successfully.\n");
                        break;
                    default:
                        printf("Invalid choice.\n");
                }
                flag = 1; // Set flag to indicate successful login
            }
            fprintf(tempFile, "%s\t%d\t%s\t%.2f\n", us.account, us.phone, us.password, us.balance); // Write account details to temporary file
        }
        fclose(fp);
        fclose(tempFile); 
        remove("account.txt"); 
        rename("temp.txt", "account.txt"); 

        if (flag == 0) {
            printf("Wrong Password or Account Not Found!\n");
        }
    }
    return 0;
}