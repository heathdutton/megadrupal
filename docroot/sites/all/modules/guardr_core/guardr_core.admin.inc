<?php
/**
 * @file
 * Admin related callbacks.
 */

/**
 * Callback function for admin/config/guardr-core page.
 */
function guardr_core_settings_form() {

  $form['guardr_core_intranet_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Intranet style'),
    '#description' => t('Set intranet style to hide the site name and logo?'),
  );
  $form['guardr_core_intranet_fieldset']['guardr_intranet'] = array(
    '#type' => 'radios',
    '#default_value' => variable_get('guardr_intranet', 1),
    '#options' => array(0 => t('No'), 1 => t('Yes')),
  );

  return system_settings_form($form);
}
