<?php

/**
 * @file
 * Taxonomy Machine Name Token Module File.
 */

/**
 * Implements hook_token_info_alter().
 */
function taxonomy_machine_name_token_info_alter(&$data) {
  // Modify description of node tokens for our site.
  $data['tokens']['term']['machine-name'] = array(
    'name'        => t('Machine name'),
    'description' => t('The machine name of the taxonomy term.'),
  );
}

/**
 * Implements hook_tokens_alter().
 */
function taxonomy_machine_name_tokens_alter(array &$replacements, array $context) {
  if (isset($context['tokens']['machine-name']) && $token = $context['tokens']['machine-name']) {
    $term     = FALSE;
    $sanitize = !empty($options['sanitize']);

    if ($context['type'] == 'term') {
      if (isset($context['data']['term'])) {
        $term = $context['data']['term'];
      }
      elseif (isset($context['data']['taxonomy_term'])) {
        $term = $context['data']['taxonomy_term'];
      }
    }
    elseif ($context['type'] == 'entity' && $context['data']['entity_type'] == 'taxonomy_term') {
      $term = $context['data']['entity'];
    }

    if ($term) {
      $replacements[$token] = $sanitize ? filter_xss($term->machine_name) : $term->machine_name;
    }
  }
}
