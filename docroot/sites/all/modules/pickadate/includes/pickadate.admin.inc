<?php

function pickadate_admin_settings($form, $form_state) {
  $form['pickadate_skin'] = array(
    '#title' => 'Pickadate Skin',
    '#description' => t('Select the appearance for Pickadate. Default presents it as a modal style popup, Classic presents it below the input field.'),
    '#type' => 'select',
    '#options' => array('default' => 'Default', 'classic' => 'Classic'),
    '#default_value' => variable_get('pickadate_skin', 'default'),
  );

  return system_settings_form($form);
}