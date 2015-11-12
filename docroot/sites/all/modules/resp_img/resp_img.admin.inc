<?php

/**
 * @file
 * Admin settings
 */

function resp_img_admin_settings() {
  $form = array();

  $form['resp_img_default_suffix'] = array(
    '#type' => 'textfield',
    '#title' => t('Default suffix'),
    '#description' => t('Specify the default suffix to use, ex. "_mobile". Make sure you also create the corresponding imagestyles.'),
    '#required' => TRUE,
    '#default_value' => variable_get('resp_img_default_suffix', ''),
    '#size' => 20,
  );

  $form['resp_img_forceredirect'] = array(
    '#type' => 'select',
    '#title' => t('Force redirect'),
    '#description' => t('Force redirect if cookie is not set.'),
    '#required' => TRUE,
    '#default_value' => variable_get('resp_img_forceredirect', FALSE),
    '#options' => array(0 => t('No'), 1 => ('Yes'),),
  );

  $form['resp_img_forceresize'] = array(
    '#type' => 'select',
    '#title' => t('Force resize'),
    '#description' => t('Resize images when browser windows resizes.'),
    '#required' => TRUE,
    '#default_value' => variable_get('resp_img_forceresize', FALSE),
    '#options' => array(0 => t('No'), 1 => ('Yes'),),
  );

  $form['resp_img_reloadonresize'] = array(
    '#type' => 'select',
    '#title' => t('Reload on resize'),
    '#description' => t('Reload the page when browser windows resizes, use wisely!'),
    '#required' => TRUE,
    '#default_value' => variable_get('resp_img_reloadonresize', FALSE),
    '#options' => array(0 => t('No'), 1 => ('Yes'),),
  );

  $form['resp_img_purgeexpire'] = array(
    '#type' => 'select',
    '#title' => t('Advanced support for purge/expire'),
    '#description' => t('Clear all variants using purge/expire modules.'),
    '#required' => TRUE,
    '#default_value' => variable_get('resp_img_purgeexpire', FALSE),
    '#options' => array(0 => t('No'), 1 => ('Yes'),),
  );

  $form['resp_img_block_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable responsive images for blocks.'),
    '#description' => t('If checked, the images within blocks will be responsive as well.'),
    '#default_value' => variable_get('resp_img_block_enabled', 1),
  );

  $form['resp_img_use_device_pixel_ratio'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use device pixel ratio'),
    '#description' => t('Take the device pixel ratio in to account when calculating the right suffix. !link', array('!link' => l('Click here for more information.', 'http://drupal.org/node/1550054'))),
    '#default_value' => variable_get('resp_img_use_device_pixel_ratio', FALSE),
  );

  return system_settings_form($form);
}

function resp_img_add_style_form($form, &$form_state) {
  module_load_include('inc', 'image', 'image.admin');
  $form['style'] = array(
    '#title' => t('Image style'),
    '#type' => 'select',
    '#options' => image_style_options(FALSE),
    '#required' => TRUE,
    '#description' => t('This image style will be cloned to create the responsive style'),
  );

  $form['base_name'] = array(
    '#type' => 'textfield',
    '#size' => '64',
    '#title' => t('Image style base name'),
    '#description' => t('The name is used in URLs for generated images. Use only lowercase alphanumeric characters, underscores (_), and hyphens (-).'),
    '#element_validate' => array('image_style_name_validate'),
    '#required' => TRUE,
  );
  $suffixes = resp_img_suffix_load_all();
  $options = array();
  $options[variable_get('resp_img_default_suffix', '')] = t('Default') . ' (' . variable_get('resp_img_default_suffix', '') . ')';
  foreach ($suffixes as $suffix) {
    $options[$suffix->suffix] = $suffix->label . ' (' . $suffix->suffix . ')';
  }
  $form['suffixes'] = array(
    '#title' => t('Suffixes'),
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => drupal_map_assoc(array_keys($options)),
    '#description' => t('Select the suffixes to create an image style for'),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create'),
  );
  return $form;
}

function resp_img_add_style_form_validate($form, &$form_state) {
  foreach (array_filter($form_state['values']['suffixes']) as $suffix) {
    if (image_style_load($form_state['values']['base_name'] . $suffix)) {
      form_set_error('suffixes', t('An image style with the name !name already exists', array('!name' => $form_state['values']['base_name'] . $suffix)));
    }
  }
}

function resp_img_add_style_form_submit($form, &$form_state) {
  $base = image_style_load($form_state['values']['style']);
  if (!isset($base['effects'])) {
    $base['effects'] = array();
  }
  foreach ($base['effects'] as &$effect) {
    unset($effect['isid']);
    unset($effect['ieid']);
  }
  $base['effects'] = array_values($base['effects']);
  foreach (array_filter($form_state['values']['suffixes']) as $suffix) {
    $new_style = array(
      'name' => $form_state['values']['base_name'] . $suffix,
      'module' => 'resp_img',
      'storage' => IMAGE_STORAGE_NORMAL,
      'effects' => $base['effects'],
    );
    image_default_style_save($new_style);
  }
  $form_state['redirect'] = 'admin/config/media/image-styles';
  drupal_set_message(t('The new styles have been created'));
}
