NEGOSYANTE
==========

Thank you for using Negosyante!


INTRODUCTION
------------

Negosyante is a progressive mobile first "blue and grey" theme with modular
scale typography and powerful grid-based layout engine. It's a sub-theme of
Groundwork

### What is Groundwork? ###

Groundwork is a front-end framework especially made for Drupal. It was built to
help you design beautiful websites faster and more efficient. As its name
implies, it's a groundwork which comes already with sensible generic style
defaults, CSS browser hacks, collection of mixins, bundles and more. Because of
Groundwork's basic generic style, you may also wish to use its starter sub-theme
as is, as a structured minimalistic theme or design your own.

Groundwork uses a 24-unit grid system as default. The layout options available
are endless, which makes the Groundwork suite of themes extremely versatile.
Customizing a website to specific layout and design is very easy.

Groundwork supports 17 regions whereas the regions page_top, page_bottom, and
help are special hidden regions. The other 14 regions are available to you for
unlimited block placement and layout options.

*Groundwork is not for everybody.*
If you are a designer who wants to start everything from scratch, you may want
to take a look at the great and powerful starter themes available at drupal.org
like Zen, AdaptiveTheme, Fusion or Omega, to name a few.


FEATURES
--------

Here are the key features of Groundwork:

* HTML 5
* Web accessibility optimized with ARIA roles
* Bulletproof flexible grid system with 24 columns (units) as default
* Typography using modular scale.
* True semantic markup with separation of style with content
* Supports both sidebars
* 17 regions with a special "Aside" region visible only in nodes
* Optimized typography for all standard elements
* Powerful layout engine provides unlimited layout possibilities
* Collection of preset CSS classes, mixins and style bundles
* Sexy configurable buttons
* HTML5 polyfill to support older browsers
* and many more.


GETTING STARTED
---------------

For the complete documentation, please visit http://groundwork.noceda.me

Before you begin, make sure that you have installed (and configured) Drupal.
You need to install the LESS module <https://drupal.org/project/less> as well
to utilize the power of Groundwork.


### INSTALLATION VIA COMMAND LINE (DRUSH) ###

1. Download Groundwork and Negosyante:
   drush dl groundwork negosyante
2. Enable and set your new theme negosyante as default:
   drush en negosyante --yes && drush vset theme_default negosyante


### MANUAL INSTALLATION ###

1. Download Groundwork from <https://drupal.org/project/groundwork> and
   Negosyante from <https://drupal.org/project/negosyante>
2. Extract the downloaded file and place both theme folders in your Drupal
   installation:
     * sites/all/themes/groundwork
     * sites/all/themes/negosyante
3. Edit app-settings.css.less found inside the custom directory of your theme,
   and define the location of your theme.
     For example: @themeDirectory: sites/all/themes/negosyante;
8. In the browser, log in as admin in your Drupal installation and go to
   Administration > Appearance > List: http://example.com/admin/appearance/list.
   Click the "Enable and set default" link on your new custom theme.

Note: It is not necessary to enable Groundwork but it must be present.


### RECOMMENDED MODULES ###

Negosyante is developed to be semantic, where you add styles and mixins in LESS
files, but if you wish to use presentation classes on your markup instead of
separating style with content, you may check the resources folder of Groundwork,
and use:

* Block Class <https://drupal.org/project/block_class>
  Add classes to any block through the block's configuration interface.
* Menu Attributes <https://drupal.org/project/menu_attributes>
  Specify additional attributes for menu items such as id and class.


#### Further Reading #####

* Learn more about sub-theming: <http://drupal.org/node/225125>
* Learn more about general Drupal theming in the Drupal theme guide:
    <http://drupal.org/theme-guide>


ACKNOWLEDGMENTS
---------------

Thanks to:

* the awesome contributed Drupal starter themes available at
  <http://drupal.org/project/themes> and their great theme developers
  for the inspiration and ideas.

* most of the CSS front-end frameworks out there for the inspiration and ideas,
  like Foundation, Bootstrap, LESS Framework, and many more.


FINAL WORD
----------

Thanks for downloading Negosyante and the Groundwork Framework and I hope that
you find it useful for your needs.

Please visit <http://groundwork.noceda.me> for the full documentation.

**John Noceda (JohnNoc)**
