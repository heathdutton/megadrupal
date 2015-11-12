<?php
/**
 * @file
 * best_answer.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function best_answer_views_default_views() {
  $export = array();

  $view = new view;
  $view->name = 'question_answers_best_answers_';
  $view->description = 'Answers to the current question';
  $view->tag = 'Answers';
  $view->base_table = 'node';
  $view->human_name = 'Question_Answers (Best Answers)';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'node';
  $handler->display->display_options['row_options']['view_mode'] = 'full';
  $handler->display->display_options['row_options']['links'] = 1;
  $handler->display->display_options['row_options']['comments'] = 0;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = '<em>No answers yet</em>';
  /* Sort criterion: Content: Best Answer (field_best_answer_p) */
  $handler->display->display_options['sorts']['field_best_answer_p_value']['id'] = 'field_best_answer_p_value';
  $handler->display->display_options['sorts']['field_best_answer_p_value']['table'] = 'field_data_field_best_answer_p';
  $handler->display->display_options['sorts']['field_best_answer_p_value']['field'] = 'field_best_answer_p_value';
  $handler->display->display_options['sorts']['field_best_answer_p_value']['order'] = 'DESC';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Question (field_answer_question) */
  $handler->display->display_options['arguments']['field_answer_question_nid']['id'] = 'field_answer_question_nid';
  $handler->display->display_options['arguments']['field_answer_question_nid']['table'] = 'field_data_field_answer_question';
  $handler->display->display_options['arguments']['field_answer_question_nid']['field'] = 'field_answer_question_nid';
  $handler->display->display_options['arguments']['field_answer_question_nid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_answer_question_nid']['exception']['title_enable'] = 1;
  $handler->display->display_options['arguments']['field_answer_question_nid']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['field_answer_question_nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_answer_question_nid']['specify_validation'] = 1;
  $handler->display->display_options['arguments']['field_answer_question_nid']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['field_answer_question_nid']['validate_options']['types'] = array(
    'question' => 'question',
  );
  $handler->display->display_options['arguments']['field_answer_question_nid']['validate_options']['access'] = 0;
  $handler->display->display_options['arguments']['field_answer_question_nid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['field_answer_question_nid']['not'] = 0;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'answer' => 'answer',
  );

  /* Display: Entity content */
  $handler = $view->new_display('entity_view', 'Entity content', 'entity_view_1');
  $handler->display->display_options['entity_type'] = 'node';
  $handler->display->display_options['bundles'] = array(
    0 => 'question',
  );
  $handler->display->display_options['show_title'] = 0;
  $export['question_answers_best_answers_'] = $view;

  $view = new view;
  $view->name = 'question_list_best_answers_';
  $view->description = 'Display lists of questions';
  $view->tag = 'Answers';
  $view->base_table = 'node';
  $view->human_name = 'Question_List (Best Answers)';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'created' => 'created',
    'entity_id' => 'entity_id',
  );
  $handler->display->display_options['style_options']['default'] = 'created';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-left',
      'separator' => '',
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => 'views-align-center',
      'separator' => '',
    ),
    'entity_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => 'views-align-center',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 1;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = TRUE;
  $handler->display->display_options['header']['area']['content'] = '<em>"Resolved questions" have a "Best Answer" selected</em>';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = '<em>No questions ...</em>';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Question';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['created']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['created']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['created']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['created']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['created']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['created']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['created']['alter']['html'] = 0;
  $handler->display->display_options['fields']['created']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['created']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['created']['hide_empty'] = 0;
  $handler->display->display_options['fields']['created']['empty_zero'] = 0;
  $handler->display->display_options['fields']['created']['date_format'] = 'raw time ago';
  /* Field: Content: Answer Count */
  $handler->display->display_options['fields']['entity_id']['id'] = 'entity_id';
  $handler->display->display_options['fields']['entity_id']['table'] = 'field_data_field_answer_count';
  $handler->display->display_options['fields']['entity_id']['field'] = 'field_answer_count';
  $handler->display->display_options['fields']['entity_id']['label'] = 'Answers';
  $handler->display->display_options['fields']['entity_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['entity_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['entity_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['entity_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['entity_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['entity_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['entity_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['entity_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['entity_id']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['entity_id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['entity_id']['empty'] = 'none';
  $handler->display->display_options['fields']['entity_id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['entity_id']['empty_zero'] = 1;
  $handler->display->display_options['fields']['entity_id']['type'] = 'nodereference_count_default';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'question' => 'question',
  );

  /* Display: Resolved Questions */
  $handler = $view->new_display('page', 'Resolved Questions', 'page_6');
  $handler->display->display_options['display_description'] = 'Questions for which a "best answer" has been selected';
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = TRUE;
  $handler->display->display_options['header']['area']['content'] = '<em>"Resolved questions" have a "Best Answer" selected.</em>';
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = '<em>Currently, no questions are resolved ....</em>';
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'question' => 'question',
  );
  /* Filter criterion: Content: Best Answer (field_best_answer) */
  $handler->display->display_options['filters']['field_best_answer_nid']['id'] = 'field_best_answer_nid';
  $handler->display->display_options['filters']['field_best_answer_nid']['table'] = 'field_data_field_best_answer';
  $handler->display->display_options['filters']['field_best_answer_nid']['field'] = 'field_best_answer_nid';
  $handler->display->display_options['filters']['field_best_answer_nid']['operator'] = 'not empty';
  $handler->display->display_options['path'] = 'questions/resolved';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Resolved Questions';
  $handler->display->display_options['menu']['description'] = 'Questions for which a "Best Answer" has been selected';
  $handler->display->display_options['menu']['weight'] = '10';

  /* Display: Unresolved Questions */
  $handler = $view->new_display('page', 'Unresolved Questions', 'page_7');
  $handler->display->display_options['display_description'] = 'Questions which have answers but for which no "Best Answer" has been selected';
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = TRUE;
  $handler->display->display_options['header']['area']['content'] = '<em>"Unresolved questions" have answers but no "Best Answer" selected yet</em>';
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = '<em>Currently, no questions are unresolved ....</em>';
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'question' => 'question',
  );
  /* Filter criterion: Content: Answer Count (field_answer_count) */
  $handler->display->display_options['filters']['field_answer_count_count']['id'] = 'field_answer_count_count';
  $handler->display->display_options['filters']['field_answer_count_count']['table'] = 'field_data_field_answer_count';
  $handler->display->display_options['filters']['field_answer_count_count']['field'] = 'field_answer_count_count';
  $handler->display->display_options['filters']['field_answer_count_count']['operator'] = '>';
  $handler->display->display_options['filters']['field_answer_count_count']['value']['value'] = '0';
  /* Filter criterion: Content: Best Answer (field_best_answer) */
  $handler->display->display_options['filters']['field_best_answer_nid']['id'] = 'field_best_answer_nid';
  $handler->display->display_options['filters']['field_best_answer_nid']['table'] = 'field_data_field_best_answer';
  $handler->display->display_options['filters']['field_best_answer_nid']['field'] = 'field_best_answer_nid';
  $handler->display->display_options['filters']['field_best_answer_nid']['operator'] = 'empty';
  $handler->display->display_options['path'] = 'questions/unresolved';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Unresolved Questions';
  $handler->display->display_options['menu']['description'] = 'Questions which have answers but for which no "Best Answer" has been selected';
  $handler->display->display_options['menu']['weight'] = '20';
  $export['question_list_best_answers_'] = $view;

  return $export;
}
