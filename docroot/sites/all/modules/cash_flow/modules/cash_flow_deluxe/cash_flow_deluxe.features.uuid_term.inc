<?php
/**
 * @file
 * cash_flow_deluxe.features.uuid_term.inc
 */

/**
 * Implements hook_uuid_features_default_terms().
 */
function cash_flow_deluxe_uuid_features_default_terms() {
  $terms = array();

  $terms[] = array(
    'name' => 'finance log',
    'description' => NULL,
    'format' => NULL,
    'weight' => '0',
    'uuid' => 'd0c5610d-2558-4484-b1a3-52bbc1b1b73f',
    'vocabulary_machine_name' => 'tags',
  );
  $terms[] = array(
    'name' => 'yearly',
    'description' => NULL,
    'format' => NULL,
    'weight' => '0',
    'uuid' => 'dd181bb4-64cf-8234-4924-29d7a1be3666',
    'vocabulary_machine_name' => 'tags',
  );
  $terms[] = array(
    'name' => 'weekly',
    'description' => '',
    'format' => 'filtered_html',
    'weight' => '0',
    'uuid' => 'e49559f6-dd80-c3d4-7dc4-ab93dcb27a23',
    'vocabulary_machine_name' => 'tags',
  );
  $terms[] = array(
    'name' => 'monthly',
    'description' => NULL,
    'format' => NULL,
    'weight' => '0',
    'uuid' => 'eba146e1-43b2-7d74-d520-9faffb321124',
    'vocabulary_machine_name' => 'tags',
  );
  return $terms;
}
