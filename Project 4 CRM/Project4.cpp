/* EE312 Simple CRM Project
 *
 * Rajat Ahuja
 * RA29697
 * Dr. Nandakumar
 * EE 312
 */

#include <stdio.h>
#include <assert.h>
#include "stdint.h"
#include "MyString.h"
#include "Invent.h"

#define MAX_CUSTOMERS 1000
Customer customers[MAX_CUSTOMERS];

int num_customers, numBottles, numDiapers, numRattles = 0;  //initial inventory


/* ASSUMPTIONS: (For corner cases not described in documentation)
*
* processPurchase:
* (1) - A customer CANNOT have a negative or zero (<= 0) purchase amount.
* (2) - If a purchase amount <= 0 is seen in the input file, the program will not count that person as a customer and will not output anything to the console.
*       The program will essentially pretend like that input line didn't exist and move on to the next command line.
* (3) - Anything put in the place where the name of the customer should be will be accepted as the name of the customer, regardless of whether or not the name
*		has actual letters.
* (4) - If the item requested is NOT a bottle, diaper, or rattle, the item is invalid and the program will exit.
*
* processInventory:
* (5) - An inventory command CANNOT have a negative deposit (the program ONLY allows the inventory to decrease when a valid customer purchases them).
*
* processSummarize:
* (6) - If all customers purchase the same amount of a single item, the first person to purchase that item will be reported as the person who purchased the most of that item.
*/

/**
 * Clear the inventory and reset the customer database. Free the string pointers
 * that point to the customer names stored on the heap. Finally, reset the
 * number of customers that have purchased items.
 *
 * \param: none
 * \return: none
 */
void reset(void)
{
    for (int i = 0; i < num_customers; i++) {	/* done with all purchases, can destroy strings on heap that have the customer names */
        StringDestroy(&(customers[i].name));
    }

    for (int i = 0; i < num_customers; i++) {	/* reset customer database */
        customers[i].bottles = 0;
        customers[i].diapers = 0;
        customers[i].rattles = 0;
    }

    num_customers = 0;							/* no customers initially */

    numBottles = 0;								/* clear the inventory */
    numDiapers = 0;
    numRattles = 0;
}

/**
 * Prints out to the console the current amount of items in inventory, the current
 * total number of customers who have made a valid purchase and the name of the
 * customer who has (so far) bought the most of a specific item. If no customer
 * has purchased a certain item at the time 'processSummarize' is called, prints
 * out which item hasn't been purchased yet.
 *
 * ASSUMPTION: (6) - If all customers buy the same amount of an item, the first person to buy that item is the one who purchased the most of it.
 *
 * \param: none
 * \return: none
 */
void processSummarize()
{
    printf("There are %d Bottles, %d Diapers and %d Rattles in inventory\n",
           numBottles, numDiapers, numRattles);
    printf("we have had a total of %d different customers\n", num_customers);

    int bottlesPerson = 0;			/* name points to customer that purchased the most of an item */
    int diapersPerson = 0;
    int rattlesPerson = 0;

    int maxBottles = 0;
    int maxDiapers = 0;
    int maxRattles = 0;

    for (int i = 0; i < num_customers; i++) {
        if (customers[i].bottles > maxBottles) {
            maxBottles = customers[i].bottles;
            bottlesPerson = i;
        }
    }
    if (maxBottles) {
        StringPrint(&(customers[bottlesPerson].name));
        printf(" has purchased the most Bottles (%d)\n", maxBottles);
    }
    else {
        printf("no one has purchased any Bottles\n");
    }

    for (int i = 0; i < num_customers; i++) {
        if (customers[i].diapers > maxDiapers) {
            maxDiapers = customers[i].diapers;
            diapersPerson = i;
        }
    }
    if (maxDiapers) {
        StringPrint(&(customers[diapersPerson].name));
        printf(" has purchased the most Diapers (%d)\n", maxDiapers);
    }
    else {
        printf("no one has purchased any Diapers\n");
    }

    for (int i = 0; i < num_customers; i++) {
        if (customers[i].rattles > maxRattles) {
            maxRattles = customers[i].rattles;
            rattlesPerson = i;
        }
    }
    if (maxRattles) {
        StringPrint(&(customers[rattlesPerson].name));
        printf(" has purchased the most Rattles (%d)\n", maxRattles);
    }
    else {
        printf("no one has purchased any Rattles\n");
    }

}

/**
* Prints to the console that the item specified by 'itemType' cannot be
* purchased by the customer since it exceeds the amount currently in inventory.
*
* \param Customer: name of customer
* \param itemNum: number of item currently in inventory
* \param itemType: name of item
* \return: none
*/
void lackingInventory(String* Customer, int itemNum, String* itemType)
{
    printf("Sorry ");
    StringPrint(Customer);
    printf(", we only have %d ", itemNum);
    StringPrint(itemType);
    printf("\n");
}

/**
 * Records the name, item of purchase and item quantity of a customer purchase
 * read from the input file. This information is stored in the 'customer' struct
 * initialized at the top. Furthermore, the number of customers is updated when
 * a new customer is recorded and the inventory is updated for every valid
 * purchase. 'Customer' can hold up to 1000 customers. If the number of customers
 * exceeds this capacity, program is terminated/exited. If the customer requests
 * an amount of an item exceeding the amount in inventory, prints out a message
 * stating this. Terminate/exit program if input file has invalid sequence of
 * commands (i.e. Purchase 50 Craig Bottles, etc.).
 *
 * ASSUMPTIONS: Refer to top.
 *
 * \param: none
 * \return: none
 */
