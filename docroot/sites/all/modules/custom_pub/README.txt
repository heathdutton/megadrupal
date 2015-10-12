Custom Publishing Options
=========================

How to use:

1. Install "Custom Publishing Options" from the module list page.
2. Configure Custom Publishing Options on your site at admin/structure/custom_pub, under the Structure menu.
3. On each new option created, set the Node types the option should be available from.
4. Go to the Permissions page. Grant permission to each role that should be able to use the new publishing option.

Note that, using just Custom Publishing Options, on its own, a role needs 'administer nodes' in order to see any of the core publishing options at all. Without it, they still see custom publishing options, just not 'status', 'sticky', or 'promoted' states. More on that under "Additional Features".

Views
=====

Using the Views (http://drupal.org/project/views) module opens up a whole new avenue of displaying content with Custom Publishing Options. Create your View any way you like, and under Filter you will find all Custom Publishing Options available. Create archived content sections by creating an Archive option, and Filter by that option!

Additional Features
===================

If you want greater permissions to allow a role to use custom publishing options or core states (status, sticky, promoted), it is suggested that you also pick up Override Node Options module. This module adds access control to each publish state at the role level, so 'administer nodes' is not a requirement.

Info Extend
===========

This module has a directive in its .info file you may not have seen before, 'suggests'. This property, along with others, appear on the module list page if you have the "Info Extend" module installed. Info Extend is a port of "Module Suggests" for Drupal 6 to Drupal 7. Until it can be committed or decided what should happen on drupal.org, it is hosted at GitHub for now. Initially, I had created a new project and released it on Drupal.org, but then realized a similar module existed. Hence, the holding pattern at the moment.

Info Extend: https://github.com/kevinquillen/info-extend