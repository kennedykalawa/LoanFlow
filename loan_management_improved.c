#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <math.h>

// Configuration
#define MAX_LOANS 100
#define MAX_NAME_LENGTH 50
#define FILENAME "loans.txt"
#define EXPORT_FILENAME "loans_export.csv"

// Loan status enumeration
typedef enum {
    PENDING,
    APPROVED,
    REJECTED,
    COMPLETED
} LoanStatus;

// Enhanced loan structure
typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    int age;
    float principal;           // Original loan amount
    float interest_rate;       // Annual interest rate
    int duration_months;
    float total_amount;        // Principal + interest
    float balance;             // Remaining balance
    LoanStatus status;
    char date_issued[11];      // YYYY-MM-DD format
    float monthly_payment;     // EMI
} Loan;

// Function prototypes
void clearInputBuffer();
int getValidInteger(const char* prompt, int min, int max);
float getValidFloat(const char* prompt, float min, float max);
void getValidString(const char* prompt, char* buffer, int max_length);
void calculateLoanDetails(Loan* loan);
float calculateEMI(float principal, float rate, int months);
void saveToFile(Loan loans[], int count);
int loadFromFile(Loan loans[]);
void exportToCSV(Loan loans[], int count);
void displayLoan(Loan* loan);
void displayAllLoans(Loan loans[], int count);
void addLoan(Loan loans[], int* count);
void searchLoan(Loan loans[], int count);
void makeRepayment(Loan loans[], int count);
void updateLoanStatus(Loan loans[], int count);
void showStatistics(Loan loans[], int count);
void showPendingBalances(Loan loans[], int count);
const char* getStatusString(LoanStatus status);
void getCurrentDate(char* buffer);
int getNextId(Loan loans[], int count);

