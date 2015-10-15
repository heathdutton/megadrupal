<?php

/**
 * @file
 * Administrative settings for the NetX module.
 */

/**
 * Provides authentication settings form.
 *
 * @see netx_settings_form_validate()
 *
 * @ingroup forms
 */
function netx_settings_form($form, $form_state) {
  $form['netx_server'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('NetX server url'),
    '#desription' => t('The server host to connect, e.g. example.netx.net'),
    '#default_value' => variable_get('netx_server', ''),
  );

  $form['netx_user'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('NetX server username'),
    '#default_value' => variable_get('netx_user', ''),
  );

  $form['netx_pass'] = array(
    '#type' => 'password',
    '#title' => t('NetX server password'),
  );
  // Store value to be saved and accessible for form alters.
  $form['netx_cookie'] = array(
    '#type' => 'value',
    '#value' => variable_get('netx_cookie', ''),
  );

  // @todo Provide a autocomplete and access check to categories.
  $form['netx_upload_category'] = array(
    '#type' => 'textfield',
    '#title' => t('Category to upload files'),
    '#default_value' => variable_get('netx_upload_category', 'Drupal'),
  );

  $form['netx_log_requests'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log each response time'),
    '#description' => t('Could lead to enormous watchdog entries. Not for production use'),
    '#default_value' => variable_get('netx_log_requests', FALSE),
  );

  $form['cron'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sync settings'),
  );
  $form['cron']['netx_cron_threshold'] = array(
    '#type' => 'select',
    '#title' => t('Refresh assets every'),
    '#description' => t('More information about setting up scheduled tasks can be found by <a href="@url">reading the cron tutorial on drupal.org</a>.', array('@url' => url('http://drupal.org/cron'))),
    '#default_value' => variable_get('netx_cron_threshold', DRUPAL_CRON_DEFAULT_THRESHOLD),
    '#options' => array(0 => t('Never')) + drupal_map_assoc(array(
        3600, 10800, 21600, 43200, 86400, 604800,
      ), 'format_interval'),
  );
  $form['cron']['netx_cron_threshold_attributes'] = array(
    '#type' => 'select',
    '#title' => t('Refresh attributes every'),
    '#description' => t('More information about setting up scheduled tasks can be found by <a href="@url">reading the cron tutorial on drupal.org</a>.', array('@url' => url('http://drupal.org/cron'))),
    '#default_value' => variable_get('netx_cron_threshold_attributes', DRUPAL_CRON_DEFAULT_THRESHOLD),
    '#options' => array(0 => t('Never')) + drupal_map_assoc(array(
        3600, 10800, 21600, 43200, 86400, 604800,
      ), 'format_interval'),
  );
  $form['cron']['netx_cron_threshold_categories'] = array(
    '#type' => 'select',
    '#title' => t('Refresh categories every'),
    '#description' => t('More information about setting up scheduled tasks can be found by <a href="@url">reading the cron tutorial on drupal.org</a>.', array('@url' => url('http://drupal.org/cron'))),
    '#default_value' => variable_get('netx_cron_threshold_categories', DRUPAL_CRON_DEFAULT_THRESHOLD),
    '#options' => array(0 => t('Never')) + drupal_map_assoc(array(
        3600, 10800, 21600, 43200, 86400, 604800,
      ), 'format_interval'),
  );
  $form['cron']['netx_cron_amount'] = array(
    '#type' => 'select',
    '#title' => t('Amount of assets'),
    '#description' => t('How many assets to process pre cron run'),
    '#default_value' => variable_get('netx_cron_amount', 10),
    '#options' => array(drupal_map_assoc(array(10, 30, 50, 100, 200, 500))),
  );

  return system_settings_form($form);
}

/**
 * Form validation handler for netx_settings_form().
 */
