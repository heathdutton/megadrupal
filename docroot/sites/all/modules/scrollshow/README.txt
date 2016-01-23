Description
-----------

This module allows you to turn a list of nodes into a full page scrollshow.

Full documentaton is available on drupal.org: https://drupal.org/node/2181571

You can optionally add configurable effects:

- Background video: Show a video behind the slides that advances as you scroll.

- Lightbox: Open internal links to other pages on the site in a lightbox, so you're users aren't force to leave the scrollshow (for when your scrollshow is your front page).

- Parallax layers: Show up to 5 animated parallax layers, configurable per slide.

- Curtain: Show an initial image at the top of the scrollshow that will go away when the user begins to scroll. (Like the curtain raising before a performance at the theatre.)

Requirements
------------
CTools (ctools) is required for this project: https://drupal.org/project/ctools

Installation
------------
To install this module, do the following:

1. Extract the tar ball that you downloaded from Drupal.org.

2. Upload the entire directory and all its contents to your modules directory.

Configuration
-------------
To enable and configure this module do the following:

1. Go to Admin -> Modules, and enable Scrollshow (scrollshow) and Menu Scrollsow (menu_scrollshow).

Quick Setup
-----------
This module is very basic but very powerful but deceptively simple to start using. This is the basics of the basics.

1. Create nodes (the default Article and Page work fine) in /node/add
2. Add the created nodes to a menu (Main Menu works great to try the module out) in /admin/structure/menu
3. In the menu admin select the Edit Menu - in the case of Main Menu /admin/structure/menu/manage/main-menu/edit
4. You will find a Scrollshow setting group on the menu page where you can adjust settings such as:
    - Smoothscroll easing function
    - Smoothscroll duration
    - Slide position
    - Slide spacing
    - Fallback text

    Note: The defaults work fine. You will also find Effects that can be added with appropriate libraries. These advanced features are covered in the more complete documentation on drupal.org:

Documentation
-------------
For more full documentation, check out our full documentation on drupal.org:

https://drupal.org/node/2181571

