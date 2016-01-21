SIGMAONE - Theme framework by victheme.com

Features :

Modular CSS
===========

Theme css files is located in sigmaone/css folder, they were designed
to be as modular as possible. Subtheme can easily overrides any of the
main css files by specifying css entry in the subtheme .info file.

Example :
If a subtheme called "mysubtheme" want to override sigmaone sytle.css
then in mysubtheme.info file user has to specify this entry :
stylesheets[all][] = css/style.css.

CSS file :
block.css - All CSS styling related to blocks is defined here.
colors.css - This css is to colorize sigmaone. 
comment.css - This is the default sigmaone comment related style.
contrib-fix.css - Collection of style to fix contribution module if it 
                  is broken in sigmaone.
forum.css - The main styling for forum related page
generic.css - generic class helper
ie7-fixes.css - style fix for IE7, only loaded if browser is IE7
navigation.css - Menu styling for #navigation with css dropdown
nodepage.css - Sytle related to node page.
poll.css - extra style for poll module
searchpage.ss - extra style for search page
userpage.css - extra style for user page area



Templates
=========

Sigmaone has several Drupal templates in sigmaone/templates folder, those
templates is related to sigmaone CSS files.

Subtheme can override the templates by copying the file to subtheme/templates folder
and refreshing Drupal theme cache.



Integrated VTCore template system
=================================

Sigmaone is integrated with VTCore system, any sigmaone subtheme will inherit the system.

VTCore included in sigmaone
Layout - The layout system, a VTCore core plugin.
Adminpage - Additional styling for admin page.
Blocks - Transforming Drupal standard blocks such as user new blocks into a better one.
Color - Integrating color module configuration form into VTCore.
Copyright - Provide a vt-block for copyright notice.
Loginbar - Provide a horizontal login bar block as a vt-block.
Metadata - Provide a site wide meta tagging capability to VTCore.
HTML5 - Provide a HTML5 capabilities to VTCore.
CSSManager - Change CSS information loaded by Drupal via GUI.
Page Title - Specify custom title for site.
Plugin Manager - A VTCore core plugin to manage all VTCore plugin.
RegionArea - GUI plugin for creating layout files (or database entry) that compatible with layout plugin
Responsive - Adding responsive design capabilities to VTCore.
Style - Provide styling to Drupal elements
Typography - Create custom typography CSS via GUI.
jQueryUI - Integrating jQueryUI styling to VTCore and Drupal.



Installation
============

Sigmaone doesn't need any special installation procedure, you can install it as a normal
Drupal theme installation would be.

After installation, it is REQUIRED that you set sigmaone as the administration theme.
Otherwise sigmaone / subthemes configuration setting page will not display properly, especially
for regionarea plugin and color module.

Next step after sigmaone theme is installed properly and set as administration theme,
you will need to create a subtheme for sigmaone. Eventhough sigmaone it self can act as the front
end theme, it is not adviseable to edit sigmaone directly. You will lose all the customization
and custom files whenever you upgrade sigmaone.


Creating Subtheme
=================

Copy StaterKit folder to sites/all/themes 
Change the StarterKit folder name & StarterKit.info
Edit StarterKit.info to match your theme
Edit CSS file to match your design
Create new Layout from theme configuration page
