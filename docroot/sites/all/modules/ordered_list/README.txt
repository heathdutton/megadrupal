-- SUMMARY --

This module provides the Ordered List field type. It is similar to the List
field type with two differences:
* Ordered List field keeps the order of the selected items;
* values function may be used to provide a list of values.

As a side effect, module also provides a new form element, which field widget
uses:

<?php
$form['list'] = array(
  '#type' => 'ordered_list',
  '#values' => $values,
  '#default_value' => $default_value,
);
?>


For a full description of the module, visit the project page:
  http://drupal.org/project/ordered_list

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/ordered_list


-- REQUIREMENTS --

None.


-- INSTALLATION --

* Install as usual, see https://drupal.org/node/895232 for further information.


-- CONTACT --

Current maintainers:
* Alex Zhulin (Alex Zhulin) - https://drupal.org/user/2659881

This project has been sponsored by:
* Blink Reaction
  A leader in Enterprise Drupal Development, delivering robust,
  high-performance websites for dynamic companies. Through expertise in
  seamless integration, module customization, and application development,
  Blink creates scalable and flexible web solutions that provide the best in
  dynamic user experience. Specialties: Custom Drupal Development, Enterprise
  Drupal Development, Fortune 500 Clients, End to End Solutions.
* General Electric
  American multinational conglomerate corporation, the manufacturer of many
  types of equipment, including locomotives, power plants (including nuclear
  reactors), gas turbines, aircraft engines, medical equipment, also
  manufactures lighting equipment, plastics and sealants. In 2011, company
  ranked third in the list of the largest publicly traded Forbes companies,
  and was the world's largest non-financial MNC, as well as major media
  conglomerate.
