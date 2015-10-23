<?php

function academia_form_system_theme_settings_alter(&$form, $form_state)  {
  $form['extra'] = array(
    '#type' => 'fieldset',
    '#title' => t('Additional theme settings'),
  );
  $options = array(
    'black' => t('Black'),
    'blue' => t('Blue'),
    'copper' => t('Copper'),
    'cornflower' => t('Cornflower blue'),
    'fuchsia' => t('Fuchsia (pink)'),
    'green' => t('Green'),
    'grey' => t('Grey'),
    'lime' => t('Lime'),
    'mint' => t('Mint'),
    'orange' => t('Orange'),
    'pear' => t('Pear'),
    'pink' => t('Pink'),
    'purple' => t('Purple'),
    'red' => t('Red'),
    'rose' => t('Rose'),
    'default' => t('Turquoise (default)'),    
  );
  $form['extra']['colour'] = array(
    '#type' => 'select',
    '#title' => t('Colour scheme'),
    '#options' => $options,
    '#default_value' => theme_get_setting('colour'),
  );
  $form['extra']['sidebar_layout'] = array(
    '#type' => 'radios',
    '#title' => t('Sidebar layout'),
    '#options' => array(
      'lr' => t('Sidebar | Content | Sidebar'),
      '2r' => t('Content | Sidebar | Sidebar'),
    ),
    '#default_value' => theme_get_setting('sidebar_layout'),
  );
  $form['extra']['shrink_two_sidebars'] = array(
    '#type' => 'checkbox',
    '#title' => t('Shrink sidebars when both are displayed'),
    '#description' => t('If this box is ticked, both sidebars will be reduced in width when they both appear.'),
    '#default_value' => theme_get_setting('shrink_two_sidebars'),
  );
}
