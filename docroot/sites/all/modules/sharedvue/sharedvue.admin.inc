<?php

/**
 * @file
 * Admin page callbacks for the SharedVue module.
 */

/**
 * Admin callback for the main config form
 */
function sharedvue_admin($form, &$form_state) {

  $form = array();
  //variable_del('sharedvue_endpoints');

  $sharedvue_endpoints = variable_get('sharedvue_endpoints', array());

  // If the form is being rebuilt with an existing $form_state use that instead.
  if (!empty($form_state['values']['sharedvue_endpoints'])) {
    $sharedvue_endpoints = $form_state['values']['sharedvue_endpoints'];
  }

  $sharedvue_endpoints[] = array(
    'key' => '',
    'title' => '',
  );

  //drupal_set_message(var_export($sharedvue_endpoints, true));

  $form['sharedvue_base_url'] = array(
    '#title' => t('Base URL'),
    '#type' => 'textfield',
    '#default_value' => variable_get('sharedvue_base_url', $GLOBALS['base_url']),
    '#description' => t("The URL to pass to SharedVue's content syndication service. By default this is the current site URL, but on a development site you may wish to pass an alternative value to mimic the live site. If this value is incorrect SharedVue will not serve content."),
  );

  $form['sharedvue_suppress_title'] = array(
    '#title' => t('Suppress page title'),
    '#description' => t("The content returned from the SharedVue syndication service renders Drupal's title superfluous on most themes. Enable this setting to suppress the display of the title on this page."),
    '#type' => 'checkbox',
    '#default_value' => variable_get('sharedvue_suppress_title', TRUE),
  );

  $form['sharedvue_endpoints']  = array(
    '#type' => 'fieldset',
    '#title' => t('SharedVue endpoints'),
    '#tree' => TRUE,
    '#description' => t('The endpoint key, e.g. purestorage, vmware. This will form the URL from which to pull content e.g. purestorage.sharedvue.net'),
    '#prefix' => '<div id="sharedvue-endpoints">',
    '#suffix' => '</div>',
  );

  foreach ($sharedvue_endpoints as $index => $sharedvue_endpoint) {
    $form['sharedvue_endpoints'][$index] = array(
      '#type' => 'fieldset',
      '#title' => $sharedvue_endpoint['title'] ? $sharedvue_endpoint['title'] : t('Enter endpoint details'),
      '#tree' => TRUE,
      '#description' => $sharedvue_endpoint['key'] ? l($GLOBALS['base_url'] . base_path() . drupal_get_path_alias('sharedvue/' . $sharedvue_endpoint['key']), 'sharedvue/' . $sharedvue_endpoint['key']) : '',
    );

    $form['sharedvue_endpoints'][$index]['key']  = array(
      '#title' => t('Endpoint key'),
      '#default_value' => $sharedvue_endpoint['key'],
      '#type' => 'textfield',
    );

    $form['sharedvue_endpoints'][$index]['title']  = array(
      '#title' => t('Endpoint title'),
      '#default_value' => $sharedvue_endpoint['title'],
      '#type' => 'textfield',
    );
  }

  $form['add_endpoint'] = array(
    '#value' => t('Add another endpoint'),
    '#type' => 'button',
    '#ajax' => array(
      'callback' => 'sharedvue_admin_ajax_callback',
      'wrapper' => 'sharedvue-endpoints',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  $form['actions'] = array('#type' => 'actions');

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Ajax callback for admin form.
 */
function sharedvue_admin_ajax_callback($form, $form_state) {
  return $form['sharedvue_endpoints'];
}

/**
 * Validate callback for the admin form.
 */
function sharedvue_admin_validate($form, $form_state) {
  foreach ($form_state['values']['sharedvue_endpoints'] as $index => $sharedvue_endpoint) {

    if (empty($sharedvue_endpoint['key']) xor empty($sharedvue_endpoint['title'])) {
      // @TODO: Validation error not set on correct field
      form_set_error('sharedvue_endpoints[' . $index . '][key', t('Please ensure both fields are filled out'));
    }

    if (!empty($sharedvue_endpoint['key']) && !preg_match('/^(?:[A-Za-z0-9][A-Za-z0-9\-]{0,61}[A-Za-z0-9]|[A-Za-z0-9])$/', $sharedvue_endpoint['key'])) {
      form_set_error('sharedvue_endpoints[' . $index . '][key', t('Key contains invalid characters.'));
    }
  }
}

/**
 * Submit callback for the admin form.
 */
function sharedvue_admin_submit($form, &$form_state) {

  form_state_values_clean($form_state);

  variable_set('sharedvue_base_url', $form_state['values']['sharedvue_base_url']);
  variable_set('sharedvue_suppress_title', $form_state['values']['sharedvue_suppress_title']);

  $sharedvue_endpoints = array();
  foreach ($form_state['values']['sharedvue_endpoints'] as $sharedvue_endpoint) {
    if (!empty($sharedvue_endpoint['key']) && !empty($sharedvue_endpoint['title'])) {
      $sharedvue_endpoints[] = $sharedvue_endpoint;
    }
  }
  variable_set('sharedvue_endpoints', $sharedvue_endpoints);

  drupal_set_message(t('The configuration options have been saved. You may need to <a href="/admin/config/development/performance">flush the cache</a> before new syndicated content pages will show up.'));
}
