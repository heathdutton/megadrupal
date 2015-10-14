<?php
/**
 * @file
 * Hooks for the Token module.
 */

/**
 * Implements hook_token_info_alter().
 */
function vocabulary_queue_token_info_alter(&$info) {
  $info['tokens']['term']['parents-all'] = array(
    'name' => t('Parents including self.'),
    'description' => t('Parents including self.'),
    'type' => 'array',
  );
}

/**
 * Implements hook_tokens().
 */
function vocabulary_queue_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  $sanitize = !empty($options['sanitize']);

  if ($type == 'term' && !empty($data['term'])) {
    $term = $data['term'];
    // [term:parents-all:*] chained tokens.
    if ($parents_tokens = token_find_with_prefix($tokens, 'parents-all')) {
      $terms = array_reverse(taxonomy_get_parents_all($term->tid));
      foreach ($terms as &$t) {
        $t = $t->name;
      }
      $replacements += token_generate('array', $parents_tokens, array('array' => $terms), $options);
    }
  }
  return $replacements;
}
