Delete menu options

=Overview=
Provides extra options when deleting a menu item. You can choose to delete child 
menu items and decide if the linking nodes should be deleted as well.

=Features=
* Delete child menu items
* Delete nodes linked to menu items

=Requirements=
* PHP 5

=Download with Git=
Drupal 6
git clone --recursive --branch 6.x-1.x 
  http://git.drupal.org/sandbox/mjgmaas/1886610.git delete_menu_options

Drupal 7
git clone --recursive --branch 7.x-1.x 
  http://git.drupal.org/sandbox/mjgmaas/1886610.git delete_menu_options

=Detailed description=
On the 'Delete menu item' confirmation page you can choose to delete child menu 
items and decide if the linking nodes should be deleted as well. When the 
checkbox 'Delete all children of this menu.' is checked, all child menu items of 
the parent menu item to delete, will be deleted as well. When the checkbox 
'Delete nodes of the deleted menu items (only orphaned nodes).' is checked, the 
nodes to which the child menu items link will be deleted too. When other menu 
items link to same nodes or aliases as the nodes which were set out to be 
deleted, those nodes will not be deleted.

=Versions=
Versions for both drupal 6 and 7 are available. The drupal 8 version will follow 
in the second half of 2013.

=Maintainer=
Maurice Maas, mjgmaas@gmail.com
