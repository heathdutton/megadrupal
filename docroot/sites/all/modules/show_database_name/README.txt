CONTENTS OF THIS FILE
---------------------

* Introduction
* Installation


INTRODUCTION
------------

Current maintainer:  Gregg Marshall (drupal.org/user/84148)

Display the host and database name of the default database on the status
report, toolbar (Drupal 7) and/or on the admin_menu bar if enabled.

The genesis of this module came about as once again I copied a website to
a development instance and forgot to change settings.php to use the a
development database, and ended up making changes to production when I
just wanted to try something on development.

There are two use cases for this module:
1.  Give a quick way to identify which database you are using when switching
    from development to staging to production.
2.  When you are given a website to maintain and you are trying to figure out
    which of many databases are actually driving the content you are seeing.
	
The module just reports what comes from settings.php and makes no attempt to
test the database connection.

There is a similar module, drupal.org/project/dbinfo, that can display more
information on the status report, with the dependency on CTools, and doesn't
display the information on the toolbar or admin_menu bar.


INSTALLATION
------------

* Install as usual, see http://drupal.org/node/70151 for further information.
* The module adds a permission, "view database name", which is used to control
  who can view the database host/name.

This project was sponsored by the New York State Office of Information
Technology Services Web Services Department.
