<?php

$form['glazed_settings']['layout'] = array(
  '#title' => t('Layout'),
  '#type' => 'fieldset',
);

$form['glazed_settings']['layout']['sticky_footer'] = array(
  '#type' => 'checkbox',
  '#title' => t('Sticky Footer'),
  '#default_value' => ((theme_get_setting('sticky_footer') !== null)) ? theme_get_setting('sticky_footer') : 1,
  '#description' => t('Makes footer stick to the bottom of the browser on short pages.'),

);

$form['glazed_settings']['layout']['layout_max_width'] = array(
  '#type' => 'textfield',
  '#title' => t('Layout Max-width'),
  '#default_value' => (theme_get_setting('layout_max_width')) ? theme_get_setting('layout_max_width') : '1170',
  '#size' => 9,
  '#maxlength' => 9,
  '#description' => t('Max width of the website container. 480px - 2560px. Default is 1170px. With Side-Header enabled we typically use 980px.'),
);

$form['glazed_settings']['layout']['gutter_horizontal'] = array(
  '#type' => 'textfield',
  '#title' => t('Horizontal gutter size'),
  '#default_value' => (theme_get_setting('gutter_horizontal')) ? theme_get_setting('gutter_horizontal') : '30',
  '#size' => 9,
  '#maxlength' => 9,
  '#description' => t('Width of the horizontal gutter. 0 - 100px. Default is 30px.'),
);

$form['glazed_settings']['layout']['gutter_vertical'] = array(
  '#type' => 'textfield',
  '#title' => t('Vertical gutter size'),
  '#default_value' => (theme_get_setting('gutter_vertical')) ? theme_get_setting('gutter_vertical') : '30',
  '#size' => 9,
  '#maxlength' => 9,
  '#description' => t('Width of the vertical gutter. 0 - 100px. Default is 30px.'),
);

$form['glazed_settings']['layout']['mobile_layout'] = array(
  '#title' => t('Mobile view'),
  '#type' => 'fieldset',
);


$form['glazed_settings']['layout']['mobile_layout']['gutter_horizontal_mobile'] = array(
  '#type' => 'textfield',
  '#title' => t('Horizontal gutter size'),
  '#default_value' => (theme_get_setting('gutter_horizontal_mobile')) ? theme_get_setting('gutter_horizontal_mobile') : '30',
  '#size' => 9,
  '#maxlength' => 9,
  '#description' => t('Width of the horizontal gutter in Glazed mobile view (<1200px). 0 - 100px. Default is 30.'),
);

$form['glazed_settings']['layout']['mobile_layout']['gutter_vertical_mobile'] = array(
  '#type' => 'textfield',
  '#title' => t('Vertical gutter size'),
  '#default_value' => (theme_get_setting('gutter_vertical_mobile')) ? theme_get_setting('gutter_vertical_mobile') : '30',
  '#size' => 9,
  '#maxlength' => 9,
  '#description' => t('Width of the vertical gutter in Glazed mobile view (<1200px). 0 - 100px. Default is 30px.'),
);