// Utility function implementations
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int getValidInteger(const char* prompt, int min, int max) {
    int value;
    int valid = 0;
    
    while (!valid) {
        printf("%s", prompt);
        if (scanf("%d", &value) != 1) {
            printf("Error: Please enter a valid number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();
        
        if (value < min || value > max) {
            printf("Error: Value must be between %d and %d.\n", min, max);
            continue;
        }
        valid = 1;
    }
    return value;
}

float getValidFloat(const char* prompt, float min, float max) {
    float value;
    int valid = 0;
    
    while (!valid) {
        printf("%s", prompt);
        if (scanf("%f", &value) != 1) {
            printf("Error: Please enter a valid number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();
        
        if (value < min || value > max) {
            printf("Error: Value must be between %.2f and %.2f.\n", min, max);
            continue;
        }
        valid = 1;
    }
    return value;
}

void getValidString(const char* prompt, char* buffer, int max_length) {
    int valid = 0;
    
    while (!valid) {
        printf("%s", prompt);
        if (fgets(buffer, max_length, stdin) == NULL) {
            printf("Error reading input. Please try again.\n");
            continue;
        }
        
        // Remove trailing newline
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
            len--;
        }
        
        // Check if non-empty and contains non-whitespace
        int has_content = 0;
        for (size_t i = 0; i < len; i++) {
            if (!isspace((unsigned char)buffer[i])) {
                has_content = 1;
                break;
            }
        }
        
        if (!has_content) {
            printf("Error: Name cannot be empty.\n");
            continue;
        }
        valid = 1;
    }
}

void getCurrentDate(char* buffer) {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    strftime(buffer, 11, "%Y-%m-%d", tm_info);
}

int getNextId(Loan loans[], int count) {
    int max_id = 0;
    for (int i = 0; i < count; i++) {
        if (loans[i].id > max_id) {
            max_id = loans[i].id;
        }
    }
    return max_id + 1;
}

const char* getStatusString(LoanStatus status) {
    switch(status) {
        case PENDING: return "Pending";
        case APPROVED: return "Approved";
        case REJECTED: return "Rejected";
        case COMPLETED: return "Completed";
        default: return "Unknown";
    }
}

// Calculate EMI using formula: EMI = P × r × (1 + r)^n / ((1 + r)^n - 1)
// where P = principal, r = monthly rate, n = number of months
float calculateEMI(float principal, float rate, int months) {
    if (rate == 0) {
        return principal / months;
    }
    
    float monthly_rate = rate / 100.0 / 12.0;
    float numerator = principal * monthly_rate * pow(1 + monthly_rate, months);
    float denominator = pow(1 + monthly_rate, months) - 1;
    
    return numerator / denominator;
}

void calculateLoanDetails(Loan* loan) {
    loan->monthly_payment = calculateEMI(loan->principal, loan->interest_rate, loan->duration_months);
    loan->total_amount = loan->monthly_payment * loan->duration_months;
    loan->balance = loan->total_amount;
}

// File operations
void saveToFile(Loan loans[], int count) {
    FILE* file = fopen(FILENAME, "w");
    if (!file) {
        printf("Error: Cannot save to file.\n");
        return;
    }
    
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d|%s|%d|%.2f|%.2f|%d|%.2f|%.2f|%d|%s|%.2f\n",
                loans[i].id,
                loans[i].name,
                loans[i].age,
                loans[i].principal,
                loans[i].interest_rate,
                loans[i].duration_months,
                loans[i].total_amount,
                loans[i].balance,
                loans[i].status,
                loans[i].date_issued,
                loans[i].monthly_payment);
    }
    fclose(file);
    printf("✓ Data saved successfully.\n");
}

int loadFromFile(Loan loans[]) {
    FILE* file = fopen(FILENAME, "r");
    if (!file) {
        printf("No previous data found. Starting fresh.\n");
        return 0;
    }
    
    int count = 0;
    int status_int;
    while (fscanf(file, "%d|%49[^|]|%d|%f|%f|%d|%f|%f|%d|%10[^|]|%f\n",
                  &loans[count].id,
                  loans[count].name,
                  &loans[count].age,
                  &loans[count].principal,
                  &loans[count].interest_rate,
                  &loans[count].duration_months,
                  &loans[count].total_amount,
                  &loans[count].balance,
                  &status_int,
                  loans[count].date_issued,
                  &loans[count].monthly_payment) == 11) {
        loans[count].status = (LoanStatus)status_int;
        count++;
        if (count >= MAX_LOANS) break;
    }
    fclose(file);
    printf("✓ Loaded %d loan records.\n", count);
    return count;
}

void exportToCSV(Loan loans[], int count) {
    FILE* file = fopen(EXPORT_FILENAME, "w");
    if (!file) {
        printf("Error: Cannot create export file.\n");
        return;
    }
    
    // CSV Header
    fprintf(file, "ID,Name,Age,Principal,Interest Rate,Duration (months),Total Amount,Balance,Status,Date Issued,Monthly Payment\n");
    
    // Data rows
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d,\"%s\",%d,%.2f,%.2f,%d,%.2f,%.2f,%s,%s,%.2f\n",
                loans[i].id,
                loans[i].name,
                loans[i].age,
                loans[i].principal,
                loans[i].interest_rate,
                loans[i].duration_months,
                loans[i].total_amount,
                loans[i].balance,
                getStatusString(loans[i].status),
                loans[i].date_issued,
                loans[i].monthly_payment);
    }
    fclose(file);
    printf("✓ Data exported to %s\n", EXPORT_FILENAME);
}

// Display functions
void displayLoan(Loan* loan) {
    printf("\n┌─────────────────────────────────────────┐\n");
    printf("│ Loan ID: %-31d│\n", loan->id);
    printf("├─────────────────────────────────────────┤\n");
    printf("│ Name:            %-23s│\n", loan->name);
    printf("│ Age:             %-23d│\n", loan->age);
    printf("│ Status:          %-23s│\n", getStatusString(loan->status));
    printf("│ Date Issued:     %-23s│\n", loan->date_issued);
    printf("├─────────────────────────────────────────┤\n");
    printf("│ Principal:       $%-22.2f│\n", loan->principal);
    printf("│ Interest Rate:   %-22.2f%%│\n", loan->interest_rate);
    printf("│ Duration:        %-22d mo│\n", loan->duration_months);
    printf("│ Monthly Payment: $%-22.2f│\n", loan->monthly_payment);
    printf("├─────────────────────────────────────────┤\n");
    printf("│ Total Amount:    $%-22.2f│\n", loan->total_amount);
    printf("│ Balance:         $%-22.2f│\n", loan->balance);
    printf("│ Paid:            $%-22.2f│\n", loan->total_amount - loan->balance);
    printf("└─────────────────────────────────────────┘\n");
}

