<?php
/**
 * @file
 * Administration pages.
 */

/**
 * Form callback for indexed image styles.
 */
function apachesolr_image_configuration($form, $form_state) {
  $form = array();

  // Image styles to index.
  $options = array();
  foreach (image_styles() as $style) {
    $options[$style['name']] = $style['label'];
  }
  $form['apachesolr_image_index_styles'] = array(
    '#type' => 'checkboxes',
    '#multiple' => TRUE,
    '#title' => t('Index image styles'),
    '#description' => t('Select the image styles to be indexed in Solr.'),
    '#options' => $options,
    '#default_value' => variable_get('apachesolr_image_index_styles', array()),
  );

  return system_settings_form($form);
}
