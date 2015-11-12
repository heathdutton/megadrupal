<?php
/**
 * @file
 * Janrain registration widget settings form.
 */

/**
 * Build the settings form.
 */
function janrain_widgets_settings_form($form, &$form_state) {
  $config = JanrainSdk::instance()->getConfig();
  // SSO Settings.
  $form['sso'] = array(
    '#type' => 'fieldset',
    '#title' => t('Single Sign On'),
    '#weight' => 0,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['sso']['sso_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Single Sign On'),
    '#description' => t('Check to enable Janrain Single Sign On.'),
    '#default_value' => (int) in_array('FederateWidget', $config->getItem('features')),
  );
  $form['sso']['sso_segment'] = array(
    '#type' => 'textfield',
    '#title' => t('Your SSO segment name'),
    '#description' => t('The SSO segment name from your Janrain SSO configuration (http://developers.janrain.com/how-to/single-sign-on/segments/)'),
    '#default_value' => filter_xss($config->getItem('sso.segment')),
    '#states' => array(
      'visible' => array(
        ':input[name="sso_enabled"]' => array('checked' => TRUE),
      ),
    ),
  );
  $segments = $config->getItem('sso.supportedSegments') ?: array();
  $form['sso']['sso_supported_segments'] = array(
    '#type' => 'textfield',
    '#title' => t('Your SSO supported segment names'),
    '#description' => t('The SSO supported segment names from your Janrain SSO configuration (http://developers.janrain.com/how-to/single-sign-on/segments/)'),
    '#default_value' => filter_xss(implode(', ', $segments)),
    '#states' => array(
      'visible' => array(
        ':input[name="sso_enabled"]' => array('checked' => TRUE),
      ),
    ),
  );

  // Backplane Settings.
  $form['bp'] = array(
    '#type' => 'fieldset',
    '#title' => t('Backplane'),
    '#weight' => 1,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['bp']['bp_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Backplane'),
    '#description' => t('Check to enable Backplane on your Drupal site.'),
    '#default_value' => (int) in_array('BackplaneWidget', $config->getItem('features')),
  );

  $form['#submit'][] = 'janrain_widgets_settings_form_submit';
  return system_settings_form($form);
}

/**
 * Form submit processor for SSO/Backplane settings.
 */
function janrain_widgets_settings_form_submit(&$form, &$form_state) {
  // Save Janrain settings.
  $config = JanrainSdk::instance()->getConfig();
  $config->save();
}

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Adds widget asset management fields to main config page.
 */
