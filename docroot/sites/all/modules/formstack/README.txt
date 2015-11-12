Formstack
=========

This module provides a node field that integrates with Formstack[1].

Installation & Usage
--------------------

1. Create an account on the Formstack website[1].
2. Install & Enable the module on your Drupal site in the usual way.
3. Create a Developer Application in your Formstack account.
   Direction are on the module configureation page at:
   Configuration -> Web Services -> Formstack (admin/config/services/formstack)
4. Enter Access Token on the module Configuration page.
5. Add a 'Formstack Form' field to a content type.
6. Create new content that contains your Formstack Form field. Your available
   Formstack forms will be available in a select list.

Thanks
------
The class in inc/formstack.api.inc was originally authored by Michael Mattax &
Andrew Ruszkowski in a Drupal sandbox project.
The class was updated the Formstack REST API (v2) by Art Williams (artis)
http://drupal.org/user/77599

Contributions
-------------

Patches are welcome in the issue queue.

Maintainers
-----------

Art Williams (artis) http://drupal.org/user/77599
Shawn Price (langworthy) http://drupal.org/user/25556

[1] http://formstack.com
[2] https://www.formstack.com/admin/apiKey/add
