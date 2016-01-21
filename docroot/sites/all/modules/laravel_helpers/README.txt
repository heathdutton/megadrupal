Laravel Helpers

<strong>Description</strong>
--------------------------
Bring Laravel helpers function to Drupal. This module many useful function for developer when working with Array and String. More detail about helpers read here: http://laravel.com/docs/helpers

Supported Helpers:
<ul>
  <li>Arrays</li>
  <li>Strings</li>
  <li>Miscellaneous</li>
</ul>

<strong>Example</strong>
--------------------------
<code>
<?php
// Get field value with array_get
$node = node_load(1);
$body = array_get($node->body, "und.0.safe_value");

// Get value with array_fetch
$array = array(
    array('developer' => array('name' => 'Taylor')),
    array('developer' => array('name' => 'Dayle')),
);

$array = array_fetch($array, 'developer.name');

// array('Taylor', 'Dayle');

?>
</code>
