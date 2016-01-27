
README file for the Record feature Drupal module.


Description
===========

This module started from the idea to be able to 'record' a feature. Well, not
the entire feature of course, but parts of it that are hard to backtrack when
you started creating the feature. Even more specific: the strongarm part of a
feature. If you want your feature to be truly 'activate module and go'-style,
strongarm is simply unmissable.
What variable was changed during the creation of a feature is really hard to
track. You'd have to inspect all the form elements to see how the variable could
be named. Could be, meaning that you do not get the correct variable name by
just inspecting form elements. Multiple elements could be stored in one variable
etc.

Next to changes to variables you can also choose to track changes to
permissions. A report will be generated with an overview of which permissions
were revoked and / or granted for what role.

Both the variable changes and permission changes are exportable as a .info
feature file only for use with Drush. Once the export information is added to
your feature's .info file, you can simply:

 $ drush fu feature_name

to update your feature.


Dependencies
============

- Features   http://drupal.org/project/features
- Strongarm  http://drupal.org/project/strongarm
- Drush      http://drupal.org/project/drush

Record feature wil function perfectly without these, but if you do not know what
they are, you will have no use for this module.

Ctools will be needed in the future, when the 'export to code' (like Features
does) will be complete. Mind you, this will take a while!


Installation
============

1. Extract the 'record_feature' module directory, including all its
   subdirectories, into your Drupal modules directory.

2. Go to the Administer > Modules page, and enable the module.

3. Go to the Administer > People > Permissions page, and grant 'access
   record feature' permissions to the desired roles.


Usage
=====
If you have features installed and active, you will find an extra tab on the
Features overview page (admin/structure/features). The tab is called 'Record
feature' (admin/structure/features/record).

As there is no integration whatsoever with features (yet?), you simply create a
new feature on admin/structure/features/record.
The feature you create here is not at all related in any way (yet?) to the
features listed on the Features overview page (admin/structure/features)

When creating a new feature, the name you enter should be the name of the
feature module you plan to create.
At the moment it is not that important, but it will be when Ctools exporting is
added (at some point).
The options define what changes you whish to track.
If you so desire, you can start recording immediately.

Now do the stuff in Drupal you need to do for this feature:
- configure permissions
- change settings

When that is done, stop the recording and view the feature. Everything you've
changed is listed in the report.
Uncheck what you do not wish to be exported and hit the export button.

Copy and paste the text you see now in the .info file of your feature and do:

 $ drush fu feature_name
 
All done!


Support
=======

For support requests, bug reports, and feature requests, please us the issue cue
of Menu Clone on http://drupal.org/project/issues/record_feature.

I'll try to keep up, but I'm strapped for time so...
