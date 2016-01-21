Description
==========
This module provide the user interface to add or drop
the database Table indexes for the Mysql databases.
Using this module you can have the information about what all are the Index
with associated columns and its Tables.

By navigating to the 'ADD NEW INDEX' tab ,
you will get the option to add new index for the selected tables.

Be very careful about the functionality for dropping the indexes.
Make sure that before you trying to do that you have working database backup.
Don't try to drop the default indexes, It may cause the feature problems.


REQUIREMENT
===========

 Mysql 5.6 or above version


Usage
=====

1) Download and extract to the module folder.
2) Navigate to path 'admin/modules' and enable the module
3) Click on the 'configure' wheel button or go to the path 'admin/db/index'
4) Make sure to 'clear the cache' that ones you added the new index
