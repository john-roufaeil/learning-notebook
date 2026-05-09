# Java Concurrency & Thread Manipulation - Complete Guide

## What is Concurrency?

**Concurrency** is the ability to run multiple tasks simultaneously. In Java, this is achieved through **threads** - independent paths of execution within a program.

## Why Use Concurrency?

1. **Better Performance**: Utilize multiple CPU cores
2. **Responsiveness**: Keep UI responsive while doing background work
3. **Resource Sharing**: Multiple tasks can share the same resources
4. **Efficiency**: Don't wait for slow operations (I/O, network)

---

# Part 1: Creating and Starting Threads

## Method 1: Extending Thread Class

```java
// Define a thread by extending Thread class
public class MyThread extends Thread {
    private String threadName;
    
    public MyThread(String name) {
        this.threadName = name;
    }
    
    @Override
    public void run() {
        // Code that runs in separate thread
        for (int i = 1; i <= 5; i++) {
            System.out.println(threadName + ": " + i);
            try {
                Thread.sleep(1000);  // Sleep for 1 second
            } catch (InterruptedException e) {
                System.out.println(threadName + " interrupted");
            }
        }
        System.out.println(threadName + " finished");
    }
}

// Usage
public class Main {
    public static void main(String[] args) {
        MyThread thread1 = new MyThread("Thread-1");
        MyThread thread2 = new MyThread("Thread-2");
        
        thread1.start();  // Start thread 1
        thread2.start();  // Start thread 2
        
        System.out.println("Main thread continues...");
    }
}

// Output (order may vary):
// Main thread continues...
// Thread-1: 1
// Thread-2: 1
// Thread-1: 2
// Thread-2: 2
// ...
```

## Method 2: Implementing Runnable Interface (Preferred)

```java
// Define a task by implementing Runnable
public class MyTask implements Runnable {
    private String taskName;
    
    public MyTask(String name) {
        this.taskName = name;
    }
    
    @Override
    public void run() {
        for (int i = 1; i <= 5; i++) {
            System.out.println(taskName + ": " + i);
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                System.out.println(taskName + " interrupted");
            }
        }
    }
}

// Usage
public class Main {
    public static void main(String[] args) {
        // Create threads with Runnable tasks
        Thread thread1 = new Thread(new MyTask("Task-1"));
        Thread thread2 = new Thread(new MyTask("Task-2"));
        
        thread1.start();
        thread2.start();
    }
}
```

## Method 3: Using Lambda Expressions (Modern Approach)

```java
public class Main {
    public static void main(String[] args) {
        // Create thread with lambda
        Thread thread1 = new Thread(() -> {
            for (int i = 1; i <= 5; i++) {
                System.out.println("Lambda Thread: " + i);
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        });
        
        thread1.start();
        
        // Even simpler for single operations
        new Thread(() -> System.out.println("Quick task")).start();
    }
}
```

## Thread Lifecycle States

```java
public class ThreadLifecycle {
    public static void main(String[] args) throws InterruptedException {
        Thread thread = new Thread(() -> {
            try {
                System.out.println("RUNNABLE: Thread is running");
                Thread.sleep(2000);  // TIMED_WAITING
                System.out.println("Thread woke up");
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        });
        
        // NEW: Thread created but not started
        System.out.println("State: " + thread.getState());  // NEW
        
        thread.start();
        // RUNNABLE: Thread is running or ready to run
        System.out.println("State: " + thread.getState());  // RUNNABLE
        
        Thread.sleep(500);
        // TIMED_WAITING: Thread is sleeping
        System.out.println("State: " + thread.getState());  // TIMED_WAITING
        
        thread.join();  // Wait for thread to finish
        // TERMINATED: Thread has completed
        System.out.println("State: " + thread.getState());  // TERMINATED
    }
}
```

**Thread States:**
- **NEW**: Created but not started
- **RUNNABLE**: Executing or ready to execute
- **BLOCKED**: Waiting for monitor lock
- **WAITING**: Waiting indefinitely for another thread
- **TIMED_WAITING**: Waiting for specified time
- **TERMINATED**: Completed execution

