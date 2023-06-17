#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Item {
    char item[100];
    float price;
    int qty;
};

struct Order {
    char customer[100];
    char date[100];
    int num_of_items;
    struct Item items[100];
};

void bill_header(char* name, char* date) {
    printf("\n\n");
    printf("\t    MR. CAFE\n");
    printf("\n\t   -----------------\n");
    printf("Date: %s\n", date);
    printf("Invoice To: %s\n", name);
    printf("\n");
    printf("---------------------------------------\n");
    printf("Items\t\tQty\t\tTotal\t\t\n");
    printf("---------------------------------------\n");
    printf("\n\n");
}

void bill_body(char* item, int qty, float price) {
    printf("%s\t\t%d\t\t%.2f\t\t\n", item, qty, qty * price);
}

void bill_footer(float total) {
    printf("\n");
    float dis = 0.1 * total;
    float net_total = total - dis;
    float cgst = 0.09 * net_total;
    float grand_total = net_total + 2 * cgst;
    printf("---------------------------------------\n");
    printf("Sub Total\t\t\t%.2f\n", total);
    printf("Discount @10%%\t\t\t%.2f\n", dis);
    printf("\t\t\t\t-------\n");
    printf("Net Total\t\t\t%.2f\n", net_total);
    printf("CGST @9%%\t\t\t%.2f\n", cgst);
    printf("SGST @9%%\t\t\t%.2f\n", cgst);
    printf("---------------------------------------\n");
    printf("Grand Total\t\t\t%.2f\n", grand_total);
    printf("---------------------------------------\n\n");
}

void generate_invoice() {
    struct Order orders;
    float total = 0;

    printf("\nPlease enter the name of the customer: ");
    scanf("%99[^\n]%*c", orders.customer);

    time_t t = time(NULL);
    struct tm* tm = localtime(&t);
    strftime(orders.date, sizeof(orders.date), "%d %b, %Y", tm);

    printf("Please enter the number of items: ");
    scanf("%d", &orders.num_of_items);
    getchar();

    for (int i = 0; i < orders.num_of_items; i++) {
        printf("\nItem %d:\n", i + 1);
        printf("Please enter the item: ");
        scanf("%99[^\n]%*c", orders.items[i].item);

        printf("Please enter the quantity: ");
        scanf("%d", &orders.items[i].qty);
        getchar();

        printf("Please enter the unit price: ");
        scanf("%f", &orders.items[i].price);
        getchar();

        total += orders.items[i].qty * orders.items[i].price;
    }

    bill_header(orders.customer, orders.date);
    for (int i = 0; i < orders.num_of_items; i++) {
        bill_body(orders.items[i].item, orders.items[i].qty, orders.items[i].price);
    }
    bill_footer(total);

    char save_bill;
    printf("Do you want to save the invoice? [y/n]: ");
    scanf(" %c", &save_bill);
    getchar();

    if (save_bill == 'y') {
        FILE* file = fopen("RestaurantBill.txt", "a");
        if (file != NULL) {
            fprintf(file, "Customer: %s\n", orders.customer);
            fprintf(file, "Date: %s\n", orders.date);
            fprintf(file, "Items\t\tQty\t\tTotal\n");
            fprintf(file, "---------------------------------------\n");
            for (int i = 0; i < orders.num_of_items; i++) {
                fprintf(file, "%s\t\t%d\t\t%.2f\n", orders.items[i].item, orders.items[i].qty, orders.items[i].qty * orders.items[i].price);
            }
            fprintf(file, "---------------------------------------\n");
            fprintf(file, "Sub Total\t\t\t%.2f\n", total);
            fprintf(file, "Discount @10%%\t\t\t%.2f\n", 0.1 * total);
            fprintf(file, "Net Total\t\t\t%.2f\n", total - 0.1 * total);
            fprintf(file, "CGST @9%%\t\t\t%.2f\n", 0.09 * (total - 0.1 * total));
            fprintf(file, "SGST @9%%\t\t\t%.2f\n", 0.09 * (total - 0.1 * total));
            fprintf(file, "---------------------------------------\n");
            fprintf(file, "Grand Total\t\t\t%.2f\n", total - 0.1 * total + 0.09 * (total - 0.1 * total) * 2);
            fprintf(file, "---------------------------------------\n\n");
            fclose(file);
        }
    }
}

void show_all_invoices() {
    FILE* file = fopen("RestaurantBill.txt", "r");
    if (file != NULL) {
        printf("  ****Your Previous Invoices****\n\n");
        char c;
        while ((c = fgetc(file)) != EOF) {
            printf("%c", c);
        }
        fclose(file);
    } else {
        printf("No invoices found.\n");
    }
}

void search_invoice() {
    char name[100];
    printf("Enter the name of the customer: ");
    scanf("%99[^\n]%*c", name);

    FILE* file = fopen("RestaurantBill.txt", "r");
    if (file != NULL) {
        char data[1000];
        fread(data, sizeof(char), sizeof(data), file);
        if (strstr(data, name) != NULL) {
            printf("\n\t****Invoice of %s****\n", name);
            char* invoice_start = strstr(data, "Customer: ");
            if (invoice_start != NULL) {
                printf("%s", invoice_start);
            }
        } else {
            printf("Sorry, the invoice for %s does not exist.\n", name);
        }
        fclose(file);
    } else {
        printf("No invoices found.\n");
    }
}

int main() {
    while (1) {
        printf("\t============MR. CAFE============\n");
        printf("\nPlease select your preferred operation\n");
        printf("\n1. Generate Invoice\n");
        printf("2. Show all Invoices\n");
        printf("3. Search Invoice\n");
        printf("4. Exit\n");

        int opt;
        printf("\nYour choice: ");
        scanf("%d", &opt);
        getchar();

        if (opt == 1) {
            generate_invoice();
        }
        else if (opt == 2) {
            show_all_invoices();
        }
        else if (opt == 3) {
            search_invoice();
        }
        else if (opt == 4) {
            break;
        }
        else {
            printf("Invalid option. Please try again.\n");
        }
    }

    return 0;
}