void displayAllLoans(Loan loans[], int count) {
    if (count == 0) {
        printf("\nNo loans found in the system.\n");
        return;
    }
    
    printf("\n╔═══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                    ALL LOAN RECORDS                                               ║\n");
    printf("╠════╦═══════════════════╦═════╦════════════╦═══════╦══════════╦═══════════╦═══════════╦═══════════╣\n");
    printf("║ ID ║ Name              ║ Age ║ Principal  ║ Rate  ║ Duration ║ Monthly   ║ Balance   ║ Status    ║\n");
    printf("╠════╬═══════════════════╬═════╬════════════╬═══════╬══════════╬═══════════╬═══════════╬═══════════╣\n");
    
    for (int i = 0; i < count; i++) {
        printf("║ %-2d ║ %-17s ║ %-3d ║ $%-9.2f ║ %4.1f%% ║ %-6d m ║ $%-8.2f ║ $%-8.2f ║ %-9s ║\n",
               loans[i].id,
               loans[i].name,
               loans[i].age,
               loans[i].principal,
               loans[i].interest_rate,
               loans[i].duration_months,
               loans[i].monthly_payment,
               loans[i].balance,
               getStatusString(loans[i].status));
    }
    printf("╚════╩═══════════════════╩═════╩════════════╩═══════╩══════════╩═══════════╩═══════════╩═══════════╝\n");
}

// Core functionality
void addLoan(Loan loans[], int* count) {
    if (*count >= MAX_LOANS) {
        printf("Error: Maximum loan capacity (%d) reached.\n", MAX_LOANS);
        return;
    }
    
    printf("\n═══════════════════════════════════\n");
    printf("     ADD NEW LOAN APPLICATION\n");
    printf("═══════════════════════════════════\n");
    
    Loan newLoan;
    newLoan.id = getNextId(loans, *count);
    
    getValidString("Applicant Name: ", newLoan.name, MAX_NAME_LENGTH);
    newLoan.age = getValidInteger("Age (18-100): ", 18, 100);
    newLoan.principal = getValidFloat("Loan Amount ($1000-$1000000): ", 1000.0, 1000000.0);
    newLoan.interest_rate = getValidFloat("Annual Interest Rate (0-30%): ", 0.0, 30.0);
    newLoan.duration_months = getValidInteger("Duration in months (6-360): ", 6, 360);
    
    // Calculate loan details
    calculateLoanDetails(&newLoan);
    
    // Set initial status and date
    newLoan.status = PENDING;
    getCurrentDate(newLoan.date_issued);
    
    // Add to array
    loans[*count] = newLoan;
    (*count)++;
    
    printf("\n✓ Loan application added successfully!\n");
    displayLoan(&newLoan);
}

