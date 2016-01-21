
ABOUT
-----

This module doesn't do anything on its own. Only install this if another 
module depends on, or provides optional Gearman integration.


OVERVIEW
--------

Gearman provides a generic application framework to farm out work to
other machines or processes that are better suited to do the work. It
allows you to do work in parallel, to load balance processing, and to
call functions between languages. It can be used in a variety of
applications, from high-availability web sites to the transport of
database replication events. In other words, it is the nervous system
for how distributed processing communicates.


DRUSH
-----

You can get a feeling for the utility of the gearman workflow quickly by
using the provided Drush examples. After installing the module and
configuring it to connect to your Gearmand job queue, start up a drush 
worker by running "drush gearman-worker". Then, in a second terminal 
you can send commands to the queue which will be processed by the worker.

Two example job functions are provided:

1) "reverse" - the classic gearman example:

   $> drush gearman-client reverse "reverse my text"

2) "drush-invoke" - a proof of concept allowing you to run other drush 
   commands via the persistent worker thread:
   
   $> drush gearman-client drush-invoke status
   $> drush gearman-client drush-invoke "cc all"
   
Your specific implementation will probably lead you to create your own 
job types and callbacks, which you can declare by implementing 
gearman_drush_function().


TODO
----

PHP Leaks memory! Beware letting a worker run wild. The example Drush 
based worker keeps track of its memory utilization. A setting to cap 
that memory use or provide an alert system when it reaches critical 
levels would be nice. 


LINKS
-----

 * Gearman: http://gearman.org
 * Gearman PECL extension: http://us2.php.net/manual/en/book.gearman.php
 * Overview of setup and configuration of Gearman
   http://toys.lerdorf.com/archives/51-Playing-with-Gearman.html
