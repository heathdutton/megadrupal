<?php
/**
 * @file
 * crm_core_petition.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function crm_core_petition_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'cmcp_manage_petitions';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Petitions - Manage Petitions';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Manage Online Petitions';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'manage petition pages';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'path' => 'path',
    'view_node' => 'view_node',
    'field_cmcp_active' => 'field_cmcp_active',
    'created' => 'created',
    'edit_node' => 'edit_node',
    'nothing' => 'nothing',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'path' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'view_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcp_active' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['empty_table'] = TRUE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No petitions have been created.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Name';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Path */
  $handler->display->display_options['fields']['path']['id'] = 'path';
  $handler->display->display_options['fields']['path']['table'] = 'node';
  $handler->display->display_options['fields']['path']['field'] = 'path';
  $handler->display->display_options['fields']['path']['label'] = 'Path(Token)';
  $handler->display->display_options['fields']['path']['exclude'] = TRUE;
  /* Field: Content: Link */
  $handler->display->display_options['fields']['view_node']['id'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['view_node']['field'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['label'] = 'Path';
  $handler->display->display_options['fields']['view_node']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['view_node']['alter']['text'] = '[path]';
  $handler->display->display_options['fields']['view_node']['text'] = '[path]';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_cmcp_active']['id'] = 'field_cmcp_active';
  $handler->display->display_options['fields']['field_cmcp_active']['table'] = 'field_data_field_cmcp_active';
  $handler->display->display_options['fields']['field_cmcp_active']['field'] = 'field_cmcp_active';
  $handler->display->display_options['fields']['field_cmcp_active']['label'] = 'Status';
  $handler->display->display_options['fields']['field_cmcp_active']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_cmcp_active']['alter']['text'] = 'Enabled';
  $handler->display->display_options['fields']['field_cmcp_active']['empty'] = 'Disabled';
  $handler->display->display_options['fields']['field_cmcp_active']['empty_zero'] = TRUE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Date Created';
  $handler->display->display_options['fields']['created']['date_format'] = 'long';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['exclude'] = TRUE;
  $handler->display->display_options['fields']['edit_node']['text'] = 'Edit';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Operations';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[edit_node] | Summary | Signatories';
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
    'cmcp_petition' => 'cmcp_petition',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/structure/crm-core/cmcp/manage-petitions';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'Manage Petitions';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['title'] = 'CRM Core Petition';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $export['cmcp_manage_petitions'] = $view;

  $view = new view();
  $view->name = 'cmcp_overview_widget';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Petitions Overview Widget';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Petitions Summary';
  $handler->display->display_options['css_class'] = 'summary-table';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view petition overview';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Entity Reference: Referencing entity */
  $handler->display->display_options['relationships']['reverse_field_cmcp_petition_page_crm_core_activity']['id'] = 'reverse_field_cmcp_petition_page_crm_core_activity';
  $handler->display->display_options['relationships']['reverse_field_cmcp_petition_page_crm_core_activity']['table'] = 'node';
  $handler->display->display_options['relationships']['reverse_field_cmcp_petition_page_crm_core_activity']['field'] = 'reverse_field_cmcp_petition_page_crm_core_activity';
  /* Field: COUNT(DISTINCT Content: Nid) */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['nid']['label'] = 'Active Petitions';
  /* Field: COUNT(DISTINCT CRM Core Activity: Activity ID) */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['relationship'] = 'reverse_field_cmcp_petition_page_crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['activity_id']['label'] = 'Total Signatures';
  /* Field: Global: Math expression */
  $handler->display->display_options['fields']['expression']['id'] = 'expression';
  $handler->display->display_options['fields']['expression']['table'] = 'views';
  $handler->display->display_options['fields']['expression']['field'] = 'expression';
  $handler->display->display_options['fields']['expression']['label'] = 'Average Signatures';
  $handler->display->display_options['fields']['expression']['set_precision'] = TRUE;
  $handler->display->display_options['fields']['expression']['precision'] = '2';
  $handler->display->display_options['fields']['expression']['expression'] = '[activity_id]/([nid]+.001)';
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
    'cmcp_petition' => 'cmcp_petition',
  );

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['pane_title'] = 'Petitions Summary Widget';
  $handler->display->display_options['pane_description'] = 'Metrics around all petition activity in the site.';
  $handler->display->display_options['pane_category']['name'] = 'CRM Core Petition';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $export['cmcp_overview_widget'] = $view;

  $view = new view();
  $view->name = 'cmcp_petition_targets_report';
  $view->description = 'This view provides a list of petition targets, grouped by the petition campaigns that target them.';
  $view->tag = 'crm_core_petition';
  $view->base_table = 'crm_core_activity';
  $view->human_name = 'Petition Targets Report';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Petition Targets';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view targets report';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'contact_name',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['style_options']['columns'] = array(
    'contact_name' => 'contact_name',
    'field_cmcp_petition_page' => 'field_cmcp_petition_page',
    'activity_id' => 'activity_id',
    'field_cmcp_received_date' => 'field_cmcp_received_date',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'contact_name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcp_petition_page' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'activity_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcp_received_date' => array(
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
  $handler->display->display_options['empty']['area']['content'] = '<div class="ao_no_results">No information about petition targets is stored in the system at this time. </div>';
  $handler->display->display_options['empty']['area']['format'] = 'full_html';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_cmcp_petition_targets_target_id']['id'] = 'field_cmcp_petition_targets_target_id';
  $handler->display->display_options['relationships']['field_cmcp_petition_targets_target_id']['table'] = 'field_data_field_cmcp_petition_targets';
  $handler->display->display_options['relationships']['field_cmcp_petition_targets_target_id']['field'] = 'field_cmcp_petition_targets_target_id';
  $handler->display->display_options['relationships']['field_cmcp_petition_targets_target_id']['label'] = 'CRM Contact from field_cmcp_petition_targets';
  /* Field: CRM Core Contact: Household */
  $handler->display->display_options['fields']['contact_name']['id'] = 'contact_name';
  $handler->display->display_options['fields']['contact_name']['table'] = 'field_data_contact_name';
  $handler->display->display_options['fields']['contact_name']['field'] = 'contact_name';
  $handler->display->display_options['fields']['contact_name']['relationship'] = 'field_cmcp_petition_targets_target_id';
  $handler->display->display_options['fields']['contact_name']['label'] = 'Target';
  $handler->display->display_options['fields']['contact_name']['exclude'] = TRUE;
  $handler->display->display_options['fields']['contact_name']['click_sort_column'] = 'family';
  $handler->display->display_options['fields']['contact_name']['settings'] = array(
    'format' => 'default',
    'markup' => 0,
    'output' => 'default',
    'multiple' => 'default',
    'multiple_delimiter' => ', ',
    'multiple_and' => 'text',
    'multiple_delimiter_precedes_last' => 'never',
    'multiple_el_al_min' => '3',
    'multiple_el_al_first' => '1',
  );
  $handler->display->display_options['fields']['contact_name']['group_column'] = 'entity_id';
  /* Field: CRM Activity: Petition */
  $handler->display->display_options['fields']['field_cmcp_petition_page']['id'] = 'field_cmcp_petition_page';
  $handler->display->display_options['fields']['field_cmcp_petition_page']['table'] = 'field_data_field_cmcp_petition_page';
  $handler->display->display_options['fields']['field_cmcp_petition_page']['field'] = 'field_cmcp_petition_page';
  $handler->display->display_options['fields']['field_cmcp_petition_page']['settings'] = array(
    'link' => 0,
  );
  /* Field: COUNT(CRM Core Activity: Activity ID) */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['group_type'] = 'count';
  $handler->display->display_options['fields']['activity_id']['label'] = 'Signatures';
  /* Field: MAX(CRM Activity: Date of Signature) */
  $handler->display->display_options['fields']['field_cmcp_received_date']['id'] = 'field_cmcp_received_date';
  $handler->display->display_options['fields']['field_cmcp_received_date']['table'] = 'field_data_field_cmcp_received_date';
  $handler->display->display_options['fields']['field_cmcp_received_date']['field'] = 'field_cmcp_received_date';
  $handler->display->display_options['fields']['field_cmcp_received_date']['group_type'] = 'max';
  $handler->display->display_options['fields']['field_cmcp_received_date']['label'] = 'Date of Last Signature';
  $handler->display->display_options['fields']['field_cmcp_received_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Sort criterion: CRM Core Contact: Household (contact_name:given) */
  $handler->display->display_options['sorts']['contact_name_given']['id'] = 'contact_name_given';
  $handler->display->display_options['sorts']['contact_name_given']['table'] = 'field_data_contact_name';
  $handler->display->display_options['sorts']['contact_name_given']['field'] = 'contact_name_given';
  $handler->display->display_options['sorts']['contact_name_given']['relationship'] = 'field_cmcp_petition_targets_target_id';
  /* Sort criterion: COUNT(CRM Core Activity: Activity ID) */
  $handler->display->display_options['sorts']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['sorts']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['sorts']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['sorts']['activity_id']['group_type'] = 'count';
  $handler->display->display_options['sorts']['activity_id']['order'] = 'DESC';
  /* Filter criterion: CRM Core Activity: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'crm_core_activity';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'cmcp_petition_signature' => 'cmcp_petition_signature',
  );
  /* Filter criterion: CRM Activity: Petition Targets (field_cmcp_petition_targets) */
  $handler->display->display_options['filters']['field_cmcp_petition_targets_target_id']['id'] = 'field_cmcp_petition_targets_target_id';
  $handler->display->display_options['filters']['field_cmcp_petition_targets_target_id']['table'] = 'field_data_field_cmcp_petition_targets';
  $handler->display->display_options['filters']['field_cmcp_petition_targets_target_id']['field'] = 'field_cmcp_petition_targets_target_id';
  $handler->display->display_options['filters']['field_cmcp_petition_targets_target_id']['operator'] = 'not empty';
  /* Filter criterion: CRM Core Contact: Household */
  $handler->display->display_options['filters']['contact_name']['id'] = 'contact_name';
  $handler->display->display_options['filters']['contact_name']['table'] = 'field_data_contact_name';
  $handler->display->display_options['filters']['contact_name']['field'] = 'contact_name';
  $handler->display->display_options['filters']['contact_name']['relationship'] = 'field_cmcp_petition_targets_target_id';
  $handler->display->display_options['filters']['contact_name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['contact_name']['expose']['operator_id'] = 'contact_name_op';
  $handler->display->display_options['filters']['contact_name']['expose']['label'] = 'Filter by Name';
  $handler->display->display_options['filters']['contact_name']['expose']['operator'] = 'contact_name_op';
  $handler->display->display_options['filters']['contact_name']['expose']['identifier'] = 'contact_name';
  $handler->display->display_options['filters']['contact_name']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['contact_name']['group_info']['label'] = 'Household name';
  $handler->display->display_options['filters']['contact_name']['group_info']['identifier'] = 'contact_name';
  $handler->display->display_options['filters']['contact_name']['group_info']['remember'] = FALSE;
  $handler->display->display_options['filters']['contact_name']['group_info']['group_items'] = array(
    1 => array(),
    2 => array(),
    3 => array(),
  );

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['pane_title'] = 'Petition Target Table';
  $handler->display->display_options['pane_description'] = 'Provides a pageable list of targets for petition campaigns, listing details about who has been targeted by each campaign.';
  $handler->display->display_options['pane_category']['name'] = 'CRM Core Petition';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $handler->display->display_options['inherit_panels_path'] = '1';
  $export['cmcp_petition_targets_report'] = $view;

  $view = new view();
  $view->name = 'cmcp_recent_signatures';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'crm_core_activity';
  $view->human_name = 'Petition Recent Signatures';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Recent Signatures';
  $handler->display->display_options['css_class'] = 'fancy-headers';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'activity_id' => 'activity_id',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'activity_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_cmcp_petition_page_target_id']['id'] = 'field_cmcp_petition_page_target_id';
  $handler->display->display_options['relationships']['field_cmcp_petition_page_target_id']['table'] = 'field_data_field_cmcp_petition_page';
  $handler->display->display_options['relationships']['field_cmcp_petition_page_target_id']['field'] = 'field_cmcp_petition_page_target_id';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['id'] = 'field_activity_participants_target_id';
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['table'] = 'field_data_field_activity_participants';
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['field'] = 'field_activity_participants_target_id';
  /* Field: CRM Core Activity: Activity ID */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['exclude'] = TRUE;
  /* Field: CRM Contact: Contact ID */
  $handler->display->display_options['fields']['contact_id']['id'] = 'contact_id';
  $handler->display->display_options['fields']['contact_id']['table'] = 'crm_core_contact';
  $handler->display->display_options['fields']['contact_id']['field'] = 'contact_id';
  $handler->display->display_options['fields']['contact_id']['relationship'] = 'field_activity_participants_target_id';
  $handler->display->display_options['fields']['contact_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['contact_id']['link_to_contact'] = 0;
  /* Field: CRM Activity: Date of Signature */
  $handler->display->display_options['fields']['field_cmcp_received_date']['id'] = 'field_cmcp_received_date';
  $handler->display->display_options['fields']['field_cmcp_received_date']['table'] = 'field_data_field_cmcp_received_date';
  $handler->display->display_options['fields']['field_cmcp_received_date']['field'] = 'field_cmcp_received_date';
  $handler->display->display_options['fields']['field_cmcp_received_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'field_cmcp_petition_page_target_id';
  $handler->display->display_options['fields']['title']['label'] = 'Petition';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'View Link';
  $handler->display->display_options['fields']['nothing_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'View';
  $handler->display->display_options['fields']['nothing_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['path'] = 'crm-core/contact/[contact_id]/activity/[activity_id]';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
  $handler->display->display_options['fields']['nothing_2']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_2']['label'] = 'Edit Link';
  $handler->display->display_options['fields']['nothing_2']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['text'] = 'Edit';
  $handler->display->display_options['fields']['nothing_2']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['path'] = 'crm-core/contact/[contact_id]/activity/[activity_id]/edit';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Operations';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<div class="btn btn-blue btn-dark btn-simple">[nothing_1]</div>
<div class="btn btn-green btn-dark btn-simple">[nothing_2]</div>';
  $handler->display->display_options['fields']['nothing']['element_class'] = 'field-operations';
  /* Sort criterion: CRM Activity: Date of Signature (field_cmcp_received_date) */
  $handler->display->display_options['sorts']['field_cmcp_received_date_value']['id'] = 'field_cmcp_received_date_value';
  $handler->display->display_options['sorts']['field_cmcp_received_date_value']['table'] = 'field_data_field_cmcp_received_date';
  $handler->display->display_options['sorts']['field_cmcp_received_date_value']['field'] = 'field_cmcp_received_date_value';
  $handler->display->display_options['sorts']['field_cmcp_received_date_value']['order'] = 'DESC';
  /* Filter criterion: CRM Core Activity: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'crm_core_activity';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'cmcp_petition_signature' => 'cmcp_petition_signature',
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block_1');

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['pane_title'] = 'Petitions: Recent Signatures';
  $handler->display->display_options['pane_category']['name'] = 'CRM Core Petition';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $export['cmcp_recent_signatures'] = $view;

  $view = new view();
  $view->name = 'cmcp_signatories_report';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'crm_core_activity';
  $view->human_name = 'Petition Signatories Report';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Petition Signatories';
  $handler->display->display_options['css_class'] = 'fancy-headers';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view petition signatories report';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'activity_id' => 'activity_id',
    'contact_id' => 'contact_id',
    'field_cmcp_received_date' => 'field_cmcp_received_date',
    'url' => 'url',
    'contact_name' => 'contact_name',
    'title' => 'title',
    'field_cmcp_source' => 'field_cmcp_source',
    'nothing_1' => 'nothing_1',
    'nothing_2' => 'nothing_2',
    'nothing' => 'nothing',
    'nothing_3' => 'nothing_3',
    'nothing_4' => 'nothing_4',
    'nothing_5' => 'nothing_5',
  );
  $handler->display->display_options['style_options']['default'] = 'field_cmcp_received_date';
  $handler->display->display_options['style_options']['info'] = array(
    'activity_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'contact_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcp_received_date' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'url' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'contact_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcp_source' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing_1' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing_2' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing_3' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing_4' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing_5' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  $handler->display->display_options['header']['result']['content'] = '<div class="results-summary">Displaying records @start - @end of @total.</div>';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '<div class="detail-buttons">
 <div class="btn btn-blue btn-simple">[nothing_4]</div>
 <div class="btn btn-orange btn-simple">[nothing_3]</div>
</div>';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  $handler->display->display_options['header']['area']['tokenize'] = TRUE;
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['id'] = 'field_activity_participants_target_id';
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['table'] = 'field_data_field_activity_participants';
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['field'] = 'field_activity_participants_target_id';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_cmcp_petition_page_target_id']['id'] = 'field_cmcp_petition_page_target_id';
  $handler->display->display_options['relationships']['field_cmcp_petition_page_target_id']['table'] = 'field_data_field_cmcp_petition_page';
  $handler->display->display_options['relationships']['field_cmcp_petition_page_target_id']['field'] = 'field_cmcp_petition_page_target_id';
  /* Field: CRM Core Activity: Activity ID */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['exclude'] = TRUE;
  /* Field: CRM Contact: Contact ID */
  $handler->display->display_options['fields']['contact_id']['id'] = 'contact_id';
  $handler->display->display_options['fields']['contact_id']['table'] = 'crm_core_contact';
  $handler->display->display_options['fields']['contact_id']['field'] = 'contact_id';
  $handler->display->display_options['fields']['contact_id']['relationship'] = 'field_activity_participants_target_id';
  $handler->display->display_options['fields']['contact_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['contact_id']['link_to_contact'] = 0;
  /* Field: CRM Activity: Date of Signature */
  $handler->display->display_options['fields']['field_cmcp_received_date']['id'] = 'field_cmcp_received_date';
  $handler->display->display_options['fields']['field_cmcp_received_date']['table'] = 'field_data_field_cmcp_received_date';
  $handler->display->display_options['fields']['field_cmcp_received_date']['field'] = 'field_cmcp_received_date';
  $handler->display->display_options['fields']['field_cmcp_received_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: CRM Contact: URL */
  $handler->display->display_options['fields']['url']['id'] = 'url';
  $handler->display->display_options['fields']['url']['table'] = 'views_entity_crm_core_contact';
  $handler->display->display_options['fields']['url']['field'] = 'url';
  $handler->display->display_options['fields']['url']['relationship'] = 'field_activity_participants_target_id';
  $handler->display->display_options['fields']['url']['label'] = 'Name';
  $handler->display->display_options['fields']['url']['exclude'] = TRUE;
  $handler->display->display_options['fields']['url']['alter']['text'] = '[contact_name]';
  $handler->display->display_options['fields']['url']['display_as_link'] = FALSE;
  $handler->display->display_options['fields']['url']['link_to_entity'] = 0;
  /* Field: CRM Core Contact: Household */
  $handler->display->display_options['fields']['contact_name']['id'] = 'contact_name';
  $handler->display->display_options['fields']['contact_name']['table'] = 'field_data_contact_name';
  $handler->display->display_options['fields']['contact_name']['field'] = 'contact_name';
  $handler->display->display_options['fields']['contact_name']['relationship'] = 'field_activity_participants_target_id';
  $handler->display->display_options['fields']['contact_name']['label'] = 'Name';
  $handler->display->display_options['fields']['contact_name']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['contact_name']['alter']['path'] = '[url]';
  $handler->display->display_options['fields']['contact_name']['element_class'] = 'field-object';
  $handler->display->display_options['fields']['contact_name']['click_sort_column'] = 'title';
  $handler->display->display_options['fields']['contact_name']['settings'] = array(
    'format' => 'default',
    'markup' => 0,
    'output' => 'default',
    'multiple' => 'default',
    'multiple_delimiter' => ', ',
    'multiple_and' => 'text',
    'multiple_delimiter_precedes_last' => 'never',
    'multiple_el_al_min' => '3',
    'multiple_el_al_first' => '1',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'field_cmcp_petition_page_target_id';
  /* Field: CRM Activity: Petition Source */
  $handler->display->display_options['fields']['field_cmcp_source']['id'] = 'field_cmcp_source';
  $handler->display->display_options['fields']['field_cmcp_source']['table'] = 'field_data_field_cmcp_source';
  $handler->display->display_options['fields']['field_cmcp_source']['field'] = 'field_cmcp_source';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'View Link';
  $handler->display->display_options['fields']['nothing_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'View';
  $handler->display->display_options['fields']['nothing_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['path'] = 'crm-core/contact/[contact_id]/activity/[activity_id]';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
  $handler->display->display_options['fields']['nothing_2']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_2']['label'] = 'Edit Link';
  $handler->display->display_options['fields']['nothing_2']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['text'] = 'Edit';
  $handler->display->display_options['fields']['nothing_2']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['path'] = 'crm-core/contact/[contact_id]/activity/[activity_id]/edit';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Operations';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<div class="btn btn-blue btn-dark btn-simple">[nothing_1]</div>
<div class="btn btn-green btn-dark btn-simple">[nothing_2]</div>';
  $handler->display->display_options['fields']['nothing']['element_class'] = 'field-operations';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_3']['id'] = 'nothing_3';
  $handler->display->display_options['fields']['nothing_3']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_3']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_3']['label'] = 'Export to CSV';
  $handler->display->display_options['fields']['nothing_3']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_3']['alter']['text'] = 'Export to CSV';
  $handler->display->display_options['fields']['nothing_3']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_3']['alter']['path'] = 'crm-core/reports/cmcp/petition-signatories/export';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_4']['id'] = 'nothing_4';
  $handler->display->display_options['fields']['nothing_4']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_4']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_4']['label'] = 'View All';
  $handler->display->display_options['fields']['nothing_4']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_4']['alter']['text'] = 'View all';
  $handler->display->display_options['fields']['nothing_4']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_4']['alter']['path'] = 'crm-core/reports/cmcp/petition-signatories/all';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_5']['id'] = 'nothing_5';
  $handler->display->display_options['fields']['nothing_5']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_5']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_5']['label'] = 'View paged results';
  $handler->display->display_options['fields']['nothing_5']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_5']['alter']['text'] = 'View paged results';
  $handler->display->display_options['fields']['nothing_5']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_5']['alter']['path'] = 'crm-core/reports/cmcp/petition-signatories';
  /* Filter criterion: CRM Core Activity: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'crm_core_activity';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'cmcp_petition_signature' => 'cmcp_petition_signature',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: CRM Activity: Date of Signature (field_cmcp_received_date) */
  $handler->display->display_options['filters']['field_cmcp_received_date_value']['id'] = 'field_cmcp_received_date_value';
  $handler->display->display_options['filters']['field_cmcp_received_date_value']['table'] = 'field_data_field_cmcp_received_date';
  $handler->display->display_options['filters']['field_cmcp_received_date_value']['field'] = 'field_cmcp_received_date_value';
  $handler->display->display_options['filters']['field_cmcp_received_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_cmcp_received_date_value']['group'] = 1;
  $handler->display->display_options['filters']['field_cmcp_received_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_cmcp_received_date_value']['expose']['operator_id'] = 'field_cmcp_received_date_value_op';
  $handler->display->display_options['filters']['field_cmcp_received_date_value']['expose']['label'] = 'Filter by dates between:';
  $handler->display->display_options['filters']['field_cmcp_received_date_value']['expose']['operator'] = 'field_cmcp_received_date_value_op';
  $handler->display->display_options['filters']['field_cmcp_received_date_value']['expose']['identifier'] = 'field_cmcp_received_date_value';
  $handler->display->display_options['filters']['field_cmcp_received_date_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_cmcp_received_date_value']['form_type'] = 'date_popup';
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['relationship'] = 'field_cmcp_petition_page_target_id';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Filter By Petition Page Title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: CRM Activity: Petition Source (field_cmcp_source) */
  $handler->display->display_options['filters']['field_cmcp_source_value']['id'] = 'field_cmcp_source_value';
  $handler->display->display_options['filters']['field_cmcp_source_value']['table'] = 'field_data_field_cmcp_source';
  $handler->display->display_options['filters']['field_cmcp_source_value']['field'] = 'field_cmcp_source_value';
  $handler->display->display_options['filters']['field_cmcp_source_value']['group'] = 1;
  $handler->display->display_options['filters']['field_cmcp_source_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_cmcp_source_value']['expose']['operator_id'] = 'field_cmcp_source_value_op';
  $handler->display->display_options['filters']['field_cmcp_source_value']['expose']['label'] = 'Filter by source:';
  $handler->display->display_options['filters']['field_cmcp_source_value']['expose']['operator'] = 'field_cmcp_source_value_op';
  $handler->display->display_options['filters']['field_cmcp_source_value']['expose']['identifier'] = 'field_cmcp_source_value';
  $handler->display->display_options['filters']['field_cmcp_source_value']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_cmcp_source_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'crm-core/reports/cmcp/petition-signatories';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Signatories';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_xls';
  $handler->display->display_options['style_options']['provide_file'] = 0;
  $handler->display->display_options['style_options']['parent_sort'] = 1;
  $handler->display->display_options['path'] = 'crm-core/reports/cmcp/petition-signatories/export';
  $handler->display->display_options['displays'] = array(
    'default' => 'default',
    'page' => 0,
  );

  /* Display: Page Full */
  $handler = $view->new_display('page', 'Page Full', 'page_full');
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  $handler->display->display_options['header']['result']['content'] = '<div class="results-summary">Displaying records @start - @end of @total.</div>';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '<div class="detail-buttons">
 <div class="btn btn-blue btn-simple">[nothing_5]</div>
 <div class="btn btn-orange btn-simple">[nothing_3]</div>
</div>';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  $handler->display->display_options['header']['area']['tokenize'] = TRUE;
  $handler->display->display_options['path'] = 'crm-core/reports/cmcp/petition-signatories/all';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Signatories';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['cmcp_signatories_report'] = $view;

  $view = new view();
  $view->name = 'cmcp_signatories_single_view';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'crm_core_activity';
  $view->human_name = 'Petition Signatories Report';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Signatories';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view petition signatories';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_tags'] = array(
    0 => 'cmcp_petition_signatories',
  );
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'activity_id' => 'activity_id',
    'contact_name' => 'contact_name',
    'contact_id' => 'contact_id',
    'field_cmcp_received_date' => 'field_cmcp_received_date',
    'edit_crm_activity' => 'edit_crm_activity',
    'view_crm_activity' => 'view_crm_activity',
    'nothing' => 'nothing',
  );
  $handler->display->display_options['style_options']['default'] = 'contact_name';
  $handler->display->display_options['style_options']['info'] = array(
    'activity_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'contact_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'contact_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcp_received_date' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_crm_activity' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'view_crm_activity' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  $handler->display->display_options['header']['result']['content'] = 'Displaying @start - @end signatories.';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'This petition did not gathered any signatures yet. Be the first to sign!';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['id'] = 'field_activity_participants_target_id';
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['table'] = 'field_data_field_activity_participants';
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['field'] = 'field_activity_participants_target_id';
  /* Field: CRM Core Activity: Activity ID */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['exclude'] = TRUE;
  /* Field: CRM Core Contact: Household */
  $handler->display->display_options['fields']['contact_name']['id'] = 'contact_name';
  $handler->display->display_options['fields']['contact_name']['table'] = 'field_data_contact_name';
  $handler->display->display_options['fields']['contact_name']['field'] = 'contact_name';
  $handler->display->display_options['fields']['contact_name']['relationship'] = 'field_activity_participants_target_id';
  $handler->display->display_options['fields']['contact_name']['exclude'] = TRUE;
  $handler->display->display_options['fields']['contact_name']['click_sort_column'] = 'title';
  $handler->display->display_options['fields']['contact_name']['settings'] = array(
    'format' => 'short_full',
    'markup' => 0,
    'output' => 'default',
    'multiple' => 'default',
    'multiple_delimiter' => ', ',
    'multiple_and' => 'text',
    'multiple_delimiter_precedes_last' => 'never',
    'multiple_el_al_min' => '3',
    'multiple_el_al_first' => '1',
  );
  /* Field: CRM Contact: Contact ID */
  $handler->display->display_options['fields']['contact_id']['id'] = 'contact_id';
  $handler->display->display_options['fields']['contact_id']['table'] = 'crm_core_contact';
  $handler->display->display_options['fields']['contact_id']['field'] = 'contact_id';
  $handler->display->display_options['fields']['contact_id']['relationship'] = 'field_activity_participants_target_id';
  $handler->display->display_options['fields']['contact_id']['label'] = 'Name';
  $handler->display->display_options['fields']['contact_id']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['contact_id']['alter']['text'] = '[contact_name]';
  $handler->display->display_options['fields']['contact_id']['link_to_contact'] = 1;
  /* Field: CRM Activity: Date of Signature */
  $handler->display->display_options['fields']['field_cmcp_received_date']['id'] = 'field_cmcp_received_date';
  $handler->display->display_options['fields']['field_cmcp_received_date']['table'] = 'field_data_field_cmcp_received_date';
  $handler->display->display_options['fields']['field_cmcp_received_date']['field'] = 'field_cmcp_received_date';
  $handler->display->display_options['fields']['field_cmcp_received_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: CRM Core Activity: Edit link */
  $handler->display->display_options['fields']['edit_crm_activity']['id'] = 'edit_crm_activity';
  $handler->display->display_options['fields']['edit_crm_activity']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['edit_crm_activity']['field'] = 'edit_crm_activity';
  $handler->display->display_options['fields']['edit_crm_activity']['exclude'] = TRUE;
  $handler->display->display_options['fields']['edit_crm_activity']['text'] = 'Edit';
  $handler->display->display_options['fields']['edit_crm_activity']['redirect'] = 0;
  /* Field: CRM Core Activity: Link */
  $handler->display->display_options['fields']['view_crm_activity']['id'] = 'view_crm_activity';
  $handler->display->display_options['fields']['view_crm_activity']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['view_crm_activity']['field'] = 'view_crm_activity';
  $handler->display->display_options['fields']['view_crm_activity']['exclude'] = TRUE;
  $handler->display->display_options['fields']['view_crm_activity']['text'] = 'View';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Operations';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[view_crm_activity] | [edit_crm_activity] ';
  /* Contextual filter: CRM Activity: Petition (field_cmcp_petition_page) */
  $handler->display->display_options['arguments']['field_cmcp_petition_page_target_id']['id'] = 'field_cmcp_petition_page_target_id';
  $handler->display->display_options['arguments']['field_cmcp_petition_page_target_id']['table'] = 'field_data_field_cmcp_petition_page';
  $handler->display->display_options['arguments']['field_cmcp_petition_page_target_id']['field'] = 'field_cmcp_petition_page_target_id';
  $handler->display->display_options['arguments']['field_cmcp_petition_page_target_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_cmcp_petition_page_target_id']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['field_cmcp_petition_page_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_cmcp_petition_page_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_cmcp_petition_page_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_cmcp_petition_page_target_id']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['field_cmcp_petition_page_target_id']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['field_cmcp_petition_page_target_id']['validate_options']['types'] = array(
    'cmcp_petition' => 'cmcp_petition',
  );
  /* Filter criterion: CRM Core Activity: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'crm_core_activity';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'cmcp_petition_signature' => 'cmcp_petition_signature',
  );
  /* Filter criterion: CRM Activity: View publicly (field_cmcp_public_display) */
  $handler->display->display_options['filters']['field_cmcp_public_display_value']['id'] = 'field_cmcp_public_display_value';
  $handler->display->display_options['filters']['field_cmcp_public_display_value']['table'] = 'field_data_field_cmcp_public_display';
  $handler->display->display_options['filters']['field_cmcp_public_display_value']['field'] = 'field_cmcp_public_display_value';
  $handler->display->display_options['filters']['field_cmcp_public_display_value']['value'] = array(
    1 => '1',
  );
  $export['cmcp_signatories_single_view'] = $view;

  $view = new view();
  $view->name = 'cmcp_source_summary_header';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'crm_core_activity';
  $view->human_name = 'Petitions Source Summary header';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['css_class'] = 'summary-table';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'activity_id' => 'activity_id',
    'nid' => 'nid',
    'expression' => 'expression',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'activity_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
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
    'expression' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_cmcp_petition_page_target_id']['id'] = 'field_cmcp_petition_page_target_id';
  $handler->display->display_options['relationships']['field_cmcp_petition_page_target_id']['table'] = 'field_data_field_cmcp_petition_page';
  $handler->display->display_options['relationships']['field_cmcp_petition_page_target_id']['field'] = 'field_cmcp_petition_page_target_id';
  /* Field: COUNT(CRM Core Activity: Activity ID) */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['group_type'] = 'count';
  $handler->display->display_options['fields']['activity_id']['label'] = 'Total Signatures';
  /* Field: COUNT(DISTINCT Content: Nid) */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['relationship'] = 'field_cmcp_petition_page_target_id';
  $handler->display->display_options['fields']['nid']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Global: Math expression */
  $handler->display->display_options['fields']['expression']['id'] = 'expression';
  $handler->display->display_options['fields']['expression']['table'] = 'views';
  $handler->display->display_options['fields']['expression']['field'] = 'expression';
  $handler->display->display_options['fields']['expression']['label'] = 'Average Signatures Per Petition';
  $handler->display->display_options['fields']['expression']['set_precision'] = TRUE;
  $handler->display->display_options['fields']['expression']['precision'] = '1';
  $handler->display->display_options['fields']['expression']['expression'] = '[activity_id]/[nid]';
  /* Contextual filter: CRM Activity: Petition Source (field_cmcp_source) */
  $handler->display->display_options['arguments']['field_cmcp_source_value']['id'] = 'field_cmcp_source_value';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['table'] = 'field_data_field_cmcp_source';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['field'] = 'field_cmcp_source_value';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['limit'] = '0';
  /* Filter criterion: CRM Core Activity: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'crm_core_activity';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'min' => '',
    'max' => '',
    'value' => '',
    'cmcp_petition_signature' => 'cmcp_petition_signature',
  );
  $export['cmcp_source_summary_header'] = $view;

  $view = new view();
  $view->name = 'cmcp_source_summary_top_petitions';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Petitions Source Summary Top Petitions';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Top Petitions';
  $handler->display->display_options['css_class'] = 'fancy-headers';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Entity Reference: Referencing entity */
  $handler->display->display_options['relationships']['reverse_field_cmcp_petition_page_crm_core_activity']['id'] = 'reverse_field_cmcp_petition_page_crm_core_activity';
  $handler->display->display_options['relationships']['reverse_field_cmcp_petition_page_crm_core_activity']['table'] = 'node';
  $handler->display->display_options['relationships']['reverse_field_cmcp_petition_page_crm_core_activity']['field'] = 'reverse_field_cmcp_petition_page_crm_core_activity';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Petition';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'crm-core/reports/cmcp/petition-report/summary/[nid]';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: COUNT(CRM Core Activity: Activity ID) */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['relationship'] = 'reverse_field_cmcp_petition_page_crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['group_type'] = 'count';
  $handler->display->display_options['fields']['activity_id']['label'] = 'Signatures';
  /* Sort criterion: COUNT(CRM Core Activity: Activity ID) */
  $handler->display->display_options['sorts']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['sorts']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['sorts']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['sorts']['activity_id']['relationship'] = 'reverse_field_cmcp_petition_page_crm_core_activity';
  $handler->display->display_options['sorts']['activity_id']['group_type'] = 'count';
  $handler->display->display_options['sorts']['activity_id']['order'] = 'DESC';
  /* Contextual filter: CRM Activity: Petition Source (field_cmcp_source) */
  $handler->display->display_options['arguments']['field_cmcp_source_value']['id'] = 'field_cmcp_source_value';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['table'] = 'field_data_field_cmcp_source';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['field'] = 'field_cmcp_source_value';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['relationship'] = 'reverse_field_cmcp_petition_page_crm_core_activity';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['limit'] = '0';
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
    'cmcp_petition' => 'cmcp_petition',
  );
  $export['cmcp_source_summary_top_petitions'] = $view;

  $view = new view();
  $view->name = 'cmcp_source_summary_top_signatories';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'crm_core_contact';
  $view->human_name = 'Petitions Source Summary Top Signatories';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Top Signatories';
  $handler->display->display_options['css_class'] = 'fancy-headers';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'contact_id' => 'contact_id',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'contact_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Entity Reference: Referencing entity */
  $handler->display->display_options['relationships']['reverse_field_activity_participants_crm_core_activity']['id'] = 'reverse_field_activity_participants_crm_core_activity';
  $handler->display->display_options['relationships']['reverse_field_activity_participants_crm_core_activity']['table'] = 'crm_core_contact';
  $handler->display->display_options['relationships']['reverse_field_activity_participants_crm_core_activity']['field'] = 'reverse_field_activity_participants_crm_core_activity';
  /* Field: CRM Contact: Contact ID */
  $handler->display->display_options['fields']['contact_id']['id'] = 'contact_id';
  $handler->display->display_options['fields']['contact_id']['table'] = 'crm_core_contact';
  $handler->display->display_options['fields']['contact_id']['field'] = 'contact_id';
  $handler->display->display_options['fields']['contact_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['contact_id']['link_to_contact'] = 1;
  /* Field: CRM Contact: URL */
  $handler->display->display_options['fields']['url']['id'] = 'url';
  $handler->display->display_options['fields']['url']['table'] = 'views_entity_crm_core_contact';
  $handler->display->display_options['fields']['url']['field'] = 'url';
  $handler->display->display_options['fields']['url']['exclude'] = TRUE;
  $handler->display->display_options['fields']['url']['display_as_link'] = FALSE;
  $handler->display->display_options['fields']['url']['link_to_entity'] = 0;
  /* Field: CRM Core Contact: Household */
  $handler->display->display_options['fields']['contact_name']['id'] = 'contact_name';
  $handler->display->display_options['fields']['contact_name']['table'] = 'field_data_contact_name';
  $handler->display->display_options['fields']['contact_name']['field'] = 'contact_name';
  $handler->display->display_options['fields']['contact_name']['label'] = 'Signatory';
  $handler->display->display_options['fields']['contact_name']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['contact_name']['alter']['path'] = '[url]';
  $handler->display->display_options['fields']['contact_name']['click_sort_column'] = 'title';
  $handler->display->display_options['fields']['contact_name']['settings'] = array(
    'format' => 'default',
    'markup' => 0,
    'output' => 'default',
    'multiple' => 'default',
    'multiple_delimiter' => ', ',
    'multiple_and' => 'text',
    'multiple_delimiter_precedes_last' => 'never',
    'multiple_el_al_min' => '3',
    'multiple_el_al_first' => '1',
  );
  $handler->display->display_options['fields']['contact_name']['group_column'] = 'entity_id';
  /* Field: COUNT(CRM Core Activity: Activity ID) */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['relationship'] = 'reverse_field_activity_participants_crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['group_type'] = 'count';
  $handler->display->display_options['fields']['activity_id']['label'] = 'Signatures';
  /* Sort criterion: COUNT(CRM Core Activity: Activity ID) */
  $handler->display->display_options['sorts']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['sorts']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['sorts']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['sorts']['activity_id']['relationship'] = 'reverse_field_activity_participants_crm_core_activity';
  $handler->display->display_options['sorts']['activity_id']['group_type'] = 'count';
  $handler->display->display_options['sorts']['activity_id']['order'] = 'DESC';
  /* Contextual filter: CRM Activity: Petition Source (field_cmcp_source) */
  $handler->display->display_options['arguments']['field_cmcp_source_value']['id'] = 'field_cmcp_source_value';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['table'] = 'field_data_field_cmcp_source';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['field'] = 'field_cmcp_source_value';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['relationship'] = 'reverse_field_activity_participants_crm_core_activity';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['limit'] = '0';
  $export['cmcp_source_summary_top_signatories'] = $view;

  $view = new view();
  $view->name = 'cmcp_source_summary_top_targets';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'crm_core_contact';
  $view->human_name = 'Petitions Source Summary Top Targets';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Top Targets';
  $handler->display->display_options['css_class'] = 'fancy-headers';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'contact_id' => 'contact_id',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'contact_id' => array(
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
  $handler->display->display_options['empty']['area']['content'] = '<div class="ao_no_results">No messages have been sent to targets through petitions using this source.</div>';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Relationship: Entity Reference: Referencing entity */
  $handler->display->display_options['relationships']['reverse_field_cmcp_petition_targets_crm_core_activity']['id'] = 'reverse_field_cmcp_petition_targets_crm_core_activity';
  $handler->display->display_options['relationships']['reverse_field_cmcp_petition_targets_crm_core_activity']['table'] = 'crm_core_contact';
  $handler->display->display_options['relationships']['reverse_field_cmcp_petition_targets_crm_core_activity']['field'] = 'reverse_field_cmcp_petition_targets_crm_core_activity';
  /* Field: CRM Contact: Contact ID */
  $handler->display->display_options['fields']['contact_id']['id'] = 'contact_id';
  $handler->display->display_options['fields']['contact_id']['table'] = 'crm_core_contact';
  $handler->display->display_options['fields']['contact_id']['field'] = 'contact_id';
  $handler->display->display_options['fields']['contact_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['contact_id']['link_to_contact'] = 0;
  /* Field: CRM Contact: URL */
  $handler->display->display_options['fields']['url']['id'] = 'url';
  $handler->display->display_options['fields']['url']['table'] = 'views_entity_crm_core_contact';
  $handler->display->display_options['fields']['url']['field'] = 'url';
  $handler->display->display_options['fields']['url']['exclude'] = TRUE;
  $handler->display->display_options['fields']['url']['display_as_link'] = FALSE;
  $handler->display->display_options['fields']['url']['link_to_entity'] = 0;
  /* Field: CRM Core Contact: Household */
  $handler->display->display_options['fields']['contact_name']['id'] = 'contact_name';
  $handler->display->display_options['fields']['contact_name']['table'] = 'field_data_contact_name';
  $handler->display->display_options['fields']['contact_name']['field'] = 'contact_name';
  $handler->display->display_options['fields']['contact_name']['label'] = 'Target';
  $handler->display->display_options['fields']['contact_name']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['contact_name']['alter']['path'] = '[url]';
  $handler->display->display_options['fields']['contact_name']['click_sort_column'] = 'title';
  $handler->display->display_options['fields']['contact_name']['settings'] = array(
    'format' => 'default',
    'markup' => 0,
    'output' => 'default',
    'multiple' => 'default',
    'multiple_delimiter' => ', ',
    'multiple_and' => 'text',
    'multiple_delimiter_precedes_last' => 'never',
    'multiple_el_al_min' => '3',
    'multiple_el_al_first' => '1',
  );
  $handler->display->display_options['fields']['contact_name']['group_column'] = 'entity_id';
  /* Field: COUNT(CRM Core Activity: Activity ID) */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['relationship'] = 'reverse_field_cmcp_petition_targets_crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['group_type'] = 'count';
  $handler->display->display_options['fields']['activity_id']['label'] = 'Signatures';
  $handler->display->display_options['fields']['activity_id']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['activity_id']['empty_zero'] = TRUE;
  /* Sort criterion: COUNT(CRM Core Activity: Activity ID) */
  $handler->display->display_options['sorts']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['sorts']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['sorts']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['sorts']['activity_id']['relationship'] = 'reverse_field_cmcp_petition_targets_crm_core_activity';
  $handler->display->display_options['sorts']['activity_id']['group_type'] = 'count';
  $handler->display->display_options['sorts']['activity_id']['order'] = 'DESC';
  /* Contextual filter: CRM Activity: Petition Source (field_cmcp_source) */
  $handler->display->display_options['arguments']['field_cmcp_source_value']['id'] = 'field_cmcp_source_value';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['table'] = 'field_data_field_cmcp_source';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['field'] = 'field_cmcp_source_value';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['relationship'] = 'reverse_field_cmcp_petition_targets_crm_core_activity';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_cmcp_source_value']['limit'] = '0';
  /* Filter criterion: COUNT(CRM Core Activity: Activity ID) */
  $handler->display->display_options['filters']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['filters']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['filters']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['filters']['activity_id']['relationship'] = 'reverse_field_cmcp_petition_targets_crm_core_activity';
  $handler->display->display_options['filters']['activity_id']['group_type'] = 'count';
  $handler->display->display_options['filters']['activity_id']['operator'] = '>';
  $handler->display->display_options['filters']['activity_id']['value']['value'] = '0';
  $export['cmcp_source_summary_top_targets'] = $view;

  $view = new view();
  $view->name = 'petition_page_performance_summary';
  $view->description = 'Provides some summary metrics for petition pages';
  $view->tag = 'default';
  $view->base_table = 'crm_core_activity';
  $view->human_name = 'Petition Page Performance Summary';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['css_class'] = 'summary-table';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'activity_id' => 'activity_id',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'activity_id' => array(
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
  $handler->display->display_options['empty']['area']['label'] = 'Empty results message';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No signatures have been received for this petition.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['empty']['area']['tokenize'] = TRUE;
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_cmcp_petition_page_target_id']['id'] = 'field_cmcp_petition_page_target_id';
  $handler->display->display_options['relationships']['field_cmcp_petition_page_target_id']['table'] = 'field_data_field_cmcp_petition_page';
  $handler->display->display_options['relationships']['field_cmcp_petition_page_target_id']['field'] = 'field_cmcp_petition_page_target_id';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_cmcp_active']['id'] = 'field_cmcp_active';
  $handler->display->display_options['fields']['field_cmcp_active']['table'] = 'field_data_field_cmcp_active';
  $handler->display->display_options['fields']['field_cmcp_active']['field'] = 'field_cmcp_active';
  $handler->display->display_options['fields']['field_cmcp_active']['relationship'] = 'field_cmcp_petition_page_target_id';
  $handler->display->display_options['fields']['field_cmcp_active']['label'] = 'Status';
  $handler->display->display_options['fields']['field_cmcp_active']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_cmcp_active']['alter']['text'] = 'Active';
  $handler->display->display_options['fields']['field_cmcp_active']['empty'] = 'Disabled';
  $handler->display->display_options['fields']['field_cmcp_active']['empty_zero'] = TRUE;
  /* Field: COUNT(CRM Core Activity: Activity ID) */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['group_type'] = 'count';
  $handler->display->display_options['fields']['activity_id']['label'] = 'Total Signatures';
  $handler->display->display_options['fields']['activity_id']['empty'] = '0';
  /* Field: MAX(CRM Activity: Date of Signature) */
  $handler->display->display_options['fields']['field_cmcp_received_date']['id'] = 'field_cmcp_received_date';
  $handler->display->display_options['fields']['field_cmcp_received_date']['table'] = 'field_data_field_cmcp_received_date';
  $handler->display->display_options['fields']['field_cmcp_received_date']['field'] = 'field_cmcp_received_date';
  $handler->display->display_options['fields']['field_cmcp_received_date']['group_type'] = 'max';
  $handler->display->display_options['fields']['field_cmcp_received_date']['label'] = 'Date of Last Signature';
  $handler->display->display_options['fields']['field_cmcp_received_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Contextual filter: CRM Activity: Petition (field_cmcp_petition_page) */
  $handler->display->display_options['arguments']['field_cmcp_petition_page_target_id']['id'] = 'field_cmcp_petition_page_target_id';
  $handler->display->display_options['arguments']['field_cmcp_petition_page_target_id']['table'] = 'field_data_field_cmcp_petition_page';
  $handler->display->display_options['arguments']['field_cmcp_petition_page_target_id']['field'] = 'field_cmcp_petition_page_target_id';
  $handler->display->display_options['arguments']['field_cmcp_petition_page_target_id']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['field_cmcp_petition_page_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_cmcp_petition_page_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_cmcp_petition_page_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_cmcp_petition_page_target_id']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: CRM Core Activity: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'crm_core_activity';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'cmcp_petition_signature' => 'cmcp_petition_signature',
  );

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['pane_title'] = 'CRM Core Petition: Page Summary';
  $handler->display->display_options['pane_description'] = 'Basic metrics around petition page performance';
  $handler->display->display_options['pane_category']['name'] = 'CRM Core Petition';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $handler->display->display_options['argument_input'] = array(
    'field_cmcp_petition_page_target_id' => array(
      'type' => 'panel',
      'context' => 'entity:crm_core_activity.activity-id',
      'context_optional' => 0,
      'panel' => '0',
      'fixed' => '',
      'label' => 'CRM Activity: Online Petition Page (field_cmcp_petition_page)',
    ),
  );
  $export['petition_page_performance_summary'] = $view;

  $view = new view();
  $view->name = 'petition_page_report';
  $view->description = 'Provides details about the performance of individual petition pages';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Petition Page Report';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['css_class'] = 'fancy-headers';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view petitions report';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'activity_id' => 'activity_id',
    'field_cmcp_received_date' => 'field_cmcp_received_date',
  );
  $handler->display->display_options['style_options']['default'] = 'title';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'activity_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcp_received_date' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Entity Reference: Referencing entity */
  $handler->display->display_options['relationships']['reverse_field_cmcp_petition_page_crm_core_activity']['id'] = 'reverse_field_cmcp_petition_page_crm_core_activity';
  $handler->display->display_options['relationships']['reverse_field_cmcp_petition_page_crm_core_activity']['table'] = 'node';
  $handler->display->display_options['relationships']['reverse_field_cmcp_petition_page_crm_core_activity']['field'] = 'reverse_field_cmcp_petition_page_crm_core_activity';
  $handler->display->display_options['relationships']['reverse_field_cmcp_petition_page_crm_core_activity']['label'] = 'Petition activity relationship';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Petition';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_class'] = 'field-object';
  /* Field: COUNT(CRM Core Activity: Activity ID) */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['relationship'] = 'reverse_field_cmcp_petition_page_crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['group_type'] = 'count';
  $handler->display->display_options['fields']['activity_id']['label'] = 'Signatures';
  /* Field: MAX(CRM Activity: Date of Signature) */
  $handler->display->display_options['fields']['field_cmcp_received_date']['id'] = 'field_cmcp_received_date';
  $handler->display->display_options['fields']['field_cmcp_received_date']['table'] = 'field_data_field_cmcp_received_date';
  $handler->display->display_options['fields']['field_cmcp_received_date']['field'] = 'field_cmcp_received_date';
  $handler->display->display_options['fields']['field_cmcp_received_date']['relationship'] = 'reverse_field_cmcp_petition_page_crm_core_activity';
  $handler->display->display_options['fields']['field_cmcp_received_date']['group_type'] = 'max';
  $handler->display->display_options['fields']['field_cmcp_received_date']['label'] = 'Last Signature Date';
  $handler->display->display_options['fields']['field_cmcp_received_date']['empty'] = '---';
  $handler->display->display_options['fields']['field_cmcp_received_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Summary Link';
  $handler->display->display_options['fields']['nothing']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['text'] = 'Summary';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['path'] = 'crm-core/reports/cmcp/petition-report/summary/[nid]';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
  $handler->display->display_options['fields']['nothing_2']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_2']['label'] = 'Details Link';
  $handler->display->display_options['fields']['nothing_2']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['text'] = 'Details';
  $handler->display->display_options['fields']['nothing_2']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['path'] = 'crm-core/reports/cmcp/petition-signatories?title=[title]';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'Reports';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = '<div class="btn btn-blue btn-dark btn-simple">[nothing]</div>
<div class="btn btn-blue btn-dark btn-simple">[nothing_2]</div>';
  $handler->display->display_options['fields']['nothing_1']['element_class'] = 'field-operations';
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
    'cmcp_petition' => 'cmcp_petition',
  );

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['pane_title'] = 'Petition: Petition Pages Table';
  $handler->display->display_options['pane_description'] = 'Describes performance of individual petition pages';
  $handler->display->display_options['pane_category']['name'] = 'CRM Core Petition';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $export['petition_page_report'] = $view;

  $view = new view();
  $view->name = 'petition_source_report';
  $view->description = 'Details about signatures received from different sources';
  $view->tag = 'default';
  $view->base_table = 'crm_core_activity';
  $view->human_name = 'Petition Source Report';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['css_class'] = 'fancy-headers';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view petition sources report';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_cmcp_source' => 'field_cmcp_source',
    'activity_id' => 'activity_id',
    'field_cmcp_received_date' => 'field_cmcp_received_date',
  );
  $handler->display->display_options['style_options']['default'] = 'activity_id';
  $handler->display->display_options['style_options']['info'] = array(
    'field_cmcp_source' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'activity_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcp_received_date' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: COUNT(DISTINCT CRM Activity: Petition) */
  $handler->display->display_options['fields']['field_cmcp_petition_page']['id'] = 'field_cmcp_petition_page';
  $handler->display->display_options['fields']['field_cmcp_petition_page']['table'] = 'field_data_field_cmcp_petition_page';
  $handler->display->display_options['fields']['field_cmcp_petition_page']['field'] = 'field_cmcp_petition_page';
  $handler->display->display_options['fields']['field_cmcp_petition_page']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['field_cmcp_petition_page']['label'] = '';
  $handler->display->display_options['fields']['field_cmcp_petition_page']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_cmcp_petition_page']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_cmcp_petition_page']['separator'] = '';
  /* Field: CRM Activity: Petition Source */
  $handler->display->display_options['fields']['field_cmcp_source']['id'] = 'field_cmcp_source';
  $handler->display->display_options['fields']['field_cmcp_source']['table'] = 'field_data_field_cmcp_source';
  $handler->display->display_options['fields']['field_cmcp_source']['field'] = 'field_cmcp_source';
  $handler->display->display_options['fields']['field_cmcp_source']['label'] = 'Title';
  $handler->display->display_options['fields']['field_cmcp_source']['element_class'] = 'field-object';
  /* Field: COUNT(CRM Core Activity: Activity ID) */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['group_type'] = 'count';
  $handler->display->display_options['fields']['activity_id']['label'] = 'Total Signatures';
  /* Field: MAX(CRM Activity: Date of Signature) */
  $handler->display->display_options['fields']['field_cmcp_received_date']['id'] = 'field_cmcp_received_date';
  $handler->display->display_options['fields']['field_cmcp_received_date']['table'] = 'field_data_field_cmcp_received_date';
  $handler->display->display_options['fields']['field_cmcp_received_date']['field'] = 'field_cmcp_received_date';
  $handler->display->display_options['fields']['field_cmcp_received_date']['group_type'] = 'max';
  $handler->display->display_options['fields']['field_cmcp_received_date']['label'] = 'Last Signature Date';
  $handler->display->display_options['fields']['field_cmcp_received_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Global: Math expression */
  $handler->display->display_options['fields']['expression']['id'] = 'expression';
  $handler->display->display_options['fields']['expression']['table'] = 'views';
  $handler->display->display_options['fields']['expression']['field'] = 'expression';
  $handler->display->display_options['fields']['expression']['label'] = 'Average Signatures Per Petition';
  $handler->display->display_options['fields']['expression']['set_precision'] = TRUE;
  $handler->display->display_options['fields']['expression']['precision'] = '2';
  $handler->display->display_options['fields']['expression']['expression'] = '[activity_id] / [field_cmcp_petition_page]';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'Details Link';
  $handler->display->display_options['fields']['nothing_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'Details';
  $handler->display->display_options['fields']['nothing_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['path'] = 'crm-core/reports/cmcp/petition-signatories?field_cmcp_source_value[]=[field_cmcp_source]';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
  $handler->display->display_options['fields']['nothing_2']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_2']['label'] = 'Summary Link';
  $handler->display->display_options['fields']['nothing_2']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['text'] = 'Summary';
  $handler->display->display_options['fields']['nothing_2']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['path'] = 'crm-core/reports/cmcp/sources-report/summary/[field_cmcp_source]';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Reports';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<div class="btn btn-blue btn-dark btn-simple">[nothing_2]</div>
<div class="btn btn-blue btn-dark btn-simple">[nothing_1]</div>';
  $handler->display->display_options['fields']['nothing']['element_class'] = 'field-operations';
  /* Filter criterion: CRM Core Activity: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'crm_core_activity';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'cmcp_petition_signature' => 'cmcp_petition_signature',
  );

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['pane_title'] = 'Petition: Sources Table';
  $handler->display->display_options['pane_description'] = 'Provides a list of petition signature sources, along with some basic metrics.';
  $handler->display->display_options['pane_category']['name'] = 'CRM Core Petition';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $export['petition_source_report'] = $view;

  $view = new view();
  $view->name = 'petition_top_sources';
  $view->description = 'A table displaying the top sources for petition signatures';
  $view->tag = 'default';
  $view->base_table = 'crm_core_activity';
  $view->human_name = 'Petition Top Sources';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Top Sources';
  $handler->display->display_options['css_class'] = 'fancy-headers';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view petition overview';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'activity_id' => 'activity_id',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'activity_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: CRM Activity: Petition Source */
  $handler->display->display_options['fields']['field_cmcp_source']['id'] = 'field_cmcp_source';
  $handler->display->display_options['fields']['field_cmcp_source']['table'] = 'field_data_field_cmcp_source';
  $handler->display->display_options['fields']['field_cmcp_source']['field'] = 'field_cmcp_source';
  $handler->display->display_options['fields']['field_cmcp_source']['label'] = 'Source';
  $handler->display->display_options['fields']['field_cmcp_source']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_cmcp_source']['alter']['path'] = 'crm-core/reports/cmcp/petition-signatories?field_cmcp_source_value[]=[field_cmcp_source]';
  /* Field: COUNT(CRM Core Activity: Activity ID) */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['group_type'] = 'count';
  $handler->display->display_options['fields']['activity_id']['label'] = 'Signatures';
  /* Filter criterion: CRM Core Activity: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'crm_core_activity';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'cmcp_petition_signature' => 'cmcp_petition_signature',
  );

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['pane_title'] = 'Petition: Top Sources';
  $handler->display->display_options['pane_category']['name'] = 'CRM Core Petition';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $export['petition_top_sources'] = $view;

  return $export;
}
