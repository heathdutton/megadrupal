L2j Connect Module

This module allows users to login on a Drupal website 
through L2J Server using a L2J loginserver account.

L2j Connect converts your Drupal Installation in a robust 
Account Manager, creating and editing accounts.


0. INTRODUCTION

L2J is an Open Source, Alternative Lineage 2 Game Server 
written in pure Java for best compatibility.

L2J gives you the possibility to legally host a game server 
for this popular Korean MMO created by NCSoft.

Server's first release we know of were made in June 27, 2004 by L2Chef. 
Lots of people have passed for the project, much of its code evolved 
and much more is left to do yet. 

But the project goals stood consistent during these years.

1. INSTALLATION

Place the entirety of this directory in sites/all/modules/l2j_connect.

Edit the settings.php adding the datasources (included in the step 2).
 
Navigate to administer > modules and enable L2j Connect.

2. LOGINSERVER AND GAMESERVER SETTINGS

Each database connection is specified as an array of settings 
similar to the following:

array(
   'driver' => 'mysql',
   'database' => 'databasename',
   'username' => 'username',
   'password' => 'password',
   'host' => 'localhost',
   'port' => 3306,
   'prefix' => 'myprefix_',
   'collation' => 'utf8_general_ci',
); 

Edit your settings.php file, adding the 'gameserver' and 'loginserver'. 

Your datasources now must to look like this:

$databases = array (
  'default' => 
  array (
    'default' => 
    array (
      'database' => 'drupal_database',
      'username' => 'username',
      'password' => 'password',
      'host' => 'localhost',
      'port' => '',
      'driver' => 'mysql',
      'prefix' => '',
    ),
  ),'loginserver' =>
  array (
    'default' =>
    array (
      'database' => 'l2jls',
      'username' => 'root',
      'password' => '',
      'host' => 'localhost',
      'port' => '',
      'driver' => 'mysql',
      'prefix' => '',
    )
  ),'gameserver' =>
  array (
    'default' =>
     array (
      'database' => 'l2jgs',
      'username' => 'root',
      'password' => '',
      'host' => 'localhost',
      'port' => '',
      'driver' => 'mysql',
      'prefix' => '',
    )
  ),
);

Note that 'l2jls' and 'l2jgs' and root user without password 
are built by default in l2jserver. 

Important: Change them by your own.

3. READ MORE

About L2jServer: http://www.l2jserver.com/about
Community Forum: http://www.l2jserver.com/forum
Timeline: http://trac.l2jserver.com/timeline
More info: http://www.l2jserver.com/