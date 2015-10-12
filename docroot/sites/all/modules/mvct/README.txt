"Most Viewed per Content Type" module

This is a very simple module that allows you to track page views on your site 
without having to enable the core statistics module.

This is useful if you have a big site with many content types but only need 
the statistics for some of them, so you don't need to create write queries to
the database unnecessarily on the whole web.

Note that this module is much simpler than the core statistics module and only
records the page total counts and the last visited date.  It also records only
visits to the 'full' viewmode of the nodes.

REQUIREMENTS and DEPENDENCIES:
------------------------------
None.

INSTALLATION INSTRUCTIONS:
--------------------------
1) Install as usual.
2) Go to admin/config/system/mvct and select the content types you want to 
enable the statistics for.
3) Use the statistics generated in any view, either as a field, filter or as 
a sort criterion.

NOTES:
-----
You are strongly encouraged to use this module in conjunction with views. A
default administration view is provided, showing all statistics recorded by this
module. This view can be found at /admin/config/system/mvct-statistics. You can
either modify it to suit your needs or create your own using it as an example.

Otherwise you can also query directly the database for the information you need.
The table to query is {mvct} and the fields are 'nid', 'view_count' and
'last_viewed'.