---

# Part 2: Thread Control Methods

## sleep() - Pause Execution

```java
public class SleepExample {
    public static void main(String[] args) {
        Thread thread = new Thread(() -> {
            System.out.println("Starting task...");
            try {
                // Sleep for 3 seconds
                Thread.sleep(3000);
                System.out.println("Task completed after 3 seconds");
            } catch (InterruptedException e) {
                System.out.println("Sleep interrupted!");
            }
        });
        
        thread.start();
    }
}
```

## join() - Wait for Thread to Complete

```java
public class JoinExample {
    public static void main(String[] args) throws InterruptedException {
        Thread worker = new Thread(() -> {
            System.out.println("Worker: Starting heavy computation...");
            try {
                Thread.sleep(3000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println("Worker: Computation done!");
        });
        
        worker.start();
        System.out.println("Main: Waiting for worker...");
        
        worker.join();  // Wait for worker to finish
        
        System.out.println("Main: Worker finished, continuing...");
    }
}

// Output:
// Main: Waiting for worker...
// Worker: Starting heavy computation...
// Worker: Computation done!
// Main: Worker finished, continuing...
```

## join(timeout) - Wait with Timeout

```java
public class JoinTimeoutExample {
    public static void main(String[] args) throws InterruptedException {
        Thread longTask = new Thread(() -> {
            try {
                Thread.sleep(5000);  // Takes 5 seconds
                System.out.println("Long task completed");
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        });
        
        longTask.start();
        
        // Wait maximum 2 seconds
        longTask.join(2000);
        
        if (longTask.isAlive()) {
            System.out.println("Task is still running after timeout");
        }
    }
}
```

## interrupt() - Request Thread Interruption

```java
public class InterruptExample {
    public static void main(String[] args) throws InterruptedException {
        Thread thread = new Thread(() -> {
            try {
                for (int i = 0; i < 10; i++) {
                    System.out.println("Working... " + i);
                    Thread.sleep(1000);
                }
            } catch (InterruptedException e) {
                System.out.println("Thread was interrupted!");
                return;  // Exit gracefully
            }
            System.out.println("Work completed normally");
        });
        
        thread.start();
        
        Thread.sleep(3000);  // Let it work for 3 seconds
        thread.interrupt();  // Request interruption
        
        System.out.println("Interruption requested");
    }
}
```

## Checking Interruption Status

```java
public class InterruptCheckExample {
    public static void main(String[] args) throws InterruptedException {
        Thread thread = new Thread(() -> {
            while (!Thread.interrupted()) {  // Check if interrupted
                System.out.println("Working...");
                // Do some work (no blocking call)
                for (int i = 0; i < 1000000; i++) {
                    Math.sqrt(i);
                }
            }
            System.out.println("Detected interruption, cleaning up...");
        });
        
        thread.start();
        Thread.sleep(100);
        thread.interrupt();
    }
}
```

## Thread Priority

```java
public class PriorityExample {
    public static void main(String[] args) {
        Thread lowPriority = new Thread(() -> {
            for (int i = 0; i < 5; i++) {
                System.out.println("Low priority: " + i);
            }
        });
        
        Thread highPriority = new Thread(() -> {
            for (int i = 0; i < 5; i++) {
                System.out.println("High priority: " + i);
            }
        });
        
        // Set priorities (1-10, default is 5)
        lowPriority.setPriority(Thread.MIN_PRIORITY);   // 1
        highPriority.setPriority(Thread.MAX_PRIORITY);  // 10
        
        lowPriority.start();
        highPriority.start();
    }
}
```

## Daemon Threads

```java
public class DaemonExample {
    public static void main(String[] args) throws InterruptedException {
        Thread daemonThread = new Thread(() -> {
            while (true) {
                System.out.println("Daemon: Running in background...");
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        });
        
        // Mark as daemon BEFORE starting
        daemonThread.setDaemon(true);
        daemonThread.start();
        
        Thread.sleep(3000);
        System.out.println("Main thread ending...");
        // Daemon thread will automatically terminate when main ends
    }
}
```

