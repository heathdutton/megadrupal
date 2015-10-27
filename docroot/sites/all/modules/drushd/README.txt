
CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Features
 * Requirements
 * Installation
 * Configuration
 * Custom Daemon Usage
 * Custom Daemon Development

INTRODUCTION
------------
This module is meant to provide a simple means of creating a robust
command-line-driven, fully bootstrapped PHP Daemon. It uses the PHP-Daemon
(https://github.com/shaneharter/PHP-Daemon) Library to create the Daemon (via
the Libraries API) in order to not re-invent the wheel ;-).


FEATURES
--------
 * Provides a Drush interface to start/stop your Daemon.
 * Your daemon starts in the background and is detached from the current
   terminal.
 * Daemon is fully bootstrapped so you can use any Drupal function when
   designing your task/job.
 * Easy, two-step process to help you develop a custom bootstrapped daemon with
   Drush support.
 * Add advanced functionality and override default functionality easily due to
   the class-based structure of the Daemon API.
 * Lock Files, Automatic restart (8hrs default) and Built-in Signal Handling &
   Event Logging are only a few of the features provided by the PHP-Daemon
   Library making this a fully featured & robust Daemon.

REQUIREMENTS
------------
 * Libraries API (https://www.drupal.org/project/libraries)
 * PHP-Daemon Library version 2.0 (https://github.com/shaneharter/PHP-Daemon)
 * Drush 5.x (https://github.com/drush-ops/drush)

INSTALLATION
------------
 * Install all required modules as per their instructions.
 * Install this module as you would normally install a contributed drupal
   module. See:https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.
 * Download the PHP-Daemon Library and extract it in your sites/all/libraries
   directory. The folder must be named "PHP-Daemon".

CONFIGURATION
-------------
The module has no menu or modifiable settings.  There is no configuration.  When
enabled, the module will provide a number of drush commands for control of all
daemon apiDaemons from the command-line.

CUSTOM DAEMON USAGE
-------------------
* Start Daemon
    drush daemon start [daemon-machine-name]
* Stop Daemon
    drush daemon stop [daemon-machine-name]
* Check the Status
    drush daemon status [daemon-machine-name]
* Show the Log
   * List the last 10 lines of the log file:
      drush daemon show-log [daemon-machine-name]
   * List the last N lines of the log file:
      drush daemon show-log [daemon-machine-name] --num_lines=N

CUSTOM DAEMON DEVELOPMENT
-------------------------
Using this API to create your own daemon takes only two steps:
    1. Register your Daemon with the Daemon API. This is done by implementing
       hook_daemon_api_info() in your .module file.
       See daemon_api.module:daemon_api_daemon_api_info(). for an example.
    2. Implement a DrupalDaemon class. Simply create a class extending the
       DaemonApiDaemon class. This should be located in a [classname].inc file
       in your module directory and only needs to implement one method
       executeTask().
       See DaemonApiExampleDaemon class for an example.
Finally run your daemon on the command-line using 
  drush daemon start [daemon machine name] 
(See Custom Daemon Usage above for more commands). That really is it!
