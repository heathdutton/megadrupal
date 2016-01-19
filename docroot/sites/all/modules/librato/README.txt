
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * API used


INTRODUCTION
------------

Current Maintainer: Cheatlex <jesper@funcity.dk>

Every webmaster has over time with maintenance of a major website found 
it necessary to monitor how much an individual event is used.

Many of these can usually be measured using tools like google analytics.

But as a developer, I often look at stuff like Watchdog errors or how 
many web-orders failed, and other events most of those tools don't covers.

For this I use Librato.com which allows me to transfer and show this 
informations, or any other events in nice interface, all I got to do is 
add an rules event and activate cron.

One of the advantages I get from this is the ability to overview a number 
of websites with the same metrics.

Another is the ability it offers to put an alert on a given event like 
zero items on stock, or if the amount of errors in watchdog suddenly 
increases big time.

To insure this module don’t causes a hi load on the website we use a cron 
task to send the informations to Librato, this allow the admin to do this 
in consideration to other tasks the website might have.

With the support for rules only your imagination defines what you can 
track, enjoy.


INSTALLATION
------------

1. To install this module you need to enable curl in your php installation
   and then install the module rules.

2. Copy this librato/ directory to your sites/SITENAME/modules directory.

3. Enable the module and configure admin/config/system/librato.

4. Enable cron

5. Now you are ready to setup your first rule.


API USED
-------------

The version is currently using Librato API v1.

More info about the api can be found here:
http://dev.librato.com/v1