---

# Part 3: Synchronization - Preventing Race Conditions

## The Race Condition Problem

```java
public class RaceConditionProblem {
    private static int counter = 0;
    
    public static void main(String[] args) throws InterruptedException {
        Thread t1 = new Thread(() -> {
            for (int i = 0; i < 10000; i++) {
                counter++;  // NOT thread-safe!
            }
        });
        
        Thread t2 = new Thread(() -> {
            for (int i = 0; i < 10000; i++) {
                counter++;  // NOT thread-safe!
            }
        });
        
        t1.start();
        t2.start();
        t1.join();
        t2.join();
        
        System.out.println("Counter: " + counter);
        // Expected: 20000, Actual: varies (e.g., 15847, 19234)
        // This is a RACE CONDITION!
    }
}
```

## Solution 1: Synchronized Methods

```java
public class SynchronizedCounter {
    private int counter = 0;
    
    // synchronized keyword makes method thread-safe
    public synchronized void increment() {
        counter++;
    }
    
    public synchronized int getCounter() {
        return counter;
    }
    
    public static void main(String[] args) throws InterruptedException {
        SynchronizedCounter sc = new SynchronizedCounter();
        
        Thread t1 = new Thread(() -> {
            for (int i = 0; i < 10000; i++) {
                sc.increment();
            }
        });
        
        Thread t2 = new Thread(() -> {
            for (int i = 0; i < 10000; i++) {
                sc.increment();
            }
        });
        
        t1.start();
        t2.start();
        t1.join();
        t2.join();
        
        System.out.println("Counter: " + sc.getCounter());
        // Always 20000 - CORRECT!
    }
}
```

## Solution 2: Synchronized Blocks

```java
public class SynchronizedBlock {
    private int counter = 0;
    private final Object lock = new Object();  // Lock object
    
    public void increment() {
        // Only synchronize critical section
        synchronized(lock) {
            counter++;
        }
    }
    
    // Can use 'this' as lock
    public void decrement() {
        synchronized(this) {
            counter--;
        }
    }
    
    public int getCounter() {
        synchronized(lock) {
            return counter;
        }
    }
}
```

## Static Synchronization

```java
public class StaticSynchronization {
    private static int counter = 0;
    
    // Synchronized on class object
    public static synchronized void increment() {
        counter++;
    }
    
    // Equivalent to:
    public static void incrementAlt() {
        synchronized(StaticSynchronization.class) {
            counter++;
        }
    }
}
```

## Real-World Example: Bank Account

```java
public class BankAccount {
    private double balance;
    private final Object lock = new Object();
    
    public BankAccount(double initialBalance) {
        this.balance = initialBalance;
    }
    
    public void deposit(double amount) {
        synchronized(lock) {
            System.out.println(Thread.currentThread().getName() + 
                             " depositing " + amount);
            balance += amount;
            System.out.println("New balance: " + balance);
        }
    }
    
    public void withdraw(double amount) {
        synchronized(lock) {
            if (balance >= amount) {
                System.out.println(Thread.currentThread().getName() + 
                                 " withdrawing " + amount);
                balance -= amount;
                System.out.println("New balance: " + balance);
            } else {
                System.out.println("Insufficient funds!");
            }
        }
    }
    
    public synchronized double getBalance() {
        return balance;
    }
}

// Usage
public class BankDemo {
    public static void main(String[] args) {
        BankAccount account = new BankAccount(1000);
        
        // Multiple threads accessing same account
        Thread t1 = new Thread(() -> account.deposit(500), "Thread-1");
        Thread t2 = new Thread(() -> account.withdraw(300), "Thread-2");
        Thread t3 = new Thread(() -> account.deposit(200), "Thread-3");
        
        t1.start();
        t2.start();
        t3.start();
    }
}
```

---

# Part 4: Wait, Notify, and NotifyAll

