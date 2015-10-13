<?php

/**
 * @file
 * Plugin to provide an relationship handler for term from node.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Attached Entity from Party'),
  'keyword' => 'attached_entity',
  'description' => t('Adds an attached entity from the party context based on the data set. If more than one attached entity exists for this data set only the first is returned. To handle multiple attached entity\'s in a Pane use Views'),
  'required context' => new ctools_context_required(t('Party'), 'party'),
  'context' => 'party_attached_entity_from_party_context',
  'edit form' => 'party_attached_entity_from_party_settings_form',
  'defaults' => array('data_set_name' => ''),
);

/**
 * Return a new context based on an existing context.
 */
function party_attached_entity_from_party_context($context, $conf) {
  $data_set = party_get_data_set_info($conf['data_set_name']);
  $entity_type = $data_set['entity type'];

  // If unset it wants a generic, unfilled context, which is just NULL.
  if (empty($context->data)) {
    return ctools_context_create_empty('entity:' . $entity_type, NULL);
  }

  // Load attached entity from party ID
  $entity = $context->data->getDataSetController($conf['data_set_name'])->getEntity();

  if ($entity) {
    return ctools_context_create('entity:' . $entity_type, $entity);
  }
}

/**
 * Settings form for the relationship.
 */
function party_attached_entity_from_party_settings_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $options = array();
  foreach (party_get_data_set_info() as $data_set_name => $data_set) {
    $options[$data_set_name] = $data_set['label'];
  }
  $form['data_set_name'] = array(
    '#title' => t('Data Set'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => $conf['data_set_name'],
    '#prefix' => '<div class="clearfix">',
    '#suffix' => '</div>',
  );

  return $form;
}
