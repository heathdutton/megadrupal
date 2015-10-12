<?php

$view = new view();
$view->name = 'quizz_questions';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'quiz_question_entity';
$view->human_name = 'Questions';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Questions';
$handler->display->display_options['use_ajax'] = TRUE;
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['exposed_form']['options']['autosubmit'] = TRUE;
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '50';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['pager']['options']['quantity'] = '9';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
    'qid'     => 'qid',
    'title'   => 'title',
    'type'    => 'type',
    'created' => 'created',
);
$handler->display->display_options['style_options']['default'] = 'created';
$handler->display->display_options['style_options']['info'] = array(
    'qid'     => array(
        'sortable'           => 0,
        'default_sort_order' => 'asc',
        'align'              => '',
        'separator'          => '',
        'empty_column'       => 0,
    ),
    'title'   => array(
        'sortable'           => 0,
        'default_sort_order' => 'asc',
        'align'              => '',
        'separator'          => '',
        'empty_column'       => 0,
    ),
    'type'    => array(
        'sortable'           => 0,
        'default_sort_order' => 'asc',
        'align'              => '',
        'separator'          => '',
        'empty_column'       => 0,
    ),
    'created' => array(
        'sortable'           => 1,
        'default_sort_order' => 'desc',
        'align'              => '',
        'separator'          => '',
        'empty_column'       => 0,
    ),
);
/* No results behavior: Global: Text area */
$handler->display->display_options['empty']['area']['id'] = 'area';
$handler->display->display_options['empty']['area']['table'] = 'views';
$handler->display->display_options['empty']['area']['field'] = 'area';
$handler->display->display_options['empty']['area']['empty'] = TRUE;
$handler->display->display_options['empty']['area']['content'] = 'Empty';
$handler->display->display_options['empty']['area']['format'] = 'filtered_html';
/* Field: Question: Question ID */
$handler->display->display_options['fields']['qid']['id'] = 'qid';
$handler->display->display_options['fields']['qid']['table'] = 'quiz_question_entity';
$handler->display->display_options['fields']['qid']['field'] = 'qid';
$handler->display->display_options['fields']['qid']['exclude'] = TRUE;
$handler->display->display_options['fields']['qid']['separator'] = '';
/* Field: Question: Label */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'quiz_question_entity';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = 'Question';
$handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
$handler->display->display_options['fields']['title']['alter']['path'] = 'quiz-question/[qid]';
/* Field: Question: Type */
$handler->display->display_options['fields']['type']['id'] = 'type';
$handler->display->display_options['fields']['type']['table'] = 'quiz_question_entity';
$handler->display->display_options['fields']['type']['field'] = 'type';
/* Field: Question: Created */
$handler->display->display_options['fields']['created']['id'] = 'created';
$handler->display->display_options['fields']['created']['table'] = 'quiz_question_entity';
$handler->display->display_options['fields']['created']['field'] = 'created';
$handler->display->display_options['fields']['created']['date_format'] = 'short';
$handler->display->display_options['fields']['created']['second_date_format'] = 'long';
/* Filter criterion: Question: Question category (quizz_question_category) */
$handler->display->display_options['filters']['quizz_question_category_tid']['id'] = 'quizz_question_category_tid';
$handler->display->display_options['filters']['quizz_question_category_tid']['table'] = 'field_data_quizz_question_category';
$handler->display->display_options['filters']['quizz_question_category_tid']['field'] = 'quizz_question_category_tid';
$handler->display->display_options['filters']['quizz_question_category_tid']['group'] = 1;
$handler->display->display_options['filters']['quizz_question_category_tid']['exposed'] = TRUE;
$handler->display->display_options['filters']['quizz_question_category_tid']['expose']['operator_id'] = 'quizz_question_category_tid_op';
$handler->display->display_options['filters']['quizz_question_category_tid']['expose']['label'] = 'Category';
$handler->display->display_options['filters']['quizz_question_category_tid']['expose']['operator'] = 'quizz_question_category_tid_op';
$handler->display->display_options['filters']['quizz_question_category_tid']['expose']['identifier'] = 'quizz_question_category_tid';
$handler->display->display_options['filters']['quizz_question_category_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
);
$handler->display->display_options['filters']['quizz_question_category_tid']['type'] = 'select';
$handler->display->display_options['filters']['quizz_question_category_tid']['vocabulary'] = 'quiz_question_category';
$handler->display->display_options['filters']['quizz_question_category_tid']['hierarchy'] = 1;
/* Filter criterion: Question: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'quiz_question_entity';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['group'] = 1;
$handler->display->display_options['filters']['type']['exposed'] = TRUE;
$handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
$handler->display->display_options['filters']['type']['expose']['label'] = 'Type';
$handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
$handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
$handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
);
/* Filter criterion: Question: Created */
$handler->display->display_options['filters']['created']['id'] = 'created';
$handler->display->display_options['filters']['created']['table'] = 'quiz_question_entity';
$handler->display->display_options['filters']['created']['field'] = 'created';
$handler->display->display_options['filters']['created']['operator'] = '>=';
$handler->display->display_options['filters']['created']['group'] = 1;
$handler->display->display_options['filters']['created']['exposed'] = TRUE;
$handler->display->display_options['filters']['created']['expose']['operator_id'] = 'created_op';
$handler->display->display_options['filters']['created']['expose']['label'] = 'Created';
$handler->display->display_options['filters']['created']['expose']['operator'] = 'created_op';
$handler->display->display_options['filters']['created']['expose']['identifier'] = 'created';
$handler->display->display_options['filters']['created']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
);
$handler->display->display_options['filters']['created']['form_type'] = 'date_popup';
$handler->display->display_options['filters']['created']['year_range'] = '-1:+0';

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page');
$handler->display->display_options['path'] = 'questions';
$handler->display->display_options['menu']['type'] = 'normal';
$handler->display->display_options['menu']['title'] = 'Questions';
$handler->display->display_options['menu']['name'] = 'main-menu';

return $view;
