<?php

/**
 * @file
 * Settings file
 */

/**
 * Generate the admin form.
 */
function searchcloud_block_admin() {
  $form = array();
  $form['searchcloud_block_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of terms shown'),
    '#size' => 6,
    '#maxlength' => 6,
    '#default_value' => variable_get('searchcloud_block_count', 10),
  );
  $form['searchcloud_block_stylecount'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of styles to use'),
    '#size' => 6,
    '#maxlength' => 6,
    '#default_value' => variable_get('searchcloud_block_stylecount', 5),
  );
  $form['searchcloud_block_overridepath'] = array(
    '#type' => 'textfield',
    '#title' => t('Path'),
    '#description' => t('Override the search path prefix (i.e. when a view is used as search page target)'),
    '#size' => 30,
    '#default_value' => variable_get('searchcloud_block_overridepath', ''),
  );
  $form['searchcloud_block_useparam'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use a parameter for the searchterm'),
    '#default_value' => variable_get('searchcloud_block_useparam', 0),
  );
  $form['searchcloud_block_paramname'] = array(
    '#type' => 'textfield',
    '#title' => t('Parameter name'),
    '#size' => 30,
    '#default_value' => variable_get('searchcloud_block_paramname', ''),
    '#states' => array(
      'invisible' => array(
        ':input[name="searchcloud_block_useparam"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['searchcloud_block_usepaths'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show terms from current path'),
    '#description' => t('This option makes it possible to only show the search terms searched for on the current path. Add the paths to use in the next field. If the paths do not match it shows the normal view.'),
    '#default_value' => variable_get('searchcloud_block_usepaths', 0),
  );
  $form['searchcloud_block_usepaths_pathlist'] = array(
    '#type' => 'textarea',
    '#title' => t('The paths to show the filtered searchwords'),
    '#description' => t('Enter a comma seperated list of paths to use the searchword filter on.'),
    '#default_value' => variable_get('searchcloud_block_usepaths_pathlist', ''),
  );

  $form['searchcloud_table'] = array(
    '#markup' => searchcloud_block_admin_table(),
  );
  if (user_access('searchcloud reset')) {
    $form['searchcloud_reset'] = array(
      '#markup' => '<p>' . l(t("Reset searchcloud to remove counts from terms. The added settings (hide, etc.) per term are saved. This can't be undone."), 'admin/config/search/searchcloud_block/reset') . '</p>',
    );
  }
  if (user_access('searchcloud truncate')) {
    $form['searchcloud_truncate'] = array(
      '#markup' => '<p>' . l(t("Do a full truncate of the searchcloud table. This can't be undone."), 'admin/config/search/searchcloud_block/truncate') . '</p>',
    );
  }
  return system_settings_form($form);
}

function searchcloud_block_admin_reset() {
  db_update('searchcloud_block_count')
  ->fields(array('count' => 0))
  ->execute();
  drupal_set_message(t('All terms are reset to 0 (zero)'));
  drupal_goto('admin/config/search/searchcloud_block');
}

function searchcloud_block_admin_truncate() {
  db_truncate('searchcloud_block_count')->execute();
  drupal_set_message(t('The searchcloud table is emptied.'));
  drupal_goto('admin/config/search/searchcloud_block');
}

function searchcloud_block_admin_table() {
  $header = array(
    t('Term'),
    t('Count'),
    t('Edit term'),
    t('Hide term'),
  );
  $rows = array();
  $result = searchcloud_block_get_result(TRUE, FALSE, FALSE, 'DESC');
  foreach ($result as $row) {
    if ($row->hide == 1) {
      $hide = l(t('Show term in searchcloud'), 'admin/config/search/searchcloud_block/hide/' . $row->keyword . '/0');
    }
    else {
      $hide = l(t('Hide term from searchcloud'), 'admin/config/search/searchcloud_block/hide/' . $row->keyword . '/1');
    }
    $rows[] = array(
      $row->keyword,
      $row->count,
      l(t('Edit'), 'admin/config/search/searchcloud_block/edit/' . $row->keyword),
      $hide,
    );
  }
  return theme('table', array('header' => $header, 'rows' => $rows));
}

function searchcloud_block_admin_hide() {
  $args = arg();
  db_update('searchcloud_block_count')
  ->fields(array('hide' => $args[6]))
  ->condition('keyword', $args[5])
  ->execute();
  drupal_set_message(t('Term @term hidden from searchcloud', array('@term' => $args[5])));
  drupal_goto('admin/config/search/searchcloud_block');
  exit();
}

function searchcloud_block_admin_add() {
  $form = array();
  $form['term'] = array(
    '#type' => 'textfield',
    '#title' => t('Searchterm'),
    '#required' => TRUE,
  );
  $form['count'] = array(
    '#type' => 'textfield',
    '#title' => t('Count'),
    '#size' => 6,
    '#maxlength' => 6,
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

function searchcloud_block_admin_add_validate($form, &$form_state) {
  searchcloud_block_admin_add_validate_forms($form_state['values']);
}

function searchcloud_block_admin_add_validate_forms($values) {
  $count = $values['count'];
  if (!is_numeric($count)) {
    form_set_error('count', t('The count must be a number, now it is not.'));
  }
}

function searchcloud_block_admin_add_submit($form, &$form_state) {
  $data = array(
    'keyword' => drupal_strtolower($form_state['values']['term']),
    'count' => $form_state['values']['count'],
  );
  searchcloud_block_admin_add_term($data);
  exit();
}

function searchcloud_block_admin_add_term($fields) {
  $id = db_insert('searchcloud_block_count')
  ->fields($fields)->execute();
  drupal_set_message(t('Term @term added to the searchcloud', array('@term' => $fields['keyword'])));
  drupal_goto('admin/config/search/searchcloud_block');
  exit();
}

function searchcloud_block_admin_edit() {
  $form = array();
  $term = arg(5);
  $result = searchcloud_block_get_result(TRUE, $term);
  $rowCount = $result->rowCount();
  if (!empty($rowCount)) {
    foreach ($result as $record) {
      $form['term'] = array(
        '#type' => 'textfield',
        '#title' => t('Searchterm'),
        '#required' => TRUE,
        '#default_value' => $record->keyword,
      );
      $form['orig_term'] = array(
        '#type' => 'hidden',
        '#default_value' => $record->keyword,
      );
      $form['count'] = array(
        '#type' => 'textfield',
        '#title' => t('Count'),
        '#size' => 6,
        '#maxlength' => 6,
        '#required' => TRUE,
        '#default_value' => $record->count,
      );
      $form['hide'] = array(
        '#type' => 'checkbox',
        '#title' => t('Hide'),
        '#default_value' => $record->hide,
      );
      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
      );
    }
  }
  else {
    drupal_set_message(t('No term found'), 'error');
  }
  return $form;
}

function searchcloud_block_admin_edit_validate($form, &$form_state) {
  searchcloud_block_admin_add_validate_forms($form_state['values']);
}

function searchcloud_block_admin_edit_submit($form, &$form_state) {
  $data = array(
    'keyword' => drupal_strtolower($form_state['values']['term']),
    'count' => $form_state['values']['count'],
    'hide' => $form_state['values']['hide'],
  );
  searchcloud_block_admin_edit_term($data, drupal_strtolower($form_state['values']['orig_term']));
  exit();
}

function searchcloud_block_admin_edit_term($fields, $key) {
  db_update('searchcloud_block_count')
  ->fields($fields)
  ->condition('keyword', $key)
  ->execute();
  drupal_set_message(t('Term @term changed', array('@term' => $fields['keyword'])));
  drupal_goto('admin/config/search/searchcloud_block');
  exit();
}
