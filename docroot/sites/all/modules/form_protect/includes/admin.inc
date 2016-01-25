<?php

/**
 * @file
 * Administrative tasks for Form Protect module.
 */

/**
 * Provides a form API form callback corresponding to
 * 'admin/config/system/form_protect' route.
 *
 * @param array $form
 *   A form API renderable form array.
 * @para, array $form_state
 *   A form API form state array.
 *
 * @return array
 *   The form as a renderable array.
 */
function form_protect_settings_form(array $form, array &$form_state) {
  $form['form_protect_form_ids'] = array(
    '#type' => 'textarea',
    '#title' => t('Forms IDs'),
    '#description' => t('The IDs of forms to be protected. One per line.'),
    '#default_value' => variable_get('form_protect_form_ids', FORM_PROTECT_DEFAULT_FORM_IDS),
  );
  $form['form_protect_log'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log blocked submits'),
    '#default_value' => variable_get('form_protect_log', FALSE),
  );

  return system_settings_form($form);
}
