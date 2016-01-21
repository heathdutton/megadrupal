<?php
/**
 * @file
 * crm_core_event.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function crm_core_event_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'cmcev_attendees_report';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'crm_core_activity';
  $view->human_name = 'CRM Core Event: Attendees Report';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Attendees';
  $handler->display->display_options['css_class'] = 'fancy-headers';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view attendees report';
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
    'activity_id' => 'activity_id',
    'field_cmcev_event' => 'field_cmcev_event',
    'field_cmcev_reg_date' => 'field_cmcev_reg_date',
    'field_cmcev_source' => 'field_cmcev_source',
    'field_cmcev_status' => 'field_cmcev_status',
    'contact_name' => 'contact_name',
    'url' => 'url',
  );
  $handler->display->display_options['style_options']['default'] = 'field_cmcev_reg_date';
  $handler->display->display_options['style_options']['info'] = array(
    'activity_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcev_event' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcev_reg_date' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcev_source' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcev_status' => array(
      'sortable' => 1,
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
    'url' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  $handler->display->display_options['header']['result']['content'] = 'Displaying @start - @end of @total.';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '[nothing_5] | [nothing_3]';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
  $handler->display->display_options['header']['area']['tokenize'] = TRUE;
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['id'] = 'field_activity_participants_target_id';
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['table'] = 'field_data_field_activity_participants';
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['field'] = 'field_activity_participants_target_id';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_cmcev_event_target_id']['id'] = 'field_cmcev_event_target_id';
  $handler->display->display_options['relationships']['field_cmcev_event_target_id']['table'] = 'field_data_field_cmcev_event';
  $handler->display->display_options['relationships']['field_cmcev_event_target_id']['field'] = 'field_cmcev_event_target_id';
  /* Field: Bulk operations: CRM Core Activity */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'rules_component::rules_cmcv_cancel_registration' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::crm_core_volunteer_change_status_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::rules_cmcev_cancel_registration' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 1,
      'override_label' => 1,
      'label' => 'Cancel registration',
    ),
    'rules_component::rules_cmcev_send_email' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 1,
      'override_label' => 1,
      'label' => 'Send email',
    ),
    'action::views_bulk_operations_delete_item' => array(
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
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 1,
      'override_label' => 1,
      'label' => 'Change status',
      'settings' => array(
        'show_all_tokens' => 0,
        'display_values' => array(
          'cmcev_event_registration::field_cmcev_status' => 'cmcev_event_registration::field_cmcev_status',
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
    'rules_component::rules_rules_cmcv_send_email' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
  /* Field: CRM Core Activity: Activity ID */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['exclude'] = TRUE;
  /* Field: CRM Contact: URL */
  $handler->display->display_options['fields']['url']['id'] = 'url';
  $handler->display->display_options['fields']['url']['table'] = 'views_entity_crm_core_contact';
  $handler->display->display_options['fields']['url']['field'] = 'url';
  $handler->display->display_options['fields']['url']['relationship'] = 'field_activity_participants_target_id';
  $handler->display->display_options['fields']['url']['exclude'] = TRUE;
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
  /* Field: CRM Activity: Event */
  $handler->display->display_options['fields']['field_cmcev_event']['id'] = 'field_cmcev_event';
  $handler->display->display_options['fields']['field_cmcev_event']['table'] = 'field_data_field_cmcev_event';
  $handler->display->display_options['fields']['field_cmcev_event']['field'] = 'field_cmcev_event';
  $handler->display->display_options['fields']['field_cmcev_event']['settings'] = array(
    'link' => 1,
  );
  /* Field: CRM Activity: Status */
  $handler->display->display_options['fields']['field_cmcev_status']['id'] = 'field_cmcev_status';
  $handler->display->display_options['fields']['field_cmcev_status']['table'] = 'field_data_field_cmcev_status';
  $handler->display->display_options['fields']['field_cmcev_status']['field'] = 'field_cmcev_status';
  /* Field: CRM Activity: Source */
  $handler->display->display_options['fields']['field_cmcev_source']['id'] = 'field_cmcev_source';
  $handler->display->display_options['fields']['field_cmcev_source']['table'] = 'field_data_field_cmcev_source';
  $handler->display->display_options['fields']['field_cmcev_source']['field'] = 'field_cmcev_source';
  /* Field: CRM Activity: Registration Date */
  $handler->display->display_options['fields']['field_cmcev_reg_date']['id'] = 'field_cmcev_reg_date';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['table'] = 'field_data_field_cmcev_reg_date';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['field'] = 'field_cmcev_reg_date';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'View Link';
  $handler->display->display_options['fields']['nothing_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'View';
  $handler->display->display_options['fields']['nothing_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['path'] = '[url]/activity/[activity_id]';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
  $handler->display->display_options['fields']['nothing_2']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_2']['label'] = 'Edit Link';
  $handler->display->display_options['fields']['nothing_2']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['text'] = 'Edit';
  $handler->display->display_options['fields']['nothing_2']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['path'] = '[url]/activity/[activity_id]/edit';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Operations';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<div class="btn btn-blue btn-dark btn-simple">[nothing_1]</div> <div class="btn btn-green btn-dark btn-simple">[nothing_2]</div>';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_3']['id'] = 'nothing_3';
  $handler->display->display_options['fields']['nothing_3']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_3']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_3']['label'] = 'View paged results';
  $handler->display->display_options['fields']['nothing_3']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_3']['alter']['text'] = 'View paged results';
  $handler->display->display_options['fields']['nothing_3']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_3']['alter']['path'] = 'crm-core/reports/cmcev/attendees';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_4']['id'] = 'nothing_4';
  $handler->display->display_options['fields']['nothing_4']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_4']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_4']['label'] = 'View All';
  $handler->display->display_options['fields']['nothing_4']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_4']['alter']['text'] = 'View All';
  $handler->display->display_options['fields']['nothing_4']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_4']['alter']['path'] = 'crm-core/reports/cmcev/attendees/all';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_5']['id'] = 'nothing_5';
  $handler->display->display_options['fields']['nothing_5']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_5']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_5']['label'] = 'Export to CSV';
  $handler->display->display_options['fields']['nothing_5']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_5']['alter']['text'] = 'Export to CSV';
  $handler->display->display_options['fields']['nothing_5']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_5']['alter']['path'] = 'crm-core/reports/cmcev/attendees/export';
  /* Filter criterion: CRM Core Activity: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'crm_core_activity';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'cmcev_event_registration' => 'cmcev_event_registration',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: CRM Core Contact: Household */
  $handler->display->display_options['filters']['contact_name']['id'] = 'contact_name';
  $handler->display->display_options['filters']['contact_name']['table'] = 'field_data_contact_name';
  $handler->display->display_options['filters']['contact_name']['field'] = 'contact_name';
  $handler->display->display_options['filters']['contact_name']['relationship'] = 'field_activity_participants_target_id';
  $handler->display->display_options['filters']['contact_name']['group'] = 1;
  $handler->display->display_options['filters']['contact_name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['contact_name']['expose']['operator_id'] = 'contact_name_op';
  $handler->display->display_options['filters']['contact_name']['expose']['label'] = 'Filter by name:';
  $handler->display->display_options['filters']['contact_name']['expose']['operator'] = 'contact_name_op';
  $handler->display->display_options['filters']['contact_name']['expose']['identifier'] = 'contact_name';
  $handler->display->display_options['filters']['contact_name']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['relationship'] = 'field_cmcev_event_target_id';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Filter by event:';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'event_title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: CRM Activity: Source (field_cmcev_source) */
  $handler->display->display_options['filters']['field_cmcev_source_value']['id'] = 'field_cmcev_source_value';
  $handler->display->display_options['filters']['field_cmcev_source_value']['table'] = 'field_data_field_cmcev_source';
  $handler->display->display_options['filters']['field_cmcev_source_value']['field'] = 'field_cmcev_source_value';
  $handler->display->display_options['filters']['field_cmcev_source_value']['group'] = 1;
  $handler->display->display_options['filters']['field_cmcev_source_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_cmcev_source_value']['expose']['operator_id'] = 'field_cmcev_source_value_op';
  $handler->display->display_options['filters']['field_cmcev_source_value']['expose']['label'] = 'Filter by source:';
  $handler->display->display_options['filters']['field_cmcev_source_value']['expose']['operator'] = 'field_cmcev_source_value_op';
  $handler->display->display_options['filters']['field_cmcev_source_value']['expose']['identifier'] = 'source';
  $handler->display->display_options['filters']['field_cmcev_source_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: CRM Activity: Registration Date (field_cmcev_reg_date) */
  $handler->display->display_options['filters']['field_cmcev_reg_date_value']['id'] = 'field_cmcev_reg_date_value';
  $handler->display->display_options['filters']['field_cmcev_reg_date_value']['table'] = 'field_data_field_cmcev_reg_date';
  $handler->display->display_options['filters']['field_cmcev_reg_date_value']['field'] = 'field_cmcev_reg_date_value';
  $handler->display->display_options['filters']['field_cmcev_reg_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_cmcev_reg_date_value']['group'] = 1;
  $handler->display->display_options['filters']['field_cmcev_reg_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_cmcev_reg_date_value']['expose']['operator_id'] = 'field_cmcev_reg_date_value_op';
  $handler->display->display_options['filters']['field_cmcev_reg_date_value']['expose']['label'] = 'Filter by dates between:';
  $handler->display->display_options['filters']['field_cmcev_reg_date_value']['expose']['operator'] = 'field_cmcev_reg_date_value_op';
  $handler->display->display_options['filters']['field_cmcev_reg_date_value']['expose']['identifier'] = 'field_cmcev_reg_date_value';
  $handler->display->display_options['filters']['field_cmcev_reg_date_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_cmcev_reg_date_value']['form_type'] = 'date_popup';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  $handler->display->display_options['header']['result']['content'] = '<div class="results-summary">Displaying @start - @end of @total.</div>';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '<div class="detail-buttons"><div class="btn btn-blue btn-simple">[nothing_4]</div><div class="btn btn-orange btn-simple">[nothing_5]</div></div>';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  $handler->display->display_options['header']['area']['tokenize'] = TRUE;
  $handler->display->display_options['path'] = 'crm-core/reports/cmcev/attendees';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Attendees';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'features';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Page All */
  $handler = $view->new_display('page', 'Page All', 'page_all');
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  $handler->display->display_options['header']['result']['content'] = 'Displaying @start - @end of @total.';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '[nothing_5] | [nothing_3]';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
  $handler->display->display_options['header']['area']['tokenize'] = TRUE;
  $handler->display->display_options['path'] = 'crm-core/reports/cmcev/attendees/all';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Attendees All';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Bulk operations: CRM Core Activity */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'rules_component::rules_cmcv_cancel_registration' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::crm_core_volunteer_change_status_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::rules_cmcev_cancel_registration' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 1,
      'override_label' => 1,
      'label' => 'Cancel registration',
    ),
    'rules_component::rules_cmcev_send_email' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 1,
      'override_label' => 1,
      'label' => 'Send email',
    ),
    'action::views_bulk_operations_delete_item' => array(
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
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 1,
      'override_label' => 1,
      'label' => 'Change status',
      'settings' => array(
        'show_all_tokens' => 0,
        'display_values' => array(
          'cmcev_event_registration::field_cmcev_status' => 'cmcev_event_registration::field_cmcev_status',
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
    'rules_component::rules_rules_cmcv_send_email' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
  /* Field: CRM Core Activity: Activity ID */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['exclude'] = TRUE;
  /* Field: CRM Contact: URL */
  $handler->display->display_options['fields']['url']['id'] = 'url';
  $handler->display->display_options['fields']['url']['table'] = 'views_entity_crm_core_contact';
  $handler->display->display_options['fields']['url']['field'] = 'url';
  $handler->display->display_options['fields']['url']['relationship'] = 'field_activity_participants_target_id';
  $handler->display->display_options['fields']['url']['exclude'] = TRUE;
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
  /* Field: CRM Activity: Event */
  $handler->display->display_options['fields']['field_cmcev_event']['id'] = 'field_cmcev_event';
  $handler->display->display_options['fields']['field_cmcev_event']['table'] = 'field_data_field_cmcev_event';
  $handler->display->display_options['fields']['field_cmcev_event']['field'] = 'field_cmcev_event';
  $handler->display->display_options['fields']['field_cmcev_event']['settings'] = array(
    'link' => 1,
  );
  /* Field: CRM Activity: Status */
  $handler->display->display_options['fields']['field_cmcev_status']['id'] = 'field_cmcev_status';
  $handler->display->display_options['fields']['field_cmcev_status']['table'] = 'field_data_field_cmcev_status';
  $handler->display->display_options['fields']['field_cmcev_status']['field'] = 'field_cmcev_status';
  /* Field: CRM Activity: Source */
  $handler->display->display_options['fields']['field_cmcev_source']['id'] = 'field_cmcev_source';
  $handler->display->display_options['fields']['field_cmcev_source']['table'] = 'field_data_field_cmcev_source';
  $handler->display->display_options['fields']['field_cmcev_source']['field'] = 'field_cmcev_source';
  /* Field: CRM Activity: Registration Date */
  $handler->display->display_options['fields']['field_cmcev_reg_date']['id'] = 'field_cmcev_reg_date';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['table'] = 'field_data_field_cmcev_reg_date';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['field'] = 'field_cmcev_reg_date';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'View Link';
  $handler->display->display_options['fields']['nothing_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'View';
  $handler->display->display_options['fields']['nothing_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['path'] = '[url]/activity/[activity_id]';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
  $handler->display->display_options['fields']['nothing_2']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_2']['label'] = 'Edit Link';
  $handler->display->display_options['fields']['nothing_2']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['text'] = 'Edit';
  $handler->display->display_options['fields']['nothing_2']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['path'] = '[url]/activity/[activity_id]/edit';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Operations';
  $handler->display->display_options['fields']['nothing']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[nothing_1] | [nothing_2]';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_3']['id'] = 'nothing_3';
  $handler->display->display_options['fields']['nothing_3']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_3']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_3']['label'] = 'View paged results';
  $handler->display->display_options['fields']['nothing_3']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_3']['alter']['text'] = 'View paged results';
  $handler->display->display_options['fields']['nothing_3']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_3']['alter']['path'] = 'crm-core/reports/cmcev/attendees';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_4']['id'] = 'nothing_4';
  $handler->display->display_options['fields']['nothing_4']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_4']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_4']['label'] = 'View All';
  $handler->display->display_options['fields']['nothing_4']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_4']['alter']['text'] = 'View All';
  $handler->display->display_options['fields']['nothing_4']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_4']['alter']['path'] = 'crm-core/reports/cmcev/attendees/all';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_5']['id'] = 'nothing_5';
  $handler->display->display_options['fields']['nothing_5']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_5']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_5']['label'] = 'Export to CSV';
  $handler->display->display_options['fields']['nothing_5']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_5']['alter']['text'] = 'Export to CSV';
  $handler->display->display_options['fields']['nothing_5']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_5']['alter']['path'] = 'crm-core/reports/cmcev/attendees/export';
  $handler->display->display_options['path'] = 'crm-core/reports/cmcev/attendees/export';
  $export['cmcev_attendees_report'] = $view;

  $view = new view();
  $view->name = 'cmcev_event_summary_attendees';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'crm_core_activity';
  $view->human_name = 'CRM Core Event: Event summary attendees table';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['css_class'] = 'fancy-headers';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
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
    'activity_id' => 'activity_id',
    'url' => 'url',
    'contact_name' => 'contact_name',
    'field_cmcev_status' => 'field_cmcev_status',
    'field_cmcev_source' => 'field_cmcev_source',
    'field_cmcev_reg_date' => 'field_cmcev_reg_date',
    'nothing_1' => 'nothing_1',
    'nothing_2' => 'nothing_2',
    'nothing' => 'nothing',
  );
  $handler->display->display_options['style_options']['default'] = 'field_cmcev_reg_date';
  $handler->display->display_options['style_options']['info'] = array(
    'activity_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
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
    'field_cmcev_status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcev_source' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcev_reg_date' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
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
  );
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No results found.';
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
  /* Field: CRM Contact: URL */
  $handler->display->display_options['fields']['url']['id'] = 'url';
  $handler->display->display_options['fields']['url']['table'] = 'views_entity_crm_core_contact';
  $handler->display->display_options['fields']['url']['field'] = 'url';
  $handler->display->display_options['fields']['url']['relationship'] = 'field_activity_participants_target_id';
  $handler->display->display_options['fields']['url']['exclude'] = TRUE;
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
  /* Field: CRM Activity: Status */
  $handler->display->display_options['fields']['field_cmcev_status']['id'] = 'field_cmcev_status';
  $handler->display->display_options['fields']['field_cmcev_status']['table'] = 'field_data_field_cmcev_status';
  $handler->display->display_options['fields']['field_cmcev_status']['field'] = 'field_cmcev_status';
  /* Field: CRM Activity: Source */
  $handler->display->display_options['fields']['field_cmcev_source']['id'] = 'field_cmcev_source';
  $handler->display->display_options['fields']['field_cmcev_source']['table'] = 'field_data_field_cmcev_source';
  $handler->display->display_options['fields']['field_cmcev_source']['field'] = 'field_cmcev_source';
  $handler->display->display_options['fields']['field_cmcev_source']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_cmcev_source']['alter']['path'] = 'crm-core/reports/cmcev/sources/summary/[field_cmcev_source]';
  /* Field: CRM Activity: Registration Date */
  $handler->display->display_options['fields']['field_cmcev_reg_date']['id'] = 'field_cmcev_reg_date';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['table'] = 'field_data_field_cmcev_reg_date';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['field'] = 'field_cmcev_reg_date';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'View Link';
  $handler->display->display_options['fields']['nothing_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'View';
  $handler->display->display_options['fields']['nothing_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['path'] = '[url]/activity/[activity_id]';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
  $handler->display->display_options['fields']['nothing_2']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_2']['label'] = 'Edit Link';
  $handler->display->display_options['fields']['nothing_2']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['text'] = 'Edit';
  $handler->display->display_options['fields']['nothing_2']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['path'] = '[url]/activity/[activity_id]/edit';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Operations';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<div class="btn btn-blue btn-dark btn-simple">[nothing_1]</div> <div class="btn btn-orange btn-dark btn-simple">[nothing_2]</div>';
  /* Contextual filter: CRM Activity: Event (field_cmcev_event) */
  $handler->display->display_options['arguments']['field_cmcev_event_target_id']['id'] = 'field_cmcev_event_target_id';
  $handler->display->display_options['arguments']['field_cmcev_event_target_id']['table'] = 'field_data_field_cmcev_event';
  $handler->display->display_options['arguments']['field_cmcev_event_target_id']['field'] = 'field_cmcev_event_target_id';
  $handler->display->display_options['arguments']['field_cmcev_event_target_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['field_cmcev_event_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_cmcev_event_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_cmcev_event_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_cmcev_event_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_cmcev_event_target_id']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['field_cmcev_event_target_id']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['field_cmcev_event_target_id']['validate_options']['types'] = array(
    'cmcev_event' => 'cmcev_event',
  );
  $handler->display->display_options['arguments']['field_cmcev_event_target_id']['validate']['fail'] = 'empty';
  /* Filter criterion: CRM Core Activity: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'crm_core_activity';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'cmcev_event_registration' => 'cmcev_event_registration',
  );

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['pane_title'] = 'Events summary report attendees';
  $handler->display->display_options['pane_category']['name'] = 'CRM Core Event';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $handler->display->display_options['argument_input'] = array(
    'field_cmcev_event_target_id' => array(
      'type' => 'panel',
      'context' => 'entity:crm_core_activity.activity-id',
      'context_optional' => 0,
      'panel' => '0',
      'fixed' => '',
      'label' => 'CRM Activity: Event (field_cmcev_event)',
    ),
  );
  $export['cmcev_event_summary_attendees'] = $view;

  $view = new view();
  $view->name = 'cmcev_event_summary_metrics';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'crm_core_activity';
  $view->human_name = 'CRM Core Event: Event summary metrics';
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
  /* Field: COUNT(CRM Core Activity: Activity ID) */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['group_type'] = 'count';
  $handler->display->display_options['fields']['activity_id']['label'] = 'Total registrations';
  $handler->display->display_options['fields']['activity_id']['separator'] = '';
  /* Field: SUM(CRM Activity: Amount) */
  $handler->display->display_options['fields']['field_cmcev_amount']['id'] = 'field_cmcev_amount';
  $handler->display->display_options['fields']['field_cmcev_amount']['table'] = 'field_data_field_cmcev_amount';
  $handler->display->display_options['fields']['field_cmcev_amount']['field'] = 'field_cmcev_amount';
  $handler->display->display_options['fields']['field_cmcev_amount']['group_type'] = 'sum';
  $handler->display->display_options['fields']['field_cmcev_amount']['label'] = 'Total raised';
  $handler->display->display_options['fields']['field_cmcev_amount']['type'] = 'crm_core_event_amount';
  $handler->display->display_options['fields']['field_cmcev_amount']['settings'] = array(
    'thousand_separator' => '',
    'decimal_separator' => '.',
    'scale' => '0',
    'prefix_suffix' => 1,
  );
  /* Field: Global: Math expression */
  $handler->display->display_options['fields']['expression']['id'] = 'expression';
  $handler->display->display_options['fields']['expression']['table'] = 'views';
  $handler->display->display_options['fields']['expression']['field'] = 'expression';
  $handler->display->display_options['fields']['expression']['label'] = 'Average registration amount';
  $handler->display->display_options['fields']['expression']['set_precision'] = TRUE;
  $handler->display->display_options['fields']['expression']['precision'] = '0';
  $handler->display->display_options['fields']['expression']['separator'] = '';
  $handler->display->display_options['fields']['expression']['expression'] = '[field_cmcev_amount]/[activity_id]';
  /* Contextual filter: CRM Activity: Event (field_cmcev_event) */
  $handler->display->display_options['arguments']['field_cmcev_event_target_id']['id'] = 'field_cmcev_event_target_id';
  $handler->display->display_options['arguments']['field_cmcev_event_target_id']['table'] = 'field_data_field_cmcev_event';
  $handler->display->display_options['arguments']['field_cmcev_event_target_id']['field'] = 'field_cmcev_event_target_id';
  $handler->display->display_options['arguments']['field_cmcev_event_target_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['field_cmcev_event_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_cmcev_event_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_cmcev_event_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_cmcev_event_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_cmcev_event_target_id']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['field_cmcev_event_target_id']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['field_cmcev_event_target_id']['validate_options']['types'] = array(
    'cmcev_event' => 'cmcev_event',
  );
  $handler->display->display_options['arguments']['field_cmcev_event_target_id']['validate']['fail'] = 'empty';
  /* Filter criterion: CRM Core Activity: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'crm_core_activity';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'cmcev_event_registration' => 'cmcev_event_registration',
  );

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['pane_title'] = 'Events summary report metrics';
  $handler->display->display_options['pane_category']['name'] = 'CRM Core Event';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $handler->display->display_options['argument_input'] = array(
    'field_cmcev_event_target_id' => array(
      'type' => 'panel',
      'context' => 'entity:crm_core_activity.activity-id',
      'context_optional' => 0,
      'panel' => '0',
      'fixed' => '',
      'label' => 'CRM Activity: Event (field_cmcev_event)',
    ),
  );
  $export['cmcev_event_summary_metrics'] = $view;

  $view = new view();
  $view->name = 'cmcev_events_report';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'CRM Core Event: Events Report';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Events';
  $handler->display->display_options['css_class'] = 'fancy-headers';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view event overview';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'field_cmcev_date' => 'field_cmcev_date',
    'field_cmcev_amount' => 'field_cmcev_amount',
    'activity_id' => 'activity_id',
    'field_cmcev_reg_date' => 'field_cmcev_reg_date',
    'nothing_1' => 'nothing_1',
    'nothing_2' => 'nothing_2',
    'nothing' => 'nothing',
  );
  $handler->display->display_options['style_options']['default'] = 'field_cmcev_date';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcev_date' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcev_amount' => array(
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
    'field_cmcev_reg_date' => array(
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
  );
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  $handler->display->display_options['header']['result']['content'] = '<div class="results-summary">Displaying @start - @end of @total.</div>';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '<div class="detail-buttons"><div class="btn btn-blue btn-simple">[nothing_4]</div> <div class="btn btn-orange btn-simple">[nothing_5]</div></div>';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  $handler->display->display_options['header']['area']['tokenize'] = TRUE;
  /* Relationship: Entity Reference: Referencing entity */
  $handler->display->display_options['relationships']['reverse_field_cmcev_event_crm_core_activity']['id'] = 'reverse_field_cmcev_event_crm_core_activity';
  $handler->display->display_options['relationships']['reverse_field_cmcev_event_crm_core_activity']['table'] = 'node';
  $handler->display->display_options['relationships']['reverse_field_cmcev_event_crm_core_activity']['field'] = 'reverse_field_cmcev_event_crm_core_activity';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Date */
  $handler->display->display_options['fields']['field_cmcev_date']['id'] = 'field_cmcev_date';
  $handler->display->display_options['fields']['field_cmcev_date']['table'] = 'field_data_field_cmcev_date';
  $handler->display->display_options['fields']['field_cmcev_date']['field'] = 'field_cmcev_date';
  $handler->display->display_options['fields']['field_cmcev_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: SUM(CRM Activity: Amount) */
  $handler->display->display_options['fields']['field_cmcev_amount']['id'] = 'field_cmcev_amount';
  $handler->display->display_options['fields']['field_cmcev_amount']['table'] = 'field_data_field_cmcev_amount';
  $handler->display->display_options['fields']['field_cmcev_amount']['field'] = 'field_cmcev_amount';
  $handler->display->display_options['fields']['field_cmcev_amount']['relationship'] = 'reverse_field_cmcev_event_crm_core_activity';
  $handler->display->display_options['fields']['field_cmcev_amount']['group_type'] = 'sum';
  $handler->display->display_options['fields']['field_cmcev_amount']['label'] = 'Amount raised';
  $handler->display->display_options['fields']['field_cmcev_amount']['type'] = 'crm_core_event_amount';
  $handler->display->display_options['fields']['field_cmcev_amount']['settings'] = array(
    'thousand_separator' => ' ',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: COUNT(DISTINCT CRM Core Activity: Activity ID) */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['relationship'] = 'reverse_field_cmcev_event_crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['activity_id']['label'] = 'Registered';
  /* Field: MAX(CRM Activity: Registration Date) */
  $handler->display->display_options['fields']['field_cmcev_reg_date']['id'] = 'field_cmcev_reg_date';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['table'] = 'field_data_field_cmcev_reg_date';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['field'] = 'field_cmcev_reg_date';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['relationship'] = 'reverse_field_cmcev_event_crm_core_activity';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['group_type'] = 'max';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['label'] = 'Last Registration Date';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'Summary Link';
  $handler->display->display_options['fields']['nothing_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'Summary';
  $handler->display->display_options['fields']['nothing_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['path'] = 'crm-core/reports/cmcev/events/summary/[nid]';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
  $handler->display->display_options['fields']['nothing_2']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_2']['label'] = 'Attendance Report Link';
  $handler->display->display_options['fields']['nothing_2']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['text'] = 'Attendees';
  $handler->display->display_options['fields']['nothing_2']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['path'] = 'crm-core/reports/cmcev/attendees?event_title=[title]';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Reports';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<div class="btn btn-blue btn-dark btn-simple">[nothing_1]</div> <div class="btn btn-blue btn-dark btn-simple">[nothing_2]</div>';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_3']['id'] = 'nothing_3';
  $handler->display->display_options['fields']['nothing_3']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_3']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_3']['label'] = 'View paged results';
  $handler->display->display_options['fields']['nothing_3']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_3']['alter']['text'] = 'View paged results';
  $handler->display->display_options['fields']['nothing_3']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_3']['alter']['path'] = 'crm-core/reports/cmcev/events';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_4']['id'] = 'nothing_4';
  $handler->display->display_options['fields']['nothing_4']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_4']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_4']['label'] = 'View All';
  $handler->display->display_options['fields']['nothing_4']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_4']['alter']['text'] = 'View All';
  $handler->display->display_options['fields']['nothing_4']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_4']['alter']['path'] = 'crm-core/reports/cmcev/events/all';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_5']['id'] = 'nothing_5';
  $handler->display->display_options['fields']['nothing_5']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_5']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_5']['label'] = 'Export to CSV';
  $handler->display->display_options['fields']['nothing_5']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_5']['alter']['text'] = 'Export to CSV';
  $handler->display->display_options['fields']['nothing_5']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_5']['alter']['path'] = 'crm-core/reports/cmcev/events/export';
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
    'cmcev_event' => 'cmcev_event',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Filter by title:';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Content: Date -  start date (field_cmcev_date) */
  $handler->display->display_options['filters']['field_cmcev_date_value']['id'] = 'field_cmcev_date_value';
  $handler->display->display_options['filters']['field_cmcev_date_value']['table'] = 'field_data_field_cmcev_date';
  $handler->display->display_options['filters']['field_cmcev_date_value']['field'] = 'field_cmcev_date_value';
  $handler->display->display_options['filters']['field_cmcev_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_cmcev_date_value']['group'] = 1;
  $handler->display->display_options['filters']['field_cmcev_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_cmcev_date_value']['expose']['operator_id'] = 'field_cmcev_date_value_op';
  $handler->display->display_options['filters']['field_cmcev_date_value']['expose']['label'] = 'Filter by dates between:';
  $handler->display->display_options['filters']['field_cmcev_date_value']['expose']['operator'] = 'field_cmcev_date_value_op';
  $handler->display->display_options['filters']['field_cmcev_date_value']['expose']['identifier'] = 'field_cmcev_date_value';
  $handler->display->display_options['filters']['field_cmcev_date_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_cmcev_date_value']['form_type'] = 'date_popup';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'crm-core/reports/cmcev/events';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Events';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'features';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Page All */
  $handler = $view->new_display('page', 'Page All', 'page_all');
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  $handler->display->display_options['header']['result']['content'] = 'Displaying @start - @end of @total.';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '[nothing_5] | [nothing_3]';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
  $handler->display->display_options['header']['area']['tokenize'] = TRUE;
  $handler->display->display_options['path'] = 'crm-core/reports/cmcev/events/all';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Events';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'features';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Date */
  $handler->display->display_options['fields']['field_cmcev_date']['id'] = 'field_cmcev_date';
  $handler->display->display_options['fields']['field_cmcev_date']['table'] = 'field_data_field_cmcev_date';
  $handler->display->display_options['fields']['field_cmcev_date']['field'] = 'field_cmcev_date';
  $handler->display->display_options['fields']['field_cmcev_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: SUM(CRM Activity: Amount) */
  $handler->display->display_options['fields']['field_cmcev_amount']['id'] = 'field_cmcev_amount';
  $handler->display->display_options['fields']['field_cmcev_amount']['table'] = 'field_data_field_cmcev_amount';
  $handler->display->display_options['fields']['field_cmcev_amount']['field'] = 'field_cmcev_amount';
  $handler->display->display_options['fields']['field_cmcev_amount']['relationship'] = 'reverse_field_cmcev_event_crm_core_activity';
  $handler->display->display_options['fields']['field_cmcev_amount']['group_type'] = 'sum';
  $handler->display->display_options['fields']['field_cmcev_amount']['label'] = 'Amount raised';
  $handler->display->display_options['fields']['field_cmcev_amount']['type'] = 'crm_core_event_amount';
  $handler->display->display_options['fields']['field_cmcev_amount']['settings'] = array(
    'thousand_separator' => ' ',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: COUNT(DISTINCT CRM Core Activity: Activity ID) */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['relationship'] = 'reverse_field_cmcev_event_crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['activity_id']['label'] = 'Attendance';
  /* Field: MAX(CRM Activity: Registration Date) */
  $handler->display->display_options['fields']['field_cmcev_reg_date']['id'] = 'field_cmcev_reg_date';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['table'] = 'field_data_field_cmcev_reg_date';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['field'] = 'field_cmcev_reg_date';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['relationship'] = 'reverse_field_cmcev_event_crm_core_activity';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['group_type'] = 'max';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['label'] = 'Last Registration Date';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'Summary Link';
  $handler->display->display_options['fields']['nothing_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'Summary';
  $handler->display->display_options['fields']['nothing_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['path'] = 'crm-core/reports/cmcev/overview';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
  $handler->display->display_options['fields']['nothing_2']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_2']['label'] = 'Attendance Report Link';
  $handler->display->display_options['fields']['nothing_2']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['text'] = 'Attendance';
  $handler->display->display_options['fields']['nothing_2']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['path'] = 'crm-core/reports/cmcev/overview';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Reports';
  $handler->display->display_options['fields']['nothing']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[nothing_1] | [nothing_2]';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_3']['id'] = 'nothing_3';
  $handler->display->display_options['fields']['nothing_3']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_3']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_3']['label'] = 'View paged results';
  $handler->display->display_options['fields']['nothing_3']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_3']['alter']['text'] = 'View paged results';
  $handler->display->display_options['fields']['nothing_3']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_3']['alter']['path'] = 'crm-core/reports/cmcev/events';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_4']['id'] = 'nothing_4';
  $handler->display->display_options['fields']['nothing_4']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_4']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_4']['label'] = 'View All';
  $handler->display->display_options['fields']['nothing_4']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_4']['alter']['text'] = 'View All';
  $handler->display->display_options['fields']['nothing_4']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_4']['alter']['path'] = 'crm-core/reports/cmcev/events/all';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_5']['id'] = 'nothing_5';
  $handler->display->display_options['fields']['nothing_5']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_5']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_5']['label'] = 'Export to CSV';
  $handler->display->display_options['fields']['nothing_5']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_5']['alter']['text'] = 'Export to CSV';
  $handler->display->display_options['fields']['nothing_5']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_5']['alter']['path'] = 'crm-core/reports/cmcev/events/export';
  $handler->display->display_options['path'] = 'crm-core/reports/cmcev/events/export';
  $export['cmcev_events_report'] = $view;

  $view = new view();
  $view->name = 'cmcev_overview_signups';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'crm_core_activity';
  $view->human_name = 'CRM Core Event: Overview signups';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Recent signups';
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
  /* Field: CRM Activity: Participants */
  $handler->display->display_options['fields']['field_activity_participants']['id'] = 'field_activity_participants';
  $handler->display->display_options['fields']['field_activity_participants']['table'] = 'field_data_field_activity_participants';
  $handler->display->display_options['fields']['field_activity_participants']['field'] = 'field_activity_participants';
  $handler->display->display_options['fields']['field_activity_participants']['label'] = 'Attendee';
  $handler->display->display_options['fields']['field_activity_participants']['settings'] = array(
    'link' => 1,
  );
  $handler->display->display_options['fields']['field_activity_participants']['delta_offset'] = '0';
  /* Field: CRM Activity: Registration Date */
  $handler->display->display_options['fields']['field_cmcev_reg_date']['id'] = 'field_cmcev_reg_date';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['table'] = 'field_data_field_cmcev_reg_date';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['field'] = 'field_cmcev_reg_date';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: CRM Activity: Event */
  $handler->display->display_options['fields']['field_cmcev_event']['id'] = 'field_cmcev_event';
  $handler->display->display_options['fields']['field_cmcev_event']['table'] = 'field_data_field_cmcev_event';
  $handler->display->display_options['fields']['field_cmcev_event']['field'] = 'field_cmcev_event';
  $handler->display->display_options['fields']['field_cmcev_event']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_cmcev_event']['alter']['path'] = 'crm-core/reports/cmcev/events/summary/[field_cmcev_event-target_id]';
  $handler->display->display_options['fields']['field_cmcev_event']['settings'] = array(
    'link' => 0,
  );
  /* Sort criterion: CRM Activity: Registration Date (field_cmcev_reg_date) */
  $handler->display->display_options['sorts']['field_cmcev_reg_date_value']['id'] = 'field_cmcev_reg_date_value';
  $handler->display->display_options['sorts']['field_cmcev_reg_date_value']['table'] = 'field_data_field_cmcev_reg_date';
  $handler->display->display_options['sorts']['field_cmcev_reg_date_value']['field'] = 'field_cmcev_reg_date_value';
  $handler->display->display_options['sorts']['field_cmcev_reg_date_value']['order'] = 'DESC';
  /* Filter criterion: CRM Core Activity: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'crm_core_activity';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'cmcev_event_registration' => 'cmcev_event_registration',
  );

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['pane_title'] = 'Overview signups';
  $handler->display->display_options['pane_category']['name'] = 'CRM Core Event';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $export['cmcev_overview_signups'] = $view;

  $view = new view();
  $view->name = 'cmcev_registrants';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'crm_core_activity';
  $view->human_name = 'Event Registrants';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
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
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  $handler->display->display_options['header']['result']['content'] = 'Displaying @start - @end of @total.';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'Looks like no one is registered to this event yet. Be the first!';
  $handler->display->display_options['empty']['area']['format'] = 'plain_text';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['id'] = 'field_activity_participants_target_id';
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['table'] = 'field_data_field_activity_participants';
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['field'] = 'field_activity_participants_target_id';
  /* Field: Bulk operations: CRM Core Activity */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'rules_component::rules_cmcv_cancel_registration' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::crm_core_volunteer_change_status_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::rules_cmcev_cancel_registration' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 1,
      'override_label' => 1,
      'label' => 'Cancel registration',
    ),
    'rules_component::rules_cmcev_send_email' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 1,
      'override_label' => 1,
      'label' => 'Send email',
    ),
    'action::views_bulk_operations_delete_item' => array(
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
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 1,
      'override_label' => 1,
      'label' => 'Change status',
      'settings' => array(
        'show_all_tokens' => 1,
        'display_values' => array(
          'cmcev_event_registration::field_cmcev_status' => 'cmcev_event_registration::field_cmcev_status',
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
    'rules_component::rules_rules_cmcv_send_email' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
  /* Field: CRM Core Activity: Activity ID */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['exclude'] = TRUE;
  /* Field: CRM Contact: URL */
  $handler->display->display_options['fields']['url']['id'] = 'url';
  $handler->display->display_options['fields']['url']['table'] = 'views_entity_crm_core_contact';
  $handler->display->display_options['fields']['url']['field'] = 'url';
  $handler->display->display_options['fields']['url']['relationship'] = 'field_activity_participants_target_id';
  $handler->display->display_options['fields']['url']['exclude'] = TRUE;
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
  /* Field: CRM Activity: Status */
  $handler->display->display_options['fields']['field_cmcev_status']['id'] = 'field_cmcev_status';
  $handler->display->display_options['fields']['field_cmcev_status']['table'] = 'field_data_field_cmcev_status';
  $handler->display->display_options['fields']['field_cmcev_status']['field'] = 'field_cmcev_status';
  /* Field: CRM Activity: Registration Date */
  $handler->display->display_options['fields']['field_cmcev_reg_date']['id'] = 'field_cmcev_reg_date';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['table'] = 'field_data_field_cmcev_reg_date';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['field'] = 'field_cmcev_reg_date';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'View Link';
  $handler->display->display_options['fields']['nothing_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'View';
  $handler->display->display_options['fields']['nothing_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['path'] = '[url]/activity/[activity_id]';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
  $handler->display->display_options['fields']['nothing_2']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_2']['label'] = 'Edit Link';
  $handler->display->display_options['fields']['nothing_2']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['text'] = 'Edit';
  $handler->display->display_options['fields']['nothing_2']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['path'] = '[url]/activity/[activity_id]/edit';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Operations';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[nothing_1] | [nothing_2]';
  /* Contextual filter: CRM Activity: Event (field_cmcev_event) */
  $handler->display->display_options['arguments']['field_cmcev_event_target_id']['id'] = 'field_cmcev_event_target_id';
  $handler->display->display_options['arguments']['field_cmcev_event_target_id']['table'] = 'field_data_field_cmcev_event';
  $handler->display->display_options['arguments']['field_cmcev_event_target_id']['field'] = 'field_cmcev_event_target_id';
  $handler->display->display_options['arguments']['field_cmcev_event_target_id']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['field_cmcev_event_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_cmcev_event_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_cmcev_event_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_cmcev_event_target_id']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: CRM Core Activity: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'crm_core_activity';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'cmcev_event_registration' => 'cmcev_event_registration',
  );

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['pane_category']['name'] = 'CRM Core Event';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $handler->display->display_options['allow']['use_pager'] = 0;
  $handler->display->display_options['allow']['items_per_page'] = 0;
  $handler->display->display_options['allow']['offset'] = 0;
  $handler->display->display_options['allow']['link_to_view'] = 0;
  $handler->display->display_options['allow']['more_link'] = 0;
  $handler->display->display_options['allow']['path_override'] = 0;
  $handler->display->display_options['allow']['title_override'] = 0;
  $handler->display->display_options['allow']['exposed_form'] = 0;
  $handler->display->display_options['allow']['fields_override'] = 0;
  $handler->display->display_options['argument_input'] = array(
    'field_cmcev_event_target_id' => array(
      'type' => 'panel',
      'context' => 'entity:crm_core_activity.activity-id',
      'context_optional' => 0,
      'panel' => '0',
      'fixed' => '',
      'label' => 'CRM Activity: Event (field_cmcev_event)',
    ),
  );
  $export['cmcev_registrants'] = $view;

  $view = new view();
  $view->name = 'cmcev_revenues_report';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'CRM Core Event: Revenues report';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Revenues';
  $handler->display->display_options['css_class'] = 'fancy-headers';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view revenues report';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_tags'] = array(
    0 => 'cmcev_date_grouping',
  );
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'field_cmcev_date_1',
      'rendered' => 1,
      'rendered_strip' => 1,
    ),
  );
  $handler->display->display_options['style_options']['columns'] = array(
    'field_cmcev_date_1' => 'field_cmcev_date_1',
    'field_cmcev_date' => 'field_cmcev_date',
    'nid' => 'nid',
    'activity_id' => 'activity_id',
    'field_cmcev_amount' => 'field_cmcev_amount',
    'expression' => 'expression',
    'nothing' => 'nothing',
  );
  $handler->display->display_options['style_options']['default'] = 'field_cmcev_date';
  $handler->display->display_options['style_options']['info'] = array(
    'field_cmcev_date_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcev_date' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nid' => array(
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
    'field_cmcev_amount' => array(
      'sortable' => 1,
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
    'nothing' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '<div class="detail-buttons"><div class="btn btn-orange btn-simple">[nothing]</div></div>&nbsp;';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  $handler->display->display_options['header']['area']['tokenize'] = TRUE;
  /* Relationship: Entity Reference: Referencing entity */
  $handler->display->display_options['relationships']['reverse_field_cmcev_event_crm_core_activity']['id'] = 'reverse_field_cmcev_event_crm_core_activity';
  $handler->display->display_options['relationships']['reverse_field_cmcev_event_crm_core_activity']['table'] = 'node';
  $handler->display->display_options['relationships']['reverse_field_cmcev_event_crm_core_activity']['field'] = 'reverse_field_cmcev_event_crm_core_activity';
  /* Field: Year/month for summary report link */
  $handler->display->display_options['fields']['field_cmcev_date_2']['id'] = 'field_cmcev_date_2';
  $handler->display->display_options['fields']['field_cmcev_date_2']['table'] = 'field_data_field_cmcev_date';
  $handler->display->display_options['fields']['field_cmcev_date_2']['field'] = 'field_cmcev_date';
  $handler->display->display_options['fields']['field_cmcev_date_2']['ui_name'] = 'Year/month for summary report link';
  $handler->display->display_options['fields']['field_cmcev_date_2']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_cmcev_date_2']['settings'] = array(
    'format_type' => 'yearmonth_numeric_',
    'fromto' => 'value',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Grouping by year field */
  $handler->display->display_options['fields']['field_cmcev_date_1']['id'] = 'field_cmcev_date_1';
  $handler->display->display_options['fields']['field_cmcev_date_1']['table'] = 'field_data_field_cmcev_date';
  $handler->display->display_options['fields']['field_cmcev_date_1']['field'] = 'field_cmcev_date';
  $handler->display->display_options['fields']['field_cmcev_date_1']['ui_name'] = 'Grouping by year field';
  $handler->display->display_options['fields']['field_cmcev_date_1']['label'] = '';
  $handler->display->display_options['fields']['field_cmcev_date_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_cmcev_date_1']['alter']['trim_whitespace'] = TRUE;
  $handler->display->display_options['fields']['field_cmcev_date_1']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['field_cmcev_date_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_cmcev_date_1']['settings'] = array(
    'format_type' => 'year',
    'fromto' => 'value',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Content: Date */
  $handler->display->display_options['fields']['field_cmcev_date']['id'] = 'field_cmcev_date';
  $handler->display->display_options['fields']['field_cmcev_date']['table'] = 'field_data_field_cmcev_date';
  $handler->display->display_options['fields']['field_cmcev_date']['field'] = 'field_cmcev_date';
  $handler->display->display_options['fields']['field_cmcev_date']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_cmcev_date']['alter']['path'] = 'crm-core/reports/cmcev/revenues/summary/[field_cmcev_date_2]';
  $handler->display->display_options['fields']['field_cmcev_date']['settings'] = array(
    'format_type' => 'month_year',
    'fromto' => 'value',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: COUNT(DISTINCT Content: Nid) */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['nid']['label'] = 'Total events';
  /* Field: COUNT(DISTINCT CRM Core Activity: Activity ID) */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['relationship'] = 'reverse_field_cmcev_event_crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['activity_id']['label'] = 'Total registrations';
  /* Field: SUM(CRM Activity: Amount) */
  $handler->display->display_options['fields']['field_cmcev_amount']['id'] = 'field_cmcev_amount';
  $handler->display->display_options['fields']['field_cmcev_amount']['table'] = 'field_data_field_cmcev_amount';
  $handler->display->display_options['fields']['field_cmcev_amount']['field'] = 'field_cmcev_amount';
  $handler->display->display_options['fields']['field_cmcev_amount']['relationship'] = 'reverse_field_cmcev_event_crm_core_activity';
  $handler->display->display_options['fields']['field_cmcev_amount']['group_type'] = 'sum';
  $handler->display->display_options['fields']['field_cmcev_amount']['label'] = 'Amount raised';
  $handler->display->display_options['fields']['field_cmcev_amount']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_cmcev_amount']['type'] = 'crm_core_event_amount';
  $handler->display->display_options['fields']['field_cmcev_amount']['settings'] = array(
    'thousand_separator' => '',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Global: Math expression */
  $handler->display->display_options['fields']['expression']['id'] = 'expression';
  $handler->display->display_options['fields']['expression']['table'] = 'views';
  $handler->display->display_options['fields']['expression']['field'] = 'expression';
  $handler->display->display_options['fields']['expression']['label'] = 'Average per event';
  $handler->display->display_options['fields']['expression']['set_precision'] = TRUE;
  $handler->display->display_options['fields']['expression']['precision'] = '2';
  $handler->display->display_options['fields']['expression']['expression'] = '[field_cmcev_amount]/[nid]';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Export to CSV';
  $handler->display->display_options['fields']['nothing']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['text'] = 'Export to CSV';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['path'] = 'crm-core/reports/cmcev/revenues/export';
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
    'cmcev_event' => 'cmcev_event',
  );
  /* Filter criterion: Content: Date -  start date (field_cmcev_date) */
  $handler->display->display_options['filters']['field_cmcev_date_value']['id'] = 'field_cmcev_date_value';
  $handler->display->display_options['filters']['field_cmcev_date_value']['table'] = 'field_data_field_cmcev_date';
  $handler->display->display_options['filters']['field_cmcev_date_value']['field'] = 'field_cmcev_date_value';
  $handler->display->display_options['filters']['field_cmcev_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_cmcev_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_cmcev_date_value']['expose']['operator_id'] = 'field_cmcev_date_value_op';
  $handler->display->display_options['filters']['field_cmcev_date_value']['expose']['label'] = 'Filter by dates between:';
  $handler->display->display_options['filters']['field_cmcev_date_value']['expose']['operator'] = 'field_cmcev_date_value_op';
  $handler->display->display_options['filters']['field_cmcev_date_value']['expose']['identifier'] = 'field_cmcev_date_value';
  $handler->display->display_options['filters']['field_cmcev_date_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_cmcev_date_value']['form_type'] = 'date_popup';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'crm-core/reports/cmcev/revenues';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Revenues';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'features';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['path'] = 'crm-core/reports/cmcev/revenues/export';
  $export['cmcev_revenues_report'] = $view;

  $view = new view();
  $view->name = 'cmcev_revenues_summary_report';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'CRM Core Event: Revenues summary report';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Events';
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
    'title' => 'title',
    'field_cmcev_date' => 'field_cmcev_date',
    'field_cmcev_amount' => 'field_cmcev_amount',
    'activity_id' => 'activity_id',
    'field_cmcev_reg_date' => 'field_cmcev_reg_date',
    'nothing_1' => 'nothing_1',
    'nothing_2' => 'nothing_2',
    'nothing' => 'nothing',
  );
  $handler->display->display_options['style_options']['default'] = 'field_cmcev_date';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcev_date' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcev_amount' => array(
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
    'field_cmcev_reg_date' => array(
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
  );
  /* Relationship: Entity Reference: Referencing entity */
  $handler->display->display_options['relationships']['reverse_field_cmcev_event_crm_core_activity']['id'] = 'reverse_field_cmcev_event_crm_core_activity';
  $handler->display->display_options['relationships']['reverse_field_cmcev_event_crm_core_activity']['table'] = 'node';
  $handler->display->display_options['relationships']['reverse_field_cmcev_event_crm_core_activity']['field'] = 'reverse_field_cmcev_event_crm_core_activity';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Date */
  $handler->display->display_options['fields']['field_cmcev_date']['id'] = 'field_cmcev_date';
  $handler->display->display_options['fields']['field_cmcev_date']['table'] = 'field_data_field_cmcev_date';
  $handler->display->display_options['fields']['field_cmcev_date']['field'] = 'field_cmcev_date';
  $handler->display->display_options['fields']['field_cmcev_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: SUM(CRM Activity: Amount) */
  $handler->display->display_options['fields']['field_cmcev_amount']['id'] = 'field_cmcev_amount';
  $handler->display->display_options['fields']['field_cmcev_amount']['table'] = 'field_data_field_cmcev_amount';
  $handler->display->display_options['fields']['field_cmcev_amount']['field'] = 'field_cmcev_amount';
  $handler->display->display_options['fields']['field_cmcev_amount']['relationship'] = 'reverse_field_cmcev_event_crm_core_activity';
  $handler->display->display_options['fields']['field_cmcev_amount']['group_type'] = 'sum';
  $handler->display->display_options['fields']['field_cmcev_amount']['label'] = 'Amount raised';
  $handler->display->display_options['fields']['field_cmcev_amount']['type'] = 'crm_core_event_amount';
  $handler->display->display_options['fields']['field_cmcev_amount']['settings'] = array(
    'thousand_separator' => ' ',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: COUNT(DISTINCT CRM Core Activity: Activity ID) */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['relationship'] = 'reverse_field_cmcev_event_crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['activity_id']['label'] = 'Attendance';
  /* Field: MAX(CRM Activity: Registration Date) */
  $handler->display->display_options['fields']['field_cmcev_reg_date']['id'] = 'field_cmcev_reg_date';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['table'] = 'field_data_field_cmcev_reg_date';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['field'] = 'field_cmcev_reg_date';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['relationship'] = 'reverse_field_cmcev_event_crm_core_activity';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['group_type'] = 'max';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['label'] = 'Last Registration Date';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'Summary Link';
  $handler->display->display_options['fields']['nothing_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'Summary';
  $handler->display->display_options['fields']['nothing_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['path'] = 'crm-core/reports/cmcev/events/summary/[nid]';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
  $handler->display->display_options['fields']['nothing_2']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_2']['label'] = 'Attendance Report Link';
  $handler->display->display_options['fields']['nothing_2']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['text'] = 'Attendees';
  $handler->display->display_options['fields']['nothing_2']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['path'] = 'crm-core/reports/cmcev/attendees?event_title=[title]';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Reports';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[nothing_1] [nothing_2]';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Date -  start date (field_cmcev_date) */
  $handler->display->display_options['arguments']['field_cmcev_date_value']['id'] = 'field_cmcev_date_value';
  $handler->display->display_options['arguments']['field_cmcev_date_value']['table'] = 'field_data_field_cmcev_date';
  $handler->display->display_options['arguments']['field_cmcev_date_value']['field'] = 'field_cmcev_date_value';
  $handler->display->display_options['arguments']['field_cmcev_date_value']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_cmcev_date_value']['default_argument_type'] = 'date';
  $handler->display->display_options['arguments']['field_cmcev_date_value']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_cmcev_date_value']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_cmcev_date_value']['summary_options']['items_per_page'] = '25';
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
    'cmcev_event' => 'cmcev_event',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['pane_title'] = 'Revenues summary report';
  $handler->display->display_options['pane_category']['name'] = 'CRM Core Event';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $handler->display->display_options['argument_input'] = array(
    'field_cmcev_date_value' => array(
      'type' => 'panel',
      'context' => 'entity:crm_core_activity.activity-id',
      'context_optional' => 0,
      'panel' => '0',
      'fixed' => '',
      'label' => 'Content: Date -  start date (field_cmcev_date)',
    ),
  );
  $export['cmcev_revenues_summary_report'] = $view;

  $view = new view();
  $view->name = 'cmcev_revenues_summary_report_metrics';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'CRM Core Event: Revenues summary report metrics';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
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
    'nid' => 'nid',
    'field_cmcev_amount' => 'field_cmcev_amount',
    'activity_id' => 'activity_id',
    'expression' => 'expression',
    'expression_1' => 'expression_1',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'nid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcev_amount' => array(
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
    'expression' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'expression_1' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Entity Reference: Referencing entity */
  $handler->display->display_options['relationships']['reverse_field_cmcev_event_crm_core_activity']['id'] = 'reverse_field_cmcev_event_crm_core_activity';
  $handler->display->display_options['relationships']['reverse_field_cmcev_event_crm_core_activity']['table'] = 'node';
  $handler->display->display_options['relationships']['reverse_field_cmcev_event_crm_core_activity']['field'] = 'reverse_field_cmcev_event_crm_core_activity';
  /* Field: COUNT(DISTINCT CRM Core Activity: Activity ID) */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['relationship'] = 'reverse_field_cmcev_event_crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['activity_id']['label'] = 'Attendance';
  $handler->display->display_options['fields']['activity_id']['exclude'] = TRUE;
  /* Field: COUNT(DISTINCT Content: Nid) */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['nid']['label'] = 'Total events';
  /* Field: SUM(CRM Activity: Amount) */
  $handler->display->display_options['fields']['field_cmcev_amount']['id'] = 'field_cmcev_amount';
  $handler->display->display_options['fields']['field_cmcev_amount']['table'] = 'field_data_field_cmcev_amount';
  $handler->display->display_options['fields']['field_cmcev_amount']['field'] = 'field_cmcev_amount';
  $handler->display->display_options['fields']['field_cmcev_amount']['relationship'] = 'reverse_field_cmcev_event_crm_core_activity';
  $handler->display->display_options['fields']['field_cmcev_amount']['group_type'] = 'sum';
  $handler->display->display_options['fields']['field_cmcev_amount']['label'] = 'Total raised';
  $handler->display->display_options['fields']['field_cmcev_amount']['type'] = 'crm_core_event_amount';
  $handler->display->display_options['fields']['field_cmcev_amount']['settings'] = array(
    'thousand_separator' => '',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Global: Math expression */
  $handler->display->display_options['fields']['expression']['id'] = 'expression';
  $handler->display->display_options['fields']['expression']['table'] = 'views';
  $handler->display->display_options['fields']['expression']['field'] = 'expression';
  $handler->display->display_options['fields']['expression']['label'] = 'Average amount';
  $handler->display->display_options['fields']['expression']['set_precision'] = TRUE;
  $handler->display->display_options['fields']['expression']['precision'] = '2';
  $handler->display->display_options['fields']['expression']['separator'] = '';
  $handler->display->display_options['fields']['expression']['expression'] = '[field_cmcev_amount]/[nid]';
  /* Field: Global: Math expression */
  $handler->display->display_options['fields']['expression_1']['id'] = 'expression_1';
  $handler->display->display_options['fields']['expression_1']['table'] = 'views';
  $handler->display->display_options['fields']['expression_1']['field'] = 'expression';
  $handler->display->display_options['fields']['expression_1']['label'] = 'Average attendees';
  $handler->display->display_options['fields']['expression_1']['set_precision'] = TRUE;
  $handler->display->display_options['fields']['expression_1']['precision'] = '0';
  $handler->display->display_options['fields']['expression_1']['expression'] = '[activity_id]/[nid]';
  /* Contextual filter: Content: Date -  start date (field_cmcev_date) */
  $handler->display->display_options['arguments']['field_cmcev_date_value']['id'] = 'field_cmcev_date_value';
  $handler->display->display_options['arguments']['field_cmcev_date_value']['table'] = 'field_data_field_cmcev_date';
  $handler->display->display_options['arguments']['field_cmcev_date_value']['field'] = 'field_cmcev_date_value';
  $handler->display->display_options['arguments']['field_cmcev_date_value']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_cmcev_date_value']['default_argument_type'] = 'date';
  $handler->display->display_options['arguments']['field_cmcev_date_value']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_cmcev_date_value']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_cmcev_date_value']['summary_options']['items_per_page'] = '25';
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
    'cmcev_event' => 'cmcev_event',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['pane_title'] = 'Revenues summary report metrics';
  $handler->display->display_options['pane_category']['name'] = 'CRM Core Event';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $handler->display->display_options['argument_input'] = array(
    'field_cmcev_date_value' => array(
      'type' => 'panel',
      'context' => 'entity:crm_core_activity.activity-id',
      'context_optional' => 0,
      'panel' => '0',
      'fixed' => '',
      'label' => 'Content: Date -  start date (field_cmcev_date)',
    ),
  );
  $export['cmcev_revenues_summary_report_metrics'] = $view;

  $view = new view();
  $view->name = 'cmcev_sources_report';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'crm_core_activity';
  $view->human_name = 'CRM Core Event: Sources report';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['css_class'] = 'fancy-headers';
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
    'field_cmcev_source' => 'field_cmcev_source',
    'activity_id' => 'activity_id',
    'field_cmcev_reg_date' => 'field_cmcev_reg_date',
    'nothing' => 'nothing',
  );
  $handler->display->display_options['style_options']['default'] = 'field_cmcev_source';
  $handler->display->display_options['style_options']['info'] = array(
    'field_cmcev_source' => array(
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
    'field_cmcev_reg_date' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
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
  /* Field: CRM Activity: Source */
  $handler->display->display_options['fields']['field_cmcev_source_1']['id'] = 'field_cmcev_source_1';
  $handler->display->display_options['fields']['field_cmcev_source_1']['table'] = 'field_data_field_cmcev_source';
  $handler->display->display_options['fields']['field_cmcev_source_1']['field'] = 'field_cmcev_source';
  $handler->display->display_options['fields']['field_cmcev_source_1']['label'] = 'Source hidden';
  $handler->display->display_options['fields']['field_cmcev_source_1']['exclude'] = TRUE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'Summary Report Link';
  $handler->display->display_options['fields']['nothing_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'Summary';
  $handler->display->display_options['fields']['nothing_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['path'] = 'crm-core/reports/cmcev/sources/summary/[field_cmcev_source_1]';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
  $handler->display->display_options['fields']['nothing_2']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_2']['label'] = 'Signup details link';
  $handler->display->display_options['fields']['nothing_2']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['text'] = 'Signup details';
  $handler->display->display_options['fields']['nothing_2']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['path'] = 'crm-core/reports/cmcev/attendees?source=[field_cmcev_source_1]';
  /* Field: CRM Activity: Source */
  $handler->display->display_options['fields']['field_cmcev_source']['id'] = 'field_cmcev_source';
  $handler->display->display_options['fields']['field_cmcev_source']['table'] = 'field_data_field_cmcev_source';
  $handler->display->display_options['fields']['field_cmcev_source']['field'] = 'field_cmcev_source';
  $handler->display->display_options['fields']['field_cmcev_source']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_cmcev_source']['alter']['path'] = 'crm-core/reports/cmcev/sources/summary/[field_cmcev_source]';
  $handler->display->display_options['fields']['field_cmcev_source']['empty'] = 'N/A';
  /* Field: COUNT(DISTINCT CRM Core Activity: Activity ID) */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['activity_id']['label'] = 'Total registrations';
  /* Field: MAX(CRM Activity: Registration Date) */
  $handler->display->display_options['fields']['field_cmcev_reg_date']['id'] = 'field_cmcev_reg_date';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['table'] = 'field_data_field_cmcev_reg_date';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['field'] = 'field_cmcev_reg_date';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['group_type'] = 'max';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['label'] = 'Last registration date';
  $handler->display->display_options['fields']['field_cmcev_reg_date']['settings'] = array(
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
  $handler->display->display_options['fields']['nothing']['label'] = 'Reports';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<div class="btn btn-blue btn-dark btn-simple">[nothing_1]</div> <div class="btn btn-blue btn-dark btn-simple">[nothing_2]</div>';
  /* Filter criterion: CRM Core Activity: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'crm_core_activity';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'cmcev_event_registration' => 'cmcev_event_registration',
  );

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['pane_title'] = 'Sources report table view';
  $handler->display->display_options['pane_category']['name'] = 'CRM Core Event';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $export['cmcev_sources_report'] = $view;

  $view = new view();
  $view->name = 'cmcev_sources_summary_report';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'crm_core_activity';
  $view->human_name = 'CRM Core Event: Sources summary report';
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
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No results for this source.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: COUNT(CRM Core Activity: Activity ID) */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['group_type'] = 'count';
  $handler->display->display_options['fields']['activity_id']['label'] = 'Total attendees';
  /* Field: SUM(CRM Activity: Amount) */
  $handler->display->display_options['fields']['field_cmcev_amount']['id'] = 'field_cmcev_amount';
  $handler->display->display_options['fields']['field_cmcev_amount']['table'] = 'field_data_field_cmcev_amount';
  $handler->display->display_options['fields']['field_cmcev_amount']['field'] = 'field_cmcev_amount';
  $handler->display->display_options['fields']['field_cmcev_amount']['group_type'] = 'sum';
  $handler->display->display_options['fields']['field_cmcev_amount']['label'] = 'Total raised';
  $handler->display->display_options['fields']['field_cmcev_amount']['empty'] = '0.00';
  $handler->display->display_options['fields']['field_cmcev_amount']['settings'] = array(
    'thousand_separator' => '',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Global: Math expression */
  $handler->display->display_options['fields']['expression']['id'] = 'expression';
  $handler->display->display_options['fields']['expression']['table'] = 'views';
  $handler->display->display_options['fields']['expression']['field'] = 'expression';
  $handler->display->display_options['fields']['expression']['label'] = 'Average registration amount';
  $handler->display->display_options['fields']['expression']['set_precision'] = TRUE;
  $handler->display->display_options['fields']['expression']['precision'] = '2';
  $handler->display->display_options['fields']['expression']['separator'] = '';
  $handler->display->display_options['fields']['expression']['expression'] = '[field_cmcev_amount]/[activity_id]';
  /* Contextual filter: CRM Activity: Source (field_cmcev_source) */
  $handler->display->display_options['arguments']['field_cmcev_source_value']['id'] = 'field_cmcev_source_value';
  $handler->display->display_options['arguments']['field_cmcev_source_value']['table'] = 'field_data_field_cmcev_source';
  $handler->display->display_options['arguments']['field_cmcev_source_value']['field'] = 'field_cmcev_source_value';
  $handler->display->display_options['arguments']['field_cmcev_source_value']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['field_cmcev_source_value']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_cmcev_source_value']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_cmcev_source_value']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_cmcev_source_value']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_cmcev_source_value']['validate']['fail'] = 'empty';
  $handler->display->display_options['arguments']['field_cmcev_source_value']['limit'] = '0';
  /* Filter criterion: CRM Core Activity: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'crm_core_activity';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'cmcev_event_registration' => 'cmcev_event_registration',
  );

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['pane_title'] = 'Sources summary report metrics';
  $handler->display->display_options['pane_category']['name'] = 'CRM Core Event';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $handler->display->display_options['argument_input'] = array(
    'field_cmcev_source_value' => array(
      'type' => 'panel',
      'context' => 'entity:crm_core_activity.activity-id',
      'context_optional' => 0,
      'panel' => '0',
      'fixed' => '',
      'label' => 'CRM Activity: Source (field_cmcev_source)',
    ),
  );
  $export['cmcev_sources_summary_report'] = $view;

  $view = new view();
  $view->name = 'cmcev_summary_panel';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'crm_core_activity';
  $view->human_name = 'CRM Core Event: Summary Panel';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Event Summary';
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
  /* Field: COUNT(CRM Core Activity: Activity ID) */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['group_type'] = 'count';
  $handler->display->display_options['fields']['activity_id']['label'] = 'Total registrations';
  $handler->display->display_options['fields']['activity_id']['separator'] = '';
  /* Field: SUM(CRM Activity: Amount) */
  $handler->display->display_options['fields']['field_cmcev_amount']['id'] = 'field_cmcev_amount';
  $handler->display->display_options['fields']['field_cmcev_amount']['table'] = 'field_data_field_cmcev_amount';
  $handler->display->display_options['fields']['field_cmcev_amount']['field'] = 'field_cmcev_amount';
  $handler->display->display_options['fields']['field_cmcev_amount']['group_type'] = 'sum';
  $handler->display->display_options['fields']['field_cmcev_amount']['label'] = 'Total raised';
  $handler->display->display_options['fields']['field_cmcev_amount']['settings'] = array(
    'thousand_separator' => '',
    'decimal_separator' => '.',
    'scale' => '0',
    'prefix_suffix' => 1,
  );
  /* Field: Global: Math expression */
  $handler->display->display_options['fields']['expression']['id'] = 'expression';
  $handler->display->display_options['fields']['expression']['table'] = 'views';
  $handler->display->display_options['fields']['expression']['field'] = 'expression';
  $handler->display->display_options['fields']['expression']['label'] = 'Average registration amount';
  $handler->display->display_options['fields']['expression']['set_precision'] = TRUE;
  $handler->display->display_options['fields']['expression']['precision'] = '0';
  $handler->display->display_options['fields']['expression']['separator'] = '';
  $handler->display->display_options['fields']['expression']['expression'] = '[field_cmcev_amount]/[activity_id]';
  /* Filter criterion: CRM Core Activity: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'crm_core_activity';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'cmcev_event_registration' => 'cmcev_event_registration',
  );

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['pane_title'] = 'Event Summary Widget';
  $handler->display->display_options['pane_description'] = 'Displays metrics around events';
  $handler->display->display_options['pane_category']['name'] = 'CRM Core Event';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $handler->display->display_options['argument_input'] = array(
    'field_cmcev_event_target_id' => array(
      'type' => 'panel',
      'context' => 'entity:crm_core_activity.activity-id',
      'context_optional' => 0,
      'panel' => '0',
      'fixed' => '',
      'label' => 'CRM Activity: Event (field_cmcev_event)',
    ),
  );
  $export['cmcev_summary_panel'] = $view;

  return $export;
}
