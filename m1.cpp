/*
 * FILE:m1.cpp
 * PROJECT:M1
 * PROGRAMMER:MANREET THIND (Student ID:8982315)
 * FIRSTVERSION:03/06/24
 * DESCRIPTION :
 * The functions in this file that keeps track of fares for flights and displays information about them.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct FlightNode
{
	char* destination;
	char* date;
	float fare;
	struct FlightNode* prevFare;
	struct FlightNode* nextFare;
	struct FlightNode* prevDest;
	struct FlightNode* nextDest;
};

//funtion prototypes
FlightNode* createNode(char* destination, char* date, float fare);
void insertByFare(FlightNode **head, FlightNode **tail, FlightNode* newNode);
void insertByDestination(FlightNode ** head, FlightNode ** tail, FlightNode * newNode);
FlightNode* findFlight(FlightNode* head, char* destination, char* date);
void deleteNode(FlightNode *node, FlightNode **head, FlightNode** tail);
void displayListByFare(FlightNode* head);
void displayListByDestination(FlightNode* head);
void freeList(FlightNode* head);

int main()
{
	FlightNode* fareHead = NULL;
	FlightNode* fareTail = NULL;
	FlightNode* destHead = NULL;
	FlightNode* destTail = NULL;

	//array holding maximum length
	char destination[31];
	char date[31];
	float fare{};
	char input[30];

	while (1)
	{
		//Asking user to enter destination
		printf("Enter destination: ");
		fgets(destination, sizeof(destination), stdin);
		destination[strcspn(destination, "\n")] = '\0';

		if (strcmp(destination, ".") == 0)
			break;

		//Asking user to enter date
		printf("Enter date: ");
		fgets(date, sizeof(date), stdin);
		date[strcspn(date, "\n")] = '\0';

		if (strcmp(date, ".") == 0)
			break;

		//Asking user to enter fare
		printf("Enter fare: ");
		fgets(input, sizeof(input), stdin);
		sscanf_s(input, "%f", &fare);

		FlightNode* newNode = createNode(destination, date, fare);

		insertByFare(&fareHead, &fareTail, newNode);
		insertByDestination(&destHead, &destTail, newNode);
	}

	//Displaying flights sorted by fare
	printf("\nFlights sorted by Fare\n");
	displayListByFare(fareHead);

	//Displaying flights sorted by destination
	printf("\nFlights sorted by destination\n");
	displayListByDestination(destHead);

	//Search destination of a flight
	printf("Enter destination to search:");
	fgets(destination, sizeof(destination), stdin);
	destination[strcspn(destination, "\n")] = '\0';

	//Search destination of a flight
	printf("Enter date to search:");
	fgets(date, sizeof(date), stdin);
	date[strcspn(date, "\n")] = '\0';

	FlightNode *foundFlight = findFlight(destHead, destination, date);
	if (foundFlight)
	{
		printf("Fare for the flight is: %.2f\n", foundFlight->fare);

		printf("Enter new fare (or same to keep changed): ");
		fgets(input, sizeof(input), stdin);
		sscanf_s(input, "%f", &fare);

		if( fare!= foundFlight->fare)
		{
			foundFlight->fare = fare;
			deleteNode(foundFlight, &fareHead, &fareTail);
			insertByFare(&fareHead, &fareTail, foundFlight);
		}
		else
		{
			//print if fare gets unchanged
			printf("Fare is unchanged.\n");
		}
	}
 else
 {
	//print if flight is not found
	printf("Flight not found.\n");
 }

 //displaying updated flights sorted by fare
 printf("\nUpdated flights sorted by fare:\n");
 displayListByFare(fareHead);

 //displaying updated flights soreted by destination
 printf("\nUpdated flights sorted by destination:\n");
 displayListByDestination(destHead);

 freeList(fareHead);

 return 0;
}

/*  
Function: createNode()
Parameter:
char* destination: pointer to a C-style string containing the destination of the flight.
char* date: pointer to a C-style string containing the date of the flight.
float fare: fare of the flight.
Return Value:
FlightNode pointer: pointer to the newly created FlightNode struct.
Description: This function creates a new FlightNode struct and initializes its fields with the provided information. 
*/
FlightNode* createNode(char* destination, char* date, float fare)
{
	FlightNode* newNode = (FlightNode*)malloc(sizeof(FlightNode));
	if (!newNode)
	{
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}

	newNode->destination = (char*)malloc((strlen(destination) + 1) * sizeof(char));
	if (!newNode->destination)
	{
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	strcpy_s(newNode->destination, strlen(destination) + 1,destination);

	newNode->date = (char*)malloc((strlen(date) + 1) * sizeof(char));
	if (!newNode->date)
	{
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	strcpy_s(newNode->date, strlen(date) + 1,date);

	newNode->fare = fare;
	newNode->prevFare = NULL;
	newNode->nextFare = NULL;
	newNode->prevDest = NULL;
	newNode->nextDest = NULL;
	return newNode;
}

/*
Function: insertByFare()
Parameter:
FlightNode **head: double pointer to the head of the linked list sorted by fare.
FlightNode **tail: double pointer to the tail of the linked list sorted by fare.
FlightNode *newNode: pointer to the new flight to be inserted.
Return Value: void
Description: This function inserts a new flight into the linked list sorted by fare.
*/
void insertByFare(FlightNode **head, FlightNode **tail, FlightNode *newNode)
{
	FlightNode* current = *head;

	if (!current || current->fare > newNode->fare)
	{
		newNode->nextFare = current;
		if (current)
		{
			current->prevFare = newNode;
		}
		else
		{
			*tail = newNode;
		}
		*head = newNode;
		return;
	}

	while (current->nextFare && current->nextFare->fare < newNode->fare)
	{
		current = current->nextFare;
	}

	newNode->nextFare = current->nextFare;
	if (current->nextFare)
	{
		current->nextFare->prevFare = newNode;
	}
	else
	{
		*tail = newNode;
	}
	current->nextFare = newNode;
	newNode->prevFare = current;
}

/*
Function: insertByDestination()
Parameter:
FlightNode **head: double pointer to the head of the linked list sorted by destination.
FlightNode **tail: double pointer to the tail of the linked list sorted by destination.
FlightNode *newNode: pointer to the new flight to be inserted.
Return Value: void
Description: This function inserts a new flight into the linked list sorted by destination.
*/
void insertByDestination(FlightNode** head, FlightNode** tail, FlightNode* newNode)
{
	FlightNode* current = *head;

	if (!current || strcmp(current->destination, newNode->destination) < 0)
	{
		newNode->nextDest = current;
		if (current)
		{
			current->prevDest = newNode;
		}
		else
		{
			*tail = newNode;
		}
		*head = newNode;
		return;
	}
	while (current->nextDest && strcmp(current->nextDest->destination, newNode->destination) <= 0)
	{
		current = current->nextDest;
	}
	newNode->nextDest = current->nextDest;
	if (current->nextDest)
	{
		current->nextDest->prevDest = newNode;
	}
	else
	{
		*tail = newNode;
	}
	current->nextDest = newNode;
	newNode->prevDest = current;
}

/*
Function: findFlight()
Parameter:
FlightNode* head: pointer to the head of the linked list of flights.
char* destination: pointer to a C-style string containing the destination to search for.
char* date: pointer to a C-style string containing the date to search for.
Return Value:
FlightNode pointer: pointer to the node containing the flight if found, otherwise NULL.
Description: This function searches for a flight with the specified destination and date in the linked list.
*/
FlightNode* findFlight(FlightNode* head, char* destination, char* date)
{
	while (head)
	{
		if (strcmp(head->destination, destination) == 0 && strcmp(head->date, date) == 0)
		{
			return head;
		}
		head = head->nextDest;
	}
	return NULL;
}

/*
Function: deleteNode() 
Parameter:
FlightNode* node: pointer to the node to be deleted.
FlightNode** head: double pointer to the head of the linked list.
FlightNode** tail: double pointer to the tail of the linked list.
Return Value: void
Description: This function deletes a node from the linked list and frees the associated memory.
*/
void deleteNode(FlightNode* node, FlightNode** head, FlightNode** tail)
{
	if (!node)
	{
		return;
	}
	if (node->prevFare)
	{
		node->prevFare->nextFare = node->nextFare;
	}
	else
	{
		*head = node->nextFare;
	}
	if (node->nextFare)
	{
		node->nextFare->prevFare = node->prevFare;
	}
	else
	{
		*tail = node->prevFare;
	}
	//free the allocated memory
	free(node->destination);
	free(node->date);
	free(node);
}

/*
Function: displayListByFare()
Parameter:
FlightNode* head: pointer to the head of the linked list sorted by fare.
Return Value: void
Description: This function displays the flights in the linked list sorted by fare.
*/
void displayListByFare(FlightNode* head)
{
	while (head)
	{
		//display list by fare with specified spacing
		printf("%-35s%-35s%.2f\n", head->destination, head->date, head->fare);
		head = head->nextFare;
	}
}

/*
Function: displayListByDestination()
Parameter:
FlightNode* head: pointer to the head of the linked list sorted by destination.
Return Value: void
Description: This function displays the flights in the linked list sorted by destination.
*/
void displayListByDestination(FlightNode* head)
{
	while (head)
	{
		//display list by destination with specified spacing
		printf("%-35s%-35s%.2f\n", head->destination, head->date, head->fare);
		head = head->nextDest;
	}
}

/*
 Function: freeList()
 Parameter:
 FlightNode * head: pointer to the head of the linked list.
 Return Value: void
 Description: This function frees the memory allocated for the linked list.
 */
void freeList(FlightNode * head)
{
		FlightNode* temp;
		while (head)
		{
			temp = head;
			head = head->nextFare;
			//free up everything
			free(temp->destination);
			free(temp->date);
			free(temp);
		}
}