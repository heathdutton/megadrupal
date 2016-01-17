<?php
/**
 * @file
 * This file contains all the admin functions.
 */

/**
 * Reach us administrative form settings().
 */
function reach_us_form($form, &$form_state) {
  $services = _reach_us_sort_services();
  $form = array();
  // Build Config fields for each services.
  foreach ($services as $key => $service) {
    $varlinktext = 'reach_us_serv_' . $key . '_linktext';
    $varid = 'reach_us_serv_' . $key . '_id';
    $varweight = 'reach_us_serv_' . $key . '_weight';
    $varname = 'reach_us_serv_' . $key . '_name';

    // First column in table, Name of the service.
    $form['services'][$key][$varname] = array(
      '#markup' => check_plain($services[$key]['label']),
    );

    // Second column in table, Get the Link text to render in front end.
    $default_varlinktext = variable_get($varlinktext, '');
    $form['services'][$key][$varlinktext] = array(
      '#type' => 'textfield',
      '#title' => NULL,
      '#size' => 30,
      '#default_value' => trim($default_varlinktext) ? $default_varlinktext : $services[$key]['linktext'],
    );

    // Third column in table, Get ID for each service.
    $default_varid = variable_get($varid, '');
    $form['services'][$key][$varid] = array(
      '#type' => 'textfield',
      '#title' => NULL,
      '#default_value' => trim($default_varid) ? $default_varid : '',
      '#description' => filter_xss($services[$key]['description']),
      '#size' => 30,
    );
    // Forth Column, set the weight for each the row.
    $default_varweight = variable_get($varweight, 10);
    $form['services'][$key][$varweight] = array(
      '#type' => 'weight',
      '#default_value' => $default_varweight ? $default_varweight : 10,
    );
  }
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['settings']['reach_us_display'] = array(
    '#type' => 'select',
    '#title' => t('Link type'),
    '#options' => array(
      'icon' => t('Icon'),
      'text' => t('Text'),
      'both' => t('Both'),
    ),
    '#default_value' => variable_get('reach_us_display', 'icon'),
    '#description' => t('Select the service link type as either icon or text'),
  );

  $form['settings']['reach_us_iconsize'] = array(
    '#type' => 'select',
    '#title' => t('Icon size'),
    '#options' => array(
      'sm' => t('Small: 32x32px'),
      'md' => t('Medium: 48x48px'),
      'lg' => t('Large: 64x64px'),
    ),
    '#default_value' => array(variable_get('reach_us_iconsize', 'sm')),
  );

  $form['settings']['reach_us_visibility'] = array(
    '#type' => 'select',
    '#title' => t('Visibility'),
    '#options' => array(
      'vertical' => t('Vertical'),
      'horizontal' => t('Horizontal'),
    ),
    '#default_value' => variable_get('reach_us_visibility', 'horizontal'),
    '#description' => t('Select how the items to be display, top to bottom or left to right'),
  );

  $form['settings']['reach_us_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Default message.'),
    '#default_value' => variable_get('reach_us_message', ''),
    '#description' => t('For some apps, Example FB messenger or SMS, its good to send just some short message, so you can back to them.'),
  );

  $form['#theme'] = 'reach_us_form';
  return system_settings_form($form);
}

/**
 * Theme the admin form.
 *
 * This helps the services in admin are draggable and order-able.
 */
function theme_reach_us_form($variables) {
  $form = $variables['form'];
  $rows = array();

  foreach (element_children($form['services']) as $key) {
    $row = array();
    $varlinktext = 'reach_us_serv_' . $key . '_linktext';
    $varid = 'reach_us_serv_' . $key . '_id';
    $varweight = 'reach_us_serv_' . $key . '_weight';
    $varname = 'reach_us_serv_' . $key . '_name';

    if (isset($form['services'][$key][$varweight])) {
      $row[] = drupal_render($form['services'][$key][$varname]);
      $row[] = drupal_render($form['services'][$key][$varlinktext]);
      $row[] = drupal_render($form['services'][$key][$varid]);

      // Now, render the weight row.
      $form['services'][$key][$varweight]['#attributes']['class'][] = 'reach-us-weight';
      $row[] = drupal_render($form['services'][$key][$varweight]);

      // Add the new row to our collection of rows, and make draggable.
      $rows[] = array(
        'data' => $row,
        'class' => array('draggable'),
      );
    }
  }

  // Render a list of header titles, and our array of rows, into a table.
  $header = array(t('Service name'));
  $header[] = t('Link Text');
  $header[] = t('Service ID');
  $header[] = '';

  $output = '';
  if (count($rows)) {
    $output .= theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'attributes' => array('id' => 'reach-us-weighted-form')));
  }
  $output .= drupal_render_children($form);
  drupal_add_tabledrag('reach-us-weighted-form', 'order', 'self', 'reach-us-weight');
  return $output;
}

/**
 * Validation handler for the follow_links_form.
 */
function reach_us_form_validate($form, &$form_state) {
  $services = reach_us_services();
  foreach ($services as $key => $service) {
    // Take only services id set and validation function available.
    $varid = 'reach_us_serv_' . $key . '_id';
    // Validate the input if validator is present.
    if (trim($form_state['values'][$varid]) && isset($services[$key]['valid'])) {
      $varlinktext = 'reach_us_serv_' . $key . '_linktext';
      // Set error if a service not valid.
      $idvalid = call_user_func($services[$key]['valid'], $form_state['values'][$varid]);
      if ($idvalid) {
        form_set_error($varid, filter_xss($idvalid));
      }
      else {
        if (!trim($form_state['values'][$varlinktext])) {
          $errormsg = t('Fill the Link text for @label', array('@label' => $services[$key]['label']));
          form_set_error($varlinktext, $errormsg);
        }
      }
    }
  }
}
