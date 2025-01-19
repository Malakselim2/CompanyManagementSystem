#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>


typedef struct
{
    int day;
    int month;
    int year;
} Date;

typedef struct
{
    int id;
    char Name[100];
    float salary;
    Date birthDate;
    char address[100];
    char mobileNumber[12];
    Date enrollmentDate;
    char email[100];
} Employee;

Employee employees[200];
int NumEmployee = 0;

void LOAD(FILE* file);
void QUERY(char *partialName);
void ADD();
void DELETE();
void MODIFY();
void PRINT();
void SAVE();

void printMenu()
{
    printf("\nMenu:\n");
    printf("1. ADD\n");
    printf("2. DELETE\n");
    printf("3. MODIFY\n");
    printf("4. SEARCH\n");
    printf("5. PRINT\n");
    printf("6. SAVE\n");
    printf("7. QUIT\n");
}

int main()
{
    FILE *file = fopen("Company.txt", "r");
    LOAD(file);
    fclose(file);

    int choice;
    int saveChoice = 0;

    do
    {
        printMenu();
        printf("Enter your choice: ");
        checkfordigit(&choice);
        for (int k = 0; choice < 1 || choice > 7; k++)
        {
            printf("ERROR: Please choose a valid number: ");
            checkfordigit(&choice);
            k = 0;
        }

        switch (choice)
        {
        case 1:
            ADD();
            break;
        case 2:
            DELETE();
            break;
        case 3:
            MODIFY();
            break;
        case 4:
        {
            char partialName[100];
            printf("Enter part of the employee's name to search: ");
            scanf("%s", partialName);
            QUERY(partialName);
            break;
        }
        case 5:
            PRINT();
            break;
        case 6:
            SAVE();
            saveChoice = 1;
            break;
        case 7:
            if (saveChoice == 0)
            {
                QUIT();
            }

        }
    }
    while (choice != 7);

    return 0;
}
void  checkfordigit(int *choice)
{
    char input[100];
    int validInput = 0;

    do
    {
        scanf("%s", input);
        validInput = 1;
        for (int i = 0; input[i] != '\0'; i++)
        {
            if (!isdigit(input[i]))
            {
                validInput = 0;
                break;
            }
        }

        if (!validInput)
        {
            printf("ERROR: Please choose a valid number : ");
        }
        else
        {
            *choice = atoi(input);
            return;
        }
    }
    while (!validInput);
}


void salaryValidation(char *salary)
{
    int dotCount = 0;
    int i = 0;

    while (salary[i] != '\0')
    {
        if (!((salary[i] >= '0' && salary[i] <= '9') || (i == 0 && salary[i] == '-')) && (salary[i] != '.'))
        {
            printf("ERROR: Please enter a valid salary: ");
            scanf("%s", salary);
            i = -1;
            dotCount = 0;
        }
        else if (salary[i] == '.')
        {
            dotCount++;
        }
        i++;
    }

    if (dotCount > 1)
    {
        printf("Invalid salary: %s\n", salary);
        printf("Please enter a valid salary: ");
        scanf("%s", salary);
        salaryValidation(salary);
    }
}

void idValidation(char *Id)
{
    int i = 0;
    while (Id[i] != '\0')
    {
        if (!((Id[i] >= '0') && (Id[i] <= '9')))
        {
            printf("Invalid Id Please enter another valid ID: ");
            scanf("%s", Id);
            i = -1;
        }
        i++;
    }
    int id = atoi(Id);
    for (int j = 0; j < NumEmployee; j++)
    {
        if (employees[j].id == id)
        {
            printf("Another employee has the same Id please enter another valid ID: ");
            scanf("%s", Id);
            idValidation(Id);
        }
    }
}

void validateidonly(char *id)
{
    int i=0;
    while (id[i] != '\0')
    {
        if(!isdigit(id[i]))
        {
            printf("Invalid id Please enter another valid ID: ");
            scanf("%s", id);
            validateidonly(id);
            i = -1;
        }
        i++;
    }


}

void validateName(char *name)
{
    int i = 0;
    while (name[i] != '\0')
    {
        if (!isalpha(name[i]) && name[i] != ' ')
        {
            printf("Invalid name. Name should only contain alphabetic characters.\n");
            printf("Please enter a valid name: ");
            scanf(" %[^\n]", name);
            validateName(name);
            i = 0;
        }
        else
        {
            i++;
        }
    }
}


void validatePhoneNumber(char *phone)
{

    if (strlen(phone) != 11 || phone[0] != '0' || phone[1] != '1'|| phone[2] != '1' && phone[2] != '2'&& phone[2] != '0' && phone[2] != '5')
    {
        printf("Invalid phone number. Please enter a valid one: ");
        scanf("%s", phone);
        validatePhoneNumber(phone);
        return;
    }

    for (int i = 0; i < 11; i++)
    {
        if (!isdigit(phone[i]))
        {
            printf("Invalid phone number. Please enter a valid one: ");
            scanf("%s", phone);
            validatePhoneNumber(phone);
            return ;
        }
    }
}

