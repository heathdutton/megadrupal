Summary:

The Webform Field Repeat module enables the webform to remember selected field
entries from current submission for up to an hour . It uses the $_SESSION to do
so. If the session is active, the form will be populated with selected entries
from remembered submission when opened again.

The fields can be selected on Webform >> Form settings for each individual 
webform. Once the fields has been selected the end users will see a check box
at the bottom of the form that they can check to have the form populated when
they open the form for a new submission. This module depends on Webform module.

This is useful when a user needs to submit a form multiple times but doesn't
want to enter the repeated information each time. For example, to order a 
number of books on a form, that allows for one book to be ordered at a time,
the name and address of the user does not have to entered every time. 

Project's Page: https://drupal.org/sandbox/qudsia.aziz/2023513
Repository: git clone --branch 7.x-1.x 
qudsia.aziz@git.drupal.org:sandbox/qudsia.aziz/2023513.git webform_field_repeat


Installation: 

The Webform Field Repeat module installs like any other Drupal module. This 
module creates a table in the database upon installation and removes the table
when the module is uninstalled.

Security:

The Webform Field Repeat module uses $_SESSION to keep track of whether it has
to load data from a previous submission or not.

Anonymous users using the same computer might accidentally use one 
another's drupal session so it is not recommended to select fields that will 
hold sensitive information to be repeated.
