Drupal's regular menu system administration doesn't scale very well. This is 
due to the fact that for all node edit pages, node type edit pages and 
vocabulary edit pages (when Taxonomy Menu is active), Drupal loads all menu
items from all menus in the system. For large amount of menu items, this slows
down the system to an unusably slow speed, or even causes timouts.

This is a known problem in Drupal core. This comment is from the menu module:

// The menu_links table can be practically any size and we need a way to
// allow contrib modules to provide more scalable pattern choosers.
// hook_form_alter is too late in itself because all the possible parents are
// retrieved here, unless menu_override_parent_selector is set to TRUE.
if (variable_get('menu_override_parent_selector', FALSE)) {
  return array();
}

Menuperformance takes advantage of this, and sets the variable when installed. 
It then proceeds to add its own AJAX based parent menu item selector widgets 
for the node edit, node type edit and vocabulary edit pages.

---

How to use:
- Enable the module.
- Go to /admin/config/user-interface/menuperformance , and activate the 
module. Checking the checkbox sets Drupal variable 
menu_override_parent_selector.
- After the module is activated, the menu item selectors on the above 
mentioned pages should have been replaced with a multiple dropdown 
AJAX-powered selector instead of the standard single dropdown.
