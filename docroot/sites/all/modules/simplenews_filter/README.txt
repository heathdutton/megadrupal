
DESCRIPTION
-----------

Filter simplenews mailing lists using views.


REQUIREMENTS
------------

 * Simplenews (7.x-2.x) and Views modules.


INSTALLATION
------------

 1. CREATE DIRECTORY

    Create a new directory "simplenews_filter" in the sites/all/modules
    directory and place the entire contents of this folder in it.

 2. ENABLE THE MODULE

    Enable the module on the Modules admin page.

 3. ACCESS PERMISSION

    Grant the access at the Access control page:
      People > Permissions.

 4. CONFIGURE SIMPLENEWS_FILTER

    Configure Simplenews filter defaults on the Simplenews admin pages:
      Configuration > Simplenews > Filters.

    Filter newsletter recipients on the "Newsletter" tab of each newsletter:
      node/[nid]/simplenews.


DOCUMENTATION
-------------

Simplenews filter allows you to select any view of type "simplenews_subscriber"
on which to filter newsletter subscriptions against. The recipients must already
be subscribed to the newsletter.

If the selected view has exposed filters, they will be loaded on the
newsletter's send page via AJAX and can be modified to alter the view's results.
When the newsletter is sent, the view query is modified with details required by
the Simplenews mail spool and the results are then inserted into the mail spool.

Some example use cases:

 * Wanting to email all subscribers (with registered accounts) who live in a
     particular country (profile field)
 * Wanting to email all subscribers (with registered accounts) who have created
     some piece of content.
 * And pretty much anything else that is possible with views.
