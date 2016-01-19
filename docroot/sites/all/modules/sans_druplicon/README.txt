SANS DRUPLICON
--------------

by Andrew Kamm, ak@kamm.co

This module provides a no-hassle means to remove the most visible instances
of the Druplicon from a site, replacing them with a less intrusive alternative.
The goal is to reduce the amount of boilerplate configuration (or theming)
sitebuilders must do at the beginning of a project to make it presentable to
clients.

USE AND CONFIGURATION
---------------------

  * Upon enabling this module, the alternate logo and favicon images will begin
    being used. If your theme settings are set to hide the logo or favicon,
    Sans Druplicon will not attempt to replace it.

  * Image selection and fine-tuning are available at
    admin/appearance/sans_druplicon.

IMAGES
------

Images are included with sans_druplicon and stored in subdirectories of the
images/ directory. Each image set has three image files:

  * favicon-dark.ico (32x32, used as a favicon in address bar)

  * favicon-light.ico (32x32, used as a favicon with admin_menu module)

  * logo.png (72x72, used as the primary logo image)

Each image set has its directory name explicitly declared in
sans_druplicon_whitelist().

Colors used are from the Drupal color guide at
http://drupal.org/node/1051644.

All included icons provided are public domain or CC0.
