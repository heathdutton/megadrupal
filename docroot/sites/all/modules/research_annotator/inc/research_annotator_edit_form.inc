<?php
/**
 * @file: Functionality and handlers for the annotation edit form.
 */

/**
 * Validation handler for the annotation edit form.
 */
function research_annotator_validate_edited_annotation($form, &$form_state) {
   
  if (!research_annotator_access_edit_page($form_state['values']['annotation_id'])) {
    form_set_error('annotation_edit', t('Sorry, you are not allowed to edit this annotation.'));
  }
  elseif ($form_state['values']['paragraph_index'] == '' || !is_numeric($form_state['values']['paragraph_index'])) {
    form_set_error('paragraph_index', t('The edited annotation must have a numeric paragraph index.'));
  }
  
}

/**
 * Submission handler for the annotation edit form.
 */
function research_annotator_submit_edited_annotation($form, &$form_state) {
  $o_annotation = array_pop(entity_load('research_annotation', array($form_state['values']['annotation_id'])));
  
  $o_annotation->annotation = $form_state['values']['annotation_body']['value'];
  $o_annotation->annotation_format = $form_state['values']['annotation_body']['format'];
  $o_annotation->updated = REQUEST_TIME;
  $m_savedStatus = entity_save('research_annotation', $o_annotation);
  
  if ($m_savedStatus === FALSE || $m_savedStatus == 1) {
    drupal_set_message(t('This annotation could not be edited.'), 'error');
  }
  else {
    drupal_set_message(t('Your annotation has been successfully edited.'));
    $o_annotatedNode = node_load($o_annotation->node_id);
    $s_redirectAlias = drupal_get_path_alias('node/' . $o_annotatedNode->nid);
    drupal_goto($s_redirectAlias);
  }
  
}