function netx_settings_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  $cookie = variable_get('netx_cookie', '');
  if (!empty($cookie)) {
    // Authorized, old and new comparison.
    $authorize = FALSE;
    foreach (array('netx_server', 'netx_user', 'netx_pass') as $var) {
      $old = variable_get($var);
      $new = $values[$var];
      if ($var === 'netx_pass' && empty($new) && !empty($old)) {
        // Password field could be left empty to preserve old value.
        $values[$var] = $old;
        // Set form value to old one to properly update variable.
        form_set_value($form[$var], $old, $form_state);
        $new = $old;
      }
      if ($old !== $new) {
        $authorize = TRUE;
        $cookie = '';
        // Force to authorize because setting is changed.
        break;
      }
    }
  }
  else {
    // Not authorized yet, require password.
    $authorize = TRUE;
    if (empty($values['netx_pass'])) {
      form_set_error('netx_password', t('Provide a password'));
      // Do not validate because of known error.
      $authorize = FALSE;
    }
    elseif (empty($values['netx_server']) || empty($values['netx_user'])) {
      // Do not validate wrong values.
      $authorize = FALSE;
    }
  }

  if ($authorize) {
    $repo = new NetXRepository($values['netx_server']);
    try {
      $account = $repo->authenticateAndGetUserBean($values['netx_user'], $values['netx_pass']);
    }
    catch (Exception $e) {
      // Hide exception.
    }
    if (empty($account)) {
      form_set_error('netx_user', t('Authorization failed'));
    }
    else {
      // Set new cookie value to be saved.
      $cookie = $repo->getResponseCookies();
      form_set_value($form['netx_cookie'], $cookie, $form_state);
    }
  }

  // Check category existence only for authorized repository.
  $category_name = $values['netx_upload_category'];
  if ($cookie && ($authorize || $category_name !== variable_get('netx_upload_category', 'Drupal'))) {
    // Reuse authenticated repo if any.
    if (!isset($repo)) {
      $repo = new NetXRepository();
    }
    $categories = $repo->getCategoryTree();
    $category_id = _netx_find_category($category_name, $categories['subCategories']);
    // Make sure the default category exists.
    if (empty($category_id)) {
      // Try to create default category. Validation looks is not a right way to
      // create new categories, but this way code is sure that user have enough
      // rights to manage repo.
      $category_id = $repo->createCategory($category_name);
      if (empty($category_id)) {
        form_set_error('netx_upload_category', t('Failed to create category. Check that provided user/password have enough access to repository.'));
      }
      else {
        drupal_set_message(t('New category @category created, make sure to allow permissions to access it.', array(
          '@category' => $category_name,
        )), 'warning');
        watchdog('netx', 'Remote category @category with @id created', array(
          '@category' => $category_name,
          '@id' => $category_id,
        ), WATCHDOG_INFO);
      }
    }
  }
  // @todo Check access permissions needed to manage repo.
}

/**
 * Page callback to update file with pulled attributes from NetX.
 *
 * @param object $file
 *   The file object to update.
 */
function netx_sync_get_asset($file) {
  if (empty($file->metadata['assetId'])) {
    // When no metadata for file, try to get it from stream wrapper.
    // @codingStandardsIgnoreStart
    /** @var \MediaNetXStreamWrapper $wrapper */
    // @codingStandardsIgnoreEnd
    $wrapper = file_stream_wrapper_get_instance_by_uri($file->uri);
    $asset_id = $wrapper->getAssetID();
  }
  else {
    $asset_id = $file->metadata['assetId'];
  }

  if (empty($asset_id)) {
    drupal_set_message(t('No asset id for @uri', array('@uri' => $file->uri)), 'error');
  }
  else {
    $repo = new NetXRepository();
    if ($asset = $repo->getAssetBean($asset_id)) {
      $file->metadata = $asset;
      file_save($file);
      drupal_set_message(t('Asset attributes has been updated.'));
    }
    else {
      drupal_set_message(t('Error pulling asset(@asset_id) attributes.', array(
        '@asset_id' => $asset_id,
      )), 'error');
    }
  }
  drupal_goto('file/' . $file->fid);
}
