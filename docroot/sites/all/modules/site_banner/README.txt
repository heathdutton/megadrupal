
-- SUMMARY --

Site banner allows you to display a banner at the top and bottom of a 
Drupal site. It is displayed at a fixed position so content is viewed
within these top and bottom banners much like the administrator's 
toolbar. It also adds the banners to printed copies of Drupal site 
content.

It's a useful module for:
- internal sites where you hold sensitive information, and want to 
make sure that your users are aware of the content's sensitivity.
It would also help if you need that information displayed on the 
printed version of the site.
- site admins deploying a test site and want to let all visitors know 
that the site is for testing.

It also has integration with the Context module, so you can configure 
contexts to alter the banner status, background and text color and 
text depending on the context displayed. I found this is particularly 
useful if you combine it with the "Taxonomy" condition: so your 
banner text changes along with the content being displayed. For 
example, if you have sensitive information on a particular Drupal 
node that your visitors need to know is sensitive.

This module is compatible with Internet Explorer 8 - where the top
and bottom banners are rendered correctly on screen and in print.
Firefox 22 renders these banners well, except you need to change the 
page margins in the print version. Chromium 25 renders the top and 
bottom banners on the first page, but  it does obscure some text on 
the first page footer area. I haven't found a way to change the 
print settings in Chromium. All testing was done on Windows XP
or Windows 7.

I have not been able to test this module in Opera, Safari or other 
versions of browsers or operating systems, but I welcome 
contributions from anyone!

-- REQUIREMENTS --

* No formal prerequisites

* Context (https://drupal.org/project/context) module and its
  dependencies if you want to use the Context module integration.

-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further 
  information.

* Go to Administration » Configuration » User interface » Site Banner
  and configure the site banner text and colours.


-- CONFIGURATION --

* Go to Administration » Configuration » User interface » Site Banner
  and

  - set the banner text and either: 
  
    - either select a predefined background colour,

    - or select a new/existing custom colour and define it using a 
    six-digit hexadecimal value. For example, #000000 for black and 
    #FFFFFF for white.

* The theme adjusts the following regions in the Drupal page (by CSS 
  tags):
  
  - #page-wrapper for all content, and
  
  - body.toolbar for the administrator toolbar, and body.overlay to 
  compensate for adjusting the above #page-wrapper.

  If your theme or module does not look right with the Site Banner 
  module, please change the CSS in your theme/module, or in the 
  "site_banner_screen.css" file in this module.
  
  I have developed this module with the Toolbar module (in Drupal 
  core) Administrator Menu and the Adminimal Administrator Menu 
  modules. Please contact the developer for support for other
  toolbars, or feel free to commit your fixes as you develop them.

* To use the context module reactions, go to Administration » 
  Structure » Context.
  
  - add a new context or edit an existing context, and
  
  - under "Reactions", select "Change site banner status", "Change 
  site banner text", "Change site banner background color" or 
  "Change site banner text color".
  
  Background color and text color work identically to the 
  administration page version. However, changing site banner text 
  allows you to combine multiple contexts tagged with the same tag: 
  in this instance, the first detected prepend, append and delimiter 
  texts are used. The module will warn if different texts are 
  detected in successive contexts tagged with the same tag. The site
  banner status and debug mode work on a logical-OR basis: if at
  least one active context or the main settings enable the site
  banner or its debug mode then it will be enabled. 

-- REFERENCES --

The CSS code used to create the top and bottom banners was based on 
the article from Dynamic Drive CSS library: 

http://www.dynamicdrive.com/style/layouts/category/C11

-- CONTACT --

Current maintainers:
* Anthony Joseph (ajosephau - https://drupal.org/user/2543514)
