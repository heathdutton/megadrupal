The Simple Meta module provides a flexible and easy method to set meta tags, such as page title, description and keywords for pages.


You don't need to edit each node page anymore. Just click on the "Meta" tab which appears after module installation and set meta data for the page.
You can set meta data not only for node pages, but for any page.

Simple Meta 6.2.x: 
Simple Meta module was totally reworked to make code clear and extendable.
Simple Meta provides hook_simpleta_info() now, so you can define your own meta tags. Please see simplemeta_simplemeta_info() for details.


Installing Simple Meta:
1. Copy the "Simple Meta" directory into the 'sites/all/modules'
2. Enable the "Simple Meta" module from the module administration page (Administer --> Site configuration --> Modules).

Using Simple Meta:
1. Grant "administer simplemeta" permission to needed user roles.
2. Set the meta for the current page by clicking on the "Meta" tab at the left of the page.
3. Administer meta on the Simple Meta administration page (Administer --> Content management --> SimpleMeta).
4. Configure on-page form appearance and multilanguage support on the settings page (Administer --> Content management --> SimpleMeta --> Settings)

Upgrading from 6.1.x to 6.2.x:
1. Please follow http://drupal.org/node/672472
2. Permission provided by the module was renamed, you need to grant "administer simplemeta" permission to needed user roles

Upgrading from 6.1.x to 7.1.x:
Please upgrade from 6.1.x to 6.2.x first, then upgrade to 7.1.x