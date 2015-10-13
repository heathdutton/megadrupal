<?php

/**
 * @file
 * Token callbacks for the Entity Review module.
 */

/**
 * Get all existing review types and expected data types.
 *
 * @return array
 *  Array of review types.
 *  Format: entity_type => expected_data_type
 */
function _entity_review_get_token_types() {
  $types = &drupal_static(__FUNCTION__);

  if (!isset($types)) {
    $types = db_select('entity_review', 'e')
      ->fields('e', array('type'))
      ->distinct()
      ->execute()
      ->fetchCol();

    $types = array_combine(array_values($types), array_values($types));

    //@HACK: For some reasons a token for 'taxonomy_term' is just 'term'.
    if (isset($types['taxonomy_term'])) {
      $types['taxonomy_term'] = 'term';
    }
  }

  return $types;
}

/**
 * Implements hook_token_info().
 */
function entity_review_token_info() {
  $tokens = array();
  foreach (_entity_review_get_token_types() as $type => $expected_data_type) {
    $tokens[$type] = array(
      'name' => t('Related %type', array('%type' => $type)),
      'description' => t('Related %type.', array('%type' => $type)),
      'type' => $expected_data_type,
    );
  }

  return array(
    'tokens' => array(
      'review' => $tokens,
    ),
  );
}

/**
 * Implements hook_tokens().
 */
function entity_review_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $output = array();

  if ($type == 'review' && !empty($data['review'])) {
    $review = $data['review'];

    foreach (_entity_review_get_token_types() as $type => $expected_data_type) {
      if ($scope_tokens = token_find_with_prefix($tokens, $type)) {
        $entities = entity_load($type, array($review->eid));

        $scope_data = array(
          $expected_data_type => reset($entities),
        );

        foreach (token_generate($expected_data_type, $scope_tokens, $scope_data, $options) as $original => $value) {
          $output[$original] = $value;
        }
      }
    }
  }

  return $output;
}