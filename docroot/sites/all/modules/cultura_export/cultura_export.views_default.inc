<?php

/**
 * @file
 * View for sharing content of Cultura Exchange site with archival site.
 */

/**
 * Implements hook_views_default_views().
 */
function cultura_export_views_default_views() {
  $views = array();

  $view = new view();
  $view->name = 'culturaexchange_export';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'culturaexchange_export';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'culturaexchange_export';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '999';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'node';
  $handler->display->display_options['row_options']['view_mode'] = 'full';
  $handler->display->display_options['row_options']['links'] = FALSE;
  $handler->display->display_options['row_options']['comments'] = TRUE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = 'nid';
  $handler->display->display_options['fields']['nid']['element_type'] = '0';
  $handler->display->display_options['fields']['nid']['element_label_type'] = '0';
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nid']['element_default_classes'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Prompts */
  $handler->display->display_options['fields']['field_prompts']['id'] = 'field_prompts';
  $handler->display->display_options['fields']['field_prompts']['table'] = 'field_data_field_prompts';
  $handler->display->display_options['fields']['field_prompts']['field'] = 'field_prompts';
  $handler->display->display_options['fields']['field_prompts']['label'] = 'prompts_first';
  $handler->display->display_options['fields']['field_prompts']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_prompts']['alter']['text'] = '[field_prompts-first]';
  $handler->display->display_options['fields']['field_prompts']['element_type'] = '0';
  $handler->display->display_options['fields']['field_prompts']['element_label_type'] = '0';
  $handler->display->display_options['fields']['field_prompts']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_prompts']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['field_prompts']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_prompts']['click_sort_column'] = 'first';
  $handler->display->display_options['fields']['field_prompts']['settings'] = array(
    'style' => 'block',
    'first' => array(
      'hidden' => 0,
      'format' => '_none',
      'prefix' => '',
      'suffix' => '',
    ),
    'second' => array(
      'hidden' => 0,
      'format' => '_none',
      'prefix' => '',
      'suffix' => '',
    ),
  );
  /* Field: Content: Prompts */
  $handler->display->display_options['fields']['field_prompts_1']['id'] = 'field_prompts_1';
  $handler->display->display_options['fields']['field_prompts_1']['table'] = 'field_data_field_prompts';
  $handler->display->display_options['fields']['field_prompts_1']['field'] = 'field_prompts';
  $handler->display->display_options['fields']['field_prompts_1']['label'] = 'prompts_second';
  $handler->display->display_options['fields']['field_prompts_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_prompts_1']['alter']['text'] = '[field_prompts_1-second]';
  $handler->display->display_options['fields']['field_prompts_1']['element_type'] = '0';
  $handler->display->display_options['fields']['field_prompts_1']['element_label_type'] = '0';
  $handler->display->display_options['fields']['field_prompts_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_prompts_1']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['field_prompts_1']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_prompts_1']['click_sort_column'] = 'second';
  $handler->display->display_options['fields']['field_prompts_1']['settings'] = array(
    'style' => 'inline',
    'first' => array(
      'hidden' => 0,
      'format' => '_none',
      'prefix' => '',
      'suffix' => '',
    ),
    'second' => array(
      'hidden' => 0,
      'format' => '_none',
      'prefix' => '',
      'suffix' => '',
    ),
  );
  /* Field: Content: Answers */
  $handler->display->display_options['fields']['cultura_discussion_answers']['id'] = 'cultura_discussion_answers';
  $handler->display->display_options['fields']['cultura_discussion_answers']['table'] = 'field_data_cultura_discussion_answers';
  $handler->display->display_options['fields']['cultura_discussion_answers']['field'] = 'cultura_discussion_answers';
  $handler->display->display_options['fields']['cultura_discussion_answers']['label'] = 'answers_first';
  $handler->display->display_options['fields']['cultura_discussion_answers']['exclude'] = TRUE;
  $handler->display->display_options['fields']['cultura_discussion_answers']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['cultura_discussion_answers']['alter']['text'] = '[cultura_discussion_answers-first]';
  $handler->display->display_options['fields']['cultura_discussion_answers']['element_type'] = '0';
  $handler->display->display_options['fields']['cultura_discussion_answers']['element_label_type'] = '0';
  $handler->display->display_options['fields']['cultura_discussion_answers']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['cultura_discussion_answers']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['cultura_discussion_answers']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['cultura_discussion_answers']['click_sort_column'] = 'first';
  $handler->display->display_options['fields']['cultura_discussion_answers']['settings'] = array(
    'style' => 'block',
    'first' => array(
      'hidden' => 0,
      'format' => 'plain_text',
      'prefix' => '',
      'suffix' => '',
    ),
    'second' => array(
      'hidden' => 0,
      'format' => 'full_html',
      'prefix' => '',
      'suffix' => '',
    ),
  );
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'answers_first';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[cultura_discussion_answers]';
  $handler->display->display_options['fields']['nothing']['element_type'] = '0';
  $handler->display->display_options['fields']['nothing']['element_label_type'] = '0';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nothing']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['nothing']['element_default_classes'] = FALSE;
  /* Field: Content: Answers */
  $handler->display->display_options['fields']['cultura_discussion_answers_1']['id'] = 'cultura_discussion_answers_1';
  $handler->display->display_options['fields']['cultura_discussion_answers_1']['table'] = 'field_data_cultura_discussion_answers';
  $handler->display->display_options['fields']['cultura_discussion_answers_1']['field'] = 'cultura_discussion_answers';
  $handler->display->display_options['fields']['cultura_discussion_answers_1']['label'] = 'answers_second';
  $handler->display->display_options['fields']['cultura_discussion_answers_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['cultura_discussion_answers_1']['alter']['text'] = '[cultura_discussion_answers_1-second]';
  $handler->display->display_options['fields']['cultura_discussion_answers_1']['element_type'] = '0';
  $handler->display->display_options['fields']['cultura_discussion_answers_1']['element_label_type'] = '0';
  $handler->display->display_options['fields']['cultura_discussion_answers_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['cultura_discussion_answers_1']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['cultura_discussion_answers_1']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['cultura_discussion_answers_1']['click_sort_column'] = 'second';
  $handler->display->display_options['fields']['cultura_discussion_answers_1']['settings'] = array(
    'style' => 'block',
    'first' => array(
      'hidden' => 0,
      'format' => 'full_html',
      'prefix' => '',
      'suffix' => '',
    ),
    'second' => array(
      'hidden' => 0,
      'format' => 'full_html',
      'prefix' => '',
      'suffix' => '',
    ),
  );
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'post_date';
  $handler->display->display_options['fields']['created']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['created']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['created']['date_format'] = 'custom';
  $handler->display->display_options['fields']['created']['custom_date_format'] = 'c';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: Content: Question type */
  $handler->display->display_options['fields']['cultura_question_type']['id'] = 'cultura_question_type';
  $handler->display->display_options['fields']['cultura_question_type']['table'] = 'field_data_cultura_question_type';
  $handler->display->display_options['fields']['cultura_question_type']['field'] = 'cultura_question_type';
  $handler->display->display_options['fields']['cultura_question_type']['label'] = 'question_type';
  $handler->display->display_options['fields']['cultura_question_type']['element_type'] = '0';
  $handler->display->display_options['fields']['cultura_question_type']['element_label_type'] = '0';
  $handler->display->display_options['fields']['cultura_question_type']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['cultura_question_type']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['cultura_question_type']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['cultura_question_type']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Tags */
  $handler->display->display_options['fields']['cultura_tags']['id'] = 'cultura_tags';
  $handler->display->display_options['fields']['cultura_tags']['table'] = 'field_data_cultura_tags';
  $handler->display->display_options['fields']['cultura_tags']['field'] = 'cultura_tags';
  $handler->display->display_options['fields']['cultura_tags']['label'] = 'tags';
  $handler->display->display_options['fields']['cultura_tags']['element_type'] = '0';
  $handler->display->display_options['fields']['cultura_tags']['element_label_type'] = '0';
  $handler->display->display_options['fields']['cultura_tags']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['cultura_tags']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['cultura_tags']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['cultura_tags']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['cultura_tags']['delta_offset'] = '0';
  /* Field: Content statistics: Total views */
  $handler->display->display_options['fields']['totalcount']['id'] = 'totalcount';
  $handler->display->display_options['fields']['totalcount']['table'] = 'node_counter';
  $handler->display->display_options['fields']['totalcount']['field'] = 'totalcount';
  $handler->display->display_options['fields']['totalcount']['label'] = 'total_views';
  $handler->display->display_options['fields']['totalcount']['element_type'] = '0';
  $handler->display->display_options['fields']['totalcount']['element_label_type'] = '0';
  $handler->display->display_options['fields']['totalcount']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['totalcount']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['totalcount']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['totalcount']['separator'] = '';
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
    'cultura_discussion' => 'cultura_discussion',
  );

  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'culturaexchange_xml');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_xml';
  $handler->display->display_options['style_options']['provide_file'] = 0;
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['transform'] = 1;
  $handler->display->display_options['style_options']['root_node'] = 'nodes';
  $handler->display->display_options['style_options']['item_node'] = 'node';
  $handler->display->display_options['style_options']['no_entity_encode'] = array(
    'cultura_discussion_answers' => 'cultura_discussion_answers',
  );
  $handler->display->display_options['path'] = 'admin/cultura/export/preview.xml';

  $views['cultura_export'] = $view;

  $view = new view();
  $view->name = 'cultura_comment_export';
  $view->description = 'Comments export portion of view';
  $view->tag = 'default';
  $view->base_table = 'comment';
  $view->human_name = 'Cultura comment export';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Comment: Content */
  $handler->display->display_options['relationships']['nid']['id'] = 'nid';
  $handler->display->display_options['relationships']['nid']['table'] = 'comment';
  $handler->display->display_options['relationships']['nid']['field'] = 'nid';
  $handler->display->display_options['relationships']['nid']['required'] = TRUE;
  /* Field: Comment: ID */
  $handler->display->display_options['fields']['cid']['id'] = 'cid';
  $handler->display->display_options['fields']['cid']['table'] = 'comment';
  $handler->display->display_options['fields']['cid']['field'] = 'cid';
  $handler->display->display_options['fields']['cid']['label'] = 'cid';
  $handler->display->display_options['fields']['cid']['element_type'] = '0';
  $handler->display->display_options['fields']['cid']['element_label_type'] = '0';
  $handler->display->display_options['fields']['cid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['cid']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['cid']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['cid']['link_to_comment'] = FALSE;
  /* Field: Comment: Title */
  $handler->display->display_options['fields']['subject']['id'] = 'subject';
  $handler->display->display_options['fields']['subject']['table'] = 'comment';
  $handler->display->display_options['fields']['subject']['field'] = 'subject';
  $handler->display->display_options['fields']['subject']['label'] = 'title';
  $handler->display->display_options['fields']['subject']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['subject']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['subject']['element_label_colon'] = FALSE;
  /* Field: Comment: Author */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'comment';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = 'author';
  $handler->display->display_options['fields']['name']['element_type'] = '0';
  $handler->display->display_options['fields']['name']['element_label_type'] = '0';
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['name']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['name']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
  /* Field: Comment: Comment */
  $handler->display->display_options['fields']['comment_body']['id'] = 'comment_body';
  $handler->display->display_options['fields']['comment_body']['table'] = 'field_data_comment_body';
  $handler->display->display_options['fields']['comment_body']['field'] = 'comment_body';
  $handler->display->display_options['fields']['comment_body']['label'] = 'comment';
  $handler->display->display_options['fields']['comment_body']['element_type'] = '0';
  $handler->display->display_options['fields']['comment_body']['element_label_type'] = '0';
  $handler->display->display_options['fields']['comment_body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['comment_body']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['comment_body']['element_default_classes'] = FALSE;
  /* Field: Comment: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'comment';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'post_date';
  $handler->display->display_options['fields']['created']['element_type'] = '0';
  $handler->display->display_options['fields']['created']['element_label_type'] = '0';
  $handler->display->display_options['fields']['created']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['created']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['created']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['created']['date_format'] = 'custom';
  $handler->display->display_options['fields']['created']['custom_date_format'] = 'c';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  $handler->display->display_options['fields']['created']['timezone'] = 'UTC';
  /* Field: Comment: Nid */
  $handler->display->display_options['fields']['nid_1']['id'] = 'nid_1';
  $handler->display->display_options['fields']['nid_1']['table'] = 'comment';
  $handler->display->display_options['fields']['nid_1']['field'] = 'nid';
  $handler->display->display_options['fields']['nid_1']['label'] = 'nid';
  $handler->display->display_options['fields']['nid_1']['element_type'] = '0';
  $handler->display->display_options['fields']['nid_1']['element_label_type'] = '0';
  $handler->display->display_options['fields']['nid_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nid_1']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['nid_1']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['nid_1']['separator'] = '';
  /* Sort criterion: Comment: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'comment';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  /* Filter criterion: Comment: Approved */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'comment';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status_node']['id'] = 'status_node';
  $handler->display->display_options['filters']['status_node']['table'] = 'node';
  $handler->display->display_options['filters']['status_node']['field'] = 'status';
  $handler->display->display_options['filters']['status_node']['relationship'] = 'nid';
  $handler->display->display_options['filters']['status_node']['value'] = 1;
  $handler->display->display_options['filters']['status_node']['group'] = 1;
  $handler->display->display_options['filters']['status_node']['expose']['operator'] = FALSE;

  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'culturaexchange_comment_xml');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_xml';
  $handler->display->display_options['style_options']['provide_file'] = 0;
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['transform'] = 1;
  $handler->display->display_options['style_options']['transform_type'] = 'underline';
  $handler->display->display_options['style_options']['root_node'] = 'comments';
  $handler->display->display_options['style_options']['item_node'] = 'comment';
  $handler->display->display_options['path'] = 'admin/cultura/export/comment-preview.xml';

  $views['cultura_comment_export'] = $view;

  return $views;
}
