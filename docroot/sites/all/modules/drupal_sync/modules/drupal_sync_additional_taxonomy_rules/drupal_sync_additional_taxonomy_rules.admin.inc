<?php

/**
 * @file
 * Settings forms
 */

/**
 * Create settings form in drupal configuration setings section for module
 */
function drupal_sync_additional_taxonomy_rules_admin_settings_form($form, &$form_state) {

  $node_types = node_type_get_types();
  $vocs = taxonomy_vocabulary_get_names();

  $voc_options = array(0 => t('-- None --'));
  foreach ($vocs as $voc) {
    $voc_options[$voc->vid] = $voc->name;
  }

  $node_types = node_type_get_types();
  $options = array(0 => t('-- None --'));
  foreach ($node_types as $node_type) {
    $options[$node_type->type] = $node_type->name;
  }

  $data = variable_get('drupal_sync_additional_taxonomy_rules_manage_content_type_vocabulary', array(0 => 0));

  $form['attention'] = array(
    '#markup' => '<h2>' . t('Choose relations vocabulary from node type') . '</h2>'
  );

  $form['content_type'] = array(
    '#type' => 'select',
    '#title' => t('Select content type for adding site term referrence.'),
    '#options' => $options,
    '#default_value' => array(key($data)),
  );

  $form['vocabulary'] = array(
    '#type' => 'select',
    '#title' => t('Select vocabulary for adding site term referrence.'),
    '#options' => $voc_options,
    '#default_value' => array($data[key($data)]),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Form submit
 */
function drupal_sync_additional_taxonomy_rules_admin_settings_form_submit($form, $form_state) {
  $content_type = (isset($form_state['values']['content_type'])) ? $form_state['values']['content_type'] : 0;
  $voc = isset($form_state['values']['vocabulary']) ? $form_state['values']['vocabulary'] : 0;
  $data = array($content_type => $voc);
  variable_set('drupal_sync_additional_taxonomy_rules_manage_content_type_vocabulary', $data);
  drupal_set_message(t('Settings successfully saved.'), 'status');
}