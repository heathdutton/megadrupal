User-agent Assign Role
======================

This module will assign a role to an anonymous if the HTTP_USER_AGENT is one
of a specified set of values.


Use Cases
---------

When the URL is pasted into the share textbox on Facebook, the page will be scraped for content and
images. If the page is unpublished or protected, Facebook will cache an error page and this error page
will be displayed for future shares. This condition persists unless manually rescraped.

The Facebook bot can be identified by HTTP_USER_AGENT and IP address range. This module enables Drupal
to assign a user role based on these identifiers. This role could then be used to show unpublished
content using [View Unpublished](https://www.drupal.org/project/view_unpublished).

Another use case would be to assign a specific role to visitors with Internet Explorer 6.


Limitations
-----------

This module only allows a single assignment of a role. It will not assign one role to User-agent "A"
and a different role to User-agent "B".

Other modules may redirect for user 0, overriding this module.
