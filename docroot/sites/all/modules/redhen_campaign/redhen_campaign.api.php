<?php
/**
  * @file
  * API documentation for hooks provided by the RedHen Campaign module.
*/

/**
  * Alter the list of fields allowed to inherit their values from an instance
  * of the same field on their parent campaign.
  *
  * @param array $field_types
  *
  * @return array
*/
function hook_redhen_campaign_inheritable_field_types_alter(&$field_types = array()) {
  $field_types[] = 'example_field';
}
