<?php

/**
 * Implements of hook_views_data().
 */
function aul_roles_views_data() {
  
  $data = array();

  $data['node']['aul_roles_grants'] = array(
    'title' => t('AUL Roles grants'),
    'help' => t('Represents role\'s AUL grants'),
    'field' => array(
      'handler' => 'views_handler_aul_roles_grants_field',
    ),
  );

  return $data;
}
