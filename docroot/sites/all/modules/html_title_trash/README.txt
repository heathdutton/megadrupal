
HTML Title Trash
================
Does two things:
o builds on the lovely "HTML Title" module (required), extending that module's
  functionality for node titles to include BLOCK titles as well
o extends the set of allowable HTML elements in titles with: 
  a, br, div, q, mark, small, span

Notes
o you may use class names: <div class="centered big-red">...
o you CANNOT use styles: <div style="color:red">...
o for security reasons you CANNOT use most attributes, like "onclick=..."
o if you need more HTML elements in titles, add these to the .info file
  (use with care!)


Installation
------------
Install HTML Title Trash like any other Drupal module. Use Drush if you wish.
Visit admin/config/content/html_title to configure the HTML elements you want to
allow in node and block titles on your site.
Then insert desired HTML elements into your node and block titles!


Author: RdeBoer, Melbourne, Australia
http://drupal.org/project/html_title_trash
