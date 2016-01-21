<?php

/**
 * @file
 * Page and form generation for the addressfield_ro module.
 */

/**
 * Page callback: Administer addressfield_ro addresses.
 *
 * Display an administration list of addresses and the create address form.
 */
function addressfield_ro_admin() {
  $header = array(
    array(
      'data' => t('ID'),
      'field' => 'a.id',
      'sort' => 'inc',
    ),
    array(
      'data' => t('Locality'),
      'field' => 'a.locality',
    ),
    array(
      'data' => t('County'),
      'field' => 'a.administrative_area',
    ),
  );

  // Admin links.
  if (addressfield_ro_access(array('edit', 'delete'))) {
    $header[] = array(
      'data' => t('Links'),
    );
  }

  // Get addresses from the database.
  $query = db_select('addressfield_ro', 'a')->extend('PagerDefault')->extend('TableSort');
  $query->fields('a', array('id', 'locality', 'administrative_area'));
  $results = $query
      ->limit(50)
      ->orderByHeader($header)
      ->execute();

  $basepath = addressfield_ro_basepath();
  $rows = array();
  foreach ($results as $result) {
    $links = array();
    if (user_access('edit romanian address')) {
      $links[] = l(t('Edit'), "{$basepath}/{$result->id}/edit");
    }

    if (user_access('delete romanian address')) {
      $links[] = l(t('Delete'), "{$basepath}/{$result->id}/delete");
    }
    $rows[] = array(
      'data' => array(
        check_plain($result->id),
        check_plain($result->locality),
        check_plain(addressfield_ro_get_county_name($result->administrative_area)),
        implode(' | ', $links),
      ),
    );
  }

  $build = array();
  if (addressfield_ro_access(array('create'))) {
    $build['form'] = array(
      '#type' => 'fieldset',
      '#title' => t('Create a new address'),
    );
    $build['form']['add_form'] = drupal_get_form('addressfield_ro_edit_form');
  }

  $build['table'] = array(
    '#caption' => t('Address list'),
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#sticky' => TRUE,
    '#empty' => t('No addresses available.'),
  );
  $build['pager'] = array('#theme' => 'pager');

  return $build;
}

/**
 * Form builder: 'addressfield_ro_delete_form'.
 */
function addressfield_ro_delete_form($form, &$form_state, $id = NULL) {
  if (!$id || !is_numeric($id)) {
    drupal_not_found();
  }
  $form['id'] = array(
    '#type' => 'value',
    '#value' => $id,
  );
  return confirm_form($form,
      t('Are you sure to delete this address?'),
      addressfield_ro_basepath(),
      t('The address with ID %id will be deleted. This action cannot be undone.', array('%id' => $id)), t('Delete'), t('Cancel')
  );
}

/**
 * Submit handler for 'addressfield_ro_delete_form'.
 *
 * @see: addressfield_ro_delete_form()
 */
function addressfield_ro_delete_form_submit($form, &$form_state) {
  addressfield_ro_address_delete($form_state['values']['id']);
  drupal_set_message(t('The address with ID %id have been deleted.', array('%id' => $form_state['values']['id'])));
  watchdog('addressfield_ro', 'The address with ID %id have been deleted.', array('%id' => $form_state['values']['id']));

  // @TODO: get the destination.
  $form_state['redirect'] = addressfield_ro_basepath();
}

/**
 * Form builder: 'addressfield_ro_edit_form'.
 */
function addressfield_ro_edit_form($form, &$form_state, $address_id = NULL) {
  $form = array(
    'id' => array(
      '#title' => t('ID'),
      '#type' => 'value',
      '#value' => $address_id,
    ),
    'locality' => array(
      '#title' => t('Locality'),
      '#type' => 'textfield',
      '#required' => TRUE,
    ),
    'administrative_area' => array(
      '#title' => t('County'),
      '#type' => 'select',
      '#options' => addressfield_ro_get_counties(),
      '#required' => TRUE,
    ),
    'is_edit' => array(
      '#type' => 'value',
      '#value' => 'add',
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save new address'),
    ),
  );

  // Add values when editing an existing address.
  $address = addressfield_ro_address_load($address_id);
  if ($address) {
    $form['locality']['#default_value'] = $address['locality'];
    $form['administrative_area']['#default_value'] = $address['administrative_area'];
    $form['submit']['#value'] = t('Save address');
    $form['is_edit']['#value'] = 'edit';
  }

  return $form;
}

/**
 * Submit handler for 'addressfield_ro_edit_form'.
 *
 * @see: addressfield_ro_edit_form()
 */
function addressfield_ro_edit_form_submit($form, &$form_state) {
  $address = array(
    'id' => $form_state['values']['id'],
    'locality' => $form_state['values']['locality'],
    'administrative_area' => $form_state['values']['administrative_area'],
  );
  if ($save = addressfield_ro_address_save($address)) {
    if ($form_state['values']['is_edit'] == 'add') {
      drupal_set_message(t('New address with ID %id have been added.', array('%id' => $save['id'])));
    }
    if ($form_state['values']['is_edit'] == 'edit') {
      drupal_set_message(t('The address with ID %id have been saved.', array('%id' => $save['id'])));
    }
  }
  else {
    drupal_set_message(t('There was a problem saving address ID %id.', array('%id' => $save['id'])));
  }

  $form_state['redirect'] = addressfield_ro_basepath();
}

/**
 * Autocomplete callback for addressfield_ro.
 *
 * @param string $county_id
 *   The country id.
 * @param string $locality
 *   The locality to search for.
 */
function addressfield_ro_autocomplete($county_id, $locality = '') {
  $matches = array();
  if ($locality) {
    $result = db_select(ADDRESSFIELD_RO_BASE_TABLE, 'a')
        ->fields('a', array('locality'))
        ->condition('a.locality', db_like($locality) . '%', 'LIKE')
        ->range(0, 25);
    if ($county_id != 'null') {
      $result->condition('administrative_area', $county_id);
    }
    $result = $result->execute();

    foreach ($result as $address) {
      $matches[$address->locality] = check_plain($address->locality);
    }
  }

  drupal_json_output($matches);
}

/**
 * Get a county name by own id.
 *
 * @param string $county_id
 *   County ID.
 *
 * @return string
 *   County name.
 */
function addressfield_ro_get_county_name($county_id) {
  $counties = addressfield_ro_get_counties();
  if (isset($counties[$county_id])) {
    return $counties[$county_id];
  }

  return '';
}
