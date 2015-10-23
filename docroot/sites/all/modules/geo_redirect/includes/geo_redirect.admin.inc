<?php

/**
 * @file
 * Page callbacks for GeoRedirect module.
 */

/**
 * Settings form for GeoRedirect.
 */
function geo_redirect_add($form, &$form_state) {
  drupal_set_title(t('Add Redirect URL'));
  $country_names = variable_get('geo_redirect_countries');
  $array_sort = asort($country_names);
  $form['geo_redirect_country'] = array(
    '#type' => 'select',
    '#title' => t('Select country'),
    '#options' => isset($country_names) ? $country_names : array(),
    '#description' => t('Select the country for which you want the site to be redirected.'),
    '#required' => TRUE,
  );
  $form['geo_redirect_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Enter URL'),
    '#required' => TRUE,
    '#description' => t('Enter the URL where you want to redirect the user from above country e.g. (@example).', array('@example' => 'http://www.example.com')),
    '#required' => TRUE,
  );
  $form['geo_redirect_user_allowed'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow <em>@user</em> path.', array('@user' => '/user')),
    '#description' => t('<p>If you check this box, user from above country can access the path <em>/user</em>.</p> <p><strong>It is strongly recommanded that you should check this box, if you are trying to redirect users from your country.</strong></p>'),
    '#default_value' => 1,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
  );
  $form['#validate'][] = 'geo_redirect_add_validate';
  return $form;
}

/**
 * Validation function for GeoRedirect Settings form.
 */
function geo_redirect_add_validate($form, &$form_state) {
  $geo_redirect_url = $form_state['values']['geo_redirect_url'];
  if (!valid_url($geo_redirect_url, TRUE)) {
    form_set_error('geo_redirect_url', t('Please enter valid URL.'));
  }
}

/**
 * Submit Handler for GeoRedirect Settings form.
 */
function geo_redirect_add_submit($form, &$form_state) {
  $country_names = variable_get('geo_redirect_countries');
  $id = db_insert('geo_redirect_list')
    ->fields(array(
      'country_code' => $form_state['values']['geo_redirect_country'],
      'redirect_url' => $form_state['values']['geo_redirect_url'],
      'is_user_allowed' => $form_state['values']['geo_redirect_user_allowed'],
      )
    )
    ->execute();
  unset($country_names[$form_state['values']['geo_redirect_country']]);
  variable_set('geo_redirect_countries', $country_names);
  $form_state['redirect'] = 'admin/config/geo-redirect/list';
  drupal_set_message(t('Added to Redirect list successfully.'), 'status');
}

/**
 * GeoRedirect Listing.
 */
function geo_redirect_listing() {
  $output = '';
  $output .= '<p>' . l(t('Add Redirect URL'), 'admin/config/geo-redirect') . '</p>';
  $result = db_select('geo_redirect_list', 'gr')
    ->fields('gr', array('gid', 'country_code', 'redirect_url'))
    ->execute();
  $number_of_rows = $result->rowCount();
  if (!$number_of_rows) {
    $output .= '<p>' . t('No Redirect URLs added yet.') . '</p>';
    return $output;
  }

  $rows = array();
  $header = array('Country', 'Redirect URL', 'Operations');
  $country_names = _geo_redirect_get_country_names();
  foreach ($result as $item) {
    $rows[] = array(
      array('data' => $country_names[$item->country_code]),
      array('data' => l($item->redirect_url, $item->redirect_url)),
      array('data' => l(t('Edit'), 'admin/config/geo-redirect/edit/' . $item->gid) . '&nbsp&nbsp&nbsp' . l(t('Delete'), 'admin/config/geo-redirect/delete/' . $item->gid)),
    );
  }
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  return $output;
}

/**
 * Edit GeoRedirect URL.
 */
