<?php
/**
 * Message Private views modifications for OG.
 */

/**
 * Implements hook_views_default_views_alter().
 */
function message_private_og_views_default_views_alter(&$views) {
  if (isset($views['message_private']) && is_object($views['message_private'])) {
    // Get handler for existing display "Sent Messages" & add new Groups field.
    $handler =& $views['message_private']->display['page_2']->handler;
    $handler->display->display_options['defaults']['fields'] = FALSE;
    /* Field: User: Name */
    $handler->display->display_options['fields']['name']['id'] = 'name';
    $handler->display->display_options['fields']['name']['table'] = 'users';
    $handler->display->display_options['fields']['name']['field'] = 'name';
    $handler->display->display_options['fields']['name']['relationship'] = 'user';
    $handler->display->display_options['fields']['name']['label'] = 'From User';
    /* Field: Message: To Groups */
    $handler->display->display_options['fields']['field_message_group_ref']['id'] = 'field_message_group_ref';
    $handler->display->display_options['fields']['field_message_group_ref']['table'] = 'field_data_field_message_group_ref';
    $handler->display->display_options['fields']['field_message_group_ref']['field'] = 'field_message_group_ref';
    $handler->display->display_options['fields']['field_message_group_ref']['settings'] = array(
      'link' => 0,
    );
    $handler->display->display_options['fields']['field_message_group_ref']['delta_offset'] = '0';
    /* Field: Message: Message ID */
    $handler->display->display_options['fields']['mid']['id'] = 'mid';
    $handler->display->display_options['fields']['mid']['table'] = 'message';
    $handler->display->display_options['fields']['mid']['field'] = 'mid';
    $handler->display->display_options['fields']['mid']['exclude'] = TRUE;
    /* Field: Message: Subject */
    $handler->display->display_options['fields']['field_message_subject']['id'] = 'field_message_subject';
    $handler->display->display_options['fields']['field_message_subject']['table'] = 'field_data_field_message_subject';
    $handler->display->display_options['fields']['field_message_subject']['field'] = 'field_message_subject';
    $handler->display->display_options['fields']['field_message_subject']['alter']['make_link'] = TRUE;
    $handler->display->display_options['fields']['field_message_subject']['alter']['path'] = 'message/[mid]';
    /* Field: Message: View message */
    $handler->display->display_options['fields']['view']['id'] = 'view';
    $handler->display->display_options['fields']['view']['table'] = 'message';
    $handler->display->display_options['fields']['view']['field'] = 'view';
    $handler->display->display_options['fields']['view']['label'] = 'Operations';
    $handler->display->display_options['fields']['view']['element_label_colon'] = FALSE;
    /* Field: Message: Delete message */
    $handler->display->display_options['fields']['delete']['id'] = 'delete';
    $handler->display->display_options['fields']['delete']['table'] = 'message';
    $handler->display->display_options['fields']['delete']['field'] = 'delete';
    $handler->display->display_options['fields']['delete']['label'] = '';
    $handler->display->display_options['fields']['delete']['element_label_colon'] = FALSE;
    /* Field: Message: Timestamp */
    $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
    $handler->display->display_options['fields']['timestamp']['table'] = 'message';
    $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
    $handler->display->display_options['fields']['timestamp']['date_format'] = 'short';
    $handler->display->display_options['fields']['timestamp']['second_date_format'] = 'long';
    $handler->display->display_options['defaults']['arguments'] = FALSE;
    /* Contextual filter: Global: Null */
    $handler->display->display_options['arguments']['null']['id'] = 'null';
    $handler->display->display_options['arguments']['null']['table'] = 'views';
    $handler->display->display_options['arguments']['null']['field'] = 'null';
    $handler->display->display_options['arguments']['null']['default_argument_type'] = 'fixed';
    $handler->display->display_options['arguments']['null']['summary']['number_of_records'] = '0';
    $handler->display->display_options['arguments']['null']['summary']['format'] = 'default_summary';
    $handler->display->display_options['arguments']['null']['summary_options']['items_per_page'] = '25';
    /* Contextual filter: Message: To Groups (field_message_group_ref) */
    $handler->display->display_options['arguments']['field_message_group_ref_target_id']['id'] = 'field_message_group_ref_target_id';
    $handler->display->display_options['arguments']['field_message_group_ref_target_id']['table'] = 'field_data_field_message_group_ref';
    $handler->display->display_options['arguments']['field_message_group_ref_target_id']['field'] = 'field_message_group_ref_target_id';
    $handler->display->display_options['arguments']['field_message_group_ref_target_id']['default_action'] = 'default';
    $handler->display->display_options['arguments']['field_message_group_ref_target_id']['default_argument_type'] = 'og_user_groups';
    $handler->display->display_options['arguments']['field_message_group_ref_target_id']['summary']['number_of_records'] = '0';
    $handler->display->display_options['arguments']['field_message_group_ref_target_id']['summary']['format'] = 'default_summary';
    $handler->display->display_options['arguments']['field_message_group_ref_target_id']['summary_options']['items_per_page'] = '25';
    $handler->display->display_options['defaults']['filter_groups'] = FALSE;
    $handler->display->display_options['defaults']['filters'] = FALSE;
    /* Filter criterion: Message: Type */
    $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
    $handler->display->display_options['filters']['type_1']['table'] = 'message';
    $handler->display->display_options['filters']['type_1']['field'] = 'type';
    $handler->display->display_options['filters']['type_1']['value'] = array(
      'private_message' => 'private_message',
    );
    /* Filter criterion: Message: To Groups (field_message_group_ref) */
    $handler->display->display_options['filters']['field_message_group_ref_target_id']['id'] = 'field_message_group_ref_target_id';
    $handler->display->display_options['filters']['field_message_group_ref_target_id']['table'] = 'field_data_field_message_group_ref';
    $handler->display->display_options['filters']['field_message_group_ref_target_id']['field'] = 'field_message_group_ref_target_id';
    $handler->display->display_options['filters']['field_message_group_ref_target_id']['exposed'] = TRUE;
    $handler->display->display_options['filters']['field_message_group_ref_target_id']['expose']['operator_id'] = 'field_message_group_ref_target_id_op';
    $handler->display->display_options['filters']['field_message_group_ref_target_id']['expose']['label'] = 'Groups';
    $handler->display->display_options['filters']['field_message_group_ref_target_id']['expose']['operator'] = 'field_message_group_ref_target_id_op';
    $handler->display->display_options['filters']['field_message_group_ref_target_id']['expose']['identifier'] = 'field_message_group_ref_target_id';
    $handler->display->display_options['filters']['field_message_group_ref_target_id']['expose']['remember_roles'] = array(
      2 => '2',
      1 => 0,
      3 => 0,
    );
    $handler->display->display_options['path'] = 'user/%/messages/group';
    $handler->display->display_options['menu']['type'] = 'tab';
    $handler->display->display_options['menu']['title'] = 'Group';
    $handler->display->display_options['menu']['weight'] = '12';
    $handler->display->display_options['menu']['context'] = 0;
    $handler->display->display_options['menu']['context_only_inline'] = 0;
  }
}
