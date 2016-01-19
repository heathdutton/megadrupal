## Overview ##
All New York State websites are required to have the Global Navigation
(sometimes called Agency Navigation) bar at the top and bottom of the site
surrounding any other
content.  This module makes it easy to integrate them on a Drupal site
using the Drupal menu system.

## Requirements ##
To use this module, you muse have the following installed on the site:

- [Libraries](https://www.drupal.org/project/libraries) module, installed and
enabled.
- [NYS Global Navigation repository](https://github.com/ny/global-navigation)
(for CSS, JS and fonts).
[Download .zip version](https://github.com/ny/global-navigation/archive/master.zip).
Note: to avoid a 404 error, log into GitHub before accessing these links
(you must also have been granted access, see restrictions).

## Recommended Modules ##
These modules are recommended when using this module:

- [Special Menu Items](https://www.drupal.org/project/special_menu_items)
to allow adding menu items that don't have links.  User the <nolink\>
URL option.
- [NYS Universal Navigation](https://www.drupal.org/project/nys_unav),
a companion module that automatically inserts the required Universal
Navigation header and footer outside the Global Navigation header/footer.

## Restrictions ##
This Drupal module was developed for use by New York State agencies and entities
for official New York State websites to be compliant per ITS mandate policy
NYS-S05-001:

[https://www.its.ny.gov/document/state-common-web-banner](https://www.its.ny.gov/document/state-common-web-banner)

For use on other sites, please contact the New York State Office of
Information Services WebNY team at webnysupport@its.ny.gov for guidance and
authorization for use. 

The CSS, JavaScript and fonts that make up the library
used for this module can be found at:

  [https://github.com/ny/global-navigation](https://github.com/ny/global-navigation)

Because that repository contains fonts that are licensed only for use on
New York State websites, it is not public.  If you need access to the
repository, contact the WebNY team.

## Installation ##
To install the module see

[http://drupal.org/node/70151](http://drupal.org/node/70151)

- Download the Global Navigation library from 

    [https://github.com/ny/global-navigation](https://github.com/ny/global-navigation)

    Rename the directory from global-navigation-master to nys\_global\_nav
and copy the renamed directory into your /sites/all/libraries directory.

    When the library is installed correctly if you have the IMPLEMENTATION.md file
is at sites/all/libraries/nys\_global\_nav/IMPLEMENTATION.md.

    This repository/library contains fonts that are licensed only for use on New York
State websites, therefore it is not public.  Contact the WebNY team at
webnysupport@its.ny.gov for access to the repository. 
- Enable the module.

- Go to Configuration >> User Interface >> NYS Global Navigation
(/admin/config/user-interface/nys-global-nav) to configure the module.
*Note that if you do not configure the module, only the "hamburger" menu
will be visible.*

- It is **strongly** recommended that you install and enable the module
  [Special menu items](https://www.drupal.org/project/special_menu_items) at<br/>
https://www.drupal.org/project/special\_menu\_items<br/>
to facilitate creating menus that are compatible with the Global Navigation
"click to open" sub-menu items.

- **If you are using the Special Menu Items module to create <nolink\> menu
entries,** go to Configuration >> System >> Special menu items
(/admin/config/system/special\_menu_items) and use <a href="#"\> as the HTML
for "nolink" instead of the default <span\>. 
- To allow users to administer the configuration of this module, set the
*Administer NYS Global Navigation* permission for those roles at People >>
Permissions (/admin/people/permissions#module-nys\_global_nav) or 
the Permissions link on the module page.

- The module will automatically insert the Global Navigation header at the top
and the Global Navigation footer at the bottom of your website's page;
outside of any page HTML.  This is  preferred for the horizontal
agency navigation header format.  For the vertical agency navigation format, 
disable the automatic insertion of the Global Navigation header
and use the Global Navigation Header block to place the navigation.

## Configuration ##
The module can be configured by going to Configuration >> User Interface >>
NYS Global Navigation (/admin/config/user-interface/nys-global-nav) or
the Configure link on the module page.
*Note that if you do not configure the module, only the "hamburger" menu
will be visible.*

The configuration options available are:

![](https://www.drupal.org/files/NYS%20Global%20Navigation%20Configuration%20Collapsed-With%20Numbers.png)

1. **Agency name**:  This can be much longer than the text field would indicate,
although if it is too long it might not style correctly.  You can also
indicate where to break the name using the <br/\> HTML tag.  Any other HTML
tags will be filtered out during output.
1. **Agency grouping color**:  Select the agency grouping color per NYS Branding
Guidelines, which determines the color pallet used by the Global Navigation
header/footer.  The available options are:
	- Administration
	- Business
	- Education
	- Health and Human Services
	- Local and Regional authorities
	- Public Safety
	- Recreation and Environment
	- Statewide Elected Officials
	- Transportation and Utilities
1. **Global navigation header automatic insertion**:  This option is
checked/enabled by default.  Enabling this option will cause the Global
Navigation header to be automatically inserted into the hidden region Page
Top of the theme.
    - If this option is checked/enabled, the Global Navigation
Header block will be disabled
    - If this option is unchecked/disabled,
the block will be enabled and can be placed via the Blocks user interface.
1. **Agency navigation header format**:  This option selects the format of the
Global Navigation header.  There are three options.  The preferred option for
all new sites is Horizontal -- stacked.  Selecting Vertical can result in odd
default formatting if automatic insertion is enabled.  It is designed
to accommodate older sites with designs that can't accommodate the horizontal
format.  For the Vertical format, disable automatic insertion (option #3) and
use the Drupal block system to place the resulting menu.
The available options are:
	- Horizontal - stacked
	- Horizontal - unstacked
	- Vertical
1. **Global navigation header menu**:  This option allows you to choose which
menu to use in the Global Navigation header.  The list is populated
automatically from menus defined in Drupal. To use a custom menu
(recommended), define the menu first then select it from the list here.
Menus in the header should have between five and seven first level items.
If there are second level items, the first level item must have a path of
<nolink\> to support the click open/click closed functionality of the library.
1. **Global navigation footer automatic insertion**:  This option is
checked/enabled by default.  Enabling this option will cause the Global
Navigation footer to be automatically inserted into the hidden region Page
Bottom of the theme.
    - If this option is checked/enabled, the Global Navigation
Footer block will be disabled
    - If this option is unchecked/disabled,
the block will be enabled and can be placed via the Blocks user interface.
1. **Global navigation footer menu**:  This option allows you to choose which
menu to use in the Global Navigation footer.  The list is populated
automatically from menus defined in Drupal. To use a custom menu
(recommended), define the menu first then select it from the list here.
Menus in the footer should have between two and five first level items,
formatted as the column titles/headings. The first level items should have
multiple second level items and have a path of <nolink\> although having the
column heading being a link is supported.
1. **Social Media Options**:  This fieldset defaults closed given its length.
When you click the SOCIAL MEDIA OPTIONS, the link expands the fieldset and
exposes 19 social media URL fields.  If none of the fields has a URL,
then the social media section is not displayed in the footer.  When any of the
fields has a URL, then a horizontal rule with Connect with Us is displayed with
the appropriate social media icon(s).  There should be no more than six social
media icons defined.  When entering the URL, include the entire URL including
http:// or https://

![](https://www.drupal.org/files/NYS%20Global%20Navigation%20Configuration%20Expanded-cropped.png)

## Using the Global Navigation Module ##
Enabling the module on your site will, by default, insert the Global Navigation
at the top and the Global Navigation footer at the bottom of your website's
page, and outside of any site-generated HTML.  The Global Navigation will appear
inside the Universal Navigation header/footer if the Universal Navigation module
is installed and enabled.

Note: The Global Navigation will **not** display on any site administration
pages.

If the automatic insertion options are disabled on the  module configuration
page, the module adds two blocks and defines two functions.
Either blocks or theme functions can be used depending on your theme.
The option to use automatic insertion or blocks is independent for the header
and footer,  allowing a site builder flexibility in placement approach.

Enable this module after your other modules and it will adapt to
most module weighting situations.

If you have the NYS Universal Navigation implemented via blocks, and placed
through the user interface, do not use the automatic insertion
options of this module.  The aggressive placement of automatic
insertion will cause the Global Navigation to appear before the NYS Universal
Navigation.

The module only uses the first and second levels of any menu.  Menu items
below the first or second levels are ignored per the NYS Branding Guidelines.
Any disabled menu items will also be ignored.

To support the mobile user experience, top level navigation items in the header
that have second level items cannot have valid links associated with them.
When a visitor clicks on a top level nav item which contains subnav items,
it needs to display the sub navigation.  But if you enter a valid link URL
for a top level nav item it will navigate to that link instead of
opening the sub navigation.

We recommend you create two new menus, Structure >> Menus >> Add menu
(admin/structure/menu/add):  Global Navigation Header Menu and
Global Navigation Footer Menu.  Next, add the links appropriate for each.
Top level menu items that have second level items should have their
path set to <nolink\> (assuming you are using the suggested Special Menu Items
module).
Use the Drupal menu drag and drop interface to organize your menus.

A Global Header Menu example that looks like:

 ![](https://www.drupal.org/files/Global%20Header%20Example%20Menu.png)

Will display:

![](https://www.drupal.org/files/Global%20Header%20Example.png)

A Global Footer Menu example that looks like:

![](https://www.drupal.org/files/Global%20Footer%20Example%20Menu.png)

Will display:

![](https://www.drupal.org/files/Global%20Footer%20Example.png)

The module uses the nys/global-navigation GitHub
repository, treated as a Drupal library, for its CSS and JS which is continually
being developed and updated.  This module will be updated to specify a minimum
revision of the library to use on your site.  Check your site's Status
report and to see the version of the Global Navigation library.  If your copy
of the library is out of date, you will be directed to update the library
manually (libraries aren't managed via the Drupal update process).

## Using the Module Without Automatic Insertion##

If you find the automatic option doesn’t work on your particular site consider the following:

- your site design necessitates using the vertical format of the menu
- you want to use the additional flexibility offered by blocks

If using blocks via your UI, or functions (which you can use in a template),
you can disable the automatic addition of either the header and footer via
either the UI or with the addition to your site's settings.php file.

In your site's settings.php file, about line 330, is a block of comments that
starts with "Variable overrides:".  After the block comment, there are several
lines that are commented out with a # character of the form

  `$conf['site_name'] = 'My Drupal site';`.  

After the commented out lines, add the line(s):

    $conf['nys_global_nav_header_auto'] = FALSE;
    $conf['nys_global_nav_footer_auto'] = FALSE;

A gentle reminder -- make sure you have the semicolon (;) at the end of the
line, otherwise your site might not load.

Another use for this feature would be to disable the Global Navigation on
your development environment by putting the setting(s) above in your
local.settings.php file or other environment-specific configuration area
(some sites keep that configuration in the settings.php inside conditional
statements).

Once you have disabled the automatic insertion of the Global Navigation,
you have two options:  use blocks in your theme or functions in your theme
templates.
 
Note: If automatic insertion is enabled, the corresponding block option
is disabled.  However, the function to insert the header or footer via a
function call from a template is always available.

If your theme has header and footer regions,they may not be named header and
footer, that are full page width, and don't have anything above
the header or below the footer, enable the module and use block placement method
(the structure >> blocks page in Drupal or using context, panels, etc if
you are using one of those contributed modules.).

If your theme doesn't have appropriate regions, it is possible to add new
regions in your theme, using the information
at [https://www.drupal.org/node/171224](https://www.drupal.org/node/171224)
or following the tutorial at
[https://www.ostraining.com/blog/drupal/block-region-drupal-theme/](https://www.ostraining.com/blog/drupal/block-region-drupal-theme/),
then place the blocks as described.  Adding new regions will require
modifying your page.tpl.php file, remember to modify custom page templates
(e.g. page--front.tpl.php, etc).

Alternatively, you can modify your theme template(s) to insert the Global
Navigation HTML during output. The easiest template to modify is the core system
html.tlp.php file which is used to wrap the page with the html doctype, output
the styles and scripts and add the accessibility skip link.

You can make a single change to that template.  Remember **Don't hack core**.
If your theme, or base theme if using a subtheme, does not have
the html.tpl.php file in it's template directory, you can copy the default
Drupal html.tpl.php from the core /modules/system directory into your theme's
template directory. If your theme doesn't have a template directory,
create one at the same level as the theme .info file).  *Note: Clear
caches after copying the template file to cause Drupal to rebuild the theme
registry and "discover" the new file.*

In the theme html.tpl.php file add the following code right after the
block comment documenting the variables and before the HTML that starts
<!DOCTYPE....

    <?php
    if (module_exists('nys_global_nav')) {
      $page_top = nys_global_nav_header() . $page_top;
      $page_bottom = $page_bottom . nys_global_nav_footer();
    }
    ?>

When this code is inside a template, use both the <?php and ?> closing tags.
The order of the variables in the assignment statements are important.  The
first ensures the Global Navigation header is before any module generated
HTML, the second ensures the Global Navigation footer is after any module
generated HTML. If you are using the Universal Navigation module with automatic
insertion, it is recommended you use automatic insertion for Global Navigation
as well.

## Additional Notes##

For many sites, the Global Navigation header won't have any z-index issues, but
we have seen problems with certain themes, such as sites based on Omega, where
the theme seems to set the content container to a z-index of 11.  If the pull
down menus appear behind other content, you can set a z-index on the Global
header container in your site's theme CSS. Several Drupal functions and modules
use z-index to appear on top of the site, such as administrative overlay (500)
and admin menu module (999), avoid placing the header on top of them.  The
following is an example of how to set the z-index CSS in your theme:  

	/*
	 * Global Nav z-index
	 */
	
	.nys-global-header{
	    z-index: 100;
	}


If you are having trouble seeing the Social Media icons, add the following lines
to your .htaccess file. See the Apache documentation for more details on this:<br/>
[http://httpd.apache.org/docs/2.0/mod/mod_mime.html#addtype](http://httpd.apache.org/docs/2.0/mod/mod_mime.html#addtype)

	AddType image/svg+xml svg
	AddType image/svg+xml svgz 

## Credits ##
This Drupal module was sponsored by the New York State Office of Information
Technology Services WebNY department.  Development team included Justin 
Winter, Eric Steinborn, Adam Fasoldt, Jason Cortes and Gregg Marshall.

