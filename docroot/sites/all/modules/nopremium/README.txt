NODE OPTION PREMIUM

DESCRIPTION
This module adds a new node option Premium content along with core publishing options (Published, Promoted to front page, Sticky at top of lists).
When a node is published as premium content, only users with proper privileges may view the full content of the node.
Users without proper privileges can still access premium nodes but only get the content rendered in teaser context when trying to view the full content. An additional message informs them that the content is available to premium users only.
This module is an alternative to the Premium module. Its main goal is the same but the way it works in the background is different.
Please try both and choose the one that best suits your needs, but *do not install both at the same time*.

FEATURES
* Mimics the way core publishing options work to ensure maximum integration with Drupal core and contributed modules.
* Easy to use and to configure: you just need to set permissions.
* Provides a permission view full premium content for each content type.
* Provides a permission override premium option for each content type. It allows users without the administer nodes permission to change the Premium content option on node editing. This completes what provides the essential Override Node Options module for core publishing options.
* Provides 2 actions/node operations: Make content premium and Make content non-premium.
* Integrates with CCK: the teaser displayed to non-premium users is not only the teaser of the body field but includes also the CCK fields rendered in teaser context as configured on the Display fields page.
* Integrates with Views: the module exposes a Premium content field, filter and sort.
* Integrates with Rules: the module provides a Content is premium condition.
* Allows to theme the message displayed to non-premium users.
* Allows to theme the teaser & message combination displayed to non-premium users.

INSTALLATION
Copy the nopremium folder to your sites/all/modules directory.
Navigate to admin/build/modules and enable the module.
Navigate to admin/user/permissions and set the proper permissions for each content type.

WHO CAN FULLY VIEW A PREMIUM NODE OF TYPE NODE_TYPE?
Obviously, any user with the view full NODE_TYPE premium content permission can fully view it. But also:
* Any user with the administer nodes permission.
* The author of the node.
* Any user with the edit any NODE_TYPE content permission.

LIMITATIONS
Node Option Premium does not have any effect on individual fields displayed in views. But remember that you can control the way fields are displayed at theming level by overriding the proper views templates and checking the value of the Premium content field.
Node Option Premium has no built-in support for time-limited premium contents. But as it integrates with Rules, you can achieve this by using Rules scheduler with it.

CONTACT
Henri MEDOT <henri.medot[AT]absyx[DOT]fr>
http://www.absyx.fr

IF YOU LIKE THIS MODULE, PLEASE REVIEW IT AND ADD IT TO FAVORITES AT DRUPALMODULES.COM.
