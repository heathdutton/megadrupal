                         W3C Validator
                         -------------
                         
by Dominique CLAUSE: http://drupal-addict.com

This module uses the api provided by the W3C markup validator to validate the
html of any url or of all urls on your site (w3c_validator_site).

It requires to have a validator instance installed somewhere.
NOTE : You could use the official validator for testing but it is higly
discouraged to use it for high volume validation.

For Debian based distributions you can install an instance of the w3c validator
by installing the package w3c_markup_validator.

Once you have the validator installed you can activate this module and go to
admin/settings/w3c_validator and enter the url of the installed validator.

To use the official W3C validator you can enter this url:
http://validator.w3.org/check

To use default install of the w3c_markup_validator package the url might be
like: http://localhost/w3c-markup-validator

For validating single urls you should visit the path /validator and enter any
url.

For validating your entire site you should visit admin/reports/w3c_validator.