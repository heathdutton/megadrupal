<?php

/**
 * @file
 * Providing extra functionality for the Avatar UI via views.
 */


/**
 * Implement hook_views_data_alter().
 */
function alter_ego_views_data_alter(&$data) {
  
  $data['avatar']['name']['field']['handler'] = 'avatar_handler_name_field';
  
  
  $data['avatar']['link_avatar'] = array(
    'field' => array(
      'title' => t('Link'),
      'click sortable' => TRUE,
      'help' => t('Provide a link to the avatar.'),
      'handler' => 'avatar_handler_link_field',
    ),
  );
  $data['avatar']['edit_avatar'] = array(
    'field' => array(
      'title' => t('Edit Link'),
      'help' => t('Provide a link to the edit form for the avatar.'),
      'handler' => 'avatar_handler_edit_link_field',
    ),
  );
  $data['avatar']['delete_avatar'] = array(
    'field' => array(
      'title' => t('Delete Link'),
      'help' => t('Provide a link to delete the avatar.'),
      'handler' => 'avatar_handler_delete_link_field',
    ),
  );
  // This content of this field are decided based on the menu structure that
  // follows avatars/avatar/%aid/op
  $data['avatar']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this avatar.'),
      'handler' => 'avatar_handler_avatar_operations_field',
    ),
  );
}
