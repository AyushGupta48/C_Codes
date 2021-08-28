// Slide
// slide.c
//
// This program was written by Ayush Gupta (z5362798)


// Version 1.0.0 (2021-03-08): Initial Assignment Release
//Program which acts as a small space invaders type game

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIZE 15
#define EMPTY 0
#define STONE 1

void print_map(int map[SIZE][SIZE], int laser_y);
void laser_fire (int map[SIZE][SIZE], int laser_y);
int laser_movement (int laser_axis, int laser_y);
void shift_left (int map[SIZE][SIZE]);
int loss_check(int count_loss, int loss_condition, int map[SIZE][SIZE]);
void rotation_clockwise (int map[SIZE][SIZE]);
void rotation_anti_clockwise (int map[SIZE][SIZE]);
int rot_counter (int rotation_counter);
void tnt_blocks (int map[SIZE][SIZE], int blast_radius, int row, int col);



int main (void) {

    int block_num, stone_row, stone_col, input_third; //Varibles for stones 
    int laser_configuration, laser_axis; //Variables for laser
    
    //I wasn't too sure how to split the loss and win state variables 
    //into separate functions, so I made all the variables in the main void
    int win_condition = 0;
    int count_loss = 0;
    int win_row = 0;
    int win_col = 0;
    int loss_condition = 0;
    
    int rotation_direction; //Rotation variables
    int rotation_counter = 0;
    
    // This line creates our 2D array called "map" and sets all
    // of the blocks in the map to EMPTY.
    int map[SIZE][SIZE] = {EMPTY};

    // This line creates our laser_y variable. The laser starts in the
    // middle of the map, at position 7.
    int laser_y = SIZE / 2;

    printf("How many blocks? ");
    scanf("%d", &block_num);//Scan in the number of blocks.


    printf("Enter blocks:\n");
    for (int i = 0; i < block_num; i++) { //Enter exact number of blocks needed
        
        scanf("%d %d %d", &stone_row, &stone_col, &input_third);  
                                                        
            
        if (stone_row >= 0 && stone_row <= SIZE-1 
        && stone_col >= 0 && stone_col <= SIZE-1) { 
                
            if (input_third == STONE) { //If it is indeed a stone
                //Print 1 for every valid coordinate inputted
                map[stone_row][stone_col] = STONE; 
            }
                
            else if (input_third >= 4 && input_third <= 9) {
                    
                map[stone_row][stone_col] = input_third;
            }
        }   
            
    }


    print_map(map, laser_y); 
        
    while (scanf("%d", &laser_configuration) == 1) { //Grabs inputs till EOF
        
            
        //If first digit is indeed a 1 --> represents laser movement
        if (laser_configuration == 1) { 
            //Scans 2nd digit and assigns variable
            scanf("%d", &laser_axis); 
            laser_y = laser_movement (laser_axis, laser_y);
            print_map(map, laser_y);
        }
            
        //If first digit is a 2 --> Shootfire (map, la function
        else if (laser_configuration == 2) { 
            laser_fire (map, laser_y);
            print_map(map, laser_y); 
            win_condition = 0; //Reinitialises 
          
        }
        
        //If first digit is a 3--> Shift left function
        else if (laser_configuration == 3) { 
            //Check loss_condition before shifting
            //You cant lose as soon as you start game so this will allow a shift 
            //regardless of stone placement
            if (loss_condition < 1) { 
                shift_left (map); 
                print_map(map, laser_y);
            }
            
            loss_condition = loss_check(count_loss, loss_condition, map);
           
            //Lost game if 2 (2 allows a print of the map as well)
            if (loss_condition == 2) { 
                print_map(map, laser_y); 
                printf("Game Lost!\n");  
                return 0; 
            }
            
        }
        
        //If first digit is a 4--> Rotation
        else if (laser_configuration == 4) {
            //Clockwise or anti 
            scanf("%d", &rotation_direction); 
            
            if (rotation_direction == 1) {
                //Rotation is done once
                rotation_counter = rot_counter(rotation_counter); 
                
                //If 1 then clockwise
                if (rotation_counter == 1) { 
                    rotation_clockwise(map);
                    print_map(map, laser_y);
                }
                
                //Can't do more than once
                else {
                    print_map(map, laser_y); 
                }
                
                
            }
           
            //If 2 then anti 
            else if (rotation_direction == 2) { 
                rotation_counter = rot_counter(rotation_counter);
               
                //Same logic as above
                if (rotation_counter == 1) { 
                    rotation_anti_clockwise(map);
                    print_map(map, laser_y);
                }
               
                else {
                    print_map(map, laser_y);
                }
            }
           
            //If second digit is not a 1 or 2, simply print map 
            else {
                print_map(map, laser_y); 
            }
            
        }
    
        //I could not figure out a way to make a separate function for this 
        //and this way still worked and still seemed clean
        while (win_row < SIZE) { 
            while (win_col < SIZE) {
                win_condition += map[win_row][win_col]; //Adds values
                win_col++;
            }
            win_row++;
            win_col = 0; //reinitialise
        }
        win_row = 0; //reinitialise
        
        //If nothing was added to win_condition in previous while statement,
        //game has won
        if (win_condition == 0) {
            
            printf("Game Won!\n");
            return 0;
        }
        
    }
        
    return 0;
    
    
}

// Print out the contents of the map array. 
// Also print out a > symbol to denote the current laser position.
void print_map(int map[SIZE][SIZE], int laser_y) {
    int i = 0;
    while (i < SIZE) {
        if (i == laser_y) {
            printf("> ");
        } else {
            printf("  ");
        }
        int j = 0;
        while (j < SIZE) {
            printf("%d ", map[i][j]);
            j++;
        }
        printf("\n");
        i++;
    }
}


