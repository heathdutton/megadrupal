<?php
/**
 * @file
 * Page and form generation for the addressfield_hu module.
 */

function addressfield_hu_admin() {

  $header = array(
    array(
      'data' => t('Postal code'),
      'field' => 'a.postal_code',
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
  if (user_access('edit hungarian address') || user_access('delete hungarian address')) {
    $header[] = array(
      'data' => t('Links'),
    );
  }

  $query = db_select('addressfield_hu', 'a')->extend('PagerDefault')->extend('TableSort');
  $query->fields('a', array('postal_code', 'locality', 'administrative_area'));

  $results = $query
    ->limit(50)
    ->orderByHeader($header)
    ->execute();

  $rows = array();
  foreach ($results as $result) {
    $links = array();
    if (user_access('edit hungarian address')) {
      $links[] = l(t('Edit'), "admin/structure/addressfield_hu/{$result->postal_code}/edit");
    }

    if (user_access('delete hungarian address')) {
      $links[] = l(t('Delete'), "admin/structure/addressfield_hu/{$result->postal_code}/delete");
    }
    $rows[] = array('data' =>
      array(
      // Table cells
        check_plain($result->postal_code),
        check_plain($result->locality),
        check_plain(addressfield_hu_get_county_name($result->administrative_area)),
        implode(' ', $links),
      ),
    );
  }

  $build = array();

  if (user_access('create hungarian address')) {
    $build['add_form'] = drupal_get_form('addressfield_hu_edit_form');
  }

  $build['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No addresse available.'),
  );
  $build['pager'] = array('#theme' => 'pager');

  //dsm(get_defined_vars());
  return $build;
}

function addressfield_hu_edit($postal_code) {
  $address = addressfield_hu_get_address_by_postal_code($postal_code);
  $build = drupal_get_form('addressfield_hu_edit_form', $address);
  dsm(get_defined_vars());
  return $build;
}

function addressfield_hu_delete($postal_code) {
  $build = drupal_get_form('addressfield_hu_delete_form', $postal_code);
  //dsm(get_defined_vars());
  return $build;
}

function addressfield_hu_delete_form($form, &$form_state, $postal_code) {
  $form['postal_code'] = array(
    '#type' => 'value',
    '#value' => $postal_code,
  );
  return confirm_form($form, t('Are you sure to delete this address?'), 'admin/structure/addressfield_hu', t('The address with postal code %postal_code will be deleted. This action cannot be undone.', array('%postal_code' => $postal_code)), t('Delete'), t('Cancel'));
}

function addressfield_hu_delete_form_submit($form, &$form_state) {
  addressfield_hu_address_delete($form_state['values']['postal_code']);
  drupal_set_message(t('The address with postal code %postal_code have been deleted.', array('%postal_code' => $form_state['values']['postal_code'])));
  watchdog('addressfield_hu', 'The address with postal code %postal_code have been deleted.', array('%postal_code' => $form_state['values']['postal_code']));

  $form_state['redirect'] = 'admin/structure/addressfield_hu';
  return;
}

function addressfield_hu_edit_form($form, &$form_state, $address = NULL) {
  $form = array(
    'postal_code' => array(
      '#title' => t('Postal code'),
      '#type' => 'textfield',
      '#requiered' => TRUE,
    ),
    'locality' => array(
      '#title' => t('Locality'),
      '#type' => 'textfield',
      '#requiered' => TRUE,
    ),
    'administrative_area' => array(
      '#title' => t('County'),
      '#type' => 'select',
      '#options' => addressfield_hu_get_counties(),
      '#requiered' => TRUE,
    ),
    'is_edit' => array(
      '#type' => 'value',
      '#value' => 'add',
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save new address'),
    ),
    '#validate' => array('addressfield_hu_addresse_form_validate'),
  );
  if (!is_null($address)) {
    $form['postal_code']['#default_value'] = $address['postal_code'];
    $form['locality']['#default_value'] = $address['locality'];
    $form['administrative_area']['#default_value'] = $address['administrative_area'];
    $form['submit']['#value'] = t('Save address');
    $form['is_edit'] = array(
      '#type' => 'value',
      '#value' => 'edit',
    );
  }
  return $form;
}

function addressfield_hu_addresse_form_validate($form, &$form_state) {

}

function addressfield_hu_edit_form_submit($form, &$form_state) {
  $address = array(
    'postal_code' => $form_state['values']['postal_code'],
    'locality' => $form_state['values']['locality'],
    'administrative_area' => $form_state['values']['administrative_area'],
  );
  if (addressfield_hu_address_save($address)) {
    if ($form_state['values']['is_edit'] == 'add') {
      drupal_set_message(t('New address with postal code %postal_code have been added.', array('%postal_code' => $form_state['values']['postal_code'])));
    }
    if ($form_state['values']['is_edit'] == 'edit') {
      drupal_set_message(t('The address with postal code %postal_code have been saved.', array('%postal_code' => $form_state['values']['postal_code'])));
    }
  };

  $form_state['redirect'] = 'admin/structure/addressfield_hu';
  return;
}

function addressfield_hu_autocomplete($county_id, $string = '') {
  $matches = array();
  if ($string) {
    $result = db_select(ADDRESSFIELD_HU_BASE_TABLE, 'a')
    ->fields('a', array('locality'))
     ->condition('a.locality', db_like($string) . '%', 'LIKE')
     ->range(0, 25);
    if ($county_id != 'null') {
      $result->condition('administrative_area', $county_id);
    }
    $result = $result->execute();

    foreach ($result as $user) {
      $matches[$user->locality] = check_plain($user->locality);
    }
  }
  drupal_json_output($matches);
}

function addressfield_hu_csv_debug() {
  $build = array();
  $build['form'] = drupal_get_form('addressfield_hu_csv_import');
  if (isset($_GET['debug'])) {
    addressfield_hu_fetch_csv(TRUE);
  }
  return $build;
}

function addressfield_hu_csv_import($form, &$form_state) {
  $menuitem = menu_get_item();
  if (!isset($_GET['submit'])) {
    $form = array(
      'markup' => array(
        '#markup' => t('Click to import csv data provided by this module.') . '</br>',
      ),
      'submit_debug' => array(
        '#type' => 'submit',
        '#value' => t('Check CSV'),
        '#submit' => array('addressfield_hu_csv_debug_submit'),
      ),
      'submit_no_confirm' => array(
        '#type' => 'submit',
        '#value' => t('Import'),
        '#submit' => array('addressfield_hu_no_confirm_submit'),
      )
    );

    return $form;
  }
  else {
    $form = array(
      'markup' => array(
        '#markup' => t('Click Confirm button will <strong>delete all exisiting address record</strong>!') . ' ',
      ),
    );
    return confirm_form($form, t('Are you sure to start import?'), 'admin/structure/addressfield_hu');
  }
}

function addressfield_hu_no_confirm_submit($form, &$form_state) {
  $menuitem = menu_get_item();
  global $base_url;
  $url = $base_url . url($menuitem['path'], array('query' => array('submit' => '1')));
  $form_state['redirect'] = $url;
}

function addressfield_hu_csv_debug_submit($form, &$form_state) {
  $menuitem = menu_get_item();
  global $base_url;
  $url = $base_url . url($menuitem['path'], array('query' => array('debug' => '1')));
  $form_state['redirect'] = $url;
}

function addressfield_hu_csv_import_submit($form, &$form_state) {
  if (isset($_GET['submit'])) {
    $cache = array();
    $error = array();
    $count = 0;
    db_truncate(ADDRESSFIELD_HU_BASE_TABLE)->execute();
    foreach (addressfield_hu_fetch_csv() as $record) {
      $index = $record['postal_code'] . '-' . $record['locality'];
      if (!array_key_exists($index, $cache)) {
        drupal_write_record(ADDRESSFIELD_HU_BASE_TABLE, $record);
        $count++;
        $cache[$index] = 1;
      }
      else {
        $error[] = implode(', ', $record);
      }
    }
    drupal_set_message(t('@count address was successful imported.', array('@count' => $count)));
    if (!empty($error)) {
      drupal_set_message(theme('item_list', array('type' => 'ol', 'title' => t('The following addresses was not added because of duplicated postal code'), 'items' => $error)), 'warning');
    }
  }
}