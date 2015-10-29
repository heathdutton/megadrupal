
Installation

- Download fusebasic from http://drupal.org/project/fusebasic
- Unpack the downloaded file and place the fusebasic folder in your Drupal installation under
  one of the following locations:

    * sites/all/themes
    * sites/default/themes
    * sites/example.com/themes

- Log in as an administrator on your Drupal site and go to
  Administer > Site building > Themes (admin/build/themes) and make fusebasic the default theme.

- if you want to change the name of the theme from 'fusebasic' to another name like 'mytheme',
follow these steps (to do BEFORE enabling the theme) :

	- rename the theme folder to 'mytheme'
	- rename fusebasic.info to mytheme.info
	- Edit fusebasic.info and change the name, description, projet (can be deleted)
	- In fusebasic.info replace [fusebasic_block_editing] and [fusebasic_rebuild_registry]
	  by [mytheme_block_editing] and [mytheme_rebuild_registry]
	- In template.php change each iteration of 'fusebasic' to 'mytheme'
	- In theme-settings.php change each iteration of 'fusebasic' to 'mytheme'

__________________________________________________________________________________________

What are the files for ?
------------------------

- fusebasic.info => provide informations about the theme, like regions, css, settings, js ...
- block-system-main.tpl.php => template to edit the content
- block.tpl.php => template to edit the blocks
- comment.tpl.php => template to edit the comments
- node.tpl.php => template to edit the nodes (in content)
- page.tpl.php => template to edit the page
- template.php => used to modify drupal's default behavior before outputting HTML through
  the theme
- theme-settings => used to create additional settings in the theme settings page

In /CSS
-------

- default.css => define default classes, browser resets and admin styles
- ie6 => used to debug IE6
- ie7 => used to debug IE7
- layout.css => define the layout of the theme
- print.css => define the way the theme look like when printed
- style.css => contains some default font styles. that's where you can add custom css
- tabs.css => styles for the admin tabs (from ZEN)

__________________________________________________________________________________________

Changing the Layout

The layout used in fusebasic is fairly similar to the Holy Grail method. It has been tested on
all major browser including IE (5>8), Opera, Firefox, Safari, Chrome ...
The purpose of this method is to have a minimal markup for an ideal display.
For accessibility and search engine optimization, the best order to display a page is ]
the following :

	1. header
	2. content
	3. sidebars
	4. footer

This is how the page template is buit in fusebasic, and it works in fluid and fixed layout.
Refers to the notes in layout.css to see how to modify the layout.

__________________________________________________________________________________________

UPDATING fusebasic

Once you start using fusebasic, you will massively change it until a point where it has nothing
to do with fusebasic anymore. Unlike ZEN, fusebasic is not intended to be use as a base theme for a
sub-theme (even though it is possible to do so). Because of this, it is not necessary to
update your theme when a new version of fusebasic comes out. Always see fusebasic as a STARTER, and
as soon as you start using it, it is not fusebasic anymore, but your own theme.

If you didn't rename your theme, but you don't want to be notified when fusebasic has a new version
by the update module, simply delete "project = "fusebasic" in fusebasic.info

__________________________________________________________________________________________

Thanks for using fusebasic, and remember to use the issue queue in drupal.org for any question
or bug report:

http://drupal.org/project/issues/fusebasic
