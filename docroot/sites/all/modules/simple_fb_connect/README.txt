SIMPLE FACEBOOK CONNECT MODULE

Simple FB Connect is a Facebook Connect Integration for Drupal.

It adds to the site:
* A new url: /user/simple-fb-connect
* A tab on /user page pointing to the above url

HOW IT WORKS
------------
User can click on the "Register / Login with FB" tab on the user login page
You can add a button or link anywhere on the site that points 
to /user/simple-fb-connect. So theming and customizing the button or link 
is very flexible.

When the user opens the /user/simple-fb-connect link, it automatically takes 
user to Facebook and asks for his permission. Once granted the module checks 
the users email. If the email address is found on the Drupal Site, he is logged 
in automatically. Otherwise a new user account is created with the email address
and the user is logged in.

SETUP
-----
1. Create a Facebook App on Facebook
-  Create a new app on https://developers.facebook.com/apps
   This will give you an App ID/API Key and an App Secret
-  Edit the App settings on facebook to enable "Website with Facebook Login"
-  For Site URL, enter the url of your Drupal site

2. Download Facebook PHP SDK v4 and extract to sites/all/libraries/facebook-php-sdk-v4
-  Go to https://github.com/facebook/facebook-php-sdk-v4/releases/
-  Download the latest 4.0.x release (zip or tar.gz). Note that only 4.0.x is supported.
-  Extract the SDK to sites/all/libraries. Rename the extracted directory to facebook-php-sdk-v4 so
   that you have the following folder structure: sites/all/libraries/facebook-php-sdk-v4/src/Facebook
-  Example commands on Linux for downloading 4.0.23 version of the SDK, assuming that you are
   currently in the root of your Drupal site:

   cd sites/all/libraries
   wget https://github.com/facebook/facebook-php-sdk-v4/archive/4.0.23.tar.gz
   tar -xvf 4.0.23.tar.gz
   mv facebook-php-sdk-v4-4.0.23/ facebook-php-sdk-v4

3. Enable Simple FB Connect module and configure at
   admin/config/people/simple-fb-connect
-  Enter your app id and secret key obtained from facebook and save your settings

4. Check installation status at
   admin/reports/status
-  If you can see a SDK version number, you are ready for rock and roll!

5. Give it a try
-  Click on the "Register / Login with FB" tab on the user login page
-  Alternatively you can add a link anywhere on the site that points to /user/simple-fb-connect

HOW TO DEFINE POST LOGIN URL DYNAMICALLY
----------------------------------------
When user has successfully logged in with FB, the module redirects the user to the path
defined on module settings (e.g. <front> or user).

It is possible to override the post login path by using the ?destination url parameter.
In the following example, the user would be redirected to 'custom/path/here' after FB login.
http://example.com/user/simple-fb-connect?destination=custom/path/here

If you want to have a block on the side bar of every page and would like the user to be
returned to this same page after login, you can create a custom block (with PHP Filter module enabled)
as follows:

<?php
print '<a href="/user/simple-fb-connect?destination=' . current_path() . '">Login with Facebook</a>';
?>

FURTHER DOCUMENTATION
---------------------
Advanced instructions can be found at module documentation page:
https://www.drupal.org/node/2474731

SUPPORT REQUESTS
----------------
Before posting a support request, carefully read the instructions provided in this README file and
do the setup in this order. Once you have done this, you can post a support request at
https://www.drupal.org/project/issues/simple_fb_connect

When postig a support request:
-  Please start your support request with the following phrase:

   I have read the README file and I have installed the Facebook PHP SDK v4 to
   sites/all/libraries/facebook-php-sdk-v4

- Please inform what does the status report say at admin/reports/status

- Please inform if you see any log entries at admin/reports/dblog