function geo_redirect_edit($form, &$form_state, $gid) {
  $result = db_select('geo_redirect_list', 'gr')
    ->fields('gr', array('gid', 'country_code', 'redirect_url', 'is_user_allowed'))
    ->condition('gid', $gid, '=')
    ->execute();
  $number_of_rows = $result->rowCount();
  if (!$number_of_rows) {
    drupal_not_found();
    exit();
  }
  foreach ($result as $item) {
    $country_code = $item->country_code;
    $redirect_url = $item->redirect_url;
    $is_user_allowed = $item->is_user_allowed;
  }
  $form_state['storage']['value'] = $gid;
  $all_country_names = _geo_redirect_get_country_names();
  $redirect_country_names = variable_get('geo_redirect_countries');
  $redirect_country_names[$country_code] = $all_country_names[$country_code];
  $array_sort = asort($redirect_country_names);
  $form_state['storage']['country_array'] = $redirect_country_names;
  $form['geo_redirect_edit_country'] = array(
    '#type' => 'select',
    '#title' => t('Select country'),
    '#options' => isset($redirect_country_names) ? $redirect_country_names : array(),
    '#description' => t('Select the country for which you want the site to be redirected.'),
    '#required' => TRUE,
    '#default_value' => $country_code,
  );
  $form['geo_redirect_edit_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Enter URL'),
    '#required' => TRUE,
    '#description' => t('Enter the URL where you want to redirect the user from above country e.g. (@example).', array('@example' => 'http://www.example.com')),
    '#required' => TRUE,
    '#default_value' => $redirect_url,
  );
  $form['geo_redirect_edit_user_allowed'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow <em>@user</em> path.', array('@user' => '/user')),
    '#description' => t('<p>If you check this box, user from above country can access the path <em>/user</em>.</p> <p><strong>It is strongly recommanded that you should check this box, if you are trying to redirect users from your country.</strong></p>'),
    '#default_value' => $is_user_allowed,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );
  $form['#validate'][] = 'geo_redirect_edit_validate';
  return $form;
}

/**
 * Validation function for GeoRedirect Edit form.
 */
function geo_redirect_edit_validate($form, &$form_state) {
  $geo_redirect_url = $form_state['values']['geo_redirect_edit_url'];
  if (!valid_url($geo_redirect_url, TRUE)) {
    form_set_error('geo_redirect_edit_url', t('Please enter valid URL.'));
  }
}

/**
 * Submit Handler for GeoRedirect Edit form.
 */
function geo_redirect_edit_submit($form, &$form_state) {
  $gid = $form_state['storage']['value'];
  $country_names = $form_state['storage']['country_array'];
  $id = db_update('geo_redirect_list')
    ->fields(array(
      'country_code' => $form_state['values']['geo_redirect_edit_country'],
      'redirect_url' => $form_state['values']['geo_redirect_edit_url'],
      'is_user_allowed' => $form_state['values']['geo_redirect_edit_user_allowed'],
      )
    )
    ->condition('gid', $gid, '=')
    ->execute();
  unset($country_names[$form_state['values']['geo_redirect_edit_country']]);
  variable_set('geo_redirect_countries', $country_names);
  $form_state['redirect'] = 'admin/config/geo-redirect/list';
  drupal_set_message(t('Redirect list updated successfully.'), 'status');
}

/**
 * Delete GeoRedirect URL.
 */
function geo_redirect_delete($form, &$form_state, $gid) {
  $form_state['storage']['value'] = $gid;
  $result = db_select('geo_redirect_list', 'gr')
    ->fields('gr', array('country_code'))
    ->condition('gid', $gid, '=')
    ->execute();
  $number_of_rows = $result->rowCount();
  if (!$number_of_rows) {
    drupal_not_found();
    exit;
  }
  $all_country_names = _geo_redirect_get_country_names();
  $form_state['storage']['country_names'] = $all_country_names;
  foreach ($result as $item) {
    $country_code = $item->country_code;
  }
  $current_country = $all_country_names[$country_code];
  $form_state['storage']['country_code'] = $country_code;
  return confirm_form($form, t('Are you sure you want to delete Redirect URL for <em>%country</em>?', array('%country' => $current_country)), 'admin/config/geo-redirect/list');
}

/**
 * Submit Handler for Delete GeoRedirect URL.
 */
function geo_redirect_delete_submit($form, &$form_state) {
  $gid = $form_state['storage']['value'];
  $all_country_names = $form_state['storage']['country_names'];
  $country_code = $form_state['storage']['country_code'];
  $redirect_country_names = variable_get('geo_redirect_countries');
  $redirect_country_names[$country_code] = $all_country_names[$country_code];
  $array_sort = asort($redirect_country_names);
  $result = db_delete('geo_redirect_list')
    ->condition('gid', $gid, '=')
    ->execute();
  variable_set('geo_redirect_countries', $redirect_country_names);
  $form_state['redirect'] = 'admin/config/geo-redirect/list';
}
