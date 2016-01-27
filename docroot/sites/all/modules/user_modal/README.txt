OVERVIEW
========
User modal module allows opening the Register/ Login/ Reset password menu items
as tabs. Since the tabs are shown via JS and not AJAX, this may lead to a better 
user experience, as there is no time waiting for the selected tab to load.
Furthermore, the user modal form can be used by other implementing modules, and
for example allow a user to submit a node and register in the same time.
The module requires the client have JS enabled.

INSTALLATION
=============
1) Enable module and its dependencies.
2) In the permissions page, allow anonymous user "Access the administrative 
   overlay". Optionally, if you will use user-modal as a base for other modals
   that include a register/ login, but also might be used by authenticated users
   it is recommended to also allow the same permission to authenticated users. 
3) Optional; As the goal of user modal is to allow easy and fast registration 
   or login, thus it is recommended to set the account settings via admin/config/people/accounts
     - "Who can register accounts" set to Visitors
     - Uncheck "Require e-mail verification when a visitor creates an account."
  The above settings will insure that a registered user will immediately be 
  logged in.
  
DEVELOPER TIPS
==============
For easier themeing, you can add the current path to the overlay, which will add
a class with the current menu item

  /**
   * Preprocess overlay.
   */
  function MYTHEME_preprocess_overlay(&$variables) {
    // Add the path to the content class.
    $item = menu_get_item();
    $path = str_replace('%', '', $item['path']);
    // Remove trailing slash.
    $path = trim($path, '/');
    $path = str_replace('/', '-', $path);
  
    $variables['content_attributes_array']['class'][] = $path;
  }  

As admin users will also want to use the new overlay, you should set the 
administration theme via admin/appearance to your custom theme. 
To make sure, they do however get the admin theme opened in "admin" path, you 
can implement hook_custom_theme() to return the correct theme that should be 
used.
   
  /**
   * Implements hook_custom_theme().
   *
   * Make sure admin pages are served in "seven" theme.
   */
  function MYMODULE_custom_theme() {
    $item = menu_get_item();
    if (strpos($item['path'], 'admin') === 0) {
      return 'seven';
    }
  }

DEVELOPERS
==========
Developed by gizra.com; Sponsored by Medico.com