## Producer-Consumer Problem

```java
public class ProducerConsumer {
    private final List<Integer> buffer = new ArrayList<>();
    private final int CAPACITY = 5;
    private final Object lock = new Object();
    
    // Producer adds items
    public void produce(int value) throws InterruptedException {
        synchronized(lock) {
            // Wait if buffer is full
            while (buffer.size() == CAPACITY) {
                System.out.println("Buffer full, producer waiting...");
                lock.wait();  // Release lock and wait
            }
            
            buffer.add(value);
            System.out.println("Produced: " + value);
            lock.notifyAll();  // Wake up consumers
        }
    }
    
    // Consumer removes items
    public int consume() throws InterruptedException {
        synchronized(lock) {
            // Wait if buffer is empty
            while (buffer.isEmpty()) {
                System.out.println("Buffer empty, consumer waiting...");
                lock.wait();  // Release lock and wait
            }
            
            int value = buffer.remove(0);
            System.out.println("Consumed: " + value);
            lock.notifyAll();  // Wake up producers
            return value;
        }
    }
}

// Usage
public class Demo {
    public static void main(String[] args) {
        ProducerConsumer pc = new ProducerConsumer();
        
        // Producer thread
        Thread producer = new Thread(() -> {
            try {
                for (int i = 1; i <= 10; i++) {
                    pc.produce(i);
                    Thread.sleep(500);
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        });
        
        // Consumer thread
        Thread consumer = new Thread(() -> {
            try {
                for (int i = 1; i <= 10; i++) {
                    pc.consume();
                    Thread.sleep(1000);  // Slower than producer
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        });
        
        producer.start();
        consumer.start();
    }
}
```

## wait() vs sleep()

```java
public class WaitVsSleep {
    private final Object lock = new Object();
    
    public void demonstrateWait() throws InterruptedException {
        synchronized(lock) {
            System.out.println("Before wait - lock held");
            lock.wait(2000);  // Releases lock, waits 2 seconds
            System.out.println("After wait - lock reacquired");
        }
    }
    
    public void demonstrateSleep() throws InterruptedException {
        synchronized(lock) {
            System.out.println("Before sleep - lock held");
            Thread.sleep(2000);  // Keeps lock, sleeps 2 seconds
            System.out.println("After sleep - lock still held");
        }
    }
}
```

**Key Differences:**
- `wait()`: Releases lock, must be in synchronized block
- `sleep()`: Keeps lock, can be called anywhere
- `wait()`: Woken by `notify()`/`notifyAll()`
- `sleep()`: Wakes after time expires

---

# Part 5: Modern Concurrency Utilities

## ExecutorService - Thread Pool Management

```java
import java.util.concurrent.*;

public class ExecutorExample {
    public static void main(String[] args) {
        // Create thread pool with 3 threads
        ExecutorService executor = Executors.newFixedThreadPool(3);
        
        // Submit tasks
        for (int i = 1; i <= 10; i++) {
            int taskId = i;
            executor.submit(() -> {
                System.out.println("Task " + taskId + 
                                 " running on " + 
                                 Thread.currentThread().getName());
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            });
        }
        
        executor.shutdown();  // No new tasks accepted
        
        try {
            // Wait for all tasks to complete
            if (!executor.awaitTermination(60, TimeUnit.SECONDS)) {
                executor.shutdownNow();  // Force shutdown
            }
        } catch (InterruptedException e) {
            executor.shutdownNow();
        }
        
        System.out.println("All tasks completed");
    }
}
```

## Types of Executors

