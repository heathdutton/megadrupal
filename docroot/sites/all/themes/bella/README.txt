-- SUMMARY --
The Bella theme is designed to be more configurable and more user-friendly than most
standard Drupal themes, with an emphasis on the "little things" that most themes lack.
Some examples range from being able to (easily) customize the breadcrumb separator, to
being able to set whether or not user pictures are displayed in "teaser" mode, all the way
to telling the theme to scan for custom css files so that modifications can be made to the
theme without having to hard-code said modifications (because when you hard-code
something, the next time the theme requires an update, all modifications will be lost).
Not to mention hard-coding is a big no-no. Never hard-code stuff. It may seem convenient
at the time, but ultimately it will make things much harder.

-- REQUIREMENTS --
1) Drupal
2) The file found here: http://www.hooktheme.com/hookthemejs.zip

-- INSTALLATION --
1) Install the theme just as you normally would any other theme
2) Download the file http://www.hooktheme.com/hookthemejs.zip and extract its contents
   into the folder named "js" which should already exist inside this theme's directory.
    - So if the theme lives at "sites/all/themes/bella", copy or move each .js file from
      the zip file you downloaded so that it now lives at
      "sites/all/themes/bella/js/[WHATEVERFILE].js"
3) Once installed, navigate to www.yoursite.com/?q=admin/appearance/settings/bella to
   configure the theme to your liking.

-- CONFIGURATION --
Once installed, go to www.yoursite.com/?q=admin/appearance/settings/bella. At this point
you should see the normal theme options at the top, and at the bottom you should see some
additional options, as do follow:

------------------------------------------------------------------------------------------
* Show/hide titles

** Sometimes, depending on the genre of the site, you may wish to disable the display of
page titles while in "full" view. To do so, go to the theme settings page and check the
box that reads "Hide titles on full pages".
------------------------------------------------------------------------------------------
* Social links

** You may optionally add some of your own social links to your site's display by
enabling / configuring them on the theme settings page.
------------------------------------------------------------------------------------------
* Breadcrumb

** Display breadcrumb: Sometimes you may not want a breadcrumb displayed at all anywhere
on your site. If that's the case, simply uncheck this box and no breadcrumb will be
displayed.

** Breadcrumb separator: This allows you to customize what is displayed in between each
breadcrumb item. An example of what you can choose to display in between might be a
"heart" symbol, in which case you could enter &hearts; into the box and the result would
be each item in the breadcrumb area being separated by a "heart" symbol. NOTE: if you
don't surround &hearts; with a space on both sides, there will be no spaces in between
the separators and the breadcrumb items (so in most cases you'll probably want to put
a space on both sides of &hearts;). You could also enter &nbsp;&hearts;&nbsp; without
the surrounding spaces and achieve the same results. Another possibility would be to enter
&nbsp;&#9829;&nbsp; which would produce the exact same output as &nbsp;&hearts;&nbsp;
would.

You can also enter something like <span class="my-custom-breadcrumb-class"></span> -- this
is especially useful if you tell the theme to scan for custom stylesheets (explained
below). Allowed HTML tags are <div> and/or <span> for this setting.
------------------------------------------------------------------------------------------
* Menus

** Expand main menu: If your main menu has sub-menu items that you want to be presented
inside a dropdown menu, you will need to enable this feature by checking the box. You'll
then need to navigate to http://www.yoursite.com/?q=admin/structure/menu/manage/main-menu
and, next to each menu item, you should see a link that says "edit". If you want a
particular menu item to appear in the dropdown menu, click its "edit" link and make sure
the box that reads "Show as expanded" is checked.
------------------------------------------------------------------------------------------
* Front page

** Show the title on the front page: By default, this box should be checked. If you wish
to hide the page title on the front page only, uncheck this box. Note that this setting
has no effect on the titles of teasers that appear on the front page... Only the main
title itself will be hidden (or shown).

** Hide "No front page content has been created yet": If this option is selected, and if
the user is viewing the front page, and if no content has been promoted to the front page
yet, the message that would normally be displayed to the user that reads "No front page
content has been created yet" will be hidden.
------------------------------------------------------------------------------------------
* Teasers

** Display a separator between teasers on the front page: You might want each node on the
front page of your site to be separated from the next one, making it "stand out" a little.
In that case, depending on the theme you're using, selecting this box will cause a small
separator to be displayed between each node on the front page.

