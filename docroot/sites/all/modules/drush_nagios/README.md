# Drush Nagios (drush_nagios)

## Introduction

Isolated Nagios/Icinga functionality from [drush_multi][1]

For the usage as [Nagios][2]/[Icinga][3] [NRPE Plugin][4] via [Drush][5].

It just prints a message and exit with an exit status.

You can find the original [project page][6] at http://drupal.org/project/drush_nagios .

## Commands

drush_nagios includes the following command:

## check-updates

Checks for pending updates including Drupal (core), modules and themes.

## check-db-updates

Checks for pending database updates.

## Installation

### Manual method

Download from [project page][6] and extract this extension to a number of places:

- In a .drush folder in your HOME folder.
- Along with one of your existing modules. If your command is related to an existing module, this is the preferred option.
- In a folder specified with the include option.
- In /path/to/drush/commands (not a "Smart Thing to Do", but it would work).

### Drush method 

Since this is as drupal.org project, you might install it via drush, just type:
	
    drush dl drush_nagios

## Documentation

Every command got a help.
Type `drush help COMMAND` for options and further informations.

- See [example.aliases.drushrc.php][7] for Drush alias configuration
- See [Drush Nagios (drush_nagios) Doxygen Dokumentation][8]

## Credits

Thanks to the mighty hundfred, friend and Bastard Operator from Hell,
who inspired me to write this (Nagios) Drush plugin.

This module was and written by [fl3a][9], [Florian Latzel][10],   
[Drupal-CMS-[Consulting  Architecture Development] and Open Source support in Cologne][9],      
proud member of [Reinblau, Drupal Agency in Berlin, Cologne and Munich][11].   

[1]:http://drupal.org/project/drush_multi
[2]:http://en.wikipedia.org/wiki/Nagios
[3]:http://en.wikipedia.org/wiki/Icinga
[4]:http://en.wikipedia.org/wiki/Nagios#NRPE
[5]:http://drush.ws
[6]:http://drupal.org/project/drush_nagios
[7]:https://github.com/fl3a/drush_nagios/blob/master/example/example.aliases.drushrc.php
[8]:http://is-loesungen.de/docu/drush_nagios/index.html
[9]:https://drupal.org/user/51103
[10]:http://is-loesungen.de
[11]:http://reinblau.de
