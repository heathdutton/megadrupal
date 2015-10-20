<?php

/**
 * @file The "Delete form" of cat_items.
 *
 * @author Oleksiy Kalinichenko <aexchecker@gmail.com>
 */

/**
 * Form for deletion or revert CAT item from database.
 *
 * @param array $form
 * @param array $form_state
 * @param array $cat_item
 *
 * @return array Drupal form array
 */
function cat_ui_delete_form($form, $form_state, $cat_item) {
  $form['#cat_item'] = $cat_item;

  $form['machine_name'] = array(
    '#type' => 'value',
    '#value' => $cat_item->machine_name,
  );

  if ($cat_item->is_overwritten) {
    $form['message'] = array(
      '#markup' => t('Are you sure that you want to revert CAT item "%name" from source?', array(
        '%name' => filter_xss($cat_item->name),
      ))
    );
  }
  else {
    $form['message'] = array(
      '#markup' => t('Are you sure that you want to delete CAT item "%name" from database?', array(
        '%name' => filter_xss($cat_item->name),
      ))
    );
  }

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => $cat_item->is_overwritten ? t('Revert') : t('Delete'),
  );

  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
  );

  return $form;
}

/**
 * Submit callback for deletion form.
 * Removes CAT item from database.
 *
 * @param array $form
 * @param array $form_state
 */
function cat_ui_delete_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  if ($values['op'] == $values['submit']) {
    cat_delete($values['machine_name']);
    drupal_set_message(t('CAT item "%name" was successfully deleted.', array(
      '%name' => $form['#cat_item']->name,
    )));
  }

  $form_state['redirect'] = 'admin/structure/cat';
}

