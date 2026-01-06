# Final Project Proposal

## Group Members:

Rahul Deb      
 
# Intentions:

A database allowing users to manipulate tables.
 
# Intended usage:

The server will allow for creating n numbers of tables, of which each allows to set up multiple columns…probably by asking the server user to select options and input values. Multiple clients are allowed to connect to this server, and they can select any table that the server has already created. After doing so, client users can manipulate a table by performing one of the following row operations: create, edit, delete. These will selectable options, after which the client user provides values if necessary.  
 
# Technical Details:

Tables in the database will be stored in the form of csv files, which can be manipulated by both the server and client. The columns in the tables will utilize a custom data type when allocating memory. 


Allowing multiple users to connect to the server simultaneously and manipulating the tables would use forking and sockets.

Extra:
Providing the statistics of every tables (perhaps using stat + other methods). Examples include table size, # of table rows, mean/median of quantitative columns, repetition of a certain string, etc.

... (will work on rest of technical details later) ...

A description of your technical design. This should include: 
   
How you will be using the topics covered in class in the project.
     
How you are breaking down the project and who is responsible for which parts.
  
    
# Intended pacing:

01/06: makefile and understanding how to do csv file management
01/07: working on allow user manipulation of csv files
... (will work on rest of timeline later) ...
