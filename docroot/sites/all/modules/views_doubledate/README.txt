DESCRIPTION
-----------
This module adds a custom Date format to Views, to allow two different
date formats to be applied to a single instance of a field.


REQUIREMENTS
------------
Drupal 6.x
Views


INSTALLING
----------
Refer to the handbook page on "Installing modules and themes":

    http://drupal.org/documentation/install/modules-themes


CONFIGURING AND USING
---------------------
1. Create a new view with a "Row style" of "Fields". 
2. Under the "Fields" section, add a date field.
3. In the Configuration options for the field, under the "Date format"
options, choose "Apply two date formats to a single field."
4. Specify the primary and secondary date formats, along with an
"Offset".



CREDITS
-------
This module borrows heavily from a blog post by Tancredi D'Onofrio entitled
"How to create a custom Views handler":

    http://tanc.me/blog/2011/how-create-custom-views-handler

Many thanks to Tancredi for posting the example code.
