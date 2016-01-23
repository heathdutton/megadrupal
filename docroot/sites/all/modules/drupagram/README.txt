Introduction
============
Drupagram module allows listing Instagram pictures in blocks or pages. Its
integration with Views opens the door to all sorts of formatting (ie. as an
automatic slideshow with views_slideshow). It also provides useful input filters
to easily link Instagram accounts and searches within text.

Drupagram's submodules allow posting to Instagram, executing actions/rules when
posting or login with an Instagram account.

OAuth
=====
OAuth module is required to authenticate against Instagram.

When you download the OAuth module, get the latest stable release available at
http://drupal.org/project/oauth

Once OAuth has been enabled, go to admin/config/services/instagram and follow
the instructions.

How to add an Instagram account to a Drupal user account
========================================================
See INSTALL.txt for details.

How to use the Instagram username input filter
==============================================
1. Go to admin/config/content/formats.
2. Select the text format where you want to use the filters.
3. At "Enabled filters" check the Instagram converters.

These filters are avilable when configuring Instagram Views.

How to post to Instagram
========================
Instagram's API does not allow posting pictures. Do add items to your feed, you
need to use their app on your iPhone.