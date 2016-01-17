<?php
/**
 * @file
 * Administration form for noty messages
 */

/**
 * @return array
 *   Array of the system settings form.
 */
function noty_messages_admin_settings() {
  // Per message configs.
  // Clear out the form first.
  noty_messages_messages_setup(TRUE);
  $form = array();
  // Get system settings form defaults.
  $form = system_settings_form($form);
  // Override the submit handler.
  $form['#submit'] = array('noty_messages_admin_settings_submit');
  // Attach for specific JS.
  $form['#attached']['js'] = array(
    drupal_get_path('module', 'noty_messages') . '/js/noty_admin_form.js',
  );
  $form['settings'] = array(
    '#type' => 'vertical_tabs',
    '#tree' => TRUE,
  );
  $message_types = _noty_messages_get_types(TRUE);
  $defaults = _noty_messages_get_config('global', TRUE, $message_types);
  // Generate global configuration tab.
  $default_is_noty = $defaults['is_noty'];
  unset($default_is_noty['global']);
  $lib_exists = is_file(libraries_get_path('noty') . '/js/jquery.noty.js');
  if (!$lib_exists) {
    $form['code_status'] = array(
      '#type' => 'fieldset',
      '#title' => t('Noty settings'),
      '#weight' => '-10',
      '#description' => t('Please download the !url and place it in the libraries/noty directory. So that jquery.noty.js file is located in libraries/noty/js/jquery.noty.js', array('!url' => l(t('Noty Library'), 'https://github.com/needim/noty/'))),
    );
    return $form;
  }
  noty_messages_generate_settings_form($form, 'global', $defaults);
  unset($message_types['global']);
  // Append the is_noty config.
  $form['settings']['global']['is_noty'] = array(
    '#type' => 'checkboxes',
    '#options' => $message_types,
    '#title' => t('Messages to display in noty format'),
    '#default_value' => $defaults['is_noty'],
    '#weight' => '-5',
  );
  $form['settings']['global']['theme'] = array(
    '#type' => 'select',
    '#title' => t('Theme'),
    '#options' => array(
      'noty_theme_default' => t('Default'),
      'noty_theme_mitgux' => t('Mitgux'),
      'noty_theme_twitter' => t('Twitter Bootstrap'),
      'noty_theme_facebook' => t('Facebook'),
      'noty_theme_custom' => t('Custom'),
    ),
    '#description' => t('Theme to use for these messages. Create a file called noty_theme_custom.css in your theme/css directory to use custom'),
    '#weight' => '-4',
    '#default_value' => $defaults['theme']['global'],
  );

  // Generate fieldsets for the rest.
  $defaults = _noty_messages_get_config('single', TRUE, $message_types);
  foreach ($message_types as $message_type) {
    noty_messages_generate_settings_form($form, $message_type, $defaults);
    // @todo Buttons.
    // @todo jquery_ui_easing.
  }
  $form['actions']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset all settings'),
    '#submit' => array('noty_messages_settings_reset'),
  );
  return $form;
}

/**
 * Generate settings form.
 *
 * @param array $form
 *   The original form array.
 * @param string $message_type
 *   The message type fieldset for which the form is being generated.
 * @param array $defaults
 *   The default variable configuration for this message type.
 */
