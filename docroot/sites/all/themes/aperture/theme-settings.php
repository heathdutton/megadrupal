<?php

function aperture_form_system_theme_settings_alter(&$form, $form_state)  {
  $form['extra'] = array(
    '#type' => 'fieldset',
    '#title' => t('Colour scheme'),
    '#weight' => -1,
  );
  $options = array(
    'babyblue' => t('Baby Blue'),
    'beige' => t('Beige'),
    'black' => t('Black'),
    'blue' => t('Blue'),
    'blue-stripe' => t('Blue Stripe'),
    'green' => t('Green'),
    'green-stripe' => t('Green Stripe'),
    'grey' => t('Grey'),
    'orange-stripe' => t('Orange Stripe'),
    'red' => t('Red'),
    'white' => t('White'),
    'yellow' => t('Yellow'),
    'yellow-stripe' => t('Yellow Stripe'),
  );
  $form['extra']['colour'] = array(
    '#type' => 'select',
    '#title' => t('Colour scheme'),
    '#options' => $options,
    '#default_value' => theme_get_setting('colour'),
  );
}
