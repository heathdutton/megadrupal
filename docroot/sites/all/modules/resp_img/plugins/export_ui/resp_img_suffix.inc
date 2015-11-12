<?php

$plugin = array(
  'schema' => 'resp_img_suffix',
  'access' => 'administer image styles',
  'menu' => array(
    'menu prefix' => 'admin/config/media',
    'menu item' => 'resp_img',
    'menu title' => 'Responsive images',
    'menu description' => 'Define suffixes for responsive images',
  ),
  'title singular' => t('responsive image suffix'),
  'title singular proper' => t('Responsive image suffix'),
  'title plural' => t('responsive image suffixes'),
  'title plural proper' => t('Responsive image suffixes'),
  'form' => array(
    'settings' => 'resp_img_suffix_ctools_export_ui_form',
  ),
);

/**
 * Responsive images set form
 */
function resp_img_suffix_ctools_export_ui_form(&$form, &$form_state) {
  unset($form['info']['name']);
  $resp_img_suffix = $form_state['item'];

  $form['id'] = array(
    '#type' => 'hidden',
    '#value' => isset($resp_img_suffix->id) ? $resp_img_suffix->id : NULL,
  );

  // Human-readable set label
  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => isset($resp_img_suffix->label) ? $resp_img_suffix->label : '',
    '#description' => t('The human-readable description of this suffix'),
    '#required' => TRUE,
  );

  // Machine-readable set name.
  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($resp_img_suffix->name) ? $resp_img_suffix->name : '',
    '#disabled' => FALSE,
    '#machine_name' => array(
      'exists' => 'resp_img_suffix_load',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this suffix.
      It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['suffix'] = array(
    '#type' => 'textfield',
    '#title' => t('Suffix'),
    '#description' => t('The image formatter suffix (e.g. "_mobile")'),
    '#required' => TRUE,
    '#default_value' => isset($resp_img_suffix->suffix) ? $resp_img_suffix->suffix : "",
  );

  $form['breakpoint'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#description' => t('The smallest width for this suffix. Images will be rendered in this 
      format when the window is above this width. If the window width is smaller than any of 
      the defined suffix\' widths, the default will be used.'),
    '#required' => TRUE,
    '#default_value' => isset($resp_img_suffix->breakpoint) ? $resp_img_suffix->breakpoint : "",
  );
  return $form;
}
