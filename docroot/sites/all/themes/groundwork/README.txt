GROUNDWORK FRONT-END FRAMEWORK
==============================

Thank you for using Groundwork!


INTRODUCTION
------------

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
* An empty starter stylesheet
* and many more.


GETTING STARTED
---------------

For the complete documentation, please visit http://groundwork.noceda.me

Before you begin, make sure that you have installed (and configured) Drupal.
You need to install the LESS module <https://drupal.org/project/less> as well
to utilize the power of Groundwork.


### INSTALLATION VIA COMMAND LINE (DRUSH) ###

1. Download Groundwork:
   drush dl groundwork
2. Clear the Drush cache:
   drush cc drush
3. Create your custom theme. For this example, we name it Philippine Sea with
   machine name philsea:
   drush groundwork 'Philippine Sea' philsea
4. Enable and set your new theme philsea as default:
   drush en philsea --yes && drush vset theme_default philsea

See "drush help groundwork" to view advanced options.


### MANUAL INSTALLATION ###

1. Download Groundwork from <https://drupal.org/project/groundwork>
2. Extract the downloaded file and place the groundwork folder in your Drupal
   installation under one of the following locations:
     * sites/all/themes
     * sites/default/themes
     * sites/example.com/themes

   Please check Drupal documentation to learn the differences of these folders:
   <https://drupal.org/getting-started/install-contrib/themes>
3. Copy the folder CustomWork (found inside the Groundwork folder) to the same
   folder as where you extracted Groundwork.
     For example:
     sites/all/themes/groundwork
     sites/all/themes/CustomWork
4. Rename the folder of CustomWork to the desired machine name of your new
   custom theme. For example: philsea
     sites/all/philsea
5. Inside the 'philsea' folder, rename CustomWork.info.txt to the same name of
   the folder and remove the '.txt'.
     For example: philsea.info
6. Edit philsea.info and find name= CustomwWork This is the human-friendly name
   of your theme. Change it to the name of your theme.
     For example: name= Philippine Sea
7. Edit app-settings.css.less found inside the custom directory of your new
   theme, and define the location of your theme.
     For example: @themeDirectory: sites/all/themes/philsea;
8. In the browser, log in as admin in your Drupal installation and go to
   Administration > Appearance > List: http://example.com/admin/appearance/list.
   Click the "Enable and set default" link on your new custom theme.

Note: It is not necessary to enable Groundwork but it must be present.

### ALTERNATIVE INSTALLATION ###

Use an existing sub-theme. Easy design development:

1. Look for a Groundwork sub-theme which looks the nearest you want to achieve
   with your site design. Download and install it.
2. Go to the custom folder of the subtheme and rename all 5 files that end with
   css.less.txt to css.less.
   For example: app-settings.css.less.txt to app-settings.css.less
3. Customize away!

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

Thanks for downloading the Groundwork Framework and I hope that you find it
useful for your needs.

Please visit <http://groundwork.noceda.me> for the full documentation and demo.

**John Noceda (JohnNoc)**
