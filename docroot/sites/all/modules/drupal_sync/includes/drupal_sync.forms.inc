<?php

/**
 * @file
 * drupal_sync.forms.inc
 * Provide various forms
 */

/**
 * Add sync settings form fields to entity edit form
 *
 * @param array $form
 * @param array $form_state
 *
 * @return array
 */
function drupal_sync_add_entity_sync_settings_form($form, &$form_state) {
  $entity_type = $form_state['build_info']['args'][0];
  $entity_type_id = _drupal_sync_get_entity_type_id($entity_type, $form_state['build_info']['args'][1]);

  $drupal_sync_settings = variable_get('drupal_sync_settings', array());

  if (!empty($drupal_sync_settings['drupal_sync_entities'][$entity_type][$entity_type_id])) {
    $form['#attached']['css'][] = drupal_get_path('module', 'drupal_sync') . '/css/drupal_sync_admin.css';

    $form['drupal_sync_settings'] = array(
      '#tree' => TRUE
    );
    $form['drupal_sync_settings']['entity_type'] = array(
      '#type' => 'value',
      '#value' => $entity_type,
    );
    $form['drupal_sync_settings']['entity_type_id'] = array(
      '#type' => 'value',
      '#value' => $entity_type_id,
    );

    $drupal_sync_relation_settings = variable_get('drupal_sync_remote_relation_settings', array());

    $entity_relation_settings = !(empty($drupal_sync_relation_settings[$entity_type][$entity_type_id])) ? $drupal_sync_relation_settings[$entity_type][$entity_type_id] : array();
    foreach ($drupal_sync_settings['drupal_sync_remote'] as $site_key => $site) {
      $form['drupal_sync_settings']['drupal_sync_remote'][$site_key] = array(
        '#type' => 'fieldset',
        '#title' => t('@site_key Site: @site_url', array('@site_key' => ($site_key + 1), '@site_url' => $site['url'])),
        '#collapsible' => TRUE,
        '#prefix' => '<div id="drupal-sync-current-site-' . $site_key . '">',
        '#suffix' => '</div>',
      );

      if (!empty($site['url']) && !empty($site['login']) && !empty($site['pass'])) {

        $remote_config = drupal_sync_get_remote_config($site['url'], $site['login'], $site['pass']);

        $form_state['remote_config'] = $remote_config;

        if (!empty($remote_config[$entity_type]) && is_array($remote_config[$entity_type])) {
          $remote_entity_types_default = array(0 => t('-- not sync --'));
          foreach ($remote_config[$entity_type] as $remote_entity_type_id => $remote_entity_type_info) {
            $remote_entity_types_default[$remote_entity_type_id] = $remote_entity_type_info['name'];
          }

          $current_remote_entity_type = !empty($entity_relation_settings[$site_key]['type']) ? $entity_relation_settings[$site_key]['type'] : 0;
          $form['drupal_sync_settings']['drupal_sync_remote'][$site_key]['type'] = array(
            '#type' => 'select',
            '#title' => t('Choose remote @entity type', array('@entity' => $entity_type)),
            '#options' => $remote_entity_types_default,
            '#default_value' => $current_remote_entity_type,
            '#ajax' => array(
              'callback' => 'drupal_sync_current_entity_type_ajax_callback',
              'wrapper' => 'drupal-sync-current-site-' . $site_key,
            ),
          );

          $drupal_sync_both = (isset($entity_relation_settings[$site_key]['drupal_sync_both'])) ? $entity_relation_settings[$site_key]['drupal_sync_both'] : array();
          $form['drupal_sync_settings']['drupal_sync_remote'][$site_key]['drupal_sync_both'] = array(
            '#type' => 'checkbox',
            '#title' => t('Set the same field mapping on the remote site'),
            '#description' => t('Apply the same settings on the remote site if checked.'),
            '#default_value' => $drupal_sync_both,
          );
          $relationship_section = (isset($entity_relation_settings[$site_key])) ? $entity_relation_settings[$site_key] : array();

          _drupal_sync_current_node_type_ref($form, $form_state, $entity_type, $entity_type_id, $current_remote_entity_type, $site_key, $relationship_section);
        }
        else {
          $form['drupal_sync_settings']['drupal_sync_remote'][$site_key]['empty'] = array(
            '#title' => t('Remote site is unavailable, check connection settings'),
            '#type' => 'item',
            '#markup' => l(t('Connection settings'), 'admin/config/drupal_sync/settings', array('attributes' => array('target' => '_blank'), 'fragment' => 'edit-drupal-sync-settings-drupal-sync-remote-' . $site_key)),
          );

          if (!empty($drupal_sync_relation_settings[$entity_type][$entity_type_id][$site_key])) {
            $form['drupal_sync_settings']['drupal_sync_remote'][$site_key]['keep_settings'] = array(
              '#type' => 'select',
              '#title' => t('Keep previous saved options?'),
              '#options' => array('keep' => 'Keep', 'delete' => 'Delete'),
              '#default_value' => 'keep',
            );

            $form['drupal_sync_settings']['drupal_sync_remote'][$site_key]['previous_settings'] = array(
              '#type' => 'value',
              '#value' => $drupal_sync_relation_settings[$entity_type][$entity_type_id][$site_key],
            );
          }
        }
      }
    }
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Add to form selects with entities fields mapping
 *
 * @param array $form
 * @param array $form_state
 * @param string $entity_type
 * @param string $remote_entity_type_id
 * @param string $site_key
 * @param array $relationship_section
 */
function _drupal_sync_current_node_type_ref(&$form, $form_state, $entity_type, $entity_type_id, $remote_entity_type_id, $site_key, $relationship_section = array()) {

  if (isset($form_state['values']['drupal_sync_settings']['drupal_sync_remote'][$site_key]['type'])) {
    //process ajax entity type selection
    $remote_entity_type_id =$form_state['values']['drupal_sync_settings']['drupal_sync_remote'][$site_key]['type'];
  }

  if (!empty($remote_entity_type_id)) {
    $sync_types = module_invoke_all('drupal_sync_get_sync_types');

    $current_fields = (isset($sync_types[$entity_type][$entity_type_id]['fields'])) ? $sync_types[$entity_type][$entity_type_id]['fields'] : array();

    $remote_config = (isset($form_state['remote_config'])) ? $form_state['remote_config'] : array();
    $remote_fields = (isset($remote_config[$entity_type][$remote_entity_type_id]['fields'])) ? $remote_config[$entity_type][$remote_entity_type_id]['fields'] : array();

    $form['drupal_sync_settings']['drupal_sync_remote'][$site_key]['remote_fields_info'] = array(
        '#type' => 'value',
        '#value' => $remote_fields,
    );

    $tmp = array(0 => t('-- not sync --'));
    foreach ($remote_fields as $key => $value) {
      $tmp[$key] = $value['label'] . ' (' . t('Remote') . ')';
    }
    $remote_fields = $tmp;
    unset($tmp);

    foreach ($current_fields as $field_key => $field) {
      $drupal_sync_current_entity_type = (isset($relationship_section['ref'][$field_key])) ? $relationship_section['ref'][$field_key] : array();
      $form['drupal_sync_settings']['drupal_sync_remote'][$site_key]['ref'][$field_key] = array(
        '#type' => 'select',
        '#title' => $field['label'] . ' (' . t('Local') . ')',
        '#options' => $remote_fields,
        '#prefix' => '<div class="drupal-sync-current-node-type-select">',
        '#suffix' => '</div>',
        '#default_value' => $drupal_sync_current_entity_type,
      );
    }
  }
  else {
    unset($form['drupal_sync_settings']['drupal_sync_remote'][$site_key]['ref']);
  }
}

/**
 * Form drupal_sync_add_entity_sync_settings_form validation
 *
 * @param array $form
 * @param array $form_state
 */
function drupal_sync_add_entity_sync_settings_form_validate($form, $form_settings) {
  foreach ($form_settings['values']['drupal_sync_settings']['drupal_sync_remote'] as $site_key => $site_values) {
    //check unique field mapping
    if (!empty($site_values['ref'])) {
      $duplicate_values = _drupal_sync_get_duplicates_values($site_values['ref']);
      if (!empty($duplicate_values)) {
        foreach ($site_values['ref'] as $field_key => $field_remote_key) {
          if (in_array($field_remote_key, $duplicate_values) && !empty($field_remote_key)) {
            form_set_error('drupal_sync_settings][drupal_sync_remote][' . $site_key . '][ref][' . $field_key, 'Error, Select different fields for each local field');
          }
        }
      }
    }
  }
}

/**
 * Form drupal_sync_add_entity_sync_settings_form submit
 *
 * @param array $form
 * @param array $form_state
 */
function drupal_sync_add_entity_sync_settings_form_submit($form, &$form_state) {
  $entity_type = $form_state['values']['drupal_sync_settings']['entity_type'];
  $entity_type_id = $form_state['values']['drupal_sync_settings']['entity_type_id'];

  $values = $form_state['values']['drupal_sync_settings']['drupal_sync_remote'];

  foreach ($values as $site_key => $site) {
    if (!empty($site['keep_settings'])) {
      if ($site['keep_settings'] == 'keep') {
        $values[$site_key] = $values[$site_key]['previous_settings'];
      }
      else {
        unset($values[$site_key]['previous_settings']);
      }
      unset($values[$site_key]['keep_settings']);
    }
  }

  $drupal_sync_remote_relation_settings = variable_get('drupal_sync_remote_relation_settings', array());
  $old_values = isset($drupal_sync_remote_relation_settings[$entity_type][$entity_type_id]) ? $drupal_sync_remote_relation_settings[$entity_type][$entity_type_id] : array();

  $drupal_sync_remote_relation_settings[$entity_type][$entity_type_id] = $values;
  variable_set('drupal_sync_remote_relation_settings', $drupal_sync_remote_relation_settings);
  drupal_sync_process_both_sync_flag_changes($old_values, $values, $entity_type, $entity_type_id);
  drupal_set_message(t('The settings are saved.'), 'status');
}

/**
 * Ajax callback for drupal_sync_add_entity_sync_settings_form
 *
 * @param array $form
 * @param array $form_state
 *
 * @return array
 */
function drupal_sync_current_entity_type_ajax_callback($form, $form_state) {
  $site_key = $form_state['triggering_element']['#array_parents'][2];
  return $form['drupal_sync_settings']['drupal_sync_remote'][$site_key];

}
/**
 * Set the same
 *
 * @param array $old_values
 * @param array $new_values
 * @param string $entity_type
 * @param string $entity_type_id
 */
function drupal_sync_process_both_sync_flag_changes($old_values, $new_values, $entity_type, $entity_type_id) {
  global $base_url;

  $drupal_sync_settings = variable_get('drupal_sync_settings', array());
  $settings_to_send = array();
  foreach ($new_values as $remote_site_id => $site_info) {

    $old_sync_value = isset($old_values[$remote_site_id]['drupal_sync_both']) ? $old_values[$remote_site_id]['drupal_sync_both'] : 0;
    $new_sync_value = isset($site_info['drupal_sync_both']) ? $site_info['drupal_sync_both'] : 0;

    if ($old_sync_value != $new_sync_value) {
      if ($new_sync_value == 1) {
        //if "Sync in both directions " checked
        //send all settings to remote site
        $settings_to_send[$remote_site_id] = $site_info;
      }
      else {
        //if "Sync in both directions " unchecked
        //uncheck "Sync in both directions " on remote site
        $settings_to_send[$remote_site_id] = $site_info;
      }
    }
    elseif ($new_sync_value == 1) {
      //if "Sync in both directions " checked
      //send all settings to remote site
      $settings_to_send[$remote_site_id] = $site_info;
    }
    else {
      //if "Sync in both directions" not checked and wasnt chechecked before submit, just do nothing
      continue;
    }

    $url = !empty($drupal_sync_settings['drupal_sync_remote'][$remote_site_id]['url']) ? $drupal_sync_settings['drupal_sync_remote'][$remote_site_id]['url'] : NULL;
    if (!empty($settings_to_send[$remote_site_id]['type']) && isset($url)) {
      $settings_formated = array();
      $settings_formated['site_url'] = $base_url;
      $settings_formated['entity_type'] = $entity_type;
      $settings_formated['reciever_entity_id'] = $settings_to_send[$remote_site_id]['type'];
      $settings_formated['settings']['drupal_sync_both'] = $settings_to_send[$remote_site_id]['drupal_sync_both'];
      $settings_formated['settings']['type'] = $entity_type_id;

      $sync_types = module_invoke_all('drupal_sync_get_sync_types');
      $settings_formated['settings']['remote_fields_info'] = (isset($sync_types[$entity_type][$entity_type_id]['fields'])) ? $sync_types[$entity_type][$entity_type_id]['fields'] : array();

      $ref = array();
      if (!empty($new_values[$remote_site_id]['remote_fields_info'])) {
        foreach ($new_values[$remote_site_id]['remote_fields_info'] as $field_name => $field_info) {
          $reference_field_name = array_search($field_name, $new_values[$remote_site_id]['ref']);
          $ref[$field_name] = $reference_field_name ? $reference_field_name : 0;
        }
      }
      $settings_formated['settings']['ref'] = $ref;
      drupal_sync_send_settings_to_remote_site($remote_site_id, $settings_formated, $url);
    }
    elseif (!empty($old_values[$remote_site_id]['type']) && isset($url)) {
      // if empty $settings_to_send[$remote_site_id]['type'], clear settings on remote site
      $settings_formated = array();
      $settings_formated['site_url'] = $base_url;
      $settings_formated['entity_type'] = $entity_type;
      $settings_formated['reciever_entity_id'] = $old_values[$remote_site_id]['type'];
      $settings_formated['settings']['drupal_sync_both'] = isset($settings_to_send[$remote_site_id]['drupal_sync_both']) ? $settings_to_send[$remote_site_id]['drupal_sync_both'] : 0;
      drupal_sync_send_settings_to_remote_site($remote_site_id, $settings_formated, $url);
    }
  }
}

/**
 * Send entyti sync settings to the remote site, and process xml responce
 *
 * @param string $remote_site_id
 * @param array $settings_formated
 * @param string $url
 */
function drupal_sync_send_settings_to_remote_site($remote_site_id, $settings_formated, $url) {
  $result = drupal_sync_xmlrpc_send_entity_sync_settings($url, $settings_formated);
  //check result
  if (!empty($result['status']) && $result['status'] == 'ok') {
    drupal_set_message(t('Settings applied successfully on the site: @url', array('@url' => $url)), 'status');
  }
  else {
    $error_message = '';
    if (!empty($result['message'])) {
      $error_message = t('Error synchronization settings with site: @url, Error message: @message', array('@url' => $url, '@message' => $result['message']));
    }
    else {
      $error_message = t('Error synchronization settings with site: @url', array('@url' => $url));
    }
    drupal_set_message($error_message, 'error');
  }
}