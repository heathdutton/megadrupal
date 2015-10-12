<?php

/**
 * @file
 * Contains page callbacks.
 */

/**
 * Form for ordering a node's child pages.
 *
 * Largely based on book_admin_edit().
 */
function book_child_order_form($form, $form_state, $node)  {
  drupal_set_title($node->title);
  $form['#node'] = $node;

  module_load_include('inc', 'book', 'book.admin');

  _book_admin_table($node, $form);

  // Some tweaks to the table form that Book module produces.
  foreach (element_children($form['table']) as $key) {
    // Fix depth so the indentation is correct.
    $form['table'][$key]['depth']['#value'] -= ($node->book['depth'] -1);

    // Change the title column cells to a markup label rather than an editable
    // text field.
    $form['table'][$key]['title'] = array(
      '#markup' => check_plain($form['table'][$key]['title']['#default_value']),
    );
  }

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save child page order'),
  );

  return $form;
}

/**
 * Form validation handler.
 */
function book_child_order_form_validate($form, $form_state) {
  if ($form_state['values']['tree_hash'] != $form_state['values']['tree_current_hash']) {
    form_set_error('', t('This book has been modified by another user, the changes could not be saved.'));
  }
}

/**
 * Form submit handler.
 *
 * Mostly copied from book_admin_edit_submit().
 */
function book_child_order_form_submit($form, $form_state) {
  // Save elements in the same order as defined in post rather than the form.
  // This ensures parents are updated before their children, preventing orphans.
  $order = array_flip(array_keys($form_state['input']['table']));

  $form['table'] = array_merge($order, $form['table']);

  foreach (element_children($form['table']) as $key) {
    if ($form['table'][$key]['#item']) {
      $row = $form['table'][$key];
      $values = $form_state['values']['table'][$key];

      // Update menu item if moved.
      if ($row['plid']['#default_value'] != $values['plid'] || $row['weight']['#default_value'] != $values['weight']) {
        $row['#item']['plid'] = $values['plid'];
        $row['#item']['weight'] = $values['weight'];
        menu_link_save($row['#item']);
      }
    }
  }
}
