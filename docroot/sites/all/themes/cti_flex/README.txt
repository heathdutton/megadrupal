
ABOUT THIS THEME
=================

CTI Flex is a highly configurable sub-theme of the Zen base theme for Drupal 7. 
On the theme settings page, users can select a page layout, a color-scheme, 
font-family and font-size and can select from different options for block corner 
styles. In addition, if the Skinr module is installed, several block styles 
are provided to match the available color schemes. Finally, the theme has 
been set up to allow for easy development of your own custom color scheme. 

THEME FEATURES
==============

- A Zen sub-theme
- 1,2 or 3 columns
- Fluid or fixed width styles
- 22 pre-designed color schemes
- Template for creating custom color scheme
- Configurable font styles/sizes
- CSS3 round corners (only in compliant browsers)
- Extra block styles if used with the Skinr module
- Built-in styling for dropdowns using the Superfish module

INSTALLATION AND BASIC CONFIGURATION
======================================

1. Install both CTI Flex and the Zen base theme in the same theme directory

2. Enable and set CTI Flex as the default (you do not need to enable Zen)

3. Install and enable the Skinr module (optional)

4. Set any necessary permissions for the Skinr module 

5. Go to the CTI Flex theme configuration page to select a layout, color scheme, 
font family, and block corner styles. 

6. Select styles for individual blocks (if Skinr is installed) by selecting the
"Edit Skin" option and choosing options in the CTI Flex Block Styles section.  


LOCAL.CSS AND CREATING CUSTOM COLOR SCHEMES
===========================================

If you want to modify the theme or add additional styles, it is recommended to
override CSS or add new CSS in local.css instead of modifying the existing
stylesheets. *Important - Be sure to make a copy of this file when doing theme
upgrades and replace local.css after the upgrade.  

The local.css stylesheet also provides a template to help you create your
own custom color scheme.  To do so, on the theme configuration 
page (admin/appearance/settings/cti_flex), in the Color Scheme
section, select "Custom" from the dropdown.  Then refer to the section 
below, entitled "CUSTOM COLOR SCHEME STYLES".  Colors have been set
using text notation, to make it quick and easy to identify where the 
colors are applied.  Replace these colors with your own HTML color codes
to create your own color scheme.