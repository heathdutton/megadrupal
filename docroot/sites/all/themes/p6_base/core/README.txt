About
-----
Project6 Base (p6_base) theme is meant to be used as a startup theme for your
web projects. It comes with a sub-theme which should be the one to be used. Do
not use the p6_base directly.


Installation
------------
Read "Installing themes" at http://drupal.org/node/176045, if you are not
familiar with installing themes, or need help installing.

Download the theme from the project page at http://drupal.org/project/p6_base to
your themes folder. It is usually sites/all/themes directory.

There are two methods for creating a sub-theme for p6_base main theme. The
easiest and safest way is using the Drush tool. You can also create a sub-theme
manually.

  1.Creating a sub-theme using Drush

    Copy the downloaded theme into your site's themes directory.
    i.e. sites/all/themes/p6_base

    Empty Drush cache so the Drush command would be available.
      drush cache-clear drush

    Run following command to see available options:
      drush help p6_base

    Create a sub-theme, using the default options.
      drush p6_base "My theme name"

    Or, create a sub-theme with the options you define:
      drush p6_base "My theme name" my_theme --description="My theme description..."

    Drush, will create a sub-theme in the same directory as p6_base with the
    defined options.

  2.Creating a sub-theme manually

    Copy "P6_SUBTHEME" folder to sites/all/themes directory.
    Rename "P6_SUBTHEME" folder to "my_theme"
    Rename P6_SUBTHEME.info.txt to my_theme.info
    Rename all the files prefixed with "p6_subtheme." to "my_theme."
    Edit "my_theme.info" and "template.php" to do a find & replace;
    Find "P6_SUBTHEME" and replace with "my_theme".


Enable your new theme
---------------------
Visit /admin/appearance page and enable your new sub-theme.


Sponsorship
-----------
This project is developed by Osman Gormus and sponsored by Project6 Design, Inc., a leading Drupal design firm in the San Francisco Bay Area. Visit us at www.project6.com or contact us at drupal@project6.com
