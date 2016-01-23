<?php
/**
 * @file
 * abtest_dashboard.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function abtest_dashboard_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'abtest_details';
  $view->description = '';
  $view->tag = 'abtest';
  $view->base_table = 'abtest';
  $view->human_name = 'A/B test details';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'A/B test';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access abtest dashboard';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'flipped';
  $handler->display->display_options['style_options']['columns'] = array(
    'counter' => 'counter',
    'value' => 'value',
    'participants' => 'participants',
    'conversions' => 'conversions',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'counter' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'value' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'participants' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'conversions' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['flipped_table_header_first_field'] = 1;
  /* Relationship: A/B test: A/B test alternative */
  $handler->display->display_options['relationships']['alternatives']['id'] = 'alternatives';
  $handler->display->display_options['relationships']['alternatives']['table'] = 'abtest';
  $handler->display->display_options['relationships']['alternatives']['field'] = 'alternatives';
  $handler->display->display_options['relationships']['alternatives']['required'] = TRUE;
  /* Field: Global: View result counter */
  $handler->display->display_options['fields']['counter']['id'] = 'counter';
  $handler->display->display_options['fields']['counter']['table'] = 'views';
  $handler->display->display_options['fields']['counter']['field'] = 'counter';
  $handler->display->display_options['fields']['counter']['label'] = '';
  $handler->display->display_options['fields']['counter']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['counter']['alter']['text'] = 'Group [counter]';
  $handler->display->display_options['fields']['counter']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['counter']['counter_start'] = '1';
  /* Field: A/B test alternative: Value */
  $handler->display->display_options['fields']['value']['id'] = 'value';
  $handler->display->display_options['fields']['value']['table'] = 'abtest_alternatives';
  $handler->display->display_options['fields']['value']['field'] = 'value';
  $handler->display->display_options['fields']['value']['relationship'] = 'alternatives';
  /* Field: A/B test alternative: Participants / Total Participants */
  $handler->display->display_options['fields']['total_participants_percent']['id'] = 'total_participants_percent';
  $handler->display->display_options['fields']['total_participants_percent']['table'] = 'abtest_alternatives';
  $handler->display->display_options['fields']['total_participants_percent']['field'] = 'total_participants_percent';
  $handler->display->display_options['fields']['total_participants_percent']['relationship'] = 'alternatives';
  $handler->display->display_options['fields']['total_participants_percent']['label'] = '';
  $handler->display->display_options['fields']['total_participants_percent']['exclude'] = TRUE;
  $handler->display->display_options['fields']['total_participants_percent']['element_label_colon'] = FALSE;
  /* Field: A/B test alternative: Participants */
  $handler->display->display_options['fields']['participants']['id'] = 'participants';
  $handler->display->display_options['fields']['participants']['table'] = 'abtest_alternatives';
  $handler->display->display_options['fields']['participants']['field'] = 'participants';
  $handler->display->display_options['fields']['participants']['relationship'] = 'alternatives';
  $handler->display->display_options['fields']['participants']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['participants']['alter']['text'] = '[participants] ([total_participants_percent])';
  /* Field: A/B test alternative: Conversions */
  $handler->display->display_options['fields']['conversions']['id'] = 'conversions';
  $handler->display->display_options['fields']['conversions']['table'] = 'abtest_alternatives';
  $handler->display->display_options['fields']['conversions']['field'] = 'conversions';
  $handler->display->display_options['fields']['conversions']['relationship'] = 'alternatives';
  /* Field: A/B test alternative: Conversions / Participants */
  $handler->display->display_options['fields']['conversions_percent']['id'] = 'conversions_percent';
  $handler->display->display_options['fields']['conversions_percent']['table'] = 'abtest_alternatives';
  $handler->display->display_options['fields']['conversions_percent']['field'] = 'conversions_percent';
  $handler->display->display_options['fields']['conversions_percent']['relationship'] = 'alternatives';
  /* Contextual filter: A/B test: Test ID */
  $handler->display->display_options['arguments']['test_id']['id'] = 'test_id';
  $handler->display->display_options['arguments']['test_id']['table'] = 'abtest';
  $handler->display->display_options['arguments']['test_id']['field'] = 'test_id';
  $handler->display->display_options['arguments']['test_id']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['test_id']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['test_id']['title'] = 'A/B test: %1';
  $handler->display->display_options['arguments']['test_id']['breadcrumb_enable'] = TRUE;
  $handler->display->display_options['arguments']['test_id']['breadcrumb'] = '%1';
  $handler->display->display_options['arguments']['test_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['test_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['test_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['test_id']['summary_options']['items_per_page'] = '25';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'abtest/%/view';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'View';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'A/B test';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $translatables['abtest_details'] = array(
    t('Master'),
    t('A/B test'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('alternative'),
    t('Group [counter]'),
    t('Value'),
    t('Participants'),
    t('[participants] ([total_participants_percent])'),
    t('.'),
    t(','),
    t('Conversions'),
    t('Conversions / Participants'),
    t('All'),
    t('A/B test: %1'),
    t('%1'),
    t('Page'),
  );
  $export['abtest_details'] = $view;

  $view = new view();
  $view->name = 'abtest_listing';
  $view->description = '';
  $view->tag = 'abtest';
  $view->base_table = 'abtest';
  $view->human_name = 'A/B test listing';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'A/B tests';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access abtest dashboard';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'test_name' => 'test_name',
    'conversion_name' => 'conversion_name',
    'created' => 'created',
  );
  $handler->display->display_options['style_options']['default'] = 'test_name';
  $handler->display->display_options['style_options']['info'] = array(
    'test_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'conversion_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: A/B test: Test ID */
  $handler->display->display_options['fields']['test_id']['id'] = 'test_id';
  $handler->display->display_options['fields']['test_id']['table'] = 'abtest';
  $handler->display->display_options['fields']['test_id']['field'] = 'test_id';
  $handler->display->display_options['fields']['test_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['test_id']['separator'] = '';
  /* Field: A/B test: Name */
  $handler->display->display_options['fields']['test_name']['id'] = 'test_name';
  $handler->display->display_options['fields']['test_name']['table'] = 'abtest';
  $handler->display->display_options['fields']['test_name']['field'] = 'test_name';
  $handler->display->display_options['fields']['test_name']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['test_name']['alter']['path'] = 'abtest/[test_id]/view';
  /* Field: A/B test: Total participants */
  $handler->display->display_options['fields']['total_participants']['id'] = 'total_participants';
  $handler->display->display_options['fields']['total_participants']['table'] = 'abtest';
  $handler->display->display_options['fields']['total_participants']['field'] = 'total_participants';
  $handler->display->display_options['fields']['total_participants']['label'] = 'Participants';
  /* Field: A/B test: Total conversions */
  $handler->display->display_options['fields']['total_conversions']['id'] = 'total_conversions';
  $handler->display->display_options['fields']['total_conversions']['table'] = 'abtest';
  $handler->display->display_options['fields']['total_conversions']['field'] = 'total_conversions';
  $handler->display->display_options['fields']['total_conversions']['label'] = 'Conversions';
  /* Field: A/B test: Status */
  $handler->display->display_options['fields']['conversion_name']['id'] = 'conversion_name';
  $handler->display->display_options['fields']['conversion_name']['table'] = 'abtest';
  $handler->display->display_options['fields']['conversion_name']['field'] = 'conversion_name';
  $handler->display->display_options['fields']['conversion_name']['type'] = 'running-notrunning';
  $handler->display->display_options['fields']['conversion_name']['not'] = 0;
  /* Field: A/B test: Created date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'abtest';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Created';
  $handler->display->display_options['fields']['created']['date_format'] = 'custom';
  $handler->display->display_options['fields']['created']['custom_date_format'] = 'Y-m-d';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'abtest/dashboard';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'A/B tests';
  $translatables['abtest_listing'] = array(
    t('Master'),
    t('A/B tests'),
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
    t('Test ID'),
    t('.'),
    t('Name'),
    t('Participants'),
    t('Conversions'),
    t('Status'),
    t('Created'),
    t('Page'),
  );
  $export['abtest_listing'] = $view;

  return $export;
}
