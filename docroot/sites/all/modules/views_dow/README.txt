VIEWS DAY OF WEEK README
===================================
This module adds a filter handler to Views 3 and enables content to be
filtered based on the current day of the week.

INSTALLATION
============
Enable the module as usual.

CONFIGURATION
=============
First you need to create a custom field for a content type. Let's call it
"Show on day" (field_show_on_day).

Select "List (text)" as the field type and choose any widget.

Copy this into the "Allowed values list" textarea:

1|Monday
2|Tuesday
3|Wednesday
4|Thursday
5|Friday
6|Saturday
7|Sunday

Go to the module's configuration page located at admin/config/views-dow.
Enter the machine name of the field you just created.

USAGE
=====
Now when you add content you can select on which day(s) (Mon-Sun) the content
should be shown.

Create a new view listing your content. Add a new filter and select your 
custom field "Show on day". There are no settings available to just save.

The view now only shows content that has been marked with the current
day of the week.

CREDITS
=======
Module developed by Rade (http://drupal.org/user/604700).
