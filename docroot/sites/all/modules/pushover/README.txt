
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Example
 * Extending
 * Installation


INTRODUCTION
------------

Current Maintainer: andeersg <anders.2205@gmail.com>

Ever wanted to get notifications on your phone when someting happens on your 
Drupal site? Drupal Pushover gives you the functionality to set up rules and 
recieve push notifications on your phone via Pushover and their app 
(Available for iOS and Android only).

EXAMPLE
-------

With this module you can set up rules and get notification when anything from
critical system log messages appear to new users registered on your site.

INSTALLATION
------------

The Rules module is required for installing this module. After you have 
installed it you have to go to https://pushover.net and register an application.
Afterwards you can input your API key at "/admin/config/services/pushover".

Before you can recieve notifications you must input your client ID (also from 
https://pushover.net) on your profile page.

If you want other users to enter their client ID you must give them the 
right permission.
