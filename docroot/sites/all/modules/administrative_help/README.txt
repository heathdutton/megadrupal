ActionKit Module
Created by Nicole Benes
-----------------------------

I. Overview

When developing a Drupal site for our clients, we found it helpful to create
a help content type where we could write instructions on how to do
things specific to the given site. This module takes that concept and turns
it into something you can drop into any site. This module does the following 
things:

	* Creates a content Type "Administrative Help"
	* Adds a permission which is required to view the help nodes
	* Adds a block which displays all the help nodes which are published
	* The functionality is fairly simple, but saves time from having to 
	  set up something similar on new sites.


II. Installition Instructions

1) Extract the module to /sites/all/modules
2) Enable the "Administration Help" under "Other
3) Give the permissions under "Administrative Help" to appropriate roles
4) If desired, display the "Administration Help Topics" block in a desired
   location such as on /admin/help.
