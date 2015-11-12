<?php
/**
 * Returns form for changing configuration options
 *
 * @param $form
 *   form
 * @param $form_state
 *   form state
 *
 * @return
 *   configuration form
 */
function addressfield_title_admin_form($form, &$form_state) {
  $form = array();
  $form['addressfield_title_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Title values'),
    '#description' => t('Separate prefixes with a space and do not include the leading dot..'),
    '#default_value' => variable_get("addressfield_title_prefix", "Miss Mrs Ms Mr Dr Prof Rev Lady Lord Sir"),
  );
  $form['addressfield_title_form_element'] = array(
    '#type' => 'select',
    '#title' => t('Form element'),
    '#description' => t('The form element that allows you to choose between the title options.'),
    '#options' => array(
      'select' => t('Select list'),
      'radios' => t('Radio buttons'),
    ),
    '#default_value' => variable_get('addressfield_title_form_element', 'select'),
  );
  return system_settings_form($form);
}
