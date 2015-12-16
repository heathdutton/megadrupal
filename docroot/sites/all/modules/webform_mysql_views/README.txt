
-- SUMMARY --

The Webform MySQL Views builds flattened, read-only MySQL views of webform
submission data. These views may be useful when you need to access this data
from an external application in an automated fashion without exporting,
importing, or the use of a web-based API.

-- REQUIREMENTS --

* Your Drupal database must be using the MySQL backend.

* Your MySQL server must be version 5.0 or later

* The MySQL user specified in your Drupal settings.php file must have permission
  to create views.

* Webform Module

-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

* To manage MySQL views for your webforms, log in as an administrator and go to
  the Administer > Content Management > Web Forms page and click on the MySQL
  Views tab.


-- COMMON ISSUES --

If you see incomplete data output when querying a view, you will likely need to
change (or add) the `group_concat_max_len' setting for MySQL. put the following
into your my.conf file and restart MySQL:

[mysqld]
group_concat_max_len = 8192

Alternatively, you can specify a group_concat_max_len for just this Drupal
via the settings.php file:

$databases['default']['default']['init_commands'] = array(
  'group_concat' => 'SET group_concat_max_len=8192',
);



This original 6.x.1.x version of the project was sponsored by:
* The Proof Group LLC
  Visit http://proofgroup.com for more information.