```java
public class ExecutorTypes {
    public static void main(String[] args) {
        // 1. Fixed thread pool
        ExecutorService fixed = Executors.newFixedThreadPool(5);
        
        // 2. Cached thread pool (creates threads as needed)
        ExecutorService cached = Executors.newCachedThreadPool();
        
        // 3. Single thread executor (tasks run sequentially)
        ExecutorService single = Executors.newSingleThreadExecutor();
        
        // 4. Scheduled executor (for delayed/periodic tasks)
        ScheduledExecutorService scheduled = 
            Executors.newScheduledThreadPool(2);
        
        // Schedule task after 5 seconds
        scheduled.schedule(() -> {
            System.out.println("Executed after 5 seconds");
        }, 5, TimeUnit.SECONDS);
        
        // Schedule periodic task (every 2 seconds)
        scheduled.scheduleAtFixedRate(() -> {
            System.out.println("Periodic task: " + 
                             System.currentTimeMillis());
        }, 0, 2, TimeUnit.SECONDS);
        
        // Don't forget to shutdown
        // fixed.shutdown();
        // cached.shutdown();
        // single.shutdown();
        // scheduled.shutdown();
    }
}
```

## Callable and Future - Return Values from Threads

```java
import java.util.concurrent.*;

public class CallableFutureExample {
    public static void main(String[] args) throws Exception {
        ExecutorService executor = Executors.newFixedThreadPool(2);
        
        // Callable returns a value
        Callable<Integer> task1 = () -> {
            Thread.sleep(2000);
            return 100;
        };
        
        Callable<String> task2 = () -> {
            Thread.sleep(3000);
            return "Task completed";
        };
        
        // Submit tasks and get Futures
        Future<Integer> future1 = executor.submit(task1);
        Future<String> future2 = executor.submit(task2);
        
        System.out.println("Tasks submitted, doing other work...");
        
        // Get results (blocks until ready)
        Integer result1 = future1.get();  // Waits if not done
        String result2 = future2.get();
        
        System.out.println("Result 1: " + result1);
        System.out.println("Result 2: " + result2);
        
        executor.shutdown();
    }
}
```

## Future with Timeout and Cancellation

```java
public class FutureAdvanced {
    public static void main(String[] args) throws Exception {
        ExecutorService executor = Executors.newSingleThreadExecutor();
        
        Future<String> future = executor.submit(() -> {
            Thread.sleep(5000);  // Takes 5 seconds
            return "Done";
        });
        
        // Check if done
        System.out.println("Is done? " + future.isDone());  // false
        
        try {
            // Wait maximum 2 seconds
            String result = future.get(2, TimeUnit.SECONDS);
        } catch (TimeoutException e) {
            System.out.println("Task timed out!");
            future.cancel(true);  // Cancel the task
        }
        
        System.out.println("Is cancelled? " + future.isCancelled());
        
        executor.shutdown();
    }
}
```

## CountDownLatch - Wait for Multiple Threads

```java
import java.util.concurrent.CountDownLatch;

public class CountDownLatchExample {
    public static void main(String[] args) throws InterruptedException {
        int numWorkers = 3;
        CountDownLatch latch = new CountDownLatch(numWorkers);
        
        // Create worker threads
        for (int i = 1; i <= numWorkers; i++) {
            int workerId = i;
            new Thread(() -> {
                try {
                    System.out.println("Worker " + workerId + " starting");
                    Thread.sleep(2000);
                    System.out.println("Worker " + workerId + " finished");
                    latch.countDown();  // Decrement count
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }).start();
        }
        
        System.out.println("Main thread waiting for workers...");
        latch.await();  // Wait until count reaches 0
        System.out.println("All workers finished!");
    }
}
```

## CyclicBarrier - Synchronize Multiple Threads

```java
import java.util.concurrent.CyclicBarrier;

public class CyclicBarrierExample {
    public static void main(String[] args) {
        int numThreads = 3;
        
        // Barrier action runs when all threads reach barrier
        CyclicBarrier barrier = new CyclicBarrier(numThreads, () -> {
            System.out.println("All threads reached barrier!");
        });
        
        for (int i = 1; i <= numThreads; i++) {
            int threadId = i;
            new Thread(() -> {
                try {
                    System.out.println("Thread " + threadId + " working...");
                    Thread.sleep(threadId * 1000);
                    System.out.println("Thread " + threadId + 
                                     " reached barrier");
                    barrier.await();  // Wait for others
                    System.out.println("Thread " + threadId + 
                                     " continuing...");
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }).start();
        }
    }
}
```

