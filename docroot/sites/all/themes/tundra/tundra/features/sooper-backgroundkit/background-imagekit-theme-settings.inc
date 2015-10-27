<?php

drupal_add_library('system', 'farbtastic');
drupal_add_js(drupal_get_path('theme', 'tundra') . "/features/sooper-backgroundkit/tundra.backgroundkit.admin.js", 'file');
drupal_add_js('$(function () {$("#edit-backgroundkit").fieldset_tabs();});', 'inline');

$form['arctica_settings']['backgroundkit'] = array(
  '#title' => t('Site Background'),
  '#type' => 'fieldset',
  '#collapsible' => TRUE,
  '#collapsed' => TRUE,
  '#weight' => 5,
);

$form['arctica_settings']['backgroundkit']["gradient"] = array(
  '#title' => t('Background gradient'),
  '#type' => 'fieldset',
);

$form['arctica_settings']['backgroundkit']["gradient"]['gradient_enable'] = array(
  '#type'          => 'checkbox',
  '#title'         => t('Enable background gradient'),
  '#default_value' => theme_get_setting('gradient_enable'),
);

$form['arctica_settings']['backgroundkit']["gradient"]['gradient_opacity'] = array(
  '#type' => 'textfield',
  '#title' => t('Gradient opacity'),
  '#default_value' => theme_get_setting('gradient_opacity'),
  '#prefix' => '<div id="gradient-opacity-slider"></div>',
  '#size' => 3,
  '#maxlength' => 3,
  '#description' => t('Opacity of this background. If you lower the opacity the gradient will blend with the background color.'),
  '#element_validate' => array('_arctica_is_number'),
);

$form['arctica_settings']['backgroundkit']["gradient"]['gradient_color_top'] = array(
  '#type' => 'textfield',
  '#title' => t('Top color'),
  '#default_value' => theme_get_setting('gradient_color_top'),
  '#size' => 8,
  '#maxlength' => 8,
  '#element_validate' => array('_tundra_is_hex_color'),
);

$form['arctica_settings']['backgroundkit']["gradient"]['gradient_color_bottom'] = array(
  '#type' => 'textfield',
  '#title' => t('Bottom color'),
  '#default_value' => theme_get_setting('gradient_color_bottom'),
  '#size' => 8,
  '#maxlength' => 8,
  '#element_validate' => array('_tundra_is_hex_color'),
);

$form['arctica_settings']['backgroundkit']["image"] = array(
  '#title' => t('Background image'),
  '#type' => 'fieldset',
);

$form['arctica_settings']['backgroundkit']["image"]['bg_image_enable'] = array(
  '#type'          => 'checkbox',
  '#title'         => t('Enable background image'),
  '#default_value' => theme_get_setting('bg_image_enable'),
);

$form['arctica_settings']['backgroundkit']["image"]['image_opacity'] = array(
  '#type' => 'textfield',
  '#title' => t('Image opacity'),
  '#default_value' => theme_get_setting('image_opacity'),
  '#prefix' => '<div id="image-opacity-slider"></div>',
  '#size' => 3,
  '#maxlength' => 3,
  '#description' => t('Opacity of this background. If you lower the opacity the background color and/or gradient will blend with the image. Using a low opacity you can easily add textures to your background colors.'),
  '#element_validate' => array('_arctica_is_number'),
);

$form['arctica_settings']['backgroundkit']["image"]['bg_behave'] = array(
  '#type'          => 'select',
  '#title'         => t('Background image behavior'),
  '#default_value' => theme_get_setting('bg_behave'),
  '#options'       => array('tile' => 'Tile (repeating)', 'cover' => 'cover (fullscreen background)'),
);

$bg_images = array();
$bg_image_suffix = '<h3>' . t('Available background images:') . '</h3>';

foreach ($theme_chain as $theme) {
  foreach (file_scan_directory(drupal_get_path('theme', $theme) . '/backgrounds', '/\.(jpg|gif|png?)$/') as $file) {
    $key = $theme . '|' . $file->filename;
    $bg_images[$key] = $file->filename;
  }
}
asort($bg_images, SORT_STRING);

foreach ($bg_images as $key => $name) {
  $parts = explode('|', $key);
  $bg_image_suffix .= '<div class="demo-background-wrapper"><p>' . $name . '</p> <div class="demo-background" style="background:#ccc url(' . $base_path . drupal_get_path('theme', $parts[0]) . '/backgrounds/' . $parts[1] . ') center center;"></div></div>';
}
$form['arctica_settings']['backgroundkit']["image"]['background_image'] = array(
  '#type'          => 'select',
  '#title'         => t('Background Image'),
  '#default_value' => theme_get_setting('background_image'),
  '#options'       => $bg_images,
  '#suffix'        => $bg_image_suffix,
);
