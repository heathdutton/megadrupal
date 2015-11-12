--------------------------------------------------------------------------------
                                Style Switcher 2
--------------------------------------------------------------------------------

Provides a stylesheet switching functionality.

--------------------------------------------------------------------------------
INTRODUCTION
--------------------------------------------------------------------------------

This module takes the fuss out of creating themes or building sites with
alternate stylesheets. Themer can provide a theme with alternate stylesheets.
Site builder can add alternate stylesheets found in the internet right in the
admin section. And this module presents all those styles to site users as
a block with a list of links. So any site user is able to choose the style of
the site he/she prefers.

There are many commonly known JavaScript techniques to switch CSS stylesheets
"live", without reloading the page. Some examples of them and relevant
JavaScript files can be found here:
  http://www.alistapart.com/articles/alternate
  http://www.kelvinluck.com/2009/07/jquery-styleswitch-now-with-toggle/
  http://www.immortalwolf.com/code-repository/javascript-components/

This module uses a slightly different and more powerful mechanism. But
usability is the same: click on a style label » get the new look of the site.

Works in all major browsers (including Internet Explorer).

For more information about the module, visit the Drupal.org Documentation page:
 * https://www.drupal.org/node/2544814
To submit bug reports and feature suggestions, or to track changes:
 * https://www.drupal.org/project/issues/styleswitcher

--------------------------------------------------------------------------------
INSTALLATION/UPDATE
--------------------------------------------------------------------------------

Install as you would normally install a contributed Drupal module. See
https://www.drupal.org/documentation/install/modules-themes/modules-7 for
further information.

If you're updating from a previous version don't forget to run update.php. See
https://www.drupal.org/node/1782798 for further information.

--------------------------------------------------------------------------------
CONFIGURATION
--------------------------------------------------------------------------------

There are 2 ways to configure the list of styles for switching. And they can be
combined.

 * One way is for site admins:

   1. Create stylesheet(s) in site's public directory, for example: in
      sites/default/files, or anywhere in the internet. Just remember that these
      locations along with the CSS files must be publicly accessible.

   2. Go to Administration » Configuration » User interface » Styleswitcher.
      There you'll find a module's configuration form and a link to add
      stylesheets for switching. Add all your styles providing their labels and
      paths.

 * Another way is mostly for themers. For site builders it is appropriate only
   if you use a custom theme that you are able to edit:

   1. In your theme's directory or subdirectory create stylesheet(s) for each
      different style.

      Note that each stylesheet when switched is being added to the existing set
      of other site's stylesheets.

   2. In your theme's .info file add a section like the following, replacing
      labels and paths with yours:

        ; Switching stylesheets
        styleswitcher[css][Light Blue] = css/blue.css
        styleswitcher[css][All Black] = css/black.css
        styleswitcher[default] = All Black

      where "All Black" is the label of the style, and css/black.css is the
      location of the stylesheet file, relative to theme's root. A line with
      Default is optional - it is for new site visitors. If it is not set then
      no of alternatives will be visible until user explicitly chooses one.

      NOTE: Do not specify in this section any of other existing CSS loaded by
      Drupal core, modules or your theme. They are being loaded unconditionally.
      If you want to set some of your theme's stylesheets for switching then
      move them from "stylesheets" section of .info file to the "styleswitcher"
      section.

The next steps are for both of previous ways:

 3. In the Styleswitcher admin section you can see tabs for each of site's
    enabled theme. There you can reorganize how style links will be displayed
    to users: you can reorder them, enable/disable some of them and choose
    another default style for each theme. This means that each of your site's
    theme can have its own set of styles for switching.

    Note that styles added in admin form will be available for all themes, but
    those provided in theme's .info will only be available for that theme and no
    other.

 4. Finally, go to Administration » Structure » Blocks, find "Style Switcher"
    block, place it wherever you want and press "Save". You can also configure
    the block however you like.

    Note that this block is the only way to provide a style switching for users.
    So if you need this functionality in multiple themes then you should
    configure the block to be displayed in appropriate themes.

That's it. The block will present links for each of your styles to you and your
users.

--------------------------------------------------------------------------------
AUTHORS & MAINTAINERS
--------------------------------------------------------------------------------

 * Mike Shiyan [https://www.drupal.org/u/pingwin4eg]
 * Alan Burke [https://www.drupal.org/u/alanburke]
 * Roger López [https://www.drupal.org/u/zroger]
 * Kristjan Jansen [https://www.drupal.org/u/kika]