** Display a separator between teasers throughout the entire site: Same as above, except
the rule applies to any teasers found on your site (not just on the front page, although
the front page is included). The above setting has no effect if this box is checked.

** Display user pictures inside teasers: By default, Drupal core has never offered an
option to exclude user pictures from teasers only, and it doesn't look like that will
change with Drupal 8 either. It *does* give the option to exclude (or include) "submitted
by" information on each content type, but there's no option for differentiating between
teaser mode and page mode. By (de-)selecting this box, you can toggle whether your users'
pictures, if they have pictures, will be displayed in page mode and teaser mode, or just
in page mode (not within teasers).
------------------------------------------------------------------------------------------
* Custom files (theme customizations)

** Scan for custom .inc (PHP) files: If there are certain PHP functions you'd like to be
available to you, but maybe you don't want to or don't know how to write a custom module,
you can optionally create a file inside the theme directory called "bella.custom.inc". By
doing so, any functions defined inside that file will automatically be included just
before the theme is rendered. It's much like adding custom functions to the template.php
file, except when the theme requires an update, the custom functions you may have
otherwise hard-coded into the template.php will not be lost.

For example, you may wish to customize the way the "submitted by" information appears on
each node. After enabling this setting and creating the "bella.custom.inc" so that it now
lives at sites/all/themes/bella/bella.custom.inc (assuming this theme itself lives at
sites/all/themes/bella), you could then place the following code inside your new
bella.custom.inc file:

<?php
function bella_preprocess_node(&$vars) {
  $vars['submitted'] = $vars['date'] . ' &hearts; ' . $vars['name'];
}

... which would result in the "submitted by" area being displayed as the node published
date and the node author's name, separated by a heart symbol.

NOTE: remember to clear the cache after making any changes such as this.

** Scan for custom stylesheets: If this box is checked, your site will scan the directory
where the theme is located for customized stylesheets; e.g. if you installed the theme by
placing it inside the sites/all/themes directory, then the directory at
sites/all/themes/bella will be scanned for custom stylesheets in the following order:

1) First, the system will check for a stylesheet called custom-style.css (so if your theme
is located at sites/all/themes/bella then the system will look for the file
sites/all/themes/bella/custom-style.css). If it finds this file, the file will be added to
every site of your Drupal installation that uses this theme. Anything found here should be
able to override anything found inside the theme's default stylesheet.

Example: custom-style.css

******

  If your custom breadcrumb separator is set to <span class="custom-breadcrumb"></span>,
  inside the sites/all/themes/bella/custom-style.css file would be a good place to put the
  CSS code for the class "my-custom-breadcrumb-class". For instance, say you wanted to use
  an image of your own named breadcrumb-image.png as the separator. One possibility would
  be to upload breadcrumb-image.png into the sites/all/themes/bella directory, then inside
  your newly-created custom-style.css file add the following code:

.my-custom-breadcrumb-class {
  background: url("breadcrumb-image.png") no-repeat scroll center center transparent;
  display: inline-block;
  width: 20px;
}
******

2) Next, the system will check for stylesheets based on domain name alone. Assuming your
domain name is www.yoursite.com, if there is a file named
custom-style-www.yoursite.com.css" inside this theme's directory, the system will in turn
add that file (stylesheet) ONLY when the user visits www.yoursite.com (so if they visit
www.yoursite.org or subdomain.yoursite.com, the file custom-style-www.yoursite.com.css
will NOT appear). Anything found here should be able to override what may have been found
inside the "custom-style.css" file as well as anything inside the theme's default
stylesheet.

Example: custom-style-www.yoursite.com.css

3) Regardless of whether the above file(s) were found or not, the system will now check
for more customized files so that site admins can add custom stylesheets based on other
criteria, the first of which is based on media type alone. Each of these files will be
able to override what may have been found so far in any of the stylesheets above and/or
anything inside the theme's default stylesheet. Possible media types are: all, aural,
braille, handheld, projection, print, screen, tty, and tv.

Example: custom-style-handheld.css

4) Now the system will check for stylesheets based on a combination of the site domain as
well as the media type. Each of these files will be able to further override what may have
been found so far in any of the stylesheets above and/or anything inside the theme's
default stylesheet. And regardless of whether those file(s) were found or not, the system
will now check for even deeper customization files so that each site in a multi-site
installation can use its own specific file(s).