## Semaphore - Control Access to Resources

```java
import java.util.concurrent.Semaphore;

public class SemaphoreExample {
    public static void main(String[] args) {
        // Allow 3 threads to access resource simultaneously
        Semaphore semaphore = new Semaphore(3);
        
        for (int i = 1; i <= 10; i++) {
            int userId = i;
            new Thread(() -> {
                try {
                    System.out.println("User " + userId + " waiting...");
                    semaphore.acquire();  // Get permit
                    System.out.println("User " + userId + 
                                     " accessing resource");
                    Thread.sleep(2000);
                    System.out.println("User " + userId + " done");
                    semaphore.release();  // Release permit
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }).start();
        }
    }
}
```

---

# Part 6: Concurrent Collections

## ConcurrentHashMap - Thread-Safe Map

```java
import java.util.concurrent.ConcurrentHashMap;

public class ConcurrentHashMapExample {
    public static void main(String[] args) {
        ConcurrentHashMap<String, Integer> map = new ConcurrentHashMap<>();
        
        // Multiple threads can safely modify
        Thread t1 = new Thread(() -> {
            for (int i = 0; i < 1000; i++) {
                map.put("Key" + i, i);
            }
        });
        
        Thread t2 = new Thread(() -> {
            for (int i = 0; i < 1000; i++) {
                map.put("Key" + i, i * 2);
            }
        });
        
        t1.start();
        t2.start();
        
        try {
            t1.join();
            t2.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        
        System.out.println("Map size: " + map.size());
    }
}
```

## CopyOnWriteArrayList - Thread-Safe List

```java
import java.util.concurrent.CopyOnWriteArrayList;

public class CopyOnWriteExample {
    public static void main(String[] args) {
        CopyOnWriteArrayList<String> list = new CopyOnWriteArrayList<>();
        
        // Safe for concurrent reads and writes
        list.add("A");
        list.add("B");
        list.add("C");
        
        // Iterator won't throw ConcurrentModificationException
        for (String item : list) {
            System.out.println(item);
            list.add("D");  // Safe to modify while iterating
        }
    }
}
```

## BlockingQueue - Producer-Consumer Pattern

```java
import java.util.concurrent.*;

public class BlockingQueueExample {
    public static void main(String[] args) {
        BlockingQueue<Integer> queue = new ArrayBlockingQueue<>(10);
        
        // Producer
        Thread producer = new Thread(() -> {
            try {
                for (int i = 1; i <= 20; i++) {
                    queue.put(i);  // Blocks if queue is full
                    System.out.println("Produced: " + i);
                    Thread.sleep(500);
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        });
        
        // Consumer
        Thread consumer = new Thread(() -> {
            try {
                while (true) {
                    Integer value = queue.take();  // Blocks if empty
                    System.out.println("Consumed: " + value);
                    Thread.sleep(1000);
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        });
        
        producer.start();
        consumer.start();
    }
}
```

---

# Part 7: Atomic Variables

## AtomicInteger - Lock-Free Thread Safety

```java
import java.util.concurrent.atomic.AtomicInteger;

public class AtomicExample {
    private static AtomicInteger counter = new AtomicInteger(0);
    
    public static void main(String[] args) throws InterruptedException {
        Thread t1 = new Thread(() -> {
            for (int i = 0; i < 10000; i++) {
                counter.incrementAndGet();  // Atomic operation
            }
        });
        
        Thread t2 = new Thread(() -> {
            for (int i = 0; i < 10000; i++) {
                counter.incrementAndGet();
            }
        });
        
        t1.start();
        t2.start();
        t1.join();
        t2.join();
        
        System.out.println("Counter: " + counter.get());  // Always 20000
    }
}
```

## Common Atomic Operations

