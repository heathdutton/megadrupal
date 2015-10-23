<?php

/**
 * @file
 * Administrative functionality for managing key configurations.
 */

/**
 * Menu callback; displays the list of key configurations.
 */
function key_configs_list() {
  $configs = key_get_configs();

  $header = array(
    t('Name'),
    t('Provider'),
    array('data' => t('Operations'), 'colspan' => '2'),
  );
  $rows = array();

  foreach ($configs as $name => $config) {
    $label = $config['label'];
    $name = $config['name'];
    $description = $config['description'];
    $provider = key_get_provider($config['provider']);

    $config_url_string = str_replace('_', '-', $name);

    $variables = array(
      'label' => $label,
      'name' => $name,
      'description' => $description,
    );

    // Set the name column.
    $row = array(theme('key_configs_list_description', $variables));

    // Set the key provider column.
    $row[] = array('data' => $provider['title']);

    // Set the edit column.
    $row[] = array('data' => l(t('edit'), KEY_MENU_PATH . '/manage/' . $config_url_string));

    // Set the delete column.
    $row[] = array('data' => l(t('delete'), KEY_MENU_PATH . '/manage/' . $config_url_string . '/delete'));

    $rows[] = $row;
  }

  $build['key_configs_list_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No keys are available. <a href="@link">Add a key</a>.', array('@link' => url(KEY_MENU_PATH . '/add'))),
  );

  return $build;
}

/**
 * Form constructor for the key configuration edit form.
 *
 * @param array $config
 *   (optional) An array representing the configuration, when editing an
 *   existing configuration.
 *
 * @ingroup forms
 */
function key_config_form($form, &$form_state, $config = NULL) {
  // Clear the plugin cache on the first page load, but not on AJAX refreshes.
  if (!isset($form_state['values'])) {
    _key_clear_plugin_cache();
  }

  // Get all valid key providers.
  $providers = key_get_providers(FALSE);

  // Get all valid key providers as options.
  $provider_options = key_get_providers_as_options(FALSE);

  // Store the original key in the form.
  if (!empty($config['name'])) {
    $key_value = key_get_key($config['name']);

    $form['#original_key'] = array(
      'config' => $config,
      'key_value' => $key_value,
    );

    if (!empty($key_value) && $key_value_obscure = ctools_plugin_get_function($providers[$config['provider']], 'key value obscure')) {
      $form['#original_key']['key_value_obscured'] = call_user_func($key_value_obscure, $key_value, $config);
    }
  }

  // Determine the current key provider.
  if (isset($form_state['values']['provider'])) {
    $provider = $form_state['values']['provider'];
  }
  elseif (isset($config['provider'])) {
    $provider = $config['provider'];
  }
  else {
    $provider = NULL;
  }

  $form['label'] = array(
    '#title' => t('Name'),
    '#type' => 'textfield',
    '#default_value' => $config['label'],
    '#description' => t('The human-readable name of the key.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => $config['name'],
    '#maxlength' => 32,
    '#disabled' => isset($config['name']),
    '#machine_name' => array(
      'exists' => 'key_config_load',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for the key. It must only contain lowercase letters, numbers, and underscores.'),
  );
  $form['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textfield',
    '#default_value' => $config['description'],
    '#description' => t('A short description of the key.'),
  );
  $form['provider'] = array(
    '#type' => 'select',
    '#title' => t('Key provider'),
    '#description' => t('The key provider to use.'),
    '#required' => TRUE,
    '#options' => $provider_options,
    '#default_value' => $config['provider'],
    '#ajax' => array(
      'method' => 'replace',
      'callback' => 'key_provider_extras_ajax',
      'wrapper' => 'key-provider-extras-wrapper',
    ),
  );
  $form['provider_extras'] = array(
    '#type' => 'container',
    '#prefix' => '<div id="key-provider-extras-wrapper">',
    '#suffix' => '</div>',
  );
  $form['provider_extras']['provider_settings'] = array(
    '#type' => 'container',
    '#title' => t('Key provider settings'),
    '#collapsible' => TRUE,
    '#tree' => TRUE,
  );
  $form['provider_extras']['key_value'] = array(
    '#type' => 'container',
    '#title' => t('Key value'),
    '#collapsible' => TRUE,
    '#tree' => TRUE,
  );

  // Unset the input values for provider settings and key value,
  // so they can be updated via AJAX.
  unset($form_state['input']['provider_settings']);
  unset($form_state['input']['key_value']);

  if ($provider) {
    // Add the Base64 checkbox to the settings, if allowed.
    if ($providers[$provider]['allow base64 encoding']) {
      $form['provider_extras']['provider_settings']['base64_encoded'] = array(
        '#type' => 'checkbox',
        '#title' => t('Base64-encoded'),
        '#description' => t('Check this if the key has been Base64-encoded. If the key should be used as-is, without Base64-decoding, leave this unchecked.'),
        '#default_value' => isset($config['provider_settings']['base64_encoded']) ? $config['provider_settings']['base64_encoded'] : 0,
      );
    }

    // Add any additional provider settings.
    if ($provider_settings_form = ctools_plugin_get_function($providers[$provider], 'provider settings form')) {
      $form['provider_extras']['provider_settings'] += call_user_func($provider_settings_form, $config['provider_settings']);
    }

    // If there are any provider settings, make the container a fieldset.
    if (element_children($form['provider_extras']['provider_settings'])) {
      $form['provider_extras']['provider_settings']['#type'] = 'fieldset';
    }

    // Add the value form, if defined.
    if ($provider && $key_value_form = ctools_plugin_get_function($providers[$provider], 'key value form')) {
      $form['provider_extras']['key_value']['#type'] = 'fieldset';

      // Determine what to display for the key value, if anything.
      if (isset($form['#original_key']['key_value_obscured'])) {
        $key_value = ($provider == $config['provider']) ? $form['#original_key']['key_value_obscured'] : '';
      }
      else {
        $key_value = (isset($form['#original_key']['key_value'])) ? $form['#original_key']['key_value'] : '';
      }

      $form['provider_extras']['key_value'] += call_user_func($key_value_form, array('key_value' => $key_value));
    }
  }

  $form['created'] = array(
    '#type' => 'value',
    '#value' => $config['created'],
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save key'),
    '#submit' => array('key_config_form_submit'),
    '#weight' => 40,
  );
  if (isset($config['name'])) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete key'),
      '#submit' => array('key_config_form_delete_submit'),
      '#limit_validation_errors' => array(),
      '#weight' => 45,
    );
  }
  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => KEY_MENU_PATH,
    '#weight' => 50,
  );

  return $form;
}

