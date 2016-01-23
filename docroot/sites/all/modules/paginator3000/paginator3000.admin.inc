<?php
/**
 * @file
 * Administation pages for Paginator 3000 module.
 */

/**
 * Module settings form.
 */
function paginator3000_settings_form() {
  $form = array();

  $form['paginator3000_override_pager'] = array(
    '#type' => 'select',
    '#title' => t('Site-wide pager'),
    '#description' => t('Set site-wide pager.'),
    '#options' => array(
      'core' => t('Drupal core pager'),
      'paginator3000' => t('Paginator 3000'),
    ),
    '#default_value' => variable_get('paginator3000_override_pager', 'core'),
  );

  return system_settings_form($form);
}
