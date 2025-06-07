# Operating-Systems
Operating Systems Project for year 3 of Computer Engineering and Informatics Department of University of Patras
# Εργασία Λειτουργικών Συστημάτων – 1ο Project

## 📌 Περιγραφή
Αυτή η εργασία περιλαμβάνει δύο κύρια ερωτήματα:

### 🔧 Ερώτημα 1: Shell Scripting (Bash)
Ανάπτυξη ενός διαδραστικού `shell script` για τη διαχείριση αρχείου επιχειρήσεων (`Businesses.csv`) με δυνατότητες:
- Προβολής εγγραφών
- Επεξεργασίας τιμών με χρήση `awk`
- Επιλογής αρχείου και αποθήκευσης αλλαγών
- Χρήση `case` και `while` loops για μενού

### 🔧 Ερώτημα 2: Διεργασίες και Παράλληλος Υπολογισμός
Υλοποίηση του **Monte Carlo integration** με χρήση:
- `fork()` για δημιουργία child processes
- `shared memory` (mmap) και `wait()` για συγχρονισμό
- Παράλληλος υπολογισμός ολοκληρωμάτων
- Μέτρηση χρόνου και σφαλμάτων

### ✅ Επιπλέον
- Εκτενής χρήση δομών C
- Διαχείριση μνήμης και επεξεργασίας με POSIX system calls
- Παραδείγματα εκτέλεσης και screenshots στο αρχείο αναφοράς

## 👥 Ομάδα
- Θεόδωρος Κατσάντας – AM: 1097459  
- Αγγελική Δούβρη – AM: 1097441  
- Αγάπη Αυγουστινού – AM: 1093327

## 🛠️ Τεχνολογίες
- Bash (Ubuntu WSL)
- C (Linux system programming)

# Εργασία Λειτουργικών Συστημάτων – 2ο Project

## 📌 Περιγραφή
Αντικείμενο της άσκησης είναι η υλοποίηση **προσομοίωσης χρονοδρομολογητή (scheduler)** διεργασιών σε Linux:

### 🔁 Πολιτικές Εκτέλεσης:
- **FCFS (First Come First Serve)**
- **Round Robin (RR)** με ρυθμιζόμενο time quantum

### 🧠 Κύρια χαρακτηριστικά:
- Δομές `Process`, `Node`, `Queue`
- Χρήση `execlp`, `fork`, `waitpid`
- Επεξεργασία σημάτων `SIGSTOP`, `SIGCONT`, `SIGCHLD`
- Προσομοίωση I/O με `SIGUSR1`, `SIGUSR2`, `alarm()`
- Διαχείριση εισόδου/εξόδου σε δεύτερη φάση (`scheduler_io`)

### 🧪 Περιπτώσεις δοκιμών:
- homogeneous.txt
- reverse.txt
- mixed.txt

## 👥 Ομάδα
- Θεόδωρος Κατσάντας – AM: 1097459  
- Αγγελική Δούβρη – AM: 1097441  
- Αγάπη Αυγουστινού – AM: 1093327

## 🛠️ Τεχνολογίες
- C (Unix System Programming)
- Signals, Processes, Semaphores
