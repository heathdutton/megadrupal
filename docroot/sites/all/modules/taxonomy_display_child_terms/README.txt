Currently this module provides :
1. A plugin to replace "core" taxonomy display which 
   will fetch the current page taxonomy children and
   render them
2. Two new taxonomy "view mode" of "parents viewmode"
   and "children viewmode".
3. Tested using Display Suite module to utilize the 
   new viewmodes with different layout.

How to install :
1. checkout : http://drupal.org/node/1254878 for 
   proper taxonomy_display Installation.
2. in the "term display" select the "Child Terms" 
   and save the form
3. Utilize Display suite to switch style between 
   "parent term (current page term)" and "child 
   term (current page child terms).
4. If you select the "Associated content display" to 
   core then if the terms has node to display it will 
   display the node too underneath the terms.