<?php
/**
 * @file
 * Administration of the Aus Post Postage Assessment Calculator.
 */

/**
 * Configure module settings including API key. Most options not available
 * until API Key is validated.
 */
function uc_auspost_pac_config_form($form, &$form_state) {

  // Aus Post PAC/PCS API Key.
  $api_key = variable_get('uc_auspost_pac_var_api_key', '');
  $form['keys'] = array(
    '#type' => 'fieldset',
    '#title' => t('API Key'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,

    'api_key' => array(
      '#type' => 'textfield',
      '#title' => t('Postage Assessment Calculation/PCS API Key'),
      '#description' => t('API Key can be obtained at the <a href="@url">Australia Post Developer Centre</a>. API Keys consist of letters, numbers, and sometimes dashes. Your key will be validated with AusPost.', array(
        '@url' => 'https://auspost.com.au/devcentre/pacpcs-registration.asp',
      )),
      '#required' => TRUE,
      '#default_value' => $api_key,
    ),

    // Any change in the key must be validated.
    'api_key_old' => array(
      '#type' => 'value',
      '#value' => $api_key,
    ),
  );

  if ($api_key) {
    // Collapse the API Key fieldset by default.
    $form['keys']['#collapsed'] = TRUE;
    $form['keys']['#description'] = t('Changing the API Key will result in reverification of the key.');

    $country_codes = cache_get('appac:country', 'cache_uc_auspost_pac_glacial');
    $country_codes = ($country_codes ? $country_codes->data : array());
    $source_countries = variable_get('uc_auspost_pac_var_source_countries', array('US', 'GB'));
    $default_country_codes = array();
    foreach ($source_countries as $country) {
      $default_country_codes[$country] = $country;
    }

    // Inform the user about the current state of the glacial cache.
    $glacial_info = _uc_auspost_pac_glacial_cache_info();

    $form['glacial'] = array(
      '#type' => 'fieldset',
      '#title' => t('Long term cached data'),
      '#description' => t('A cache of mostly static data sourced from the AusPost API is kept separate from the normal cache flushing. You can clear and rebuild this cache with the buttons below. <strong>You will most likely never need to do this.</strong> These buttons do not save any settings, such as a new API Key entered above. Reverting to the default cache re-installs the static elements hard coded into the module.'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,

      'info' => array(
        '#prefix' => '<p>',
        '#suffix' => '</p>',
        '#markup' => t('The static elements in your Glacial Cache are: <strong>!gcs_name</strong> - !gcs_desc', array(
          '!gcs_name' => $glacial_info['name'],
          '!gcs_desc' => $glacial_info['description']
        )),
        '#weight' => 0,
      ),

      'flush' => array(
        '#type' => 'checkbox',
        '#title' => t('Also flush the glacial and all other caches before rebuilding'),
        '#options' => array(
          0 => 'no-flush',
          1 => 'flush',
        ),
        '#weight' => 5,
      ),

      'glacial_actions' => array(
        '#type' => 'actions',
        '#weight' => 10,

        'rebuild' => array(
          '#type' => 'submit',
          '#value' => t('Rebuild Glacial Cache'),
          // Re-use the existing validation of the API Key to make sure that
          // flushing will work.
          '#validate' => array('uc_auspost_pac_config_form_validate'),
          '#submit' => array('uc_auspost_pac_rebuild_glacial_cache'),
        ),

        'revert' => array(
          '#type' => 'submit',
          '#value' => t('Revert to default cache'),
          // Do not need to validate this submission.
          '#submit' => array('uc_auspost_pac_revert_glacial_cache'),
        ),
      ),

      'advanced' => array(
        '#type' => 'fieldset',
        '#title' => t('Advanced regeneration settings'),
        '#description' => t('These settings are only used when re-generating the the long term cache and are not used during normal function. Do not change these settings unless you have reviewed the code and understand the concequences.'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#weight' => 15,

        'from_postcode' => array(
          '#type' => 'textfield',
          '#title' => t('From postcode'),
          '#description' => t('The from_postcode argument when requesting the Domestic Parcel Service list.'),
          '#element_validate' => array('element_validate_integer_positive'),
          '#required' => TRUE,
          '#default_value' => variable_get('uc_auspost_pac_var_from_postcode', 2000),
        ),

        'to_postcode' => array(
          '#type' => 'textfield',
          '#title' => t('To postcode'),
          '#description' => t('The to_postcode argument when requesting the Domestic Parcel Service list.'),
          '#element_validate' => array('element_validate_integer_positive'),
          '#required' => TRUE,
          '#default_value' => variable_get('uc_auspost_pac_var_to_postcode', 3000),
        ),

        'source_countries' => array(
          '#type' => 'select',
          '#title' => t('Country codes'),
          '#description' => t('The country_code values for use when requesting the Internation Letter and Parcel Service lists. Ctrl-click to multi-select. Keep the number selected low (default is 2 - US,GB).'),
          '#options' => $country_codes,
          '#multiple' => TRUE,
          '#required' => !empty($country_codes),
          '#default_value' => $default_country_codes,
        ),
      ),
    );
  }

  $form['actions'] = array(
    '#type' => 'actions',

    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save settings'),
    ),
  );

  return $form;
}

/**
 * Validation handler for module settings form.
 *
 * @see uc_auspost_pac_config_form()
 */
function uc_auspost_pac_config_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  $api_key_should_be_valid = TRUE;

  // If the API Key has changed, refresh the package information to verify that
  // it is still valid.
  if ($values['api_key'] != $values['api_key_old']) {

    // API keys have changed to be all lower case letters and numbers, with
    // dashes separating parts. The length has increased to 36 chars too.
    // Formerly 32 chars long, upper and lower case letters and numbers.
    if (!preg_match('/^[0-9A-Za-z-]*$/', $values['api_key'])) {
      form_set_error('api_key', t('API Key should consist of letters, numbers and dashes only.'));
      // No more checking needed.
      return;
    }

    // Verify that the API Key is valid by trying a known valid quote request.
    // Example provided from the Tech Spec v1.3.
    // letter/domestic/service.json?length=114&width=162&thickness=4&weight=20.
    require_once dirname(__FILE__) . '/uc_auspost_pac.quote.inc';
    $letter_test = _uc_auspost_pac_api('letter/domestic/service', array(
      'length' => 114,
      'width' => 162,
      'thickness' => 4,
      'weight' => 20,
    ), TRUE, UC_AUSPOST_PAC_CACHE_NONE, $values['api_key']);
    if ($letter_test['error']) {
      form_set_error('api_key', t('API server returned error when testing your key. @error', array('@error' => $letter_test['error'])));
    }
  }

  // @todo Use the PCS API to verify the postcodes are correct. For now, they
  // are only validated as positive integers.
  /*
  if (isset($values['from_postcode'])) {
    // The from_postcode and to_postcode must be valid.
  }
  */

  // Process the countries list into the expect form.
  if (isset($values['source_countries'])) {
    $form_state['values']['source_countries'] = array_keys(array_filter($values['source_countries']));
  }
}

/**
 * Submit handler for module settings form.
 *
 * @see uc_auspost_pac_config_form()
 */
function uc_auspost_pac_config_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  $keys = array(
    'api_key',
    'from_postcode',
    'to_postcode',
    'source_countries',
  );
  foreach ($keys as $key) {
    // Certain keys may not exist based on the state of the form.
    if (isset($values[$key])) {
      variable_set('uc_auspost_pac_var_' . $key, $values[$key]);
    }
  }

  drupal_set_message(t('Settings have been saved.'));

  // If the glacial cache does not contain the marker data that says things
  // should be built, then rebuild all the data as a batch process.
  if (!($cache = cache_get('appac:built-marker', 'cache_uc_auspost_pac_glacial'))) {
    drupal_set_message(t('Rebuilding missing data from API server.'));

    require_once dirname(__FILE__) . '/uc_auspost_pac.batch.inc';
    batch_set(_uc_auspost_pac_batch_init());
  }
}

