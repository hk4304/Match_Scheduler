#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int no_of_teams;

void constrain_Input();

//graph initialized
typedef struct 
{
    int numVertices;
    int** adjacencyMatrix;
} Graph;

//function for creating and memory allocating graph
Graph* createGraph(int numVertices);

//function that makes edges to represent match of every team with every other team
void match_initialize(Graph* graph);


//function to check if there's an edge
int has_edge(Graph* graph,int from_node,int to_node);

//day node
struct list
{
    int day_no;
    struct list* next;
} ;
typedef struct list day_node;

//days list graph
typedef struct 
{
    int no_of_teams;
    day_node** team_number;
} day_list;

//function to create and initialize graph which check the days schedules of each team
day_list* create_n_initialize_day_list(int no_of_teams);


//function to check if the day is available for matching (no other match of team on same day)
int available(int day,int team_number,day_list* days_list);

//function to add a day in the teams reserved days list (day_list[team_number])
void add_day(int day, day_list* days_list, int team_number);

//function that returns a day which is available for both team 1 and team2
int assign_day(int team_1,int team_2,day_list* days_list);

//function for scheduling the matches that is function for assigning days to each [team1][team2]combination
void greedy_scheduler(Graph* graph , day_list* days_list);

//function to find the no of days it took for matches to complete
int no_of_days(Graph* graph);

// function to print matches day wise
void print_schedule(Graph* graph,int numDays);

int main()
{
    constrain_Input();
    Graph * graph = createGraph(no_of_teams);
    match_initialize(graph);
    day_list* days_list = create_n_initialize_day_list(no_of_teams);
    //till here function is working fine 
    //the problem is in one of the below functions
    greedy_scheduler(graph, days_list);
    int numDays = no_of_days(graph);
    int total_matches = (no_of_teams*(no_of_teams-1))/2;

    printf("total no of matches :%d\n",total_matches);

    printf("no of days it will take to finish all the matches :%d\n",(numDays+1));
    print_schedule(graph, numDays);
    return 0;
}


//contrain input function
void constrain_Input()
{
    printf("Tournament type << ROUND ROBIN >>\n");
    printf("algorithm used for scheduling matches : \"Greedy-Algorithm\"\n");
    printf("Enter the number of teams: ");
    scanf("%d", &no_of_teams);
    while (no_of_teams <= 1) 
    {
        printf("Enter the number of teams (should be greater than 1): ");
        scanf("%d", &no_of_teams);
    }
}

//function for creating and memory allocating graph
Graph* createGraph(int numVertices)
{
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (graph == NULL)
    {
        printf("Memory allocation failed. Unable to create the graph.\n");
        return NULL;
    }

    graph->numVertices = numVertices;
    graph->adjacencyMatrix = (int**)malloc(numVertices * sizeof(int*));

    if (graph->adjacencyMatrix == NULL) 
    {
        printf("Memory allocation failed. Unable to create the adjacency matrix.\n");
        free(graph);
        return NULL;
    }

    for (int i = 0; i < numVertices; i++) 
    {
        graph->adjacencyMatrix[i] = (int*)calloc(numVertices , sizeof(int));

        if (graph->adjacencyMatrix[i] == NULL) 
        {
            printf("Memory allocation failed. Unable to create the adjacency matrix.\n");
            free(graph->adjacencyMatrix);
            free(graph);
            return NULL;
        }
    }

    return graph;
}

//function that makes edges to represent match
void match_initialize(Graph* graph) 
{
    for (int i = 0; i < graph->numVertices; i++) 
    {
        for (int j = i + 1; j < graph->numVertices; j++) 
        {
            graph->adjacencyMatrix[i][j] = 1;
        }
    }
}


//function to check if there's an edge
int has_edge(Graph* graph,int from_node,int to_node)
{
    int check;
    if(graph->adjacencyMatrix[from_node][to_node]==1)
    {
        check=1;
        return check;
    }
    else 
    {
        check=0;
        return check;
    }
}

