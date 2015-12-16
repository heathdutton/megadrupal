Nuke Drupal Frontend
====================

What is it?
-----------
Removes Drupal's frontend, when using Drupal as a services layer only.

Specifically:

1. Uninstalls frontend-only core modules (some are truly only useful when Drupal renders the frontend)
2. For all other core modules, removes access to the paths only useful to Drupal's frontend
3. Redirects disallowed paths to user login or, (if allowed) to the admin screen
4. Properly redirects entity frontend paths to their corresponding edit screen
5. Remaps breadcrumbs to match the new backend-only experience

Why bother?
-----------
To help avoid confusion when not using Drupal's built-in frontend. 

API
---
See included ndf.api.php for Drupal hook documentation.

Installation
------------
See [Installing modules (Drupal 7)](https://www.drupal.org/node/895232).

License
-------
See [GNU General Public License, version 2](http://www.gnu.org/licenses/old-licenses/gpl-2.0.html).

Contributors
------------
- [scottrigby](https://www.drupal.org/u/scottrigby)

