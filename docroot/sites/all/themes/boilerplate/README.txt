Boilerplate was put together by Dan Linn at Metal Toad Media for use internally. It was 
good enough he felt he should let others use it, too. He agreed with the Basic theme 
(http://drupal.org/project/basic) developers and thought Zen was getting a little hefty. 
Using elements from Basic, Boron (Another Metal Toad theme), and the 
HTML 5 Boilerplate 3.0, this theme was born.

__________________________________________________________________________________________

Installation

- Download Boilerplate from http://drupal.org/project/boilerplate
- Unpack the downloaded file and place the Boilerplate folder in your Drupal installation under 
  one of the following locations:

    * sites/all/themes
    * sites/default/themes
    * sites/example.com/themes 

- Log in as an administrator on your Drupal site and go to 
  Administer > Site building > Themes (admin/build/themes) and make Boilerplate the default theme.

- if you want to change the name of the theme from 'Boilerplate' to another name like 'mytheme',
follow these steps (to do BEFORE enabling the theme) :

	- rename the theme folder to 'mytheme'
	- rename boilerplate.info to mytheme.info
	- Edit boilerplate.info and change the name, description, project (can be deleted)
	- In template.php change each iteration of 'boilerplate' to 'mytheme'
	- In mytheme.info change each iteration of 'boilerplate' to 'mytheme'
	- In theme-settings.php change each iteration of 'boilerplate' to 'mytheme'

  - OR, you can simply use Drush to create a new theme based on Boilerplate.  Just type "drush boilerplate <new theme name>" and you're done.
__________________________________________________________________________________________

What are the files for ?
------------------------

- boilerplate.info => provide informations about the theme, like regions, css, settings, js ...
- block-system-main.tpl.php => template to edit the content
- block.tpl.php => template to edit the blocks
- comment.tpl.php => template to edit the comments
- node.tpl.php => template to edit the nodes (in content)
- page.tpl.php => template to edit the page
- template.php => used to modify drupal's default behavior before outputting HTML through 
  the theme
- theme-settings => used to create additional settings in the theme settings page

In /css/compiled
-------

- ie6 => used to debug IE6
- ie7 => used to debug IE7
- ie8 => used to debug IE7
- style.css => contains the base css loaded on every page. It is recommended that you use the 
               included Sass files to edit these styles: http://sass-lang.com/
- tabs.css => styles for the admin tabs (from ZEN)

In /SASS
-------

- _base.scss => define sass variables and partials to be included in all .scss files
- _custom.scss => define custom styles for page elements
- _defaults.scss => establishes general rules and browser resets
- _ie6.scss => Fiexes to support IE6
- _ie7.scss => Fiexes to support IE7
- _ie8.scss => Fiexes to support IE8
- _layout.scss => define the layout (positioning) of the theme's major elements
- _mixins.scss => define custom sass functions and mixins for use in other .scss files
- _mobile.scss => define the way the theme addapts on mobile devices
- _print.scss => define the way the theme look like when printed
- style.scss => assembles all partials into a base css to be loaded on every page
- tabs.scss => styles for the admin tabs (from ZEN)

__________________________________________________________________________________________

Changing the Layout

The layout used in Boilerplate is fairly similar to the Holy Grail method. It has been tested on 
all major browser including IE (5>8), Opera, Firefox, Safari, Chrome ...
The purpose of this method is to have a minimal markup for an ideal display. 
For accessibility and search engine optimization, the best order to display a page is ]
the following :

	1. header
	2. content
	3. sidebars
	4. footer

This is how the page template is built in boilerplate, and it works in fluid and fixed layout.
Refers to the notes in _layout.scss to see how to modify the layout.

__________________________________________________________________________________________

UPDATING boilerplate

Once you start using boilerplate, you will massively change it until a point where it has nothing
to do with boilerplate anymore. Unlike ZEN, Boilerplate is not intended to be use as a base theme for a 
sub-theme (even though it is possible to do so). Because of this, it is not necessary to
update your theme when a new version of Boilerplate comes out. Always see boilerplate as a STARTER, and 
as soon as you start using it, it is not Boilerplate anymore, but your own theme.

If you didn't rename your theme, but you don't want to be notified when boilerplate has a new version
by the update module, simply delete "project = "boilerplate" in boilerplate.info

__________________________________________________________________________________________

Thanks for using Boilerplate, and remember to use the issue queue in drupal.org for any question
or bug report:

http://drupal.org/project/issues/boilerplate

Current maintainers:
Dan Linn - dan@metaltoad.com