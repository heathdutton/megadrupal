<?php
/**
 * @file
 * cp_search.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function cp_search_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'cp_browse';
  $view->description = 'A faceted search page for Course planner items (excluding lessons)';
  $view->tag = 'courseplanner';
  $view->base_table = 'search_api_index_cp_node_index';
  $view->human_name = 'Browse course planner items';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['link_display'] = 'custom_url';
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Search';
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Show all';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'nid' => 'nid',
    'body_value' => 'body_value',
    'title' => 'title',
    'type' => 'title',
    'cp_canonical_course' => 'cp_canonical_course',
    'cp_keywords' => 'cp_keywords',
    'author' => 'author',
    'changed' => 'changed',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'nid' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'body_value' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'align' => '',
      'separator' => '<br/>',
      'empty_column' => 0,
    ),
    'type' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cp_canonical_course' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cp_keywords' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'author' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'changed' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: Indexed Content: Node ID */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nid']['separator'] = '';
  $handler->display->display_options['fields']['nid']['link_to_entity'] = 0;
  /* Field: The main body text: Text (indexed) */
  $handler->display->display_options['fields']['body_value']['id'] = 'body_value';
  $handler->display->display_options['fields']['body_value']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['body_value']['field'] = 'body_value';
  $handler->display->display_options['fields']['body_value']['label'] = '';
  $handler->display->display_options['fields']['body_value']['exclude'] = TRUE;
  $handler->display->display_options['fields']['body_value']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body_value']['link_to_entity'] = 0;
  /* Field: Indexed Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'node/[nid]';
  $handler->display->display_options['fields']['title']['alter']['alt'] = '[body_value]';
  $handler->display->display_options['fields']['title']['link_to_entity'] = 0;
  /* Field: Indexed Content: Content type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['label'] = 'Type';
  $handler->display->display_options['fields']['type']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['type']['alter']['text'] = '([type])';
  $handler->display->display_options['fields']['type']['link_to_entity'] = 0;
  $handler->display->display_options['fields']['type']['format_name'] = 1;
  /* Field: Indexed Content: Canonical course */
  $handler->display->display_options['fields']['cp_canonical_course']['id'] = 'cp_canonical_course';
  $handler->display->display_options['fields']['cp_canonical_course']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['cp_canonical_course']['field'] = 'cp_canonical_course';
  $handler->display->display_options['fields']['cp_canonical_course']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['cp_canonical_course']['view_mode'] = 'full';
  /* Field: Indexed Content: Keywords */
  $handler->display->display_options['fields']['cp_keywords']['id'] = 'cp_keywords';
  $handler->display->display_options['fields']['cp_keywords']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['cp_keywords']['field'] = 'cp_keywords';
  $handler->display->display_options['fields']['cp_keywords']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['cp_keywords']['view_mode'] = 'full';
  /* Field: Indexed Content: Author */
  $handler->display->display_options['fields']['author']['id'] = 'author';
  $handler->display->display_options['fields']['author']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['author']['field'] = 'author';
  $handler->display->display_options['fields']['author']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['author']['view_mode'] = 'full';
  /* Field: Indexed Content: Date changed */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = 'Last updated';
  $handler->display->display_options['fields']['changed']['date_format'] = 'short';
  $handler->display->display_options['fields']['changed']['link_to_entity'] = 0;
  /* Sort criterion: Search: Relevance */
  $handler->display->display_options['sorts']['search_api_relevance']['id'] = 'search_api_relevance';
  $handler->display->display_options['sorts']['search_api_relevance']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['sorts']['search_api_relevance']['field'] = 'search_api_relevance';
  $handler->display->display_options['sorts']['search_api_relevance']['order'] = 'DESC';
  /* Sort criterion: Indexed Content: Date changed */
  $handler->display->display_options['sorts']['changed']['id'] = 'changed';
  $handler->display->display_options['sorts']['changed']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['sorts']['changed']['field'] = 'changed';
  $handler->display->display_options['sorts']['changed']['order'] = 'DESC';
  /* Filter criterion: Indexed Content: Content type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'cp_offering' => 'cp_offering',
    'cp_outline' => 'cp_outline',
    'cp_section' => 'cp_section',
    'cp_resource' => 'cp_resource',
  );
  /* Filter criterion: Search: Fulltext search */
  $handler->display->display_options['filters']['search_api_views_fulltext']['id'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['filters']['search_api_views_fulltext']['field'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['exposed'] = TRUE;
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator_id'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['label'] = 'Search';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['identifier'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );

  /* Display: Resources (no book references) */
  $handler = $view->new_display('panel_pane', 'Resources (no book references)', 'panel_pane_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Browse resources';
  $handler->display->display_options['display_description'] = 'Search results for Course planner resources that aren\'t book references';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['link_display'] = FALSE;
  $handler->display->display_options['link_display'] = 'custom_url';
  $handler->display->display_options['link_url'] = 'browse/resources';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'nid' => 'nid',
    'body_value' => 'body_value',
    'title' => 'title',
    'cp_resource_type' => 'title',
    'cp_resource_source' => 'cp_resource_source',
    'cp_resource_reference' => 'cp_resource_reference',
    'cp_resource_file' => 'cp_resource_reference',
    'cp_resource_link' => 'cp_resource_reference',
    'cp_canonical_course' => 'cp_canonical_course',
    'cp_keywords' => 'cp_keywords',
    'changed' => 'changed',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'nid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'body_value' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'align' => '',
      'separator' => '<br/>',
      'empty_column' => 0,
    ),
    'cp_resource_type' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cp_resource_source' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cp_resource_reference' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cp_resource_file' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cp_resource_link' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cp_canonical_course' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cp_keywords' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'changed' => array(
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
  $handler->display->display_options['defaults']['footer'] = FALSE;
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['area']['id'] = 'area';
  $handler->display->display_options['footer']['area']['table'] = 'views';
  $handler->display->display_options['footer']['area']['field'] = 'area';
  $handler->display->display_options['footer']['area']['content'] = 'Book references are excluded from this search. If you want to search book references too, go to the tab "All resources".';
  $handler->display->display_options['footer']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Indexed Content: Node ID */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nid']['separator'] = '';
  $handler->display->display_options['fields']['nid']['link_to_entity'] = 0;
  /* Field: The main body text: Text (indexed) */
  $handler->display->display_options['fields']['body_value']['id'] = 'body_value';
  $handler->display->display_options['fields']['body_value']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['body_value']['field'] = 'body_value';
  $handler->display->display_options['fields']['body_value']['label'] = '';
  $handler->display->display_options['fields']['body_value']['exclude'] = TRUE;
  $handler->display->display_options['fields']['body_value']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body_value']['link_to_entity'] = 0;
  /* Field: Indexed Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'node/[nid]';
  $handler->display->display_options['fields']['title']['alter']['alt'] = '[body_value]';
  $handler->display->display_options['fields']['title']['link_to_entity'] = 0;
  /* Field: Indexed Content: Type of resource */
  $handler->display->display_options['fields']['cp_resource_type']['id'] = 'cp_resource_type';
  $handler->display->display_options['fields']['cp_resource_type']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['cp_resource_type']['field'] = 'cp_resource_type';
  $handler->display->display_options['fields']['cp_resource_type']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['cp_resource_type']['alter']['text'] = '([cp_resource_type])';
  /* Field: Indexed Content: Source */
  $handler->display->display_options['fields']['cp_resource_source']['id'] = 'cp_resource_source';
  $handler->display->display_options['fields']['cp_resource_source']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['cp_resource_source']['field'] = 'cp_resource_source';
  $handler->display->display_options['fields']['cp_resource_source']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['cp_resource_source']['view_mode'] = 'full';
  /* Field: Indexed Content: Reference */
  $handler->display->display_options['fields']['cp_resource_reference']['id'] = 'cp_resource_reference';
  $handler->display->display_options['fields']['cp_resource_reference']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['cp_resource_reference']['field'] = 'cp_resource_reference';
  /* Field: Indexed Content: File */
  $handler->display->display_options['fields']['cp_resource_file']['id'] = 'cp_resource_file';
  $handler->display->display_options['fields']['cp_resource_file']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['cp_resource_file']['field'] = 'cp_resource_file';
  $handler->display->display_options['fields']['cp_resource_file']['label'] = '';
  $handler->display->display_options['fields']['cp_resource_file']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['cp_resource_file']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['cp_resource_file']['delta_offset'] = '0';
  /* Field: Indexed Content: Link */
  $handler->display->display_options['fields']['cp_resource_link']['id'] = 'cp_resource_link';
  $handler->display->display_options['fields']['cp_resource_link']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['cp_resource_link']['field'] = 'cp_resource_link';
  $handler->display->display_options['fields']['cp_resource_link']['label'] = '';
  $handler->display->display_options['fields']['cp_resource_link']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['cp_resource_link']['click_sort_column'] = 'url';
  /* Field: Indexed Content: Canonical course */
  $handler->display->display_options['fields']['cp_canonical_course']['id'] = 'cp_canonical_course';
  $handler->display->display_options['fields']['cp_canonical_course']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['cp_canonical_course']['field'] = 'cp_canonical_course';
  $handler->display->display_options['fields']['cp_canonical_course']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['cp_canonical_course']['view_mode'] = 'full';
  /* Field: Indexed Content: Keywords */
  $handler->display->display_options['fields']['cp_keywords']['id'] = 'cp_keywords';
  $handler->display->display_options['fields']['cp_keywords']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['cp_keywords']['field'] = 'cp_keywords';
  $handler->display->display_options['fields']['cp_keywords']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['cp_keywords']['view_mode'] = 'full';
  /* Field: Indexed Content: Date changed */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = 'Last updated';
  $handler->display->display_options['fields']['changed']['date_format'] = 'short';
  $handler->display->display_options['fields']['changed']['link_to_entity'] = 0;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Indexed Content: Content type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'cp_resource' => 'cp_resource',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Indexed Content: Type of resource */
  $handler->display->display_options['filters']['cp_resource_type']['id'] = 'cp_resource_type';
  $handler->display->display_options['filters']['cp_resource_type']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['filters']['cp_resource_type']['field'] = 'cp_resource_type';
  $handler->display->display_options['filters']['cp_resource_type']['operator'] = '<>';
  $handler->display->display_options['filters']['cp_resource_type']['value'] = array(
    'book' => 'book',
  );
  $handler->display->display_options['filters']['cp_resource_type']['group'] = 1;
  /* Filter criterion: Search: Fulltext search */
  $handler->display->display_options['filters']['search_api_views_fulltext']['id'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['filters']['search_api_views_fulltext']['field'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['group'] = 1;
  $handler->display->display_options['filters']['search_api_views_fulltext']['exposed'] = TRUE;
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator_id'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['label'] = 'Search';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['identifier'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['pane_title'] = 'Resource browser (no books)';
  $handler->display->display_options['pane_description'] = 'Search results for Course planner resources.';
  $handler->display->display_options['pane_category']['name'] = 'Course planner';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $handler->display->display_options['allow']['use_pager'] = 0;
  $handler->display->display_options['allow']['items_per_page'] = 0;
  $handler->display->display_options['allow']['offset'] = 0;
  $handler->display->display_options['allow']['link_to_view'] = 0;
  $handler->display->display_options['allow']['more_link'] = 0;
  $handler->display->display_options['allow']['path_override'] = 0;
  $handler->display->display_options['allow']['title_override'] = 'title_override';
  $handler->display->display_options['allow']['exposed_form'] = 0;
  $handler->display->display_options['allow']['fields_override'] = 0;
  $handler->display->display_options['inherit_panels_path'] = '1';

  /* Display: Resources */
  $handler = $view->new_display('panel_pane', 'Resources', 'cp_browse_resources');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Browse all resources';
  $handler->display->display_options['display_description'] = 'Search results for all Course planner resources';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['link_display'] = FALSE;
  $handler->display->display_options['link_display'] = 'custom_url';
  $handler->display->display_options['link_url'] = 'browse/resources-all';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'nid' => 'nid',
    'body_value' => 'body_value',
    'title' => 'title',
    'cp_resource_type' => 'title',
    'cp_resource_source' => 'cp_resource_source',
    'cp_resource_reference' => 'cp_resource_reference',
    'cp_resource_file' => 'cp_resource_reference',
    'cp_resource_link' => 'cp_resource_reference',
    'cp_canonical_course' => 'cp_canonical_course',
    'cp_keywords' => 'cp_keywords',
    'changed' => 'changed',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'nid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'body_value' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'align' => '',
      'separator' => '<br/>',
      'empty_column' => 0,
    ),
    'cp_resource_type' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cp_resource_source' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cp_resource_reference' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cp_resource_file' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cp_resource_link' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cp_canonical_course' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cp_keywords' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'changed' => array(
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
  /* Field: Indexed Content: Node ID */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nid']['separator'] = '';
  $handler->display->display_options['fields']['nid']['link_to_entity'] = 0;
  /* Field: The main body text: Text (indexed) */
  $handler->display->display_options['fields']['body_value']['id'] = 'body_value';
  $handler->display->display_options['fields']['body_value']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['body_value']['field'] = 'body_value';
  $handler->display->display_options['fields']['body_value']['label'] = '';
  $handler->display->display_options['fields']['body_value']['exclude'] = TRUE;
  $handler->display->display_options['fields']['body_value']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body_value']['link_to_entity'] = 0;
  /* Field: Indexed Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'node/[nid]';
  $handler->display->display_options['fields']['title']['alter']['alt'] = '[body_value]';
  $handler->display->display_options['fields']['title']['link_to_entity'] = 0;
  /* Field: Indexed Content: Type of resource */
  $handler->display->display_options['fields']['cp_resource_type']['id'] = 'cp_resource_type';
  $handler->display->display_options['fields']['cp_resource_type']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['cp_resource_type']['field'] = 'cp_resource_type';
  $handler->display->display_options['fields']['cp_resource_type']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['cp_resource_type']['alter']['text'] = '([cp_resource_type])';
  /* Field: Indexed Content: Source */
  $handler->display->display_options['fields']['cp_resource_source']['id'] = 'cp_resource_source';
  $handler->display->display_options['fields']['cp_resource_source']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['cp_resource_source']['field'] = 'cp_resource_source';
  $handler->display->display_options['fields']['cp_resource_source']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['cp_resource_source']['view_mode'] = 'full';
  /* Field: Indexed Content: Reference */
  $handler->display->display_options['fields']['cp_resource_reference']['id'] = 'cp_resource_reference';
  $handler->display->display_options['fields']['cp_resource_reference']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['cp_resource_reference']['field'] = 'cp_resource_reference';
  /* Field: Indexed Content: File */
  $handler->display->display_options['fields']['cp_resource_file']['id'] = 'cp_resource_file';
  $handler->display->display_options['fields']['cp_resource_file']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['cp_resource_file']['field'] = 'cp_resource_file';
  $handler->display->display_options['fields']['cp_resource_file']['label'] = '';
  $handler->display->display_options['fields']['cp_resource_file']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['cp_resource_file']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['cp_resource_file']['delta_offset'] = '0';
  /* Field: Indexed Content: Link */
  $handler->display->display_options['fields']['cp_resource_link']['id'] = 'cp_resource_link';
  $handler->display->display_options['fields']['cp_resource_link']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['cp_resource_link']['field'] = 'cp_resource_link';
  $handler->display->display_options['fields']['cp_resource_link']['label'] = '';
  $handler->display->display_options['fields']['cp_resource_link']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['cp_resource_link']['click_sort_column'] = 'url';
  /* Field: Indexed Content: Canonical course */
  $handler->display->display_options['fields']['cp_canonical_course']['id'] = 'cp_canonical_course';
  $handler->display->display_options['fields']['cp_canonical_course']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['cp_canonical_course']['field'] = 'cp_canonical_course';
  $handler->display->display_options['fields']['cp_canonical_course']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['cp_canonical_course']['view_mode'] = 'full';
  /* Field: Indexed Content: Keywords */
  $handler->display->display_options['fields']['cp_keywords']['id'] = 'cp_keywords';
  $handler->display->display_options['fields']['cp_keywords']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['cp_keywords']['field'] = 'cp_keywords';
  $handler->display->display_options['fields']['cp_keywords']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['cp_keywords']['view_mode'] = 'full';
  /* Field: Indexed Content: Date changed */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = 'Last updated';
  $handler->display->display_options['fields']['changed']['date_format'] = 'short';
  $handler->display->display_options['fields']['changed']['link_to_entity'] = 0;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Indexed Content: Content type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'cp_resource' => 'cp_resource',
  );
  /* Filter criterion: Search: Fulltext search */
  $handler->display->display_options['filters']['search_api_views_fulltext']['id'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['filters']['search_api_views_fulltext']['field'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['exposed'] = TRUE;
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator_id'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['label'] = 'Search';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['identifier'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['pane_title'] = 'Resource browser (all)';
  $handler->display->display_options['pane_description'] = 'Search results for Course planner resources, including book references.';
  $handler->display->display_options['pane_category']['name'] = 'Course planner';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $handler->display->display_options['allow']['use_pager'] = 0;
  $handler->display->display_options['allow']['items_per_page'] = 0;
  $handler->display->display_options['allow']['offset'] = 0;
  $handler->display->display_options['allow']['link_to_view'] = 0;
  $handler->display->display_options['allow']['more_link'] = 0;
  $handler->display->display_options['allow']['path_override'] = 0;
  $handler->display->display_options['allow']['title_override'] = 'title_override';
  $handler->display->display_options['allow']['exposed_form'] = 0;
  $handler->display->display_options['allow']['fields_override'] = 0;
  $handler->display->display_options['inherit_panels_path'] = '1';

  /* Display: All items */
  $handler = $view->new_display('panel_pane', 'All items', 'browse_pane');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Browse Course planner items';
  $handler->display->display_options['display_description'] = 'Search results for all Course planner items';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pane_title'] = 'Item browser';
  $handler->display->display_options['pane_description'] = 'Search results for all Course planner items.';
  $handler->display->display_options['pane_category']['name'] = 'Course planner';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $handler->display->display_options['allow']['use_pager'] = 0;
  $handler->display->display_options['allow']['items_per_page'] = 0;
  $handler->display->display_options['allow']['offset'] = 0;
  $handler->display->display_options['allow']['link_to_view'] = 0;
  $handler->display->display_options['allow']['more_link'] = 0;
  $handler->display->display_options['allow']['path_override'] = 0;
  $handler->display->display_options['allow']['title_override'] = 'title_override';
  $handler->display->display_options['allow']['exposed_form'] = 0;
  $handler->display->display_options['allow']['fields_override'] = 0;
  $handler->display->display_options['inherit_panels_path'] = '1';

  /* Display: Search block (not used) */
  $handler = $view->new_display('panel_pane', 'Search block (not used)', 'cp_search_block');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Browse resources';
  $handler->display->display_options['display_description'] = 'This display is only used for creating a block with exposed filters. It is not used in itself.';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['link_display'] = FALSE;
  $handler->display->display_options['link_display'] = 'custom_url';
  $handler->display->display_options['link_url'] = 'browse/resources';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'nid' => 'nid',
    'body_value' => 'body_value',
    'title' => 'title',
    'cp_resource_type' => 'title',
    'cp_resource_source' => 'cp_resource_source',
    'cp_resource_reference' => 'cp_resource_reference',
    'cp_resource_file' => 'cp_resource_reference',
    'cp_resource_link' => 'cp_resource_reference',
    'cp_canonical_course' => 'cp_canonical_course',
    'cp_keywords' => 'cp_keywords',
    'changed' => 'changed',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'nid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'body_value' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'align' => '',
      'separator' => '<br/>',
      'empty_column' => 0,
    ),
    'cp_resource_type' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cp_resource_source' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cp_resource_reference' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cp_resource_file' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cp_resource_link' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cp_canonical_course' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cp_keywords' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'changed' => array(
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
  $handler->display->display_options['exposed_block'] = TRUE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Indexed Content: Node ID */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nid']['separator'] = '';
  $handler->display->display_options['fields']['nid']['link_to_entity'] = 0;
  /* Field: The main body text: Text (indexed) */
  $handler->display->display_options['fields']['body_value']['id'] = 'body_value';
  $handler->display->display_options['fields']['body_value']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['body_value']['field'] = 'body_value';
  $handler->display->display_options['fields']['body_value']['label'] = '';
  $handler->display->display_options['fields']['body_value']['exclude'] = TRUE;
  $handler->display->display_options['fields']['body_value']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body_value']['link_to_entity'] = 0;
  /* Field: Indexed Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'node/[nid]';
  $handler->display->display_options['fields']['title']['alter']['alt'] = '[body_value]';
  $handler->display->display_options['fields']['title']['link_to_entity'] = 0;
  /* Field: Indexed Content: Type of resource */
  $handler->display->display_options['fields']['cp_resource_type']['id'] = 'cp_resource_type';
  $handler->display->display_options['fields']['cp_resource_type']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['cp_resource_type']['field'] = 'cp_resource_type';
  $handler->display->display_options['fields']['cp_resource_type']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['cp_resource_type']['alter']['text'] = '([cp_resource_type])';
  /* Field: Indexed Content: Source */
  $handler->display->display_options['fields']['cp_resource_source']['id'] = 'cp_resource_source';
  $handler->display->display_options['fields']['cp_resource_source']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['cp_resource_source']['field'] = 'cp_resource_source';
  $handler->display->display_options['fields']['cp_resource_source']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['cp_resource_source']['view_mode'] = 'full';
  /* Field: Indexed Content: Reference */
  $handler->display->display_options['fields']['cp_resource_reference']['id'] = 'cp_resource_reference';
  $handler->display->display_options['fields']['cp_resource_reference']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['cp_resource_reference']['field'] = 'cp_resource_reference';
  /* Field: Indexed Content: File */
  $handler->display->display_options['fields']['cp_resource_file']['id'] = 'cp_resource_file';
  $handler->display->display_options['fields']['cp_resource_file']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['cp_resource_file']['field'] = 'cp_resource_file';
  $handler->display->display_options['fields']['cp_resource_file']['label'] = '';
  $handler->display->display_options['fields']['cp_resource_file']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['cp_resource_file']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['cp_resource_file']['delta_offset'] = '0';
  /* Field: Indexed Content: Link */
  $handler->display->display_options['fields']['cp_resource_link']['id'] = 'cp_resource_link';
  $handler->display->display_options['fields']['cp_resource_link']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['cp_resource_link']['field'] = 'cp_resource_link';
  $handler->display->display_options['fields']['cp_resource_link']['label'] = '';
  $handler->display->display_options['fields']['cp_resource_link']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['cp_resource_link']['click_sort_column'] = 'url';
  /* Field: Indexed Content: Canonical course */
  $handler->display->display_options['fields']['cp_canonical_course']['id'] = 'cp_canonical_course';
  $handler->display->display_options['fields']['cp_canonical_course']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['cp_canonical_course']['field'] = 'cp_canonical_course';
  $handler->display->display_options['fields']['cp_canonical_course']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['cp_canonical_course']['view_mode'] = 'full';
  /* Field: Indexed Content: Keywords */
  $handler->display->display_options['fields']['cp_keywords']['id'] = 'cp_keywords';
  $handler->display->display_options['fields']['cp_keywords']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['cp_keywords']['field'] = 'cp_keywords';
  $handler->display->display_options['fields']['cp_keywords']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['cp_keywords']['view_mode'] = 'full';
  /* Field: Indexed Content: Date changed */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = 'Last updated';
  $handler->display->display_options['fields']['changed']['date_format'] = 'short';
  $handler->display->display_options['fields']['changed']['link_to_entity'] = 0;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Indexed Content: Content type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'cp_resource' => 'cp_resource',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Indexed Content: Type of resource */
  $handler->display->display_options['filters']['cp_resource_type']['id'] = 'cp_resource_type';
  $handler->display->display_options['filters']['cp_resource_type']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['filters']['cp_resource_type']['field'] = 'cp_resource_type';
  $handler->display->display_options['filters']['cp_resource_type']['operator'] = '<>';
  $handler->display->display_options['filters']['cp_resource_type']['value'] = array(
    'book' => 'book',
  );
  $handler->display->display_options['filters']['cp_resource_type']['group'] = 1;
  /* Filter criterion: Search: Fulltext search */
  $handler->display->display_options['filters']['search_api_views_fulltext']['id'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['table'] = 'search_api_index_cp_node_index';
  $handler->display->display_options['filters']['search_api_views_fulltext']['field'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['group'] = 1;
  $handler->display->display_options['filters']['search_api_views_fulltext']['exposed'] = TRUE;
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator_id'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['label'] = 'Search';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['identifier'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['pane_title'] = 'Seach pane (not used)';
  $handler->display->display_options['pane_description'] = 'This display is only used for creating a block with exposed filters. It is not used in itself.';
  $handler->display->display_options['pane_category']['name'] = 'Course planner';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $handler->display->display_options['allow']['use_pager'] = 0;
  $handler->display->display_options['allow']['items_per_page'] = 0;
  $handler->display->display_options['allow']['offset'] = 0;
  $handler->display->display_options['allow']['link_to_view'] = 0;
  $handler->display->display_options['allow']['more_link'] = 0;
  $handler->display->display_options['allow']['path_override'] = 0;
  $handler->display->display_options['allow']['title_override'] = 'title_override';
  $handler->display->display_options['allow']['exposed_form'] = 0;
  $handler->display->display_options['allow']['fields_override'] = 0;
  $handler->display->display_options['inherit_panels_path'] = '1';
  $translatables['cp_browse'] = array(
    t('Master'),
    t('more'),
    t('Search'),
    t('Show all'),
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
    t('.'),
    t('Title'),
    t('[body_value]'),
    t('Type'),
    t('([type])'),
    t('Canonical course'),
    t('Keywords'),
    t('Author'),
    t('Last updated'),
    t('Resources (no book references)'),
    t('Browse resources'),
    t('Search results for Course planner resources that aren\'t book references'),
    t('Book references are excluded from this search. If you want to search book references too, go to the tab "All resources".'),
    t('Type of resource'),
    t('([cp_resource_type])'),
    t('Source'),
    t('Reference'),
    t('Resource browser (no books)'),
    t('Search results for Course planner resources.'),
    t('Course planner'),
    t('Resources'),
    t('Browse all resources'),
    t('Search results for all Course planner resources'),
    t('Resource browser (all)'),
    t('Search results for Course planner resources, including book references.'),
    t('All items'),
    t('Browse Course planner items'),
    t('Search results for all Course planner items'),
    t('Item browser'),
    t('Search results for all Course planner items.'),
    t('Search block (not used)'),
    t('This display is only used for creating a block with exposed filters. It is not used in itself.'),
    t('Seach pane (not used)'),
  );
  $export['cp_browse'] = $view;

  return $export;
}