void validateEmail(char *email)
{
    int i = 0;
    char beforeAt[100];
    char afterAt[100];
    int atSymbolFound = 0;

    while (email[i] != '\0')
    {
        if (email[i] == '@')
        {
            atSymbolFound = 1;
            break;
        }
        i++;
    }

    if (!atSymbolFound)
    {
        printf("Email is not valid. Please enter a valid email: ");
        scanf("%s", email);
        validateEmail(email);
        return;
    }

    i = 0;
    while (email[i] != '@')
    {
        beforeAt[i] = email[i];
        i++;
    }
    beforeAt[i] = '\0';
    i++;

    int j = 0;
    while (email[i] != '\0')
    {
        afterAt[j] = email[i];
        i++;
        j++;
    }
    afterAt[j] = '\0';

    for (i = 0; beforeAt[i] != '\0'; i++)
    {
        if (!isalpha(beforeAt[i]) && !isdigit(beforeAt[i]))
        {
            printf("Email is not valid. Please enter a valid email: ");
            scanf("%s", email);
            validateEmail(email);
            return;
        }
    }

    int dotCount = 0;
    for (i = 0; afterAt[i] != '\0'; i++)
    {
        if (!isalpha(afterAt[i]) && afterAt[i] != '.')
        {
            printf("Email is not valid. Please enter a valid email: ");
            scanf("%s", email);
            validateEmail(email);
            return;
        }
        if (afterAt[i] == '.')
        {
            dotCount++;
        }
    }

    if (dotCount == 0)
    {
        printf("Email is not valid. Please enter a valid email: ");
        scanf("%s", email);
        validateEmail(email);
        return;
    }
}
void validatedate(char* date)
{
    int day, month, year;

    while (1)
    {
        char dayStr[3];
        strncpy(dayStr, date, 2);
        dayStr[2] = '\0';
        day = atoi(dayStr);

        char monthStr[3];
        strncpy(monthStr, date + 3, 2);
        monthStr[2] = '\0';
        month = atoi(monthStr);

        char yearStr[5];
        strncpy(yearStr, date + 6, 4);
        yearStr[4] = '\0';
        year = atoi(yearStr);

        if (day <= 0 || day > 31 || month <= 0 || month > 12 || year < 1000 || year > 9999)
        {
            printf("Invalid date. Please enter a valid date:");
            scanf("%s", date);
        }
        else
        {
            break;
        }
    }
}

