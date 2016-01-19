<?php

/**
 * @file
 * Administrative pages for the Host Redirect module.
 */

/**
 * Admin form for configuring Domain 301 Redirect.
 */
function hostredir_form_admin($form_state) {
  $redirects = variable_get('hostredir_redirects', array());
  $form['hostredir_from']['#tree'] = TRUE;
  $form['hostredir_to']['#tree'] = TRUE;
  $i = 0;
  foreach ($redirects as $from => $to) {
    $form['hostredir_from'][$i] = array(
      '#type' => 'textfield',
      '#default_value' => $from,
    );
    $form['hostredir_to'][$i] = array(
      '#type' => 'textfield',
      '#default_value' => $to,
    );
    $i++;
  }
  $form['hostredir_from'][$i] = array(
    '#type' => 'textfield',
  );
  $form['hostredir_to'][$i] = array(
    '#type' => 'textfield',
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Checks if valid redirections were given.
 */
function hostredir_form_admin_validate($form, &$form_state) {
  foreach ($form_state['values']['hostredir_from'] as $i => $from) {
    $to = &$form_state['values']['hostredir_to'][$i];
    // Do not check empty lines.
    if (empty($from) && empty($to)) {
      continue;
    }
    if (empty($from)) {
      form_set_error('hostredir_from][' . $i, t('Both from and to hostnames must be provided for a working redirection.'));
    }
    if (empty($to)) {
      form_set_error('hostredir_to][' . $i, t('Both from and to hostnames must be provided for a working redirection.'));
    }
    // Accept only alphanumeric characters, dash and full stop in hostnames.
    if (!preg_match('/^[a-z0-9.-]*$/', $from)) {
      form_set_error('hostredir_from][' . $i, t('Invalid hostname.'));
    }
    if (!preg_match('/^[a-z0-9.-]*$/', $to)) {
      form_set_error('hostredir_to][' . $i, t('Invalid hostname.'));
    }
  }
}

/**
 * Stores valid redirections.
 */
function hostredir_form_admin_submit($form, &$form_state) {
  $redirects = array();
  foreach ($form_state['values']['hostredir_from'] as $i => $from) {
    $to = &$form_state['values']['hostredir_to'][$i];
    // Do not store empty lines.
    if (empty($from) && empty($to)) {
      continue;
    }
    $redirects[$from] = $to;
  }
  variable_set('hostredir_redirects', $redirects);
}

/**
 * Renders the admin form as an HTML table.
 */
function theme_hostredir_form_admin($variables) {
  $form = &$variables['form'];
  $rows = array();
  foreach (element_children($form['hostredir_from']) as $i) {
    $rows[] = array(
      drupal_render($form['hostredir_from'][$i]),
      drupal_render($form['hostredir_to'][$i]),
    );
  }
  $header = array(
    t('Redirect from host'),
    t('Redirect to host'),
  );
  return theme('table', array('header' => $header, 'rows' => $rows)) . drupal_render_children($form);
}
