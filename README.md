# C From Zero to Hero final project

Main goal of this final project is to create a db-like utility in C for the terminal.

The utility is called `dbview` which, given a file name of employee data via flag `-f` it loads the 
data from that file into memory, performs security checks on it, and the applies the additional 
functions based on flags provided:

- `-a`: Add a new employee to the ddbb.
- `-d`: Delete and employee from ddbb.
- `-u`: Update employee data from ddbb.
