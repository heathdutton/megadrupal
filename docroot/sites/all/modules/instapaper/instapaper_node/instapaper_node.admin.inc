<?php

/**
 * Menu callback.
 * Create the settings form for instapaper_node.
 */
function instapaper_node_settings_form($form_state) {

  $form['instapaper_node_label'] = array(
    '#type' => 'select',
    '#title' => t('Instapaper Button Label'),
    '#default_value' => variable_get('instapaper_node_label', 'Send to Instapaper'),
    '#options' => array(
      'Send to Instapaper' => t('Send to Instapaper'),
      'Read Later' => t('Read Later'),
      'Read Later with Instapaper' => t('Read Later with Instapaper'),
      'Save with Instapaper' => t('Save with Instapaper'),
      'Instapaper: Read Later' => t('Instapaper: Read Later'),
    ),
    '#description' => t('Sets the label on the Instapaper button'),
  );

  $form['instapaper_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('The content types to enable Instapaper for'),
    '#default_value' => variable_get('instapaper_node_types', array()),
    '#options' => node_type_get_names(),
    '#description' => t('On the specified node types an Add to Instapaper item will be enabled for logged in and annon users.'),
  );
  return system_settings_form($form);
}