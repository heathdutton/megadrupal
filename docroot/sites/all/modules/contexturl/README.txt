
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installing
 * How can you contribute 


INTRODUCTION
------------

Current Maintainer: gianfrasoft <http://drupal.org/user/363149>

Context URL is a very little and fast module. It gives you the opportunity to
use the complete page url as Context condition: you can check if the URL
string matches a given shell wildcard pattern, also described in fnmatch PHP
function.

So you can use wildcard * and []. Take a look at this:
http://php.net/manual/en/function.fnmatch.php

Context URL is descended (uses code) from Context PHP, a more powerful and
flexible module for Context management.

You can use it instead of a more sophisticated module like Domain Context if
you want to check the domain URL. You can use, also, in place of Context 
plugin from path because it doesn't give you the possibility to check the
complete URL but only the page part.



INSTALLING
----------

Install normally. Remember this module is an extension for Context module. 

Once Context URL is installed and enabled, you can access to the 
admin/structure/context page, edit your context and add the condition named
Page URL.



HOW CAN YOU CONTRIBUTE?
-----------------------

Report any bugs, feature requests, etc. in the issue tracker.

Write a review for this module at drupalmodules.com.

