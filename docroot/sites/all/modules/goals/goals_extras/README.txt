Goals Extras Module
-------------------
by Scott Weston, http://drupal.org/user/1116332

About the Goals Extras Module
------------------------------
This module provides some add-ons to the Goals Module that some site
administrators may wish to implement, including:
* An image field to store a Goal Image (e.g. trophy, badge, ribbon, etc.) to
  illustrate completed goals for a user.
* A points field to store the number of points to grant to a user when the
  goal is completed.
* A view which generates a block of Goal Images of goals the user has
  completed.

Installation
------------
The Goals Extras module has the following dependencies:
* Goals Module (http://drupal.org/project/goals)
* Number field (Core - Optional)
* Image field (Core - Optional)
* Views Module (http://drupal.org/project/views)
* User Points Module (http://drupal.org/project/userpoints)

Once the module is enabled, two new fields are added to the Goal entity or
Image and User Points. There is also a block created
(goals_completed_by_user-block) which can be placed in an appropriate region
within your theme.

When a goal is completed by a user, the number of points indicated in the User
Points field is granted to the user.

The fields and view added by the Goals Extras module can be altered with image
styles, etc. as necessary to fit your needs.

Uninstalling Goals Extra Module
-------------------------------
Since it is unknown what modifications have been made to the fields and view
added, the module does not automatically delete the field instances and field
when the module is uninstalled. This is to prevent potentially breaking your
site or its functionality.

Once the module is disabled and uninstalled, the following steps will remove
any trace of the module:
* Disable the module (admin/modules)
* Uninstall the module (admin/modules/uninstall)
* Delete any 'overridden' instance of the view from the database
  (admin/structure/views)
* Remove the goal_userpoints field (admin/config/goals/manage/fields)
* Remove the goal_img field (admin/config/goals/manage/fields)
