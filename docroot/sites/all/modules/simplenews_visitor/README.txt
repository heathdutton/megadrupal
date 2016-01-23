
DESCRIPTION
-----------

Simplenews publishes and sends newsletters to lists of subscribers.

This module alters the URLs inside the mail body and when user clicks any
links, it is sent to a page that collect the user data and then redirects at
the original URL.

The URL change and the redirect are hidden to the user and quite in realtime.

The admin sees the log of the clicks in the email/newsletter and it can order
them by user email (single click or grouped clicks) or by urls (single click
or grouped clicks) or by date (when clicked).

The admin can see also which user never has clicked a link in the newslettere
and can resend to him the newslettere (maybe lost email).

The user list who clicked or not clicked can be exported in a .csv file.

REQUIREMENTS
------------

 * Simplenews
 * Mimemail

INSTALLATION
------------

 1. CREATE DIRECTORY

    Create a new directory "simplenews_visitor" in the sites/all/modules
    directory and place the entire contents of this simplenews_visitor folder
    in it.

 2. ENABLE THE MODULE

    Enable the module on the Modules admin page.

 3. ACCESS PERMISSION

    Grant the access at the Access control page:
      People > Permissions.

 4. CONFIGURE SIMPLENEWS VISITOR

    Configure Simplenews Visitor on the tab under Simplenews admin pages:

      Configuration > Web Services > Simplenews.
      http://www.example.com/admin/config/services/simplenews/visitor


CONFIGURATION
-------------

- URL KEY

It needs a key to verify the authenticity of the URL who is used for the
redirect.

Once saved the key, it's important never change it to avoid that all URLs into
the old newsletters will be rejected.

- MAX VALUES

It's possible to set a limit for every newsletters so the user that owns the
newsletter can't send too much email.

A value of 0 gives a unlimited grants but a value sets the max and a cumulative
count checks every mailing if the user has enought disponibility.

The base idea for above was born for a module used for a mailing service site,
so the user buys packets of mailing (automatic selling/payment not implemented
at the moment).
