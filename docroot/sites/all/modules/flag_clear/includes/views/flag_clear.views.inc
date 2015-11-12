<?php

/**
 * @file
 * Provides support for the Views module.
 */

/**
 * Implements hook_views_data().
 */
function flag_clear_views_data() {
  $data = array();

  // Clear flags content links.
  $data['flagging']['clear_link'] = array(
    'title' => t('Clear flags'),
    'help' => t('Clear all flags for this entity.'),
    'field' => array(
      'handler' => 'flag_clear_handler_field_clear',
    ),
  );
  // Unflag content links.
  $data['flagging']['unflag_link'] = array(
    'title' => t('Unflag'),
    'help' => t('Unflag this entity for a specific user. (Requires Username/UID field)'),
    'field' => array(
      'handler' => 'flag_clear_handler_field_unflag',
    ),
  );

  return $data;
}

/**
 * Implements hook_views_default_views().
 */
function flag_clear_views_default_views() {
  $export = array();

  $node_flags = flag_get_flags('node');
  $user_flags = flag_get_flags('user');
  $comment_flags = flag_get_flags('comment');

  if (!empty($node_flags)) {
    $flag_id = array_keys($node_flags);
    // Content flag manager.
    $view = new view();
    $view->name = 'flag_manager';
    $view->description = '';
    $view->tag = 'default';
    $view->base_table = 'node';
    $view->human_name = 'Flag manager - Content';
    $view->core = 7;
    $view->api_version = '3.0';
    $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
    /* Display: Master */
    $handler = $view->new_display('default', 'Master', 'default');
    $handler->display->display_options['title'] = 'Flagged content';
    $handler->display->display_options['use_more_always'] = FALSE;
    $handler->display->display_options['access']['type'] = 'perm';
    $handler->display->display_options['access']['perm'] = 'clear flags';
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['pager']['type'] = 'full';
    $handler->display->display_options['pager']['options']['items_per_page'] = '20';
    $handler->display->display_options['style_plugin'] = 'table';
    /* Relationship: Flags: node_flag */
    $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'node';
    $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['flag_content_rel']['flag'] = array_pop($flag_id);
    $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
    /* Relationship: Flags: User */
    $handler->display->display_options['relationships']['uid']['id'] = 'uid';
    $handler->display->display_options['relationships']['uid']['table'] = 'flagging';
    $handler->display->display_options['relationships']['uid']['field'] = 'uid';
    $handler->display->display_options['relationships']['uid']['relationship'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['uid']['required'] = TRUE;
    /* Field: Content: Title */
    $handler->display->display_options['fields']['title']['id'] = 'title';
    $handler->display->display_options['fields']['title']['table'] = 'node';
    $handler->display->display_options['fields']['title']['field'] = 'title';
    $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
    $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
    /* Field: User: Name */
    $handler->display->display_options['fields']['name']['id'] = 'name';
    $handler->display->display_options['fields']['name']['table'] = 'users';
    $handler->display->display_options['fields']['name']['field'] = 'name';
    $handler->display->display_options['fields']['name']['relationship'] = 'uid';
    $handler->display->display_options['fields']['name']['label'] = 'User';
    $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
    /* Field: Flags: Clear flag link */
    $handler->display->display_options['fields']['clear_link']['id'] = 'clear_link';
    $handler->display->display_options['fields']['clear_link']['table'] = 'flagging';
    $handler->display->display_options['fields']['clear_link']['field'] = 'clear_link';
    $handler->display->display_options['fields']['clear_link']['relationship'] = 'flag_content_rel';
    $handler->display->display_options['fields']['clear_link']['label'] = 'Clear all flags';
    $handler->display->display_options['fields']['clear_link']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['clear_link']['link_text'] = 'Clear flags';
    /* Field: Flags: Unflag link */
    $handler->display->display_options['fields']['unflag_link']['id'] = 'unflag_link';
    $handler->display->display_options['fields']['unflag_link']['table'] = 'flagging';
    $handler->display->display_options['fields']['unflag_link']['field'] = 'unflag_link';
    $handler->display->display_options['fields']['unflag_link']['relationship'] = 'flag_content_rel';
    $handler->display->display_options['fields']['unflag_link']['label'] = 'Unflag';
    $handler->display->display_options['fields']['unflag_link']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['unflag_link']['link_text'] = 'Unflag';
    /* Field: Flags: Flagged time */
    $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
    $handler->display->display_options['fields']['timestamp']['table'] = 'flagging';
    $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
    $handler->display->display_options['fields']['timestamp']['relationship'] = 'flag_content_rel';
    $handler->display->display_options['fields']['timestamp']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['timestamp']['date_format'] = 'time ago';
    /* Sort criterion: Content: Post date */
    $handler->display->display_options['sorts']['created']['id'] = 'created';
    $handler->display->display_options['sorts']['created']['table'] = 'node';
    $handler->display->display_options['sorts']['created']['field'] = 'created';
    $handler->display->display_options['sorts']['created']['order'] = 'DESC';
    /* Filter criterion: Content: Title */
    $handler->display->display_options['filters']['title']['id'] = 'title';
    $handler->display->display_options['filters']['title']['table'] = 'node';
    $handler->display->display_options['filters']['title']['field'] = 'title';
    $handler->display->display_options['filters']['title']['operator'] = 'allwords';
    $handler->display->display_options['filters']['title']['exposed'] = TRUE;
    $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
    $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
    $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
    $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
    $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
      2 => '2',
      1 => 0,
      3 => 0,
    );
    /* Filter criterion: Flags: User uid */
    $handler->display->display_options['filters']['uid']['id'] = 'uid';
    $handler->display->display_options['filters']['uid']['table'] = 'flagging';
    $handler->display->display_options['filters']['uid']['field'] = 'uid';
    $handler->display->display_options['filters']['uid']['relationship'] = 'flag_content_rel';
    $handler->display->display_options['filters']['uid']['value'] = '';
    $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
    $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
    $handler->display->display_options['filters']['uid']['expose']['label'] = 'User';
    $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
    $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
    $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
      2 => '2',
      1 => 0,
      3 => 0,
    );
    /* Display: Content */
    $handler = $view->new_display('page', 'Flagged content', 'flagged_content');
    $handler->display->display_options['path'] = 'admin/config/content/flag-manager/content';
    $handler->display->display_options['menu']['type'] = 'normal';
    $handler->display->display_options['menu']['title'] = 'Flagged content';
    $handler->display->display_options['menu']['weight'] = '0';
    $handler->display->display_options['menu']['name'] = 'management';
    $handler->display->display_options['menu']['context'] = 0;
    $handler->display->display_options['menu']['context_only_inline'] = 0;
    $export['flag_manager'] = $view;

    foreach ($node_flags as $node_flag) {
      /* Display: Content */
      $handler = $view->new_display('page', 'Flagged content - ' . $node_flag->title, 'flagged_content_' . $node_flag->fid);
      $handler->display->display_options['path'] = 'admin/config/content/flag-manager/content/' . $node_flag->fid;
      $handler->display->display_options['menu']['type'] = 'normal';
      $handler->display->display_options['menu']['title'] = 'Flagged content - ' . $node_flag->title;
      $handler->display->display_options['menu']['weight'] = '0';
      $handler->display->display_options['menu']['name'] = 'management';
      $handler->display->display_options['menu']['context'] = 0;
      $handler->display->display_options['menu']['context_only_inline'] = 0;
      /* Relationship: Flags: node_flag */
      $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
      $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'node';
      $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
      $handler->display->display_options['relationships']['flag_content_rel']['flag'] = $node_flag->fid;
      $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
      $export['flag_manager'] = $view;
    }
  }

  if (!empty($user_flags)) {
    $flag_id = array_keys($user_flags);
    // User flag manager.
    $view = new view();
    $view->name = 'flag_manager_users';
    $view->description = '';
    $view->tag = 'default';
    $view->base_table = 'users';
    $view->human_name = 'Flag manager - Users';
    $view->core = 7;
    $view->api_version = '3.0';
    $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

    /* Display: Master */
    $handler = $view->new_display('default', 'Master', 'default');
    $handler->display->display_options['title'] = 'Flagged users';
    $handler->display->display_options['use_more_always'] = FALSE;
    $handler->display->display_options['access']['type'] = 'perm';
    $handler->display->display_options['access']['perm'] = 'clear flags';
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['pager']['type'] = 'full';
    $handler->display->display_options['pager']['options']['items_per_page'] = '20';
    $handler->display->display_options['style_plugin'] = 'table';
    /* Relationship: Flags: user_flag */
    $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'users';
    $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['flag_content_rel']['flag'] = array_pop($flag_id);
    $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
    /* Relationship: Flags: User */
    $handler->display->display_options['relationships']['uid']['id'] = 'uid';
    $handler->display->display_options['relationships']['uid']['table'] = 'flagging';
    $handler->display->display_options['relationships']['uid']['field'] = 'uid';
    $handler->display->display_options['relationships']['uid']['relationship'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['uid']['required'] = TRUE;
    /* Field: User: Name */
    $handler->display->display_options['fields']['name']['id'] = 'name';
    $handler->display->display_options['fields']['name']['table'] = 'users';
    $handler->display->display_options['fields']['name']['field'] = 'name';
    $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
    $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
    $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
    /* Field: User: Name */
    $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
    $handler->display->display_options['fields']['name_1']['table'] = 'users';
    $handler->display->display_options['fields']['name_1']['field'] = 'name';
    $handler->display->display_options['fields']['name_1']['relationship'] = 'uid';
    $handler->display->display_options['fields']['name_1']['label'] = 'Flagged by';
    $handler->display->display_options['fields']['name_1']['element_label_colon'] = FALSE;
    /* Field: Flags: Clear flag link */
    $handler->display->display_options['fields']['clear_link']['id'] = 'clear_link';
    $handler->display->display_options['fields']['clear_link']['table'] = 'flagging';
    $handler->display->display_options['fields']['clear_link']['field'] = 'clear_link';
    $handler->display->display_options['fields']['clear_link']['relationship'] = 'flag_content_rel';
    $handler->display->display_options['fields']['clear_link']['label'] = 'Clear all flags';
    $handler->display->display_options['fields']['clear_link']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['clear_link']['link_text'] = 'Clear flags';
    /* Field: Flags: Unflag link */
    $handler->display->display_options['fields']['unflag_link']['id'] = 'unflag_link';
    $handler->display->display_options['fields']['unflag_link']['table'] = 'flagging';
    $handler->display->display_options['fields']['unflag_link']['field'] = 'unflag_link';
    $handler->display->display_options['fields']['unflag_link']['relationship'] = 'flag_content_rel';
    $handler->display->display_options['fields']['unflag_link']['label'] = 'Unflag';
    $handler->display->display_options['fields']['unflag_link']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['unflag_link']['link_text'] = 'Unflag';
    /* Sort criterion: User: Created date */
    $handler->display->display_options['sorts']['created']['id'] = 'created';
    $handler->display->display_options['sorts']['created']['table'] = 'users';
    $handler->display->display_options['sorts']['created']['field'] = 'created';
    $handler->display->display_options['sorts']['created']['order'] = 'DESC';
    /* Filter criterion: User: Name */
    $handler->display->display_options['filters']['uid']['id'] = 'uid';
    $handler->display->display_options['filters']['uid']['table'] = 'users';
    $handler->display->display_options['filters']['uid']['field'] = 'uid';
    $handler->display->display_options['filters']['uid']['value'] = '';
    $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
    $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
    $handler->display->display_options['filters']['uid']['expose']['label'] = 'Name';
    $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
    $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
    $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
      2 => '2',
      1 => 0,
      3 => 0,
    );
    /* Display: Flag manager - Users */
    $handler = $view->new_display('page', 'Flagged users', 'flagged_users');
    $handler->display->display_options['path'] = 'admin/config/content/flag-manager/users';
    $handler->display->display_options['menu']['type'] = 'normal';
    $handler->display->display_options['menu']['title'] = 'Flagged users';
    $handler->display->display_options['menu']['weight'] = '0';
    $handler->display->display_options['menu']['name'] = 'management';
    $handler->display->display_options['menu']['context'] = 0;
    $handler->display->display_options['menu']['context_only_inline'] = 0;

    foreach ($user_flags as $user_flag) {
      /* Display: Flag manager - Users */
      $handler = $view->new_display('page', 'Flagged users - ' . $user_flag->title, 'flagged_users_' . $user_flag->fid);
      $handler->display->display_options['path'] = 'admin/config/content/flag-manager/users/' . $user_flag->fid;
      $handler->display->display_options['menu']['type'] = 'normal';
      $handler->display->display_options['menu']['title'] = 'Flagged users - ' . $user_flag->title;
      $handler->display->display_options['menu']['weight'] = '0';
      $handler->display->display_options['menu']['name'] = 'management';
      $handler->display->display_options['menu']['context'] = 0;
      $handler->display->display_options['menu']['context_only_inline'] = 0;
      /* Relationship: Flags: user_flag */
      $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
      $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'users';
      $handler->display->display_options['relationships']['flag_content_rel']['field'] = $user_flag->fid;
      $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
      $export['flag_manager_users'] = $view;
    }
  }

  if (!empty($comment_flags)) {
    $flag_id = array_keys($comment_flags);
    // Comment flag manager.
    $view = new view();
    $view->name = 'flag_manager_comments';
    $view->description = '';
    $view->tag = 'default';
    $view->base_table = 'comment';
    $view->human_name = 'Flag manager - Comments';
    $view->core = 7;
    $view->api_version = '3.0';
    $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

    /* Display: Master */
    $handler = $view->new_display('default', 'Master', 'default');
    $handler->display->display_options['title'] = 'Flagged comments';
    $handler->display->display_options['use_more_always'] = FALSE;
    $handler->display->display_options['access']['type'] = 'perm';
    $handler->display->display_options['access']['perm'] = 'clear flags';
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['pager']['type'] = 'full';
    $handler->display->display_options['pager']['options']['items_per_page'] = '20';
    $handler->display->display_options['style_plugin'] = 'table';
    /* Relationship: Comment: Content */
    $handler->display->display_options['relationships']['nid']['id'] = 'nid';
    $handler->display->display_options['relationships']['nid']['table'] = 'comment';
    $handler->display->display_options['relationships']['nid']['field'] = 'nid';
    $handler->display->display_options['relationships']['nid']['required'] = TRUE;
    /* Relationship: Flags: comment_flag */
    $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'comment';
    $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['flag_content_rel']['flag'] = array_pop($flag_id);
    $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
    /* Relationship: Flags: User */
    $handler->display->display_options['relationships']['uid']['id'] = 'uid';
    $handler->display->display_options['relationships']['uid']['table'] = 'flagging';
    $handler->display->display_options['relationships']['uid']['field'] = 'uid';
    $handler->display->display_options['relationships']['uid']['relationship'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['uid']['required'] = TRUE;
    /* Field: Comment: Title */
    $handler->display->display_options['fields']['subject']['id'] = 'subject';
    $handler->display->display_options['fields']['subject']['table'] = 'comment';
    $handler->display->display_options['fields']['subject']['field'] = 'subject';
    $handler->display->display_options['fields']['subject']['alter']['word_boundary'] = FALSE;
    $handler->display->display_options['fields']['subject']['alter']['ellipsis'] = FALSE;
    $handler->display->display_options['fields']['subject']['element_label_colon'] = FALSE;
    /* Field: Comment: Author */
    $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
    $handler->display->display_options['fields']['name_1']['table'] = 'comment';
    $handler->display->display_options['fields']['name_1']['field'] = 'name';
    $handler->display->display_options['fields']['name_1']['element_label_colon'] = FALSE;
    /* Field: User: Name */
    $handler->display->display_options['fields']['name']['id'] = 'name';
    $handler->display->display_options['fields']['name']['table'] = 'users';
    $handler->display->display_options['fields']['name']['field'] = 'name';
    $handler->display->display_options['fields']['name']['relationship'] = 'uid';
    $handler->display->display_options['fields']['name']['label'] = 'User';
    $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
    /* Field: Flags: Clear flag link */
    $handler->display->display_options['fields']['clear_link']['id'] = 'clear_link';
    $handler->display->display_options['fields']['clear_link']['table'] = 'flagging';
    $handler->display->display_options['fields']['clear_link']['field'] = 'clear_link';
    $handler->display->display_options['fields']['clear_link']['relationship'] = 'flag_content_rel';
    $handler->display->display_options['fields']['clear_link']['label'] = 'Clear all flags';
    $handler->display->display_options['fields']['clear_link']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['clear_link']['link_text'] = 'Clear flags';
    /* Field: Flags: Unflag link */
    $handler->display->display_options['fields']['unflag_link']['id'] = 'unflag_link';
    $handler->display->display_options['fields']['unflag_link']['table'] = 'flagging';
    $handler->display->display_options['fields']['unflag_link']['field'] = 'unflag_link';
    $handler->display->display_options['fields']['unflag_link']['relationship'] = 'flag_content_rel';
    $handler->display->display_options['fields']['unflag_link']['label'] = 'Unflag';
    $handler->display->display_options['fields']['unflag_link']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['unflag_link']['link_text'] = 'Unflag';
    /* Field: Flags: Flagged time */
    $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
    $handler->display->display_options['fields']['timestamp']['table'] = 'flagging';
    $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
    $handler->display->display_options['fields']['timestamp']['relationship'] = 'flag_content_rel';
    $handler->display->display_options['fields']['timestamp']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['timestamp']['date_format'] = 'raw time ago';
    /* Sort criterion: Comment: Post date */
    $handler->display->display_options['sorts']['created']['id'] = 'created';
    $handler->display->display_options['sorts']['created']['table'] = 'comment';
    $handler->display->display_options['sorts']['created']['field'] = 'created';
    $handler->display->display_options['sorts']['created']['order'] = 'DESC';
    /* Filter criterion: Comment: Title */
    $handler->display->display_options['filters']['subject']['id'] = 'subject';
    $handler->display->display_options['filters']['subject']['table'] = 'comment';
    $handler->display->display_options['filters']['subject']['field'] = 'subject';
    $handler->display->display_options['filters']['subject']['exposed'] = TRUE;
    $handler->display->display_options['filters']['subject']['expose']['operator_id'] = 'subject_op';
    $handler->display->display_options['filters']['subject']['expose']['label'] = 'Title';
    $handler->display->display_options['filters']['subject']['expose']['operator'] = 'subject_op';
    $handler->display->display_options['filters']['subject']['expose']['identifier'] = 'subject';
    $handler->display->display_options['filters']['subject']['expose']['remember_roles'] = array(
      2 => '2',
      1 => 0,
      3 => 0,
    );
    /* Filter criterion: Content: Author uid */
    $handler->display->display_options['filters']['uid']['id'] = 'uid';
    $handler->display->display_options['filters']['uid']['table'] = 'node';
    $handler->display->display_options['filters']['uid']['field'] = 'uid';
    $handler->display->display_options['filters']['uid']['relationship'] = 'nid';
    $handler->display->display_options['filters']['uid']['value'] = '';
    $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
    $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
    $handler->display->display_options['filters']['uid']['expose']['label'] = 'Author';
    $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
    $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'author';
    $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
      2 => '2',
      1 => 0,
      3 => 0,
    );
    /* Display: Flag manager - Comments */
    $handler = $view->new_display('page', 'Flagged comments', 'flagged_comments');
    $handler->display->display_options['path'] = 'admin/config/content/flag-manager/comments';
    $handler->display->display_options['menu']['type'] = 'normal';
    $handler->display->display_options['menu']['title'] = 'Flagged comments';
    $handler->display->display_options['menu']['weight'] = '0';
    $handler->display->display_options['menu']['name'] = 'management';
    $handler->display->display_options['menu']['context'] = 0;
    $handler->display->display_options['menu']['context_only_inline'] = 0;

    foreach ($comment_flags as $comment_flag) {
      /* Display: Flag manager - Comments */
      $handler = $view->new_display('page', 'Flagged comments - ' . $comment_flag->title, 'flagged_comments_' . $comment_flag->fid);
      $handler->display->display_options['path'] = 'admin/config/content/flag-manager/comments/' . $comment_flag->fid;
      $handler->display->display_options['menu']['type'] = 'normal';
      $handler->display->display_options['menu']['title'] = 'Flagged comments - ' . $comment_flag->title;
      $handler->display->display_options['menu']['weight'] = '0';
      $handler->display->display_options['menu']['name'] = 'management';
      $handler->display->display_options['menu']['context'] = 0;
      $handler->display->display_options['menu']['context_only_inline'] = 0;
      /* Relationship: Flags: comment_flag */
      $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
      $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'comment';
      $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
      $handler->display->display_options['relationships']['flag_content_rel']['flag'] = $comment_flag->fid;
      $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
      $export['flag_manager_comments'] = $view;
    }
  }

  return $export;
}
