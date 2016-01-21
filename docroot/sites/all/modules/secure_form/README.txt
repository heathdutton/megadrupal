SUMMARY
=======

The secure form is a utility module to restrict file upload,
if there is a mismatch between the MIME type and the extension of the file.

It also provides an option to
set the autocomplete property off for form fields.

A privileged user can manage (enable/disable)
both these options from configuration section.

This module is useful in the following cases.
1) Turn off auto complete option/property from all the user input fields/forms.
2) Prevent or restrict the user from uploading a file by changing
the extension of the file (extension not matched with file MIME type).

It uses mime_content_type() or finfo_open()/finfo_file() PHP functions
to validate files during the upload process.

REQUIREMENTS
============

Windows users must include the bundled "php_fileinfo.dll" DLL file
in "php.ini".

To accomplish this,edit php.ini and uncomment
the following line, if it is commented.

extension=php_fileinfo.dll

Restart Apache for new php.ini to take effect.

INSTALLATION
============

Install as usual,
see https://drupal.org/documentation/install/modules-themes/modules-7
for further information.

CONFIGURATION
=============

Configure the module in 
Administration -> Configuration -> Content authoring -> Secure form

Authors/maintainers
===================

Clinton Correya - https://drupal.org/user/313749
Srutheesh P P  - https://drupal.org/user/883388
Rony George - https://drupal.org/user/337817
Arun M V - https://drupal.org/user/2727679