//function to create and initialize graph which check the days schedules of each team
day_list* create_n_initialize_day_list(int no_of_teams) 
{
    day_list* newDayList = (day_list*)malloc(sizeof(day_list));
    if (newDayList == NULL) 
    {
        printf("Memory allocation failed. Unable to initialize day list.\n");
        return NULL;
    }
    newDayList->no_of_teams = no_of_teams;
    newDayList->team_number = (day_node**)malloc(no_of_teams * sizeof(day_node*));
    if (newDayList->team_number == NULL) 
    {
        printf("Memory allocation failed. Unable to initialize scheduling list.\n");
        free(newDayList);
        return NULL;
    }
    
    // Initialize each day's scheduling list to be empty.
    for (int i = 0; i < no_of_teams; i++) 
    {
        newDayList->team_number[i] = NULL;
    }
    
    return newDayList;
}


//function to check if the day is available for matching (no other match of team on same day)
int available(int day,int team_number,day_list* days_list)
{
    int a=-1;
    day_node* current = days_list->team_number[team_number];
    while (current!=NULL)
    {
        if(current->day_no == day)
        {
            a=0;
            return a;
        }
        else
        {
            current=current->next;
        }
    }
    a=1;
    return a;
}

//function to add a day in the teams reserved days list (day_list[team_number])
void add_day(int day, day_list* days_list, int team_number)
{
    day_node* node = (day_node*)malloc(sizeof(day_node));
    node->day_no = day;
    node->next = days_list->team_number[team_number];
    days_list->team_number[team_number] = node;
}


//function that returns a day which is available for both team 1 and team2
int assign_day(int team_1, int team_2, day_list* days_list)
{
    for (int i = 0; i < 1000; i++)
    {
        if (available(i, team_1, days_list) && available(i, team_2, days_list))
        {
            add_day(i, days_list, team_1);
            add_day(i, days_list, team_2);
            return i;
        }
    }
    return -1; //returning -1 to indicate an error.
}


//function for scheduling the matches that is function for assigning days to each [team1][team2]combination
void greedy_scheduler(Graph* graph, day_list* days_list)
{
    int limit = graph->numVertices;
    for (int team_1 = 0; team_1 < limit; team_1++)
    {
        for (int team_2 = team_1 + 1; team_2 < limit; team_2++)
        {
            int day = assign_day(team_1, team_2, days_list);
            graph->adjacencyMatrix[team_1][team_2] = day;
            graph->adjacencyMatrix[team_2][team_1] = day;  // Symmetric, so set both sides.
        }
    }
}



//function to find the no of days it took for matches to complete
int no_of_days(Graph* graph)
{
    int numTeams = graph->numVertices;
    
    // to find the maximum day assigned to any match
    int No_ofDays = 0;  // Initialize to 0

    for (int i = 0; i < numTeams; i++) 
    {
        for (int j = 0; j < numTeams; j++) 
        {
            if (i != j && graph->adjacencyMatrix[i][j] > No_ofDays)
            {
                No_ofDays = graph->adjacencyMatrix[i][j];
            }
        }
    }

    return No_ofDays;
}

// Function to print matches day wise
void print_schedule(Graph* graph, int numDays)
{
    int num_of_teams = graph->numVertices;

    for (int day = 0; day <= numDays; day++) 
    {
        printf("Day %d:\n", day+1);
        int matchesPrinted = 0;

        for (int i = 0; i < num_of_teams; i++) 
        {
            for (int j = i + 1; j < num_of_teams; j++) 
            {
                if (graph->adjacencyMatrix[i][j] == day) 
                {
                    printf("Match: Team %d vs Team %d\n", i + 1, j + 1);
                    matchesPrinted = 1;
                }
            }
        }

        if (matchesPrinted == 0) 
        {
            printf("No matches scheduled for this day.\n");
        }
        
        printf("\n");
    }
}