```java
import java.util.concurrent.atomic.*;

public class AtomicOperations {
    public static void main(String[] args) {
        AtomicInteger atomicInt = new AtomicInteger(10);
        
        // Get and set operations
        int value = atomicInt.get();              // Get: 10
        atomicInt.set(20);                        // Set: 20
        
        // Increment/decrement
        int newValue = atomicInt.incrementAndGet();  // ++, returns 21
        newValue = atomicInt.getAndIncrement();      // returns 21, then ++
        newValue = atomicInt.decrementAndGet();      // --, returns 21
        newValue = atomicInt.getAndDecrement();      // returns 21, then --
        
        // Add/subtract
        newValue = atomicInt.addAndGet(5);        // Add 5, returns result
        newValue = atomicInt.getAndAdd(5);        // Returns old, then add 5
        
        // Compare and set
        boolean success = atomicInt.compareAndSet(20, 30);
        // If current value is 20, set to 30, return true
        
        // Other atomic types
        AtomicLong atomicLong = new AtomicLong(1000L);
        AtomicBoolean atomicBool = new AtomicBoolean(true);
        AtomicReference<String> atomicRef = 
            new AtomicReference<>("Initial");
    }
}
```

---

# Part 8: ReentrantLock - Advanced Locking

## ReentrantLock vs Synchronized

```java
import java.util.concurrent.locks.*;

public class ReentrantLockExample {
    private final ReentrantLock lock = new ReentrantLock();
    private int counter = 0;
    
    public void increment() {
        lock.lock();  // Acquire lock
        try {
            counter++;
        } finally {
            lock.unlock();  // Always release in finally
        }
    }
    
    // Try lock with timeout
    public boolean tryIncrement() {
        try {
            if (lock.tryLock(1, TimeUnit.SECONDS)) {
                try {
                    counter++;
                    return true;
                } finally {
                    lock.unlock();
                }
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
        return false;
    }
    
    public int getCounter() {
        return counter;
    }
}
```

## ReadWriteLock - Multiple Readers, Single Writer

```java
import java.util.concurrent.locks.*;

public class ReadWriteLockExample {
    private final ReadWriteLock rwLock = new ReentrantReadWriteLock();
    private final Lock readLock = rwLock.readLock();
    private final Lock writeLock = rwLock.writeLock();
    private int value = 0;
    
    // Multiple threads can read simultaneously
    public int read() {
        readLock.lock();
        try {
            System.out.println(Thread.currentThread().getName() + 
                             " reading: " + value);
            Thread.sleep(100);
            return value;
        } catch (InterruptedException e) {
            return -1;
        } finally {
            readLock.unlock();
        }
    }
    
    // Only one thread can write at a time
    public void write(int newValue) {
        writeLock.lock();
        try {
            System.out.println(Thread.currentThread().getName() + 
                             " writing: " + newValue);
            Thread.sleep(100);
            value = newValue;
        } catch (InterruptedException e) {
            e.printStackTrace();
        } finally {
            writeLock.unlock();
        }
    }
}
```

---

# Common Concurrency Issues

## 1. Deadlock

```java
public class DeadlockExample {
    private final Object lock1 = new Object();
    private final Object lock2 = new Object();
    
    public void method1() {
        synchronized(lock1) {
            System.out.println("Thread 1: Holding lock1...");
            try { Thread.sleep(100); } catch (InterruptedException e) {}
            
            synchronized(lock2) {
                System.out.println("Thread 1: Holding lock1 & lock2");
            }
        }
    }
    
    public void method2() {
        synchronized(lock2) {
            System.out.println("Thread 2: Holding lock2...");
            try { Thread.sleep(100); } catch (InterruptedException e) {}
            
            synchronized(lock1) {
                System.out.println("Thread 2: Holding lock1 & lock2");
            }
        }
    }
    
    // This will cause DEADLOCK!
    // Thread 1 waits for lock2, Thread 2 waits for lock1
}

// Solution: Always acquire locks in same order
public class DeadlockSolution {
    private final Object lock1 = new Object();
    private final Object lock2 = new Object();
    
    public void method1() {
        synchronized(lock1) {
            synchronized(lock2) {  // Same order
                // Work
            }
        }
    }
    
    public void method2() {
        synchronized(lock1) {      // Same order
            synchronized(lock2) {
                // Work
            }
        }
    }
}
```

