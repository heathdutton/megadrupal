<?php

/**
 * Implements hook_views_data().
 */
function role_indicators_views_data() {
  $data = array();
  // Roles with view access to a term
  $data['taxonomy_term_data']['role_indicators_view'] = array(
    'title' => t('Role indicator'),
    'help' => t('Markup indicating which roles have view access to the given term.'),
    'field' => array(
      'handler' => 'role_indicators_handler_field_term_grants',
    ),
  );
  return $data;
}
