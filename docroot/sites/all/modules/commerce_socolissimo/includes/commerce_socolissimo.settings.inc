<?php

/**
 * @file
 * Page callbacks and form builder functions for administering the SoColissimo
 * flexibility shipping services.
 */


/**
 * Form callback: edit the global settings.
 */
function commerce_socolissimo_settings_form($form, &$form_state) {

  $socolissimo_api = variable_get('commerce_socolissimo_api', array());
  $socolissimo_settings = variable_get('commerce_socolissimo_settings', array());
  $socolissimo_map = _commerce_socolissimo_map_settings();

  $form['commerce_socolissimo_api'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#description' => t('Theses informations are mandatory to use the web services provided by So Colissimo.') . '<br />' . t('To subscribe at So Colissimo get in touch with your regular business contact La Poste or call 36.34'),
    '#title' => 'API Credentials',
  );

  $form['commerce_socolissimo_api']['username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#default_value' => isset($socolissimo_api['username']) ? $socolissimo_api['username'] : '',
    '#required' => TRUE,
  );

  $form['commerce_socolissimo_api']['password'] = array(
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#default_value' => isset($socolissimo_api['password']) ? $socolissimo_api['password'] : '',
    '#required' => TRUE,
  );

  $form['commerce_socolissimo_api']['url'] = array(
    '#type' => 'textfield',
    '#title' => t('API url'),
    '#size' => 80,
    '#default_value' => isset($socolissimo_api['url']) ? $socolissimo_api['url'] : 'https://pfi.telintrans.fr/pointretrait-ws-cxf/PointRetraitServiceWS/2.0?wsdl',
    '#required' => TRUE,
  );

  $form['commerce_socolissimo_settings'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#description' => t('Additional settings for So Colissimo Flexibility module.'),
    '#title' => t('Settings'),
  );

  $form['commerce_socolissimo_settings']['belgium'] = array(
    '#type' => 'checkbox',
    '#title' => t('enable Belgium shipping services'),
    '#default_value' => isset($socolissimo_settings['belgium']) ? $socolissimo_settings['belgium'] : 1,
  );

  $form['commerce_socolissimo_settings']['order_dispatching_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Dispatching time for a So Colissimo order.'),
    '#description' => t('Value in day(s)'),
    '#default_value' => isset($socolissimo_settings['order_dispatching_time']) ? $socolissimo_settings['order_dispatching_time'] : '',
  );

  $form['commerce_socolissimo_settings']['debug'] = array(
    '#type' => 'radios',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Log debug informations'),
    '#default_value' => isset($socolissimo_settings['debug']) ? $socolissimo_settings['debug'] : '',
    '#options' => array(0 => t('Disable'), 1 => t('Enable')),
  );

  $form['commerce_socolissimo_map'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t("Default values are optimized for checkout process. But you can change them according to your needs."),
    '#title' => t('Google map settings'),
  );
  $form['commerce_socolissimo_map']['google_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Google map API key'),
    '#description' => t("Using an API key enables you to monitor your application's Maps API usage, and ensures that Google can contact you about your application if necessary. If your application's Maps API usage exceeds the !usage_limits, you must use an API key in order to purchase additional quota.", array('!usage_limits' => l(t('Usage Limits'), 'https://developers.google.com/maps/documentation/javascript/usage#usage_limits'))),
    '#default_value' => $socolissimo_map['google_api_key'],
  );
  $form['commerce_socolissimo_map']['id'] = array(
    '#type' => 'textfield',
    '#title' => t('Map id'),
    '#description' => t('HTML id attribute'),
    '#default_value' => $socolissimo_map['id'],
  );
  $form['commerce_socolissimo_map']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Map width'),
    '#description' => t('HTML width attribute'),
    '#default_value' => $socolissimo_map['width'],
  );
  $form['commerce_socolissimo_map']['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Map height'),
    '#description' => t('HTML height attribute'),
    '#default_value' => $socolissimo_map['height'],
  );
  $form['commerce_socolissimo_map']['zoom'] = array(
    '#type' => 'textfield',
    '#title' => t('Map zoom'),
    '#description' => t('Define a static zoom value. ( Not used when auto-zoom is enabled )'),
    '#element_validate' => array('element_validate_integer_positive'),
    '#default_value' => $socolissimo_map['zoom'],
    '#states' => array(
      'disabled' => array(
        ':input[name="socolissimo_map[autozoom]"]' => array('value' => 1),
      ),
    ),
  );
  $form['commerce_socolissimo_map']['autozoom'] = array(
    '#type' => 'radios',
    '#title' => t('Map auto-zoom'),
    '#description' => t('Allow map to get the correct zoom value according to the generated markers.'),
    '#default_value' => $socolissimo_map['autozoom'],
    '#options' => array(0 => t('Disable'), 1 => t('Enable')),
  );
  $form['commerce_socolissimo_map']['street_view_control'] = array(
    '#type' => 'radios',
    '#title' => t('Street view control'),
    '#description' => t('Enable Street View.'),
    '#default_value' => $socolissimo_map['street_view_control'],
    '#options' => array(0 => t('Disable'), 1 => t('Enable')),
  );
  $form['commerce_socolissimo_map']['map_type_control'] = array(
    '#type' => 'radios',
    '#title' => t('Map controls'),
    '#description' => t('Show map controls.'),
    '#default_value' => $socolissimo_map['map_type_control'],
    '#options' => array(0 => t('Disable'), 1 => t('Enable')),
  );

  return system_settings_form($form);
}

