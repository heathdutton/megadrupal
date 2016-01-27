********************************************************************
                     D R U P A L  -  A U T H Y
********************************************************************
Name: Authy
Author: Audun Larsen <larsen at xqus dot com>
Drupal: 7
********************************************************************

DESCRIPTION:

Secure your Drupal site with two-factor authentication provided by
Authy. 

With the Authy module installed on your website your users can use
their smart phone as a one-time-password generator, greatly
improving the security of your site.

You can request an one time password from your users not only when
they sign in, but also when they are submitting any forms of your
choice.

********************************************************************
INSTALLATION:

1. Download and install the Authy module, and the Libraries module.

2. Download the authy-php library from:
   https://github.com/authy/authy-php/tarball/master
   Upload this to the libraries directory (normally
   sites/all/libraries/authy-php).
   
3. Create a new Application at www.authy.com, and copy the API key.

4. Enable the Authy module, and navigate to the Authy module
   configuration page (/admin/config/people/authy).
   
5. Enter the API key from step 3, and the same application name 
   that you used when you created the application at www.authy.com.

6. Create a test account and enable Authy on it by navigating to the
   user page and clicking the Authy tab.
   
********************************************************************
DESIGN CHOICES:

* All users with Authy enabled will be prompted for a Authy token on
  login.
  
* You can add forms you wish to be protected with Authy tokens on
  the configuration page. 

********************************************************************
CONTACT:

* Audun Larsen (xqus) - https://drupal.org/user/8737/
