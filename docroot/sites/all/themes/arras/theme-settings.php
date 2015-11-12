<?php

function arras_form_system_theme_settings_alter(&$form, $form_state)  {
  $form['extra'] = array(
    '#type' => 'fieldset',
    '#title' => t('Colour scheme'),
    '#weight' => -1,
  );
  $options = array(
    'blue' => t('Blue'),
    'default' => t('Black'),
    'green' => t('Green'),
    // 'legacy' => t('Legacy'),
    'orange' => t('Orange'),
    'red' => t('Red'),
    'violet' => t('Violet'),
  );
  $form['extra']['colour'] = array(
    '#type' => 'select',
    '#title' => t('Colour scheme'),
    '#options' => $options,
    '#default_value' => theme_get_setting('colour'),
  );
}
