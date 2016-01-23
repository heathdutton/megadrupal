
-- SUMMARY --

Field Variables provides field values as variables in a view mode for an
entity. It's an alternative for those who prefer to work with files rather
than configuration or when using tailored tpl files is the best fit.

E.g. if you add the fields "introduction" and "facts" to the Article node type
these will be available as the following variables in the node tpl file:

* Raw values:
  $article_introduction
  $article_facts

* Formatted with the default formatter:
  $article_introduction_formatted
  $article_facts_formatted
  Please note that the formatter support is experimental and doesn't work for
  all fields.

The heavy lifting is made by the Field Extract module. Field Variables only
support the fields for which Field Extract has support. Please see the
description on the Field Extract project page for more details.


-- REQUIREMENTS --

Field Extract module
http://drupal.org/project/field_extract


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

* Configure which bundles and view modes for which field variables should be
  loaded under Administration » Structure » Field variables.

* Enable the Field Variables Developer module when developing. When it's enabled
  field variables for all bundles and view modes will be loaded automatically.
  You'll also get two additional variables with information about which fields
  are available and their values for the specific view mode.

  Use <?php echo $fieldvariables; ?> to get a list of all available field
  variables.
  You can also use <?php echo $fieldvariables_values; ?> to get both available
  fields and their values.


-- CONTACT --

Current maintainers:
* Per Sandström (persand) - http://drupal.org/user/224831

This project has been sponsored by:
* Kollegorna
  http://www.kollegorna.se
  https://github.com/kollegorna
  A small development shop from Stockholm, Sweden specialized in Drupal.
