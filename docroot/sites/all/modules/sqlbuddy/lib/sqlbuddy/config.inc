<?php
/*

SQL Buddy - Web based MySQL administration
http://www.sqlbuddy.com/

config.inc
- options for customizing sql buddy

MIT license

2008 Calvin Lough <http://calv.in>

*/


/* The values below are for the login.inc page */

$drupal_connection = Database::getConnection();
$drupal_connection_options = $drupal_connection->getConnectionOptions();

global $sbconfig;
$sbconfig['DefaultAdapter'] = $drupal_connection_options['driver'];
$sbconfig['DefaultHost'] = $drupal_connection_options['host'];
$sbconfig['DefaultUser'] = $drupal_connection_options['username'];

/*
* If you want to enable automatic login, you can include your password below. To 
* automatically connect to SQLite, include the filename of the database instead.
* Note: This is generally not recommended because it means that anyone with the 
* proper url will have access to your data. It should only be used on machines
* that are not accessible from the internet (local testing boxes) or if you have
* set up some other form of authentication. Use as your own discretion.
*/

// MySQL
// $sbconfig['DefaultPass'] = "";
$sbconfig['DefaultPass'] = $drupal_connection_options['password'];

// SQLite
// $sbconfig['DefaultDatabase'] = "";
$sbconfig['DefaultDatabase'] = $drupal_connection_options['database'];

/*
* By default, when you view the homepage of your SQL Buddy installation, a check is
* performed to see if a newer version of the application is available. No personal 
* information is sent about your installation and the response from the server is sent
* as plain text. If, for some reason, you want to disable this behaviour, set this option
* to true to disable automatic checking for updates. To learn about new versions, you can
* always check the projects website at http://www.sqlbuddy.com/
*/

$sbconfig['EnableUpdateCheck'] = true;

/*
* This controls how many rows are displayed at once on the browse tab. If you are on a local
* machine or have a fast internet connection, you could increase this value.
*/

$sbconfig['RowsPerPage'] = 100;

/*
* When set to true, the server will attempt to compress all content before it is sent to the 
* browser. Although unlikely, there is a chance that using gzip will cause issues on certain setups. 
* If you are having trouble getting pages to load properly, you could try disabling gzip.
*/

$sbconfig['EnableGzip'] = true;
?>
