<?php

/**
 * @file
 * Admin settings for Organic groups module.
 */

/**
 * Menu callback; Admin settings form.
 *
 * Add global setting for enabling group writing.
 */
function og_ldap_api_admin_settings($form, $form_state) {
  $values = !empty($form_state['values']) ? $form_state['values'] : array();

  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'og_ldap_api') . '/og_ldap_api.admin.inc';

  $server_plugins = og_ldap_api_group_server_plugins_options();
  $default_server_plugin = variable_get('og_ldap_api_default_group_server_plugin', '');
  if (isset($values['og_ldap_api_default_group_server_plugin'])) {
    $default_server_plugin = $values['og_ldap_api_default_group_server_plugin'];
  }

  $server_plugin_info = array();
  if (!empty($default_server_plugin)) {
    $server_plugin_info = og_ldap_api_group_server_plugin($default_server_plugin);
  }

  $ldap_operations_options = og_ldap_api_membership_operations_options();
  if (isset($server_plugin_info['update capabilities']) && is_array($server_plugin_info['update capabilities'])) {
    $available_ldap_operations = array_combine($server_plugin_info['update capabilities'], $server_plugin_info['update capabilities']);

    $ldap_operations_options = array_intersect_key($ldap_operations_options, $available_ldap_operations);
  }

  $default_ldap_operations = variable_get('og_ldap_api_allowed_ldap_membership_ops', array());
  if (isset($values['og_ldap_api_allowed_ldap_membership_ops'])) {
    $default_ldap_operations = $values['og_ldap_api_allowed_ldap_membership_ops'];
  }
  $default_ldap_operations = array_intersect($default_ldap_operations, array_keys($ldap_operations_options));

  $form['og_ldap_api_default_group_server_plugin'] = array(
    '#type' => 'select',
    '#title' => t('LDAP group server plugin'),
    '#default_value' => $default_server_plugin,
    '#options' => $server_plugins,
    '#empty_value' => '',
    '#empty_option' =>  t('- None -'),
    '#description' => t('Select the plugin that will control group and membership updates to the LDAP server.'),
    '#ajax' => array(
      'callback' => 'og_ldap_api_admin_settings_capabilities_ajax',
      'wrapper' => 'og-ldap-api-ldap-wrapper',
     ),
  );
  if (empty($server_plugins)) {
    $form['og_ldap_api_default_group_server_plugin'] = array(
      '#empty_option' =>  t('- No plugins available -'),
      '#disabled' => TRUE,
    ) + $form['og_ldap_api_default_group_server_plugin'];
    unset($form['#ajax']);
  }

  $form['ldap'] = array(
    '#type' => 'fieldset',
    '#title' => t('Updates pushed to the LDAP server (write operations)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#prefix' => '<div id="og-ldap-api-ldap-wrapper">',
    '#suffix' => '</div>',
    '#description' => t('Organic Groups can perform membership updates back to the LDAP server.<br />
                        A specific group node\'s LDAP dn fields must be populated in order to allow writes back to the LDAP server.<br />
                        The binding service account must have write permissions to LDAP base dn.'),
  );

  $form['ldap']['og_ldap_api_allowed_ldap_membership_ops'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Operations allowed'),
    '#default_value' => $default_ldap_operations,
    '#options' => $ldap_operations_options,
    '#disabled' => !$default_server_plugin,
    '#prefix' => '<div id="og-ldap-api-allowed-ldap-membership-ops-wrapper">',
    '#suffix' => '</div>',
  );

  $entity_delete_options = array('user::user' => t('User'));
  $group_bundles = og_get_all_group_bundle();
  foreach ($group_bundles as $entity_type => $bundles) {
    foreach ($bundles as $bundle_name => $bundle_label) {
      $entity_delete_options[$entity_type . '::' . $bundle_name] = t('@bundle_label (@entity_type)', array(
        '@entity_type' => $entity_type,
        '@bundle_label' => $bundle_label,
      ));
    }
  }

  $form['ldap']['og_ldap_api_revoke_entity_delete_exclusions'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Do not revoke memberships on delete of the following:'),
    '#disabled' => empty($ldap_operations_options['revoke']),
    '#default_value' => variable_get('og_ldap_api_revoke_entity_delete_exclusions', array()),
    '#options' => $entity_delete_options,
    '#disabled' => !$default_server_plugin,
    '#states' => array(
      'invisible' => array(
        ':input[name="og_ldap_api_allowed_ldap_membership_ops[revoke]"]' => array('checked' => FALSE),
      ),
    ),
  );

  // Filter checkbox values
  $form['array_filter'] = array('#type' => 'value', '#value' => TRUE);

  return system_settings_form($form);
}

/**
 * AJAX callback for group server plugin capabilities
 */
function og_ldap_api_admin_settings_capabilities_ajax($form, $form_state) {
  return $form['ldap'];
}
