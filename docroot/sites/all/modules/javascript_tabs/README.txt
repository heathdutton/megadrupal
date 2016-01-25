JavaScript Tabs is a module which hides the default Drupal admin tabs
(View / Edit and any secondary tabs) and places them neatly in a drop-
down menu. A small blue icon will appear instead - simply click this to
show and hide the menu. Although some CSS is included to position this
icon to the right, it may be necessary to add some of your own to
position it exactly where you'd like within your own themes.

To use JavaScript Tabs, simply enable the module and navigate to
admin/config/user-interface/javascript-tabs to set which themes this
should be enabled on - by default this is none. Then, optionally, set
the page-specific visibility settings to enable/disable JavaScript Tabs
on specific pages and paths.

NOTE: If a theme already overrides the tabs (such as Drupal's stock
admin theme, Seven) then JavaScript Tabs will do its best to detect a
conflict and not to appear on that theme. This is because the theme's
code will likely be designed to modify Drupal's default tab array, and
if Javascript Tabs has already modified it then this could otherwise
cause the theme to crash.
