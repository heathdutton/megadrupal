<?php

/**
 * @file
 * Panels integration for Field Collection interface.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Configurable Field Collection Form'),
  'description' => t('TODO'),
  'category' => t('Form'),
  'edit form' => 'panels_fcf_edit_form',
  'render callback' => 'panels_fcf_render',
  'required context' => new ctools_context_required(t('Entity'), 'entity'),
  'admin info' => 'panels_fcf_admin_info',
  'defaults' => array(
    'context' => '',
    'field_name' => '',
  ),
);

/**
 * Admin info callback for panel pane.
 */
function panels_fcf_admin_info($subtype, $conf, $contexts) {

  $block = new stdClass();
  $block->title = $conf['override_title'] ? $conf['override_title_text'] : '';
  $params = array(
    '%entity' => $conf['context'],
    '%fc' => $conf['field_name'],
  );
  $block->content = t('Field Collection "%fc" form for entity "%entity".', $params);

  return $block;
}

/**
 * Edit form callback for the content type.
 */
function panels_fcf_edit_form($form, &$form_state) {

  $conf = $form_state['conf'];

  $options = _panels_fcf_load_all_field_collections();
  $form['field_name'] = array(
    '#type' => 'select',
    '#title' => t('Field Collection'),
    '#description' => t('The name of the Field Collection to use.'),
    '#default_value' => $conf['field_name'],
    '#options' => $options,
  );

  return $form;
}

/**
 * The submit form stores the data in $conf.
 */
function panels_fcf_edit_form_submit($form, &$form_state) {

  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    if (isset($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}

/**
 * Run-time rendering of the body of the block (content type).
 */
function panels_fcf_render($subtype, $conf, $panel_args, $context = NULL) {

  $block = new stdClass();

  $block->title = '';
  $block->content = '';

  if (!$context->empty) {
    $entity_type = end($context->type);
    list($entity_id) = entity_extract_ids($entity_type, $context->data);

    module_load_include('inc', 'field_collection', 'field_collection.pages');

    // @TODO: someting goes wrong if there are two such panels on one page,
    // and when you submit one of them, both will display the submitted form.
    // The problem must be somewhere in field_collection_item_form().
    $form = field_collection_item_add($conf['field_name'], $entity_type, $entity_id);
    if (!is_numeric($form)) {
      $block->content = drupal_render($form);
    }
    else {
      if (user_access('use panels dashboard')) {
        drupal_set_message(t("Can't display the field collection form."), 'error');
      }
    }
  }

  return $block;
}

/**
 * Helper function to return all the field collections.
 *
 * @return array
 *   Ready to use as #options in a dropdown.
 */
function _panels_fcf_load_all_field_collections() {

  $instances = field_info_instances();
  $field_collections = array();
  foreach ($instances as $type_bundles) {
    foreach ($type_bundles as $bundle_instances) {
      foreach ($bundle_instances as $field_name => $instance) {
        $field = field_info_field($field_name);
        if ($field['type'] == 'field_collection') {
          $field_collections[] = $field_name;
        }
      }
    }
  }

  return drupal_map_assoc($field_collections);
}
