<?php
/**
 * @file
 * open_badges.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function open_badges_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'manage_badges';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Manage Badges';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Manage Badges';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => '3',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'field_badge_type' => 'field_badge_type',
    'field_course' => 'field_course',
    'field_lesson' => 'field_lesson',
    'edit_node' => 'edit_node',
    'delete_node' => 'edit_node',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_badge_type' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_course' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_lesson' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_node' => array(
      'align' => '',
      'separator' => ' / ',
      'empty_column' => 0,
    ),
    'delete_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No badge found.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Badge type */
  $handler->display->display_options['fields']['field_badge_type']['id'] = 'field_badge_type';
  $handler->display->display_options['fields']['field_badge_type']['table'] = 'field_data_field_badge_type';
  $handler->display->display_options['fields']['field_badge_type']['field'] = 'field_badge_type';
  $handler->display->display_options['fields']['field_badge_type']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_badge_type']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Course */
  $handler->display->display_options['fields']['field_course']['id'] = 'field_course';
  $handler->display->display_options['fields']['field_course']['table'] = 'field_data_field_course';
  $handler->display->display_options['fields']['field_course']['field'] = 'field_course';
  $handler->display->display_options['fields']['field_course']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_course']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Lesson */
  $handler->display->display_options['fields']['field_lesson']['id'] = 'field_lesson';
  $handler->display->display_options['fields']['field_lesson']['table'] = 'field_data_field_lesson';
  $handler->display->display_options['fields']['field_lesson']['field'] = 'field_lesson';
  $handler->display->display_options['fields']['field_lesson']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_lesson']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Edit';
  /* Field: Content: Delete link */
  $handler->display->display_options['fields']['delete_node']['id'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['delete_node']['field'] = 'delete_node';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'badge' => 'badge',
  );
  /* Filter criterion: Content: Badge type (field_badge_type) */
  $handler->display->display_options['filters']['field_badge_type_tid']['id'] = 'field_badge_type_tid';
  $handler->display->display_options['filters']['field_badge_type_tid']['table'] = 'field_data_field_badge_type';
  $handler->display->display_options['filters']['field_badge_type_tid']['field'] = 'field_badge_type_tid';
  $handler->display->display_options['filters']['field_badge_type_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_badge_type_tid']['expose']['operator_id'] = 'field_badge_type_tid_op';
  $handler->display->display_options['filters']['field_badge_type_tid']['expose']['label'] = 'Badge type';
  $handler->display->display_options['filters']['field_badge_type_tid']['expose']['operator'] = 'field_badge_type_tid_op';
  $handler->display->display_options['filters']['field_badge_type_tid']['expose']['identifier'] = 'field_badge_type_tid';
  $handler->display->display_options['filters']['field_badge_type_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_badge_type_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_badge_type_tid']['vocabulary'] = 'badge_type';
  $handler->display->display_options['filters']['field_badge_type_tid']['hierarchy'] = 1;
  $handler->display->display_options['filters']['field_badge_type_tid']['error_message'] = FALSE;
  /* Filter criterion: Content: Course (field_course) */
  $handler->display->display_options['filters']['field_course_tid']['id'] = 'field_course_tid';
  $handler->display->display_options['filters']['field_course_tid']['table'] = 'field_data_field_course';
  $handler->display->display_options['filters']['field_course_tid']['field'] = 'field_course_tid';
  $handler->display->display_options['filters']['field_course_tid']['value'] = '';
  $handler->display->display_options['filters']['field_course_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_course_tid']['expose']['operator_id'] = 'field_course_tid_op';
  $handler->display->display_options['filters']['field_course_tid']['expose']['label'] = 'Course';
  $handler->display->display_options['filters']['field_course_tid']['expose']['operator'] = 'field_course_tid_op';
  $handler->display->display_options['filters']['field_course_tid']['expose']['identifier'] = 'field_course_tid';
  $handler->display->display_options['filters']['field_course_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_course_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_course_tid']['vocabulary'] = 'courses';
  $handler->display->display_options['filters']['field_course_tid']['hierarchy'] = 1;
  /* Filter criterion: Content: Lesson (field_lesson) */
  $handler->display->display_options['filters']['field_lesson_tid']['id'] = 'field_lesson_tid';
  $handler->display->display_options['filters']['field_lesson_tid']['table'] = 'field_data_field_lesson';
  $handler->display->display_options['filters']['field_lesson_tid']['field'] = 'field_lesson_tid';
  $handler->display->display_options['filters']['field_lesson_tid']['value'] = '';
  $handler->display->display_options['filters']['field_lesson_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_lesson_tid']['expose']['operator_id'] = 'field_lesson_tid_op';
  $handler->display->display_options['filters']['field_lesson_tid']['expose']['label'] = 'Lesson';
  $handler->display->display_options['filters']['field_lesson_tid']['expose']['operator'] = 'field_lesson_tid_op';
  $handler->display->display_options['filters']['field_lesson_tid']['expose']['identifier'] = 'field_lesson_tid';
  $handler->display->display_options['filters']['field_lesson_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_lesson_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_lesson_tid']['vocabulary'] = 'lessons';
  $handler->display->display_options['filters']['field_lesson_tid']['hierarchy'] = 1;

  /* Display: Manage Badges */
  $handler = $view->new_display('page', 'Manage Badges', 'page');
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Badge type */
  $handler->display->display_options['fields']['field_badge_type']['id'] = 'field_badge_type';
  $handler->display->display_options['fields']['field_badge_type']['table'] = 'field_data_field_badge_type';
  $handler->display->display_options['fields']['field_badge_type']['field'] = 'field_badge_type';
  $handler->display->display_options['fields']['field_badge_type']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_badge_type']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Course */
  $handler->display->display_options['fields']['field_course']['id'] = 'field_course';
  $handler->display->display_options['fields']['field_course']['table'] = 'field_data_field_course';
  $handler->display->display_options['fields']['field_course']['field'] = 'field_course';
  $handler->display->display_options['fields']['field_course']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_course']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Lesson */
  $handler->display->display_options['fields']['field_lesson']['id'] = 'field_lesson';
  $handler->display->display_options['fields']['field_lesson']['table'] = 'field_data_field_lesson';
  $handler->display->display_options['fields']['field_lesson']['field'] = 'field_lesson';
  $handler->display->display_options['fields']['field_lesson']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_lesson']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Action';
  /* Field: Content: weight Selector */
  $handler->display->display_options['fields']['field_weight_selector']['id'] = 'field_weight_selector';
  $handler->display->display_options['fields']['field_weight_selector']['table'] = 'field_data_field_weight';
  $handler->display->display_options['fields']['field_weight_selector']['field'] = 'field_weight_selector';
  $handler->display->display_options['fields']['field_weight_selector']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_weight_selector']['weight_range'] = '100';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: weight (field_weight) */
  $handler->display->display_options['sorts']['field_weight_value']['id'] = 'field_weight_value';
  $handler->display->display_options['sorts']['field_weight_value']['table'] = 'field_data_field_weight';
  $handler->display->display_options['sorts']['field_weight_value']['field'] = 'field_weight_value';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'badge' => 'badge',
  );
  /* Filter criterion: Content: Badge type (field_badge_type) */
  $handler->display->display_options['filters']['field_badge_type_tid']['id'] = 'field_badge_type_tid';
  $handler->display->display_options['filters']['field_badge_type_tid']['table'] = 'field_data_field_badge_type';
  $handler->display->display_options['filters']['field_badge_type_tid']['field'] = 'field_badge_type_tid';
  $handler->display->display_options['filters']['field_badge_type_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_badge_type_tid']['expose']['operator_id'] = 'field_badge_type_tid_op';
  $handler->display->display_options['filters']['field_badge_type_tid']['expose']['label'] = 'Badge type';
  $handler->display->display_options['filters']['field_badge_type_tid']['expose']['description'] = ' ';
  $handler->display->display_options['filters']['field_badge_type_tid']['expose']['operator'] = 'field_badge_type_tid_op';
  $handler->display->display_options['filters']['field_badge_type_tid']['expose']['identifier'] = 'field_badge_type_tid';
  $handler->display->display_options['filters']['field_badge_type_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_badge_type_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_badge_type_tid']['vocabulary'] = 'badge_type';
  $handler->display->display_options['filters']['field_badge_type_tid']['hierarchy'] = 1;
  $handler->display->display_options['filters']['field_badge_type_tid']['error_message'] = FALSE;
  /* Filter criterion: Content: Course (field_course) */
  $handler->display->display_options['filters']['field_course_tid']['id'] = 'field_course_tid';
  $handler->display->display_options['filters']['field_course_tid']['table'] = 'field_data_field_course';
  $handler->display->display_options['filters']['field_course_tid']['field'] = 'field_course_tid';
  $handler->display->display_options['filters']['field_course_tid']['value'] = '';
  $handler->display->display_options['filters']['field_course_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_course_tid']['expose']['operator_id'] = 'field_course_tid_op';
  $handler->display->display_options['filters']['field_course_tid']['expose']['label'] = 'Course';
  $handler->display->display_options['filters']['field_course_tid']['expose']['operator'] = 'field_course_tid_op';
  $handler->display->display_options['filters']['field_course_tid']['expose']['identifier'] = 'field_course_tid';
  $handler->display->display_options['filters']['field_course_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_course_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_course_tid']['vocabulary'] = 'courses';
  $handler->display->display_options['filters']['field_course_tid']['hierarchy'] = 1;
  /* Filter criterion: Content: Lesson (field_lesson) */
  $handler->display->display_options['filters']['field_lesson_tid']['id'] = 'field_lesson_tid';
  $handler->display->display_options['filters']['field_lesson_tid']['table'] = 'field_data_field_lesson';
  $handler->display->display_options['filters']['field_lesson_tid']['field'] = 'field_lesson_tid';
  $handler->display->display_options['filters']['field_lesson_tid']['value'] = '';
  $handler->display->display_options['filters']['field_lesson_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_lesson_tid']['expose']['operator_id'] = 'field_lesson_tid_op';
  $handler->display->display_options['filters']['field_lesson_tid']['expose']['label'] = 'Lesson';
  $handler->display->display_options['filters']['field_lesson_tid']['expose']['operator'] = 'field_lesson_tid_op';
  $handler->display->display_options['filters']['field_lesson_tid']['expose']['identifier'] = 'field_lesson_tid';
  $handler->display->display_options['filters']['field_lesson_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_lesson_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_lesson_tid']['vocabulary'] = 'lessons';
  $handler->display->display_options['filters']['field_lesson_tid']['hierarchy'] = 1;
  $handler->display->display_options['path'] = 'admin/open_badges/manage-badges';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Manage Badges';
  $handler->display->display_options['menu']['weight'] = '1';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Manage Award */
  $handler = $view->new_display('page', 'Manage Award', 'page_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Manage Award';
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No award found.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Content: User (field_user) */
  $handler->display->display_options['relationships']['field_user_uid']['id'] = 'field_user_uid';
  $handler->display->display_options['relationships']['field_user_uid']['table'] = 'field_data_field_user';
  $handler->display->display_options['relationships']['field_user_uid']['field'] = 'field_user_uid';
  $handler->display->display_options['relationships']['field_user_uid']['delta'] = '-1';
  /* Relationship: Content: Badge (field_badge) */
  $handler->display->display_options['relationships']['field_badge_nid']['id'] = 'field_badge_nid';
  $handler->display->display_options['relationships']['field_badge_nid']['table'] = 'field_data_field_badge';
  $handler->display->display_options['relationships']['field_badge_nid']['field'] = 'field_badge_nid';
  $handler->display->display_options['relationships']['field_badge_nid']['delta'] = '-1';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'field_badge_nid';
  $handler->display->display_options['fields']['title']['label'] = 'Badge Name';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: User: E-mail */
  $handler->display->display_options['fields']['mail']['id'] = 'mail';
  $handler->display->display_options['fields']['mail']['table'] = 'users';
  $handler->display->display_options['fields']['mail']['field'] = 'mail';
  $handler->display->display_options['fields']['mail']['relationship'] = 'field_user_uid';
  $handler->display->display_options['fields']['mail']['label'] = 'User e-mail';
  $handler->display->display_options['fields']['mail']['element_label_colon'] = FALSE;
  /* Field: Content: Baked image */
  $handler->display->display_options['fields']['field_baked_image']['id'] = 'field_baked_image';
  $handler->display->display_options['fields']['field_baked_image']['table'] = 'field_data_field_baked_image';
  $handler->display->display_options['fields']['field_baked_image']['field'] = 'field_baked_image';
  $handler->display->display_options['fields']['field_baked_image']['label'] = 'Badge image';
  $handler->display->display_options['fields']['field_baked_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_baked_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_baked_image']['settings'] = array(
    'image_style' => '',
    'image_link' => '',
  );
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Awarded On';
  $handler->display->display_options['fields']['created']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['created']['date_format'] = 'medium';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: Content: Delete link */
  $handler->display->display_options['fields']['delete_node']['id'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['delete_node']['field'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['label'] = 'Revoke';
  $handler->display->display_options['fields']['delete_node']['text'] = 'Revoke';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'award' => 'award',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Badge type (field_badge_type) */
  $handler->display->display_options['filters']['field_badge_type_tid']['id'] = 'field_badge_type_tid';
  $handler->display->display_options['filters']['field_badge_type_tid']['table'] = 'field_data_field_badge_type';
  $handler->display->display_options['filters']['field_badge_type_tid']['field'] = 'field_badge_type_tid';
  $handler->display->display_options['filters']['field_badge_type_tid']['relationship'] = 'field_badge_nid';
  $handler->display->display_options['filters']['field_badge_type_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_badge_type_tid']['expose']['operator_id'] = 'field_badge_type_tid_op';
  $handler->display->display_options['filters']['field_badge_type_tid']['expose']['label'] = 'Badge type';
  $handler->display->display_options['filters']['field_badge_type_tid']['expose']['operator'] = 'field_badge_type_tid_op';
  $handler->display->display_options['filters']['field_badge_type_tid']['expose']['identifier'] = 'field_badge_type_tid';
  $handler->display->display_options['filters']['field_badge_type_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_badge_type_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_badge_type_tid']['vocabulary'] = 'badge_type';
  $handler->display->display_options['filters']['field_badge_type_tid']['hierarchy'] = 1;
  /* Filter criterion: Content: Course (field_course) */
  $handler->display->display_options['filters']['field_course_tid']['id'] = 'field_course_tid';
  $handler->display->display_options['filters']['field_course_tid']['table'] = 'field_data_field_course';
  $handler->display->display_options['filters']['field_course_tid']['field'] = 'field_course_tid';
  $handler->display->display_options['filters']['field_course_tid']['relationship'] = 'field_badge_nid';
  $handler->display->display_options['filters']['field_course_tid']['value'] = '';
  $handler->display->display_options['filters']['field_course_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_course_tid']['expose']['operator_id'] = 'field_course_tid_op';
  $handler->display->display_options['filters']['field_course_tid']['expose']['label'] = 'Course';
  $handler->display->display_options['filters']['field_course_tid']['expose']['operator'] = 'field_course_tid_op';
  $handler->display->display_options['filters']['field_course_tid']['expose']['identifier'] = 'field_course_tid';
  $handler->display->display_options['filters']['field_course_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_course_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_course_tid']['vocabulary'] = 'courses';
  $handler->display->display_options['filters']['field_course_tid']['hierarchy'] = 1;
  /* Filter criterion: Content: Lesson (field_lesson) */
  $handler->display->display_options['filters']['field_lesson_tid']['id'] = 'field_lesson_tid';
  $handler->display->display_options['filters']['field_lesson_tid']['table'] = 'field_data_field_lesson';
  $handler->display->display_options['filters']['field_lesson_tid']['field'] = 'field_lesson_tid';
  $handler->display->display_options['filters']['field_lesson_tid']['relationship'] = 'field_badge_nid';
  $handler->display->display_options['filters']['field_lesson_tid']['value'] = '';
  $handler->display->display_options['filters']['field_lesson_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_lesson_tid']['expose']['operator_id'] = 'field_lesson_tid_op';
  $handler->display->display_options['filters']['field_lesson_tid']['expose']['label'] = 'Lesson';
  $handler->display->display_options['filters']['field_lesson_tid']['expose']['operator'] = 'field_lesson_tid_op';
  $handler->display->display_options['filters']['field_lesson_tid']['expose']['identifier'] = 'field_lesson_tid';
  $handler->display->display_options['filters']['field_lesson_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_lesson_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_lesson_tid']['vocabulary'] = 'lessons';
  $handler->display->display_options['filters']['field_lesson_tid']['hierarchy'] = 1;
  /* Filter criterion: User: E-mail */
  $handler->display->display_options['filters']['mail']['id'] = 'mail';
  $handler->display->display_options['filters']['mail']['table'] = 'users';
  $handler->display->display_options['filters']['mail']['field'] = 'mail';
  $handler->display->display_options['filters']['mail']['relationship'] = 'field_user_uid';
  $handler->display->display_options['filters']['mail']['exposed'] = TRUE;
  $handler->display->display_options['filters']['mail']['expose']['operator_id'] = 'mail_op';
  $handler->display->display_options['filters']['mail']['expose']['label'] = 'E-mail';
  $handler->display->display_options['filters']['mail']['expose']['operator'] = 'mail_op';
  $handler->display->display_options['filters']['mail']['expose']['identifier'] = 'mail';
  $handler->display->display_options['filters']['mail']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['path'] = 'admin/open_badges/manage-award';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Manage Awards';
  $handler->display->display_options['menu']['weight'] = '2';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['manage_badges'] = $view;

  $view = new view();
  $view->name = 'manage_issuer';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Manage Issuer';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Manage Issuer';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => '3',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No issuer found.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Organization name';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Email */
  $handler->display->display_options['fields']['field_email']['id'] = 'field_email';
  $handler->display->display_options['fields']['field_email']['table'] = 'field_data_field_email';
  $handler->display->display_options['fields']['field_email']['field'] = 'field_email';
  $handler->display->display_options['fields']['field_email']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_email']['type'] = 'email_plain';
  /* Field: Content: Origin */
  $handler->display->display_options['fields']['field_origin']['id'] = 'field_origin';
  $handler->display->display_options['fields']['field_origin']['table'] = 'field_data_field_origin';
  $handler->display->display_options['fields']['field_origin']['field'] = 'field_origin';
  $handler->display->display_options['fields']['field_origin']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_origin']['click_sort_column'] = 'url';
  $handler->display->display_options['fields']['field_origin']['type'] = 'link_plain';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Edit';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'issuer' => 'issuer',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/open_badges/manage-issuer';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Manage Issuers';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['manage_issuer'] = $view;

  $view = new view();
  $view->name = 'my_backpack';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'My Backpack';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'My Backpack';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'grid';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Header: Global: PHP */
  $handler->display->display_options['header']['php']['id'] = 'php';
  $handler->display->display_options['header']['php']['table'] = 'views';
  $handler->display->display_options['header']['php']['field'] = 'php';
  $handler->display->display_options['header']['php']['php_output'] = '<?php
drupal_add_js(\'http://beta.openbadges.org/issuer.js\', \'external\');
?>
';
  /* Relationship: Content: User (field_user) */
  $handler->display->display_options['relationships']['field_user_uid']['id'] = 'field_user_uid';
  $handler->display->display_options['relationships']['field_user_uid']['table'] = 'field_data_field_user';
  $handler->display->display_options['relationships']['field_user_uid']['field'] = 'field_user_uid';
  $handler->display->display_options['relationships']['field_user_uid']['required'] = TRUE;
  $handler->display->display_options['relationships']['field_user_uid']['delta'] = '-1';
  /* Relationship: Content: Badge (field_badge) */
  $handler->display->display_options['relationships']['field_badge_nid']['id'] = 'field_badge_nid';
  $handler->display->display_options['relationships']['field_badge_nid']['table'] = 'field_data_field_badge';
  $handler->display->display_options['relationships']['field_badge_nid']['field'] = 'field_badge_nid';
  $handler->display->display_options['relationships']['field_badge_nid']['required'] = TRUE;
  $handler->display->display_options['relationships']['field_badge_nid']['delta'] = '-1';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['relationship'] = 'field_badge_nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'field_badge_nid';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Baked image */
  $handler->display->display_options['fields']['field_baked_image']['id'] = 'field_baked_image';
  $handler->display->display_options['fields']['field_baked_image']['table'] = 'field_data_field_baked_image';
  $handler->display->display_options['fields']['field_baked_image']['field'] = 'field_baked_image';
  $handler->display->display_options['fields']['field_baked_image']['label'] = '';
  $handler->display->display_options['fields']['field_baked_image']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_baked_image']['alter']['path'] = 'node/[nid]';
  $handler->display->display_options['fields']['field_baked_image']['alter']['absolute'] = TRUE;
  $handler->display->display_options['fields']['field_baked_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_baked_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_baked_image']['settings'] = array(
    'image_style' => '',
    'image_link' => '',
  );
  /* Field: User: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'users';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['relationship'] = 'field_user_uid';
  $handler->display->display_options['fields']['uid']['label'] = '';
  $handler->display->display_options['fields']['uid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['uid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['uid']['link_to_user'] = FALSE;
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php']['id'] = 'php';
  $handler->display->display_options['fields']['php']['table'] = 'views';
  $handler->display->display_options['fields']['php']['field'] = 'php';
  $handler->display->display_options['fields']['php']['label'] = '';
  $handler->display->display_options['fields']['php']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['php']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php']['php_value'] = 'global $base_url;
return \'<a href="javascript:OpenBadges.issue([\\\'\'.$base_url.\'/openbadging_amobb/assertions/\'.urlencode($row->title).\'/\'.$row->uid.\'\\\'],function(errors, successes){ });"><img src="\'.$base_url.\'/\'.drupal_get_path(\'module\', \'open_badges\').\'/i/push_to_backpack.png\'.\'"></a>\';';
  $handler->display->display_options['fields']['php']['php_output'] = '<?php
print $row->php;
?>';
  $handler->display->display_options['fields']['php']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php']['php_click_sortable'] = '';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: User: Uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'users';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['relationship'] = 'field_user_uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'user';
  $handler->display->display_options['arguments']['uid']['default_argument_options']['user'] = FALSE;
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['uid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['uid']['validate']['type'] = 'user';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'award' => 'award',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'user/%/my-backpack';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'My Backpack';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 1;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['my_backpack'] = $view;

  return $export;
}
