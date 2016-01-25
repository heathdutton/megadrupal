INTRODUCTION
------------
This module extends the functionality of the Personalize module, allowing to swap page content. Site administrators can select a control path and then swap in content from other Drupal paths in order to personalize the user experience at that URL. In short, it does for static pages what Personalize Blocks does for blocks, Personalize fields does for field and Personalize Panels does for panel panes.

It may be helpful to clarify what is meant by "page" for the purposes of this module. In a typical drupal setup, page.tpl.php outputs all drupal-generated dynamic content with:

<?php print render($page['content']); ?>

 The $page['content'] is what gets personalized and replaced with the $page['content'] output of the other selected paths in the option set.

REQUIREMENTS
------------
This module requires the following modules:
 * Personalize (https://www.drupal.org/project/panels)


INSTALLATION
------------
 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

CONFIGURATION
-------------
1. Note that in order to add personalized content, you will need to configure Personalize and Create a campaign.
2. Once this is complete, navigate to admin/structure/personalize/variations/personalize-pages/manage to add a new variation set. The form is very simple and straightforward. Keep in mind, the control path is the path where the personalization will take effect. None of the variation paths will be affected.

 MAINTAINERS
-----------
Current maintainers:
 * Michael DeWolf (mrmikedewolf) - https://drupal.org/user/2679073