/**
 * Submit handler to optionally flush the glacial cache and then rebuild the
 * the values from the API server.
 */
function uc_auspost_pac_rebuild_glacial_cache($form, &$form_state) {

  if ($form_state['values']['flush']) {
    variable_set('uc_auspost_pac_flush_glacial_cache', TRUE);
    drupal_set_message(t('Flushing long-term glacial cache.'));
    drupal_flush_all_caches();
  }

  require_once dirname(__FILE__) . '/uc_auspost_pac.batch.inc';
  batch_set(_uc_auspost_pac_batch_init());
}

/**
 * Submit handler to revert the glacial cache back to the one provided in the
 * install or update process.
 */
function uc_auspost_pac_revert_glacial_cache($form, &$form_state) {

  require_once dirname(__FILE__) . '/uc_auspost_pac.install';
  _uc_auspost_pac_install_prebuilt_cache();

  drupal_set_message(t('Glacial cache elements reverted to defaults.'));
}

/**
 * Configuration form to change settings specific to the shipping method.
 */
function uc_auspost_pac_config_method_form($form, &$form_state, $method_id) {

  // Get the settings for this method. $method_id variable can be trusted
  // because this form is normally only called from statically entered menu
  // items.
  $settings = _uc_auspost_pac_get_method_settings($method_id);

  $form = array();

  $form['method_id'] = array(
    '#type' => 'value',
    '#value' => $method_id,
  );
  $form['settings'] = array(
    '#type' => 'value',
    '#value' => $settings, 
  );

  // Packing method; pulled from the packing method info.
  $packing_methods = uc_auspost_pac_get_packing_method();
  $options = array();
  foreach ($packing_methods as $id => $method) {
    $ops = '';
    if (isset($method['config'])) {
      $ops = l(t('settings'), 'admin/store/settings/quotes/auspost-pac/packing/' . $id . '/' . $method_id, array('query' => drupal_get_destination()));
    }
    $options[$id] = array(
      'name' => $method['title'],
      'desc' => isset($method['description']) ? $method['description'] : '',
      'ops' => $ops,
    );
  }

  $form['packing'] = array(
    '#type' => 'fieldset',
    '#title' => t('Packing Method'),

    'packing_method' => array(
      '#type' => 'tableselect',
      '#header' => array(
        'name' => t('Name'),
        'desc' => t('Description'),
        'ops' => t('Ops'),
      ),
      '#options' => $options,
      '#multiple' => FALSE,
      '#required' => TRUE,
      '#default_value' => $settings['packing_method'],
    ),
  );

  $form['markup'] = array(
    '#type' => 'fieldset',
    '#title' => t('Post-quote adjustments'),
    '#description' => t('A fixed or percentage amount can be added to the returned shipping rate.'),

    'markup_type' => array(
      '#type' => 'select',
      '#title' => t('Markup type'),
      '#options' => array(
        UC_AUSPOST_PAC_MARKUP_NONE => t('None'),
        UC_AUSPOST_PAC_MARKUP_PERCENT => t('Percentage'),
        UC_AUSPOST_PAC_MARKUP_FIXED => t('Fixed'),
      ),
      '#default_value' => $settings['markup_type'],
    ),

    'markup_amount' => array(
      '#type' => 'textfield',
      '#title' => t('Markup amount'),
      '#size' => 5,
      '#description' => t('A positive or negative number. 5 for 5% markup, or 10 for a $10 fixed markup. -10 for a $10 markdown.'),
      '#default_value' => $settings['markup_amount'],
    ),
  );

  // Allowed services.
  $services = _uc_auspost_pac_method_accessorials_options($method_id);
  $form['services'] = array(
    '#type' => 'fieldset',
    '#title' => t('Enabled Services'),
    '#description' => t('Not all of these services will be available in all situations. Not selecting a service here enables all services.'),

    'enabled_services' => array(
      '#type' => 'checkboxes',
      '#title' => t('Enabled Services'),
      '#options' => $services,
      '#default_value' => $settings['enabled_services'],
    ),
  );

  $form['actions'] = array(
    '#type' => 'actions',

    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save settings'),
    ),
  );

  return $form;
}

