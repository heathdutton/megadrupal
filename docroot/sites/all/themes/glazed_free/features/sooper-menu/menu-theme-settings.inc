<?php

$form['glazed_settings']['menu'] = array(
  '#title' => t('Menu'),
  '#type' => 'fieldset',
);

// $form['glazed_settings']['menu']['menu_style'] = array(
//     '#type' => 'radios',
//     '#title' => t('Menu Style'),
//     '#default_value' => (theme_get_setting('menu_style')) ? theme_get_setting('menu_style') : 'normal',
//     '#options' => array(
//       'normal' => t('Normal'),
//       'full_screen' => t('Full Screen'),
//       'off_canvas' => t('Off-Canvas (side fold-out)')
//     ),
//     '#description' => t('Choose your menu style. Only Normal menu style supports Mega Menu dropdowns.'),
// );

$form['glazed_settings']['menu']['menu_hover'] = array(
    '#type' => 'radios',
    '#title' => t('Hover Style'),
    '#default_value' => (theme_get_setting('menu_hover')) ? theme_get_setting('menu_hover') : 'opacity',
    '#options' => array(
      'opacity' => t('Opacity'),
      'background' => t('Colored Background'),
      'text' => t('Colored text')
    ),
);

$form['glazed_settings']['menu']['menu_type'] = array(
    '#type' => 'radios',
    '#title' => t('Typogrpahic Style'),
    '#default_value' => (theme_get_setting('menu_type')) ? theme_get_setting('menu_type') : '',
    '#options' => array(
      '' => t('Normal'),
      'uppercase' => t('Uppercase'),
      'bold' => t('Bold'),
      'lead' => t('Lead text')
    ),
);

$form['glazed_settings']['menu']['menu_full_screen_opacity'] = array(
  '#type' => 'textfield',
  '#title' => t('Menu Background opacity'),
  '#default_value' => (theme_get_setting('menu_full_screen_opacity')) ? theme_get_setting('menu_full_screen_opacity') : 1,
  '#size' => 9,
  '#maxlength' => 9,
  '#description' => t('0 is fully transparent and 1 is fully opaque.'),
);
