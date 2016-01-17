<?php
/**
 * @file
 * recruiter_admin.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function recruiter_admin_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'roleassign_roles';
  $strongarm->value = array(
    100482362 => '100482362',
    115362423 => '115362423',
    139881272 => '139881272',
    29617163 => '29617163',
    2108160 => '2108160',
    239386854 => '239386854',
    145257234 => '145257234',
    30037204 => 0,
    244994979 => 0,
  );
  $export['roleassign_roles'] = $strongarm;

  return $export;
}
