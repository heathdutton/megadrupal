================
Le Gate
================

Le Gate is a simple module that restricts user access to pages on a site, and then provides two mechanisms
by which users can then gain access. It was first developed as an "age gate" module to allow access to a site
only when an appropriate age is selected (>15yo for COPPA). But then we decided to make it more generic.

When a user tries to access a restricted page they are redirected to a configurable and themable page
(the "le-gate" page) that presents one of two ways to gain access. When access is gained a javascript cookie is
set.

The two 'access mechanisms' are:

1) Date: A date field is presented and if the date entered is within a configurable range then access is granted.

2) Links (yes/no): Two configurable links are presented and when the correct (configurable) one is clicked then
   access is granted.

===============
Installation and Configuration

1) Install the module in the usual way.
2) Navigate to /admin/config/system/le-gate
3) Select whether to skip admin pages. If checked then le-gate will not kick in on any admin pages (recommended).
4) Enter in a title for the le-gate page. It will appear as the page title.
5) Enter some instructions. These will appear above the date field or links.
6) Select whether to use paths to put the gate up on. If unchecked (default and recommended) then all paths
   will be gated.
7) Enter in Drupal paths on which the gate should appear. You can exclude paths by prefixing them with '~'.
8) Enter in the message that should be presented to users when they fail to gain access.
9) Select the gate type (Date, or Links). This will change which settings are shown.
   A) Date Settings:
    i) Select a date comparison type:
        'Before' - will pass when the user-entered date is before the Date 1 date (below)
        'After' - will pass when the user-entered date is after the Date 1 date
        'Between' - will pass when the user-entered date is between the Date 1 and Date 2 dates

    ii) Enter a Redirect Drupal path. Users will be redirected to this path when they gain access. You can leave
    it blank so that users remain on the same page.

   B) Links Settings:
     i) Select which link should provide access when clicked
     ii) Enter in a Path and Text for each link. Path should be a Drupal path. Users will be redirected to this
        path when access is determined. You can leave it blank to have users remain on the same page.

This module was developed and supported by Ted Benice @ BTW Labs LLC.