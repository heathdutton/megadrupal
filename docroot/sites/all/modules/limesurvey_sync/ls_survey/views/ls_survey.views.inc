<?php
/**
 * @file
 * Functions for compatibility between ls_survey module and Views module
 */

/**
 * Implements hook_views_data().
 */
function ls_survey_views_data() {
  $data = array();
  $data['node_type']['table']['group'] = t('Node type');

  // tables + fields that can be used for SQL Joins
  // Handle type field.
  $data['node_type']['type'] = array(
    'title' => t('Type machine-name'), // The item it appears as on the UI,
    // The help that appears on the UI,
    'help' => t('The type of a node (for example, "blog", "forum", "story", etc).'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_node_type',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_type',
    ),
    'relationship' => array(
      'label' => t('Answers nodes'),
      'base' => 'node',
      'base field' => 'type',
      // This allows us to not show this relationship if the base is already
      // node so users won't create circular relationships.
      'skip base' => array('node'),
    ),
  );

  // Handle orig_type field.
  $data['node_type']['orig_type'] = array(
    'title' => t('Original type machine name'),
    'help' => t('The original machine-readable name of this node type. This may be different from the current type name'),
    // This allows us to not show this relationship if the base is already
    // node_revisions so users won't create circular relationships.
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'relationship' => array(
      'label' => t('Answers content type'),
      'base' => 'ls_survey',
      'base field' => 'ls_orig_type',
      'skip base' => array('node'),
    ),
  );

  $data['node_type']['table']['join'] = array(
    'node' => array(
      'left_field' => 'type',
      'field' => 'type',
    ),
    'ls_answ' => array(
      'left_table' => 'node',
      'left_field' => 'type',
      'field' => 'type',
    ),
  );

  $data['ls_survey']['table']['group'] = t('LS surveys');
  // tables + fields that can be used for SQL Joins
  $data['ls_survey']['table']['join'] = array(
    'node_revisions' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
    'node' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
    'node_type' => array(
      'left_field' => 'orig_type',
      'field' => 'ls_orig_type',
    ),
  );

  module_load_include('inc', 'ls_survey', 'ls_survey');
  // ls_sid
  $data['ls_survey']['ls_sid'] = array(
    'title' => ls_survey_display_sync_headers('ls_sid'),
    'help' => t('Default survey ID'),
    'field' => array(
      'handler' => 'ls_survey_views_handlers_field_ls_fields',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // ls_lang
  $data['ls_survey']['ls_lang'] = array(
    'title' => ls_survey_display_sync_headers('ls_lang'),
    'help' => t('Default survey language'),
    'field' => array(
      'handler' => 'ls_survey_views_handlers_field_ls_fields',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Handle ls_orig_type.
  $data['ls_survey']['ls_orig_type'] = array(
    'title' => ls_survey_display_sync_headers('ls_orig_type'),
    'help' => t('Type of the associated answers content type'),
    'field' => array(
      'handler' => 'ls_survey_views_handlers_field_ls_fields',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'relationship' => array(
      'label' => t('Associated answers content type'),
      'base' => 'node_type',
      'base field' => 'orig_type',
    ),
  );

  $data['ls_survey']['table']['base'] = array(
    'field' => 'vid',
    'title' => t('LimeSurvey surveys'),
    'help' => t("LimeSurvey surveys nodes"),
    'weight' => 1,
  );

  // When using the new 'LimeSurvey' type you need to use relationships to
  // access fields in other tables.

  // Relationship to the 'Node revision' table

  $data['ls_survey']['vid'] = array(
    'title' => t('Survey revisions'),
    'help' => t('LimeSurvey survey node revision'),
    'relationship' => array(
      'label' => t('Node revision'),
      'base' => 'node_revisions',
      'base field' => 'vid',
      // This allows us to not show this relationship if the base is already
      // node_revisions so users won't create circular relationships.
      'skip base' => array('node'),
    ),
  );

  // Relationship to the 'Node' table
  $data['ls_survey']['nid'] = array(
    'title' => t('Survey node'),
    'help' => t('LimeSurvey survey node'),
    'relationship' => array(
      'label' => t('Survey node'),
      'base' => 'node',
      'base field' => 'nid',
      // This allows us to not show this relationship if the base is already
      // node so users won't create circular relationships.
      'skip base' => array('node_revisions'),
    ),
  );

  return $data;
}

/**
 * Implements hook_views_default_views().
 */
function ls_survey_views_default_views() {
  $view = new view();
  $view->name = 'surveys_and_associated_answers';
  $view->description = 'This is a basic start in order to create views lists. It query all surveys with their associated answers.';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'surveys_and_associated_answers';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '30';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'title_1' => 'title_1',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: LS surveys: Answers content type */
  $handler->display->display_options['relationships']['ls_orig_type']['id'] = 'ls_orig_type';
  $handler->display->display_options['relationships']['ls_orig_type']['table'] = 'ls_survey';
  $handler->display->display_options['relationships']['ls_orig_type']['field'] = 'ls_orig_type';
  /* Relationship: Node type: Type machine-name */
  $handler->display->display_options['relationships']['type']['id'] = 'type';
  $handler->display->display_options['relationships']['type']['table'] = 'node_type';
  $handler->display->display_options['relationships']['type']['field'] = 'type';
  $handler->display->display_options['relationships']['type']['relationship'] = 'ls_orig_type';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Survey title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: LS surveys: Answers content type */
  $handler->display->display_options['fields']['ls_orig_type']['id'] = 'ls_orig_type';
  $handler->display->display_options['fields']['ls_orig_type']['table'] = 'ls_survey';
  $handler->display->display_options['fields']['ls_orig_type']['field'] = 'ls_orig_type';
  $handler->display->display_options['fields']['ls_orig_type']['exclude'] = TRUE;
  /* Field: LS surveys: Default survey ID */
  $handler->display->display_options['fields']['ls_sid']['id'] = 'ls_sid';
  $handler->display->display_options['fields']['ls_sid']['table'] = 'ls_survey';
  $handler->display->display_options['fields']['ls_sid']['field'] = 'ls_sid';
  /* Field: LS surveys: Default survey language */
  $handler->display->display_options['fields']['ls_lang']['id'] = 'ls_lang';
  $handler->display->display_options['fields']['ls_lang']['table'] = 'ls_survey';
  $handler->display->display_options['fields']['ls_lang']['field'] = 'ls_lang';
  /* Field: Node type: Type machine-name */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'node_type';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['relationship'] = 'ls_orig_type';
  /* Field: Content: Type */
  $handler->display->display_options['fields']['type_1']['id'] = 'type_1';
  $handler->display->display_options['fields']['type_1']['table'] = 'node';
  $handler->display->display_options['fields']['type_1']['field'] = 'type';
  $handler->display->display_options['fields']['type_1']['relationship'] = 'type';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['relationship'] = 'type';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_2']['id'] = 'title_2';
  $handler->display->display_options['fields']['title_2']['table'] = 'node';
  $handler->display->display_options['fields']['title_2']['field'] = 'title';
  $handler->display->display_options['fields']['title_2']['relationship'] = 'type';
  /* Field: LS answers: Answer status */
  $handler->display->display_options['fields']['ls_status']['id'] = 'ls_status';
  $handler->display->display_options['fields']['ls_status']['table'] = 'ls_answ';
  $handler->display->display_options['fields']['ls_status']['field'] = 'ls_status';
  $handler->display->display_options['fields']['ls_status']['relationship'] = 'type';
  /* Field: LS answers: Synchronization status */
  $handler->display->display_options['fields']['ls_sync']['id'] = 'ls_sync';
  $handler->display->display_options['fields']['ls_sync']['table'] = 'ls_answ';
  $handler->display->display_options['fields']['ls_sync']['field'] = 'ls_sync';
  $handler->display->display_options['fields']['ls_sync']['relationship'] = 'type';
  /* Field: LS answers: Last import */
  $handler->display->display_options['fields']['ls_last_import']['id'] = 'ls_last_import';
  $handler->display->display_options['fields']['ls_last_import']['table'] = 'ls_answ';
  $handler->display->display_options['fields']['ls_last_import']['field'] = 'ls_last_import';
  $handler->display->display_options['fields']['ls_last_import']['relationship'] = 'type';
  $handler->display->display_options['fields']['ls_last_import']['date_format'] = 'long';
  /* Field: LS answers: Answer ID */
  $handler->display->display_options['fields']['ls_aid']['id'] = 'ls_aid';
  $handler->display->display_options['fields']['ls_aid']['table'] = 'ls_answ';
  $handler->display->display_options['fields']['ls_aid']['field'] = 'ls_aid';
  $handler->display->display_options['fields']['ls_aid']['relationship'] = 'type';
  /* Field: LS answers: Survey ID */
  $handler->display->display_options['fields']['ls_sid_1']['id'] = 'ls_sid_1';
  $handler->display->display_options['fields']['ls_sid_1']['table'] = 'ls_answ';
  $handler->display->display_options['fields']['ls_sid_1']['field'] = 'ls_sid';
  $handler->display->display_options['fields']['ls_sid_1']['relationship'] = 'type';
  /* Field: LS answers: Survey language */
  $handler->display->display_options['fields']['ls_lang_1']['id'] = 'ls_lang_1';
  $handler->display->display_options['fields']['ls_lang_1']['table'] = 'ls_answ';
  $handler->display->display_options['fields']['ls_lang_1']['field'] = 'ls_lang';
  $handler->display->display_options['fields']['ls_lang_1']['relationship'] = 'type';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'survey' => 'survey',
  );
  $translatables['surveys_and_associated_answers'] = array(
    t('Master'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Associated answers content type'),
    t('Answers nodes'),
    t('Survey title'),
    t('Answers content type'),
    t('Default survey ID'),
    t('Default survey language'),
    t('Type machine-name'),
    t('Type'),
    t('Nid'),
    t('Title'),
    t('Answer status'),
    t('Synchronization status'),
    t('Last import'),
    t('Answer ID'),
    t('Survey ID'),
    t('Survey language'),
  );
  // Add view to list of views to provide.
  $views[$view->name] = $view;

  // At the end, return array of default views.
  return $views;
}