void LOAD(FILE* file)
{
    file = fopen("Company.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    int i = 0;

    printf("Loading employee data ... \n");

    while (fscanf(file, "%d,", &employees[i].id) == 1)
    {
        fscanf(file, " %[^\n,],", employees[i].Name);
        fscanf(file, "%f,", &employees[i].salary);
        fscanf(file, "%02d-%02d-%d,", &employees[i].birthDate.day, &employees[i].birthDate.month, &employees[i].birthDate.year);
        fscanf(file, " %[^\n,],", employees[i].address);
        fscanf(file, " %[^\n,],", employees[i].mobileNumber);
        fscanf(file, "%02d-%02d-%d,", &employees[i].enrollmentDate.day, &employees[i].enrollmentDate.month, &employees[i].enrollmentDate.year);
        fscanf(file, "%s,", employees[i].email);

        i++;
    }
    NumEmployee = i;
    printf("Loaded %d employees from File: Employees Data\n", NumEmployee);

    fclose(file);
}

void QUERY(char* partialName)
{
    validateName(partialName);
    int found = 0;
    for (int i = 0; i < NumEmployee; i++)
    {
        char lowercaseName[100];
        char lowercasePartialName[100];

        strcpy(lowercaseName, employees[i].Name);
        strcpy(lowercasePartialName, partialName);

        for (int j = 0; lowercaseName[j] != '\0'; j++)
        {
            lowercaseName[j] = tolower(lowercaseName[j]);
        }

        for (int j = 0; lowercasePartialName[j] != '\0'; j++)
        {
            lowercasePartialName[j] = tolower(lowercasePartialName[j]);
        }

        if (strstr(lowercaseName, lowercasePartialName) != NULL)
        {
            printf("\nID: %d \nName: %s \nSalary: %0.2f \nBirth Date: %02d-%02d-%d \nAddress: %s \nMobile Phone: %s \nEnrollment Date: %02d-%02d-%d \nEmail: %s\n",employees[i].id,employees[i].Name,employees[i].salary,employees[i].birthDate.day,employees[i].birthDate.month,employees[i].birthDate.year,employees[i].address,employees[i].mobileNumber,employees[i].enrollmentDate.day,employees[i].enrollmentDate.month,employees[i].enrollmentDate.year,employees[i].email);
            found = 1;
        }
    }

    if (!found)
    {
        printf("No matching employees found.\n");
    }
}
void ADD()
{
    Employee newEmployee;
    printf("\nEnter the data for the new employee:");

    char id[100];
    printf("\nID: ");
    scanf("%s", id);
    idValidation(id);
    strcpy(newEmployee.id, id);

    printf("Full Name: ");
    char name[100];
    scanf(" %[^\n]", name);
    validateName(name);
    strcpy(newEmployee.Name, name);

    printf("Salary: ");
    char salary[100];
    scanf("%s", salary);
    salaryValidation(salary);
    newEmployee.salary = atof(salary);


    printf("Date of Birth (format: dd-mm-yyyy): ");
    char date[100];
    scanf("%s", date);
    validatedate(date);
    sscanf(date, "%02d-%02d-%d", &newEmployee.birthDate.day, &newEmployee.birthDate.month, &newEmployee.birthDate.year);


    printf("Address: ");
    char Address[100];
    scanf(" %[^\n]", Address);
    strcpy(newEmployee.address, Address);



    printf("Mobile Number: ");
    char phone[100];
    scanf(" %[^\n]", phone);
    validatePhoneNumber(phone);
    strcpy(newEmployee.mobileNumber, phone);

    time_t currentTime;
    time(&currentTime);
    struct tm* localTime = localtime(&currentTime);
    newEmployee.enrollmentDate.day = localTime->tm_mday;
    newEmployee.enrollmentDate.month = localTime->tm_mon + 1;
    newEmployee.enrollmentDate.year = localTime->tm_year + 1900;

    printf("Email: ");
    char email[100];
    scanf(" %[^\n]", email);
    validateEmail(email);
    strcpy(newEmployee.email, email);

    employees[NumEmployee] = newEmployee;
    NumEmployee++;
    printf("Employee added successfully.\n");
}
void  DELETE()
{
    int i, j;
    char id[100];
    printf("ID: ");
    scanf("%s", id);
    validateidonly(id);
    int deleteId = atoi(id);

    for (i = 0; i < NumEmployee; i++)
    {
        if (employees[i].id == deleteId)
        {
            for (j = i; j < NumEmployee - 1; j++)
            {
                employees[j] = employees[j + 1];
            }
            NumEmployee--;
            printf("Employee with ID %d deleted successfully.\n", deleteId);
            return;
        }
    }
    printf("Employee with ID %d not found.\n", deleteId);
}

void MODIFY()
{
    printf("Enter employee id to be modified ID: ");
    char id[100];
    scanf("%s", id);
    validateidonly(id);
    int modified =atoi(id);

    int found = 0;
    for (int i = 0; i < NumEmployee; i++)
    {
        if (employees[i].id == modified)
        {
            found = 1;

            printf("\nSelect the field to modify:\n");
            printf("1. Name\n2. Salary\n3. Mobile Number\n4. Address\n5. Email\n");
            int choice;
            printf("Enter your choice : ");
            checkfordigit(&choice);

            for (int k= 0; choice < 1 || choice > 5; k++)
            {
                printf("ERROR: Please choose a valid number: ");
                checkfordigit(&choice);
                k = 0;
            }

            switch (choice)
            {
            case 1:
                printf("Enter the new name: ");
                char name[100];
                scanf(" %[^\n]", name);
                validateName(name);
                strcpy(employees[i].Name, name);
                break;
            case 2:
                printf("Enter the new salary: ");
                char salary[100];
                scanf("%s", salary);
                salaryValidation(salary);
                employees[i].salary = atof(salary);
                break;
            case 3:
                printf("Mobile Number: ");
                char phone[100];
                scanf(" %[^\n]", phone);
                validatePhoneNumber(phone);
                strcpy(employees[i].mobileNumber, phone);
                break;
            case 4:
                printf("Enter the new address: ");
                char Address[100];
                scanf(" %[^\n]", Address);
                strcpy(employees[i].address, Address);
                break;
            case 5:
                printf("Enter the new email: ");
                char email[100];
                scanf(" %[^\n]", email);
                validateEmail(email);
                strcpy(employees[i].email, email);

                break;
            default:
                printf("Invalid choice.\n");
                return;
            }

            printf("Employee information modified successfully.\n");
        }
    }

    if (!found)
    {
        printf("Employee with ID %d not found.\n", modified);

    }
}

void SortByName()
{
    for (int i = 0; i < NumEmployee - 1; i++)
    {
        for (int j = 0; j < NumEmployee - i - 1; j++)
        {
            if (strcmp(employees[j].Name, employees[j + 1].Name) > 0)
            {
                Employee temp = employees[j];
                employees[j] = employees[j + 1];
                employees[j + 1] = temp;
            }
        }
    }
}
void  SortByDOB()
{
    for (int i = 0; i < NumEmployee - 1; i++)
    {
        for (int j = 0; j < NumEmployee - i - 1; j++)
        {
            if (employees[j].birthDate.year < employees[j + 1].birthDate.year)
            {
                Employee temp = employees[j];
                employees[j] = employees[j + 1];
                employees[j + 1] = temp;
            }
            else if (employees[j].birthDate.year == employees[j + 1].birthDate.year)
            {
                if (employees[j].birthDate.month < employees[j + 1].birthDate.month)
                {
                    Employee temp = employees[j];
                    employees[j] = employees[j + 1];
                    employees[j + 1] = temp;
                }
                else if (employees[j].birthDate.month == employees[j + 1].birthDate.month)
                {
                    if (employees[j].birthDate.day < employees[j + 1].birthDate.day)
                    {
                        Employee temp = employees[j];
                        employees[j] = employees[j + 1];
                        employees[j + 1] = temp;
                    }
                }
            }
        }
    }
}

void SortBySalary()
{
    for (int i = 0; i < NumEmployee - 1; i++)
    {
        for (int j = 0; j < NumEmployee - i - 1; j++)
        {
            if (employees[j].salary < employees[j + 1].salary)
            {
                Employee temp = employees[j];
                employees[j] = employees[j + 1];
                employees[j + 1] = temp;
            }
        }
    }
}

void PRINT()
{
    int choice;
    printf("Choose sorting criteria:\n");
    printf("1. Sort by Name\n2. Sort by Date of Birth\n3. Sort by Salary\n");
    printf("Enter your choice (1-3): ");
    checkfordigit(&choice);

    for (int k= 0; choice < 1 || choice > 3; k++)
    {
        printf("ERROR: Please choose a valid number: ");
        checkfordigit(&choice);
        k = 0;
    }

    switch (choice)
    {
    case 1:
        SortByName();
        break;
    case 2:
        SortByDOB();
        break;
    case 3:
        SortBySalary();
        break;
    default:
        printf("Invalid choice.\n");
        return;
    }
    for (int i = 0; i < NumEmployee; i++)
    {
        printf("\nID: %d \nName: %s \nSalary: %0.2f \nBirth Date: %02d-%02d-%d \nAddress: %s \nMobile Phone: %s \nEnrollment Date: %02d-%02d-%d \nEmail: %s\n",employees[i].id,employees[i].Name,employees[i].salary,employees[i].birthDate.day,employees[i].birthDate.month,employees[i].birthDate.year,employees[i].address,employees[i].mobileNumber,employees[i].enrollmentDate.day,employees[i].enrollmentDate.month,employees[i].enrollmentDate.year,employees[i].email);
    }
}
void SAVE()
{
    FILE* file = fopen("Company.txt", "w+");
    if (file == NULL)
    {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < NumEmployee; i++)
    {
        fprintf(file, "%d,%s,%0.2f,%02d-%02d-%d,%s,%s,%02d-%02d-%d,%s\n",employees[i].id,employees[i].Name,employees[i].salary,employees[i].birthDate.day,employees[i].birthDate.month,employees[i].birthDate.year,employees[i].address,employees[i].mobileNumber,employees[i].enrollmentDate.day,employees[i].enrollmentDate.month,employees[i].enrollmentDate.year,employees[i].email);
    }
    printf("\nChanges have been successfully saved.");

    fclose(file);
}

void QUIT()
{
    printf("WARNING: All changes will be discarded. Are you sure you want to quit? (Yes/No):\n");
    printf("1. Yes\n2. No\n");
    printf("Enter you choice: ");
    int choice;
    checkfordigit(&choice);
    for (int k= 0; choice < 1 || choice > 2; k++)
    {
        printf("ERROR: Please choose a valid number: ");
        checkfordigit(&choice);
        k = 0;
    }

    switch (choice)
    {
    case 1:
        printf("Exiting without saving changes.\n");
        break;
    case 2:
        printf("Do you want to save before quitting? (Yes/No):\n");
        printf("1. Yes\n2. No\n");
        printf("Enter you choice: ");

        int saveChoice;
        checkfordigit(&choice);
        for (int k= 0; choice < 1 || choice > 2; k++)
        {
            printf("ERROR: Please choose a valid number: ");
            checkfordigit(&choice);
            k = 0;
        }


        if (choice == 1)
        {
            SAVE();
        }
        break;
    }
}





