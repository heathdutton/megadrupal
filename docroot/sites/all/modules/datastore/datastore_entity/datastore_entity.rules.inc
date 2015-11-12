<?php

/**
 * @file 
 * rules integration for the datastore_entity module
 */
 
/**
 * Implements hook_rules_action_info() on behalf of the datastore_entity module.
 */
function datastore_entity_rules_action_info() {
  return array(
    'datastore_entity_add_values' => array(
      'label' => t('Add datastore values to a datastore'),
      'group' => t('DataStore'),
      'parameter' => array(
        'domain' => array(
          'type' => 'token',
          'label' => t('Domain'),
          'options list' => 'datastore_entity_get_domain_list',
        ),
        'category' => array(
          'type' => 'token',
          'label' => t('Category'),
        ),
        'reference' => array(
          'type' => 'token',
          'label' => t('Reference'),
          'optional' => TRUE,
        ),
        'values' => array(
          'label' => t('Datastore value/timestamp set'),
          'type' => 'list<field_item_datastore_field>',
        ),
      ),
    ),
  );
}