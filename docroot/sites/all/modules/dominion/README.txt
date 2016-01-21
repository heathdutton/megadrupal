=======
Porting to 7.x in progress
=======

This is the readme for 6.x
PREFACE
-------
Dominion is a subsite builder with a user-friendly backend, intended for
editors.

Dominion is build upon the domain module. Each subsite has a subdomain of the
main domain by default, but can also have its own domain.

INSTALLATION
------------
Download and install the domain module from http://drupal.org/project/domain.

Enable domain, domain_conf and dominion. You may also enable one of the extra
modules:

- dominion_alias
  Adds the possibility to add aliases to subdomains
- dominion_contact
  Provides a contactform per subsite, based on the webform module.
- dominion_faq
  Provides a FAQ per subsite, based on the faq module.
- dominion_forum
  Provides a forum per subsite, based on the Drupal core forum module.
- dominion_guestbook
  Provides a guestbook per subsite. Based on CCK nodes and comments.
- dominion_nodewords
  Provide keywords and description for nodewords per subsite.
- dominion_theme
  Allows different themes for subsites. This module is even able to rewrite CSS
  from themes (requires integration on theming side).

CONFIGURATION
-------------
After you have enabled the modules you can go to admin/build/dominion/settings
to configure Dominion. Then you can go to admin/build/dominion to add subsites.

THEMES
------
You may add configuration options to your theme which can then be configured
when adding / editing a subsite. This requires a dominion.inc file in your theme
folder. Please look at the provided dominion.example.inc for details.
