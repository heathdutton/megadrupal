<?php
/**
 * @file
 * Implements default views for UI.
 */

/**
 * Implements hook_views_default_views().
 */
function elearning_ui_views_default_views() {
  $views = array();
  $elearning_entities = elearning_get_elearning_entity_info();
  foreach ($elearning_entities as $entity_type => $entity_info) {

    $bundles = array_keys($entity_info['bundles']);

    $weight = array_search($entity_type, array_keys($elearning_entities));
    $view = new view();
    $view->name = $entity_type;
    $view->description = 'A list of all ' . $entity_info['plural label'];
    $view->tag = 'elearning';
    $view->base_table = $entity_type;
    $view->human_name = $entity_info['plural label'];
    $view->core = 7;
    $view->api_version = '3.0';
    $view->disabled = FALSE;

    $handler = $view->new_display('default', 'Master', 'default');
    $handler->display->display_options['title'] = $entity_info['plural label'];
    $handler->display->display_options['use_ajax'] = TRUE;
    $handler->display->display_options['use_more_always'] = FALSE;
    $handler->display->display_options['access']['type'] = 'perm';
    $handler->display->display_options['access']['perm'] = 'administer ' . str_replace('_', ' ', $entity_type);
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['exposed_form']['options']['autosubmit'] = TRUE;
    $handler->display->display_options['pager']['type'] = 'full';
    $handler->display->display_options['pager']['options']['items_per_page'] = '20';
    $handler->display->display_options['style_plugin'] = 'table';
    $handler->display->display_options['style_options']['columns'] = array(
      'name' => 'name',
      $entity_info['entity keys']['id'] => $entity_info['entity keys']['id'],
    );
    $handler->display->display_options['style_options']['default'] = '-1';
    $handler->display->display_options['style_options']['info'] = array(
      'name' => array(
        'sortable' => 0,
        'default_sort_order' => 'asc',
        'align' => '',
        'separator' => '',
      ),
      $entity_info['entity keys']['id'] => array(
        'sortable' => 0,
        'default_sort_order' => 'asc',
        'align' => '',
        'separator' => '',
      ),
    );

    $handler->display->display_options['empty']['area']['id'] = 'area';
    $handler->display->display_options['empty']['area']['table'] = 'views';
    $handler->display->display_options['empty']['area']['field'] = 'area';
    $handler->display->display_options['empty']['area']['label'] = 'Empty';
    $handler->display->display_options['empty']['area']['content'] = 'No ' . strtolower($entity_info['plural label']) . ' have been created yet';

    $handler->display->display_options['fields'][$entity_info['entity keys']['id']]['id'] = $entity_info['entity keys']['id'];
    $handler->display->display_options['fields'][$entity_info['entity keys']['id']]['table'] = $entity_type;
    $handler->display->display_options['fields'][$entity_info['entity keys']['id']]['field'] = $entity_info['entity keys']['id'];

    $handler->display->display_options['fields']['label']['id'] = 'label';
    $handler->display->display_options['fields']['label']['table'] = $entity_type;
    $handler->display->display_options['fields']['label']['field'] = 'label';

    if (!empty($entity_info['bundles']) && $bundles[0] != $entity_type) {

      $handler->display->display_options['fields']['type']['id'] = 'type';
      $handler->display->display_options['fields']['type']['table'] = $entity_type;
      $handler->display->display_options['fields']['type']['field'] = 'type';
    }

    $handler->display->display_options['fields']['operations']['id'] = 'operations';
    $handler->display->display_options['fields']['operations']['table'] = $entity_type;
    $handler->display->display_options['fields']['operations']['field'] = 'operations';

    $handler = $view->new_display('page', $entity_info['plural label'], $entity_type);
    $handler->display->display_options['path'] = 'admin/elearning/' . str_replace('elearning_', '', $entity_type) . '/list';
    $handler->display->display_options['menu']['type'] = 'default tab';
    $handler->display->display_options['menu']['title'] = 'List';
    $handler->display->display_options['menu']['weight'] = $weight - 10;
    $handler->display->display_options['menu']['context'] = 0;
    $handler->display->display_options['menu']['context_only_inline'] = 0;
    $handler->display->display_options['tab_options']['type'] = 'normal';
    $handler->display->display_options['tab_options']['title'] = $entity_info['plural label'];
    $handler->display->display_options['tab_options']['weight'] = $weight - 10;
    $handler->display->display_options['tab_options']['name'] = 'management';

    $views[$view->name] = $view;

    $view = new view();
    $view->name = $entity_type . '_result';
    $view->description = '';
    $view->tag = 'default';
   $view->base_table = $entity_type . '_result';
    $view->human_name = $entity_info['label'] . ' results';
    $view->core = 7;
    $view->api_version = '3.0';
    $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

    /* Display: Master */
    $handler = $view->new_display('default', 'Master', 'default');
    $handler->display->display_options['title'] = $entity_info['label'] . ' results';
    $handler->display->display_options['use_more_always'] = FALSE;
    $handler->display->display_options['access']['type'] = 'perm';
    $handler->display->display_options['access']['perm'] = 'administer elearning results';
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['pager']['type'] = 'full';
    $handler->display->display_options['pager']['options']['items_per_page'] = '50';
    $handler->display->display_options['style_plugin'] = 'table';

    $handler->display->display_options['style_options']['columns'] = array(
      'views_bulk_operations' => 'views_bulk_operations',
      'result_id' => 'result_id',
      'question' => 'question',
      'name' => 'name',
      'correct' => 'correct',
    );
    $handler->display->display_options['style_options']['default'] = 'result_id';
    $handler->display->display_options['style_options']['info'] = array(
      'views_bulk_operations' => array(
        'align' => '',
        'separator' => '',
        'empty_column' => 0,
      ),
      'result_id' => array(
        'sortable' => 1,
        'default_sort_order' => 'desc',
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
    $handler->display->display_options['empty']['area']['content'] = 'There are no '.$entity_info['label'].' results available.';
    $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
    /* Relationship: Course result: Owner */
    $handler->display->display_options['relationships']['uid']['id'] = 'uid';
    $handler->display->display_options['relationships']['uid']['table'] = $entity_type . '_result';
    $handler->display->display_options['relationships']['uid']['field'] = 'uid';
    /* Field: Bulk operations: Course result */
    $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
    $handler->display->display_options['fields']['views_bulk_operations']['table'] = $entity_type . '_result';
    $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
    $handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
    $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
    $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
    $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
    $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
    $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
      'action::views_bulk_operations_delete_item' => array(
        'selected' => 1,
        'postpone_processing' => 0,
        'skip_confirmation' => 0,
        'override_label' => 0,
        'label' => '',
      ),
    );
    /* Field: Course result: Course result ID */
    $handler->display->display_options['fields']['result_id']['id'] = 'result_id';
    $handler->display->display_options['fields']['result_id']['table'] = $entity_type . '_result';
    $handler->display->display_options['fields']['result_id']['field'] = 'result_id';
    /* Field: Course result: Course */
    $handler->display->display_options['fields'][str_replace('elearning_', '', $entity_type)]['id'] = $entity_type;
    $handler->display->display_options['fields'][str_replace('elearning_', '', $entity_type)]['table'] = 'views_entity_'.$entity_type . '_result';
    $handler->display->display_options['fields'][str_replace('elearning_', '', $entity_type)]['field'] = str_replace('elearning_', '', $entity_type);
    $handler->display->display_options['fields'][str_replace('elearning_', '', $entity_type)]['link_to_entity'] = 1;
    $handler->display->display_options['fields'][str_replace('elearning_', '', $entity_type)]['bypass_access'] = 1;

    /* Field: User: Name */
    $handler->display->display_options['fields']['name']['id'] = 'name';
    $handler->display->display_options['fields']['name']['table'] = 'users';
    $handler->display->display_options['fields']['name']['field'] = 'name';
    $handler->display->display_options['fields']['name']['relationship'] = 'uid';
    $handler->display->display_options['fields']['name']['label'] = 'Owner';

    if ($entity_type != "elearning_question") {

      $handler->display->display_options['fields']['score_formatted']['id'] = 'score_formatted';
      $handler->display->display_options['fields']['score_formatted']['table'] = 'views_entity_' . $entity_type . '_result';
      $handler->display->display_options['fields']['score_formatted']['field'] = 'score_formatted';
      $handler->display->display_options['fields']['score_formatted']['label'] = 'Score';
      $handler->display->display_options['fields']['score_formatted']['link_to_entity'] = 0;

      $handler->display->display_options['filters']['status']['id'] = 'status';
      $handler->display->display_options['filters']['status']['table'] = $entity_type . '_result';
      $handler->display->display_options['filters']['status']['field'] = 'status';
      $handler->display->display_options['filters']['status']['exposed'] = TRUE;
      $handler->display->display_options['filters']['status']['expose']['operator_id'] = 'status_op';
      $handler->display->display_options['filters']['status']['expose']['label'] = 'Status';
      $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
      $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
    }
    else {
      $handler->display->display_options['fields']['correct']['id'] = 'correct';
      $handler->display->display_options['fields']['correct']['table'] = $entity_type . '_result';
      $handler->display->display_options['fields']['correct']['field'] = 'correct';
      $handler->display->display_options['fields']['correct']['type'] = 'unicode-yes-no';
      $handler->display->display_options['fields']['correct']['not'] = 0;
    }

    /* Filter criterion: User: Name */
    $handler->display->display_options['filters']['uid']['id'] = 'uid';
    $handler->display->display_options['filters']['uid']['table'] = 'users';
    $handler->display->display_options['filters']['uid']['field'] = 'uid';
    $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
    $handler->display->display_options['filters']['uid']['value'] = '';
    $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
    $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
    $handler->display->display_options['filters']['uid']['expose']['label'] = 'Owner';
    $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
    $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
    $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
      2 => '2',
      1 => 0,
      3 => 0,
      4 => 0,
      5 => 0,
    );

    /* Display: Course results */
    $handler = $view->new_display('page', $entity_info['label'] .' results', $entity_type . '_result');
    $handler->display->display_options['path'] = 'admin/elearning/'.str_replace('elearning_', '', $entity_type).'/results';
    $handler->display->display_options['menu']['type'] = 'normal';
    $handler->display->display_options['menu']['title'] = 'Results';
    $handler->display->display_options['menu']['description'] = $entity_info['label'] .' results';
    $handler->display->display_options['menu']['weight'] = '0';
    $handler->display->display_options['menu']['name'] = 'management';
    $handler->display->display_options['menu']['context'] = 1;
    $handler->display->display_options['menu']['context_only_inline'] = 0;
    $handler->display->display_options['tab_options']['weight'] = '0';
    $translatables[$entity_type.'_results'] = array(
      t('Master'),
      t('@entity_type results',array('@entity_type' =>  strtolower($entity_info['label']))),
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
      t('There are no entity_type results available.',array('@entity_type' =>  strtolower($entity_info['label']))),
      t('User'),
      t('- Choose an operation -'),
      t('@entity_type result ID',array('@entity_type' =>  strtolower($entity_info['label']))),
      t('.'),
      t(','),
      t('@entity_type',array('@entity_type' =>  strtolower($entity_info['label']))),
      t('Score'),
      t('Owner'),
      t('Status'),
    );

      $views[$view->name] = $view;
  }


  $view = new view();
  $view->name = 'elearning_course_access';
  $view->description = '';
  $view->tag = 'elearning';
  $view->base_table = 'elearning_course_access';
  $view->human_name = 'Course access records';
  $view->core = 7;
  $view->api_version = '3.0';
  /* Edit this to true to make a default view disabled initially */

  $view->disabled = FALSE;

  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Course access';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer elearning course access';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'course_access_id' => 'course_access_id',
    'name' => 'name',
    'label' => 'label',
    'granted' => 'granted',
    'status' => 'status',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'course_access_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'label' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'granted' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
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
  $handler->display->display_options['empty']['area']['content'] = 'There are no courses available for granting or denying access.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Relationship: Course Access Record: Course */

  $handler->display->display_options['relationships']['course_id']['id'] = 'course_id';
  $handler->display->display_options['relationships']['course_id']['table'] = 'elearning_course_access';
  $handler->display->display_options['relationships']['course_id']['field'] = 'course_id';
  /* Relationship: Course Access Record: User */

  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'elearning_course_access';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: Bulk operations: Course Access Record */

  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'elearning_course_access';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_delete_revision' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::elearning_deny_access_to_course' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_script_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::elearning_grant_access_to_course' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'show_all_tokens' => 1,
        'display_values' => array(
          '_all_' => '_all_',
        ),
      ),
    ),
    'action::views_bulk_operations_argument_selector_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'url' => '',
      ),
    ),
    'action::system_send_email_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::rules_test' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
  /* Field: User: Name */

  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'User';
  /* Field: Course: Label */

  $handler->display->display_options['fields']['label']['id'] = 'label';
  $handler->display->display_options['fields']['label']['table'] = 'elearning_course';
  $handler->display->display_options['fields']['label']['field'] = 'label';
  $handler->display->display_options['fields']['label']['relationship'] = 'course_id';
  $handler->display->display_options['fields']['label']['label'] = 'Course';
  /* Field: Course Access Record: Last granted */

  $handler->display->display_options['fields']['granted']['id'] = 'granted';
  $handler->display->display_options['fields']['granted']['table'] = 'elearning_course_access';
  $handler->display->display_options['fields']['granted']['field'] = 'granted';
  $handler->display->display_options['fields']['granted']['date_format'] = 'short';
  /* Field: Course Access Record: Status */

  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'elearning_course_access';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['type'] = 'unicode-yes-no';
  $handler->display->display_options['fields']['status']['not'] = 0;

  /* Display: Course access */

  $handler = $view->new_display('page', 'Course access', 'page');
  $handler->display->display_options['path'] = 'admin/elearning/course/access';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Course access';
  $handler->display->display_options['menu']['description'] = 'Overview of course access entities.';
  $handler->display->display_options['menu']['weight'] = '-1';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Grant access */

  $handler = $view->new_display('page', 'Grant access', 'course_grant_access');
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */

  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'All users have been granted access to this course.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Bulk operations: Course Access Record */

  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'elearning_course_access';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_delete_revision' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::elearning_deny_access_to_course' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_script_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::elearning_grant_access_to_course' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'show_all_tokens' => 1,
        'display_values' => array(
          '_all_' => '_all_',
        ),
      ),
    ),
    'action::views_bulk_operations_argument_selector_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'url' => '',
      ),
    ),
    'action::system_send_email_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::mimemail_send_email_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
  /* Field: User: Name */

  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Course: Course ID */

  $handler->display->display_options['arguments']['course_id']['id'] = 'course_id';
  $handler->display->display_options['arguments']['course_id']['table'] = 'elearning_course';
  $handler->display->display_options['arguments']['course_id']['field'] = 'course_id';
  $handler->display->display_options['arguments']['course_id']['relationship'] = 'course_id';
  $handler->display->display_options['arguments']['course_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['course_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['course_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['course_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['course_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['course_id']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['course_id']['validate']['type'] = 'numeric';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Course Access Record: Status */

  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'elearning_course_access';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '0';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Granted';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['path'] = 'admin/elearning/course/%/grant';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Grant access';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 1;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Deny access */

  $handler = $view->new_display('page', 'Deny access', 'course_deny_access');
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */

  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No users have been granted access to this course.';
  $handler->display->display_options['empty']['area']['format'] = 'plain_text';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Bulk operations: Course Access Record */

  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'elearning_course_access';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_delete_revision' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::elearning_deny_access_to_course' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_script_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::elearning_grant_access_to_course' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'show_all_tokens' => 1,
        'display_values' => array(
          '_all_' => '_all_',
        ),
      ),
    ),
    'action::views_bulk_operations_argument_selector_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'url' => '',
      ),
    ),
    'action::system_send_email_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::mimemail_send_email_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
  /* Field: User: Name */

  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Course: Course ID */

  $handler->display->display_options['arguments']['course_id']['id'] = 'course_id';
  $handler->display->display_options['arguments']['course_id']['table'] = 'elearning_course';
  $handler->display->display_options['arguments']['course_id']['field'] = 'course_id';
  $handler->display->display_options['arguments']['course_id']['relationship'] = 'course_id';
  $handler->display->display_options['arguments']['course_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['course_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['course_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['course_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['course_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['course_id']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['course_id']['validate']['type'] = 'numeric';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Course Access Record: Status */

  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'elearning_course_access';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Granted';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['path'] = 'admin/elearning/course/%/deny';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Deny access';
  $handler->display->display_options['menu']['weight'] = '1';
  $handler->display->display_options['menu']['context'] = 1;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  $views[$view->name] = $view;

  $view = new view();
  $view->name = 'elearning_question_results_per_exercise';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'elearning_question_result';
  $view->human_name = 'Question results';
  $view->core = 7;
  $view->api_version = '3.0';
  /* Edit this to true to make a default view disabled initially */

  $view->disabled = FALSE;

  /* Display: Master */

  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Question results';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer elearning results';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'counter' => 'counter',
    'question_1' => 'question_1',
    'given_answer' => 'given_answer',
    'feedback' => 'feedback',
    'correct' => 'correct',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'counter' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'question_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'given_answer' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'feedback' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'correct' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
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
  $handler->display->display_options['empty']['area']['content'] = 'There are no Question results available.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Relationship: Question result: Exercise result */

  $handler->display->display_options['relationships']['exercise_result']['id'] = 'exercise_result';
  $handler->display->display_options['relationships']['exercise_result']['table'] = 'elearning_question_result';
  $handler->display->display_options['relationships']['exercise_result']['field'] = 'exercise_result';
  /* Relationship: Question result: Question */

  $handler->display->display_options['relationships']['question_id']['id'] = 'question_id';
  $handler->display->display_options['relationships']['question_id']['table'] = 'elearning_question_result';
  $handler->display->display_options['relationships']['question_id']['field'] = 'question_id';
  /* Field: Global: View result counter */

  $handler->display->display_options['fields']['counter']['id'] = 'counter';
  $handler->display->display_options['fields']['counter']['table'] = 'views';
  $handler->display->display_options['fields']['counter']['field'] = 'counter';
  $handler->display->display_options['fields']['counter']['label'] = '';
  $handler->display->display_options['fields']['counter']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['counter']['counter_start'] = '1';
  /* Field: Question: Question type */

  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'elearning_question';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['relationship'] = 'question_id';
  /* Field: Field: Question */

  $handler->display->display_options['fields']['question']['id'] = 'question';
  $handler->display->display_options['fields']['question']['table'] = 'field_data_question';
  $handler->display->display_options['fields']['question']['field'] = 'question';
  $handler->display->display_options['fields']['question']['relationship'] = 'question_id';
  $handler->display->display_options['fields']['question']['type'] = 'question';
  /* Field: Question result: Given answer */

  $handler->display->display_options['fields']['given_answer']['id'] = 'given_answer';
  $handler->display->display_options['fields']['given_answer']['table'] = 'views_entity_elearning_question_result';
  $handler->display->display_options['fields']['given_answer']['field'] = 'given_answer';
  $handler->display->display_options['fields']['given_answer']['link_to_entity'] = 0;
  /* Field: Question result: Feedback */

  $handler->display->display_options['fields']['feedback']['id'] = 'feedback';
  $handler->display->display_options['fields']['feedback']['table'] = 'views_entity_elearning_question_result';
  $handler->display->display_options['fields']['feedback']['field'] = 'feedback';
  $handler->display->display_options['fields']['feedback']['link_to_entity'] = 0;
  /* Field: Question result: Correct */

  $handler->display->display_options['fields']['correct']['id'] = 'correct';
  $handler->display->display_options['fields']['correct']['table'] = 'elearning_question_result';
  $handler->display->display_options['fields']['correct']['field'] = 'correct';
  $handler->display->display_options['fields']['correct']['type'] = 'unicode-yes-no';
  $handler->display->display_options['fields']['correct']['not'] = 0;
  /* Sort criterion: Question result: Weight */

  $handler->display->display_options['sorts']['weight']['id'] = 'weight';
  $handler->display->display_options['sorts']['weight']['table'] = 'elearning_question_result';
  $handler->display->display_options['sorts']['weight']['field'] = 'weight';
  /* Contextual filter: Exercise result: Exercise result ID */

  $handler->display->display_options['arguments']['result_id']['id'] = 'result_id';
  $handler->display->display_options['arguments']['result_id']['table'] = 'elearning_exercise_result';
  $handler->display->display_options['arguments']['result_id']['field'] = 'result_id';
  $handler->display->display_options['arguments']['result_id']['relationship'] = 'exercise_result';
  $handler->display->display_options['arguments']['result_id']['default_action'] = 'access denied';
  $handler->display->display_options['arguments']['result_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['result_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['result_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['result_id']['summary_options']['items_per_page'] = '25';
  $translatables['elearning_question_results_per_exercise'] = array(
    t('Master'),
    t('Question results'),
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
    t('There are no Question results available.'),
    t('Exercise result'),
    t('Question'),
    t('Question type'),
    t('Given answer'),
    t('Feedback'),
    t('Correct'),
    t('All'),
  );
  $views[$view->name] = $view;

  return $views;
}
