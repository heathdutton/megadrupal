; $Id: README.txt,v 1.1.2.1 2010/09/10 18:17:14 steve918 Exp $

Confident CAPTCHA module for Drupal
====================

The Confident CAPTCHA module uses the Confident Technologies service to stop
spammers without burdening your users. For more information
and to sign-up for an account go to:

    http://login.confidenttechnologies.com

INSTALLATION
------------

0. Confident CAPTCHA depends on the CAPTCHA module, you will
   need to install it before hand.  For more information about
   the Drupal CAPTCHA system see:
       http://drupal.org/project/captcha
       
   Confident CAPTCHA is based on the Confident PHP Libraries
   and includes a modified version (to account for some http_build_query
   Drupal oddities) primary development of the libraries is on github:
        http://github.com/ConfidentTech/confidentcaptcha-php


1. Extract the Confident CAPTCHA module to your
   modules directory (sites/all/modules).
   

CONFIGURATION
-------------
   
1. Enable Confident CAPTCHA and CAPTCHA modules in:
       admin/build/modules
   
2. You'll now find a Confident CAPTCHA tab in the CAPTCHA
   administration page available at:
       admin/user/captcha/confidentcaptcha

6. Register for a Confident CAPTCHA account:
       http://login.confidenttechnologies.com

7. Input your Customer ID, Site ID, plus your API username and
   password into the Confident CAPTCHA settings. The rest of
   the settings should be fine as their defaults.

8. Visit the Captcha administration page and set where you
   want the Confident CAPTCHA form to be presented:
       admin/user/captcha

KNOWN ISSUES
____________

1. If the Captcha is set to flyout or lightbox mode, you may have to do some custom css to insure
compatibility with your chosen theme.  The module does do some automatic checking to make sure that
it's contents won't be hidden by a parent with a css property of overflow: hidden.

CHANGELOG
---------
08-31-2010 - v1.2.0 - Incorporates modal captcha type and migrated to library version 20100813_PHP_1.2

THANKS
---------
Huge thanks to the writers of the reCAPTCHA and VidoopCAPTCHA module that 
served as the base for the Confident CAPTCHA module.
