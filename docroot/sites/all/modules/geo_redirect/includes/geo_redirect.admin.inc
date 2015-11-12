<?php

/**
 * @file
 * Page callbacks for GeoRedirect module.
 */

/**
 * Geo redirect listing page.
 */
function geo_redirect_list() {
  $output = '';

  $destination = drupal_get_destination();

  $result = db_select('geo_redirect_list', 'gr')
    ->fields('gr', array('gid', 'country_code', 'redirect_url'))
    ->execute();

  $rows = array();
  $header = array('Country', 'Redirect URL', 'Operations');
  $country_names = geo_redirect_country_names();
  foreach ($result as $item) {
    $row = array();
    $row[] = array(
      'data' => $country_names[$item->country_code],
    );
    $row[] = array(
      'data' => l($item->redirect_url, $item->redirect_url),
    );

    $operations = array();
    $operations['edit'] = array(
      'title' => t('Edit'),
      'href' => 'admin/config/search/geo-redirect/edit/' . $item->gid,
      'query' => $destination,
    );
    $operations['delete'] = array(
      'title' => t('Delete'),
      'href' => 'admin/config/search/geo-redirect/delete/' . $item->gid,
      'query' => $destination,
    );
    $row['operations'] = array(
      'data' => array(
        '#theme' => 'links',
        '#links' => $operations,
        '#attributes' => array('class' => array('links', 'inline', 'nowrap')),
      ),
    );
    $rows[] = $row;
  }
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No geo redirect URLs available.')));
  return $output;
}

/**
 * Edit geo redirect URL.
 */
function geo_redirect_edit_form($form, &$form_state, $gid = NULL) {

  if (isset($gid)) {
    $geo_redirect = geo_redirect_load($gid);
    if (!($geo_redirect)) {
      drupal_not_found();
      exit();
    }
  }
  else {
    $geo_redirect = new stdClass();
    // Merge default values for geo redirect object.
    geo_redirect_object_prepare($geo_redirect);
  }

  $form['gid'] = array('#type' => 'hidden', '#value' => $geo_redirect->gid);
  $form['is_new'] = array('#type' => 'hidden', '#value' => empty($geo_redirect->gid));

  $form['country_code'] = array(
    '#type' => 'select',
    '#title' => t('Select country'),
    '#options' => geo_redirect_country_names(),
    '#description' => t('Select the country for which you want the site to be redirected.'),
    '#required' => TRUE,
    '#default_value' => $geo_redirect->country_code,
  );
  $form['redirect_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Enter URL'),
    '#required' => TRUE,
    '#description' => t('Enter the URL where you want to redirect the user from above country e.g. (@example).', array('@example' => 'http://www.example.com')),
    '#required' => TRUE,
    '#default_value' => $geo_redirect->redirect_url,
  );
  $form['is_user_allowed'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow <em>@user</em> path.', array('@user' => '/user')),
    '#description' => t('<p>If you check this box, user from above country can access the path <em>/user</em>.</p> <p><strong>It is strongly recommanded that you should check this box, if you are trying to redirect users from your country.</strong></p>'),
    '#default_value' => $geo_redirect->is_user_allowed,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['#validate'][] = 'geo_redirect_edit_form_validate';
  return $form;
}

/**
 * Validation function for geo redirect form.
 */
function geo_redirect_edit_form_validate($form, &$form_state) {
  $geo_redirect_url = $form_state['values']['redirect_url'];
  if (!valid_url($geo_redirect_url, TRUE)) {
    form_set_error('redirect_url', t('%url is not valid url.', array('%url' => $geo_redirect_url)));
  }
  // Do not allow to add redirect for same country (only for new redirects).
  if ($form_state['values']['is_new']) {
    $country_code = $form_state['values']['country_code'];
    if ($exists = geo_redirect_load_by_country_code($country_code)) {
      $countries = geo_redirect_country_names();
      form_set_error('country_code', t('Redirect for %country already exists.', array('%country' => $countries[$country_code])));
    }
  }
}

/**
 * Submit Handler for Geo redirect form.
 */
function geo_redirect_edit_form_submit($form, &$form_state) {
  form_state_values_clean($form_state);
  $geo_redirect = (object) $form_state['values'];
  geo_redirect_save($geo_redirect);
  $form_state['redirect'] = 'admin/config/search/geo-redirect';
  drupal_set_message(t('Geo redirect has been saved successfully.'), 'status');
}

/**
 * Delete Geo redirect url.
 */
function geo_redirect_delete_form($form, &$form_state, $gid) {
  $geo_redirect = geo_redirect_load($gid);

  $form['gid'] = array('#type' => 'value', '#value' => $gid);
  $countries = geo_redirect_country_names();
  return confirm_form($form, t('Are you sure you want to delete redirect URL for <em>%country</em>?', array('%country' => $countries[$geo_redirect->country_code])), 'admin/config/search/geo-redirect/list');
}

/**
 * Submit handler for geo redirect url delete form.
 */
function geo_redirect_delete_form_submit($form, &$form_state) {
  geo_redirect_delete($form_state['values']['gid']);
  drupal_set_message(t('The geo redirect has been deleted.'));
  $form_state['redirect'] = 'admin/config/search/geo-redirect/list';
}

/**
 * Settings form for geo redirect.
 */
function geo_redirect_settings_form($form, &$form_state) {
  $form = array();
  // Enable geo redirect.
  $form['geo_redirect_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable geo redirect'),
    '#default_value' => variable_get('geo_redirect_enabled', TRUE),
    '#description' => t('Check this box if you want to enable geo redirection. Uncheck to disable it.'),
  );
  // Log redirect entries.
  $form['geo_redirect_log'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log geo redirections'),
    '#default_value' => variable_get('geo_redirect_log', FALSE),
    '#description' => t('Log entry when user is redirected.'),
  );
  // Allow debugging.
  $form['geo_redirect_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable debugging'),
    '#default_value' => variable_get('geo_redirect_debug', FALSE),
    '#description' => t('Enable debugging for Geo Redirect. You can specify IP address with URL to test with. E.g. http://yoursite.com/?grip_debug=86.30.200.145'),
  );
  // User roles.
  $form['geo_redirect_user_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#options' => user_roles(),
    '#default_value' => variable_get('geo_redirect_user_roles', array(DRUPAL_ANONYMOUS_RID => DRUPAL_ANONYMOUS_RID)),
    '#description' => t('Select user roles for which redirection is enabled.'),
  );
  return system_settings_form($form);
}
