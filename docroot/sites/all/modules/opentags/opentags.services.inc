<?php
/**
 * @file
 * Provide a set of services.
 */

/**
 * Page callback of opentags/services/get-terms-data.
 *
 * Returns the json encoded data of specific vocabulary, which can be used as
 * data source for the OpenTags UI widgets.
 *
 * Returned json format:
 *
 *   - success: If the term data is retrieved successfully, it's set to 1.
 *     Otherwise, 0.
 *   - error: If it fails, this property will have the related error message.
 *   - data: The returned terms data.
 *
 * e.g.
 *
 *   - {"success":1,"data":[{"tid":"31","label":"Foo","value":"foo"}]}
 *   - {"success":0,"error":"vocabulary name is invalid"}
 */
function opentags_service_get_terms_data() {
  $output = array(
    'success' => 0,
  );

  if (isset($_REQUEST['vocab'])) {
    $vocab_name = $_REQUEST['vocab'];
    $vocab = taxonomy_vocabulary_machine_name_load($vocab_name);
    if ($vocab) {
      $fields = field_info_instances('taxonomy_term', $vocab_name);
      if (isset($fields['field_opentags_term_value']) && isset($fields['field_opentags_term_disabled'])) {
        $terms = taxonomy_get_tree($vocab->vid, 0, NULL, TRUE);
        if (count($terms) > 0) {
          $output['success'] = 1;
          $output['data'] = opentags_create_widget_data_source($terms);
        }
        else {
          $output['error'] = 'no term exists in the vocabulary';
        }
      }
      else {
        $output['error'] = 'vocabulary structure is corrupt';
      }
    }
    else {
      $output['error'] = 'vocabulary name is invalid';
    }
  }
  else {
    $output['error'] = 'vocabulary name not specified';
  }

  drupal_json_output($output);
}

/**
 * Convert a array of Drupal term objects to a nested term array.
 *
 * @param array $raw_terms
 *   An array of Drupal term objects.
 *
 * @return array
 *   The converted term array ready used as widget data source.
 */
function opentags_create_widget_data_source(array $raw_terms) {
  $terms = array();
  // Add level 1 terms.
  for ($i = 0; $i < count($raw_terms); $i++) {
    if ($raw_terms[$i]->parents[0] == 0) {
      $terms[] = opentags_create_widget_data_source_item($raw_terms[$i]);
    }
  }
  // Append child terms.
  for ($i = 0; $i < count($terms); $i++) {
    opentags_append_child_term($raw_terms, $terms[$i]);
  }

  return $terms;
}

/**
 * Convert a Drupal term object to a term item used for the widget.
 *
 * @param object $raw_term
 *   A Drupal term object.
 *
 * @return array
 *   An associative array present a term item.
 */
function opentags_create_widget_data_source_item($raw_term) {
  $term = array();
  $term['tid'] = $raw_term->tid;
  $term['label'] = $raw_term->name;
  if (isset($raw_term->field_opentags_term_value)) {
    $term['value'] = $raw_term->field_opentags_term_value[LANGUAGE_NONE][0]['value'];
  }
  else {
    $term['value'] = $raw_term->name;
  }
  if (isset($raw_term->field_opentags_term_disabled)) {
    if ($raw_term->field_opentags_term_disabled[LANGUAGE_NONE][0]['value']) {
      $term['disabled'] = TRUE;
    }
  }
  return $term;
}

/**
 * Append child terms to parent terms to build nested term structure.
 *
 * @param array $raw_terms
 *   A list of all terms.
 * @param array $parent_term
 *   A term item. Passed by reference.
 */
function opentags_append_child_term(array $raw_terms, array &$parent_term) {
  $child_terms = array();
  for ($i = 0; $i < count($raw_terms); $i++) {
    if ($raw_terms[$i]->parents[0] == $parent_term['tid']) {
      $child_terms[] = opentags_create_widget_data_source_item($raw_terms[$i]);
    }
  }
  if (count($child_terms) > 0) {
    $parent_term['items'] = $child_terms;
    for ($j = 0; $j < count($parent_term['items']); $j++) {
      opentags_append_child_term($raw_terms, $parent_term['items'][$j]);
    }
  }
}
