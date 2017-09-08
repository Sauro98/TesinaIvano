#include "queue.h"

// class constructor
Queue::Queue(int n)
{
    array = new node_f*[n];
    max_length = n;
    length = 0;
}

// class destructor
Queue::~Queue()
{
	free(array);
}

//adds an element
void Queue::enqueue(node_f* new_node){
    if(length >= max_length){
        println("Insertion error: queue is full");
        return;
    }
    array[length] = new_node;
    length++;
    return;
}

//removes an element
node_f* Queue::dequeue(){
    if(length == 0){
        println("Dequeuing error: queue is empty");
        return NULL;
    }
    node_f* to_return = array[0];
    length = length -1;
    for(int index = 0;index < length;index++){
        array[index] = array[index + 1];
    }
    return to_return;
}
//return an element without removing it
node_f* Queue::peek(){
    if(length == 0){
        println("Peek error: queue is empty");
        return NULL;
    }
    return array[0];
}
//returns the current array lenght
int Queue::get_length(){
    return length;
}

void Queue::print_queue(){
    println("QUEUE");
    for(int a = 0;a <length;a++){
        val_f vf = array[a]->c_f;
        print("char at "<<a);
        int c = vf.value;
        print("  "<<(int)c);
        print(" "<< c);
        int f = vf.frequence;
        println(" frequence "<< f);
    }    
    println("QUEUE END"); 
    return;
}
