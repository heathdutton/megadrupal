<?php
$form['arctica_settings']['dropdown'] = array(
  '#title' => t('Dropdown Menus'),
  '#type' => 'fieldset',
  '#collapsible' => TRUE,
  '#collapsed' => TRUE,
  '#weight' => 2,
);

// Width of Drop-down menus
$form['arctica_settings']['dropdown']['sooperfish_width'] = array(
  '#type' => 'select',
  '#title' => t('Drop-down Menus Sub-Level Menu Width'),
  '#default_value' => theme_get_setting('sooperfish_width'),
  '#options' => _arctica_size_range(150, 300, FALSE, 200, 10),
  '#description' => t('Select the width the submenus.'),
);

$form['arctica_settings']['dropdown']['sooperfish'] = array(
  '#title' => t('sooperfish Dropdowns'),
  '#type' => 'fieldset',
  '#collapsible' => TRUE,
  '#collapsed' => FALSE,
);

$form['arctica_settings']['dropdown']['sooperfish']['sooperfish_enable'] = array(
  '#type'          => 'checkbox',
  '#title'         => t('Enable sooperfish dropdowns'),
  '#default_value' => theme_get_setting('sooperfish_enable'),
);

$form['arctica_settings']['dropdown']['sooperfish']['sooperfish_invoke'] = array(
  '#type' => 'textfield',
  '#title' => t('Element for sooperFish invocation'),
  '#default_value' => theme_get_setting('sooperfish_invoke'),
  '#description' => t('Enter a jQuery- or CSS selector to mark the menu(s) you want to turn into animated dropdown menus.'),
);

$form['arctica_settings']['dropdown']['sooperfish']['sooperfish_dualcolumn'] = array(
  '#type' => 'textfield',
  '#title' => t('2-Column Dropdown'),
  '#default_value' => theme_get_setting('sooperfish_dualcolumn'),
  '#size' => 3,
  '#maxlength' => 3,
  '#description' => t('Enter the maximum number of list items in a 1-column dropdown. When this limit is exceeded a 2-column dropdown will show.'),
  '#element_validate' => array('_arctica_is_number'),
);

$form['arctica_settings']['dropdown']['sooperfish']['sooperfish_triplecolumn'] = array(
  '#type' => 'textfield',
  '#title' => t('3-Column Dropdown'),
  '#default_value' => theme_get_setting('sooperfish_triplecolumn'),
  '#size' => 3,
  '#maxlength' => 3,
  '#description' => t('Enter the maximum number of list items in a 2-column dropdown. When this limit is exceeded a 3-column dropdown will show.'),
  '#element_validate' => array('_arctica_is_number'),
);

$form['arctica_settings']['dropdown']['sooperfish']['sooperfish_delay'] = array(
  '#type' => 'textfield',
  '#title' => t('Mouse-Out Delay'),
  '#default_value' => theme_get_setting('sooperfish_delay'),
  '#size' => 6,
  '#maxlength' => 6,
  '#description' => t('Enter the time in milliseconds that SooperFish will wait when the mouse moves out of the menu. This prevents the menu for disappearing when a user accidentaly crosses moves out of its bounds. 500 is the recommended value.'),
  '#element_validate' => array('_arctica_is_number'),
);

$form['arctica_settings']['dropdown']['sooperfish']['show_anim'] = array(
  '#title' => t('Menu-Show Animation'),
  '#type' => 'fieldset',
  '#collapsible' => TRUE,
  '#collapsed' => TRUE,
);

$form['arctica_settings']['dropdown']['sooperfish']['show_anim']['sooperfish_speed_show'] = array(
  '#type' => 'textfield',
  '#title' => t('Animation Speed'),
  '#default_value' => theme_get_setting('sooperfish_speed_show'),
  '#size' => 6,
  '#maxlength' => 6,
  '#description' => t('Enter the animation speed in milliseconds.'),
  '#element_validate' => array('_arctica_is_number'),
);

$form['arctica_settings']['dropdown']['sooperfish']['show_anim']['sooperfish_easing_show'] = array(
  '#type' => 'select',
  '#title' => t('Animation Easing'),
  '#default_value' => theme_get_setting('sooperfish_easing_show'),
  '#options' => $easing_options,
  '#description' => t('Select the function by which the animation gains speed.'),
);

$form['arctica_settings']['dropdown']['sooperfish']['show_anim']['sooperfish_properties_show'] = array(
  '#type' => 'checkboxes',
  '#title' => t('Animation actions'),
  '#default_value' => theme_get_setting('sooperfish_properties_show'),
  '#options' => array(
  'opacity' => t('Fade'),
  'width' => t('Fold horizontal'),
  'height' => t('Fold vertical'),
),
  '#description' => t('Select the properties you would like to animate.'),
);

$form['arctica_settings']['dropdown']['sooperfish']['hide_anim'] = array(
  '#title' => t('Menu-hide Animation'),
  '#type' => 'fieldset',
  '#collapsible' => TRUE,
  '#collapsed' => TRUE,
);

$form['arctica_settings']['dropdown']['sooperfish']['hide_anim']['sooperfish_speed_hide'] = array(
  '#type' => 'textfield',
  '#title' => t('Animation Speed'),
  '#default_value' => theme_get_setting('sooperfish_speed_hide'),
  '#size' => 6,
  '#maxlength' => 6,
  '#description' => t('Enter the animation speed in milliseconds.'),
  '#element_validate' => array('_arctica_is_number'),
);

$form['arctica_settings']['dropdown']['sooperfish']['hide_anim']['sooperfish_easing_hide'] = array(
  '#type' => 'select',
  '#title' => t('Animation Easing'),
  '#default_value' => theme_get_setting('sooperfish_easing_hide'),
  '#options' => $easing_options,
  '#description' => t('Select the function by which the animation gains speed.'),
);

$form['arctica_settings']['dropdown']['sooperfish']['hide_anim']['sooperfish_properties_hide'] = array(
  '#type' => 'checkboxes',
  '#title' => t('Animation actions'),
  '#default_value' => theme_get_setting('sooperfish_properties_hide'),
  '#options' => array(
  'opacity' => t('Fade'),
  'width' => t('Fold horizontal'),
  'height' => t('Fold vertical'),
),
  '#description' => t('Select the properties you would like to animate.'),
);