void searchLoan(Loan loans[], int count) {
    if (count == 0) {
        printf("\nNo loans in the system.\n");
        return;
    }
    
    printf("\n═══════════════════════════════════\n");
    printf("          SEARCH LOAN\n");
    printf("═══════════════════════════════════\n");
    printf("1. Search by ID\n");
    printf("2. Search by Name\n");
    int choice = getValidInteger("Choose option: ", 1, 2);
    
    if (choice == 1) {
        int id = getValidInteger("Enter Loan ID: ", 1, 9999);
        int found = 0;
        for (int i = 0; i < count; i++) {
            if (loans[i].id == id) {
                displayLoan(&loans[i]);
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("No loan found with ID %d.\n", id);
        }
    } else {
        char searchName[MAX_NAME_LENGTH];
        getValidString("Enter name to search: ", searchName, MAX_NAME_LENGTH);
        
        int found = 0;
        for (int i = 0; i < count; i++) {
            if (strcasecmp(loans[i].name, searchName) == 0) {
                displayLoan(&loans[i]);
                found = 1;
            }
        }
        if (!found) {
            printf("No loan found for \"%s\".\n", searchName);
        }
    }
}

void makeRepayment(Loan loans[], int count) {
    if (count == 0) {
        printf("\nNo loans in the system.\n");
        return;
    }
    
    int id = getValidInteger("Enter Loan ID for repayment: ", 1, 9999);
    int found = 0;
    
    for (int i = 0; i < count; i++) {
        if (loans[i].id == id) {
            found = 1;
            
            if (loans[i].balance <= 0) {
                printf("This loan has already been fully paid.\n");
                break;
            }
            
            printf("\nCurrent balance: $%.2f\n", loans[i].balance);
            printf("Suggested monthly payment: $%.2f\n", loans[i].monthly_payment);
            
            float payment = getValidFloat("Enter payment amount: $", 0.01, loans[i].balance * 1.5);
            
            if (payment >= loans[i].balance) {
                printf("\n✓ Payment covers full balance!\n");
                printf("  Previous balance: $%.2f\n", loans[i].balance);
                printf("  Payment: $%.2f\n", payment);
                if (payment > loans[i].balance) {
                    printf("  Overpayment refund: $%.2f\n", payment - loans[i].balance);
                }
                loans[i].balance = 0;
                loans[i].status = COMPLETED;
                printf("  New balance: $0.00\n");
                printf("  Loan status: COMPLETED\n");
            } else {
                loans[i].balance -= payment;
                printf("\n✓ Payment recorded successfully!\n");
                printf("  Payment: $%.2f\n", payment);
                printf("  Remaining balance: $%.2f\n", loans[i].balance);
                printf("  Payments remaining: %.1f months\n", 
                       loans[i].balance / loans[i].monthly_payment);
            }
            break;
        }
    }
    
    if (!found) {
        printf("No loan found with ID %d.\n", id);
    }
}

void updateLoanStatus(Loan loans[], int count) {
    if (count == 0) {
        printf("\nNo loans in the system.\n");
        return;
    }
    
    int id = getValidInteger("Enter Loan ID to update status: ", 1, 9999);
    int found = 0;
    
    for (int i = 0; i < count; i++) {
        if (loans[i].id == id) {
            found = 1;
            printf("\nCurrent status: %s\n", getStatusString(loans[i].status));
            printf("\n1. Pending\n2. Approved\n3. Rejected\n4. Completed\n");
            int statusChoice = getValidInteger("Select new status: ", 1, 4);
            
            loans[i].status = (LoanStatus)(statusChoice - 1);
            printf("✓ Status updated to: %s\n", getStatusString(loans[i].status));
            break;
        }
    }
    
    if (!found) {
        printf("No loan found with ID %d.\n", id);
    }
}

void showPendingBalances(Loan loans[], int count) {
    printf("\n╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║              LOANS WITH PENDING BALANCES                      ║\n");
    printf("╠════╦═══════════════════╦═══════════╦════════════╦═════════════╣\n");
    printf("║ ID ║ Name              ║ Balance   ║ Monthly    ║ Status      ║\n");
    printf("╠════╬═══════════════════╬═══════════╬════════════╬═════════════╣\n");
    
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (loans[i].balance > 0) {
            printf("║ %-2d ║ %-17s ║ $%-8.2f ║ $%-9.2f ║ %-11s ║\n",
                   loans[i].id,
                   loans[i].name,
                   loans[i].balance,
                   loans[i].monthly_payment,
                   getStatusString(loans[i].status));
            found = 1;
        }
    }
    
    if (!found) {
        printf("║                     No pending balances!                      ║\n");
    }
    printf("╚════╩═══════════════════╩═══════════╩════════════╩═════════════╝\n");
}

