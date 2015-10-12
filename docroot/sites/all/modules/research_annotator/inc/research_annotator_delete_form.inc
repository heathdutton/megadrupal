<?php
/**
 * @file: Functionality and handlers for the annotation deletion form.
 */

/**
 * Validation handler for the annotation edit form.
 */
function research_annotator_validate_deleted_annotation($form, &$form_state) {
  
  if (!research_annotator_access_edit_page($form_state['values']['annotation_id'])) {
    form_set_error('annotation_delete', t('Sorry, you are not allowed to delete this annotation.'));
  }
  
}

/**
 * Submission handler for the annotation edit form.
 */
function research_annotator_submit_deleted_annotation($form, &$form_state) {
  $o_annotation = array_pop(entity_load('research_annotation', array($form_state['values']['annotation_id'])));
  
  $b_deletedStatus = entity_delete('research_annotation', $form_state['values']['annotation_id']);
  
  if ($b_deletedStatus === FALSE) {
    drupal_set_message(t('This annotation could not be deleted.'), 'error');
  }
  else {
    drupal_set_message(t('Your annotation has been deleted successfully.'));
    $o_annotatedNode = node_load($o_annotation->node_id);
    $s_redirectAlias = drupal_get_path_alias('node/' . $o_annotatedNode->nid);
    drupal_goto($s_redirectAlias);
  }
  
}
