<?php
/*
 * @file: Additional functions for administrative tasks and interfaces.
 */

/*
 * Validation Handler.
 * Makes sure annotation settings are properly validated.
 */
function research_annotator_validate($form, &$form_state) {
  
  if (
    $form_state['values']['research_annotator_settings']['research_annotator_enabled'] !== 0 &&
    $form_state['values']['research_annotator_settings']['research_annotator_block_region'] === '0'
  ) {
    form_set_error('research_annotator_settings][research_annotator_block_region', t('Please select a block region for your annotations.'));
  }
  if (
    $form_state['values']['research_annotator_settings']['research_annotator_enabled'] !== 0 &&
    $form_state['values']['research_annotator_settings']['research_annotator_date_format'] === ''
  ) {
    form_set_error('research_annotator_settings][research_annotator_date_format', t('Please specify a date format for your annotations.'));
  }
  
}

/*
 * Submission Handler.
 * Handles the setting of annotation variables.
 */
function research_annotator_submit($form, &$form_state) {
  
  // Get a field identifier so we can save variables on a per content type basis.
  $s_fieldIdentifier = $form['#instance']['field_name'] . '_' . $form['#instance']['bundle'];
  
  if ($form_state['values']['research_annotator_settings']['research_annotator_enabled'] == 0) {
    variable_del('research_annotator_' . $s_fieldIdentifier . '_enabled');
    variable_del('research_annotator_' . $s_fieldIdentifier . '_date_format');
    variable_del('research_annotator_' . $s_fieldIdentifier . '_block_region');
    variable_del('research_annotator_' . $s_fieldIdentifier . '_block_weight');
    variable_del('research_annotator_' . $s_fieldIdentifier . '_cache_disabled');
  }
  else {
    variable_set(
      'research_annotator_' . $s_fieldIdentifier . '_enabled', 
      $form_state['values']['research_annotator_settings']['research_annotator_enabled']
    );
    
    variable_set(
      'research_annotator_' . $s_fieldIdentifier . '_date_format', 
      $form_state['values']['research_annotator_settings']['research_annotator_date_format']
    );

    variable_set(
      'research_annotator_' . $s_fieldIdentifier . '_block_region', 
      $form_state['values']['research_annotator_settings']['research_annotator_block_region']
    );

    variable_set(
      'research_annotator_' . $s_fieldIdentifier . '_block_weight', 
      (integer) $form_state['values']['research_annotator_settings']['research_annotator_block_weight']
    );
    
    variable_set(
      'research_annotator_' . $s_fieldIdentifier . '_cache_disabled', 
      $form_state['values']['research_annotator_settings']['research_annotator_cache_settings']
    );

  }
}

/*
 * Helper function.
 * Returns select list options for the block region field.
 */
function research_annotator_get_block_regions() {

  $arr_blockRegions = system_region_list(variable_get('theme_default'), REGIONS_VISIBLE);
  array_unshift($arr_blockRegions, t('- None -'));
  
  return $arr_blockRegions;
}

/**
 * Submission handler.
 * Handles which annotations are kept while on node edit pages.
 */
function research_annotator_update_kept_revisions($form, &$form_state) {
  
  $b_entitesHaveBeenDeleted = FALSE;
  
  foreach ($form_state['values']['body']['research_annotations'] as $s_annotationId => $m_isKept) {
    if ($m_isKept === 0) {
      $b_entitesHaveBeenDeleted = entity_delete('research_annotation', $s_annotationId);
    }
  }
  
  if ($b_entitesHaveBeenDeleted !== FALSE) {
    drupal_set_message(t('You have successfully removed the annotations that you have unchecked.'), 'status', FALSE);
  }
}
