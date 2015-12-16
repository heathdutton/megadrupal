Overview:
--------
The dba module provides Drupal administrators with direct access to their
Drupal database tables from within the standard Drupal user interface.

WARNING:  If a user is granted (or manages to aquire) 'adminster database'
permissions, they are able to directly alter the database.  At minimum, they
are able to modify data, and possibly to drop tables.  Depending on how you
have defined your database permissions, the user may also be able to modify
other databases unrelated to your Drupal installation.  Use at your own risk!


Features:
--------
 - Can list all tables in your default database
 - Can view data from one or more tables
 - Generates annotated mysqlreport output for MySQL databases
 - Generates limited annotated sql report output for PostgreSQL databases
 - Can export data from one ore more tables into any of the following formats:
    o CSV
    o HTML
    o mysqldump (only works with MySQL databases)
    o OpenOffice Spreadsheet
    o OpenOffice Document
    o Microsoft Excel
    o Microsoft Word
 - Can truncate one or more tables
 - Fully supports MySQL, PostgreSQL and SQLite
 - Drush integration
    o drush dba-sqlreport: displays sqlreport output (ie mysqlreport)


Installation and configuration:
------------------------------
Copy the module into the appropriate modules subdirectory and enable it.

Credits:
-------
 - Written and maintained by Jeremy Andrews <jeremy@kerneltrap.org>
 - Pre Drupal-7 PostgreSQL support provided by AAM <aam@ugpl.de>
 - Co-maintained for a while by Derek Wright [http://drupal.org/user/46549]
