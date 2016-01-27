<?php
// $Id$

/**
 * Implementation hook_rules_action_info().
 */
function markov_chain_rules_action_info() {
  $items = array();

  $items['set_field_value'] = array(
    'label' => t('Set field value with Markov Chain'),
    'base' => 'markov_chain_set_field',
    'parameter' => array(
      'node' => array(
        'type' => 'node',
        'label' => t('Node'),
        'save' => TRUE,
      ),
      'field' => array(
        'type' => 'text',
        'label' => t('Field'),
        'options list' => 'markov_chain_rules_fields_select',
      ),
      'chain' => array(
        'type' => 'text',
        'label' => t('Chain'),
        'options list' => 'markov_chain_rules_select',
      ),
    ),
    'group' => t('Node'),
    'access callback' => 'markov_chain_access',
  );
  return $items;
}

function markov_chain_set_field($node, $field, $chain) {
  $chain = markov_chain_load($chain);
  $node->{$field}['und'] = array();
  $field_info = field_read_field($field);
  $und_key = key($field_info['columns']);
  $values = $chain->generate();
  foreach ($values as $value) {
    $node->{$field}['und'][] = array($und_key => $value);
  }
}

function markov_chain_access() {
  return TRUE;
}

function markov_chain_rules_select() {
  $chains = array_keys(markov_chain_load_settings());
  return array_combine($chains, $chains);
}

function markov_chain_rules_fields_select() {
  $fields = array_keys(field_read_fields());
  return array_combine($fields, $fields);
}
