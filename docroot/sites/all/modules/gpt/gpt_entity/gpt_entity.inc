<?php

/**
 * Attach GPT targeting form to entities.
 *
 * @param array $entity_type
 * @param object $entity
 * @param array $form
 * @param array $form_state
 * @param string $langcode
 */
function gpt_entity_form($entity_type, $entity, &$form, &$form_state, $langcode) {
  // Certain modules do not use $form_state[$entity_type] for storage of their
  // $entity upon submit, but instead something that is not retrievable
  // (ie custom, such as "term" instead of "taxonomy_term"). Instead of making
  // switch statements that would have to be maintained, we just attach the
  // entity here.
  $form_state[$entity_type] = $entity;

  // Load defaults.
  $data = array();
  list($identifier) = entity_extract_ids($entity_type, $entity);
  if ($identifier) {
    $data = gpt_entity_crud_load($entity->entity_type, $identifier);
  }

  // Loaded for subform.
  ctools_include('gpt.admin', 'gpt', FALSE);
  _gpt_admin_attach_css();

  // Define form.
  $form['gpt_entity'] = array(
    '#type' => 'fieldset',
    '#title' => t('Google Publisher Tags'),
    '#group' => 'additional_settings',
    'gpt_entity_wrapper' => array(
      '#prefix' => '<div id="gpt-entity-wrapper">',
      '#suffix' => '</div>',
      '#type' => 'subform',
      '#subform_id' => 'gpt_settings_form',
      '#subform_file' => array('module' => 'gpt', 'name' => 'gpt.admin.inc'),
      // We don't need to pass arguments as the subform will populate from global.
      '#subform_arguments' => array($data),
      '#subform_default_triggering_element' => array('submit'),
      '#required' => TRUE,
    )
  );
  // Enable error validation.
  $form['actions']['submit']['#submit'][] = 'subform_submit_all';
  $form['actions']['submit']['#submit'][] = 'gpt_entity_form_submit';
  // Attach form as a vertical tab, if the form supports them.
  foreach (element_children($form) as $key) {
    if (isset($form[$key]['#type']) && $form[$key]['#type'] == 'vertical_tabs') {
      $form['gpt_entity']['#group'] = $key;
      // TODO: Add JS for tab label.
      break;
    }
  }

  // TODO: Remove gpt_entity_form_alter() if http://drupal.org/node/1284642 is
  // fixed in core and uncomment the line below.
  //$form['#submit'][] = 'gpt_entity_form_submit';
}

/**
 * Submit handler for GPT entity form.
 */
function gpt_entity_form_submit_handler(&$form, &$form_state) {
  // For some reason we are getting two runs on this submit handler. The first
  // should be ignored.
  if (!isset($form_state['temporary']['subform']['subform-gpt_entity_wrapper']['prepared_values'])) {
    return;
  }
  $data = $form_state['temporary']['subform']['subform-gpt_entity_wrapper']['prepared_values'];

  // Remove non-assignments.
  foreach ($data as $k => $v) {
    if ($v === NULL) {
      unset($data[$k]);
    }
  }

  // Retrieve the identifier for the entity.
  list($id) = entity_extract_ids($form['#entity_type'], $form_state[$form['#entity_type']]);

  // If no data, then attempt a delete.
  if (empty($data)) {
    gpt_entity_crud_delete($form['#entity_type'], $id);
  }
  // Else save.
  else {
    gpt_entity_crud_save($form['#entity_type'], $id, $data);
  }
}
