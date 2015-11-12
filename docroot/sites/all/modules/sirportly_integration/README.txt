-- SUMMARY --

This module provides very basic integration with Sirportly
(http://sirportly.com)- A helpdesk tool. Allowing people to submit a support
ticket from the admin section of your Drupal 7 site, view open tickets that
they've sent through the module interface, and allows you, the administrator,
to select a single knowledgebase from your Sirportly account that users can
view inside the administration area of their site.

This module does not have any other dependancies other than Drupal core
(for this release), you'll need to create a user on your Sirportly site
and give them permissions to use the API. You'll also have to generate an
API key for use with this.

-- REQUIREMENTS --

None

-- INSTALLATION --

1. Download and install the official Sirportly PHP wrapper
from `https://github.com/sirportly/php-library`. Place the contents of the
library into `sites/all/libraries/sirportly`, so that the library sits
at `sites/all/libraries/sirportly/class.php`.

2. Enable the module.

3. Click the configure link on the modules list or navigate

to http://yoursite.com/admin/configure/services/sirportly

4. Enter your api credentials and click save, more options will appear.

5. Configure ticket priority and brand settings, this is required by the API
and you will not be able to access the support tab until you do this.

6. Navigate to /suppor where you'll find the ticket dashboard, and the
'submit' and 'knowledgebase' tabs.

-- CUSTOMIZATION --

There are several customisable templates that you can override by
duplicating them and placing them in your theme folder:

1. sp-kb-page.tpl.php - The knowledgebase template, prints out the main
content and also prints the hierarchical menu.

2. sp-support-page.tpl.php - Prints a drupal-constructed table displaying
tickets submitted from here that still have a status of 'new', or 'open'.
3. view-ticket.tpl.php - Prints the information of a single ticket that the
customer has sent.
