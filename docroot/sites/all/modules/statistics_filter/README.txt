This module allows customizable filtering of hits from particular user roles or
from crawlers.

--------------------------------------------------------------------------------
Benefits
--------------------------------------------------------------------------------

For sites with light traffic (i.e., most sites), a large percentage of the gross
hits recorded by statistics.module are either from the site administrator or
from search engines. Filtering out these hits makes the collected statistics
more accurately reflect traffic from real, human visitors.

--------------------------------------------------------------------------------
Dependencies
--------------------------------------------------------------------------------

- Statistics (Drupal Core)
- Browscap (optional)

--------------------------------------------------------------------------------
Installation
--------------------------------------------------------------------------------

Download the module and simply copy it into your contributed modules folder:
[for example, your_drupal_path/sites/all/modules] and enable it from the
modules administration/management page.
More information at: Installing contributed modules (Drupal 7).

--------------------------------------------------------------------------------
Configuration
--------------------------------------------------------------------------------

After successful installation, you just need to go to Statistics settings page
(admin/config/system/statistics) and configure as you want.

To ignore search engines, you must install and enable the browscap module.

--------------------------------------------------------------------------------
Credits
--------------------------------------------------------------------------------

Mike Ryan (http://drupal.org/user/4420) is the author of this module.
Much of the code is based directly on the core statistics module.

Initial conversion to Drupal 6 by Frank Ralf
(http://drupal.org/user/216048), co-maintainer.

Nhat Tran (http://drupal.org/user/998946) is the maintainer for branch 7.x
