## Implement singly-linked list
Suppose that there are three kinds of operations on singly-linked list: 
* Search 
* Insert
* Remove

Searching threads merely examine the list; hence they can execute concurrently with each other.   
Inserting threads add new items to the front of the list; insertions must be mutually exclusive to preclude two inserters from inserting new items at about the same time.   
However, one insert can proceed in parallel with any number of searches.   
Finally, the remove operation removes items from anywhere in the list.   
At most one thread can remove items at a time, and a remove must also be mutually exclusive with searches and inserts.


