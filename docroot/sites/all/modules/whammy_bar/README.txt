If I've done my job, you will fucking _hate_ using the Whammy Bar module.

This module isn't for you, fearless developer. The Whammy Bar is a curated Wordpress style menu 
for non-developer Drupal site maintainers, editors, and users. The goal of the Whammy Bar is to 
provide a professional and simple menu that is designed to make Drupal easier to use.

This module was sponsored by Zujava.com.

Installation
------------
1) Copy the whammy_bar directory to the modules folder in your installation.

2) Enable the module using Administer -> Modules (/admin/modules)

Configuration
-------------
Configure the Whammy Bar for your site at
Configuration -> Administration -> Whammy Bar (/admin/config/administration/whammy-bar)

Using a Hook to Alter Links
---------------------------
You can programmtically alter (add/delete/change) the links that appear in the menu
with the hook_whammy_bar_links_alter() function. Here is a simple example:

/**
 * Implementation of hook_whammy_bar_links_alter().
 *
 * Add my own links
 */
function MYMODULE_whammy_bar_links_alter(&$links) {
  global $user;

  $links['profile-links']['children']['MYMODULE-comments'] = array(
    'data' => l(t('Comments'), 'user/' . $user->uid . '/comments'),
    'weight' => 5,
  );
}

Support
-------
If you have a problem, file a request or issue on the whammy bar queue at
http://drupal.org/project/issues/whammy_bar. DO NOT POST IN THE FORUMS.
Posting in the issue queues is a direct line of communication with the module
authors.

No guarantee is provided with this software, no matter how critical your
information, module authors are not responsible for damage caused by this
software or obligated in any way to correct problems you may experience.

Licensed under the GPL 2.0.
http://www.gnu.org/licenses/gpl-2.0.txt