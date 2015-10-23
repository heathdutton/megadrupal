********************************************************************
                     D R U P A L    M O D U L E
********************************************************************
Name: Search Restrict
Author: Robert Castelo <www.codepositive.com>
Drupal: 7.x
********************************************************************
DESCRIPTION:

Restrict by role who can search for each content type.

Approach of this module is to re-write the search query, so that 
content is indexed and available as search results to users in role(s) 
that have permissions to view it, but not displayed to other roles.

********************************************************************
PREREQUISITES:

Search module (Drupal core).



********************************************************************
INSTALLATION:

Note: It is assumed that you have Drupal up and running.  Be sure to
check the Drupal web site if you need assistance.  If you run into
problems, you should always read the INSTALL.txt that comes with the
Drupal package and read the online documentation.

1. Place the entire module directory into your Drupal directory:
   sites/all/modules/
   

2. Enable the module modules by navigating to:

   administer > build > modules
     
  Click the 'Save configuration' button at the bottom to commit your
  changes. 



********************************************************************
USAGE:

On the configuration form of each each content type set the role(s)
that can search for nodes of that content type.


********************************************************************
TO DO

Write tests


********************************************************************
ACKNOWLEDGEMENT

Developed by Robert Castelo for Code Positive <http://www.codepositive.com>
Initial development sponsored by Greenpeace UK <http://www.greenpeace.org.uk>

Drupal 6 update contributions by:
Hans Nilsson (Blackdog) <http://drupal.org/user/110169>
Daniel F. Kudwien (Sun) <http://drupal.org/user/54136> 

Drupal 7 update contributions by:
Alan Davison (Alan D.) <http://drupal.org/user/198838>
Jeff Schuler (jeffschuler) <http://drupal.org/user/239714>
Niklas Fiekas (Niklas Fiekas) <http://drupal.org/user/1089248>

  
