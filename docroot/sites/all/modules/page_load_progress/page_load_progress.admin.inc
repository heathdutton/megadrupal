<?php

/**
 * @file
 * Admin settings forms for Page Load Progress.
 */

/**
 * Implements hook_form_settings().
 */
function page_load_progress_settings($form, &$form_state) {

  $form = array();

  $form['page_load_progress_time'] = array(
    '#type' => 'select',
    '#title' => t('Time to wait before locking the screen'),
    '#description' => t('Enter the time you want to wait before showing the image lock.'),
    '#options' => array(
      10   => t('Show immediatly'),
      1000 => t('Show after a 1 second delay'),
      3000 => t('Show after a 3 seconds delay'),
      5000 => t('Show after a 5 seconds delay'),
    ),
    '#default_value' => variable_get('page_load_progress_time', 10),
  );

  $form['page_load_progress_elements'] = array(
    '#type' => 'textfield',
    '#title' => t('Elements that will trigger the throbber'),
    '#description' => t('Enter the elements that will trigger the effect separated by commas. Classes should have their leading dot and ids their leading hashes.'),
    '#default_value' => variable_get('page_load_progress_elements', '.form-submit'),
  );

  return system_settings_form($form);
}
