# Milestone
Course project for Data Structure.  
Containers like ***vector, list, deque, stack, queue, priority\_queue, map*** and ***unordered\_map*** were implemented.  
Also, some generic algorithms like ***sort, list\_sort*** and ***make\_heap*** were done at last.  
All the code were checked with valgrind to ensure that there's no memory leak. Suggestions are welcome!

## Implementation description
###vector
&emsp;The key parts of a vector are the ___erase___ and ___insert___ operations, whose time complexity are O(n).  
&emsp;Attentation should be paid to the so called ___"deep copy"___ operations when moving the objects inside or using the assignment operator, since objects may have pointers pointing to dynamic memory outside the vector.  
&emsp;To handle this, I generally calls the placement new operator to construct an new object in target address and calls the destructor to desturct the original object later.  
&emsp;Additionally, The ___iterator___ I design can check the validility of itself, and the time complexity is ___O(1)___. That is, given a pointer pointing to a vector, the ___isValid___ function can tell if the iterator is ascripted to the given vector, and if the iterator is still valid after insert or erase operations were taken, since these operations may cause the vector to allocate a larger memory block and to release the original one.

###list
&emsp;The key parts of a list are the ___erase___ and ___insert___ functions, whose time complexity are O(1). Proper construction, destruction and memory management plays a significant role in the implementation as well.  
&emsp;With an additional node named "___last___" inside each list, it's much easier to indicate the end() position, which is post to the last data node. Also, we benefit a lot from this structure when implementing other member functions.  
&emsp;Although the erase and insert operations on certain data node won't cause iterators pointing to other data nodes to be invalid,  I implement the ___isValid___ function inside the ___iterator___ to check if the iterator is ascripted to the given list and if the iterator is still valid after inserting or erasing with ___O(n)___ time complexity.

###deque
&emsp;The key parts of a list are the ___erase___ and ___insert___ functions.

###stack
&emsp;Actually, stack is an ___adapter___ instead of a container. It's based on the ___vector___ or ___list___ so that the ___push___, ___pop___ and ___top___ can be realized easily by calling corresponding functions in specified container. Also, we needn't to worry about the memory management since it has been guaranteed before.  
&emsp;Since the elements inside a stack are not allowed to be accessed, we don't have to provide relevant iterator.

###queue
&emsp;Similarly, queue is also an ___adapter___ instead of a container. As we need to support operations on the front, vector is not suitable any more. Thus, it was implemented with the ___list___ we created before. Operations like ___push___, ___pop___ and ___front___ can be done easily by calling their counterparts in list.  
&emsp;Since the elements inside a queue are not premitted to be traversed, we needn't to provide relevant iterator.

###priority_queue
&emsp;Here I implemented the priority_queue with both ___binary heap___ and ___fibonacci heap___.  The difference is that binary heap supports all the oridinary operations except the ___merge___ operation while fibonacci heap can handle it well.  
&emsp;For a binary heap, when the top element get extracted, a ___percolateDown___ process will be taken to select the new front element and to re-balance the heap. Similarly, a ___percolateUp___ process will be applied after a new element was appended to the back so that it can be properly placed. Both of these two operations have logarithmic complexity, so the time cost of the ___pop___ and ___push___ operations are ___O(log(n))___.  
&emsp;Theoretically, the time consumption of the __pop___ operation for a fibonacci heap is ___O(log(n))___ and the ___push___ operation is ___O(1)___ since fibonacci heap adopts a "___lazy___" strategy to store new elements. That is, it simply inserts the element into its data list when a new element comes, so the time consumption is constant. However, a ___consolidate___ process will be taken after the root node was extracted, which effectively re-ordered the heap and takes logarithmic time complexity.  
&emsp;Naturally, iterator is not needed for this data structure as the traversal of a heap is meaningless.

###map

###unordered_map

## Declaration
The framework of this project is based on the course project of DS2016, ACM Class, SJTU. I'm full of admiration for the wonderful work they've done!
