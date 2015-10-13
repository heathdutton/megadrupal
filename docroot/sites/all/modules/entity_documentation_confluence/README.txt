README.txt
==========

A submodule of Entity Documentation, that exports entity configuration
documentation and saves it as a page at Confluence Plugin.

This module make use of Confluence restful API in order to retrieve, create
and delete a confluence page.
It handles both Authenticated request and non Authenticated ones, based on the
settings page.

Documentation
=================

In order to export Documentation from Drupal to Confluence pages it is necessary
to fill the Settings Form at the configuration page:
admin/config/development/entity-documentation/settings.
It is required to enter a parent id for the module to recognise how to create
the documentation page tree. This tree has the following structure:

->$parent_page
  ->$entity_parent_page
    ->$bundle_page1
    ->$bundle_page2
    ->$bundle_page3

The parent id in the settings page is referring to parent page of the
documentation page tree.
If for any reason you moved the $entity_parent_page to a different space or you
somehow delete it, the module creates a new entity parent page and add the
bundle/s as child/children of this recently created page.

You can export documentation either manually using the exporter link
"Confluence" in admin/config/development/entity-documentation or leave it all up
to cron by checking the auto export setting for "Confluence" in
admin/config/development/entity-documentation/settings.

There is a hook available for altering the http request arguments. Please check
entity_documentation_confluence.api.php .

If something went wrong check the error log for the response data.
Have in mind that Confluence generates Captcha for continuous Failed Logins.

AUTHOR/MAINTAINER
======================
Author: Christina Kaloudi(Christina Kal) (https://drupal.org/user/2793535)
