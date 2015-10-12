Description
-----------
This module provides functionality to create a single page website.
It allows you to automatically create a single page from a menu. 
The module will render all the content from the links 
that are configured in the menu.It will then override the menu links 
so that they refer to an anchor instead of a new page.

Installation
------------
To install this module, do the following:

1. Extract the tar ball that you downloaded from Drupal.org.

2. Upload the entire directory and all its contents to your modules directory.

Configuration
-------------
To enable and configure this module do the following:

1. Go to Admin -> Modules, and enable Single Page Site.

2. Go to Admin -> Configuration -> System -> Single Page Site Settings, and make
   any necessary configuration changes. 
   
   a) Choose the menu which you want to create a single page for
   b) Define the class/id of the menu wrapper
   c) Define the class(es) of the menu items that should 
      implement the single page navigation.
      (Maybe you don't want all the menu items to be overwritten by an anchor, 
	    eg. contact form on separate page.)  If you don't fill out this field 
      all menu items will be rendered on your single page.
   d) Go to structure -> menus -> "your single page menu" 
      and give all the menu links that have to appear on the single page 
      the class you defined in step 3.
   e) If you don't want to use the title of the menu item as section title
      on your single page, you can use the name attribute of the menu link as title) 
   f) If you want to render content on your single page but don't want 
      the menu item to show up in your menu, navigate to the menu item 
      and give it the class "hide".
   g) Go to /single-page-site and anjoy your one-pager.
   h) Don't forget to set the permissions if you want anonymous users to see your single page
      (People >> Permissions >> View single page site)