function noty_messages_generate_settings_form(&$form, $message_type, $defaults) {
  $settings_key = $message_type;
  $form['settings'][$message_type] = array(
    '#type' => 'fieldset',
    '#title' => t('@type messages', array('@type' => drupal_ucfirst($message_type))),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t('Configuration for @type messages.', array('@type' => $message_type)),
  );
  if ($message_type != 'global') {
    $form['settings'][$message_type]['use_global'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use global configuration'),
      '#description' => t('Check to ignore the message settings in this pane and relegate to globals'),
      '#default_value' => !empty($defaults['use_global'][$message_type]),
    );
  }
  // Message display.
  $form['settings'][$message_type]['type'] = array(
    '#type' => 'select',
    '#title' => t('Message type'),
    '#options' => array(
      'alert' => t('Alert'),
      'error' => t('Error'),
      'success' => t('Success'),
    ),
    '#description' => t('Message display type'),
    '#default_value' => $defaults['type'][$message_type],
    '#states' => array(
      'invisible' => array(
        'input[name="settings[' . $message_type . '][use_global]"]' => array('checked' => TRUE),
      ),
      'disabled' => array(
        'input[name="settings[' . $message_type . '][use_global]"]' => array('checked' => TRUE),
      ),
    ),
  );

  // Layout of messages on the screen.
  $form['settings'][$message_type]['layout'] = array(
    '#type' => 'select',
    '#title' => t('Message layout'),
    '#options' => array(
      'top' => t('Top'),
      'topCenter' => t('Top Center'),
      'bottom' => t('Bottom'),
      'center' => t('Center'),
      'topLeft' => t('Top Left'),
      'topRight' => t('Top Right'),
    ),
    '#description' => t('Where the messages should appear on the screen'),
    '#default_value' => $defaults['layout'][$message_type],
    '#states' => array(
      'invisible' => array(
        'input[name="settings[' . $message_type . '][use_global]"]' => array('checked' => TRUE),
      ),
      'disabled' => array(
        'input[name="settings[' . $message_type . '][use_global]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['settings'][$message_type]['text_align'] = array(
    '#type' => 'select',
    '#title' => t('Message text alignment'),
    '#options' => array(
      'left' => t('Left'),
      'right' => t('Right'),
      'center' => t('Center'),
      'justify' => t('Justify'),
    ),
    '#description' => t('How should the text inside the message be aligned'),
    '#default_value' => $defaults['text_align'][$message_type],
    '#states' => array(
      'invisible' => array(
        'input[name="settings[' . $message_type . '][use_global]"]' => array('checked' => TRUE),
      ),
      'disabled' => array(
        'input[name="settings[' . $message_type . '][use_global]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['settings'][$message_type]['speed'] = array(
    '#type' => 'textfield',
    '#title' => t('Speed'),
    '#field_suffix' => t('ms'),
    '#size' => 30,
    '#description' => t('The time in milliseconds for the notice to fade in and fade out'),
    '#default_value' => $defaults['speed'][$message_type],
    '#element_validate' => array('element_validate_integer_positive'),
    '#states' => array(
      'invisible' => array(
        'input[name="settings[' . $message_type . '][use_global]"]' => array('checked' => TRUE),
      ),
      'disabled' => array(
        'input[name="settings[' . $message_type . '][use_global]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['settings'][$message_type]['timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Timeout'),
    '#field_suffix' => t('ms'),
    '#size' => 30,
    '#description' => t('The time in milliseconds the notice will display for before fading out. For sticky messages set 0'),
    '#default_value' => $defaults['timeout'][$message_type],
    '#element_validate' => array('element_validate_integer'),
    '#states' => array(
      'invisible' => array(
        'input[name="settings[' . $message_type . '][use_global]"]' => array('checked' => TRUE),
      ),
      'disabled' => array(
        'input[name="settings[' . $message_type . '][use_global]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['settings'][$message_type]['closable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Closable'),
    '#description' => t('Check to enable close button.'),
    '#default_value' => $defaults['closable'][$message_type],
    '#states' => array(
      'invisible' => array(
        'input[name="settings[' . $message_type . '][use_global]"]' => array('checked' => TRUE),
      ),
      'disabled' => array(
        'input[name="settings[' . $message_type . '][use_global]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['settings'][$message_type]['click_close'] = array(
    '#type' => 'checkbox',
    '#title' => t('Close on self click'),
    '#description' => t('Can the user dismiss the message by clicking on it'),
    '#default_value' => $defaults['click_close'][$message_type],
    '#states' => array(
      'invisible' => array(
        'input[name="settings[' . $message_type . '][use_global]"]' => array('checked' => TRUE),
      ),
      'disabled' => array(
        'input[name="settings[' . $message_type . '][use_global]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['settings'][$message_type]['modal'] = array(
    '#type' => 'checkbox',
    '#title' => t('Message modal'),
    '#description' => t('Should the background fade out when message is displayed'),
    '#default_value' => $defaults['modal'][$message_type] ,
    '#states' => array(
      'invisible' => array(
        'input[name="settings[' . $message_type . '][use_global]"]' => array('checked' => TRUE),
      ),
      'disabled' => array(
        'input[name="settings[' . $message_type . '][use_global]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['settings'][$message_type]['admin_path_disable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable noty messages on admin paths'),
    '#description' => t('Checking this box will disable noty messages on admin paths,
      as determined by the path_is_admin() function.'),
    '#default_value' => $defaults['admin_path_disable'][$message_type],
    '#states' => array(
      'invisible' => array(
        'input[name="settings[' . $message_type . '][use_global]"]' => array('checked' => TRUE),
      ),
      'disabled' => array(
        'input[name="settings[' . $message_type . '][use_global]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['settings'][$message_type]['test_noty'] = array(
    '#type' => 'button',
    '#value' => t("Test @type messages", array('@type' => $message_type)),
    '#name' => $message_type,
    '#attributes' => array('class' => array('test-noty-messages')),
  );
}

/**
 * Form submission handler for the configuration form.
 *
 * @see noty_messages_settings()
 */
function noty_messages_admin_settings_submit($form, &$form_state) {
  if (!form_get_errors()) {
    // Compile arrays to store in variables.
    $message_types = _noty_messages_get_types(TRUE);
    $values = array();
    // Pull globals out.
    $single_settings = $form_state['values']['settings'];
    $single_settings = array_intersect_key($single_settings, array_flip(array_values($message_types)));
    $global_settings = $single_settings['global'];
    unset($single_settings['global']);
    // Filter out unneeded configs.
    $config_keys = _noty_messages_get_config('global');
    $global_settings = array_intersect_key($global_settings, array_flip($config_keys));
    // Set globals first.
    foreach ($global_settings as $setting_name => $setting) {
      $values[$setting_name] = _noty_messages_get_default($message_types, $setting);
    }
    $config_keys = _noty_messages_get_config('single');
    foreach ($single_settings as $message_type => $settings) {
      $settings = array_intersect_key($settings, array_flip($config_keys));
      // Always set use global parameter.
      $values['use_global'][$message_type] = $settings['use_global'];
      // Check to make sure not set to use globals and that it's a noty type.
      if ((bool) $settings['use_global'] != TRUE && (bool) $values['is_noty'][$message_type] != FALSE) {
        // Get the settings in.
        foreach ($settings as $setting_name => $setting) {
          $values[$setting_name][$message_type] = $setting;
        }
      }
    }
    foreach ($values as $key => $value) {
      variable_set('noty_messages_' . $key, $value);
    }
    drupal_set_message(t('The configuration has been saved.'));
  }
}

/**
 * Callback function for settings reset.
 *
 * @param array $form
 *   The admin form array.
 * @param array $form_state
 *   The values inside the form.
 */
function noty_messages_settings_reset($form, &$form_state) {
  $defaults = _noty_messages_get_config();
  foreach ($defaults as $key => $value) {
    variable_del('noty_messages_' . $value);
  }
}