function janrain_widgets_form_janrain_admin_ui_settings_form_alter(&$form, &$form_state, $form_id) {
  $form['janrain_widgets'] = array(
    '#type' => 'fieldset',
    '#title' => t('Registration Packages'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => 3,
    '#description' => 'Manage your Janrain Registration packages and Drupal block placement.',
    '#states' => array(
      'visible' => array(
        ':input[name="janrain_product"]' => array('value' => 1),
      ),
    ),
  );

  $form['janrain_widgets']['package_upload'] = array(
    '#type' => 'fieldset',
    '#title' => t('Upload'),
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
    '#weight' => 0,
  );

  $form['janrain_widgets']['package_upload']['new_pkg'] = array(
    '#type' => 'managed_file',
    '#title' => t('Upload Janrain Registration Package'),
    '#description' => 'After uploading the file, save this form.',
    '#size' => '100',
    '#upload_validators' => array(
      'file_validate_extensions' => array('zip'),
      'janrain_widgets_file_validator' => array(),
    ),
    '#upload_location' => 'public://janrain_widgets',
    '#weight' => 0,
  );

  $form['janrain_widgets']['packages'] = array(
    '#type' => 'fieldset',
    '#title' => t('Manage'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#weight' => 1,
    '#description' => t('Download or remove your Janrain Registration packages.'),
  );

  // Manage existing widgets.
  foreach (_janrain_widgets_list_pkgs() as $uri => $fid) {
    $basename = basename($uri, '.zip');
    $form['janrain_widgets']['packages']["pkg_{$fid}"] = array(
      '#type' => 'managed_file',
      '#title' => filter_xss($basename),
      '#default_value' => $fid,
      '#process' => array('_janrain_widgets_mf_process'),
      '#description' => t('Janrain blocks created for this package will be tagged with ({{name}})', array('{{name}}' => $basename)),
    );
  }

  $form['#attributes'] = array('enctype' => "multipart/form-data");
  $form['#submit'][] = 'janrain_widgets_janrain_settings_form_submit';
}

/**
 * Process the managed file widget for active packages.
 *
 * When a user deletes a managed file they should not immediately be presented
 * with an upload interface in this context. This disables access to upload
 * features and reminds that the form must be submitted to persist the removal.
 */
function _janrain_widgets_mf_process($element, &$form_state, $form) {
  $element = file_managed_file_process($element, $form_state, $form);
  $element['upload_button']['#access'] = FALSE;
  $element['upload']['#access'] = FALSE;
  if ($element['fid']['#value'] == 0) {
    $element['#title'] .= ' will be deleted when you save this form.';
  }
  return $element;
}

/**
 * Validate the widget package.
 */
function janrain_widgets_file_validator($file) {
  $tmp = drupal_realpath($file->uri);
  $sdk = JanrainSdk::instance();
  $sdk->addFeatureByName('CaptureWidget');
  return $sdk->CaptureWidget->validateAssetsZip($tmp);
}

/**
 * Validate the submitted form.
 */
function janrain_widgets_settings_form_validate(&$form, &$form_state) {
  $sdk = JanrainSdk::instance();
  $config = $sdk->getConfig();
  $values = &$form_state['values'];

  // Federate.
  if (1 === $values['sso_enabled']) {
    // Activate federate settings for widgets.
    $sdk->addFeatureByName('FederateWidget');

    // Process federate segment.
    $segment = trim($values['sso_segment']);
    if ($segment && !ctype_alnum($segment)) {
      // Invalid characters in segment.
      form_set_error('sso_segment', t('Invalid segment name'));
    }
    else {
      // No error.
      $config->setItem('sso.segment', $segment);
    }

    // Process federate segments.
    $supported = trim($values['sso_supported_segments']);
    // Get unique segment list from user input.
    $segments = array_unique(preg_split('|[,\\s]+|', $supported, NULL, PREG_SPLIT_NO_EMPTY));
    // Reduce to simple everything is good or something is bad.
    $allgood = array_reduce($segments, function ($carry, $item) {
      // Check for valid segment names.
      return $carry && ctype_alnum($item);
    }, TRUE);
    if ($allgood) {
      $config->setItem('sso.supportedSegments', $segments);
    }
    else {
      form_set_error('sso_supported_segments', t('Invalid supported segments name'));
    }
  }
  else {
    unset($sdk->FederateWidget);
  }

  // Backplane.
  if (1 === $values['bp_enabled']) {
    // Check that the app is configured to support BP.
    $bpserver = $config->getItem('backplane.serverBaseUrl');
    $bpbus = $config->getItem('backplane.busName');
    if (filter_var($bpserver, FILTER_VALIDATE_URL, FILTER_FLAG_HOST_REQUIRED)
        && !empty($bpbus)) {
      $sdk->addFeatureByName('BackplaneWidget');
    }
    else {
      form_set_error('bp_enabled', t('Backplane server is invalid.'));
    }
  }
  else {
    unset($sdk->BackplaneWidget);
  }
}

/**
 * Submit handler for widget_settings_form.
 *
 * Note: This gets called after janrain_admin_ui_settings_form_submit
 */
function janrain_widgets_janrain_settings_form_submit(&$form, &$form_state) {
  $sdk = JanrainSdk::instance();
  $config = $sdk->getConfig();

  // Short-circuit for login-only.
  if (_janrain_is_login_only()) {
    drupal_flush_all_caches();
    unset($sdk->CaptureWidget);
    $config->save();
    return;
  }
  unset($sdk->EngageWidget);

  // Install new package.
  $new_fid = $form_state['values']['new_pkg'];
  if ($new_fid) {
    $new_pkg = _janrain_widgets_save_pkg($new_fid);
    if (!_janrain_widgets_install_pkg($new_pkg)) {
      drupal_set_message(t('An error occured while installing a package, please check watchdog.'), 'error');
    }
  }

  // Purge removed widgets.
  foreach ($form_state['values'] as $name => $value) {
    sscanf($name, 'pkg_%d', $file_fid);
    // Skip non-file entries.
    if (!$file_fid) {
      continue;
    }

    // Process file.
    if ($file_fid && 0 === $value) {
      if (!_janrain_widgets_remove_pkg($file_fid)) {
        drupal_set_message(t('An error occured while deleting a package, please check watchdog.'), 'error');
      }
    }
  }
  // Ensure package state is consistent and clear cache.
  if (!_janrain_widgets_discover_pkgs()) {
    drupal_set_message(t('An error occured with package discovery, please check watchdog.'), 'error');
  }
  // Flush caches and save.  Needs to clear blocks, pages, themes, when
  // updating available widgets.
  drupal_flush_all_caches();
  $config->save();
}
