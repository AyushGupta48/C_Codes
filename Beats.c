// Assignment 2 21T1 COMP1511: Beats by CSE
// beats.c
//
// This program was written by Ayush Gupta (z5362798)
// on April, 2021
//
// Version 1.0.0: Assignment released.

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Add any extra #includes your code needs here.

#include "beats.h"

// Add your own #defines here.

//////////////////////////////////////////////////////////////////////

// You don't have to use the provided struct track, you are free to
// make your own struct instead.
// If you use the provided struct track, you will have to add fields
// to store other information.

struct track {
    // TODO: You will have to add extra fields here in Stage 2.
    
    struct beat *first;
    struct beat *current;
    int status;
};

// You don't have to use the provided struct beat, you are free to
// make your own struct instead.
// If you use the provided struct beat, you may add fields
// to it to store other information.

struct beat {
    // You may choose to add or change fields in this struct.
    
    struct note *notes;
    struct beat *next;
    struct note *first;
};

// You don't have to use the provided struct note, you are free to
// make your own struct instead.
// If you use the provided struct note, you add fields
// to it to store other information.

struct note {
    // You may choose to add or change fields in this struct.
    int octave;
    int key;
    struct note *next;
};

// Add any other structs you define here.

//////////////////////////////////////////////////////////////////////

// Add prototypes for any extra functions you create here.



// Return a malloced Beat with fields initialized.
Beat create_beat(void) {
    Beat new_beat = malloc(sizeof (struct beat));
    assert(new_beat != NULL);

    new_beat->next = NULL;
    new_beat->notes = NULL;

    // You do not need to change this function, unless you change
    // the implementation of struct beat.

    return new_beat;
}

// You need to implement the following functions.
// You can find descriptions of what each function should do in beats.h

//////////////////////////////////////////////////////////////////////
//                        Stage 1 Functions                         //
//////////////////////////////////////////////////////////////////////

// Add a note to the end of a beat.
int add_note_to_beat(Beat beat, int octave, int key) {

    if (octave < 0 || octave >= 10) {
        return INVALID_OCTAVE;
    }
    
    if (key < 0 || key >= 12) {
        return INVALID_KEY;
    }
    
    
    //makes struct for new note
    struct note *new_note = malloc(sizeof (struct note)); 
    new_note->octave = octave;
    new_note->key = key;
    new_note->next = NULL; //initialises next note
    
    struct note *current_note = beat->notes;
    
    if (beat->notes == NULL) {
        beat->notes = new_note;
    }
    
    else {
            
        //Check note exists
        while (current_note->next != NULL) {
            current_note = current_note->next;
        }
        
        if (octave < current_note->octave)
        {
            return NOT_HIGHEST_NOTE;
        }
        
        else if (octave == current_note->octave && key <= current_note->key)
        {
            return NOT_HIGHEST_NOTE;
        }
        
        //If conditions are met, then add new note to list
        else
        {
            current_note->next = new_note;
        }
    }
    
    return VALID_NOTE;
    
}

// Print the contents of a beat.
void print_beat(Beat beat) {
    struct note *new_note = beat->notes;
    
    while (new_note != NULL) {
        
        printf("%d %02d ", new_note->octave, new_note->key);
        
        if (new_note->next != NULL) {
            
            printf("| ");
            
        }
        
        new_note = new_note->next;
    }
    
    printf("\n");
 
}

// Count the number of notes in a beat that are in a given octave.
int count_notes_in_octave(Beat beat, int octave) {    
    
    struct note *current_note = beat->notes;
    int counter = 0;
    
    while (current_note != NULL) {
        
        if (current_note->octave == octave) {
            counter++;
        }
        
        current_note = current_note->next;
    }
    
    return counter;
}

//////////////////////////////////////////////////////////////////////
//                        Stage 2 Functions                         //
//////////////////////////////////////////////////////////////////////

// Return a malloced track with fields initialized.
Track create_track(void) {    
    
    Track new_track = malloc(sizeof (struct track));
        
    new_track->first = NULL;
    new_track->current = NULL;
    
    return new_track;
}
    
// Add a beat after the current beat in a track.
void add_beat_to_track(Track track, Beat beat) {
   
    //If there is no beat, new beat is first
    if (track->current == NULL) {
    
        beat->next = track->first;
        track->first = beat;
        
    }
    
    //beat is added and current beat is incremented
    else {
    
        beat->next = track->current->next;
        track->current->next = beat;
        
    }
}

