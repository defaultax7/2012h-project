#include "performance.h"

Performance::Performance(int total_note, int perfect, int miss, int good, int combo, Score score) : total_note(total_note) , perfect(perfect) , miss(miss) , good(good) , highest_combo(combo) , score(score)
{

}

Performance::rank Performance::get_rank() const
{
    if(total_note == perfect){
        // all perfect
        return S;
    }else if(total_note == highest_combo){
        // full combo
        return A;
    }else if( ((double)miss)/total_note < 0.1){
        // hit 90% of the notes
        return B;
    }else if( ((double)miss)/total_note < 0.3){
        // hit 70% of the notes
        return C;
    }else{
        return D;
    }

}
