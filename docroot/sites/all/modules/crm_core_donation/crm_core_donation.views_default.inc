<?php
/**
 * @file
 * crm_core_donation.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function crm_core_donation_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'cmcd_details';
  $view->description = 'All donor activity. Filterable by source, date, amount.';
  $view->tag = 'default';
  $view->base_table = 'crm_core_activity';
  $view->human_name = 'Donation Details';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Donation Details';
  $handler->display->display_options['css_class'] = 'fancy-headers';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view donation details report';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'created' => 'created',
    'field_cmcd_amount' => 'field_cmcd_amount',
    'activity_id' => 'activity_id',
    'field_activity_participants' => 'field_activity_participants',
    'contact_id' => 'contact_id',
    'contact_name_2' => 'contact_name_2',
    'field_cmcd_source' => 'field_cmcd_source',
    'edit_crm_activity' => 'edit_crm_activity',
    'view_crm_activity' => 'view_crm_activity',
    'nothing' => 'nothing',
  );
  $handler->display->display_options['style_options']['default'] = 'created';
  $handler->display->display_options['style_options']['info'] = array(
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcd_amount' => array(
      'sortable' => 1,
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
    'field_activity_participants' => array(
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
    'contact_name_2' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcd_source' => array(
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
  $handler->display->display_options['header']['result']['content'] = '<div class="results-summary">Displaying @start - @end of @total.</div>';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '<div class="detail-buttons"><div class="btn btn-blue btn-simple">[nothing_1]</div><div class="btn btn-orange btn-simple">[nothing_2]</div></div>';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  $handler->display->display_options['header']['area']['tokenize'] = TRUE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'There are no donations stored in the system at this time.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_activity_participants_target_id_1']['id'] = 'field_activity_participants_target_id_1';
  $handler->display->display_options['relationships']['field_activity_participants_target_id_1']['table'] = 'field_data_field_activity_participants';
  $handler->display->display_options['relationships']['field_activity_participants_target_id_1']['field'] = 'field_activity_participants_target_id';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['id'] = 'field_activity_participants_target_id';
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['table'] = 'field_data_field_activity_participants';
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['field'] = 'field_activity_participants_target_id';
  /* Field: CRM Activity: Receive Date */
  $handler->display->display_options['fields']['field_cmcd_receive_date']['id'] = 'field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['table'] = 'field_data_field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['field'] = 'field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: CRM Activity: Amount */
  $handler->display->display_options['fields']['field_cmcd_amount']['id'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['table'] = 'field_data_field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['field'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['type'] = 'crm_core_donation_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['settings'] = array(
    'thousand_separator' => ',',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: CRM Core Activity: Activity ID */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['exclude'] = TRUE;
  /* Field: CRM Activity: Participants */
  $handler->display->display_options['fields']['field_activity_participants']['id'] = 'field_activity_participants';
  $handler->display->display_options['fields']['field_activity_participants']['table'] = 'field_data_field_activity_participants';
  $handler->display->display_options['fields']['field_activity_participants']['field'] = 'field_activity_participants';
  $handler->display->display_options['fields']['field_activity_participants']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_activity_participants']['settings'] = array(
    'link' => 0,
  );
  $handler->display->display_options['fields']['field_activity_participants']['delta_limit'] = '1';
  $handler->display->display_options['fields']['field_activity_participants']['delta_offset'] = '0';
  /* Field: CRM Contact: Contact ID */
  $handler->display->display_options['fields']['contact_id']['id'] = 'contact_id';
  $handler->display->display_options['fields']['contact_id']['table'] = 'crm_core_contact';
  $handler->display->display_options['fields']['contact_id']['field'] = 'contact_id';
  $handler->display->display_options['fields']['contact_id']['relationship'] = 'field_activity_participants_target_id_1';
  $handler->display->display_options['fields']['contact_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['contact_id']['link_to_contact'] = 0;
  /* Field: CRM Core Contact: Household */
  $handler->display->display_options['fields']['contact_name_2']['id'] = 'contact_name_2';
  $handler->display->display_options['fields']['contact_name_2']['table'] = 'field_data_contact_name';
  $handler->display->display_options['fields']['contact_name_2']['field'] = 'contact_name';
  $handler->display->display_options['fields']['contact_name_2']['relationship'] = 'field_activity_participants_target_id';
  $handler->display->display_options['fields']['contact_name_2']['label'] = 'Participant Name';
  $handler->display->display_options['fields']['contact_name_2']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['contact_name_2']['alter']['path'] = 'crm-core/contact/[contact_id]';
  $handler->display->display_options['fields']['contact_name_2']['click_sort_column'] = 'family';
  $handler->display->display_options['fields']['contact_name_2']['settings'] = array(
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
  /* Field: CRM Activity: Source */
  $handler->display->display_options['fields']['field_cmcd_source']['id'] = 'field_cmcd_source';
  $handler->display->display_options['fields']['field_cmcd_source']['table'] = 'field_data_field_cmcd_source';
  $handler->display->display_options['fields']['field_cmcd_source']['field'] = 'field_cmcd_source';
  $handler->display->display_options['fields']['field_cmcd_source']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_cmcd_source']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_cmcd_source']['alter']['path'] = 'crm-core/reports/cmcd/donation-sources/summary/[field_cmcd_source-value]';
  /* Field: CRM Core Activity: Edit link */
  $handler->display->display_options['fields']['edit_crm_activity']['id'] = 'edit_crm_activity';
  $handler->display->display_options['fields']['edit_crm_activity']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['edit_crm_activity']['field'] = 'edit_crm_activity';
  $handler->display->display_options['fields']['edit_crm_activity']['exclude'] = TRUE;
  $handler->display->display_options['fields']['edit_crm_activity']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['edit_crm_activity']['alter']['text'] = 'edit';
  $handler->display->display_options['fields']['edit_crm_activity']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['edit_crm_activity']['alter']['path'] = 'crm-core/activity/[activity_id]/edit';
  $handler->display->display_options['fields']['edit_crm_activity']['redirect'] = 0;
  /* Field: CRM Core Activity: Link */
  $handler->display->display_options['fields']['view_crm_activity']['id'] = 'view_crm_activity';
  $handler->display->display_options['fields']['view_crm_activity']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['view_crm_activity']['field'] = 'view_crm_activity';
  $handler->display->display_options['fields']['view_crm_activity']['label'] = 'View';
  $handler->display->display_options['fields']['view_crm_activity']['exclude'] = TRUE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Operations';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<div class="btn btn-blue btn-dark btn-simple">[view_crm_activity]</div> <div class="btn btn-green btn-dark btn-simple">[edit_crm_activity]</div>';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'View All';
  $handler->display->display_options['fields']['nothing_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'View All';
  $handler->display->display_options['fields']['nothing_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['path'] = 'crm-core/reports/cmcd/cmcd-details/full';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
  $handler->display->display_options['fields']['nothing_2']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_2']['label'] = 'Export to CSV';
  $handler->display->display_options['fields']['nothing_2']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['text'] = 'Export to CSV';
  $handler->display->display_options['fields']['nothing_2']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['path'] = 'crm-core/reports/cmcd/cmcd-details/export';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_3']['id'] = 'nothing_3';
  $handler->display->display_options['fields']['nothing_3']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_3']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_3']['label'] = 'View paged results';
  $handler->display->display_options['fields']['nothing_3']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_3']['alter']['text'] = 'View paged results';
  $handler->display->display_options['fields']['nothing_3']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_3']['alter']['path'] = 'crm-core/reports/cmcd/cmcd-details';
  /* Sort criterion: CRM Activity: Receive Date (field_cmcd_receive_date) */
  $handler->display->display_options['sorts']['field_cmcd_receive_date_value']['id'] = 'field_cmcd_receive_date_value';
  $handler->display->display_options['sorts']['field_cmcd_receive_date_value']['table'] = 'field_data_field_cmcd_receive_date';
  $handler->display->display_options['sorts']['field_cmcd_receive_date_value']['field'] = 'field_cmcd_receive_date_value';
  $handler->display->display_options['sorts']['field_cmcd_receive_date_value']['order'] = 'DESC';
  /* Filter criterion: CRM Core Activity: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'crm_core_activity';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'cmcd_donation' => 'cmcd_donation',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: CRM Activity: Amount (field_cmcd_amount) */
  $handler->display->display_options['filters']['field_cmcd_amount_value']['id'] = 'field_cmcd_amount_value';
  $handler->display->display_options['filters']['field_cmcd_amount_value']['table'] = 'field_data_field_cmcd_amount';
  $handler->display->display_options['filters']['field_cmcd_amount_value']['field'] = 'field_cmcd_amount_value';
  $handler->display->display_options['filters']['field_cmcd_amount_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_cmcd_amount_value']['group'] = 1;
  $handler->display->display_options['filters']['field_cmcd_amount_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_cmcd_amount_value']['expose']['operator_id'] = 'field_cmcd_amount_value_op';
  $handler->display->display_options['filters']['field_cmcd_amount_value']['expose']['label'] = 'Filter by amounts between';
  $handler->display->display_options['filters']['field_cmcd_amount_value']['expose']['operator'] = 'field_cmcd_amount_value_op';
  $handler->display->display_options['filters']['field_cmcd_amount_value']['expose']['identifier'] = 'field_cmcd_amount_value';
  $handler->display->display_options['filters']['field_cmcd_amount_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: CRM Activity: Receive Date (field_cmcd_receive_date) */
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['id'] = 'field_cmcd_receive_date_value';
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['table'] = 'field_data_field_cmcd_receive_date';
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['field'] = 'field_cmcd_receive_date_value';
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['group'] = 1;
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['expose']['operator_id'] = 'field_cmcd_receive_date_value_op';
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['expose']['label'] = 'Filter by dates between';
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['expose']['operator'] = 'field_cmcd_receive_date_value_op';
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['expose']['identifier'] = 'field_cmcd_receive_date_value';
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['year_range'] = '-0:+0';
  /* Filter criterion: CRM Activity: Source (field_cmcd_source) */
  $handler->display->display_options['filters']['field_cmcd_source_value']['id'] = 'field_cmcd_source_value';
  $handler->display->display_options['filters']['field_cmcd_source_value']['table'] = 'field_data_field_cmcd_source';
  $handler->display->display_options['filters']['field_cmcd_source_value']['field'] = 'field_cmcd_source_value';
  $handler->display->display_options['filters']['field_cmcd_source_value']['group'] = 1;
  $handler->display->display_options['filters']['field_cmcd_source_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_cmcd_source_value']['expose']['operator_id'] = 'field_cmcd_source_value_op';
  $handler->display->display_options['filters']['field_cmcd_source_value']['expose']['label'] = 'Filter by source';
  $handler->display->display_options['filters']['field_cmcd_source_value']['expose']['operator'] = 'field_cmcd_source_value_op';
  $handler->display->display_options['filters']['field_cmcd_source_value']['expose']['identifier'] = 'field_cmcd_source_value';
  $handler->display->display_options['filters']['field_cmcd_source_value']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_cmcd_source_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );

  /* Display: Paged Results */
  $handler = $view->new_display('page', 'Paged Results', 'page');
  $handler->display->display_options['display_comment'] = 'This view provides a detailed listing of all donations in the system.';
  $handler->display->display_options['display_description'] = 'This shows 50 items per page.';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['path'] = 'crm-core/reports/cmcd/cmcd-details';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Donation Details';
  $handler->display->display_options['menu']['weight'] = '-7';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'tab';
  $handler->display->display_options['tab_options']['title'] = 'Donation Details';
  $handler->display->display_options['tab_options']['weight'] = '0';

  /* Display: Full Results */
  $handler = $view->new_display('page', 'Full Results', 'page_1');
  $handler->display->display_options['display_description'] = 'Full detailed report, no filters.';
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
  $handler->display->display_options['header']['area']['content'] = ' [nothing_3] [nothing_2]';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
  $handler->display->display_options['header']['area']['tokenize'] = TRUE;
  $handler->display->display_options['path'] = 'crm-core/reports/cmcd/cmcd-details/full';

  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: CRM Activity: Receive Date */
  $handler->display->display_options['fields']['field_cmcd_receive_date']['id'] = 'field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['table'] = 'field_data_field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['field'] = 'field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: CRM Activity: Amount */
  $handler->display->display_options['fields']['field_cmcd_amount']['id'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['table'] = 'field_data_field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['field'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['type'] = 'crm_core_donation_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['settings'] = array(
    'thousand_separator' => ',',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: CRM Core Activity: Activity ID */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['exclude'] = TRUE;
  /* Field: CRM Activity: Participants */
  $handler->display->display_options['fields']['field_activity_participants']['id'] = 'field_activity_participants';
  $handler->display->display_options['fields']['field_activity_participants']['table'] = 'field_data_field_activity_participants';
  $handler->display->display_options['fields']['field_activity_participants']['field'] = 'field_activity_participants';
  $handler->display->display_options['fields']['field_activity_participants']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_activity_participants']['settings'] = array(
    'link' => 0,
  );
  $handler->display->display_options['fields']['field_activity_participants']['delta_limit'] = '1';
  $handler->display->display_options['fields']['field_activity_participants']['delta_offset'] = '0';
  /* Field: CRM Contact: Contact ID */
  $handler->display->display_options['fields']['contact_id']['id'] = 'contact_id';
  $handler->display->display_options['fields']['contact_id']['table'] = 'crm_core_contact';
  $handler->display->display_options['fields']['contact_id']['field'] = 'contact_id';
  $handler->display->display_options['fields']['contact_id']['relationship'] = 'field_activity_participants_target_id_1';
  $handler->display->display_options['fields']['contact_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['contact_id']['link_to_contact'] = 0;
  /* Field: CRM Core Contact: Household */
  $handler->display->display_options['fields']['contact_name_2']['id'] = 'contact_name_2';
  $handler->display->display_options['fields']['contact_name_2']['table'] = 'field_data_contact_name';
  $handler->display->display_options['fields']['contact_name_2']['field'] = 'contact_name';
  $handler->display->display_options['fields']['contact_name_2']['relationship'] = 'field_activity_participants_target_id';
  $handler->display->display_options['fields']['contact_name_2']['label'] = 'Participant Name';
  $handler->display->display_options['fields']['contact_name_2']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['contact_name_2']['alter']['path'] = 'crm-core/contact/[contact_id]';
  $handler->display->display_options['fields']['contact_name_2']['click_sort_column'] = 'family';
  $handler->display->display_options['fields']['contact_name_2']['settings'] = array(
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
  /* Field: CRM Activity: Source */
  $handler->display->display_options['fields']['field_cmcd_source']['id'] = 'field_cmcd_source';
  $handler->display->display_options['fields']['field_cmcd_source']['table'] = 'field_data_field_cmcd_source';
  $handler->display->display_options['fields']['field_cmcd_source']['field'] = 'field_cmcd_source';
  $handler->display->display_options['fields']['field_cmcd_source']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_cmcd_source']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_cmcd_source']['alter']['path'] = 'crm-core/reports/cmcd/source-summary/[field_cmcd_source-value]';
  /* Field: CRM Core Activity: Edit link */
  $handler->display->display_options['fields']['edit_crm_activity']['id'] = 'edit_crm_activity';
  $handler->display->display_options['fields']['edit_crm_activity']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['edit_crm_activity']['field'] = 'edit_crm_activity';
  $handler->display->display_options['fields']['edit_crm_activity']['exclude'] = TRUE;
  $handler->display->display_options['fields']['edit_crm_activity']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['edit_crm_activity']['alter']['text'] = 'edit';
  $handler->display->display_options['fields']['edit_crm_activity']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['edit_crm_activity']['alter']['path'] = 'crm-core/activity/[activity-id]/edit';
  $handler->display->display_options['fields']['edit_crm_activity']['redirect'] = 0;
  /* Field: CRM Core Activity: Link */
  $handler->display->display_options['fields']['view_crm_activity']['id'] = 'view_crm_activity';
  $handler->display->display_options['fields']['view_crm_activity']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['view_crm_activity']['field'] = 'view_crm_activity';
  $handler->display->display_options['fields']['view_crm_activity']['label'] = 'View';
  $handler->display->display_options['fields']['view_crm_activity']['exclude'] = TRUE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Operations';
  $handler->display->display_options['fields']['nothing']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[view_crm_activity] | [edit_crm_activity]';
  $handler->display->display_options['path'] = 'crm-core/reports/cmcd/cmcd-details/export';
  $export['cmcd_details'] = $view;

  $view = new view();
  $view->name = 'cmcd_details_callouts';
  $view->description = 'List of the 10 most recent donations. ';
  $view->tag = 'default';
  $view->base_table = 'crm_core_activity';
  $view->human_name = 'Donation Details Callouts';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Recent donations';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view donation details report';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'created' => 'created',
    'field_cmcd_amount' => 'field_cmcd_amount',
    'activity_id' => 'activity_id',
    'field_activity_participants' => 'field_activity_participants',
    'contact_id' => 'contact_id',
    'contact_name_2' => 'contact_name_2',
    'field_cmcd_source' => 'field_cmcd_source',
  );
  $handler->display->display_options['style_options']['default'] = 'created';
  $handler->display->display_options['style_options']['info'] = array(
    'created' => array(
      'sortable' => 0,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcd_amount' => array(
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
    'field_activity_participants' => array(
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
    'contact_name_2' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcd_source' => array(
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
  $handler->display->display_options['empty']['area']['content'] = 'There are no donations stored in the system at this time.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_activity_participants_target_id_1']['id'] = 'field_activity_participants_target_id_1';
  $handler->display->display_options['relationships']['field_activity_participants_target_id_1']['table'] = 'field_data_field_activity_participants';
  $handler->display->display_options['relationships']['field_activity_participants_target_id_1']['field'] = 'field_activity_participants_target_id';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['id'] = 'field_activity_participants_target_id';
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['table'] = 'field_data_field_activity_participants';
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['field'] = 'field_activity_participants_target_id';
  /* Field: CRM Activity: Receive Date */
  $handler->display->display_options['fields']['field_cmcd_receive_date']['id'] = 'field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['table'] = 'field_data_field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['field'] = 'field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['label'] = 'Date received';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: CRM Activity: Amount */
  $handler->display->display_options['fields']['field_cmcd_amount']['id'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['table'] = 'field_data_field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['field'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_cmcd_amount']['alter']['text'] = '$[field_cmcd_amount]';
  $handler->display->display_options['fields']['field_cmcd_amount']['settings'] = array(
    'thousand_separator' => ',',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: CRM Core Activity: Activity ID */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['exclude'] = TRUE;
  /* Field: CRM Activity: Participants */
  $handler->display->display_options['fields']['field_activity_participants']['id'] = 'field_activity_participants';
  $handler->display->display_options['fields']['field_activity_participants']['table'] = 'field_data_field_activity_participants';
  $handler->display->display_options['fields']['field_activity_participants']['field'] = 'field_activity_participants';
  $handler->display->display_options['fields']['field_activity_participants']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_activity_participants']['settings'] = array(
    'link' => 0,
  );
  $handler->display->display_options['fields']['field_activity_participants']['delta_limit'] = '1';
  $handler->display->display_options['fields']['field_activity_participants']['delta_offset'] = '0';
  /* Field: CRM Contact: Contact ID */
  $handler->display->display_options['fields']['contact_id']['id'] = 'contact_id';
  $handler->display->display_options['fields']['contact_id']['table'] = 'crm_core_contact';
  $handler->display->display_options['fields']['contact_id']['field'] = 'contact_id';
  $handler->display->display_options['fields']['contact_id']['relationship'] = 'field_activity_participants_target_id_1';
  $handler->display->display_options['fields']['contact_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['contact_id']['link_to_contact'] = 0;
  /* Field: CRM Core Contact: Household */
  $handler->display->display_options['fields']['contact_name_2']['id'] = 'contact_name_2';
  $handler->display->display_options['fields']['contact_name_2']['table'] = 'field_data_contact_name';
  $handler->display->display_options['fields']['contact_name_2']['field'] = 'contact_name';
  $handler->display->display_options['fields']['contact_name_2']['relationship'] = 'field_activity_participants_target_id';
  $handler->display->display_options['fields']['contact_name_2']['label'] = 'Participant Name';
  $handler->display->display_options['fields']['contact_name_2']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['contact_name_2']['alter']['path'] = 'crm-core/contact/[contact_id]';
  $handler->display->display_options['fields']['contact_name_2']['click_sort_column'] = 'family';
  $handler->display->display_options['fields']['contact_name_2']['settings'] = array(
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
  /* Field: CRM Activity: Source */
  $handler->display->display_options['fields']['field_cmcd_source']['id'] = 'field_cmcd_source';
  $handler->display->display_options['fields']['field_cmcd_source']['table'] = 'field_data_field_cmcd_source';
  $handler->display->display_options['fields']['field_cmcd_source']['field'] = 'field_cmcd_source';
  $handler->display->display_options['fields']['field_cmcd_source']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_cmcd_source']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_cmcd_source']['alter']['path'] = 'crm-core/reports/cmcd/donation-sources/summary/[field_cmcd_source-value]';
  /* Sort criterion: CRM Activity: Receive Date (field_cmcd_receive_date) */
  $handler->display->display_options['sorts']['field_cmcd_receive_date_value']['id'] = 'field_cmcd_receive_date_value';
  $handler->display->display_options['sorts']['field_cmcd_receive_date_value']['table'] = 'field_data_field_cmcd_receive_date';
  $handler->display->display_options['sorts']['field_cmcd_receive_date_value']['field'] = 'field_cmcd_receive_date_value';
  $handler->display->display_options['sorts']['field_cmcd_receive_date_value']['order'] = 'DESC';
  /* Filter criterion: CRM Core Activity: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'crm_core_activity';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'cmcd_donation' => 'cmcd_donation',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['pane_title'] = 'Recent donations';
  $handler->display->display_options['pane_category']['name'] = 'CRM Core Donation';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $export['cmcd_details_callouts'] = $view;

  $view = new view();
  $view->name = 'cmcd_donors_recent_donors';
  $view->description = 'New donors in the system';
  $view->tag = 'default';
  $view->base_table = 'crm_core_contact';
  $view->human_name = 'Donors Recent Donors';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view donor reports';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'contact_id' => 'contact_id',
    'contact_name' => 'contact_name',
    'field_cmcd_receive_date' => 'field_cmcd_receive_date',
  );
  $handler->display->display_options['style_options']['default'] = 'field_cmcd_receive_date';
  $handler->display->display_options['style_options']['info'] = array(
    'contact_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'contact_name' => array(
      'sortable' => 0,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcd_receive_date' => array(
      'sortable' => 0,
      'default_sort_order' => 'desc',
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
  $handler->display->display_options['fields']['contact_id']['link_to_contact'] = 0;
  /* Field: CRM Core Contact: Household */
  $handler->display->display_options['fields']['contact_name']['id'] = 'contact_name';
  $handler->display->display_options['fields']['contact_name']['table'] = 'field_data_contact_name';
  $handler->display->display_options['fields']['contact_name']['field'] = 'contact_name';
  $handler->display->display_options['fields']['contact_name']['label'] = 'Donor';
  $handler->display->display_options['fields']['contact_name']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['contact_name']['alter']['path'] = 'crm-core/contact/[contact_id]';
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
  /* Field: MIN(CRM Activity: Receive Date) */
  $handler->display->display_options['fields']['field_cmcd_receive_date']['id'] = 'field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['table'] = 'field_data_field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['field'] = 'field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['relationship'] = 'reverse_field_activity_participants_crm_core_activity';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['group_type'] = 'min';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  $handler->display->display_options['fields']['field_cmcd_receive_date']['group_column'] = 'entity_id';
  /* Field: SUM(CRM Activity: Amount) */
  $handler->display->display_options['fields']['field_cmcd_amount']['id'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['table'] = 'field_data_field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['field'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['relationship'] = 'reverse_field_activity_participants_crm_core_activity';
  $handler->display->display_options['fields']['field_cmcd_amount']['group_type'] = 'sum';
  $handler->display->display_options['fields']['field_cmcd_amount']['settings'] = array(
    'thousand_separator' => ',',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: CRM Activity: Source */
  $handler->display->display_options['fields']['field_cmcd_source']['id'] = 'field_cmcd_source';
  $handler->display->display_options['fields']['field_cmcd_source']['table'] = 'field_data_field_cmcd_source';
  $handler->display->display_options['fields']['field_cmcd_source']['field'] = 'field_cmcd_source';
  $handler->display->display_options['fields']['field_cmcd_source']['relationship'] = 'reverse_field_activity_participants_crm_core_activity';
  /* Sort criterion: MIN(CRM Activity: Receive Date (field_cmcd_receive_date)) */
  $handler->display->display_options['sorts']['field_cmcd_receive_date_value']['id'] = 'field_cmcd_receive_date_value';
  $handler->display->display_options['sorts']['field_cmcd_receive_date_value']['table'] = 'field_data_field_cmcd_receive_date';
  $handler->display->display_options['sorts']['field_cmcd_receive_date_value']['field'] = 'field_cmcd_receive_date_value';
  $handler->display->display_options['sorts']['field_cmcd_receive_date_value']['relationship'] = 'reverse_field_activity_participants_crm_core_activity';
  $handler->display->display_options['sorts']['field_cmcd_receive_date_value']['group_type'] = 'min';
  $handler->display->display_options['sorts']['field_cmcd_receive_date_value']['order'] = 'DESC';
  /* Filter criterion: CRM Core Activity: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'crm_core_activity';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['relationship'] = 'reverse_field_activity_participants_crm_core_activity';
  $handler->display->display_options['filters']['type']['value'] = array(
    'min' => '',
    'max' => '',
    'value' => '',
    'cmcd_donation' => 'cmcd_donation',
  );
  $export['cmcd_donors_recent_donors'] = $view;

  $view = new view();
  $view->name = 'cmcd_page_details';
  $view->description = 'Details report for specific donation pages.';
  $view->tag = 'default';
  $view->base_table = 'crm_core_activity';
  $view->human_name = 'Online Donations';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Online Donations';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view donation page details report';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'created' => 'created',
    'field_cmcd_amount' => 'field_cmcd_amount',
    'activity_id' => 'activity_id',
    'field_activity_participants' => 'field_activity_participants',
    'contact_id' => 'contact_id',
    'contact_name_2' => 'contact_name_2',
    'field_cmcd_source' => 'field_cmcd_source',
    'field_cmcd_page' => 'field_cmcd_page',
    'edit_crm_activity' => 'edit_crm_activity',
    'view_crm_activity' => 'view_crm_activity',
    'nothing' => 'nothing',
  );
  $handler->display->display_options['style_options']['default'] = 'created';
  $handler->display->display_options['style_options']['info'] = array(
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcd_amount' => array(
      'sortable' => 1,
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
    'field_activity_participants' => array(
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
    'contact_name_2' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcd_source' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcd_page' => array(
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
  $handler->display->display_options['header']['result']['content'] = '<div class="results-summary">Displaying @start - @end of @total.</div>';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '<div class="detail-buttons"><div class="btn btn-blue">[nothing_2]</div><div class="btn">[nothing_3]</div></div>';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  $handler->display->display_options['header']['area']['tokenize'] = TRUE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No results.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_activity_participants_target_id_1']['id'] = 'field_activity_participants_target_id_1';
  $handler->display->display_options['relationships']['field_activity_participants_target_id_1']['table'] = 'field_data_field_activity_participants';
  $handler->display->display_options['relationships']['field_activity_participants_target_id_1']['field'] = 'field_activity_participants_target_id';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['id'] = 'field_activity_participants_target_id';
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['table'] = 'field_data_field_activity_participants';
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['field'] = 'field_activity_participants_target_id';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_cmcd_page_target_id']['id'] = 'field_cmcd_page_target_id';
  $handler->display->display_options['relationships']['field_cmcd_page_target_id']['table'] = 'field_data_field_cmcd_page';
  $handler->display->display_options['relationships']['field_cmcd_page_target_id']['field'] = 'field_cmcd_page_target_id';
  $handler->display->display_options['relationships']['field_cmcd_page_target_id']['label'] = 'Donation page node referenced by field_cmcd_page';
  /* Field: CRM Core Activity: Date created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'long';
  /* Field: CRM Activity: Amount */
  $handler->display->display_options['fields']['field_cmcd_amount']['id'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['table'] = 'field_data_field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['field'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['type'] = 'crm_core_donation_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['settings'] = array(
    'thousand_separator' => ',',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: CRM Core Activity: Activity ID */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['exclude'] = TRUE;
  /* Field: CRM Activity: Participants */
  $handler->display->display_options['fields']['field_activity_participants']['id'] = 'field_activity_participants';
  $handler->display->display_options['fields']['field_activity_participants']['table'] = 'field_data_field_activity_participants';
  $handler->display->display_options['fields']['field_activity_participants']['field'] = 'field_activity_participants';
  $handler->display->display_options['fields']['field_activity_participants']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_activity_participants']['settings'] = array(
    'link' => 0,
  );
  $handler->display->display_options['fields']['field_activity_participants']['delta_limit'] = '1';
  $handler->display->display_options['fields']['field_activity_participants']['delta_offset'] = '0';
  /* Field: CRM Contact: Contact ID */
  $handler->display->display_options['fields']['contact_id']['id'] = 'contact_id';
  $handler->display->display_options['fields']['contact_id']['table'] = 'crm_core_contact';
  $handler->display->display_options['fields']['contact_id']['field'] = 'contact_id';
  $handler->display->display_options['fields']['contact_id']['relationship'] = 'field_activity_participants_target_id_1';
  $handler->display->display_options['fields']['contact_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['contact_id']['link_to_contact'] = 0;
  /* Field: CRM Core Contact: Household */
  $handler->display->display_options['fields']['contact_name_2']['id'] = 'contact_name_2';
  $handler->display->display_options['fields']['contact_name_2']['table'] = 'field_data_contact_name';
  $handler->display->display_options['fields']['contact_name_2']['field'] = 'contact_name';
  $handler->display->display_options['fields']['contact_name_2']['relationship'] = 'field_activity_participants_target_id';
  $handler->display->display_options['fields']['contact_name_2']['label'] = 'Donor';
  $handler->display->display_options['fields']['contact_name_2']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['contact_name_2']['alter']['path'] = 'crm-core/contact/[contact_id]';
  $handler->display->display_options['fields']['contact_name_2']['click_sort_column'] = 'family';
  $handler->display->display_options['fields']['contact_name_2']['settings'] = array(
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
  /* Field: CRM Activity: Donation Page */
  $handler->display->display_options['fields']['field_cmcd_page']['id'] = 'field_cmcd_page';
  $handler->display->display_options['fields']['field_cmcd_page']['table'] = 'field_data_field_cmcd_page';
  $handler->display->display_options['fields']['field_cmcd_page']['field'] = 'field_cmcd_page';
  $handler->display->display_options['fields']['field_cmcd_page']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_cmcd_page']['alter']['path'] = 'crm-core/reports/cmcd/donation-pages/summary/[field_cmcd_page-target_id] ';
  $handler->display->display_options['fields']['field_cmcd_page']['settings'] = array(
    'link' => 0,
  );
  /* Field: CRM Core Activity: Edit link */
  $handler->display->display_options['fields']['edit_crm_activity']['id'] = 'edit_crm_activity';
  $handler->display->display_options['fields']['edit_crm_activity']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['edit_crm_activity']['field'] = 'edit_crm_activity';
  $handler->display->display_options['fields']['edit_crm_activity']['exclude'] = TRUE;
  $handler->display->display_options['fields']['edit_crm_activity']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['edit_crm_activity']['alter']['text'] = 'edit';
  $handler->display->display_options['fields']['edit_crm_activity']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['edit_crm_activity']['alter']['path'] = 'crm-core/activity/[activity_id]/edit';
  $handler->display->display_options['fields']['edit_crm_activity']['redirect'] = 0;
  /* Field: CRM Core Activity: Link */
  $handler->display->display_options['fields']['view_crm_activity']['id'] = 'view_crm_activity';
  $handler->display->display_options['fields']['view_crm_activity']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['view_crm_activity']['field'] = 'view_crm_activity';
  $handler->display->display_options['fields']['view_crm_activity']['label'] = 'View';
  $handler->display->display_options['fields']['view_crm_activity']['exclude'] = TRUE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Operations';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<div class="btn btn-blue">[view_crm_activity]</div> <div class="btn">[edit_crm_activity]</div>';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'View paged results';
  $handler->display->display_options['fields']['nothing_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'View paged results';
  $handler->display->display_options['fields']['nothing_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['path'] = 'crm-core/reports/cmcd/donation-pages/details';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
  $handler->display->display_options['fields']['nothing_2']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_2']['label'] = 'View All';
  $handler->display->display_options['fields']['nothing_2']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['text'] = 'View All';
  $handler->display->display_options['fields']['nothing_2']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['path'] = 'crm-core/reports/cmcd/cmcd-page-details/full';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_3']['id'] = 'nothing_3';
  $handler->display->display_options['fields']['nothing_3']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_3']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_3']['label'] = 'Export to CSV';
  $handler->display->display_options['fields']['nothing_3']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_3']['alter']['text'] = 'Export to CSV';
  $handler->display->display_options['fields']['nothing_3']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_3']['alter']['path'] = 'crm-core/reports/cmcd/cmcd-page-details/export';
  /* Filter criterion: CRM Core Activity: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'crm_core_activity';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'cmcd_donation' => 'cmcd_donation',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: CRM Activity: Amount (field_cmcd_amount) */
  $handler->display->display_options['filters']['field_cmcd_amount_value']['id'] = 'field_cmcd_amount_value';
  $handler->display->display_options['filters']['field_cmcd_amount_value']['table'] = 'field_data_field_cmcd_amount';
  $handler->display->display_options['filters']['field_cmcd_amount_value']['field'] = 'field_cmcd_amount_value';
  $handler->display->display_options['filters']['field_cmcd_amount_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_cmcd_amount_value']['group'] = 1;
  $handler->display->display_options['filters']['field_cmcd_amount_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_cmcd_amount_value']['expose']['operator_id'] = 'field_cmcd_amount_value_op';
  $handler->display->display_options['filters']['field_cmcd_amount_value']['expose']['label'] = 'Filter by amounts between';
  $handler->display->display_options['filters']['field_cmcd_amount_value']['expose']['operator'] = 'field_cmcd_amount_value_op';
  $handler->display->display_options['filters']['field_cmcd_amount_value']['expose']['identifier'] = 'field_cmcd_amount_value';
  $handler->display->display_options['filters']['field_cmcd_amount_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: CRM Activity: Receive Date (field_cmcd_receive_date) */
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['id'] = 'field_cmcd_receive_date_value';
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['table'] = 'field_data_field_cmcd_receive_date';
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['field'] = 'field_cmcd_receive_date_value';
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['group'] = 1;
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['expose']['operator_id'] = 'field_cmcd_receive_date_value_op';
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['expose']['label'] = 'Filter by dates between';
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['expose']['operator'] = 'field_cmcd_receive_date_value_op';
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['expose']['identifier'] = 'field_cmcd_receive_date_value';
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['year_range'] = '-0:+0';
  /* Filter criterion: CRM Activity: Source (field_cmcd_source) */
  $handler->display->display_options['filters']['field_cmcd_source_value']['id'] = 'field_cmcd_source_value';
  $handler->display->display_options['filters']['field_cmcd_source_value']['table'] = 'field_data_field_cmcd_source';
  $handler->display->display_options['filters']['field_cmcd_source_value']['field'] = 'field_cmcd_source_value';
  $handler->display->display_options['filters']['field_cmcd_source_value']['value'] = array(
    'Online Donation' => 'Online Donation',
  );
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['relationship'] = 'field_cmcd_page_target_id';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Filter by Donation Page';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );

  /* Display: Paged Results */
  $handler = $view->new_display('page', 'Paged Results', 'page');
  $handler->display->display_options['display_comment'] = 'This view provides a detailed listing of all donations in the system.';
  $handler->display->display_options['display_description'] = 'This shows 50 items per page.';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['path'] = 'crm-core/reports/cmcd/donation-pages/details';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Online Donations';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Full Results */
  $handler = $view->new_display('page', 'Full Results', 'page_1');
  $handler->display->display_options['display_description'] = 'Full detailed report, no filters.';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
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
  $handler->display->display_options['header']['area']['content'] = '<div class="detail-buttons"><div class="btn btn-blue btn-simple">[nothing_1]</div><div class="btn btn-blue btn-simple">[nothing_3]</div></div>';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  $handler->display->display_options['header']['area']['tokenize'] = TRUE;
  $handler->display->display_options['path'] = 'crm-core/reports/cmcd/cmcd-page-details/full';

  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: CRM Core Activity: Date created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'long';
  /* Field: CRM Activity: Amount */
  $handler->display->display_options['fields']['field_cmcd_amount']['id'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['table'] = 'field_data_field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['field'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['type'] = 'crm_core_donation_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['settings'] = array(
    'thousand_separator' => ',',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: CRM Core Activity: Activity ID */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['exclude'] = TRUE;
  /* Field: CRM Activity: Participants */
  $handler->display->display_options['fields']['field_activity_participants']['id'] = 'field_activity_participants';
  $handler->display->display_options['fields']['field_activity_participants']['table'] = 'field_data_field_activity_participants';
  $handler->display->display_options['fields']['field_activity_participants']['field'] = 'field_activity_participants';
  $handler->display->display_options['fields']['field_activity_participants']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_activity_participants']['settings'] = array(
    'link' => 0,
  );
  $handler->display->display_options['fields']['field_activity_participants']['delta_limit'] = '1';
  $handler->display->display_options['fields']['field_activity_participants']['delta_offset'] = '0';
  /* Field: CRM Contact: Contact ID */
  $handler->display->display_options['fields']['contact_id']['id'] = 'contact_id';
  $handler->display->display_options['fields']['contact_id']['table'] = 'crm_core_contact';
  $handler->display->display_options['fields']['contact_id']['field'] = 'contact_id';
  $handler->display->display_options['fields']['contact_id']['relationship'] = 'field_activity_participants_target_id_1';
  $handler->display->display_options['fields']['contact_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['contact_id']['link_to_contact'] = 0;
  /* Field: CRM Core Contact: Household */
  $handler->display->display_options['fields']['contact_name_2']['id'] = 'contact_name_2';
  $handler->display->display_options['fields']['contact_name_2']['table'] = 'field_data_contact_name';
  $handler->display->display_options['fields']['contact_name_2']['field'] = 'contact_name';
  $handler->display->display_options['fields']['contact_name_2']['relationship'] = 'field_activity_participants_target_id';
  $handler->display->display_options['fields']['contact_name_2']['label'] = 'Participant Name';
  $handler->display->display_options['fields']['contact_name_2']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['contact_name_2']['alter']['path'] = 'crm-core/contact/[contact_id]';
  $handler->display->display_options['fields']['contact_name_2']['click_sort_column'] = 'family';
  $handler->display->display_options['fields']['contact_name_2']['settings'] = array(
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
  /* Field: CRM Activity: Source */
  $handler->display->display_options['fields']['field_cmcd_source']['id'] = 'field_cmcd_source';
  $handler->display->display_options['fields']['field_cmcd_source']['table'] = 'field_data_field_cmcd_source';
  $handler->display->display_options['fields']['field_cmcd_source']['field'] = 'field_cmcd_source';
  $handler->display->display_options['fields']['field_cmcd_source']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_cmcd_source']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_cmcd_source']['alter']['path'] = 'crm-core/reports/cmcd/source-summary/[field_cmcd_source-value]';
  /* Field: CRM Core Activity: Edit link */
  $handler->display->display_options['fields']['edit_crm_activity']['id'] = 'edit_crm_activity';
  $handler->display->display_options['fields']['edit_crm_activity']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['edit_crm_activity']['field'] = 'edit_crm_activity';
  $handler->display->display_options['fields']['edit_crm_activity']['exclude'] = TRUE;
  $handler->display->display_options['fields']['edit_crm_activity']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['edit_crm_activity']['alter']['text'] = 'edit';
  $handler->display->display_options['fields']['edit_crm_activity']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['edit_crm_activity']['alter']['path'] = 'crm-core/activity/[activity-id]/edit';
  $handler->display->display_options['fields']['edit_crm_activity']['redirect'] = 0;
  /* Field: CRM Core Activity: Link */
  $handler->display->display_options['fields']['view_crm_activity']['id'] = 'view_crm_activity';
  $handler->display->display_options['fields']['view_crm_activity']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['view_crm_activity']['field'] = 'view_crm_activity';
  $handler->display->display_options['fields']['view_crm_activity']['label'] = 'View';
  $handler->display->display_options['fields']['view_crm_activity']['exclude'] = TRUE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Operations';
  $handler->display->display_options['fields']['nothing']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[view_crm_activity] | [edit_crm_activity]';
  $handler->display->display_options['path'] = 'crm-core/reports/cmcd/cmcd-page-details/export';
  $export['cmcd_page_details'] = $view;

  $view = new view();
  $view->name = 'cmcd_page_manager';
  $view->description = 'Used for managing donation pages in CRM Core.';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Donation Page Manager';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Manage Donation Pages';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
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
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Name';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: CRM Core Profile: CRM Core Profile name */
  $handler->display->display_options['fields']['profile_name']['id'] = 'profile_name';
  $handler->display->display_options['fields']['profile_name']['table'] = 'crm_core_profile_nodes';
  $handler->display->display_options['fields']['profile_name']['field'] = 'profile_name';
  $handler->display->display_options['fields']['profile_name']['exclude'] = TRUE;
  /* Field: CRM Core Profile: Label */
  $handler->display->display_options['fields']['label']['id'] = 'label';
  $handler->display->display_options['fields']['label']['table'] = 'crm_core_profile';
  $handler->display->display_options['fields']['label']['field'] = 'label';
  $handler->display->display_options['fields']['label']['label'] = 'Donation Form';
  $handler->display->display_options['fields']['label']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['label']['alter']['path'] = 'admin/structure/crm-core/profile/manage/[profile_name]';
  $handler->display->display_options['fields']['label']['empty'] = '---';
  /* Field: CRM Core Profile: Machine name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'crm_core_profile';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['empty'] = '---';
  /* Field: CRM Core Profile: Profile status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'crm_core_profile';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = 'Enabled?';
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Field: CRM Core Profile: Page redirect */
  $handler->display->display_options['fields']['page_redirect']['id'] = 'page_redirect';
  $handler->display->display_options['fields']['page_redirect']['table'] = 'crm_core_profile';
  $handler->display->display_options['fields']['page_redirect']['field'] = 'page_redirect';
  $handler->display->display_options['fields']['page_redirect']['label'] = 'Redirect Path';
  $handler->display->display_options['fields']['page_redirect']['empty'] = '---';
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
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
    'cmcd_page' => 'cmcd_page',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'crm-core/cmcd-pages';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Manage Donation Pages';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'crm-core-admin-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['cmcd_page_manager'] = $view;

  $view = new view();
  $view->name = 'cmcd_source_callouts';
  $view->description = 'Provides an overview of donation sources and links to detail reports.';
  $view->tag = 'default';
  $view->base_table = 'crm_core_activity';
  $view->human_name = 'Donation Source Callouts';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Top sources';
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
    'field_cmcd_source' => 'field_cmcd_source',
    'field_cmcd_amount' => 'field_cmcd_amount',
    'activity_id' => 'activity_id',
    'field_cmcd_receive_date' => 'field_cmcd_receive_date',
  );
  $handler->display->display_options['style_options']['default'] = 'activity_id';
  $handler->display->display_options['style_options']['info'] = array(
    'field_cmcd_source' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcd_amount' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'activity_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcd_receive_date' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: CRM Activity: Source */
  $handler->display->display_options['fields']['field_cmcd_source']['id'] = 'field_cmcd_source';
  $handler->display->display_options['fields']['field_cmcd_source']['table'] = 'field_data_field_cmcd_source';
  $handler->display->display_options['fields']['field_cmcd_source']['field'] = 'field_cmcd_source';
  $handler->display->display_options['fields']['field_cmcd_source']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_cmcd_source']['alter']['path'] = 'crm-core/reports/cmcd/donation-sources/summary/[field_cmcd_source]';
  /* Field: SUM(CRM Activity: Amount) */
  $handler->display->display_options['fields']['field_cmcd_amount']['id'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['table'] = 'field_data_field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['field'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['group_type'] = 'sum';
  $handler->display->display_options['fields']['field_cmcd_amount']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_cmcd_amount']['alter']['text'] = '$[field_cmcd_amount]';
  $handler->display->display_options['fields']['field_cmcd_amount']['settings'] = array(
    'thousand_separator' => ',',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: COUNT(CRM Core Activity: Activity ID) */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['group_type'] = 'count';
  $handler->display->display_options['fields']['activity_id']['label'] = 'Total Donations';
  /* Field: MAX(CRM Activity: Receive Date) */
  $handler->display->display_options['fields']['field_cmcd_receive_date']['id'] = 'field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['table'] = 'field_data_field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['field'] = 'field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['group_type'] = 'max';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['label'] = 'Last Donation';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Filter criterion: CRM Core Activity: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'crm_core_activity';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'cmcd_donation' => 'cmcd_donation',
  );

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['pane_title'] = 'Top sources';
  $handler->display->display_options['pane_category']['name'] = 'CRM Core Donation';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $export['cmcd_source_callouts'] = $view;

  $view = new view();
  $view->name = 'cmcd_source_report';
  $view->description = 'Provides an overview of donation sources and links to detail reports.';
  $view->tag = 'default';
  $view->base_table = 'crm_core_activity';
  $view->human_name = 'Donation Source Report';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Donation Sources';
  $handler->display->display_options['css_class'] = 'fancy-headers';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view donation source summary report';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_cmcd_source' => 'field_cmcd_source',
    'field_cmcd_amount' => 'field_cmcd_amount',
    'activity_id' => 'activity_id',
    'field_cmcd_amount_1' => 'field_cmcd_amount_1',
    'field_cmcd_receive_date' => 'field_cmcd_receive_date',
  );
  $handler->display->display_options['style_options']['default'] = 'field_cmcd_source';
  $handler->display->display_options['style_options']['info'] = array(
    'field_cmcd_source' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcd_amount' => array(
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
    'field_cmcd_amount_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcd_receive_date' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: CRM Activity: Source */
  $handler->display->display_options['fields']['field_cmcd_source']['id'] = 'field_cmcd_source';
  $handler->display->display_options['fields']['field_cmcd_source']['table'] = 'field_data_field_cmcd_source';
  $handler->display->display_options['fields']['field_cmcd_source']['field'] = 'field_cmcd_source';
  $handler->display->display_options['fields']['field_cmcd_source']['element_class'] = 'field-object';
  /* Field: SUM(CRM Activity: Amount) */
  $handler->display->display_options['fields']['field_cmcd_amount']['id'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['table'] = 'field_data_field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['field'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['group_type'] = 'sum';
  $handler->display->display_options['fields']['field_cmcd_amount']['type'] = 'crm_core_donation_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['settings'] = array(
    'thousand_separator' => ',',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: COUNT(CRM Core Activity: Activity ID) */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['group_type'] = 'count';
  $handler->display->display_options['fields']['activity_id']['label'] = 'Total Donations';
  /* Field: AVG(CRM Activity: Amount) */
  $handler->display->display_options['fields']['field_cmcd_amount_1']['id'] = 'field_cmcd_amount_1';
  $handler->display->display_options['fields']['field_cmcd_amount_1']['table'] = 'field_data_field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount_1']['field'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount_1']['group_type'] = 'avg';
  $handler->display->display_options['fields']['field_cmcd_amount_1']['label'] = 'Average Amount';
  $handler->display->display_options['fields']['field_cmcd_amount_1']['type'] = 'crm_core_donation_amount';
  $handler->display->display_options['fields']['field_cmcd_amount_1']['settings'] = array(
    'thousand_separator' => ' ',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: MAX(CRM Activity: Receive Date) */
  $handler->display->display_options['fields']['field_cmcd_receive_date']['id'] = 'field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['table'] = 'field_data_field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['field'] = 'field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['group_type'] = 'max';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['label'] = 'Latest donation';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['settings'] = array(
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
  $handler->display->display_options['fields']['nothing_1']['label'] = 'Summary Report';
  $handler->display->display_options['fields']['nothing_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'Summary';
  $handler->display->display_options['fields']['nothing_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['path'] = 'crm-core/reports/cmcd/donation-sources/summary/[field_cmcd_source]';
  $handler->display->display_options['fields']['nothing_1']['element_label_colon'] = FALSE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
  $handler->display->display_options['fields']['nothing_2']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_2']['label'] = 'Details';
  $handler->display->display_options['fields']['nothing_2']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['text'] = 'Details';
  $handler->display->display_options['fields']['nothing_2']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['path'] = 'crm-core/reports/cmcd/cmcd-details?field_cmcd_source_value[]=[field_cmcd_source]';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Reports';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<div class="btn btn-blue">[nothing_1]</div> <div class="btn btn-blue">[nothing_2]</div> 
';
  /* Filter criterion: CRM Core Activity: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'crm_core_activity';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'cmcd_donation' => 'cmcd_donation',
  );

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['pane_title'] = 'Donations by Source: Source Table';
  $handler->display->display_options['pane_description'] = 'A list of donation sources with total amounts raised by source.';
  $handler->display->display_options['pane_category']['name'] = 'CRM Core Donations';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $export['cmcd_source_report'] = $view;

  $view = new view();
  $view->name = 'cmcd_source_total_figures';
  $view->description = 'Provides high-level figures for a specified source';
  $view->tag = 'default';
  $view->base_table = 'crm_core_activity';
  $view->human_name = 'Donations Source Total Figures';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['css_class'] = 'summary-table';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view donation source summary report';
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
  $handler->display->display_options['fields']['activity_id']['label'] = 'Total Donations';
  /* Field: SUM(CRM Activity: Amount) */
  $handler->display->display_options['fields']['field_cmcd_amount']['id'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['table'] = 'field_data_field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['field'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['group_type'] = 'sum';
  $handler->display->display_options['fields']['field_cmcd_amount']['label'] = 'Total Amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['type'] = 'crm_core_donation_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['settings'] = array(
    'thousand_separator' => ',',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: AVG(CRM Activity: Amount) */
  $handler->display->display_options['fields']['field_cmcd_amount_1']['id'] = 'field_cmcd_amount_1';
  $handler->display->display_options['fields']['field_cmcd_amount_1']['table'] = 'field_data_field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount_1']['field'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount_1']['group_type'] = 'avg';
  $handler->display->display_options['fields']['field_cmcd_amount_1']['label'] = 'Average Amount';
  $handler->display->display_options['fields']['field_cmcd_amount_1']['type'] = 'crm_core_donation_amount';
  $handler->display->display_options['fields']['field_cmcd_amount_1']['settings'] = array(
    'thousand_separator' => ',',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Contextual filter: CRM Activity: Source (field_cmcd_source) */
  $handler->display->display_options['arguments']['field_cmcd_source_value']['id'] = 'field_cmcd_source_value';
  $handler->display->display_options['arguments']['field_cmcd_source_value']['table'] = 'field_data_field_cmcd_source';
  $handler->display->display_options['arguments']['field_cmcd_source_value']['field'] = 'field_cmcd_source_value';
  $handler->display->display_options['arguments']['field_cmcd_source_value']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['field_cmcd_source_value']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_cmcd_source_value']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_cmcd_source_value']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_cmcd_source_value']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_cmcd_source_value']['limit'] = '0';
  /* Filter criterion: CRM Core Activity: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'crm_core_activity';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'cmcd_donation' => 'cmcd_donation',
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block_1');
  $handler->display->display_options['block_description'] = 'Donation Source Totals';

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['argument_input'] = array(
    'field_cmcd_source_value' => array(
      'type' => 'panel',
      'context' => 'entity:crm_core_activity.activity-id',
      'context_optional' => 0,
      'panel' => '0',
      'fixed' => '',
      'label' => 'CRM Activity: Source (field_cmcd_source)',
    ),
  );
  $export['cmcd_source_total_figures'] = $view;

  $view = new view();
  $view->name = 'cmcd_total_figures';
  $view->description = 'Provides high-level figures for a specified source';
  $view->tag = 'default';
  $view->base_table = 'crm_core_activity';
  $view->human_name = 'Donations Total Figures';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Donations Summary';
  $handler->display->display_options['css_class'] = 'summary-table';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view donation source summary report';
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
  $handler->display->display_options['fields']['activity_id']['label'] = 'Total Donations';
  /* Field: SUM(CRM Activity: Amount) */
  $handler->display->display_options['fields']['field_cmcd_amount']['id'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['table'] = 'field_data_field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['field'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['group_type'] = 'sum';
  $handler->display->display_options['fields']['field_cmcd_amount']['label'] = 'Total Amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['alter']['text'] = '$[field_cmcd_amount] ';
  $handler->display->display_options['fields']['field_cmcd_amount']['type'] = 'crm_core_donation_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['settings'] = array(
    'thousand_separator' => ',',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: AVG(CRM Activity: Amount) */
  $handler->display->display_options['fields']['field_cmcd_amount_1']['id'] = 'field_cmcd_amount_1';
  $handler->display->display_options['fields']['field_cmcd_amount_1']['table'] = 'field_data_field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount_1']['field'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount_1']['group_type'] = 'avg';
  $handler->display->display_options['fields']['field_cmcd_amount_1']['label'] = 'Average Amount';
  $handler->display->display_options['fields']['field_cmcd_amount_1']['alter']['text'] = '$[field_cmcd_amount_1]';
  $handler->display->display_options['fields']['field_cmcd_amount_1']['type'] = 'crm_core_donation_amount';
  $handler->display->display_options['fields']['field_cmcd_amount_1']['settings'] = array(
    'thousand_separator' => ',',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Contextual filter: CRM Activity: Donation Page (field_cmcd_page) */
  $handler->display->display_options['arguments']['field_cmcd_page_target_id']['id'] = 'field_cmcd_page_target_id';
  $handler->display->display_options['arguments']['field_cmcd_page_target_id']['table'] = 'field_data_field_cmcd_page';
  $handler->display->display_options['arguments']['field_cmcd_page_target_id']['field'] = 'field_cmcd_page_target_id';
  $handler->display->display_options['arguments']['field_cmcd_page_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_cmcd_page_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_cmcd_page_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_cmcd_page_target_id']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: CRM Core Activity: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'crm_core_activity';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'cmcd_donation' => 'cmcd_donation',
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block_1');
  $handler->display->display_options['block_description'] = 'Donation Source Totals';

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['pane_title'] = 'Donation Overview Widget';
  $handler->display->display_options['pane_description'] = 'Provides some high level details around donations, and how much has been collected through the system.';
  $handler->display->display_options['pane_category']['name'] = 'CRM Core Donations';
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
    'field_cmcd_page_target_id' => array(
      'type' => 'none',
      'context' => 'entity:crm_core_activity.activity-id',
      'context_optional' => 0,
      'panel' => '0',
      'fixed' => '',
      'label' => 'CRM Activity: Donation Page (field_cmcd_page)',
    ),
  );
  $export['cmcd_total_figures'] = $view;

  $view = new view();
  $view->name = 'donation_page_overview';
  $view->description = 'Presents details about donation page performance and links to reports about donation pages.';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Donation Page Overview';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Online Donation Page Summary';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view donation page details report';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'field_cmcd_amount' => 'field_cmcd_amount',
    'activity_id' => 'activity_id',
    'field_cmcd_amount_1' => 'field_cmcd_amount_1',
    'field_cmcd_receive_date' => 'field_cmcd_receive_date',
    'nothing' => 'nothing',
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
    'field_cmcd_amount' => array(
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
    'field_cmcd_amount_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcd_receive_date' => array(
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
  /* Relationship: Entity Reference: Referencing entity */
  $handler->display->display_options['relationships']['reverse_field_cmcd_page_crm_core_activity']['id'] = 'reverse_field_cmcd_page_crm_core_activity';
  $handler->display->display_options['relationships']['reverse_field_cmcd_page_crm_core_activity']['table'] = 'node';
  $handler->display->display_options['relationships']['reverse_field_cmcd_page_crm_core_activity']['field'] = 'reverse_field_cmcd_page_crm_core_activity';
  $handler->display->display_options['relationships']['reverse_field_cmcd_page_crm_core_activity']['label'] = 'Donation Activity reference via field_cmcd_page';
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
  $handler->display->display_options['fields']['title']['element_class'] = 'field-object';
  /* Field: SUM(CRM Activity: Amount) */
  $handler->display->display_options['fields']['field_cmcd_amount']['id'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['table'] = 'field_data_field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['field'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['relationship'] = 'reverse_field_cmcd_page_crm_core_activity';
  $handler->display->display_options['fields']['field_cmcd_amount']['group_type'] = 'sum';
  $handler->display->display_options['fields']['field_cmcd_amount']['empty'] = '0';
  $handler->display->display_options['fields']['field_cmcd_amount']['type'] = 'crm_core_donation_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['settings'] = array(
    'thousand_separator' => ',',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: COUNT(CRM Core Activity: Activity ID) */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['relationship'] = 'reverse_field_cmcd_page_crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['group_type'] = 'count';
  $handler->display->display_options['fields']['activity_id']['label'] = 'Number of Donations';
  /* Field: AVG(CRM Activity: Amount) */
  $handler->display->display_options['fields']['field_cmcd_amount_1']['id'] = 'field_cmcd_amount_1';
  $handler->display->display_options['fields']['field_cmcd_amount_1']['table'] = 'field_data_field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount_1']['field'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount_1']['relationship'] = 'reverse_field_cmcd_page_crm_core_activity';
  $handler->display->display_options['fields']['field_cmcd_amount_1']['group_type'] = 'avg';
  $handler->display->display_options['fields']['field_cmcd_amount_1']['label'] = 'Average Amount';
  $handler->display->display_options['fields']['field_cmcd_amount_1']['empty'] = '0';
  $handler->display->display_options['fields']['field_cmcd_amount_1']['type'] = 'crm_core_donation_amount';
  $handler->display->display_options['fields']['field_cmcd_amount_1']['settings'] = array(
    'thousand_separator' => ',',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: MAX(CRM Activity: Receive Date) */
  $handler->display->display_options['fields']['field_cmcd_receive_date']['id'] = 'field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['table'] = 'field_data_field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['field'] = 'field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['relationship'] = 'reverse_field_cmcd_page_crm_core_activity';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['group_type'] = 'max';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['label'] = 'Last Donation';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['empty'] = '---';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['settings'] = array(
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
  $handler->display->display_options['fields']['nothing_1']['label'] = 'Summary Report Link';
  $handler->display->display_options['fields']['nothing_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'Summary';
  $handler->display->display_options['fields']['nothing_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['path'] = 'crm-core/reports/cmcd/donation-pages/summary/[nid]';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
  $handler->display->display_options['fields']['nothing_2']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_2']['label'] = 'Details Report Link';
  $handler->display->display_options['fields']['nothing_2']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['text'] = 'Details';
  $handler->display->display_options['fields']['nothing_2']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['path'] = 'crm-core/reports/cmcd/donation-pages/details?title=[title]';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Reports';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<div class="btn btn-blue btn-dark btn-simple">[nothing_1]</div> <div class="btn btn-blue btn-dark btn-simple">[nothing_2]</div> 
';
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
    'cmcd_page' => 'cmcd_page',
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block_1');
  $export['donation_page_overview'] = $view;

  $view = new view();
  $view->name = 'donations_page_total_figures';
  $view->description = 'Provides high-level figures for a specified source';
  $view->tag = 'default';
  $view->base_table = 'crm_core_activity';
  $view->human_name = 'Donations Page Total Figures';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['css_class'] = 'summary-table';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view donation source summary report';
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
  $handler->display->display_options['fields']['activity_id']['label'] = 'Total Donations';
  /* Field: SUM(CRM Activity: Amount) */
  $handler->display->display_options['fields']['field_cmcd_amount']['id'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['table'] = 'field_data_field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['field'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['group_type'] = 'sum';
  $handler->display->display_options['fields']['field_cmcd_amount']['label'] = 'Total Amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_cmcd_amount']['alter']['text'] = '$[field_cmcd_amount] ';
  $handler->display->display_options['fields']['field_cmcd_amount']['settings'] = array(
    'thousand_separator' => ',',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: AVG(CRM Activity: Amount) */
  $handler->display->display_options['fields']['field_cmcd_amount_1']['id'] = 'field_cmcd_amount_1';
  $handler->display->display_options['fields']['field_cmcd_amount_1']['table'] = 'field_data_field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount_1']['field'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount_1']['group_type'] = 'avg';
  $handler->display->display_options['fields']['field_cmcd_amount_1']['label'] = 'Average Amount';
  $handler->display->display_options['fields']['field_cmcd_amount_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_cmcd_amount_1']['alter']['text'] = '$[field_cmcd_amount_1]';
  $handler->display->display_options['fields']['field_cmcd_amount_1']['settings'] = array(
    'thousand_separator' => ',',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Contextual filter: CRM Activity: Donation Page (field_cmcd_page) */
  $handler->display->display_options['arguments']['field_cmcd_page_target_id']['id'] = 'field_cmcd_page_target_id';
  $handler->display->display_options['arguments']['field_cmcd_page_target_id']['table'] = 'field_data_field_cmcd_page';
  $handler->display->display_options['arguments']['field_cmcd_page_target_id']['field'] = 'field_cmcd_page_target_id';
  $handler->display->display_options['arguments']['field_cmcd_page_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_cmcd_page_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_cmcd_page_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_cmcd_page_target_id']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: CRM Core Activity: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'crm_core_activity';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'cmcd_donation' => 'cmcd_donation',
  );
  /* Filter criterion: CRM Activity: Source (field_cmcd_source) */
  $handler->display->display_options['filters']['field_cmcd_source_value']['id'] = 'field_cmcd_source_value';
  $handler->display->display_options['filters']['field_cmcd_source_value']['table'] = 'field_data_field_cmcd_source';
  $handler->display->display_options['filters']['field_cmcd_source_value']['field'] = 'field_cmcd_source_value';
  $handler->display->display_options['filters']['field_cmcd_source_value']['value'] = array(
    'Online Donation' => 'Online Donation',
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block_1');
  $handler->display->display_options['block_description'] = 'Donation Source Totals';

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['pane_title'] = 'Donation page: total figures';
  $handler->display->display_options['pane_description'] = 'Displays information about the total amount raised and the total number of donations for this page.';
  $handler->display->display_options['argument_input'] = array(
    'field_cmcd_page_target_id' => array(
      'type' => 'panel',
      'context' => 'entity:crm_core_activity.activity-id',
      'context_optional' => 0,
      'panel' => '0',
      'fixed' => '',
      'label' => 'CRM Activity: Donation Page (field_cmcd_page)',
    ),
  );
  $export['donations_page_total_figures'] = $view;

  $view = new view();
  $view->name = 'donor_details';
  $view->description = 'Details about donors kept in the system';
  $view->tag = 'default';
  $view->base_table = 'crm_core_activity';
  $view->human_name = 'Donor Details';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Donor Details';
  $handler->display->display_options['css_class'] = 'fancy-headers';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view donor reports';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'contact_name' => 'contact_name',
    'contact_id' => 'contact_id',
    'field_cmcd_amount' => 'field_cmcd_amount',
    'activity_id_1' => 'activity_id_1',
    'field_cmcd_receive_date' => 'field_cmcd_receive_date',
    'field_cmcd_receive_date_1' => 'field_cmcd_receive_date_1',
  );
  $handler->display->display_options['style_options']['default'] = 'field_cmcd_amount';
  $handler->display->display_options['style_options']['info'] = array(
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
    'field_cmcd_amount' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'activity_id_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcd_receive_date' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcd_receive_date_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['id'] = 'field_activity_participants_target_id';
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['table'] = 'field_data_field_activity_participants';
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['field'] = 'field_activity_participants_target_id';
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['label'] = 'Contact from Donation Record';
  /* Field: Bulk operations: CRM Contact */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'crm_core_contact';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['relationship'] = 'field_activity_participants_target_id';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'rules_component::rules_crm_core_petition_thanks_email' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
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
  /* Field: CRM Contact: Contact Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'crm_core_contact';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['relationship'] = 'field_activity_participants_target_id';
  $handler->display->display_options['fields']['type']['exclude'] = TRUE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<div class="ao_contact_icon"><span></span></div>';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  /* Field: CRM Contact: Contact ID */
  $handler->display->display_options['fields']['contact_id']['id'] = 'contact_id';
  $handler->display->display_options['fields']['contact_id']['table'] = 'crm_core_contact';
  $handler->display->display_options['fields']['contact_id']['field'] = 'contact_id';
  $handler->display->display_options['fields']['contact_id']['relationship'] = 'field_activity_participants_target_id';
  $handler->display->display_options['fields']['contact_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['contact_id']['link_to_contact'] = 0;
  /* Field: CRM Core Contact: Household */
  $handler->display->display_options['fields']['contact_name']['id'] = 'contact_name';
  $handler->display->display_options['fields']['contact_name']['table'] = 'field_data_contact_name';
  $handler->display->display_options['fields']['contact_name']['field'] = 'contact_name';
  $handler->display->display_options['fields']['contact_name']['relationship'] = 'field_activity_participants_target_id';
  $handler->display->display_options['fields']['contact_name']['label'] = 'Donor';
  $handler->display->display_options['fields']['contact_name']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['contact_name']['alter']['path'] = 'crm-core/contact/[contact_id]';
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
  /* Field: SUM(CRM Activity: Amount) */
  $handler->display->display_options['fields']['field_cmcd_amount']['id'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['table'] = 'field_data_field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['field'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['group_type'] = 'sum';
  $handler->display->display_options['fields']['field_cmcd_amount']['label'] = 'Total Amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['type'] = 'crm_core_donation_amount';
  /* Field: COUNT(CRM Core Activity: Activity ID) */
  $handler->display->display_options['fields']['activity_id_1']['id'] = 'activity_id_1';
  $handler->display->display_options['fields']['activity_id_1']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id_1']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id_1']['group_type'] = 'count';
  $handler->display->display_options['fields']['activity_id_1']['label'] = 'Number of Donations';
  /* Field: MIN(CRM Activity: Receive Date) */
  $handler->display->display_options['fields']['field_cmcd_receive_date']['id'] = 'field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['table'] = 'field_data_field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['field'] = 'field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['group_type'] = 'min';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['label'] = 'First Donation Date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_cmcd_receive_date']['alter']['path'] = 'crm-core/contact/[contact_id]/activity';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: MAX(CRM Activity: Receive Date) */
  $handler->display->display_options['fields']['field_cmcd_receive_date_1']['id'] = 'field_cmcd_receive_date_1';
  $handler->display->display_options['fields']['field_cmcd_receive_date_1']['table'] = 'field_data_field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date_1']['field'] = 'field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date_1']['group_type'] = 'max';
  $handler->display->display_options['fields']['field_cmcd_receive_date_1']['label'] = 'Last Donation Date';
  $handler->display->display_options['fields']['field_cmcd_receive_date_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_cmcd_receive_date_1']['alter']['path'] = 'crm-core/contact/[contact_id]/activity';
  $handler->display->display_options['fields']['field_cmcd_receive_date_1']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Filter criterion: CRM Core Activity: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'crm_core_activity';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'cmcd_donation' => 'cmcd_donation',
  );
  /* Filter criterion: SUM(CRM Activity: Amount (field_cmcd_amount)) */
  $handler->display->display_options['filters']['field_cmcd_amount_value']['id'] = 'field_cmcd_amount_value';
  $handler->display->display_options['filters']['field_cmcd_amount_value']['table'] = 'field_data_field_cmcd_amount';
  $handler->display->display_options['filters']['field_cmcd_amount_value']['field'] = 'field_cmcd_amount_value';
  $handler->display->display_options['filters']['field_cmcd_amount_value']['group_type'] = 'sum';
  $handler->display->display_options['filters']['field_cmcd_amount_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_cmcd_amount_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_cmcd_amount_value']['expose']['operator_id'] = 'field_cmcd_amount_value_op';
  $handler->display->display_options['filters']['field_cmcd_amount_value']['expose']['label'] = 'Filter by Amount Between';
  $handler->display->display_options['filters']['field_cmcd_amount_value']['expose']['operator'] = 'field_cmcd_amount_value_op';
  $handler->display->display_options['filters']['field_cmcd_amount_value']['expose']['identifier'] = 'field_cmcd_amount_value';
  $handler->display->display_options['filters']['field_cmcd_amount_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: CRM Activity: Receive Date (field_cmcd_receive_date) */
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['id'] = 'field_cmcd_receive_date_value';
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['table'] = 'field_data_field_cmcd_receive_date';
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['field'] = 'field_cmcd_receive_date_value';
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['expose']['operator_id'] = 'field_cmcd_receive_date_value_op';
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['expose']['label'] = 'Donation Dates Betwee';
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['expose']['operator'] = 'field_cmcd_receive_date_value_op';
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['expose']['identifier'] = 'field_cmcd_receive_date_value';
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['field_cmcd_receive_date_value']['year_range'] = '-10:+0';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'crm-core/reports/cmcd-donors/details';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Donor Details';
  $handler->display->display_options['menu']['weight'] = '-9';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['donor_details'] = $view;

  $view = new view();
  $view->name = 'donor_lybunt_report';
  $view->description = 'Last Year But Unfortunately Not This Year';
  $view->tag = 'default';
  $view->base_table = 'crm_core_activity';
  $view->human_name = 'Donor LYBUNT Report';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Donor LYBUNT Report';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view donor reports';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_tags'] = array(
    0 => 'crm_core_donation_donor_lybunt',
  );
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'contact_id' => 'contact_id',
    'contact_name' => 'contact_name',
    'field_cmcd_receive_date' => 'field_cmcd_receive_date',
    'field_cmcd_amount' => 'field_cmcd_amount',
    'activity_id' => 'activity_id',
  );
  $handler->display->display_options['style_options']['default'] = 'field_cmcd_amount';
  $handler->display->display_options['style_options']['info'] = array(
    'contact_id' => array(
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
    'field_cmcd_receive_date' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcd_amount' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
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
  );
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = '<div class="ao_no_results">Happily, everyone who donated last year made a donation in the selected year. There are no records to display.</div>';
  $handler->display->display_options['empty']['area']['format'] = 'full_html';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['id'] = 'field_activity_participants_target_id';
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['table'] = 'field_data_field_activity_participants';
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['field'] = 'field_activity_participants_target_id';
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['label'] = 'Contact from Donation Record';
  /* Field: CRM Contact: Contact ID */
  $handler->display->display_options['fields']['contact_id']['id'] = 'contact_id';
  $handler->display->display_options['fields']['contact_id']['table'] = 'crm_core_contact';
  $handler->display->display_options['fields']['contact_id']['field'] = 'contact_id';
  $handler->display->display_options['fields']['contact_id']['relationship'] = 'field_activity_participants_target_id';
  $handler->display->display_options['fields']['contact_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['contact_id']['link_to_contact'] = 0;
  /* Field: CRM Core Contact: Household */
  $handler->display->display_options['fields']['contact_name']['id'] = 'contact_name';
  $handler->display->display_options['fields']['contact_name']['table'] = 'field_data_contact_name';
  $handler->display->display_options['fields']['contact_name']['field'] = 'contact_name';
  $handler->display->display_options['fields']['contact_name']['relationship'] = 'field_activity_participants_target_id';
  $handler->display->display_options['fields']['contact_name']['label'] = 'Donor';
  $handler->display->display_options['fields']['contact_name']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['contact_name']['alter']['path'] = 'crm-core/contact/[contact_id]';
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
  /* Field: MAX(CRM Activity: Receive Date) */
  $handler->display->display_options['fields']['field_cmcd_receive_date']['id'] = 'field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['table'] = 'field_data_field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['field'] = 'field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['group_type'] = 'max';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['label'] = 'Last Donation Date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_cmcd_receive_date']['alter']['path'] = 'crm-core/contact/[contact_id]/activity';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: SUM(CRM Activity: Amount) */
  $handler->display->display_options['fields']['field_cmcd_amount']['id'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['table'] = 'field_data_field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['field'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['group_type'] = 'sum';
  $handler->display->display_options['fields']['field_cmcd_amount']['label'] = 'Total Donations';
  $handler->display->display_options['fields']['field_cmcd_amount']['type'] = 'crm_core_donation_amount';
  /* Field: COUNT(CRM Core Activity: Activity ID) */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['group_type'] = 'count';
  $handler->display->display_options['fields']['activity_id']['label'] = 'Number of Donations';
  $handler->display->display_options['fields']['activity_id']['alter']['path'] = 'crm-core/contact/[activity_id_1]/activity';
  /* Filter criterion: CRM Core Activity: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'crm_core_activity';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'cmcd_donation' => 'cmcd_donation',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'crm-core/reports/cmcd-donors/lybunt';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'LYBUNT';
  $handler->display->display_options['menu']['description'] = 'Last Year But Unfortunately Not This Year';
  $handler->display->display_options['menu']['weight'] = '-8';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['donor_lybunt_report'] = $view;

  $view = new view();
  $view->name = 'donor_sybunt_report';
  $view->description = 'Last Year But Unfortunately Not This Year';
  $view->tag = 'default';
  $view->base_table = 'crm_core_activity';
  $view->human_name = 'Donor SYBUNT Report';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Donor SYBUNT Report';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view donor reports';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_tags'] = array(
    0 => 'crm_core_donation_donor_sybunt',
  );
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'contact_id' => 'contact_id',
    'contact_name' => 'contact_name',
    'field_cmcd_receive_date' => 'field_cmcd_receive_date',
    'field_cmcd_amount' => 'field_cmcd_amount',
    'activity_id' => 'activity_id',
  );
  $handler->display->display_options['style_options']['default'] = 'field_cmcd_amount';
  $handler->display->display_options['style_options']['info'] = array(
    'contact_id' => array(
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
    'field_cmcd_receive_date' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcd_amount' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
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
  );
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = '<div class="ao_no_results">Happily, everyone who has donated previously also made a donation in the selected year. There are no records to display.</div>';
  $handler->display->display_options['empty']['area']['format'] = 'full_html';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['id'] = 'field_activity_participants_target_id';
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['table'] = 'field_data_field_activity_participants';
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['field'] = 'field_activity_participants_target_id';
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['label'] = 'Contact from Donation Record';
  /* Field: CRM Contact: Contact ID */
  $handler->display->display_options['fields']['contact_id']['id'] = 'contact_id';
  $handler->display->display_options['fields']['contact_id']['table'] = 'crm_core_contact';
  $handler->display->display_options['fields']['contact_id']['field'] = 'contact_id';
  $handler->display->display_options['fields']['contact_id']['relationship'] = 'field_activity_participants_target_id';
  $handler->display->display_options['fields']['contact_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['contact_id']['link_to_contact'] = 0;
  /* Field: CRM Core Contact: Household */
  $handler->display->display_options['fields']['contact_name']['id'] = 'contact_name';
  $handler->display->display_options['fields']['contact_name']['table'] = 'field_data_contact_name';
  $handler->display->display_options['fields']['contact_name']['field'] = 'contact_name';
  $handler->display->display_options['fields']['contact_name']['relationship'] = 'field_activity_participants_target_id';
  $handler->display->display_options['fields']['contact_name']['label'] = 'Donor';
  $handler->display->display_options['fields']['contact_name']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['contact_name']['alter']['path'] = 'crm-core/contact/[contact_id]';
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
  /* Field: MAX(CRM Activity: Receive Date) */
  $handler->display->display_options['fields']['field_cmcd_receive_date']['id'] = 'field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['table'] = 'field_data_field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['field'] = 'field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['group_type'] = 'max';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['label'] = 'Last Donation Date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_cmcd_receive_date']['alter']['path'] = 'crm-core/contact/[contact_id]/activity';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: SUM(CRM Activity: Amount) */
  $handler->display->display_options['fields']['field_cmcd_amount']['id'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['table'] = 'field_data_field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['field'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['group_type'] = 'sum';
  $handler->display->display_options['fields']['field_cmcd_amount']['label'] = 'Total Donations';
  $handler->display->display_options['fields']['field_cmcd_amount']['type'] = 'crm_core_donation_amount';
  /* Field: COUNT(CRM Core Activity: Activity ID) */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['group_type'] = 'count';
  $handler->display->display_options['fields']['activity_id']['label'] = 'Number of Donations';
  /* Filter criterion: CRM Core Activity: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'crm_core_activity';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'cmcd_donation' => 'cmcd_donation',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'crm-core/reports/cmcd-donors/sybunt';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'SYBUNT';
  $handler->display->display_options['menu']['description'] = 'Last Year But Unfortunately Not This Year';
  $handler->display->display_options['menu']['weight'] = '-7';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['donor_sybunt_report'] = $view;

  $view = new view();
  $view->name = 'donors_top_donors';
  $view->description = 'Displays a table of top donors in the system';
  $view->tag = 'default';
  $view->base_table = 'crm_core_activity';
  $view->human_name = 'Donors Top Donors';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Top Donors';
  $handler->display->display_options['css_class'] = 'fancy-headers';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view donor reports';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_activity_participants' => 'field_activity_participants',
    'contact_id' => 'contact_id',
    'contact_name' => 'contact_name',
    'field_cmcd_amount' => 'field_cmcd_amount',
    'activity_id' => 'activity_id',
    'field_cmcd_receive_date' => 'field_cmcd_receive_date',
  );
  $handler->display->display_options['style_options']['default'] = 'field_cmcd_amount';
  $handler->display->display_options['style_options']['info'] = array(
    'field_activity_participants' => array(
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
    'contact_name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cmcd_amount' => array(
      'sortable' => 0,
      'default_sort_order' => 'desc',
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
    'field_cmcd_receive_date' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['id'] = 'field_activity_participants_target_id';
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['table'] = 'field_data_field_activity_participants';
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['field'] = 'field_activity_participants_target_id';
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['label'] = 'Contact from Donation Record';
  /* Field: CRM Activity: Participants */
  $handler->display->display_options['fields']['field_activity_participants']['id'] = 'field_activity_participants';
  $handler->display->display_options['fields']['field_activity_participants']['table'] = 'field_data_field_activity_participants';
  $handler->display->display_options['fields']['field_activity_participants']['field'] = 'field_activity_participants';
  $handler->display->display_options['fields']['field_activity_participants']['label'] = 'Donor';
  $handler->display->display_options['fields']['field_activity_participants']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_activity_participants']['settings'] = array(
    'link' => 0,
  );
  $handler->display->display_options['fields']['field_activity_participants']['delta_offset'] = '0';
  /* Field: CRM Contact: Contact ID */
  $handler->display->display_options['fields']['contact_id']['id'] = 'contact_id';
  $handler->display->display_options['fields']['contact_id']['table'] = 'crm_core_contact';
  $handler->display->display_options['fields']['contact_id']['field'] = 'contact_id';
  $handler->display->display_options['fields']['contact_id']['relationship'] = 'field_activity_participants_target_id';
  $handler->display->display_options['fields']['contact_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['contact_id']['link_to_contact'] = 0;
  /* Field: CRM Core Contact: Household */
  $handler->display->display_options['fields']['contact_name']['id'] = 'contact_name';
  $handler->display->display_options['fields']['contact_name']['table'] = 'field_data_contact_name';
  $handler->display->display_options['fields']['contact_name']['field'] = 'contact_name';
  $handler->display->display_options['fields']['contact_name']['relationship'] = 'field_activity_participants_target_id';
  $handler->display->display_options['fields']['contact_name']['label'] = 'Donor';
  $handler->display->display_options['fields']['contact_name']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['contact_name']['alter']['path'] = 'crm-core/acontact[contact_id]';
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
  /* Field: SUM(CRM Activity: Amount) */
  $handler->display->display_options['fields']['field_cmcd_amount']['id'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['table'] = 'field_data_field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['field'] = 'field_cmcd_amount';
  $handler->display->display_options['fields']['field_cmcd_amount']['group_type'] = 'sum';
  $handler->display->display_options['fields']['field_cmcd_amount']['label'] = 'Total';
  $handler->display->display_options['fields']['field_cmcd_amount']['type'] = 'crm_core_donation_amount';
  /* Field: COUNT(CRM Core Activity: Activity ID) */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['group_type'] = 'count';
  $handler->display->display_options['fields']['activity_id']['label'] = 'Donations';
  /* Field: MAX(CRM Activity: Receive Date) */
  $handler->display->display_options['fields']['field_cmcd_receive_date']['id'] = 'field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['table'] = 'field_data_field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['field'] = 'field_cmcd_receive_date';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['group_type'] = 'max';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['label'] = 'Last Donation';
  $handler->display->display_options['fields']['field_cmcd_receive_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Filter criterion: CRM Core Activity: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'crm_core_activity';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'cmcd_donation' => 'cmcd_donation',
  );

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['pane_title'] = 'Donors: Top Donors';
  $handler->display->display_options['pane_description'] = 'List of the top donors, including totals and number of donations.';
  $handler->display->display_options['pane_category']['name'] = 'CRM Core Donations';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $export['donors_top_donors'] = $view;

  return $export;
}
