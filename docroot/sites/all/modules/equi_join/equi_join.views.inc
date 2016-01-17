<?php 

/**
 * Implements of hook_views_data()
 */
function equi_join_views_data() {
  $data['equi_join']['table']['group'] = t('Equi Join');  
  $data['equi_join']['table']['join'] = array(
    '#global' => array(),
  );
  return $data;
}

/**
 * Implements of hook_views_data_alter()
 */
function equi_join_views_data_alter(&$data) {
  foreach($data as $key => $entity) {
    if (isset($entity['table']['base'])) {
      // Add relationship to user table
      $data['equi_join'][$key] = array(
        'title' => $entity['table']['base']['title'],
        'help' => t('Join items of the given entity based on a specific field.'),
        'relationship' => array(
          'handler' => 'equi_join_handler_relationship',
          'field' => $entity['table']['base']['field'],
          'base' => $key,
        ),
      );
    }
  }
}
