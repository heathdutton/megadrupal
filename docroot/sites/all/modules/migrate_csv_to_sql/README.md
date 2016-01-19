# Migrate CSV to SQL

Introduction
------------
This module provides convenience functionality for migrating data from 
CSV files, via SQL tables, using the Migrate module.


Motivation
----------
Have you ever had a problem that certain rows in the CSV were empty, or didn't
 need to be imported because of bogus data / empty fields?
  
Or maybe your CSV data is denormalized and you want to skip certain rows that would
 map to the same entities, but because of the skipping, your migration always shows
 Incomplete Migration? Isn't that annoying? Especially if there are other dependent migrations
 on the incomplete one, which won't run unless executed with --force.
 
This module provides a couple of classes to allow:

* Importing multiple CSV files into a SQL table
* Simple selection / skipping of imported CSV rows, by providing a predicate function 
(hello functional filter and PHP 5.3!)
* Use of a specialized MigrateSourceSQL for your Migration classes, which uses the data 
imported from the CSV files
* Convenience functions to declare the CSV to SQL tables, in your module hook_schema


Required modules
----------------
This module depends only on the Migrate module. 


Configuration
-------------
There are no UI configurations for this module at the moment.
This is a developer code-only module.

Usage
-----
An example module is provided for building an intuition how to use the provided functionality.

Considerations
--------------
* When CSV files are imported into the DB, the rows are always appended to the table,
 so currently there is no way to delete entries in the DB, except via a DB management tool.
* Similarly, there is currently no way to update already imported rows.
* To eliminate duplicated rows when importing the same CSV file, a hash is computed based on
 a list of fields provided by the developer.

TODOs
-----
* Ability to view / edit / delete the imported CSV rows via a UI page.
* Ability to update already imported rows.
