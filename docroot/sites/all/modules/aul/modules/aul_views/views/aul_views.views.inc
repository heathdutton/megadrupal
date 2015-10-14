<?php

/**
 * Implements of hook_views_data().
 */
function aul_views_views_data() {
  
  $data = array();
  
  $data['node'] = array(
    'aul_grants' => array(
      'title' => t('Grants'),
      'help' => t('Represents all user\'s AUL grants'),
      'field' => array(
        'handler' => 'views_handler_aul_grants_field',
      ),
      'group' => t('AUL'),
    ),
    'aul_user_grants' => array(
      'title' => t('User context grants'),
      'help' => t('Represents user\'s AUL grants'),
      'field' => array(
        'handler' => 'views_handler_aul_user_grants_field',
      ),
      'group' => t('AUL'),
    ),
    'aul_user_context' => array(
      'title' => t('User'),
      'help' => t('User context for AUL grants'),
      'filter' => array(
        'label' => t('AUL user'),
        'help' => t('Filters all grants values by user.'),
        'handler' => 'views_handler_aul_user_context_filter',
      ),
      'group' => t('AUL'),
    ),
  );

  return $data;
}
