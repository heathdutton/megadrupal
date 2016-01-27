<?php
/**
 * @file
 * erpal_invoice_ui.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function erpal_invoice_ui_views_default_views() {
  $export = array();
  $view = new view();
  $view->name = 'erpal_invoice';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'commerce_order';
  $view->human_name = 'ERPAL invoice';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'ERPAL invoices';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'edit any commerce_order entity of bundle invoice';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'Invoices not found. Please, create new invoice.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_customer_target_id']['id'] = 'field_customer_target_id';
  $handler->display->display_options['relationships']['field_customer_target_id']['table'] = 'field_data_field_customer';
  $handler->display->display_options['relationships']['field_customer_target_id']['field'] = 'field_customer_target_id';
  /* Field: Bulk operations: Commerce Order */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
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
    'action::views_bulk_operations_delete_revision' => array(
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
    'rules_component::rules_commerce_billy_set_to_canceled' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::rules_commerce_billy_set_to_invoiced' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::erpal_commerce_delete_invoice' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
  /* Field: Commerce Order: Order number */
  $handler->display->display_options['fields']['order_number']['id'] = 'order_number';
  $handler->display->display_options['fields']['order_number']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['order_number']['field'] = 'order_number';
  $handler->display->display_options['fields']['order_number']['label'] = 'Invoice number';
  $handler->display->display_options['fields']['order_number']['alter']['path'] = 'admin/commerce/orders/[order_id]/pdf';
  $handler->display->display_options['fields']['order_number']['link_to_order'] = 'admin';
  /* Field: Commerce Order: Invoice PDF */
  $handler->display->display_options['fields']['commerce_billy_pdf_invoice_link']['id'] = 'commerce_billy_pdf_invoice_link';
  $handler->display->display_options['fields']['commerce_billy_pdf_invoice_link']['table'] = 'views_entity_commerce_order';
  $handler->display->display_options['fields']['commerce_billy_pdf_invoice_link']['field'] = 'commerce_billy_pdf_invoice_link';
  $handler->display->display_options['fields']['commerce_billy_pdf_invoice_link']['label'] = 'Download PDF';
  /* Field: Commerce Order: Customer */
  $handler->display->display_options['fields']['field_customer']['id'] = 'field_customer';
  $handler->display->display_options['fields']['field_customer']['table'] = 'field_data_field_customer';
  $handler->display->display_options['fields']['field_customer']['field'] = 'field_customer';
  $handler->display->display_options['fields']['field_customer']['settings'] = array(
    'link' => 1,
  );
  /* Field: Commerce Order: Order status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = 'Invoice status';
  /* Field: Commerce Order: Order total */
  $handler->display->display_options['fields']['commerce_order_total']['id'] = 'commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['table'] = 'field_data_commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['field'] = 'commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['label'] = 'Invoice total';
  $handler->display->display_options['fields']['commerce_order_total']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_order_total']['settings'] = array(
    'calculation' => FALSE,
  );
  /* Field: Commerce Order: Invoice date */
  $handler->display->display_options['fields']['field_commerce_billy_i_date']['id'] = 'field_commerce_billy_i_date';
  $handler->display->display_options['fields']['field_commerce_billy_i_date']['table'] = 'field_data_field_commerce_billy_i_date';
  $handler->display->display_options['fields']['field_commerce_billy_i_date']['field'] = 'field_commerce_billy_i_date';
  $handler->display->display_options['fields']['field_commerce_billy_i_date']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Commerce Order: Edit link */
  $handler->display->display_options['fields']['edit_order']['id'] = 'edit_order';
  $handler->display->display_options['fields']['edit_order']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['edit_order']['field'] = 'edit_order';
  /* Sort criterion: Commerce Order: Order ID */
  $handler->display->display_options['sorts']['order_id']['id'] = 'order_id';
  $handler->display->display_options['sorts']['order_id']['table'] = 'commerce_order';
  $handler->display->display_options['sorts']['order_id']['field'] = 'order_id';
  $handler->display->display_options['sorts']['order_id']['order'] = 'DESC';
  /* Filter criterion: Commerce Order: Order type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'invoice' => 'invoice',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Commerce Order: Invoice date (field_commerce_billy_i_date) */
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['id'] = 'field_commerce_billy_i_date_value';
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['table'] = 'field_data_field_commerce_billy_i_date';
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['field'] = 'field_commerce_billy_i_date_value';
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['group'] = 1;
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['expose']['operator_id'] = 'field_commerce_billy_i_date_value_op';
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['expose']['label'] = 'Invoice date';
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['expose']['operator'] = 'field_commerce_billy_i_date_value_op';
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['expose']['identifier'] = 'field_commerce_billy_i_date_value';
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['form_type'] = 'date_popup';
  /* Filter criterion: Commerce Order: Order status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = array(
    'canceled' => 'canceled',
    'invoiced' => 'invoiced',
  );
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Order status';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['status']['expose']['reduce'] = TRUE;
  /* Filter criterion: Commerce Order: Customer (field_customer) */
  $handler->display->display_options['filters']['field_customer_target_id']['id'] = 'field_customer_target_id';
  $handler->display->display_options['filters']['field_customer_target_id']['table'] = 'field_data_field_customer';
  $handler->display->display_options['filters']['field_customer_target_id']['field'] = 'field_customer_target_id';
  $handler->display->display_options['filters']['field_customer_target_id']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_customer_target_id']['expose']['operator_id'] = 'field_customer_target_id_op';
  $handler->display->display_options['filters']['field_customer_target_id']['expose']['label'] = 'Customer';
  $handler->display->display_options['filters']['field_customer_target_id']['expose']['operator'] = 'field_customer_target_id_op';
  $handler->display->display_options['filters']['field_customer_target_id']['expose']['identifier'] = 'field_customer_target_id';
  $handler->display->display_options['filters']['field_customer_target_id']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  
  /* Display: Admin page */
  $handler = $view->new_display('page', 'Admin page', 'page');
  $handler->display->display_options['defaults']['footer'] = FALSE;
  /* Footer: Global: Result summary */
  $handler->display->display_options['footer']['result']['id'] = 'result';
  $handler->display->display_options['footer']['result']['table'] = 'views';
  $handler->display->display_options['footer']['result']['field'] = 'result';
  $handler->display->display_options['footer']['result']['content'] = '@total rows';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Commerce Order: Order type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'invoice' => 'invoice',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Commerce Order: Invoice date (field_commerce_billy_i_date) */
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['id'] = 'field_commerce_billy_i_date_value';
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['table'] = 'field_data_field_commerce_billy_i_date';
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['field'] = 'field_commerce_billy_i_date_value';
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['group'] = 1;
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['expose']['operator_id'] = 'field_commerce_billy_i_date_value_op';
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['expose']['label'] = 'Invoice date';
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['expose']['operator'] = 'field_commerce_billy_i_date_value_op';
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['expose']['identifier'] = 'field_commerce_billy_i_date_value';
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['form_type'] = 'date_popup';
  /* Filter criterion: Commerce Order: Order status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = array(
    'canceled' => 'canceled',
    'invoiced' => 'invoiced',
  );
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Invoice status';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['status']['expose']['reduce'] = TRUE;
  /* Filter criterion: CRM Core Contact: Household */
  $handler->display->display_options['filters']['contact_name']['id'] = 'contact_name';
  $handler->display->display_options['filters']['contact_name']['table'] = 'field_data_contact_name';
  $handler->display->display_options['filters']['contact_name']['field'] = 'contact_name';
  $handler->display->display_options['filters']['contact_name']['relationship'] = 'field_customer_target_id';
  $handler->display->display_options['filters']['contact_name']['operator'] = 'word';
  $handler->display->display_options['filters']['contact_name']['group'] = 1;
  $handler->display->display_options['filters']['contact_name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['contact_name']['expose']['operator_id'] = 'contact_name_op';
  $handler->display->display_options['filters']['contact_name']['expose']['label'] = 'Customer';
  $handler->display->display_options['filters']['contact_name']['expose']['operator'] = 'contact_name_op';
  $handler->display->display_options['filters']['contact_name']['expose']['identifier'] = 'contact_name';
  $handler->display->display_options['filters']['contact_name']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
  );
  /* Filter criterion: Commerce Order: Order number */
  $handler->display->display_options['filters']['order_number']['id'] = 'order_number';
  $handler->display->display_options['filters']['order_number']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['order_number']['field'] = 'order_number';
  $handler->display->display_options['filters']['order_number']['operator'] = 'contains';
  $handler->display->display_options['filters']['order_number']['group'] = 1;
  $handler->display->display_options['filters']['order_number']['exposed'] = TRUE;
  $handler->display->display_options['filters']['order_number']['expose']['operator_id'] = 'order_number_op';
  $handler->display->display_options['filters']['order_number']['expose']['label'] = 'Invoice number';
  $handler->display->display_options['filters']['order_number']['expose']['operator'] = 'order_number_op';
  $handler->display->display_options['filters']['order_number']['expose']['identifier'] = 'order_number';
  $handler->display->display_options['filters']['order_number']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
  );
  $handler->display->display_options['path'] = 'admin/erpal/invoices';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'ERPAL Invoices';
  $handler->display->display_options['menu']['description'] = 'Manage ERPAL Invoices.';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['weight'] = '0';
  
  /* Display: User page */
  $handler = $view->new_display('page', 'User page', 'page_1');
  $handler->display->display_options['defaults']['footer'] = FALSE;
  /* Footer: Global: Result summary */
  $handler->display->display_options['footer']['result']['id'] = 'result';
  $handler->display->display_options['footer']['result']['table'] = 'views';
  $handler->display->display_options['footer']['result']['field'] = 'result';
  $handler->display->display_options['footer']['result']['content'] = '@total rows';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Bulk operations: Commerce Order */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
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
    'action::views_bulk_operations_delete_revision' => array(
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
    'rules_component::rules_commerce_billy_set_to_canceled' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::rules_commerce_billy_set_to_invoiced' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
  /* Field: Commerce Order: Order number */
  $handler->display->display_options['fields']['order_number']['id'] = 'order_number';
  $handler->display->display_options['fields']['order_number']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['order_number']['field'] = 'order_number';
  $handler->display->display_options['fields']['order_number']['label'] = 'Invoice number';
  $handler->display->display_options['fields']['order_number']['alter']['path'] = 'order/[order_id]';
  $handler->display->display_options['fields']['order_number']['link_to_order'] = 'admin';
  /* Field: Commerce Order: Invoice PDF */
  $handler->display->display_options['fields']['commerce_billy_pdf_invoice_link']['id'] = 'commerce_billy_pdf_invoice_link';
  $handler->display->display_options['fields']['commerce_billy_pdf_invoice_link']['table'] = 'views_entity_commerce_order';
  $handler->display->display_options['fields']['commerce_billy_pdf_invoice_link']['field'] = 'commerce_billy_pdf_invoice_link';
  $handler->display->display_options['fields']['commerce_billy_pdf_invoice_link']['label'] = 'Download PDF';
  /* Field: Commerce Order: Customer */
  $handler->display->display_options['fields']['field_customer']['id'] = 'field_customer';
  $handler->display->display_options['fields']['field_customer']['table'] = 'field_data_field_customer';
  $handler->display->display_options['fields']['field_customer']['field'] = 'field_customer';
  $handler->display->display_options['fields']['field_customer']['settings'] = array(
    'link' => 1,
  );
  /* Field: Commerce Order: Order status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = 'Invoice status';
  /* Field: Commerce Order: Order total */
  $handler->display->display_options['fields']['commerce_order_total']['id'] = 'commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['table'] = 'field_data_commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['field'] = 'commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['label'] = 'Invoice total';
  $handler->display->display_options['fields']['commerce_order_total']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_order_total']['settings'] = array(
    'calculation' => FALSE,
  );
  /* Field: Commerce Order: Invoice date */
  $handler->display->display_options['fields']['field_commerce_billy_i_date']['id'] = 'field_commerce_billy_i_date';
  $handler->display->display_options['fields']['field_commerce_billy_i_date']['table'] = 'field_data_field_commerce_billy_i_date';
  $handler->display->display_options['fields']['field_commerce_billy_i_date']['field'] = 'field_commerce_billy_i_date';
  $handler->display->display_options['fields']['field_commerce_billy_i_date']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Commerce Order: Edit link */
  $handler->display->display_options['fields']['edit_order']['id'] = 'edit_order';
  $handler->display->display_options['fields']['edit_order']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['edit_order']['field'] = 'edit_order';
  $handler->display->display_options['fields']['edit_order']['alter']['text'] = 'Edit';
  $handler->display->display_options['fields']['edit_order']['alter']['path'] = 'order/[order_id]/edit';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Commerce Order: Order type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'invoice' => 'invoice',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Commerce Order: Invoice date (field_commerce_billy_i_date) */
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['id'] = 'field_commerce_billy_i_date_value';
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['table'] = 'field_data_field_commerce_billy_i_date';
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['field'] = 'field_commerce_billy_i_date_value';
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['group'] = 1;
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['expose']['operator_id'] = 'field_commerce_billy_i_date_value_op';
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['expose']['label'] = 'Invoice date';
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['expose']['operator'] = 'field_commerce_billy_i_date_value_op';
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['expose']['identifier'] = 'field_commerce_billy_i_date_value';
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_commerce_billy_i_date_value']['form_type'] = 'date_popup';
  /* Filter criterion: Commerce Order: Order status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = array(
    'canceled' => 'canceled',
    'invoiced' => 'invoiced',
  );
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Invoice status';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['status']['expose']['reduce'] = TRUE;
  /* Filter criterion: CRM Core Contact: Household */
  $handler->display->display_options['filters']['contact_name']['id'] = 'contact_name';
  $handler->display->display_options['filters']['contact_name']['table'] = 'field_data_contact_name';
  $handler->display->display_options['filters']['contact_name']['field'] = 'contact_name';
  $handler->display->display_options['filters']['contact_name']['relationship'] = 'field_customer_target_id';
  $handler->display->display_options['filters']['contact_name']['operator'] = 'word';
  $handler->display->display_options['filters']['contact_name']['group'] = 1;
  $handler->display->display_options['filters']['contact_name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['contact_name']['expose']['operator_id'] = 'contact_name_op';
  $handler->display->display_options['filters']['contact_name']['expose']['label'] = 'Customer';
  $handler->display->display_options['filters']['contact_name']['expose']['operator'] = 'contact_name_op';
  $handler->display->display_options['filters']['contact_name']['expose']['identifier'] = 'contact_name';
  $handler->display->display_options['filters']['contact_name']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
  );
  /* Filter criterion: Commerce Order: Order number */
  $handler->display->display_options['filters']['order_number']['id'] = 'order_number';
  $handler->display->display_options['filters']['order_number']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['order_number']['field'] = 'order_number';
  $handler->display->display_options['filters']['order_number']['operator'] = 'contains';
  $handler->display->display_options['filters']['order_number']['group'] = 1;
  $handler->display->display_options['filters']['order_number']['exposed'] = TRUE;
  $handler->display->display_options['filters']['order_number']['expose']['operator_id'] = 'order_number_op';
  $handler->display->display_options['filters']['order_number']['expose']['label'] = 'Invoice number';
  $handler->display->display_options['filters']['order_number']['expose']['operator'] = 'order_number_op';
  $handler->display->display_options['filters']['order_number']['expose']['identifier'] = 'order_number';
  $handler->display->display_options['filters']['order_number']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
  );
  $handler->display->display_options['path'] = 'invoices';

  $translatables['erpal_invoice'] = array(
    t('Master'),
    t('ERPAL invoices'),
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
    t('Invoices not found. Please, create new invoice.'),
    t('Commerce Order'),
    t('- Choose an operation -'),
    t('Invoice number'),
    t('Download PDF'),
    t('Customer'),
    t('Invoice status'),
    t('Invoice total'),
    t('Invoice date'),
    t('Edit link'),
    t('Order status'),
    t('Admin page'),
    t('User page'),
    t('Edit'),
    t('Invoices by Contact'),
    t('Invoices'),
    t('Invoices not found. Please, create new invoice. '),
    t('CRM Core Contact entity referenced from field_customer'),
    t('All'),
    t('View panes'),
  );

  $export['erpal_invoice'] = $view;

  return $export;
}