void processPurchase()
{
    String custName;
    String custItem;
    int custValue;

    readString(&custName);												/* obtains name of customer */
    readString(&custItem);												/* obtain item customer wishes to buy */
    readNum(&custValue);												/* obtain amount of item */

    String Bottles = StringCreate("Bottles");
    String Diapers = StringCreate("Diapers");
    String Rattles = StringCreate("Rattles");

    bool customerRecorded = false;

    for (int i = 0; i < num_customers; i++) {							/* checks if current customer has already purchased items previously */
        if (StringIsEqualTo(&customers[i].name, &custName)) { 			/* the customer has bought items before */

            customerRecorded = true;

            if (StringIsEqualTo(&custItem, &Bottles)) {
                if (custValue <= 0) {
                    break;
                }
                else if (numBottles < custValue) {
                    lackingInventory(&(customers[i].name), numBottles, &Bottles);
                }
                else {
                    customers[i].bottles += custValue;					/* update number of bottles existing customer has purchased */
                    numBottles -= custValue;							/* update number of available bottles in inventory */
                    break;												/* no longer need to check rest of the customer record book */
                }
            }
            else if (StringIsEqualTo(&custItem, &Diapers)) {
                if (custValue <= 0) {
                    break;
                }
                else if (numDiapers < custValue) {
                    lackingInventory(&(customers[i].name), numDiapers, &Diapers);
                }
                else {
                    customers[i].diapers += custValue;					/* update number of diapers existing customer has purchased */
                    numDiapers -= custValue;							/* update number of available diapers in inventory */
                    break;												/* no longer need to check rest of the customer record book */
                }
            }
            else if (StringIsEqualTo(&custItem, &Rattles)) {
                if (custValue <= 0) {
                    break;
                }
                else if (numRattles < custValue) {
                    lackingInventory(&(customers[i].name), numRattles, &Rattles);
                }
                else {
                    customers[i].rattles += custValue;					/* update number of rattles existing customer has purchased */
                    numRattles -= custValue;							/* update number of available rattles in inventory */
                    break;												/* no longer need to check rest of the customer record book */
                }
            }
            else {
                exit(1);												/* invalid input */
            }
        }
    }

    if (customerRecorded == false) { /* new customer has been found */

        if (num_customers == MAX_CUSTOMERS) {							/* exit program if number of customers exceeds capacity of record book */
            exit(1);													/* unsuccessful termination */
        }

        if (StringIsEqualTo(&custItem, &Bottles)) {						/* record their purchase in the record book */
            if (numBottles < custValue) {
                lackingInventory(&custName, numBottles, &Bottles);
            }
            else if (custValue > 0) {
                customers[num_customers].name = StringDup(&custName);
                customers[num_customers].bottles = custValue;
                numBottles -= custValue;								/* update inventory to reflect customer purchase */
                num_customers++;										/* update number of customers that have done business with us */
            }
        }

        else if (StringIsEqualTo(&custItem, &Diapers)) {				/* record their purchase in the record book */
            if (numDiapers < custValue) {
                lackingInventory(&custName, numDiapers, &Diapers);
            }
            else if (custValue > 0) {
                customers[num_customers].name = StringDup(&custName);
                customers[num_customers].diapers = custValue;			/* reflect customer purchase in their space in record book */
                numDiapers -= custValue;								/* update inventory to reflect customer purchase */
                num_customers++;										/* update number of customers that have done business with us */
            }
        }
        else if (StringIsEqualTo(&custItem, &Rattles)) {				/* record their purchase in the record book */
            if (numRattles < custValue) {
                lackingInventory(&custName, numRattles, &Rattles);
            }
            else if (custValue > 0) {
                customers[num_customers].name = StringDup(&custName);	/* store new customer with their recorded purchase in the record book */
                customers[num_customers].rattles = custValue;			/* reflect customer purchase in their space in record book */
                numRattles -= custValue;								/* update inventory to reflect customer purchase */
                num_customers++;										/* update number of customers that have done business with us */
            }
        }
        else {
            exit(1);													/* invalid input */
        }
    }

    StringDestroy(&custName);
    StringDestroy(&custItem);

    StringDestroy(&Bottles);											/* no longer needed, deallocate space on heap */
    StringDestroy(&Diapers);
    StringDestroy(&Rattles);
}

/**
 * Adds the amount of an item specified by the input file. This information is
 * stored in a separate struct named 'inven.' Since there is no specified maximum
 * amount of items the inventory can hold, it is assumed the inventory can hold
 * up to uint16_t (65,535) items for each item type. Terminate/exit program if
 * the inventory to add is not a valid item.
 *
 * ASSUMPTION: (5) - The inventory CANNOT have negative amounts of an item.
 *
 * \param: none
 * \return: none
 */
void processInventory()
{
    String custItem;
    int custValue;

    readString(&custItem);													/* obtain item */
    readNum(&custValue);													/* obtain amount of item */

    String Bottles = StringCreate("Bottles");								/* create strings for item names */
    String Diapers = StringCreate("Diapers");
    String Rattles = StringCreate("Rattles");

    if (StringIsEqualTo(&custItem, &Bottles) && (custValue > 0)) {			/* bottles will be added to inventory */
        numBottles += custValue;
    }
    else if (StringIsEqualTo(&custItem, &Diapers) && (custValue > 0)) {		/* diapers will be added to inventory */
        numDiapers += custValue;
    }
    else if (StringIsEqualTo(&custItem, &Rattles) && (custValue > 0)) { 	/* rattles will be added to inventory */
        numRattles += custValue;
    }
    else {
        exit(1);															/* invalid input */
    }

    StringDestroy(&custItem);												/* no longer needed, deallocate space on heap */

    StringDestroy(&Bottles);
    StringDestroy(&Diapers);
    StringDestroy(&Rattles);

}




