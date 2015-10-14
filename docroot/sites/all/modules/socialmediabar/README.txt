SocialMediaBar is a sharing bar that allows you to add a sharing bar onto your
website, that will not only pin itself into a specific place in your content
but then also float to the side as the user scrolls. It uses the native sharing
systems of the different social media networks to ensure proper sharing, and is
extremely lightweight on the frontend of your site.

It is also extremely developer and version control friendly by keeping very few
configuration options, so it is very easy to add to a site and get running as
well as move between sites.

Installation and Setup
======================

1. Install like any other module[1]
2. Add the following line to the template files you want the bar to appear on:

    <?php echo socialmediabar_render(); ?>

3. Go to the admin page[2] and enable the networks you want!

[1] http://drupal.org/documentation/install/modules-themes/modules-7
[2] /admin/config/services/socialmediabar

Requirements
============

PHP 5.3+
--------

This modules requires PHP 5.3 or higher, as it uses namespaces.

cURL
----

This module requires cURL to function properly at this time. If your
installation does not have cURL enabled, the admin page will give you an alert
letting you know. SocialMediaBar will fail gracefully so as to not break your
site if you happen to be on an installation without cURL.

ShareThis APi Key for E-mail Sharing
------------------------------------

To use the e-mail sharing, you will need to get an API key for ShareThis's API.
You can get a developer account and at:

  http://developer.sharethis.com/


Supported Networks
==================

SocialMediaBar currently supports the following share networks:

    * LinkedIn
    * Twitter
    * Facebook
    * Google+
    * E-mail Sharing via ShareThis
