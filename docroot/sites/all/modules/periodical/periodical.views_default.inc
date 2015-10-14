<?php

/**
 * Implements hook_views_default_views().
 */
function periodical_views_default_views() {
  $view = new view();
  $view->name = 'periodical_issues';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'taxonomy_term_data';
  $view->human_name = 'Periodical Issues';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Periodical Issues';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'periodical_issue_date' => 'periodical_issue_date',
    'periodical_issue_num' => 'periodical_issue_num',
  );
  $handler->display->display_options['style_options']['default'] = 'periodical_issue_date';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'periodical_issue_date' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
    ),
    'periodical_issue_num' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['order'] = 'desc';

  // only display vbo field if views_bulk_operations is enabled
  if (module_exists('views_bulk_operations')) {

    $handler->display->display_options['style_options']['columns']['views_bulk_operations'] = 'views_bulk_operations';
    $handler->display->display_options['style_options']['info']['views_bulk_operations'] = array(
      'align' => '',
      'separator' => '',
    );

    /* Field: Bulk operations: Taxonomy term */
    $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
    $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'taxonomy_term_data';
    $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
    $handler->display->display_options['fields']['views_bulk_operations']['hide_alter_empty'] = FALSE;
    $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
    $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
    $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
    $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_result'] = 1;
    $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
    $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
      'action::views_bulk_operations_delete_item' => array(
        'selected' => 1,
        'use_queue' => 0,
        'skip_confirmation' => 0,
        'override_label' => 0,
        'label' => '',
      ),
    );
  }

  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['name']['link_to_taxonomy'] = TRUE;
  /* Field: Taxonomy term: Issue Date */
  $handler->display->display_options['fields']['periodical_issue_date']['id'] = 'periodical_issue_date';
  $handler->display->display_options['fields']['periodical_issue_date']['table'] = 'field_data_periodical_issue_date';
  $handler->display->display_options['fields']['periodical_issue_date']['field'] = 'periodical_issue_date';
  $handler->display->display_options['fields']['periodical_issue_date']['hide_alter_empty'] = FALSE;
  /* Field: Taxonomy term: Issue Number */
  $handler->display->display_options['fields']['periodical_issue_num']['id'] = 'periodical_issue_num';
  $handler->display->display_options['fields']['periodical_issue_num']['table'] = 'field_data_periodical_issue_num';
  $handler->display->display_options['fields']['periodical_issue_num']['field'] = 'periodical_issue_num';
  $handler->display->display_options['fields']['periodical_issue_num']['hide_alter_empty'] = FALSE;
  /* Filter criterion: Taxonomy vocabulary: Machine name */
  $handler->display->display_options['filters']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['table'] = 'taxonomy_vocabulary';
  $handler->display->display_options['filters']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['machine_name']['expose']['operator_id'] = 'machine_name_op';
  $handler->display->display_options['filters']['machine_name']['expose']['label'] = 'Periodical';
  $handler->display->display_options['filters']['machine_name']['expose']['operator'] = 'machine_name_op';
  $handler->display->display_options['filters']['machine_name']['expose']['identifier'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['expose']['remember'] = TRUE;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'admin/structure/periodical';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Periodical Issues';
  $handler->display->display_options['menu']['description'] = 'Manage periodical issues';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['tab_options']['weight'] = '0';

  $views[$view->name] = $view;

  return $views;
}