## 2. Thread Starvation

```java
public class StarvationExample {
    // Low priority threads may never get CPU time
    public static void main(String[] args) {
        Thread highPriority = new Thread(() -> {
            while (true) {
                // Constantly running
            }
        });
        highPriority.setPriority(Thread.MAX_PRIORITY);
        
        Thread lowPriority = new Thread(() -> {
            System.out.println("I may never run!");
        });
        lowPriority.setPriority(Thread.MIN_PRIORITY);
        
        highPriority.start();
        lowPriority.start();
    }
}
```

## 3. Livelock

```java
public class LivelockExample {
    private boolean flag1 = true;
    private boolean flag2 = true;
    
    // Both threads keep changing state but make no progress
    public void thread1() {
        while (flag1) {
            if (flag2) {
                System.out.println("Thread 1: Waiting for thread 2");
                flag1 = false;
                Thread.yield();
            }
        }
    }
    
    public void thread2() {
        while (flag2) {
            if (flag1) {
                System.out.println("Thread 2: Waiting for thread 1");
                flag2 = false;
                Thread.yield();
            }
        }
    }
}
```

---

# Best Practices

## 1. Use Higher-Level Concurrency Utilities

```java
// ❌ Avoid manual thread management
Thread t = new Thread(() -> { /* work */ });
t.start();

// ✅ Use ExecutorService
ExecutorService executor = Executors.newFixedThreadPool(10);
executor.submit(() -> { /* work */ });
executor.shutdown();
```

## 2. Prefer Immutability

```java
// ✅ Immutable objects are thread-safe
public final class ImmutablePoint {
    private final int x;
    private final int y;
    
    public ImmutablePoint(int x, int y) {
        this.x = x;
        this.y = y;
    }
    
    public int getX() { return x; }
    public int getY() { return y; }
}
```

## 3. Minimize Synchronization Scope

```java
// ❌ Bad: Entire method synchronized
public synchronized void process() {
    // Lots of work
    counter++;
    // More work
}

// ✅ Good: Only critical section synchronized
public void process() {
    // Lots of work
    synchronized(this) {
        counter++;
    }
    // More work
}
```

## 4. Use Thread-Safe Collections

```java
// ❌ Not thread-safe
List<String> list = new ArrayList<>();

// ✅ Thread-safe
List<String> list = new CopyOnWriteArrayList<>();
Map<String, Integer> map = new ConcurrentHashMap<>();
Queue<String> queue = new ConcurrentLinkedQueue<>();
```

---

# Summary

**Creating Threads:**
- Extend `Thread` class
- Implement `Runnable` interface (preferred)
- Use lambda expressions (modern)

**Thread Control:**
- `start()`: Begin execution
- `sleep()`: Pause thread
- `join()`: Wait for completion
- `interrupt()`: Request cancellation
- `setPriority()`: Set scheduling priority

**Synchronization:**
- `synchronized` keyword: Prevent race conditions
- `wait()`/`notify()`: Thread communication
- Locks: More flexible than synchronized

**Modern Utilities:**
- `ExecutorService`: Thread pool management
- `Future`/`Callable`: Get return values
- `CountDownLatch`: Wait for events
- `CyclicBarrier`: Synchronize threads
- `Semaphore`: Control access

**Concurrent Collections:**
- `ConcurrentHashMap`: Thread-safe map
- `CopyOnWriteArrayList`: Thread-safe list
- `BlockingQueue`: Producer-consumer

**Atomic Variables:**
- `AtomicInteger`: Lock-free operations
- Better performance than synchronized

**Common Issues:**
- Race conditions: Use synchronization
- Deadlocks: Acquire locks in order
- Thread starvation: Fair scheduling

**Best Practices:**
- Use ExecutorService over manual threads
- Prefer immutable objects
- Use concurrent collections
- Minimize synchronization scope
- Handle InterruptedException properly