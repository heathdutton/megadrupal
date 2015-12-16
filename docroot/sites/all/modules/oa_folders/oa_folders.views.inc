<?php



/**
 * Implements hook_views_data().
 */
function oa_folders_views_data() {
  $data = array();
  $data['node']['field_folder_toolbox'] = array(
    'title' => t('Folder toolbox'),
    'help' => t('Toolbox for open atrium folders files.'),
    'field' => array(
      'handler' => 'oa_folders_handler_field_folder_toolbox',
    ),
  );
  return $data;
}
