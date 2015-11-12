<?php

// optionally using version 3 googlemaps API

/**
 * @file
 * getdirections module administration functions.
 */

/**
 * Function to display the getdirections admin settings form
 * @return
 *   Returns the form.
 */
function getdirections_settings_form() {
  drupal_add_js(GETDIRECTIONS_PATH . '/js/getdirections_admin.js');

  $form = array();

  // api3
  $apikey = variable_get('getdirections_api3_key', '');
  $form['api3'] = array(
    '#type' => 'fieldset',
    '#title' => t('Google API key'),
    '#description' => t('This key is currently not required by Google.'),
    '#collapsible' => TRUE,
    #'#collapsed' => ($apikey ? TRUE : FALSE),
    '#collapsed' => TRUE,
  );
  $form['api3']['getdirections_api3_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Your Google maps API Version 3 key'),
    '#default_value' => $apikey,
    '#description' => t('For more information about this key see this !u', array('!u' => l(t('page on google'), 'http://code.google.com/apis/maps/documentation/javascript/tutorial.html#api_key', array('attributes' => array('target' => '_blank'))))),
    '#size' => 95,
    '#required' => FALSE,
  );

  // preview
  $form['getdirections_preview'] = array(
    '#type' => 'fieldset',
    '#title' => t('Getdirections Preview'),
    '#description' => t('A preview of the current map settings.') . '<br />' . t('You can adjust the default Map center, Zoom and Default map type by changing the map.') . '<br />' . t('For all other changes use the form. Remember to Save configuration when you are done.'),
    // This will store all the defaults in one variable.
    '#tree' => FALSE,
  );
  $form['getdirections_preview']['preview_map'] = array('#markup' => '');

  // default settings
  $form['getdirections_default'] = array(
    '#type' => 'fieldset',
    '#title' => t('Get directions map settings'),
    // This will store all the defaults in one variable.
    '#tree' => TRUE,
  );
  $getdirections_defaults = getdirections_defaults();
  $form['getdirections_default'] += getdirections_default_settings($getdirections_defaults);

  // return link
  $getdirections_returnlink_defaults = getdirections_returnlink_defaults();
  $returnlinkform = getdirections_returnlink_settings($getdirections_returnlink_defaults);
  if ($returnlinkform) {
    $form['getdirections_returnlink'] = array(
      '#type' => 'fieldset',
      '#title' => t('Links back to page'),
      '#description' => t("Enable and format 'Return to page' links."),
      // This will store all the defaults in one variable.
      '#tree' => TRUE,
    );
    $form['getdirections_returnlink'] += $returnlinkform;
  }

  if (module_exists('colorbox')) {
    $form['getdirections_colorbox'] = array(
      '#type' => 'fieldset',
      '#title' => t('Colorbox settings'),
      // This will store all the defaults in one variable.
      '#tree' => TRUE,
    );
    $getdirections_colorbox_defaults = getdirections_colorbox_defaults();
    $form['getdirections_colorbox'] += getdirections_colorbox_settings($getdirections_colorbox_defaults);
  }

  // misc
  $form['getdirections_misc'] = array(
    '#type' => 'fieldset',
    '#title' => t('Other settings'),
    // This will store all the defaults in one variable.
    '#tree' => TRUE,
  );
  $getdirections_misc_defaults = getdirections_misc_defaults();
  $getdirections_misc_defaults['use_advanced'] = $getdirections_defaults['use_advanced'];
  $form['getdirections_misc'] += getdirections_misc_settings($getdirections_misc_defaults);

  // user agent string
  $form['getdirections_ua'] = array(
    '#type' => 'fieldset',
    '#title' => t('User Agent detection'),
    '#description' => t('For advanced users who want to edit the User Agent detection string.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    // This will store all the defaults in one variable.
    '#tree' => TRUE,
  );
  $getdirections_ua = getdirections_ua_get();
  $form['getdirections_ua']['ua_string'] = array(
    '#type' => 'textfield',
    '#title' => t('User Agent detection'),
    '#description' => t('User Agent detection is used to determine wether the browser is on a mobile device. Edit this regular expression with care.'),
    '#size' => 60,
    '#maxlength' => 255,
    '#default_value' => $getdirections_ua['ua_string'],
    '#required' => TRUE,
  );

  // getdirections_paths
  $getdirections_paths = getdirections_paths_get();
  $form['getdirections_paths'] = array(
    '#type' => 'fieldset',
    '#title' => t('Javascript paths'),
    '#description' => t('For advanced users who want to supply their own javascript.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    // This will store all the defaults in one variable.
    '#tree' => TRUE,
  );
  $form['getdirections_paths']['getdirections_v3_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to getdirections v3 javascript file'),
    '#default_value' => $getdirections_paths['getdirections_v3_path'],
    '#size' => 60,
    '#description' => t('Where the getdirections v3 javascript file is located.')
  );
  $form['getdirections_paths']['getdirections_v3a_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to getdirections v3 advanced javascript file'),
    '#default_value' => $getdirections_paths['getdirections_v3a_path'],
    '#size' => 60,
    '#description' => t('Where the getdirections v3 advanced javascript file is located.')
  );
  $form['getdirections_paths']['getdirections_v3b_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to getdirections v3 alternate javascript file'),
    '#default_value' => $getdirections_paths['getdirections_v3b_path'],
    '#size' => 60,
    '#description' => t('Where the getdirections v3 alternate javascript file is located.')
  );
  $form['getdirections_paths']['getdirections_preview_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to getdirections preview javascript file'),
    '#default_value' => $getdirections_paths['getdirections_preview_path'],
    '#size' => 60,
    '#description' => t('Where the getdirections preview javascript file is located.')
  );

  $form['getdirections_paths']['reset'] = array(
    '#type' => 'checkbox',
    '#title' => t('Reset'),
    '#description' => t('Reset the paths to the defaults.'),
    '#default_value' => 0,
    '#return_value' => 1,
  );

  unset($form['#theme']);
  $form['#theme'] = 'getdirections_settings_form';
  $form['#validate'][] = 'getdirections_settings_validate';

  return system_settings_form($form);

}

