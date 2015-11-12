-- SUMMARY --

The Web Experience Toolkit 4 (WET_BOEW) theme is part of a suite that
implements the Web Experience Toolkit (v4.x) front-end framework into Drupal.
The framework, lead by the Government of Canada, is open-source and
collaborative, and is made to be accessible, mobile, and responsive. You can
find the full documentation here: http://wet-boew.github.io/wet-boew/index.html.

The Web Experience Toolkit distribution (https://www.drupal.org/project/wetkit)
is a content management system. If you prefer an out-of-the-box system, please
refer to the distribution.

This suite is meant for site builders who prefer to have a bit more control and
provides a simple roadmap for upgrading WET components. By decoupling the
features and leveraging Drupal’s module and theming system, we are able to
provide quicker theme updates and keep the WET overhead low.

It is made to the WET's specs so that all features work:

* Mobile support, including menus
* Plugin support, including carousel, charts, tabs, and lightbox
* It includes both the Web Usability and Intranet theme
* It has multilingual support

A demo and full list of plugins can be found here:
http://wet-boew.github.io/wet-boew/demos/index-en.html

This WET4 suite includes:

* WET4 theme
* WET4 Menu module
* WET4 Slider module
* WET4 Panel Layouts module
* WET4 Site Information module
* WET4 Migration module

-- REQUIREMENTS --

None.


-- INSTALLATION --

* Install as usual, see https://www.drupal.org/getting-started/install-contrib/
  themes for further information.

* Enable the theme and set it as default by going to Administration >
  Appearance.


-- CONFIGURATION --

* Configure the theme like other themes by going to Administration > Appearance
  > Settings > WET4

  - You can set either the WEB or Intranet theme as the base theme.
  
* If you'd like to use the WET4 main menu style, please install the WET4_menu
  module found here: https://www.drupal.org/sandbox/ashopin/2433195.

* Drupal menu blocks or blocks created by the menu_block module can be used for
  the top links, sidebar menu, footer menus, and bottom menu.

  - Set the block title to <none> for proper styling
  
* Multilingual toggles should be placed in the top_links region as the last
  block for proper styling.
  
* The main menu can be up to 2 levels deep, but links will be stripped out of
  parent links in order to be accessible on touch screen devices.

* WET4 is more strict with its sidebar blocks. It is recommended that the
  sidebar be used for one menu block only. You can use the sidebar-aside region
  for content-related blocks.
  
-- CUSTOMIZATION --

* The theme includes a subtheme folder that can be used to provide full theme
  overrides.

  - To override a template, copy it from the Wet4 folder and paste it in your
  subtheme's template folder.
  
  - Rename the folder, .info file, and edit the .info file to include your
  custom theme's name.
  
  - You can use the css/style.css file to provide your style overrides or drop
  in SASS files into your subtheme folder and reference the compiled CSS in the
  subtheme's .info
  file.


-- TROUBLESHOOTING --

* If the menu does not display, check the following:

  - Are the "Access administration menu" and "Use the administration pages and
  help" permissions enabled for the appropriate roles?

  - Does html.tpl.php of your theme output the $page_bottom variable?

* If the menu is rendered behind a Flash movie object, add this property to your
  Flash object(s):

  <param name="wmode" value="transparent" />

  See http://drupal.org/node/195386 for further information.


-- FAQ --

Q: Can I install this theme on an existing WET distribution site?

A: Yes, this theme can be installed on new Drupal installation, existing
   projects, and WET distribution sites. Please keep in mind that there are
   quite a few variances between WET3 and WET4 (i.e the grid classes) so you
   may have to do a bit of restyling or restructuring.


Q: When I add more than one menu or add non-menu blocks to the sidebar, they
   disappear in mobile view.

A: WET4 is more strict with the type of content that can be placed in the
   sidebar due to the mobile JS library. All non-menu items will be stripped
   out and only the first menu in the sidebar will be used.


-- CONTACT --

Current maintainers:
* Ash Ahmadzadeh (ashopin)- https://www.drupal.org/u/ashopin
* Steve Lavigne (Nugg) - https://www.drupal.org/u/nugg
* Pat Cooney
* Andrew Treblle

This project has been sponsored by:
* OPIN Software Inc.
  OPIN is a provider of enterprise content management solutions built with
  Drupal. At OPIN, our vision is to assist clients through Drupal
  implementation and consulting and to help them achieve an effective web
  presence.
