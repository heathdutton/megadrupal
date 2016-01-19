<?php
/**
 * @file
 * Admin form to set the Tealium profile and Tealium account.
 */

/**
 * Tealium Administrative settings form.
 */
function tealium_admin() {
  $form = array();

  $form['tealium'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tealium settings'),
    '#weight' => 0,
  );

  $form['javascript'] = array(
    '#type' => 'fieldset',
    '#title' => t('Javascript behaviour'),
    '#weight' => 1,
  );

  $form['tealium']['tealium_account'] = array(
    '#title' => t('Account'),
    '#type' => 'textfield',
    '#default_value' => variable_get('tealium_account', ''),
    '#required' => TRUE,
  );

  $form['tealium']['tealium_profile'] = array(
    '#title' => t('Profile'),
    '#type' => 'textfield',
    '#default_value' => variable_get('tealium_profile', ''),
    '#required' => TRUE,
  );

  $form['tealium']['tealium_environment'] = array(
    '#title' => t('Environment'),
    '#type' => 'textfield',
    '#default_value' => variable_get('tealium_environment', ''),
    '#required' => TRUE,
  );

  $form['tealium']['tealium_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Tealium'),
    '#default_value' => variable_get('tealium_enabled', TRUE),
    '#description' => t('Enable Tealium reporting.'),
  );

  // Formatted as radios because there are other ways to attach JS that may be
  // supported in the future (eg async attribute).
  $form['javascript']['tealium_javascript_async_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Attachment behaviour'),
    '#default_value' => variable_get('tealium_javascript_async_mode', 0),
    '#options' => array(
      0 => 'Synchronous',
      1 => 'Asynchronous, fired by document.ready',
    ),
    '#description' => t('WARNING: Do not load the JS asynchronously if you use MVT/Web Trends pixels. This may cause Web Trends to malfunction.'),
  );

  // Javascript can be attached in the header or the footer.
  $form['javascript']['tealium_javascript_scope'] = array(
    '#type' => 'radios',
    '#title' => t('Attachment location (default)'),
    '#default_value' => variable_get('tealium_javascript_scope', 'footer'),
    '#options' => array(
      'header' => 'Header',
      'footer' => 'Footer',
    ),
    '#description' => t('The Tealium JS can be attached to the header of the page or the footer.'),
  );

  // We'll add an empty element at the beginning (no theme region selected).
  $region_list = array('' => 'None') + system_region_list(tealium_get_current_theme(), REGIONS_ALL);

  // Javascript can be attached in the header or the footer.
  $form['javascript']['tealium_javascript_scope_theme'] = array(
    '#type' => 'select',
    '#title' => t('Attachment location (theme-specific)'),
    '#default_value' => variable_get('tealium_javascript_scope_theme', ''),
    '#options' => $region_list,
    '#description' => t(
      'The Tealium JS can be attached to a theme region. If you set this, it will <strong>override</strong> the header/footer attachment above.'
    ),
  );

  // Settings for node-specific Tealium tags.
  $form['tealium_user_tags'] = array(
    '#title' => t('Enabled per-node Tealium tags'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('tealium_user_tags', 1),
  );

  $form['tealium_content_types'] = array(
    '#type' => 'fieldset',
    '#title' => t('Supported content types'),
    '#description' => t('Tealium tags can be added or removed for nodes of the selected content types.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );

  $existing_settings = variable_get('tealium_content_types');

  // Create checkboxes for all node types.
  foreach (node_type_get_types() as $type => $info) {
    // Check if we have an existing (default) value for the type.
    $default = isset($existing_settings[$type]) ? $existing_settings[$type] : 0;

    $form['tealium_content_types'][$type] = array(
      '#title' => $type,
      '#type' => 'checkbox',
      '#default_value' => $default,
    );
  }

  $form['#validate'][] = 'tealium_admin_validate';
  $form['#submit'][] = 'tealium_admin_submit';

  $form = system_settings_form($form);

  return $form;
}

/**
 * Form validation handler for tealium_admin_settings_form().
 */
function tealium_admin_validate($form, &$form_state) {
  if ($form_state['values']['tealium_javascript_scope_theme'] == 'header' ||
    $form_state['values']['tealium_javascript_scope_theme'] == 'footer'
  ) {
    form_set_error(
      'tealium_javascript_async_mode',
      t("Header and footer can't be used as theme-specific attachment locations. Limitation of Drupal core.")
    );
  }
}

/**
 * Form submission handler for tealium_admin_settings_form().
 */
function tealium_admin_submit($form, &$form_state) {
  if ($form_state['values']['tealium_javascript_async_mode'] != 0) {
    drupal_set_message(
      t('You have set Tealium to load asynchronously. This may cause problems if you use MVT/Web Trends pixels.'),
      'warning'
    );
  }
}


/**
 * Get current theme.
 *
 * @return String
 */
function tealium_get_current_theme() {
  global $custom_theme, $theme, $user;

  if (!empty($user->theme)) {
    $current_theme = $user->theme;
  }
  elseif (!empty($custom_theme)) {
    $current_theme = $custom_theme;
  }
  else {
    $current_theme = $theme ? $theme : variable_get('theme_default', 'garland');
  }
  return $current_theme;
}