/**
 * Callback for AJAX form re-rendering for provider additional settings.
 */
function key_provider_extras_ajax($form, $form_state) {
  return $form['provider_extras'];
}

/**
 * Form validate handler for key_config_form().
 */
function key_config_form_validate($form, &$form_state) {
  $provider = key_get_provider($form_state['values']['provider']);

  if ($validate_callback = ctools_plugin_get_function($provider, 'config form validate')) {
    call_user_func($validate_callback, $form, $form_state);
  }
}

/**
 * Form submission handler for key_config_form().
 */
function key_config_form_submit($form, &$form_state) {
  $provider = key_get_provider($form_state['values']['provider']);

  if ($submit_callback = ctools_plugin_get_function($provider, 'config form submit')) {
    call_user_func_array($submit_callback, array($form, &$form_state));
  }

  // If the provider has changed, call the delete callback for the old
  // provider.
  if (isset($form['#original_key']['config'])) {
    $original_config = $form['#original_key']['config'];
    $original_provider = key_get_provider($original_config['provider']);
    if (($provider['name'] != $original_provider['name']) && ($delete_callback = ctools_plugin_get_function($original_provider, 'delete form submit'))) {
      call_user_func($delete_callback, $original_config);
    }
  }

  $fields = array(
    'name' => (string) $form_state['values']['name'],
    'label' => (string) $form_state['values']['label'],
    'description' => (string) $form_state['values']['description'],
    'provider' => (string) $form_state['values']['provider'],
    'provider_settings' => (isset($form_state['values']['provider_settings'])) ? serialize($form_state['values']['provider_settings']) : '',
    'created' => (string) $form_state['values']['created'],
  );

  key_save_config($fields);

  $form_state['redirect'] = KEY_MENU_PATH . '/list';
}

/**
 * Form submission handler for key_config_form().
 *
 * Handles the 'Delete' button on the key configuration form.
 */
function key_config_form_delete_submit($form, &$form_state) {
  $form_state['redirect'] = KEY_MENU_PATH . '/manage/' . str_replace('_', '-', $form['name']['#default_value']) . '/delete';
}

/**
 * Menu callback to delete a key configuration.
 */
function key_config_delete_confirm($form, &$form_state, $config) {
  $form['name'] = array('#type' => 'value', '#value' => $config['name']);
  $form['label'] = array('#type' => 'value', '#value' => $config['label']);

  $message = t('Are you sure you want to delete the key %label?', array('%label' => $config['label']));
  $caption = '<p>' . t('This action cannot be undone.') . '</p>';

  return confirm_form($form, filter_xss_admin($message), KEY_MENU_PATH, filter_xss_admin($caption), t('Delete'));
}

/**
 * Submit handler for key_config_delete_confirm().
 */
function key_config_delete_confirm_submit($form, &$form_state) {
  $config = key_get_config($form_state['values']['name']);
  $provider = key_get_provider($config['provider']);

  if ($delete_callback = ctools_plugin_get_function($provider, 'delete form submit')) {
    call_user_func($delete_callback, $config);
  }

  db_delete('key_config')
    ->condition('name', $form_state['values']['name'])
    ->execute();

  $t_args = array('%label' => $form_state['values']['label']);
  drupal_set_message(t('The key %label has been deleted.', $t_args));
  watchdog('key', 'Deleted key %label.', $t_args, WATCHDOG_NOTICE);

  $form_state['redirect'] = KEY_MENU_PATH;
}

/**
 * Returns HTML for a key configuration description.
 *
 * @param array $variables
 *   An associative array containing:
 *   - label: The human-readable label of the configuration.
 *   - name: The machine name of the configuration.
 *   - description: A brief description of the configuration.
 *
 * @ingroup themeable
 */
function theme_key_configs_list_description($variables) {
  $label = $variables['label'];
  $name = $variables['name'];
  $description = $variables['description'];

  $output = check_plain($label);
  $output .= ' <small>' . t('(Machine name: @name)', array('@name' => $name)) . '</small>';
  $output .= '<div class="description">' . filter_xss_admin($description) . '</div>';

  return $output;
}
