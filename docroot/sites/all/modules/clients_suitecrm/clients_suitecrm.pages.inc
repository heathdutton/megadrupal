<?php
/**
 * @file
 * Configuration form to enabled / disabled entity types from connections.
 */

/**
 * Form to configure which suitecrm resources shall be handled has entities.
 */
function clients_suitecrm_entity_config_form($form, $form_state) {
  $form['clients_suitecrm_enabled_entity_resources'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Resource entity configuration.'),
    '#description' => t('Select here the resources to create entities from.'),
  );
  $clients_suitecrm_enabled_entity_resources = variable_get('clients_suitecrm_enabled_entity_resources', array());

  $connections = clients_connection_load_all('suitecrm');
  foreach ($connections as $connection_name => $connection) {
    if (clients_suitecrm_connection_is_for_environment($connection_name, 'production')) {
      // Ensure we use the original name if this is a substitution.
      $connection_substituted_name = clients_connection_get_substituted_name($connection_name);
      if ($connection_substituted_name != $connection_name) {
        $connection_name = $connection_substituted_name;
        $connection = entity_load_single('clients_connection', $connection_name);
      }
      // Fetch all resources that belong to this connection.
      $efq = new EntityFieldQuery('clients_resource');
      $result = $efq
        ->entityCondition('entity_type', 'clients_resource')
        ->propertyCondition('connection', $connection_name)
        ->execute();
      $resource_options = array();
      if (!empty($result['clients_resource'])) {
        $client_resources = entity_load_multiple_by_name('clients_resource', array_keys($result['clients_resource']));
        foreach ($client_resources as $clients_resource) {
          $resource_options[$clients_resource->identifier()] = $clients_resource->label();
        }
      }

      if (!isset($clients_suitecrm_enabled_entity_resources[$connection_name])) {
        $clients_suitecrm_enabled_entity_resources[$connection_name] = array();
      }

      if (!empty($resource_options)) {
        $form['clients_suitecrm_enabled_entity_resources'][$connection_name] = array(
          '#type' => 'select',
          '#title' => t('Connection: %connection', array('%connection' => $connection->label)),
          '#multiple' => TRUE,
          '#options' => $resource_options,
          '#default_value' => $clients_suitecrm_enabled_entity_resources[$connection_name],
          '#element_validate' => array('clients_suitecrm_entity_config_form_validate_entity_resources'),
        );
      }

      // @TODO Add UI to cleanup (disabled) entity types.
      // UI should provide a list of existing entity tables of non-entity types.
    }
  }

  $form['cleanup'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cleanup'),
    '#description' => t('Delete all tables of resources that are no longer entities. This can not be undone!! All data are lost!!'),
  );
  $form['cleanup']['cleanup'] = array(
    '#type' => 'submit',
    '#value' => t('Delete leftover tables'),
    '#op' => 'cleanup',
    '#submit' => array('clients_suitecrm_ui_entity_config_form_cleanup_submit'),
  );

  $form['cache'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cache'),
    '#description' => t('Clears the SuiteCRM cache as well as the entity info / property cache.'),
  );
  $form['cache']['clear_cache'] = array(
    '#type' => 'submit',
    '#value' => t('Clear cache now'),
    '#op' => 'clear_cache',
    '#submit' => array('clients_suitecrm_ui_entity_config_form_clear_cache_submit'),
  );

  $form = system_settings_form($form);
  $form['#submit'][] = 'clients_suitecrm_entity_config_form_submit';
  return $form;
}

/**
 * Element validator to ensure the order of the items is consistent.
 *
 * This basically just is here to ensure there are no false overwrite messages
 * in features.
 */
function clients_suitecrm_entity_config_form_validate_entity_resources($element, &$form_state, $form) {
  $values = drupal_array_get_nested_value($form_state['values'], $element['#parents']);
  ksort($values);
  drupal_array_set_nested_value($form_state['values'], $element['#parents'], $values);
}

/**
 * Submit handler for the entity config.
 */
function clients_suitecrm_entity_config_form_submit($form, $form_state) {
  // Ensure the defaults are up to date.
  entity_defaults_rebuild(array('clients_resource'));

  // Fetch the related resources and re-save them. This triggers the creation
  // of the entity base tables and ensures the entity info is up to date.
  $connections = clients_connection_load_all('suitecrm');
  foreach ($connections as $connection_name => $connection) {
    $connection_name = clients_connection_get_substituted_name($connection_name);
    $resources = clients_resource_load_for_connection($connection_name);
    foreach ($resources as $resource) {
      if (isset($form_state['values']['clients_suitecrm_enabled_entity_resources'][$connection_name][$resource->name])) {
        entity_save('clients_resource', $resource);
      }
    }
  }

  // Saving the resource also triggered the entity info to be written new. This
  // means that the properties might changed as well. And all that means that we
  // might have a new entity which needs menu items. So flush all the caches.
  entity_info_cache_clear();
  entity_property_info_cache_clear();
  menu_rebuild();
}

/**
 * Cleanup callback.
 */
function clients_suitecrm_ui_entity_config_form_cleanup_submit($form, $form_state) {
  if (!empty($form_state['clicked_button']['#op']) && $form_state['clicked_button']['#op'] == 'cleanup') {
    $dropped = clients_suitecrm_entity_cleanup();
    drupal_set_message(t('Cleanup: Dropped %n tables.', array('%n' => count($dropped))));
  }
}

/**
 * Cache clear callback.
 */
function clients_suitecrm_ui_entity_config_form_clear_cache_submit($form, $form_state) {
  if (!empty($form_state['clicked_button']['#op']) && $form_state['clicked_button']['#op'] == 'clear_cache') {
    cache_clear_all(NULL, 'cache_suitecrm');
    cache_clear_all('*', 'cache_suitecrm', TRUE);
    entity_info_cache_clear();
    entity_property_info_cache_clear();
    drupal_get_complete_schema(TRUE);
    drupal_set_message(t('SuiteCRM cache cleared'));
  }
}
