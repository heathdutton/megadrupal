google_authorship
=================

Maintained by: Gabriel Sullice <gabriel@sullice.com>
                 Drupal.org: gabesullice [https://drupal.org/user/2287430]

A simple module which adds Google authorship
[https://support.google.com/webmasters/answer/1408986?hl=en]information to node
pages. It does this by changing the submitted by link to the node author to
their Google+ profile. This associates the content with the user and will
generally result in the user's profile picture being displayed in Google's
search results, hopefully leading to higher click-through rates.

Sample Search Result [https://www.google.com/search?q=google+authorship].

Features
--------

- Adds Google authorship information to node pages
- Boosts the credibility of your site when potential visitors see your site
    listed in search results
- Supports tokens (Special thanks to DamienMcKenna)
- Works with Metadata module, if enabled (Special thanks to DamienMcKenna)
- Upcoming
    - Specify which content types to display authorship information
    - Optionally retain link to user profiles and include authorship information
        in non-visible fields
    - Maintainer is open to new feature requests

How It Works
------------

This module adds a field to the Drupal 7 user entity called to store a Google+
profile ID. Using that field, if it is filled, this module alters the
Submitted by line of a node's full content display to link to the Google+
profile of the appropriate author. Finally, by setting the "rel='author'"
attribute Google search bots will recognize and display an author's profile
picture in its search results.

Installation
------------

Installation

1.  Add the module to /sites/all/modules (or /sites/all/modules/contrib).
2.  Go to admin/modules and enable the Google Authorship module.
3.  For each user for whom you would like Google to display author information,
    add their 21 digit Google+ Profile ID from their profile URL. You may also
    have users do this themselves.
4.  Ensure that the "Display Author and Date" setting is checked for all of the
    content types that you would like to have Google Authorship apply its
    changes.
5.  Each user will need to have their Google+ profile page link back to your
    domain. If they have an email at your domain on their profile, they simply
    need to input their email address in here
    [https://plus.google.com/authorship] once. If not, they should follow
    the following instructions:
    1.  Edit the Contributor To [http://plus.google.com/me/about/edit/co]
        section of their profiles (under Profile>About>Links).
    2.  In the dialog that appears, click Add custom link, and then enter the
        website URL.
    3.  If you want, click the drop-down list to specify who can see the link.
        Click Save.
6.  To verify that Google can see authorship information on your nodes, you can
    paste the URL of a node which this module should have overridden into
    Google's Structured Data Tool
    [http://www.google.com/webmasters/tools/richsnippets].

Important Notes
---------------

This module may not work with themes that override the submitted link too. If it
does not seem to be working, please first check that your theme does not
override it on its own.

Similar Projects
----------------

The Submitted By module provides similar functionality to Google Authorship.
However, Google Authorship chooses to address one specific use case and requires
much less configuration to get set up, i.e., one need not set up any tokens or
any of the additional fields the user account would require. In addition, Google
Authorship does not create a new field to be added to content types and instead
works with and overrides Drupal's default "submitted by" feature and has no
dependencies.

While Google Authorship provides less customization options, it is a simpler and
easier route for those wishing to simply add Google authorship information to
their sites.
