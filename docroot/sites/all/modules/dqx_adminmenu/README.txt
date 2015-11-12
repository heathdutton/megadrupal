
PURPOSE
===================

This thing is meant as an alternative to
http://drupal.org/admin_menu
Read the module page for more information.


ONLINE DOCS
===================

Most of the documentation for this module can be found on the project page,
http://drupal.org/project/dqx_adminmenu

There is also a blog post on
http://sesam.dqxtech.net/blog/2011-02-19/dqx-adminmenu-dev-notes
with some technical notes.


INSTALL
===================

Install like any other module.

Dependencies:
pageapi module (in http://drupal.org/pageapi)

Please disable:
admin_menu module (in http://drupal.org/admin_menu)
-> please disable, because one replaces the other.

Recommended to install/enable:
admin_views module (ships with http://drupal.org/admin_menu)
-> Enjoy direct links to node lists filtered by content type.
menu_editor module (in http://drupal.org/menu_editor)
-> Enjoy direct links to menu editing pages


CUSTOMIZE (IN CODE)
============================

The module has no configuration page to visit with your browser.
Instead, there is a nice and powerful API, that allows you to customize the menu
in code. This allows to easily migrate your customizations with version control.

The "example" folder contains an example for your customization module.
You don't want to enable the "example" module, instead you want to copy from it,
and create your own.


