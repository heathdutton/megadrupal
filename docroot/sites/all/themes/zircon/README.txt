CONTENTS OF THIS FILE
-------------------------------------------------------------------------------
 * About WeebPal
 * Zircon Specification
 * Ziron Supports Detail
 * Ziron Config Notes
 * How to use Zircon?
 * More documentation

ABOUT WEEBPAL
-------------------------------------------------------------------------------
WeebPal is a startup team consist of Developers, Stylers and Designers. Our 
slogan is "Usability - Technical - Art". We understand that Drupal is very 
strong and promising CMS and we hope that we contribute some small thing to 
help Drupal become more friendly to Users. Step by step, we'll build the themes
can combine:
 * Design
 * Easy to use
 * Easy to customize
 * Drupal theme must be web solutions
 * Support multi-devices
and give the solutions for the web cases:
 * The Popular layout
 * The Shopping online
 * The Education solution
 * The Reponsive layout
 * The News solution
 * The Social Network solution
 * ...
And to do all of theme, We believe in Nucleus sbasetheme can help us to do it.

ZIRCON SPECIFICATION
-------------------------------------------------------------------------------
We would like to start with Zircon, a very simple theme with a popular layout.

 * Nucleus compatability
     Compat with Nucleus version 1.1+

 * Layout
	 Support popular layout with three columns

 * Colors
   * red (Default)
   * blue (Blue Style)
   * orange (Orange Style)
   * pink (Pink Style)

 * Block styles
   * raw (Raw): support block without skin
   * rounded (Rounded): support block with the rounded corner
   * custom (Custom): support style for the custom blocks
   * view (Views): support style for blocks of module Views
   * badge (Badge): support block with a badge when showing
   * widget (Widget): support block for the blocks of Widget module

 * Block extend classes:
   * view-main-slideshow (Main Slideshow)
   * view-grid-panel (View Grid Panel)
   * view-horizontal-carousel (Horizontal Carousel)
   * view-vertical-carousel (Vertical Carousel)
   * block-custom-quote (Quote Block)
   * block-custom-contact (Contact Block)
   * block-custom-links (Links Block)
   * block-custom-address (Address Block)
   * block-custom-form (Form Block)
   * widget-facebook (Facebook)
   * widget-twitter (Twitter)
   * block-badge-hot (Badge Hot)
   * block-badge-new (Badge New)
   * block-badge-pick (Badge Pick)
   * block-badge-top (Badge Top)

 * Support style for third party modules:
   * Superfish
   * Quicktabs
   * Views
   * Views-Slideshow
   * jCarousel
   * Gallery Formatter
   * Colorbox
   * Widget
   * SocialMedia

ZIRCON SUPPORTS DETAIL
-------------------------------------------------------------------------------
 * Logo
   * Logo
   * Site name width Site slogan

 * Modules
   * Views
     * view-page
       * grid
       * unformatted
     * view-block
       * html-list
       * unformatted
     * views-slideshow
       * pager
       * control
       * number slide
     * jCarousel
       * horizontal
       * vertical

   * Menu
     * Superfish menu
       * horizontal
       * navbar
     * Drupal default menu
       * in menu_bar region
       * in footer region

   * Quicktabs
     * accordion
     * quicktabs
     * ui_tabs

   * Widget
     * SocialMedia
       * Facebook
       * Twitter

ZIRCON CONFIG NOTES
--------------------------
 * Logo 
   Please don't use all Logo, Sitename and Slogan on the same time
   Should use Logo or Sitename + Slogan 
 * Views
   * views-slideshow
     To best use the support style, please put image on the first

   * like panel-first region (use "view" block style & "view-grid-panel" class)
     we can put here: 
     * 1 views grid 4 columns
     * 2 views grid 2 columns
     * 4 views grid 1 columns
     * 4 custom block

   * menu_bar region: we can put here 
     * 1 horizontal superfish menu with zircon style
     * 1 navbar superfish menu with zircon style
     * 1 Drupal default like main menu

HOW TO USE ZIRCON?
-------------------------------------------------------------------------------
From Demo profile package:
This installation profile includes the same demo content of Zircon at:
http://zircon.weebpal.com/
or direct link at 
http://downloads.weebpal.com/tb_zircon.zip
http://downloads.weebpal.com/tb_zircon_blue.zip
http://downloads.weebpal.com/tb_zircon_orange.zip
http://downloads.weebpal.com/tb_zircon_pink.zip
Step 1: Extract demo profile package to your host.
Step 2: Navigate to the folder you have extracted the zip file and install 
  themebrain profile.

Theme only installation:
Step 1: Install Drupal.
Step 1: Install Nucleus basetheme from http://www.drupal.org/project/nucleus
Step 3: Extract included zip files Zircon into sites/all/themes folder.
Step 3: Enable & Set default Zircon theme. Go to theme settings under
  "Appearence" section and Install this theme via upload.
Step 4: Enable and set as default.

MORE DOCUMENTATION
-------------------------------------------------------------------------------
Read the documentation about using Nucleus at http://www.themebrain.com/guide
Read the quick guide for using Zircon at http://help.weebpal.com/zircon
Drupal theming documentation in the Theme Guide: http://drupal.org/theme-guide
