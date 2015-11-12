<?php

/**
 * @file
 * CTools content type for a clinks relationship edit form
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Party Relationship Form'),
  'content_types' => 'party_relation_relationship_form',
  // 'single' means not to be subtyped.
  'single' => TRUE,
  // Name of a function which will render the block.
  'render callback' => 'party_relation_relationship_form_render',

  // Icon goes in the directory with the content type.
  'description' => t('Show a relationship add form.'),
  'required context' => array(
    new ctools_context_required(t('Party'), 'entity:party'),
    new ctools_context_optional(t('Secondary Party (optional)'), 'entity:party'),
  ),
  'edit form' => 'party_relation_relationship_form_edit_form',
  'admin title' => 'party_relation_relationship_form_admin_title',

  // presents a block which is used in the preview of the data.
  // Pn Panels this is the preview pane shown on the panels building page.
  'category' => array(t('Party'), 0),
);

/**
 * Render the Party Attached Entity Edit Form
 *
 * @param $subtype
 * @param $conf
 *   Configuration as done at admin time
 * @param $args
 * @param $context
 *   Context - in this case we don't have any
 *
 * @return
 *   An object with at least title and content members
 */
function party_relation_relationship_form_render($subtype, $conf, $args, $context) {
  $block = new stdClass();
  $block->title = t('Create a relationship.');
  $block->content = '';

  if (!empty($context[0]->data)) {
    $party = $context[0]->data;
    $relation_type = $conf['relation_type'];
    $direction = $conf['endpoint'];

    $other_party = empty($context[1]->data) ? NULL : $context[1]->data;

    $form = party_relation_create_relationship_form_wrapper($party, $relation_type, $direction, $other_party, $conf);

    // Render it...
    $block->content = drupal_render($form);
  }

  return $block;
}

/**
 * Config Form
 */
function party_relation_relationship_form_edit_form($form, &$form_state) {
  $relation_types = relation_get_types();
  $options = array();

  foreach ($relation_types as $types => $object) {
    $options[$types] = $object->label;
  }

  $form['relation_type'] = array(
    '#title' => t('Relation type'),
    '#description' => t('Which type of relation is this form adding?'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => !empty($form_state['conf']['relation_type']) ? $form_state['conf']['relation_type'] : 'works_for',
    '#required' => TRUE,
  );

  $form['endpoint'] = array(
    '#title' => t('Context Endpoint'),
    '#description' => t('Which enpoint does the Party context sit at?'),
    '#type' => 'select',
    '#options' => array(
      'source' => t('Source (Employee)'),
      'target' => t('Target (Employer)'),
    ),
    '#default_value' => !empty($form_state['conf']['endpoint']) ? $form_state['conf']['endpoint'] : 'source',
    '#required' => TRUE,
  );

  $hats = array('All' => t('- Any -'));
  foreach (party_hat_get_all_hats() as $hat_name => $hat) {
    $hats[$hat_name] = $hat->label;
  }
  $form['hats'] = array(
    '#title' => t('Restrict by hats'),
    '#description' => t("Autocomplete field's suggestions can be restricted by party hats."),
    '#type' => 'select',
    '#options' => $hats,
    '#default_value' => !empty($form_state['conf']['hats']) ? $form_state['conf']['hats'] : 'All',
  );
  $form['message'] = array(
    '#title' => t('Message'),
    '#description' => t("Display a message on successful submission."),
    '#type' => 'textfield',
    '#default_value' => !empty($form_state['conf']['message']) ? $form_state['conf']['message'] : t('Relation has been created successfully.'),
  );
  $form['source_title'] = array(
    '#title' => t('Source Party Title'),
    '#description' => t('The title of the source entity field.'),
    '#type' => 'textfield',
    '#default_value' => !empty($form_state['conf']['source_title']) ? $form_state['conf']['source_title'] : 'Source',
  );
  $form['target_title'] = array(
    '#title' => t('Target Party Title'),
    '#description' => t('The title of the target entity field.'),
    '#type' => 'textfield',
    '#default_value' => !empty($form_state['conf']['target_title']) ? $form_state['conf']['target_title'] : 'Target',
  );

  return $form;
}

function party_relation_relationship_form_edit_form_submit(&$form, &$form_state) {
  foreach (element_children($form) as $key) {
    if (!empty($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}

/**
 * Title Callback
 */
function party_relation_relationship_form_admin_title($subtype, $conf, $context = NULL) {
  return "Relationship Form";
}
