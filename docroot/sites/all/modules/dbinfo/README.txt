h1. DB Info module

h2. What this module does
This module adds a small status report item to the /admin/status/reports page of
your Drupal website.

It tells you the database connection information for any configured databases
Drupal has connection info for. Optionally, it can test the connection on every
reports page request.

h2. Why this module was created
Websites I'm deploying use Drpual as a frontend for various database sources.
Additionaly my development workflow has various servers such as Staging,
Development and Production servers. By default Drupal does not output database
information anywhere in the administration area. To assure that servers are
configured properly this report shows site administrators what databases the
Drupal installation can connect to.

h2. Requirements
This module requires Ctools in order to generate the collapsible report(s) divs.
If your not using Ctools already you're probably not doing anything much
advanced with Drupal to begin with and likely would not need this module.

As a security measure the db password is always unset() from the report page.
All other database information is considered public knowledge.
