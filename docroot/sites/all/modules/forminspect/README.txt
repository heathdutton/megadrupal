Form inspect is a small module that enables developers to quickly view the
contents of a form array.

Form inspect no longer depends on the devel module (http://drupal.org/project/devel).
If you have devel installed and configured to use krumo (http://krumo.sourceforge.net/) Form inspect will also use krumo.

Configuration
=============
1 - After enabling the module, go to Administration >> Configuration >> Development >> Form inspect (admin/config/development/forminspect) and 
    - enable 'Display form information'
    - optionally enter form ids of forms that should be ignored. Enter one form id per line. * is a wildcard character.
2 - Form inspect provides the following access permissions at Administration >> People >> Permissions (admin/people/permissions):
    - Administer forminspect
    - View forminspect arrays

Author
======
Jorge Lopez-Lago (kurkuma)

Please use the project's issue tracker for support requests.