//Laser moving function
int laser_movement(int laser_axis, int laser_y){ 
    //If laser is within bounds of the boards, besides the top and last row
    if (laser_y > 0 && laser_y < 14) { 
        //If second number is a 1        
        if (laser_axis == 1) { 
            laser_y = laser_y + 1; //Move laser downwards        
        }
        
        //If second number is -1        
        else if (laser_axis == -1) { 
            laser_y = laser_y - 1; //Move laser upwards       
        }    
    }
    
    //If the laser is at the very top of the board        
    else if (laser_y == 0) { 
        if (laser_axis == 1) {
            laser_y = laser_y + 1;
            ///When the laser is at the very top, the only way it can go is down
            //So this allows that to occur
        }
                
        else if (laser_axis == -1) {
            laser_y = laser_y + 0;
            //If the laser is at the top, it can't go more up
            //So the last iteration of the board is printed 
            //(nothing changes from last input)
        }
    }
    
    //Same idea however reversed where when the laser is at the bottom        
    else if (laser_y == 14) { 
        if (laser_axis == 1) {
            laser_y = laser_y + 0;
        }
                
        else if (laser_axis == -1) {
            laser_y = laser_y - 1;
        }
    }
            
    
    //If second number is not a 1 or -1, nothing changes on the board        
    else {
        laser_y = laser_y + 0;
        
    }
    
    return laser_y; 
}


//Firing laser function
void laser_fire (int map[SIZE][SIZE], int laser_y){ 
    int i = 0; //col
    int j = 4; //laser
            
    while (i < SIZE && j > 0) {
        if (map[laser_y][i] == STONE) {
            //Laser fires in the one row however stones in columns are destroyed
            map[laser_y][i] = EMPTY; 
            j--;
        }
        
        else if (map[laser_y][i] >= 4 && map[laser_y][i] <= 9) {
            j = 0;
            tnt_blocks (map, map[laser_y][i], laser_y, i);
        }
        i++;
    }
}


//Shifting all stones left
void shift_left (int map[SIZE][SIZE]){ 
    int mapcopy[SIZE][SIZE];
    for (int col = 0; col < SIZE; col++) {
        for (int row = 0; row < SIZE; row++) {
            mapcopy[col][row] = map[col][row]; 
        }
    }
    
    
    int j = 1; //Only compares columns 1 - 14 since when it reaches 0, game ends
    
    while (j < SIZE) {
        for (int i = 0; i < SIZE; i++) {
            if (map[i][j] == STONE) { 
                map[i][j-1] = 1; //Shifts the 1, 1 column left
                map[i][j] = EMPTY; //Changes the right most column to a 0
            }
            
            else if (map[i][j] >= 4 && map[i][j] <= 9) {
                map[i][j-1] = mapcopy[i][j];
                map[i][j] = EMPTY;
            }
       
        }
        j++;
        
        
    }
}


//Loss Check Function
int loss_check(int count_loss, int loss_condition, int map[SIZE][SIZE]){ 
    while (count_loss < SIZE) { 
        if (map[count_loss][0] >= 1 && map[count_loss][0] <= 9) { 
            loss_condition++; //has to equal 2 to lose
        }
        count_loss++; 
    }
    count_loss = 0; //Reinitialises value of count loss
    
    return loss_condition; 
}


//Rotation Counter function
int rot_counter (int rotation_counter){ 
    
    rotation_counter++;
    
    return rotation_counter;
}


//Rotation AntiClockwise
void rotation_anti_clockwise (int map[SIZE][SIZE]){ 
    //Makes new map to work around map overwriting itself
    int mapcopy[SIZE][SIZE];
    for(int col = 0; col < SIZE; col++) {
        for (int row = 0; row < SIZE; row++) {
            mapcopy[col][row] = map[col][row]; 
        }
    }
    
    for (int col = 0; col < SIZE; col++) {
        for (int row = 0; row < SIZE; row++) {
            //Logic for rotation anticlockwise
            int row_rotate = col; 
            int col_rotate = (SIZE - 1) - row; 
            map[row][col] = mapcopy[row_rotate][col_rotate];
        }
    }
     
}     


//Rotation Clockwise
void rotation_clockwise (int map[SIZE][SIZE]){
    
    int mapcopy[SIZE][SIZE];
    for (int col = 0; col < SIZE; col++) {
        for (int row = 0; row < SIZE; row++) {
            mapcopy[col][row] = map[col][row]; //Same idea as above
        }
    }
    int j = 0;
    while (j < SIZE) {
        for (int i = 0; i < SIZE; i++) {
            //Logic rotation clockwise
            int col_rotate = i; 
            int row_rotate = (SIZE - 1) - j; 
            map[i][j] = mapcopy[row_rotate][col_rotate];
        }
        j++;
    }
    
}

void tnt_blocks (int map[SIZE][SIZE], int blast_radius, int row, int col){
    for (int col_counter = col - blast_radius; col_counter <= col + blast_radius; 
    col_counter++) {
        for (int row_counter = row - blast_radius; row_counter <= row + blast_radius;
        row_counter++) {
            //Radius Size
            int col_radius = col - col_counter;
            int row_radius = row - row_counter;
            
            //Euclidean distance
            if (col_radius * col_radius + row_radius * row_radius 
            < blast_radius * blast_radius && row_counter < SIZE &&
            col_counter < SIZE && row_counter >= 0 && col_counter >= 0) {
                map[row_counter][col_counter] = EMPTY;
            }
        }   
    }
}




