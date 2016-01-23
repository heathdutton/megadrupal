<?php

/**
 * @file
 * Tokens for selected facet fields.
 */

/**
 * Implements hook_token_info().
 */
function metatag_taxonomy_facets_token_info() {
  $token_types = array();
  $token_types['taxonomy_facets'] = array(
    'name' => t('Taxonomy facets'),
    'description' => t('Tokens related active taxonomy facets.'),
  );

  $tokens = array();

  $facets_selected = variable_get('metatag_taxonomy_facets_selected');
  $searchers = facetapi_get_searcher_info();
  foreach ($searchers as $searcher_name => $searcher_info) {
    $facets = facetapi_get_facet_info($searcher_name);
    foreach ($facets as $facet_name => $facet_info) {
      if (metatag_taxonomy_facets_is_taxonomy_facet($facet_info) && isset($facets_selected[$searcher_name][$facet_name])) {
        $token_name = str_replace(':', '-', $facet_name);
        $tokens[$token_name] = array(
          'name' => $facet_info['label'],
          'description' => "{$searcher_info['label']} {$facet_info['label']}",
          'type' => 'term',
        );
      }
    }
  }

  return array(
    'types' => $token_types,
    'tokens' => array(
      'taxonomy_facets' => $tokens,
    ),
  );
}

/**
 * Implements hook_tokens().
 */
function metatag_taxonomy_facets_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  if ($type != 'taxonomy_facets') {
    return $replacements;
  }

  foreach ($tokens as $name => $original) {
    $name_parts = explode(':', $name);
    if (count($name_parts) < 2) {
      continue;
    }
    $term_name = $name_parts[0];
    $term_tokens = token_find_with_prefix($tokens, $term_name);
    if ($term_tokens && isset($data['taxonomy_facets']->tokens)) {
      $replacements += token_generate('term', $term_tokens, array(
        'term' => $data['taxonomy_facets']->tokens[str_replace('-', ':', $term_name)],
      ), $options);
    }
  }

  return $replacements;
}
