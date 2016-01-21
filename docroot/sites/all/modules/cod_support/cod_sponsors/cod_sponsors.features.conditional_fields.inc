<?php
/**
 * @file
 * cod_sponsors.features.conditional_fields.inc
 */

/**
 * Implements hook_conditional_fields_default_fields().
 */
function cod_sponsors_conditional_fields_default_fields() {
  $items = array();

  $term = taxonomy_term_machine_name_load('module', 'sponsorship_level');
  if ($term) {
    $items["node:sponsor:0"] = array(
      'entity' => 'node',
      'bundle' => 'sponsor',
      'dependent' => 'field_sponsor_module_level',
      'dependee' => 'field_sponsor_sponsorship_level',
      'options' => array(
        'state' => 'visible',
        'condition' => 'value',
        'grouping' => 'AND',
        'effect' => 'show',
        'effect_options' => array(),
        'element_view' => array(
          1 => 1,
          2 => 2,
        ),
        'element_view_per_role' => 0,
        'element_view_roles' => array(),
        'element_edit' => array(
          1 => 1,
          3 => 3,
        ),
        'element_edit_per_role' => 0,
        'element_edit_roles' => array(),
        'selector' => '',
        'values_set' => 1,
        'value_form' => $term->tid,
        'value' => array(
          0 => array(
            'tid' => $term->tid,
          ),
        ),
        'values' => array(),
      ),
    );
  }

  return $items;
}
