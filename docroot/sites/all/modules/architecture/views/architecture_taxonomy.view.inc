<?php

/**
 * @file
 * Defines a view for displaying taxonomies
 */

  if(module_exists('taxonomy')) {
  /*
   * View 'architecture_list_all_taxonomies'
   */$view = new view();
  $view->name = 'architecture_list_all_taxonomies';
  $view->description = 'Gives you the lay of the land for your taxonomies.';
  $view->tag = 'architecture';
  $view->base_table = 'taxonomy_term_data';
  $view->human_name = 'architecture: Taxonomies';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'architecture: List All Taxonomies';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Relationship: Taxonomy term: Content with term */
  $handler->display->display_options['relationships']['nid']['id'] = 'nid';
  $handler->display->display_options['relationships']['nid']['table'] = 'taxonomy_index';
  $handler->display->display_options['relationships']['nid']['field'] = 'nid';
  /* Field: Taxonomy vocabulary: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_vocabulary';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = 'Vocabulary';
  /* Field: Taxonomy vocabulary: Description */
  $handler->display->display_options['fields']['description']['id'] = 'description';
  $handler->display->display_options['fields']['description']['table'] = 'taxonomy_vocabulary';
  $handler->display->display_options['fields']['description']['field'] = 'description';
  $handler->display->display_options['fields']['description']['label'] = 'Vocabulary Description';
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['label'] = 'Term';
  /* Field: Taxonomy term: Term description */
  $handler->display->display_options['fields']['description_1']['id'] = 'description_1';
  $handler->display->display_options['fields']['description_1']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['description_1']['field'] = 'description';
  /* Field: Taxonomy Vocabulary: Appears on (nodetypes) */
  $handler->display->display_options['fields']['nodetypes']['id'] = 'nodetypes';
  $handler->display->display_options['fields']['nodetypes']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['nodetypes']['field'] = 'nodetypes';
  /* Field: COUNT(DISTINCT Content: Nid) */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['relationship'] = 'nid';
  $handler->display->display_options['fields']['nid']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['nid']['label'] = '# of nodes using term';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'name',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
    1 => array(
      'field' => 'description',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
    2 => array(
      'field' => 'nodetypes',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'description' => 'description',
    'name_1' => 'name_1',
    'description_1' => 'description_1',
    'nid' => 'nid',
    'type' => 'type',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'description' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'description_1' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'type' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Taxonomy vocabulary: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_vocabulary';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['exclude'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['text'] = '<h2>[name]</h2>';
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  /* Field: Taxonomy vocabulary: Description */
  $handler->display->display_options['fields']['description']['id'] = 'description';
  $handler->display->display_options['fields']['description']['table'] = 'taxonomy_vocabulary';
  $handler->display->display_options['fields']['description']['field'] = 'description';
  $handler->display->display_options['fields']['description']['label'] = '';
  $handler->display->display_options['fields']['description']['exclude'] = TRUE;
  $handler->display->display_options['fields']['description']['element_label_colon'] = FALSE;
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['label'] = 'Term';
  /* Field: Taxonomy term: Term description */
  $handler->display->display_options['fields']['description_1']['id'] = 'description_1';
  $handler->display->display_options['fields']['description_1']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['description_1']['field'] = 'description';
  /* Field: Taxonomy Vocabulary: Appears on (nodetypes) */
  $handler->display->display_options['fields']['nodetypes']['id'] = 'nodetypes';
  $handler->display->display_options['fields']['nodetypes']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['nodetypes']['field'] = 'nodetypes';
  $handler->display->display_options['fields']['nodetypes']['label'] = 'Appears on';
  $handler->display->display_options['fields']['nodetypes']['exclude'] = TRUE;
  /* Field: COUNT(DISTINCT Content: Nid) */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['relationship'] = 'nid';
  $handler->display->display_options['fields']['nid']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['nid']['label'] = '# of nodes using term';
  /* Menu settings */
  $handler->display->display_options['path'] = 'admin/reports/architecture/taxonomies';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Taxonomies';
  $handler->display->display_options['menu']['description'] = 'List of taxonomies';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['weight'] = '0';

  if(module_exists('views_data_export')) {
    // Add export link to Page view if relevant.
    /* Header: Global: Text area */
    $handler->display->display_options['header']['area']['id'] = 'area';
    $handler->display->display_options['header']['area']['table'] = 'views';
    $handler->display->display_options['header']['area']['field'] = 'area';
    $handler->display->display_options['header']['area']['label'] = 'Export';
    $handler->display->display_options['header']['area']['content'] = '<a href="export" title="Export the result of this view">Export to CSV</a>';
    $handler->display->display_options['header']['area']['format'] = 'html';
    /* Display: Data export */
    $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
    $handler->display->display_options['style_options']['filename'] = variable_get('site_name') . '-' . date('YmdAHis') . '-taxonomies.csv';
    /* Menu settings */
    $handler->display->display_options['path'] = 'admin/reports/architecture/taxonomies/export';
    $handler->display->display_options['menu']['type'] = 'tab';
    $handler->display->display_options['menu']['title'] = 'Taxonomies export';
    $handler->display->display_options['menu']['description'] = 'Export list of taxonomies';
    $handler->display->display_options['menu']['weight'] = '0';
    $handler->display->display_options['menu']['context'] = 0;
    $handler->display->display_options['tab_options']['weight'] = '0';
  }
}