Example: custom-style-handheld-www.yoursite.com.css

5) At this point, the system will check for stylesheets based on language. To add a RTL
stylesheet for all sites, simply create a file called custom-style-rtl.css and place it
inside this theme's directory. To add site-specific stylesheets, use the same format as
can be found above in #4, except append "-rtl" to the filename (before the file extension).

Example: custom-style-handheld-www.yoursite.com-rtl.css

... where "handheld" is replaceable by the media type, and "www.yoursite.com" is replaced by
the specific site's domain name.

6) Finally the system will check for custom stylesheets that are to be used to cater to
users who still use Internet Explorer, aka IE. Custom stylesheets for IE will be scanned 
for in the following order, based first on browser version alone (8, 7, or 6) followed by
a combination of the browser version and the current user's language direction:

custom-style-ie8.css
custom-style-ie7.css
custom-style-ie6.css
custom-style-ie8-ltr.css
custom-style-ie7-ltr.css
custom-style-ie6-ltr.css
custom-style-ie8-rtl.css
custom-style-ie7-rtl.css
custom-style-ie6-rtl.css

**NOTE: If CSS/JS aggregation is enabled, all the custom css files mentioned above will be
included aggregation process, with the exception of the custom IE stylesheets. This is
standard practice but since most people probably don't care, I won't go into any detail
here about why this needs to happen. However, feel free to browse the contents of the
page found at http://support.microsoft.com/kb/262161 where Microsoft itself explains how
inconvenient its own browser (IE) is for a good portion of the world's population.

Here is an overview of the possible CSS files that will be scanned for, in order, assuming
1) that the domain name of your web site is www.yoursite.com, and
2) that your theme is installed at sites/all/themes/bella

http://www.yoursite.com/sites/all/themes/bella/custom-style.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-www.example.com.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-all.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-aural.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-braille.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-handheld.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-projection.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-print.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-screen.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-tty.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-tv.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-all-www.yoursite.com.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-aural-www.yoursite.com.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-braille-www.yoursite.com.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-handheld-www.yoursite.com.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-projection-www.yoursite.com.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-print-www.yoursite.com.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-screen-www.yoursite.com.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-tty-www.yoursite.com.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-tv-www.yoursite.com.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-rtl.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-all-www.yoursite.com-rtl.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-aural-www.yoursite.com-rtl.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-braille-www.yoursite.com-rtl.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-handheld-www.yoursite.com-rtl.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-projection-www.yoursite.com-rtl.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-print-www.yoursite.com-rtl.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-screen-www.yoursite.com-rtl.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-tty-www.yoursite.com-rtl.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-tv-www.yoursite.com-rtl.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-ie8.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-ie7.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-ie6.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-ie8-ltr.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-ie7-ltr.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-ie6-ltr.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-ie8-rtl.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-ie7-rtl.css
http://www.yoursite.com/sites/all/themes/bella/custom-style-ie6-rtl.css

Please note that if your site has only a left-to-right language installed, none of the CSS
files listed ending in "-rtl.css" will be scanned for.

Also note that the default stylesheet at
http://www.yoursite.com/sites/all/themes/bella/css/style.css will always be the first file
scanned for (in addition to and preceding those listed above). This way, any custom
stylesheets found will be able to override what's already set inside the default stylesheet.
------------------------------------------------------------------------------------------
* Footer message

** Automatic footer message: It's possible that you might want your site to create its own
footer message so as to avoid the need to update the copyright date each year, etc. If
that's the case, and if this box is checked, your site will display a footer message
automatically as shown on the theme settings page.
------------------------------------------------------------------------------------------
* Feed icons 

** Display feed icons: If not checked, feed icons that normally appear at the bottom of
standard pages will be hidden.
------------------------------------------------------------------------------------------
* Search box

** Display search box: If this box is checked, the search box will be displayed in the
upper-right corner of the page to anyone who has permission to search, but only if no
blocks have been placed inside the "Header Top Right" region.
------------------------------------------------------------------------------------------


-- CONTACT --
Current maintainers:
  * Jeremy Trojan (jerdiggity) - http://drupal.org/user/334965

:)
