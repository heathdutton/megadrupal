
CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Warnings
 * Maintainers

INTRODUCTION
------------

 * Appointment Calendar is a simple form to set and create slots for 
   booking in aprticular date.
   It provides a simple availability calendar to check availability of 
   selected slot,
   i.e all available/booked slots can show in availability calendar

REQUIREMENTS
------------
This module requires the following modules:
 * calendar (Contrib module)
 * date (Contrib module)

INSTALLATION
------------
 * Install as you would normally install a contributed drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

CONFIGURATION
-------------
 * After installation of module Clear all cache first (Important to do this).
 * This module will import a View block and create 
   Appointment Calendar content type (Donot Modify View that has 
   imported by module. It is highle recommened to use as it is).
 * Goto settings page and select from and to date with no. of time slots.
 * After filling all slots you will be redirected to Slot list page 
   where you can see filled dates with no.of slots.
 * In list you can edit particular date and able to change slots timings 
   or capacity of slots.
 * Now you can create a node appointment calendar 
   with date and time slot given for selected date.
 * You can check availabity of timeslot and if yes you can proceed further.
 * You can also add fields in Appointment Calendar content type if you wish to.
 * Check activated theme template folder for page--appointcal.tpl.php
 * If not present copy this template under templates folder and 
   paste it on your activated theme template folder.

WARNINGS
--------
 * Donot change imported view(Appointment Calendar).

MAINTAINER
-----------
Current maintainers:
 * A AjayKumar Reddy (ajaykumarreddy1392) - https://www.drupal.org/user/3261994
