#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_SKILLS 10
#define MAX_PROJECTS 5

struct Project
{
    char name[50];
    char domain[20];
    int project_score;
};

struct Resume
{
    char name[50];
    char skills[MAX_SKILLS][20];
    struct Project projects[MAX_PROJECTS];
    int num_skills;
    int num_projects;
    float cgpa;
    int total_score;
    int total_skill_score;   // New: Total skill score
    int total_project_score; // New: Total project score
    int skill_scores[MAX_SKILLS];
};

struct JobRequirements
{
    char skills[MAX_SKILLS][20];
    int skill_scores[MAX_SKILLS];
    int num_skills;
};

struct Node
{
    struct Resume data;
    struct Node *next;
};

struct LinkedList
{
    struct Node *head;
};

struct LinkedList *createLinkedList()
{
    struct LinkedList *newList = (struct LinkedList *)malloc(sizeof(struct LinkedList));
    if (newList == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    newList->head = NULL;
    return newList;
}

void addNode(struct LinkedList *list, struct Resume data)
{
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->next = list->head;
    list->head = newNode;
}

void swapResumeData(struct Node *node1, struct Node *node2)
{
    struct Resume tempData = node1->data;
    node1->data = node2->data;
    node2->data = tempData;
}

void rankResumes(struct LinkedList *list)
{
    int swapped;
    struct Node *ptr1;
    struct Node *lptr = NULL;

    if (list->head == NULL)
        return;

    do
    {
        swapped = 0;
        ptr1 = list->head;

        while (ptr1->next != lptr)
        {
            if (ptr1->data.total_score < ptr1->next->data.total_score)
            {
                swapResumeData(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

struct Node *searchByName(struct LinkedList *list, const char *name)
{
    struct Node *current = list->head;
    while (current != NULL)
    {
        if (strcmp(current->data.name, name) == 0)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

struct Node *searchBySkill(struct LinkedList *list, const char *skill, struct Node *foundResumes[], int *foundCount)
{
    struct Node *current = list->head;
    *foundCount = 0;

    while (current != NULL)
    {
        for (int i = 0; i < current->data.num_skills; ++i)
        {
            if (strcmp(current->data.skills[i], skill) == 0)
            {
                foundResumes[*foundCount] = current;
                (*foundCount)++;
                break;
            }
        }
        current = current->next;
    }

    return *foundCount > 0 ? foundResumes : NULL;
}

void addProjects(struct Resume *resume, const struct JobRequirements *job)
{
    resume->total_project_score = 0; // Initialize total project score

    printf("\nEnter the number of projects for %s (up to %d): ", resume->name, MAX_PROJECTS);
    scanf("%d", &resume->num_projects);

    for (int j = 0; j < resume->num_projects && j < MAX_PROJECTS; ++j)
    {
        printf("\nEnter the details of project %d for %s:\n", j + 1, resume->name);

        char projectName[50];
        int duplicate;
        do
        {
            duplicate = 0;
            printf("Project Name: ");
            scanf("%s", projectName);

            for (int k = 0; k < j; ++k)
            {
                if (strcmp(resume->projects[k].name, projectName) == 0)
                {
                    printf("Project '%s' is already listed.\n", projectName);
                    duplicate = 1;
                    break;
                }
            }
        } while (duplicate);

        strcpy(resume->projects[j].name, projectName);

        printf("Project Domain: ");
        scanf("%s", resume->projects[j].domain);

        if (strcmp(resume->projects[j].domain, "dsa") == 0)
        {
            resume->projects[j].project_score = 60;
        }
        else if (strcmp(resume->projects[j].domain, "ai") == 0)
        {
            resume->projects[j].project_score = 50;
        }
        else if (strcmp(resume->projects[j].domain, "cn") == 0)
        {
            resume->projects[j].project_score = 40;
        }
        else if (strcmp(resume->projects[j].domain, "os") == 0)
        {
            resume->projects[j].project_score = 30;
        }
        else if (strcmp(resume->projects[j].domain, "web") == 0)
        {
            resume->projects[j].project_score = 20;
        }
        else
        {
            resume->projects[j].project_score = 0;
            printf("Domain not matching with predefined domains.\n");
        }

        resume->total_project_score += resume->projects[j].project_score; // Update total project score
    }
}

void calculateSkillScores(struct Resume *resume, const struct JobRequirements *job)
{
    resume->total_skill_score = 0; // Initialize total skill score

    for (int i = 0; i < resume->num_skills; ++i)
    {
        resume->skill_scores[i] = 0; // Initialize skill score to zero
        for (int j = 0; j < job->num_skills; ++j)
        {
            if (strcmp(resume->skills[i], job->skills[j]) == 0)
            {
                resume->skill_scores[i] = job->skill_scores[j];
                resume->total_skill_score += job->skill_scores[j]; // Update total skill score
                break;
            }
        }
    }
}

void searchMenu(struct LinkedList *list)
{
    while (1)
    {
        printf("\nSearch Menu:\n");
        printf("1. Search by Name\n");
        printf("2. Search by Skill\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");

        int choice;
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            char searchName[50];
            printf("Enter the name to search: ");
            scanf("%s", searchName);

            struct Node *foundResume = searchByName(list, searchName);
            if (foundResume != NULL)
            {
                printf("Resume found:\n");
                printf("Name: %s, Skills: ", foundResume->data.name);
                for (int i = 0; i < foundResume->data.num_skills; ++i)
                {
                    printf("%s ", foundResume->data.skills[i]);
                }
                printf("Total Score: %d\n", foundResume->data.total_score);
            }
            else
            {
                printf("Resume with name '%s' not found.\n", searchName);
            }
            break;
        }

        case 2:
        {
            char searchSkill[20];
            printf("\nEnter the skill to search: ");
            scanf("%s", searchSkill);

            struct Node *foundResumes[MAX_SKILLS];
            int foundCount = 0;

            if (searchBySkill(list, searchSkill, foundResumes, &foundCount) != NULL)
            {
                printf("\nResumes found with skill '%s':\n", searchSkill);
                for (int i = 0; i < foundCount; ++i)
                {
                    printf("Name: %s, Skills: ", foundResumes[i]->data.name);
                    for (int j = 0; j < foundResumes[i]->data.num_skills; ++j)
                    {
                        printf("%s ", foundResumes[i]->data.skills[j]);
                    }
                    printf("Total Score: %d\n", foundResumes[i]->data.total_score);
                }
            }
            else
            {
                printf("No resumes found with skill '%s'.\n", searchSkill);
            }
            break;
        }
        case 3:
            exit(EXIT_SUCCESS);
            break;

        default:
            printf("Invalid choice.\n");
        }
    }
}

void displayRankedResumes(struct LinkedList *list, struct JobRequirements *job)
{
    system("cls");
    printf("\nRanked Resumes:\n");
    struct Node *current = list->head;

    FILE *file = fopen("record.txt", "w");
    if (file == NULL)
    {
        printf("Error opening file for writing.\n");
        return;
    }

    while (current != NULL)
    {
        printf("\nName: %s\nCGPA: %.2f\n\nSkills:", current->data.name, current->data.cgpa);
        for (int i = 0; i < current->data.num_skills; ++i)
        {
            printf("%s ", current->data.skills[i]);
        }
        printf("\nProjects: ");
        for (int j = 0; j < current->data.num_projects; ++j)
        {
            printf("%s (%s) ", current->data.projects[j].name, current->data.projects[j].domain);
        }

        printf("\n\nSkill Scores:\n");
        for (int i = 0; i < current->data.num_skills; ++i)
        {
            printf("%s: %d\n", current->data.skills[i], current->data.skill_scores[i]);
        }

        printf("\nProject Scores:\n");
        for (int j = 0; j < current->data.num_projects; ++j)
        {
            printf("%s (Domain: %s): %d\n", current->data.projects[j].name,
                   current->data.projects[j].domain, current->data.projects[j].project_score);
        }

        printf("\nTotal Skill Score: %d\n", current->data.total_skill_score);
        printf("Total Project Score: %d\n", current->data.total_project_score);

        printf("\n");

        fprintf(file, "\nName: %s\nCGPA: %.2f\n\nSkills:", current->data.name, current->data.cgpa);
        for (int i = 0; i < current->data.num_skills; ++i)
        {
            fprintf(file, "%s ", current->data.skills[i]);
        }
        fprintf(file, "\nProjects: ");
        for (int j = 0; j < current->data.num_projects; ++j)
        {
            fprintf(file, "%s (%s) ", current->data.projects[j].name, current->data.projects[j].domain);
        }

        fprintf(file, "\n\nSkill Scores:\n");
        for (int i = 0; i < current->data.num_skills; ++i)
        {
            fprintf(file, "%s: %d\n", current->data.skills[i], current->data.skill_scores[i]);
        }

        fprintf(file, "\nProject Scores:\n");
        for (int j = 0; j < current->data.num_projects; ++j)
        {
            fprintf(file, "%s (Domain: %s): %d\n", current->data.projects[j].name,
                    current->data.projects[j].domain, current->data.projects[j].project_score);
        }

        fprintf(file, "\nTotal Skill Score: %d\n", current->data.total_skill_score);
        fprintf(file, "Total Project Score: %d\n", current->data.total_project_score);

        fprintf(file, "\n");

        current = current->next;
    }

    fclose(file);
}

void freeLinkedList(struct LinkedList *list)
{
    struct Node *current = list->head;
    while (current != NULL)
    {
        struct Node *next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

int main()
{
    struct JobRequirements job = {
        .num_skills = 4,
        .skills = {"dsa", "os", "cn", "ai"},
        .skill_scores = {50, 20, 30, 40}};

    struct LinkedList *resumeList = createLinkedList();

    printf("\t\t\t\t*****************WELCOME TO RESUME SELECTOR*****************\n\n");
    printf("\n\t\t\t\t\t #please enter all the information in lowercase#\n\n");
    printf("\nEnter the number of resumes of candidate (up to %d): ", MAX_SKILLS);
    int num_candidates;
    scanf("%d", &num_candidates);

    for (int i = 0; i < num_candidates && i < MAX_SKILLS; ++i)
    {
        struct Resume newResume;
        printf("\nEnter the name of candidate %d: ", i + 1);
        scanf("%49s", newResume.name); // Limit input to buffer size

        printf("Enter the CGPA for %s: ", newResume.name);
        scanf("%f", &newResume.cgpa);

        if (newResume.cgpa < 8.0)
        {
            printf("CGPA must be greater than or equal to 8 to enter skills and projects.\n");
            continue;
        }

        printf("\nEnter the number of skills for %s (up to %d): ", newResume.name, MAX_SKILLS);
        scanf("%d", &newResume.num_skills);

        // Input unique skills for the resume
        printf("Enter unique skills (one per line) for %s:\n", newResume.name);
        for (int j = 0; j < newResume.num_skills; ++j)
        {
            char skill[20];
            printf("Skill %d: ", j + 1);
            scanf("%s", skill);

            // Check for duplicate skills
            int duplicate = 0;
            for (int k = 0; k < j; ++k)
            {
                if (strcmp(newResume.skills[k], skill) == 0)
                {
                    printf("Skill '%s' is already listed.\n", skill);
                    duplicate = 1;
                    break;
                }
            }

            if (duplicate)
            {
                j--; // Decrement j to allow the user to input the skill again
                continue;
            }

            strcpy(newResume.skills[j], skill);
        }

        // Add projects to the resume
        addProjects(&newResume, &job);

        // Calculate skill scores
        calculateSkillScores(&newResume, &job);

        newResume.total_score = newResume.total_skill_score + newResume.total_project_score; // Calculate total score

        addNode(resumeList, newResume);
    }

    // Rank resumes based on total scores
    rankResumes(resumeList);

    // Display ranked resumes
    displayRankedResumes(resumeList, &job);

    // Search menu
    searchMenu(resumeList);

    // Free allocated memory
    freeLinkedList(resumeList);

    return 0;
}
