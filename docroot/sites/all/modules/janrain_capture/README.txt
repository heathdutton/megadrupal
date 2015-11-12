INTRODUCTION
------------

This module accelerates the time to implement Janrain Capture into your Drupal web sites, helping you to improve your registration conversion rates by allowing your customers to register and sign-in through their Social Network of choice.

Janrain Capture is a hosted registration and authentication system that allows site owners to provide a central repository for user information, that can be deployed on one or more web sites. Registration and authentication can be done through connection to a social network identity provider such as Facebook, Google, Yahoo!, OpenID, LinkedIn, eBay, Twitter and many others or through traditional form field methods.

More information can be found at:
http://www.janrain.com/products/capture

Documentation for Janrain Capture can be found at:
http://www.janraincapture.com/docs



INSTALLATION
------------

1) This module relies on the Fancybox jQuery library version 1.3.4 to be installed at:
sites/all/libraries/fancybox/jquery.fancybox-1.3.4.pack.js

Download and unzip:
http://fancybox.googlecode.com/files/jquery.fancybox-1.3.4.zip

Move jquery.fancybox-1.3.4/fancybox to sites/all/libraries/

You also have the option of building out your own UI with your preferred iframe
tools as well.

2) Upload the janrain_capture directory to your site-specific modules directory
or to sites/all/modules

3) Enable the module(s) via the Modules screen in the admin backend

4) Visit the Janrain Capture section of your Site configuration and set your
Janrain Capture client information.

5) Include a link somewhere in your page templates to authenticate. If using
the janrain_capture_ui module you can use the PHP function
janrain_capture_url()  to construct the href and should have the classes
'fancy' and 'iframe' in order for the fancybox initialization to bind
correctly.
Example:
<a href="<?php echo janrain_capture_url(); ?>" class="iframe fancy">Log in</a>

There is also a Janrain Capture block with Login / Logout and Edit Profile
links but is generally meant for testing and may not be suitable for production
use.
