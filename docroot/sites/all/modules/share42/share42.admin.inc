<?php
/**
 * @file
 * Administration pages for the Share42 module.
 */

/**
 * Returns settings form.
 */
function share42_admin_settings() {
  $form = array();

  $form['share42_zero_counter'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show the zero counter'),
    '#default_value' => variable_get('share42_zero_counter', 0),
  );

  foreach (array('share42_top1', 'share42_top2', 'share42_margin') as $key) {
    $form[$key] = array(
      '#type' => 'textfield',
      '#size' => 3,
      '#maxlength' => 4,
      '#element_validate' => array('element_validate_number'),
    );
  }
  $form['share42_top1'] += array(
    '#title' => t('Offset without scroll'),
    '#description' => t('Distance from the top of the page to the "sticky" panel (in pixels).'),
    '#default_value' => variable_get('share42_top1', 200),
  );
  $form['share42_top2'] += array(
    '#title' => t('Offset with scroll'),
    '#description' => t('Distance from the top of the visible area of ​​the page to the floating panel (in pixels).'),
    '#default_value' => variable_get('share42_top2', 50),
  );
  $form['share42_margin'] += array(
    '#title' => t('Horizontal margin'),
    '#description' => t('Horizontal displacement of the panel. If you want to move it to the left, for example, by 70 pixels, set the value to "-70", but if you want to move it to the right, for example, by 50 pixels, set this value to "50".'),
    '#default_value' => variable_get('share42_margin', -70),
  );

  return system_settings_form($form);
}
