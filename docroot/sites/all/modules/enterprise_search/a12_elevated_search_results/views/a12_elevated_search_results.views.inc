<?php
/**
 * Views hooks for Enterprise search module.
 */

/**
 * Implements hook_views_data().
 */
function a12_elevated_search_results_views_data() {
  $data = array();

  $data['solr_best_bets']['table']['group'] = t('Best Bets');
  $data['solr_best_bets']['table']['base'] = array(
    'field' => 'content_id', // This is the identifier field for the view.
    'title' => t('Best Bets'),
    'help' => t('Best bets table containing terms associated with node.'),
  );
  $data['solr_best_bets']['content_id'] = array(
    'title' => t('Content Id'),
    'help' => t('Node Id'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => FALSE, // This is use by the table display plugin.
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
  $data['solr_best_bets']['query_text'] = array(
    'title' => t('Term'),
    'help' => t("Best bets term for the node."),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => FALSE,
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

  $data['solr_best_bets']['status'] = array(
    'title' => t('Status'),
    'help' => t("Best bet term status."),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => FALSE,
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

  if (module_exists('views_bulk_operations')) {
    $data['solr_best_bets']['views_bulk_operations'] = array(
      'title' => t("Elevated Search Operations"),
      'group' => t('Bulk operations'),
      'help' => t('Provide a checkbox to select the row for bulk operations.'),
      'real field' => 'content_id',
      'field' => array(
        'handler' => 'views_bulk_operations_handler_field_operations',
        'click sortable' => FALSE,
      ),
    );
  }

  return $data;
}

/**
 * Implements hook_views_pre_render().
 */
function a12_elevated_search_results_views_pre_render(&$view) {
  foreach ($view->result as $row) {
    if (isset($row->content_id)) {
      $nid = explode(':', $row->content_id);
      $row->content_id = $nid[2];
    }
    if (isset($row->solr_best_bets_status)) {
      switch ($row->solr_best_bets_status) {
        case '1':
          $row->solr_best_bets_status = 'Inactive';
          break;
        case '2':
          $row->solr_best_bets_status = 'Active';
          break;
        case '3':
          $row->solr_best_bets_status = 'Deleted';
          break;
        case '4':
          $row->solr_best_bets_status = 'Undefined';
          break;
      }

    }
  }
}

/**
 * Implements hook_views_default_views().
 */
function a12_elevated_search_results_views_default_views() {
  $view = new view();
  $view->name = 'elevated-search-results';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'solr_best_bets';
  $view->human_name = 'Elevated Search Results';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Elevated Search Results';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer a12 connector';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Search';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No results found.';
  $handler->display->display_options['empty']['area']['format'] = 'full_html';
  /* Field: Bulk operations: Best Bets */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'solr_best_bets';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = 'Operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '1';
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
    'action::elevated_search_delete' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 1,
      'override_label' => 0,
      'label' => '',
    ),
    'action::elevated_search_deploy' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 1,
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
  );
  /* Field: Best Bets: Term */
  $handler->display->display_options['fields']['query_text']['id'] = 'query_text';
  $handler->display->display_options['fields']['query_text']['table'] = 'solr_best_bets';
  $handler->display->display_options['fields']['query_text']['field'] = 'query_text';
  /* Field: Best Bets: Content Id */
  $handler->display->display_options['fields']['content_id']['id'] = 'content_id';
  $handler->display->display_options['fields']['content_id']['table'] = 'solr_best_bets';
  $handler->display->display_options['fields']['content_id']['field'] = 'content_id';
  $handler->display->display_options['fields']['content_id']['label'] = 'Node Id';
  $handler->display->display_options['fields']['content_id']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['content_id']['alter']['path'] = 'node/[content_id]/solr_best_bets';
  /* Field: Best Bets: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'solr_best_bets';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['alter']['trim_whitespace'] = TRUE;
  /* Filter criterion: Best Bets: Term */
  $handler->display->display_options['filters']['query_text']['id'] = 'query_text';
  $handler->display->display_options['filters']['query_text']['table'] = 'solr_best_bets';
  $handler->display->display_options['filters']['query_text']['field'] = 'query_text';
  $handler->display->display_options['filters']['query_text']['operator'] = 'contains';
  $handler->display->display_options['filters']['query_text']['exposed'] = TRUE;
  $handler->display->display_options['filters']['query_text']['expose']['operator_id'] = 'query_text_op';
  $handler->display->display_options['filters']['query_text']['expose']['operator'] = 'query_text_op';
  $handler->display->display_options['filters']['query_text']['expose']['identifier'] = 'query_text';
  $handler->display->display_options['filters']['query_text']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  /* Filter criterion: Best Bets: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'solr_best_bets';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['is_grouped'] = TRUE;
  $handler->display->display_options['filters']['status']['group_info']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['group_info']['group_items'] = array(
    1 => array(
      'title' => 'Active',
      'operator' => '=',
      'value' => '2',
    ),
    2 => array(
      'title' => 'Inactive',
      'operator' => '=',
      'value' => '1',
    ),
    3 => array(
      'title' => 'Deleted',
      'operator' => '=',
      'value' => '3',
    ),
    4 => array(
      'title' => 'Undefined',
      'operator' => '=',
      'value' => '4',
    ),
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/elevated-search-results';


  $views[$view->name] = $view;
  return $views;
}