void showStatistics(Loan loans[], int count) {
    if (count == 0) {
        printf("\nNo loans in the system to analyze.\n");
        return;
    }
    
    // Calculate statistics
    float total_principal = 0;
    float total_outstanding = 0;
    float total_collected = 0;
    int pending_count = 0, approved_count = 0, rejected_count = 0, completed_count = 0;
    
    for (int i = 0; i < count; i++) {
        total_principal += loans[i].principal;
        total_outstanding += loans[i].balance;
        total_collected += (loans[i].total_amount - loans[i].balance);
        
        switch(loans[i].status) {
            case PENDING: pending_count++; break;
            case APPROVED: approved_count++; break;
            case REJECTED: rejected_count++; break;
            case COMPLETED: completed_count++; break;
        }
    }
    
    printf("\n╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║                    LOAN STATISTICS                            ║\n");
    printf("╠═══════════════════════════════════════════════════════════════╣\n");
    printf("║  Total Loans:                    %-28d  ║\n", count);
    printf("║                                                               ║\n");
    printf("║  Status Breakdown:                                            ║\n");
    printf("║    • Pending:                    %-28d  ║\n", pending_count);
    printf("║    • Approved:                   %-28d  ║\n", approved_count);
    printf("║    • Rejected:                   %-28d  ║\n", rejected_count);
    printf("║    • Completed:                  %-28d  ║\n", completed_count);
    printf("║                                                               ║\n");
    printf("║  Financial Summary:                                           ║\n");
    printf("║    • Total Principal Issued:     $%-27.2f  ║\n", total_principal);
    printf("║    • Total Outstanding Balance:  $%-27.2f  ║\n", total_outstanding);
    printf("║    • Total Collected:            $%-27.2f  ║\n", total_collected);
    printf("║    • Collection Rate:            %-26.1f%%  ║\n", 
           total_principal > 0 ? (total_collected / (total_collected + total_outstanding)) * 100 : 0);
    printf("╚═══════════════════════════════════════════════════════════════╝\n");
}

// Main menu
int main() {
    Loan loans[MAX_LOANS];
    int count = loadFromFile(loans);
    int choice;
    
    printf("\n╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║           ADVANCED LOAN MANAGEMENT SYSTEM v2.0                ║\n");
    printf("╚═══════════════════════════════════════════════════════════════╝\n");
    
    do {
        printf("\n┌───────────────────────────────────┐\n");
        printf("│         MAIN MENU                 │\n");
        printf("├───────────────────────────────────┤\n");
        printf("│ 1. Add New Loan Application       │\n");
        printf("│ 2. View All Loans                 │\n");
        printf("│ 3. Search Loan                    │\n");
        printf("│ 4. Make Repayment                 │\n");
        printf("│ 5. Update Loan Status             │\n");
        printf("│ 6. Show Pending Balances          │\n");
        printf("│ 7. View Statistics                │\n");
        printf("│ 8. Export to CSV                  │\n");
        printf("│ 9. Save and Exit                  │\n");
        printf("└───────────────────────────────────┘\n");
        
        choice = getValidInteger("Choose an option (1-9): ", 1, 9);
        
        switch(choice) {
            case 1:
                addLoan(loans, &count);
                saveToFile(loans, count); // Auto-save
                break;
            case 2:
                displayAllLoans(loans, count);
                break;
            case 3:
                searchLoan(loans, count);
                break;
            case 4:
                makeRepayment(loans, count);
                saveToFile(loans, count); // Auto-save
                break;
            case 5:
                updateLoanStatus(loans, count);
                saveToFile(loans, count); // Auto-save
                break;
            case 6:
                showPendingBalances(loans, count);
                break;
            case 7:
                showStatistics(loans, count);
                break;
            case 8:
                exportToCSV(loans, count);
                break;
            case 9:
                saveToFile(loans, count);
                printf("\n✓ Data saved. Goodbye!\n\n");
                break;
        }
        
    } while (choice != 9);
    
    return 0;
}