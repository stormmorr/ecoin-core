This is the folder containing the mysql scripts used to generate the tables and variables used to store the values of the shares,
transactions and other ecoin data.

How to install:
1: Enter your servers mysql terminal using the mysql command and type 'CREATE DATABASE globalscapedb;'
2: Exit mysql then prepare an add command as follows (replacing the filename with the file to add.
user@mysql> mysql -u root -p globalscapedb < 'filename'.sql
