
CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Requirements
 * Terminology
 * Installation
 * Configuration
 * Usage
 * Security
 * Limitations
 * Contact

INTRODUCTION
------------
Site Memos is a very simple administration module that encourages you to
record information about your site in a permanent record that can be easily
accessed and edited by you, your team, and others who may follow you.

Site Memos does not create any memos - you have to do that yourself. It also
does not impose any structure on the memos you create. It is really just a
flag that you and your team can always find to allow you to maintain your own
memos.

Memos you may want to record could include:
 - Procedures
 - Warnings
 - Rationales about module and theme choices and configurations
 - Plans, schedules, and To-dos
 - External info - servers, Varnish, CDNs, etc.
 - Contact info
 - Recognition for key people

Site Memos is easy to install and configure.

It is used by administrators or developers, not site visitors.


REQUIREMENTS
------------
Site Memos has no special requirements.


TERMINOLOGY
-----------
In this README, we use uppercase 'Site Memos' to refer to the module, and
lowercase 'site memos' to refer to the page(s) of information that you create.


INSTALLATION
------------
Install as usual, for further information see
http://drupal.org/documentation/install/modules-themes/modules-7


CONFIGURATION
-------------
1. Create a site memos page to hold your memos. A Drupal 'Basic Page' might
   work well, or you could use some other content type. You can use an
   external page such as a Google Docs document if you like.

 - Make a note of the URL of the page, including 'HTTP'.

 - If you have chosen a page on your own site to hold your site memos, you
   probably want to leave the page 'Not published', but that is up to you.
   If you want your site visitors to view the information, go ahead and
   publish it and add it to a menu.

2. Go to the Site Memos configuration page at admin/config/system/site_memos.
   Enter the full URL of your site memos page in the 'Site Memos URL' field
   in the form and submit it.

You might want to set up multiple pages to hold various types of memos. Site
Memos doesn't have explicit support for this. However, you could accomplish
this easily by making the page specified in Site Memos an index with links to
any other pages you want.


PERMISSION
----------
The 'view site reports' permission is used to control access to the Site Memos
configuration page. This is 'on' for administrators in a default Drupal 7
installation.

Access to your site memos page is controlled by you based on the content type
you have chosen.


USAGE
-----
Keep your site memos page up to date with any information you want to keep.

You can always find the link to your site memos page at
admin/config/system/site_memos.

You can change the URL of your site memos page at any time. Just go to the
Site Memos configuration page and change the URL. Be sure to save any
information that already exists at the old URL.


SECURITY
--------
If you put sensitive information in your site memos page, be sure to leave
the page unpublished. If you need more security than that, create a separate
content type for your site memos page, and restrict access to that content
type to only those you want to see it.


LIMITATIONS
-----------

Site Memos makes no effort to verify the validity of the URL you enter in the
Site Memos configuration page.

If your site memos page is a Drupal node, Site Memos will attempt to display
the title and latest change date when you visit the Site Memos configuration
page. Otherwise, it will just show the URL that you provided.

The value of the information in your site memos page depends completely on
your diligence in keeping it complete and up-to-date.


CONTACT
-------
Current maintainers:
  Joe Casey (Joe Casey) - http://drupal.org/user/823758
