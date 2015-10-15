CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Troubleshooting
 * Maintainers


-- INTRODUCTION --

* This module allows users to comment using Facebook's commenting box
  with as little configuration as possible. It adds a new block "Facebook Comments"
  which acts as a Facebook commenting widget. What makes this module different from
  the ones that already exist on Drupal, is:
  - Unlike other modules, Facebook Comments Block uses real path URL instead of path aliases
    or clean URLs, this is specially important because this would preserve existing comments on
    a page even after the page's URL is changed.
  - Simple configuration process, the entire process takes minutes to install and configure.

* For a full description of the module, visit the project page:
  https://www.drupal.org/project/facebook_comments_block

-- REQUIREMENTS --

* No special requirements.

-- INSTALLATION --

* Install as you would normally install a contributed Drupal module. See:
  https://drupal.org/documentation/install/modules-themes/modules-7
  for further information.

-- CONFIGURATION --

* Go to "admin" -> "structure" -> "block".
* Find "Facebook Comments" block and click on configure.
* Under the "FACEBOOK COMMENTS BOX SETTINGS" you can configure the following:
  - Facebook Application ID: Optional.
  - Main domain: Optional: If you have more than one domain you can set the main domain
    for facebook comments box to use the same commenting widget across all other domains.
  - Color scheme: Set the color schema of facebook comments box.
  - Number of posts: Select how many posts tou want to display by default.
  - Width: Set width of facebook comments box.

-- TROUBLESHOOTING --

* If the block did not appear:

  - Check if you have entered a correct FACEBOOK APP ID,
    leave it blank in case you don't have a Facebook app.

-- MAINTAINERS --

Current maintainers:
* Mohammad AlQanneh (mqanneh) - https://www.drupal.org/u/mqanneh

