# Time-Travelling-File-System
This is a git-inspired version control system in which each file can have simple string based content . The system is fast with O(1) time for creation, update and other file commands while O(n + klogn) time for displaying k system wide characteristics like k recent files or biggest files. The implementation uses heaps and hashmaps.


# How to Compile and Run

For running on a Windows device with MinGW installed, open PowerShell, enter the project directory, and type:

```
g++ -std=c++11 interface.cpp -o interface.exe
```

This will compile the program and make it ready to run. You can then run it with the following command in the project directory once you've compiled:

```
./interface.exe
```

> IMPORTANT NOTE: THOUGH WE NEED TO ONLY COMPILE interface.cpp, IT USES file_structure.cpp THROUGH #include, SO IT IS VERY ESSENTIAL TO KEEP THEM IN THE SAME DIRECTORY. 2 SEPARATE FILES HAVE BEEN MADE FOR CONVENIENCE AND TO DECLUTTER, AND FOR NO OTHER REASON.

## Inputs and Error Handling

The program, once started, will dynamically take inputs and take the necessary actions just after you enter your input. To stop/end the program, input "STOP" (all caps necessary).

The expected inputs are of the following form:

> NOTE: ROOT IS NOT CONSIDERED AS A VERSION (VERSION ID = 0). SAY I HAVE 2 VERSIONS AFTER THE ROOT, THEN I'LL CONSIDER TOTAL VERSIONS = 2.

## File-Based Inputs

### Create
```
CREATE FILENAME
```
Sample input:
```
CREATE File1
```
Creates a file named `File1`. Here, `FILENAME` can be any string and does not have particular restrictions. You cannot create two files with the same name; the program reports that a file with the given name already exists.

### Read
```
READ FILENAME
```
Sample input:
```
READ File1
```
Reads `File1` if it exists.

### Insert
```
INSERT FILENAME CONTENT
```
Sample input:
```
INSERT File1 hello
```
Inserts `hello` into `File1` if it exists.

### Update
```
UPDATE FILENAME CONTENT
```
Sample input:
```
UPDATE File1 hello world
```
Updates `hello world` into `File1` if it exists.

### Snapshot
```
SNAPSHOT FILENAME MESSAGE
```
Sample input:
```
SNAPSHOT File1 first file
```
Snapshots the active version of `File1` if not already snapshotted, with the message `first file`, and if `File1` exists.

### Rollback
```
ROLLBACK FILENAME VERSION
```
Sample input:
```
ROLLBACK File1 2
```
Rolls back to version `2` of `File1` if that version exists and `File1` exists.

### History
```
HISTORY FILENAME
```
Sample input:
```
HISTORY File1
```
Outputs the history of `File1` in `version, time_snapshotted (formatted), message` format if `File1` exists.

## System-Based Inputs

### Recent files / Biggest trees
```
(RECENT_FILES or BIGGEST_TREES) NUMBER
```
Sample input:
```
RECENT_FILES 3
```
Outputs the `min(3, total files)` most recent files of the system. `NUMBER` must be a non‑negative valid integer, otherwise this will output: `INVALID INPUT`.

## Error Handling

- `INVALID INPUT`: For commands with the wrong format or number of arguments.
- `NO FILE WITH THE GIVEN NAME FOUND`: When an operation is attempted on a file that does not exist.
- `THE VERSION IS ALREADY A SNAPSHOT`: If `SNAPSHOT` is called on an already snapshotted version.
- `VERSION DOES NOT EXIST`: If `ROLLBACK` is called with a non-existent `versionID`.

## Data Structures and Methods Implemented

All primary data structures were implemented from scratch without using standard C++ containers for the core structures.

### filecont (HashMap)
A custom hash map that stores all file objects, providing O(1) average-time access to any file by its name.

### Hash Function
Generates a hash from a filename string by iteratively processing the ASCII values of its characters.

### file (Tree)
Represents a single file and its entire version history. Versions are organized as a tree structure, allowing for branching.

### TreeNode
The fundamental node of the version tree. Each `TreeNode` represents a specific version of a file, storing its content, metadata, and parent/child relationships.

### mapp (HashMap)
A simple array-based map that provides O(1) access to a version (`TreeNode`) within a file by its integer `version_id` (hash function: identity).

### recent_files & biggest_trees (Heaps)
Two separate max-heap implementations used to efficiently track and retrieve system-wide file metrics.
```