// Set a track's current beat to the next beat.
int select_next_beat(Track track) {

    //All conditions are met for TRACK_STOPPED and TRACK_PLAYING
    
    if (track->first == NULL) {

        return TRACK_STOPPED;
        
    }
    
    if (track->first != NULL && track->current == NULL) {
        
        track->current = track->first;
       
        return TRACK_PLAYING;
        
    }
    
    else if (track->current->next != NULL) {
    
        track->current = track->current->next;
 
        return TRACK_PLAYING;
        
    }
    
    else if (track->current->next == NULL) {
    
        track->current = track->current->next;
    
        return TRACK_STOPPED;
        
    }
    
    
    
    else {

        return TRACK_STOPPED;
        
    }
    
}

// Print the contents of a track.
void print_track(Track track) {
    
    Beat current_beat = track->first;
    int index_counter = 0;
    
    if (track->first == NULL) {
    
        return;
        
    }
    
    //Print relevant counter 
    while (current_beat != NULL) {
    
        index_counter++;
        
        if (current_beat == track->current) {
        
            printf("> [%d] ", index_counter);
            
            if (current_beat == NULL) {
            
                index_counter++;
                
            }
            
            else {
            
                print_beat(current_beat);
                
            }
        }
        
        //Take away the '>'
        
        else {
        
            printf(" [%d] ", index_counter);
            
            if (current_beat == NULL) {
            
                index_counter++;
                
            }
            
            else {
            
                print_beat(current_beat);
                
            } 
        }
        
        current_beat = current_beat->next;
        
    }
        
}

// Count beats after the current beat in a track.
int count_beats_left_in_track(Track track) {
   
    int beats_counter = 0;

    Beat current_beat = track->current;
    
    if (track->first == NULL) {
    
        return beats_counter;
        
    }
    
    //Following beats.h, Should the track be 'stopped',
    //this function should return the number of beats in the track
    
    else if (current_beat == NULL) {
    
        current_beat = track->first;
        
        while (current_beat != NULL) {
        
            beats_counter++;
            
            current_beat = current_beat->next;
            
        }
    }
    
    while (current_beat != NULL) {
    
        current_beat = current_beat->next;
        
        if (current_beat != NULL) {
        
            beats_counter++;
            
        }
    }
    
    return beats_counter;
}

//////////////////////////////////////////////////////////////////////
//                        Stage 3 Functions                         //
//////////////////////////////////////////////////////////////////////

// Free the memory of a beat, and any memory it points to.
void free_beat(Beat beat) {
    // Note: there is no printf in this function, as the
    // Stage 1 & 2 autotests call free_beat but don't check whether
    // the memory has been freed (so this function should do nothing in
    // Stage 1 & 2, rather than print an error).
    
    //Modify Marc Chee's lecture slides for these!!
    
    struct note *previous_beat = NULL;
    struct note *current = beat->notes;
    
    while (current != NULL) {
        previous_beat = current;
        current = current->next;
        free(previous_beat);
    }
    
    free(beat);
}

// Free the memory of a track, and any memory it points to.
void free_track(Track track) {
    // Note: there is no printf in this function, as the
    // Stage 1 & 2 autotests call free_track but don't check whether
    // the memory has been freed (so this function should do nothing in
    // Stage 1 & 2, rather than print an error).
    
    //Modify Marc Chee's lecture slides for these!!
    
    Beat previous_track = NULL;
    Beat current_track = track->first;
    
    while (current_track != NULL) {
        previous_track = current_track;
        current_track = current_track->next;
        free_beat(previous_track);
    }
    
    free(track);
}

// Remove the currently selected beat from a track.
int remove_selected_beat(Track track) {
    
    Beat previous_beat = NULL;
    Beat current_beat = track->first;
    Beat memory = track->current;
    
    int track_state = 0;
    
    //Do nothing and return TRACK_STOPPED
    if (track->current == NULL) {
        return TRACK_STOPPED;
     
    }
    
    //Searching what to delete
    while (current_beat != track->current) {
        previous_beat = current_beat;
        current_beat = current_beat->next;
    }
    
    if (previous_beat == NULL) {
        track->first = track->current->next;
        
        track_state = select_next_beat(track);
    }
    
    else {
        previous_beat->next = current_beat->next;
        track_state = select_next_beat(track);
    }
    
    free_beat(memory);
    return track_state;
}
