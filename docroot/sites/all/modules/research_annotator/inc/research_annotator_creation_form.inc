<?php
/**
 * @file: Handlers and other functionality for the annotation creation form.
 */

/*
 * Validation handler for annotation creation form.
 */
function research_annotator_validate_annotation_creation($form, &$form_state) {
  
  if (user_access('research_annotator_create_annotation') == FALSE) {
    form_set_error('annotation_creation', t('Sorry, you do not have permission to create an annotation.'));
  }
  elseif ($form_state['values']['paragraph_index'] == '' || !is_numeric($form_state['values']['paragraph_index'])) {
    form_set_error('paragraph_index', t('Please select the paragraph you want to annotate.'));
  }
  
}

/*
 * Submission handler for annotation creation form.
 */
function research_annotator_submit_annotation_creation($form, &$form_state) {
  
  global $user;
  $o_currentNode = research_annotator_get_annotation_node();
  
  // Create and prepare the entity.
  $o_entity = entity_create(
    'research_annotation',
    array(
      'node_id' => (int) $o_currentNode->nid,
      'uid' => (int) $user->uid,
      'paragraph_index' => $form_state['values']['paragraph_index'],
      'annotation' => $form_state['values']['annotation_body'],
      'annotation_format' => $form_state['values']['annotation_body']['format'],
      'created' => REQUEST_TIME,
      'updated' => REQUEST_TIME,
    )
  );
  
  $o_entity->annotation = $o_entity->annotation['value'];
  
  // Save the entity.
  $b_success = entity_save('research_annotation', $o_entity);
  
  if ($b_success) {
    drupal_set_message(t('Your annotation has been saved successfully.'));
  }
  else {
    drupal_set_message(t('Your annotation could not be saved.'), 'error');
  }
  
}