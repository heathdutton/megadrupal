<?php
/**
 * Implements hook_rules_action_info().
 */
function chatroom_archiving_rules_action_info() {
  $actions = array(
      'chatroom_archiving_archive_old_chats' => array(
          'label' => t('Archive chatroom nodes'),
          'group' => t('Chatroom'),
          'parameter' => array(
              'archivaed_node_types' => array(
                  'type' => 'list<text>',
                  'label' => t('Archivaed Node Types'),
                  'options list' => 'chatroom_archiving_bundles',
              ),
            ),
          )
      );
  return $actions;
}
function chatroom_archiving_bundles(){
  $field_types = field_info_field_by_ids();
  $bundles = array();
  foreach ($field_types as $field_info){
    if ($field_info['type'] == 'chatroom'){
      if (!empty ($field_info["bundles"]["node"]))
        foreach ($field_info["bundles"]["node"] as $bundle) {
          if(!in_array($bundle, $bundles))
                  $bundles[$bundle] = $bundle;
        }
      }
  }
  return $bundles;
}