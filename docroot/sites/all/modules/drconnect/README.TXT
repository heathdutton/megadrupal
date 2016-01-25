-- SUMMARY --

This module gives alternative way to sign-in to your site:
- It lets you use your Draugiem.lv ID and password to sign-in
to your Drupal site.
- In case the user already holds an account on the website the user can
choose to link his drupal account to his Draugiem.lv identity.

Facebook Connect module (http://drupal.org/project/fbconnect) was used as
source for my module, but lots of stuff was removed and edited. 

WARNING! For module purposes popup window is generated. I used simple
JavaScript code to close that window. I presume, it will close all other
popup windows aswell.


-- DRAUGIEM.LV PASSPORT TAB --

This module adds a new Draugiem.lv Passport tab in the user's profile page.
This tab allows to :
- Remove existing link between Draugiem.lv account and your
Drupal site account.
- Create new link between Draugiem.lv account and your Drupal site account.

-- REQUIREMENTS --
PHP 5.2 or higher versions. 
Drupal 7.x. 
Draugiem PHP Library: http://www.draugiem.lv/development/examples.zip
Draugiem API key: http://www.draugiem.lv/applications


-- INSTALLATION --

1. Upload the 'drconnect' folder into your module directory;	
2. Download the Draugiem PHP libraries from
http://www.draugiem.lv/development/examples.zip;
3. Copy the contents of "examples" folder to
sites/all/libraries/draugiem-api directory;	    
4. Create a new Draugiem.lv application:
http://www.draugiem.lv/applications/dev/?create=4 (more information here: 
http://www.draugiem.lv/applications/dev/passport/); Enter application name,
base path of your site that will be using this application
and application description;
5. Activate the module through your website's modules administration pages or
by using drush (drush en drconnect);
6. Open module settings form (admin/settings/drconnect);
7. Enter Draugiem.lv application ID and API key;
8. Activate "Draugiem.lv Passport" block from the admin/structure/block menu.

-- TROUBLESHOOTING --

If you experience about 60 sec timeout when trying to login to your Drupal site
and then get an empty response, Draugiem Passport probably doesn't
like your network setup.
We have a complicated development setup where incoming and outgoing connections
have different IP addresses. Draugiem Passport didn't like that very much.
If you have conventional setup, you probably won't have this issue.

If authentication doesn't work at all, try changing this in DraugiemApi.php:
From: http_build_query($params)
To: http_build_query($params, '', '&')

-- POSSIBLE IMPROVEMENTS --

Draugiem.pase also provides additional features, which can be used to
improve this module:
- You can get basic information about Draugiem.pase user
(name, surname, nickname, place of living, age, profile image, sex).
- You can use user's Draugiem.lv profile picture for your site's user account.
- You can get Draugiem.pase user's friends, who use this application and make
that users can see which of their Draugiem.lv friends had an account
on your site.
- You can get information about all Draugiem.pase users.
- You can add new record to user Draugiem.lv profile activities.
- You can add new notification to user Draugiem.lv profile news.
- Implement normal popup window control.

-- KNOWN ISSUES --
 
Module will cause PHP error if configuration is empty (like after
reinstalling module), but you have Draugiem Pase block somewhere on page.

If e-mail validation is disabled in user settings, Draugiem Pase linking will
not work. It works however if you remove drupal_external_login call, not sure
if it's needed. Should test if it does or doesn't work on Fbconnect.

Account linking links should have a proper theming function.

Not compatible with non-clean URLs.

When admin opens other users account tab "Draugiem Pase", he will see his
(admins) not users information.


-- DEVELOPERS --

Based on Facebook Connect module.
Module created by Mārtiņš Zaķis.
Testing by Miķelis Zaļais, mikelis@mearra.com.
Additional testing by Artis Krūmiņš.

Development at Mearra Latvia (mearra.com). 


	