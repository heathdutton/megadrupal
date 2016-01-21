Mobile Navigation module


Experimental Project

This is a sandbox project, which contains experimental code for developer 
use only.
Visit the Version control tab for full directions.

About
-----
Mobile Navigation helps you easily implement a nice solution for displaying 
menus on the mobile version of a responsive website.
I have found many ways of doing this out there, but they are not precisely 
the best solution regarding performance or they need the work of implementing
a third party JQuery plugin or having to learn javascript and programming the
whole thing.
With this module you may only need to install the module as usual and then 
out of the box have all that is needed to implement a mobile version for menus,
featuring some simple configurations that will let you personalize the behavior
of this mobile versions.

Installation instructions
-------------------------
1. Download and extract the Mobile Navigation Module into the modules directory.
2. Go to /admin/config/user-interface/mobile_navigation. Configure.

Currently Mobile Navigation only gets along with menus that uses tag < ul > for
the menu and < li > for menu items.
Mobile Navigation module uses some Javascript to clone the current menu of your
website and creates a version of it with a diferent behavior adapted to mobile.

At the Mobile Navigation configuration page you can administer Mobile 
Navigation settings.
You must set two parameters for the transformation to take place:

    A Media Query for the breakpoint. this media query must reflect the range 
    in which you don't want the mobile version to show up, that is the not 
    Mobile Range of your website. The reason for this is that Media Queries 
    are pretty complex stuff and usually responsive themes don't have mobile 
    media query, they have media queries for the NOT MOBILE displays, so you 
    can easily copy this media query from your preferred theme configuration,
     being Omega, Adaptative, any responsive theme.)
    The other parameter that must be defined is the Menu Element Selector. 
    By default this value is "#main-menu-links". That's the selector for the
     menu the Bartik theme has by default(Bartik is the default Drupal 7 theme).
    When using other menu configurations or modules(Superfish, Megamenu, any 
    module you like or the Drupal main menu), you'll have to check for the 
    corret selector, that might be #supefish-1, #megamenu-menu, 
    #navigation .menu, etc.

Additionaly there is the Mobile navigation behaviors configuration:

    Select a plugin for the menu. Currently suported plugins: Basic: Simply 
    slide showing the menu, without any special behavior on its contents.
    Accordion: Show menu and its submenus in a organized accordion structure. 
    (Applies only when the menu includes submenus.
    For the accordion plugin, you can set persistance of opened items so you 
    can have many submenus opened, or you may permit only current active 
    trail to be oppened, so when you open a submenu, any other opened submenu
    will automaticaly close.
    There is an option on the way the menu shows up, the location and the 
    direction.
    finally there is the option to have a tab handler attached to the menu.

New features on the way.

    Multiple menus with diferent configurations.
    More plugins.
    a lot more to come...

Enjoy!