function getdirections_settings_validate($form, &$form_state) {
  if ( ! is_numeric($form_state['values']['getdirections_default']['to_width']) || $form_state['values']['getdirections_default']['to_width'] < 1) {
    form_set_error('getdirections_default][to_width', t('Width of To box must be a positive number'));
  }
  if ( ! is_numeric($form_state['values']['getdirections_default']['from_width']) || $form_state['values']['getdirections_default']['from_width'] < 1) {
    form_set_error('getdirections_default][from_width', t('Width of From box must be a positive number'));
  }
  if (isset($form_state['values']['getdirections_returnlink']['page_link'])) {
    if ( empty($form_state['values']['getdirections_returnlink']['page_link'])) {
      form_set_error('getdirections_returnlink][page_link', t('Text of return link cannot be empty'));
    }
  }
  if (isset($form_state['values']['getdirections_returnlink']['user_link'])) {
    if ( empty($form_state['values']['getdirections_returnlink']['user_link'])) {
      form_set_error('getdirections_returnlink][user_link', t('Text of return link cannot be empty'));
    }
  }
  if (isset($form_state['values']['getdirections_returnlink']['term_link'])) {
    if ( empty($form_state['values']['getdirections_returnlink']['term_link'])) {
      form_set_error('getdirections_returnlink][term_link', t('Text of return link cannot be empty'));
    }
  }
  if (isset($form_state['values']['getdirections_returnlink']['comment_link'])) {
    if ( empty($form_state['values']['getdirections_returnlink']['comment_link'])) {
      form_set_error('getdirections_returnlink][comment_link', t('Text of return link cannot be empty'));
    }
  }

  if ( isset($form_state['values']['use_advanced']) && ! $form_state['values']['use_advanced']) {
    $form_state['values']['advanced_alternate'] = 0;
    $form_state['values']['advanced_autocomplete'] = 0;
    $form_state['values']['advanced_autocomplete_bias'] = 0;
  }

  $list = array('Map', 'Satellite', 'Hybrid', 'Physical');
  $found = 0;
  foreach ($list AS $maptype) {
    if (isset($form_state['values']['getdirections_default']['maptype'])
      && $form_state['values']['getdirections_default']['maptype'] == $maptype
      && isset($form_state['values']['getdirections_default']['baselayers'][$maptype])
      && $form_state['values']['getdirections_default']['baselayers'][$maptype]
      ) {
      $found = 1;
      break;
    }
  }
  if (! $found) {
    // force a setting
    $form_state['values']['getdirections_default']['maptype'] = 'Map';
    $form_state['values']['getdirections_default']['baselayers']['Map'] = 1;
  }

  $reset = $form_state['values']['getdirections_paths']['reset'];
  unset($form_state['values']['getdirections_paths']['reset']);
  if ($reset) {
    $form_state['values']['getdirections_paths'] = getdirections_paths_get(TRUE);
  }

}
