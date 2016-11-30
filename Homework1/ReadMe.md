## Implement thread local variables in C++

In Java, ThreadLocal variables are provided via a class. The class has get and set methods. The initial value of a ThreadLocal variable is obtained by calling the initialValue method, which is executed the first time a get method on the variable is called by a thread, unless a set method has already been called. The method returns null, but can be overridden to behave as desired. Because ThreadLocals are classes in Java, primitive types cannot be made thread_local. There is also a remove method which removes the current thread's value for this thread-local variable.  

Thread local variables in Java and C++ are implemented by maintaining a data structure within a thread that maps threadlocal variables to the value. Whenever a thread is terminated, the thread’s instances are handled according to the way things are done in that language.  

For this assignment, we are going to implement our own class to implement thread local variables in C++. There is no reason to do this rather than use the ones provided by C++, except this is an excellent exercise that also makes us aware of the underlying implementation so that we can make better cost benefit tradeoffs in using thread local variables.