/**
 * Validation handler for method configuration form.
 *
 * @see uc_auspost_pac_config_method_form().
 */
function uc_auspost_pac_config_method_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  if (!(empty($values['markup_amount']) && $values['markup_type'] == UC_AUSPOST_PAC_MARKUP_NONE)) {
    if (!is_numeric($values['markup_amount'])) {
      form_set_error('markup_amount', t('Markup amount must be a numeric value.'));
    }
  }
}

/**
 * Submit handler for method configuration form.
 *
 * @see uc_auspost_pac_config_method_form()
 */
function uc_auspost_pac_config_method_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  $method_id = $values['method_id'];
  $settings = $values['settings'];

  $settings['packing_method'] = $values['packing_method'];
  $settings['enabled_services'] = array_filter($values['enabled_services']);
  $settings['markup_type'] = $values['markup_type'];
  $settings['markup_amount'] = $values['markup_amount'];

  variable_set('uc_auspost_pac_var_method_' . $method_id, $settings);

  drupal_set_message(t('Settings have been saved.'));
}

/**
 * Helper function to determine the current state of the values stored in the
 * Glacial Cache. A marker is stored in the glacial cache that tells us how
 * the values were generated.
 *
 * @return array
 *   A keyed array with the information about the current state. source, name,
 *   and desription.
 */
function _uc_auspost_pac_glacial_cache_info() {

  // Default returned info.
  $info = array(
    'source' => 'unknown',
    'created' => 0,
    'name' => t('Unknown'),
    'description' => t('Marker provided by the module is missing. Please regenerate the glacial cache!'),
  );

  // Grab the marker.
  $glacial_marker = cache_get('appac:built-marker', 'cache_uc_auspost_pac_glacial');

  // If present, update the info based on the source value.
  if ($glacial_marker) {
    switch ($glacial_marker->data['source']) {
      case 'install':
        $info = array(
          'source' => 'install',
          'created' => $glacial_marker->data['created'],
          'name' => t('Defaults'),
          'description' => t('Using defaults provided by the module at install time.'),
        );
        break;
      case 'generated':
        $info = array(
          'source' => 'generated',
          'created' => $glacial_marker->data['created'],
          'name' => t('Manually Generated'),
          'description' => t('Glacial Cache was rebuilt on @date', array('@date' => format_date($glacial_marker->data['created'], 'long'))),
        );
        break;
      default:
        $info['description'] = t('Source marker %marker is unknown to this module.', array('%marker' => $glacial_marker->data['source']));
    }
  }

  return $info;
}
