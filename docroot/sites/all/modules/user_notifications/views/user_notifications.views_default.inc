<?php
/**
 * Views for the default user notifications
 */

/**
 * Implements hook_views_default_views().
 */
function user_notifications_views_default_views() {
  $views = array();

  $view = new view();
  $view->name = 'user_notifications_admin';
  $view->description = 'List all user notifications';
  $view->tag = 'user notifications';
  $view->base_table = 'user_notifications';
  $view->human_name = 'User Notifications';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'User Notifications Admin Overview';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer user notifications';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: User notification: Date created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'user_notifications';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'long';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: User notification: User Notification type */
  $handler->display->display_options['fields']['type_1']['id'] = 'type_1';
  $handler->display->display_options['fields']['type_1']['table'] = 'user_notifications';
  $handler->display->display_options['fields']['type_1']['field'] = 'type';
  $handler->display->display_options['fields']['type_1']['label'] = 'Notification Type';
  /* Field: User notification: State entity */
  $handler->display->display_options['fields']['state']['id'] = 'state';
  $handler->display->display_options['fields']['state']['table'] = 'user_notifications';
  $handler->display->display_options['fields']['state']['field'] = 'state';
  $handler->display->display_options['fields']['state']['label'] = 'Notification State';
  /* Field: User notification: Author_uid */
  $handler->display->display_options['fields']['author_uid']['id'] = 'author_uid';
  $handler->display->display_options['fields']['author_uid']['table'] = 'user_notifications';
  $handler->display->display_options['fields']['author_uid']['field'] = 'author_uid';
  $handler->display->display_options['fields']['author_uid']['label'] = 'Author UID';
  /* Field: User notification: Notification Recipient */
  $handler->display->display_options['fields']['user_notifications_recipient_uid']['id'] = 'user_notifications_recipient_uid';
  $handler->display->display_options['fields']['user_notifications_recipient_uid']['table'] = 'field_data_user_notifications_recipient_uid';
  $handler->display->display_options['fields']['user_notifications_recipient_uid']['field'] = 'user_notifications_recipient_uid';
  $handler->display->display_options['fields']['user_notifications_recipient_uid']['settings'] = array(
    'link' => 1,
  );
  /* Field: User notification: URL */
  $handler->display->display_options['fields']['url']['id'] = 'url';
  $handler->display->display_options['fields']['url']['table'] = 'views_entity_user_notifications';
  $handler->display->display_options['fields']['url']['field'] = 'url';
  $handler->display->display_options['fields']['url']['link_to_entity'] = 1;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/content/user-notifications';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'User Notifications';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'User Notifications';
  $handler->display->display_options['tab_options']['description'] = 'List all user notifications';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';

  $views[$view->name] = $view;

  $view = new view();
  $view->name = 'user_notifications';
  $view->description = 'Display a user\'s notifications';
  $view->tag = 'user_notifications';
  $view->base_table = 'user_notifications';
  $view->human_name = 'User Notifications';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'User Notifications';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view own user notifications';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'entity';
  /* Field: User notification: User Notification type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'user_notifications';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  /* Contextual filter: User notification: Notification Recipient (user_notifications_recipient_uid) */
  $handler->display->display_options['arguments']['user_notifications_recipient_uid_target_id']['id'] = 'user_notifications_recipient_uid_target_id';
  $handler->display->display_options['arguments']['user_notifications_recipient_uid_target_id']['table'] = 'field_data_user_notifications_recipient_uid';
  $handler->display->display_options['arguments']['user_notifications_recipient_uid_target_id']['field'] = 'user_notifications_recipient_uid_target_id';
  $handler->display->display_options['arguments']['user_notifications_recipient_uid_target_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['user_notifications_recipient_uid_target_id']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['user_notifications_recipient_uid_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['user_notifications_recipient_uid_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['user_notifications_recipient_uid_target_id']['summary_options']['items_per_page'] = '25';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'user-notifications';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'My Notifications';
  $handler->display->display_options['menu']['description'] = 'Display all your notifications';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'user-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  $views[$view->name] = $view;

  return $views;
}
