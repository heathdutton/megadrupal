<?php

/**
 * @file
 *
 * Plugin to provide a eck_add_form context
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('ECK add form'),
  'description' => t('An entity add form created by ECK.'),
  'context' => 'eck_pm_context_create_eck_add_form',
  'edit form' => 'eck_pm_context_eck_add_form_settings_form',
  'defaults' => array('entity_type' => '', 'type' => ''),
  'keyword' => 'eck_add',
  'context name' => 'eck_add_form',
  'convert list' => array('entity_type' => t('Entity type'), 'type' => t('Bundle')),
  'convert' => 'eck_pm_context_eck_add_form_convert',
);


/**
 * It's important to remember that $conf is optional here, because contexts
 * are not always created from the UI.
 */
function eck_pm_context_create_eck_add_form($empty, $data = NULL, $conf = FALSE) {
  static $creating = FALSE;

  $entity_type = $data['entity_type'];
  $bundle_name = $data['type'];

  $context = new ctools_context(array('form', 'entity', 'eck_add', 'eck_form', $entity_type, 'entity:' . $entity_type));
  $context->plugin = 'eck_add_form';

  if ($empty || ($creating)) {
    return $context;
  }
  $creating = TRUE;

  if (!empty($data)) {
    $entity_types = EntityType::loadAll();

    if (isset($entity_types[$entity_type]) && eck__entity_menu_access('add', $entity_type, $bundle_name)) {
      $entity_info = entity_get_info($entity_type);
      $options = array('type' => $bundle_name);

      if (!empty($entity_info['entity keys']['label'])) {
        $options[$entity_info['entity keys']['label']] = '';
      }
      $entity = entity_create($entity_type, $options);
      $form_id = "eck__entity__form_add_{$entity_type}_{$bundle_name}";;

      $form_state = array(
        'want form' => TRUE,
        'build_info' => array(
          'args' => array($entity)
        )
      );

      // Use module_load_include so that caches and stuff can know to load this.
      form_load_include($form_state, 'inc', 'eck', 'eck.entity');

      $form = drupal_build_form($form_id, $form_state);

      // In a form, $data is the object being edited.
      $context->data     = $entity;
      $context->title    = $bundle_name;
      $context->argument = $bundle_name;
      $context->entity_type = $entity_type;

      $context->form       = $form;
      $context->form_title = t('Submit @name', array('@name' => $bundle_name));
      $context->form_state = &$form_state;
      $context->form_id    = $form_id;
      $context->restrictions['form'] = array('form');
      $context->restrictions['type'] = array($bundle_name);

      $creating = FALSE;

      return $context;
    }
  }

  $creating = FALSE;
}


/**
 * Adding the element to select the entity type and the bundle.
 */
function eck_pm_context_eck_add_form_settings_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['entity_type_bundle'] = array(
    '#title' => t('Entity Type : Bundle'),
    '#type' => 'select',
    '#required' => TRUE,
    '#options' => eck_pm_entity_type_options(),
    '#default_value' => $conf['entity_type'] . ':' . $conf['type'],
    '#description' => t('Select the entity type for this form.'),
  );

  return $form;
}

/**
 * Submit callback.
 */
function eck_pm_context_eck_add_form_settings_form_submit($form, &$form_state) {
  list($entity_type, $bundle) = explode(':', $form_state['values']['entity_type_bundle'], 2);
  $form_state['conf']['entity_type'] = $entity_type;
  $form_state['conf']['type'] = $bundle;
}

/**
 * Convert a context into a string.
 */
function eck_pm_context_eck_add_form_convert($context, $type) {
  switch ($type) {
    case 'type':
      return $context->data->type;
  }
}
