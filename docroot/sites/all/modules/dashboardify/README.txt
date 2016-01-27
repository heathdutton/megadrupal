MODULE INFORMATION

Dashboardify is a module for creating complex dashboards.

Each user with proper permissions can add any block to dashboard and rearrange
blocks order on his or her dashboard page. There is one dashboard with
predefined regions available for each user. User can add unlimited instances
of the same block which can be useful for views blocks with different contextual
filters applied. Along with block information module stores data what contextual
filters were used while adding block to dashboard. Block order is autosave
via AJAX.



REQUIEMENTS

Contextual links (core module)



INSTALLING THE MODULE

1. Put the entire link module directory into your 'sites/all/modules' folder
Enable the module
2. Go to Administration > People > Permission and set proper permissions. To use
dashbords user need to have "Use own dashboard" permission and "Use contextual
links" permission
3. Optionally go to Administration > Configuration > Dashboardify settings
and set number of columns along with theirs width



KNOWN ISSUES

Module will not work properly for Views with ajax and contextual filters
enabled. Block will be added to dashboard, but without any customization.
In some specific cases such block could not render at all.



TODO

1. Implement solution for Views with ajax enabled
2. Allow users creating new dashboards
3. Allow users set and rearrange regions


I do not plan backporting this module to Drupal 6.