/**
 * Form callback: Import rates for socolissimo shipping services.
 */
function commerce_socolissimo_import_form($form, &$form_state) {
  $form['socolissimo_import'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#description' => t('Import rates for socolissimo shipping services.'),
    '#title' => t('Import'),
  );

  $form['socolissimo_import']['file'] = array(
    '#type' => 'file',
    '#title' => t('Import CSV file'),
    '#title_display' => 'invisible',
    '#description' => t('Only .csv file are allowed.'),
  );

  $form['socolissimo_import']['clear'] = array(
    '#type' => 'checkbox',
    '#title' => t('Erase all previous rates data.'),
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );

  return $form;
}

/**
 * Validation form callback: Checks data integrity.
 */
function commerce_socolissimo_import_form_validate($form, &$form_state) {
  $file = reset($_FILES);
  $file_name = reset($file['tmp_name']);
  $file_type = reset($file['type']);
  $element = $form['socolissimo_import']['file'];

  if (!empty($file_name) && !empty($file_type)) {
    if ($file_type !== 'text/csv') {
      form_error($element, t('Only CSV extension are allowed.'));
    }
    if (empty($file_name)) {
      form_error($element, t('Error when writing the file in the temporary directory.'));
    }

    $lines = array();
    if (($handle = fopen($file_name, "r")) !== FALSE) {
      while (($data = fgets($handle, 1024)) !== FALSE) {
        $lines[] = $data;
      }
      fclose($handle);
    }

    $data = _commerce_socolissimo_import_parse_data($lines);

    if (empty($data)) {
      form_error($element, t('No data found in the passed file.'));

    }
    elseif (isset($data['message'])) {
      form_error(
        $element, t(
          'Error on line !delta: !message', array(
            '!delta' => ($data['delta'] + 1),
            '!message' => $data['message'],
          )
        )
      );
    }
    else {
      form_set_value($element, $data, $form_state);
    }
  }
  else {
    form_error($element, t('Please, attach a file.'));
  }
}

/**
 * Submission form callback: saves the import values in database.
 */
function commerce_socolissimo_import_form_submit($form, &$form_state) {
  $data = $form_state['values']['socolissimo_import']['file'];

  if ($form_state['values']['socolissimo_import']['clear'] === 1) {
    // Drop every existing rates.
    $rate_ids = array_keys(entity_load('commerce_socolissimo_rate'));
    commerce_socolissimo_rate_delete_multiple($rate_ids);
  }

  if (_commerce_socolissimo_import_save_data($data)) {
    drupal_set_message(t('The data have been successfully imported in database.'), 'status');
  }
  else {
    drupal_set_message(t('Something wrong happens during the import.'), 'error');
  }
}
