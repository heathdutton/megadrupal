<?php
// $Id: mopa.admin.inc,v 1.1.4.9 2010/08/01 01:49:29 kiam Exp $

/**
 * @file
 * Administration pages for Module categories.
 */

/**
 * Form builder; display the categories list.
 */
function mopa_categories_overview() {
  $categories = _mopa_get_categories();
  $form = array('#tree' => TRUE);

  foreach ($categories as $id => $category) {
    $form[$id]['#category'] = (array)$category;

    $form[$id]['modules'] = array(
      '#value' => check_plain(truncate_utf8(implode(', ', $category->modules), 40, FALSE, TRUE))
    );

    $form[$id]['identifier'] = array(
      '#value' => check_plain($category->identifier)
    );

    $form[$id]['status'] = array(
      '#type' => 'checkbox',
      '#default_value' => $category->status,
    );

    $form[$id]['edit'] = array(
      '#value' => l(t('edit'), "admin/settings/mopa/edit/{$id}"),
    );

    $form[$id]['delete'] = array(
      '#value' => l(t('delete'), "admin/settings/mopa/delete/{$id}"),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save')
  );

  return $form;
}

/**
 * Submit function for the category list.
 */
function mopa_categories_overview_submit($form, &$form_state) {
  foreach ($form_state['values'] as $id => $category) {
    if (is_numeric($id) && $form[$id]['#category']['status'] != $form_state['values'][$id]['status']) {
      $form[$id]['#category']['modules'] = implode(', ', $form[$id]['#category']['modules']);
      $form[$id]['#category']['status'] = $form_state['values'][$id]['status'];

      drupal_write_record('mopa', $form[$id]['#category'], 'cid');
    }
  }
}

/**
 * Form builder; display the form to edit the module categories.
 */
function mopa_categories_edit($form_state, $category = NULL) {
  if (!isset($category)) {
    $category = (object) array(
      'modules' => array(),
      'identifier' => '',
      'status' => 1,
    );
  }
  else {
    $form['#cid'] = $category->cid;
  }

  // Create the list of modules base name to show like help.
  $items = array();
  $files = drupal_system_listing('\.module$', 'modules', 'name', 0);

  system_get_files_database($files, 'module');
  ksort($files);

  foreach ($files as $filename => $file) {
    $info = drupal_parse_info_file(dirname($file->filename) .'/'. $file->name .'.info');
    if (empty($info)) {
      continue;
    }

    $item = t($info['name']);

    if (!empty($info['package'])) {
      if ($info['package'] == 'Core - required' || $info['package'] == 'Core - optional') {
        continue;
      }

      $item .= ' ('. t($info['package']) .')';
    }

    $items[$filename] = $item;
  }

  if (!empty($items)) {

    $form['modules-fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Modules'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $form['modules-fieldset']['modules'] = array(
      '#type' => count($items) > 10 ? 'select' : 'checkboxes',
      '#default_value' => $category->modules,
      '#options' => $items,
    );

    if (count($items) > 10) {
      $form['modules-fieldset']['modules']['#multiple'] = TRUE;
    }

    $form['identifier'] = array(
      '#type' => 'textfield',
      '#title' => t('Category identifier'),
      '#description' => t('The category to use; use an asterisk if you want to group the modules under <em>Other</em>.'),
      '#default_value' => $category->identifier,
      '#size' => 45,
      '#maxlength' => 45,
      '#required' => TRUE,
    );

    $form['status'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enabled'),
      '#default_value' => $category->status,
    );

    $form['save'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#weight' => 10,
    );
  }
  else {
    $form['message'] = array(
      '#value' => '<p>'. t('There are no third-party modules installed.') .'</p>',
    );
  }

  return $form;
}

/**
 * Submit function for the edit form.
 */
function mopa_categories_edit_submit($form, &$form_state) {
  $form_state['values']['modules'] = implode(', ', array_filter($form_state['values']['modules']));

  if (isset($form['#cid'])) {
    $form_state['values']['cid'] = $form['#cid'];
    drupal_write_record('mopa', $form_state['values'], 'cid');
    drupal_set_message(t('The category for %modules has been updated.', array('%modules' => $form_value['modules'])));
    watchdog('mopa', 'The category for %modules has been updated.', array('%modules' => $form_value['modules']));
  }
  else {
    drupal_write_record('mopa', $form_state['values']);
    drupal_set_message(t('A category for %modules has been created.', array('%modules' => $form_value['modules'])));
    watchdog('mopa', 'A category for %modules has been created.', array('%modules' => $form_value['modules']));
  }

  $form_state['redirect'] = 'admin/modules/mopa';
}

/**
 * Form builder; display the form to delete the module category.
 */
function mopa_categories_delete_confirm($form_state, $cid) {
  $form['#cid'] = $cid;

  return confirm_form($form,
    t('Are you sure you want to delete the modules category?'),
    isset($_GET['destination']) ? $_GET['destination'] : 'admin/modules/mopa',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Submission function for the module category form.
 */
function mopa_categories_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    db_query('DELETE FROM {mopa} WHERE cid = :cid', array(':cid' => $form['#cid']));
  }

  $form_state['redirect'] = 'admin/modules/mopa';
}

function theme_mopa_categories_overview($form) {
  $rows = array();

  foreach (element_children($form) as $key) {
    if (isset($form[$key]['modules'])) {
      $category = &$form[$key];

      $row = array();
      $row[] = drupal_render($category['modules']);
      $row[] = drupal_render($category['identifier']);
      $row[] = drupal_render($category['status']);
      $row[] = drupal_render($category['edit']);
      $row[] = drupal_render($category['delete']);

      $rows[] = $row;
    }
  }
  if (empty($rows)) {
    $rows[] = array(
      array(
        'data' => t('No categories available.'),
        'colspan' => '4'
      )
    );
  }

  $header = array(t('Modules'), t('Category'), t('Enabled'));
  $header[] = array('data' => t('Operations'), 'colspan' => '2');

  return theme('table', $header, $rows) . drupal_render($form);
}
