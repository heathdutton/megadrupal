This project comprises a very simple theme.
It's designed to accompany a particular use-case scenario.

The Portal theme is designed for websites which:
* Are members only or...
* Are running CiviCRM or...
* Which want a site-theme independant login page.

The ideal use-case scenario is as follows:

    Install: Redirect 403 to User Login from:
      * http://drupal.org/project/r4032login
    Install: ThemeKey from: 
		  * http://drupal.org/project/themekey
    Install: Login Destination from:
		  * http://drupal.org/project/login_destination
    Install: Front Page from:
		  * http://drupal.org/project/front_page 

Then install the theme and setup as follows:

    Setup r4032login to redirect Access Denied errors back to the login.
		
    Setup an access control module to prevent 
		'anonymous' users from accessing any content.
		
    Setup ThemeKey to use the Portal theme for 
		'user/login', 'user/register' and 'user/password'.
		
    Setup Login Destination to send users to your 
		'home' page after login.
		
    Setup user/login as the front page for anonymous users.
		
		Setup your usual home page as front for other users as required.

Any questions or comments are welcome in the
issue queue along with bug reports and feature requests.

The theme has settings for:
* Logo display (on/off)
* Title display (on/off) 
* Civicrm logo/powered by text (on/off).
