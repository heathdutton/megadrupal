<?php
/**
 * @file
 * cash_flow_core.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function cash_flow_core_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'finance_track_money';
  $view->description = 'Shows expenses and income together.';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Track money';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Manage expenses';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['pager']['options']['expose']['items_per_page'] = TRUE;
  $handler->display->display_options['pager']['options']['expose']['items_per_page_label'] = 'Show this many items';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options'] = '25, 50, 100, 200, 350, 500';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = TRUE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'views_bulk_operations' => 'views_bulk_operations',
    'field_effective_date' => 'field_effective_date',
    'title' => 'title',
    'field_amount' => 'field_amount',
    'field_remaining' => 'field_remaining',
    'field_tax' => 'field_tax',
    'field_tax_paid' => 'field_tax_paid',
    'field_r_d' => 'field_r_d',
    'field_r_d_paid' => 'field_r_d_paid',
    'field_business_related' => 'field_business_related',
    'field_amount_transferred' => 'field_amount_transferred',
    'nid' => 'nid',
    'body' => 'body',
    'nothing' => 'nothing',
    'edit_node' => 'nothing',
    'nothing_2' => 'nothing',
    'delete_node' => 'nothing',
    'nothing_1' => 'nothing',
    'term_node_tid' => 'body',
    'field_non_business_payment_total' => 'field_amount_transferred',
  );
  $handler->display->display_options['style_options']['default'] = 'field_effective_date';
  $handler->display->display_options['style_options']['info'] = array(
    'views_bulk_operations' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_effective_date' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
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
    'field_amount' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_remaining' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_tax' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_tax_paid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_r_d' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_r_d_paid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_business_related' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_amount_transferred' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '<br />',
      'empty_column' => 0,
    ),
    'nid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'body' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing' => array(
      'align' => '',
      'separator' => ' | ',
      'empty_column' => 0,
    ),
    'edit_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing_2' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'delete_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing_1' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'term_node_tid' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_non_business_payment_total' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['sticky'] = TRUE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = TRUE;
  $handler->display->display_options['header']['area']['content'] = 'Shortcuts: <a href="node/add/expense?destination=expenses">Add new expense</a>';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
  $handler->display->display_options['header']['area']['tokenize'] = TRUE;
  /* Field: Content: Effective date */
  $handler->display->display_options['fields']['field_effective_date']['id'] = 'field_effective_date';
  $handler->display->display_options['fields']['field_effective_date']['table'] = 'field_data_field_effective_date';
  $handler->display->display_options['fields']['field_effective_date']['field'] = 'field_effective_date';
  $handler->display->display_options['fields']['field_effective_date']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_effective_date']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  /* Field: Content: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'node';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['type']['hide_alter_empty'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Name';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = FALSE;
  /* Field: Content: Amount */
  $handler->display->display_options['fields']['field_amount']['id'] = 'field_amount';
  $handler->display->display_options['fields']['field_amount']['table'] = 'field_data_field_amount';
  $handler->display->display_options['fields']['field_amount']['field'] = 'field_amount';
  $handler->display->display_options['fields']['field_amount']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_amount']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_amount']['settings'] = array(
    'thousand_separator' => ',',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Amount remaining */
  $handler->display->display_options['fields']['field_remaining']['id'] = 'field_remaining';
  $handler->display->display_options['fields']['field_remaining']['table'] = 'field_data_field_remaining';
  $handler->display->display_options['fields']['field_remaining']['field'] = 'field_remaining';
  $handler->display->display_options['fields']['field_remaining']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_remaining']['hide_alter_empty'] = FALSE;
  /* Field: Content: Amount transferred */
  $handler->display->display_options['fields']['field_amount_transferred']['id'] = 'field_amount_transferred';
  $handler->display->display_options['fields']['field_amount_transferred']['table'] = 'field_data_field_amount_transferred';
  $handler->display->display_options['fields']['field_amount_transferred']['field'] = 'field_amount_transferred';
  $handler->display->display_options['fields']['field_amount_transferred']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_amount_transferred']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_amount_transferred']['settings'] = array(
    'thousand_separator' => ',',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Business-related */
  $handler->display->display_options['fields']['field_business_related']['id'] = 'field_business_related';
  $handler->display->display_options['fields']['field_business_related']['table'] = 'field_data_field_business_related';
  $handler->display->display_options['fields']['field_business_related']['field'] = 'field_business_related';
  $handler->display->display_options['fields']['field_business_related']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_business_related']['hide_alter_empty'] = FALSE;
  /* Field: Content: Not budgeted (one-time) */
  $handler->display->display_options['fields']['field_one_time']['id'] = 'field_one_time';
  $handler->display->display_options['fields']['field_one_time']['table'] = 'field_data_field_one_time';
  $handler->display->display_options['fields']['field_one_time']['field'] = 'field_one_time';
  $handler->display->display_options['fields']['field_one_time']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_one_time']['hide_alter_empty'] = FALSE;
  /* Field: Content: Tax */
  $handler->display->display_options['fields']['field_tax']['id'] = 'field_tax';
  $handler->display->display_options['fields']['field_tax']['table'] = 'field_data_field_tax';
  $handler->display->display_options['fields']['field_tax']['field'] = 'field_tax';
  $handler->display->display_options['fields']['field_tax']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_tax']['hide_alter_empty'] = FALSE;
  /* Field: Content: Notes */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['hide_alter_empty'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'expense' => 'expense',
    'income' => 'income',
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';

  /* Display: Expenses */
  $handler = $view->new_display('page', 'Expenses', 'expenses');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Bulk operations: Content */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'node';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
  $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::node_assign_owner_action' => array(
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
    'action::node_make_sticky_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_make_unsticky_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 1,
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
    'action::node_promote_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_publish_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_unpromote_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_save_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::system_send_email_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_unpublish_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_unpublish_by_keyword_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::pathauto_node_update_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
  /* Field: Content: Amount */
  $handler->display->display_options['fields']['field_amount_1']['id'] = 'field_amount_1';
  $handler->display->display_options['fields']['field_amount_1']['table'] = 'field_data_field_amount';
  $handler->display->display_options['fields']['field_amount_1']['field'] = 'field_amount';
  $handler->display->display_options['fields']['field_amount_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_amount_1']['settings'] = array(
    'thousand_separator' => ',',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['hide_alter_empty'] = FALSE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
  $handler->display->display_options['fields']['nothing_2']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_2']['label'] = 'Add payment';
  $handler->display->display_options['fields']['nothing_2']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['text'] = 'Add payment';
  $handler->display->display_options['fields']['nothing_2']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['path'] = 'node/add/payment?edit[field_target_line_item][und]=[nid]&destination=expenses';
  /* Field: Content: Effective date */
  $handler->display->display_options['fields']['field_effective_date']['id'] = 'field_effective_date';
  $handler->display->display_options['fields']['field_effective_date']['table'] = 'field_data_field_effective_date';
  $handler->display->display_options['fields']['field_effective_date']['field'] = 'field_effective_date';
  $handler->display->display_options['fields']['field_effective_date']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_effective_date']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Name';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = FALSE;
  /* Field: Content: Expense Amount */
  $handler->display->display_options['fields']['field_amount']['id'] = 'field_amount';
  $handler->display->display_options['fields']['field_amount']['table'] = 'field_data_field_amount';
  $handler->display->display_options['fields']['field_amount']['field'] = 'field_amount';
  $handler->display->display_options['fields']['field_amount']['ui_name'] = 'Content: Expense Amount';
  $handler->display->display_options['fields']['field_amount']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_amount']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_amount']['settings'] = array(
    'thousand_separator' => ',',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Amount remaining */
  $handler->display->display_options['fields']['field_remaining']['id'] = 'field_remaining';
  $handler->display->display_options['fields']['field_remaining']['table'] = 'field_data_field_remaining';
  $handler->display->display_options['fields']['field_remaining']['field'] = 'field_remaining';
  $handler->display->display_options['fields']['field_remaining']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_remaining']['hide_alter_empty'] = FALSE;
  /* Field: Content: Business-related */
  $handler->display->display_options['fields']['field_business_related']['id'] = 'field_business_related';
  $handler->display->display_options['fields']['field_business_related']['table'] = 'field_data_field_business_related';
  $handler->display->display_options['fields']['field_business_related']['field'] = 'field_business_related';
  $handler->display->display_options['fields']['field_business_related']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_business_related']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_business_related']['settings'] = array(
    'click_to_edit' => 1,
    'empty_text' => '',
    'fallback_format' => 'list_default',
  );
  /* Field: Content: Not budgeted (one-time) */
  $handler->display->display_options['fields']['field_one_time']['id'] = 'field_one_time';
  $handler->display->display_options['fields']['field_one_time']['table'] = 'field_data_field_one_time';
  $handler->display->display_options['fields']['field_one_time']['field'] = 'field_one_time';
  $handler->display->display_options['fields']['field_one_time']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_one_time']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_one_time']['settings'] = array(
    'click_to_edit' => 1,
    'empty_text' => '',
    'fallback_format' => 'list_default',
  );
  /* Field: Content: Notes */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['alter']['max_length'] = '160';
  $handler->display->display_options['fields']['body']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['body']['alter']['trim'] = TRUE;
  $handler->display->display_options['fields']['body']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['body']['settings'] = array(
    'click_to_edit' => 1,
    'empty_text' => '',
    'fallback_format' => 'text_default',
  );
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Actions';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[nothing_2]';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['path'] = 'node/add/payment?edit[field_target_line_item][und]=[nid]&destination=expenses';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['edit_node']['text'] = 'Edit';
  /* Field: Clone link */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['ui_name'] = 'Clone link';
  $handler->display->display_options['fields']['nothing_1']['label'] = '';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'Copy';
  $handler->display->display_options['fields']['nothing_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['path'] = 'node/[nid]/clone?destination=expenses';
  $handler->display->display_options['fields']['nothing_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nothing_1']['hide_alter_empty'] = TRUE;
  /* Field: Content: Delete link */
  $handler->display->display_options['fields']['delete_node']['id'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['delete_node']['field'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['delete_node']['text'] = 'Delete';
  /* Field: Add new expense link */
  $handler->display->display_options['fields']['nothing_3']['id'] = 'nothing_3';
  $handler->display->display_options['fields']['nothing_3']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_3']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_3']['ui_name'] = 'Add new expense link';
  $handler->display->display_options['fields']['nothing_3']['label'] = '';
  $handler->display->display_options['fields']['nothing_3']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_3']['alter']['text'] = 'Add new expense';
  $handler->display->display_options['fields']['nothing_3']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_3']['alter']['path'] = 'node/add/expense?destination=expenses';
  $handler->display->display_options['fields']['nothing_3']['element_label_colon'] = FALSE;
  /* Field: Content: All taxonomy terms */
  $handler->display->display_options['fields']['term_node_tid']['id'] = 'term_node_tid';
  $handler->display->display_options['fields']['term_node_tid']['table'] = 'node';
  $handler->display->display_options['fields']['term_node_tid']['field'] = 'term_node_tid';
  $handler->display->display_options['fields']['term_node_tid']['label'] = '';
  $handler->display->display_options['fields']['term_node_tid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['term_node_tid']['vocabularies'] = array(
    'earning_account' => 0,
    'paid_with' => 0,
    'tags' => 0,
  );
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'expense' => 'expense',
  );
  /* Filter criterion: Content: Amount remaining (field_remaining) */
  $handler->display->display_options['filters']['field_remaining_value']['id'] = 'field_remaining_value';
  $handler->display->display_options['filters']['field_remaining_value']['table'] = 'field_data_field_remaining';
  $handler->display->display_options['filters']['field_remaining_value']['field'] = 'field_remaining_value';
  $handler->display->display_options['filters']['field_remaining_value']['operator'] = '!=';
  $handler->display->display_options['filters']['field_remaining_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_remaining_value']['expose']['operator_id'] = 'field_remaining_value_op';
  $handler->display->display_options['filters']['field_remaining_value']['expose']['label'] = 'Amount remaining';
  $handler->display->display_options['filters']['field_remaining_value']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['field_remaining_value']['expose']['operator'] = 'field_remaining_value_op';
  $handler->display->display_options['filters']['field_remaining_value']['expose']['identifier'] = 'field_remaining_value';
  $handler->display->display_options['filters']['field_remaining_value']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_remaining_value']['expose']['remember_roles'] = array(
    2 => '2',
    3 => '3',
    4 => '4',
    1 => 0,
  );
  /* Filter criterion: Date: Date (node) */
  $handler->display->display_options['filters']['date_filter']['id'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['table'] = 'node';
  $handler->display->display_options['filters']['date_filter']['field'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['operator'] = '>=';
  $handler->display->display_options['filters']['date_filter']['group'] = '0';
  $handler->display->display_options['filters']['date_filter']['exposed'] = TRUE;
  $handler->display->display_options['filters']['date_filter']['expose']['operator_id'] = 'date_filter_op';
  $handler->display->display_options['filters']['date_filter']['expose']['label'] = 'Effective date on or after';
  $handler->display->display_options['filters']['date_filter']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['date_filter']['expose']['operator'] = 'date_filter_op';
  $handler->display->display_options['filters']['date_filter']['expose']['identifier'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['date_filter']['expose']['remember_roles'] = array(
    2 => '2',
    3 => '3',
    4 => '4',
    1 => 0,
  );
  $handler->display->display_options['filters']['date_filter']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['date_filter']['default_to_date'] = 'now';
  $handler->display->display_options['filters']['date_filter']['year_range'] = '-10:+10';
  $handler->display->display_options['filters']['date_filter']['date_fields'] = array(
    'field_data_field_effective_date.field_effective_date_value' => 'field_data_field_effective_date.field_effective_date_value',
  );
  /* Filter criterion: Content: Special (field_special) */
  $handler->display->display_options['filters']['field_special_value']['id'] = 'field_special_value';
  $handler->display->display_options['filters']['field_special_value']['table'] = 'field_data_field_special';
  $handler->display->display_options['filters']['field_special_value']['field'] = 'field_special_value';
  $handler->display->display_options['filters']['field_special_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_special_value']['expose']['operator_id'] = 'field_special_value_op';
  $handler->display->display_options['filters']['field_special_value']['expose']['label'] = 'Special';
  $handler->display->display_options['filters']['field_special_value']['expose']['operator'] = 'field_special_value_op';
  $handler->display->display_options['filters']['field_special_value']['expose']['identifier'] = 'field_special_value';
  $handler->display->display_options['filters']['field_special_value']['expose']['remember'] = TRUE;
  /* Filter criterion: Content: Tags (field_tags) */
  $handler->display->display_options['filters']['field_tags_tid_1']['id'] = 'field_tags_tid_1';
  $handler->display->display_options['filters']['field_tags_tid_1']['table'] = 'field_data_field_tags';
  $handler->display->display_options['filters']['field_tags_tid_1']['field'] = 'field_tags_tid';
  $handler->display->display_options['filters']['field_tags_tid_1']['value'] = '';
  $handler->display->display_options['filters']['field_tags_tid_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_tags_tid_1']['expose']['operator_id'] = 'field_tags_tid_1_op';
  $handler->display->display_options['filters']['field_tags_tid_1']['expose']['label'] = 'Tags';
  $handler->display->display_options['filters']['field_tags_tid_1']['expose']['operator'] = 'field_tags_tid_1_op';
  $handler->display->display_options['filters']['field_tags_tid_1']['expose']['identifier'] = 'field_tags_tid_1';
  $handler->display->display_options['filters']['field_tags_tid_1']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_tags_tid_1']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_tags_tid_1']['expose']['remember_roles'] = array(
    2 => '2',
    3 => '3',
    1 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_tags_tid_1']['vocabulary'] = 'tags';
  $handler->display->display_options['path'] = 'expenses';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Expenses';
  $handler->display->display_options['menu']['weight'] = '10';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;

  /* Display: Income */
  $handler = $view->new_display('page', 'Income', 'page_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Manage income';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = TRUE;
  $handler->display->display_options['header']['area']['content'] = 'Shortcuts: <a href="node/add/income?destination=income">Add new income</a>';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Bulk operations: Content */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'node';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
  $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::node_assign_owner_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 0,
      'postpone_processing' => 1,
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
    'action::node_make_sticky_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_make_unsticky_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 1,
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
    'action::node_promote_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_publish_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_unpromote_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_save_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::system_send_email_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_unpublish_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_unpublish_by_keyword_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::pathauto_node_update_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
  /* Field: Content: Effective date */
  $handler->display->display_options['fields']['field_effective_date']['id'] = 'field_effective_date';
  $handler->display->display_options['fields']['field_effective_date']['table'] = 'field_data_field_effective_date';
  $handler->display->display_options['fields']['field_effective_date']['field'] = 'field_effective_date';
  $handler->display->display_options['fields']['field_effective_date']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_effective_date']['type'] = 'editable';
  $handler->display->display_options['fields']['field_effective_date']['settings'] = array(
    'click_to_edit' => 1,
    'empty_text' => '',
    'fallback_format' => 'date_default',
    'fallback_settings' => array(
      'format_type' => 'short',
      'fromto' => 'both',
      'multiple_number' => '',
      'multiple_from' => '',
      'multiple_to' => '',
      'show_repeat_rule' => 'show',
    ),
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Name';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = FALSE;
  /* Field: Content: Amount */
  $handler->display->display_options['fields']['field_amount']['id'] = 'field_amount';
  $handler->display->display_options['fields']['field_amount']['table'] = 'field_data_field_amount';
  $handler->display->display_options['fields']['field_amount']['field'] = 'field_amount';
  $handler->display->display_options['fields']['field_amount']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_amount']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_amount']['settings'] = array(
    'thousand_separator' => ',',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Amount remaining */
  $handler->display->display_options['fields']['field_remaining']['id'] = 'field_remaining';
  $handler->display->display_options['fields']['field_remaining']['table'] = 'field_data_field_remaining';
  $handler->display->display_options['fields']['field_remaining']['field'] = 'field_remaining';
  $handler->display->display_options['fields']['field_remaining']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_remaining']['hide_alter_empty'] = FALSE;
  /* Field: Content: Tax */
  $handler->display->display_options['fields']['field_tax']['id'] = 'field_tax';
  $handler->display->display_options['fields']['field_tax']['table'] = 'field_data_field_tax';
  $handler->display->display_options['fields']['field_tax']['field'] = 'field_tax';
  $handler->display->display_options['fields']['field_tax']['hide_alter_empty'] = FALSE;
  /* Field: Content: Tax paid */
  $handler->display->display_options['fields']['field_tax_paid']['id'] = 'field_tax_paid';
  $handler->display->display_options['fields']['field_tax_paid']['table'] = 'field_data_field_tax_paid';
  $handler->display->display_options['fields']['field_tax_paid']['field'] = 'field_tax_paid';
  $handler->display->display_options['fields']['field_tax_paid']['label'] = 'Tax paid?';
  $handler->display->display_options['fields']['field_tax_paid']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_tax_paid']['type'] = 'editable';
  $handler->display->display_options['fields']['field_tax_paid']['settings'] = array(
    'click_to_edit' => 0,
    'empty_text' => '',
    'fallback_format' => 'list_default',
  );
  /* Field: Content: R&D */
  $handler->display->display_options['fields']['field_r_d']['id'] = 'field_r_d';
  $handler->display->display_options['fields']['field_r_d']['table'] = 'field_data_field_r_d';
  $handler->display->display_options['fields']['field_r_d']['field'] = 'field_r_d';
  /* Field: Content: R&D paid */
  $handler->display->display_options['fields']['field_r_d_paid']['id'] = 'field_r_d_paid';
  $handler->display->display_options['fields']['field_r_d_paid']['table'] = 'field_data_field_r_d_paid';
  $handler->display->display_options['fields']['field_r_d_paid']['field'] = 'field_r_d_paid';
  $handler->display->display_options['fields']['field_r_d_paid']['type'] = 'editable';
  $handler->display->display_options['fields']['field_r_d_paid']['settings'] = array(
    'click_to_edit' => 0,
    'empty_text' => '',
    'fallback_format' => 'list_default',
  );
  /* Field: Content: Business-related */
  $handler->display->display_options['fields']['field_business_related']['id'] = 'field_business_related';
  $handler->display->display_options['fields']['field_business_related']['table'] = 'field_data_field_business_related';
  $handler->display->display_options['fields']['field_business_related']['field'] = 'field_business_related';
  $handler->display->display_options['fields']['field_business_related']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_business_related']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_business_related']['type'] = 'editable';
  $handler->display->display_options['fields']['field_business_related']['settings'] = array(
    'click_to_edit' => 1,
    'empty_text' => '',
    'fallback_format' => 'list_default',
  );
  /* Field: Content: Amount transferred */
  $handler->display->display_options['fields']['field_amount_transferred']['id'] = 'field_amount_transferred';
  $handler->display->display_options['fields']['field_amount_transferred']['table'] = 'field_data_field_amount_transferred';
  $handler->display->display_options['fields']['field_amount_transferred']['field'] = 'field_amount_transferred';
  $handler->display->display_options['fields']['field_amount_transferred']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_amount_transferred']['type'] = 'editable';
  $handler->display->display_options['fields']['field_amount_transferred']['settings'] = array(
    'click_to_edit' => 0,
    'empty_text' => '',
    'fallback_format' => 'number_decimal',
    'fallback_settings' => array(
      'thousand_separator' => ',',
      'decimal_separator' => '.',
      'scale' => '2',
      'prefix_suffix' => 1,
    ),
  );
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['hide_alter_empty'] = FALSE;
  /* Field: Content: Notes */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['alter']['max_length'] = '160';
  $handler->display->display_options['fields']['body']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['body']['alter']['trim'] = TRUE;
  $handler->display->display_options['fields']['body']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['body']['type'] = 'editable';
  $handler->display->display_options['fields']['body']['settings'] = array(
    'click_to_edit' => 1,
    'empty_text' => '',
    'fallback_format' => 'text_default',
  );
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Actions';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = 'Use for a payment';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['path'] = 'node/add/payment?edit[field_line_item][und]=[nid]&destination=income';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['edit_node']['text'] = 'Edit';
  /* Field: Actions: Copy */
  $handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
  $handler->display->display_options['fields']['nothing_2']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_2']['ui_name'] = 'Actions: Copy';
  $handler->display->display_options['fields']['nothing_2']['label'] = '';
  $handler->display->display_options['fields']['nothing_2']['alter']['text'] = 'Copy';
  $handler->display->display_options['fields']['nothing_2']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['path'] = 'node/[nid]/clone?destination=income';
  $handler->display->display_options['fields']['nothing_2']['element_label_colon'] = FALSE;
  /* Field: Content: Delete link */
  $handler->display->display_options['fields']['delete_node']['id'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['delete_node']['field'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['delete_node']['text'] = 'Delete';
  /* Field: Add new income link */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['ui_name'] = 'Add new income link';
  $handler->display->display_options['fields']['nothing_1']['label'] = '';
  $handler->display->display_options['fields']['nothing_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'Add new income';
  $handler->display->display_options['fields']['nothing_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['path'] = 'node/add/income?destination=income';
  $handler->display->display_options['fields']['nothing_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nothing_1']['empty'] = 'Add new income';
  $handler->display->display_options['fields']['nothing_1']['empty_zero'] = TRUE;
  /* Field: Content: All taxonomy terms */
  $handler->display->display_options['fields']['term_node_tid']['id'] = 'term_node_tid';
  $handler->display->display_options['fields']['term_node_tid']['table'] = 'node';
  $handler->display->display_options['fields']['term_node_tid']['field'] = 'term_node_tid';
  $handler->display->display_options['fields']['term_node_tid']['label'] = '';
  $handler->display->display_options['fields']['term_node_tid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['term_node_tid']['vocabularies'] = array(
    'earning_account' => 0,
    'paid_with' => 0,
    'tags' => 0,
  );
  /* Field: Content: Non-business payment total */
  $handler->display->display_options['fields']['field_non_business_payment_total']['id'] = 'field_non_business_payment_total';
  $handler->display->display_options['fields']['field_non_business_payment_total']['table'] = 'field_data_field_non_business_payment_total';
  $handler->display->display_options['fields']['field_non_business_payment_total']['field'] = 'field_non_business_payment_total';
  $handler->display->display_options['fields']['field_non_business_payment_total']['label'] = '';
  $handler->display->display_options['fields']['field_non_business_payment_total']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_non_business_payment_total']['alter']['text'] = 'Non-biz: [field_non_business_payment_total]';
  $handler->display->display_options['fields']['field_non_business_payment_total']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
    'income' => 'income',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Amount remaining (field_remaining) */
  $handler->display->display_options['filters']['field_remaining_value']['id'] = 'field_remaining_value';
  $handler->display->display_options['filters']['field_remaining_value']['table'] = 'field_data_field_remaining';
  $handler->display->display_options['filters']['field_remaining_value']['field'] = 'field_remaining_value';
  $handler->display->display_options['filters']['field_remaining_value']['operator'] = '>=';
  $handler->display->display_options['filters']['field_remaining_value']['group'] = 1;
  $handler->display->display_options['filters']['field_remaining_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_remaining_value']['expose']['operator_id'] = 'field_remaining_value_op';
  $handler->display->display_options['filters']['field_remaining_value']['expose']['label'] = 'Amount remaining';
  $handler->display->display_options['filters']['field_remaining_value']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['field_remaining_value']['expose']['operator'] = 'field_remaining_value_op';
  $handler->display->display_options['filters']['field_remaining_value']['expose']['identifier'] = 'field_remaining_value';
  $handler->display->display_options['filters']['field_remaining_value']['expose']['remember'] = TRUE;
  /* Filter criterion: Content: Amount transferred (field_amount_transferred) */
  $handler->display->display_options['filters']['field_amount_transferred_value']['id'] = 'field_amount_transferred_value';
  $handler->display->display_options['filters']['field_amount_transferred_value']['table'] = 'field_data_field_amount_transferred';
  $handler->display->display_options['filters']['field_amount_transferred_value']['field'] = 'field_amount_transferred_value';
  $handler->display->display_options['filters']['field_amount_transferred_value']['group'] = 1;
  $handler->display->display_options['filters']['field_amount_transferred_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_amount_transferred_value']['expose']['operator_id'] = 'field_amount_transferred_value_op';
  $handler->display->display_options['filters']['field_amount_transferred_value']['expose']['label'] = 'Transferred';
  $handler->display->display_options['filters']['field_amount_transferred_value']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['field_amount_transferred_value']['expose']['operator'] = 'field_amount_transferred_value_op';
  $handler->display->display_options['filters']['field_amount_transferred_value']['expose']['identifier'] = 'field_amount_transferred_value';
  $handler->display->display_options['filters']['field_amount_transferred_value']['expose']['remember'] = TRUE;
  /* Filter criterion: Date: Date (node) */
  $handler->display->display_options['filters']['date_filter']['id'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['table'] = 'node';
  $handler->display->display_options['filters']['date_filter']['field'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['operator'] = '<=';
  $handler->display->display_options['filters']['date_filter']['group'] = 1;
  $handler->display->display_options['filters']['date_filter']['exposed'] = TRUE;
  $handler->display->display_options['filters']['date_filter']['expose']['operator_id'] = 'date_filter_op';
  $handler->display->display_options['filters']['date_filter']['expose']['label'] = 'Date';
  $handler->display->display_options['filters']['date_filter']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['date_filter']['expose']['operator'] = 'date_filter_op';
  $handler->display->display_options['filters']['date_filter']['expose']['identifier'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['date_filter']['expose']['remember_roles'] = array(
    2 => '2',
    3 => '3',
    4 => '4',
    1 => 0,
  );
  $handler->display->display_options['filters']['date_filter']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['date_filter']['year_range'] = '-3:2100';
  $handler->display->display_options['filters']['date_filter']['date_fields'] = array(
    'field_data_field_effective_date.field_effective_date_value' => 'field_data_field_effective_date.field_effective_date_value',
  );
  /* Filter criterion: Content: Tax paid (field_tax_paid) */
  $handler->display->display_options['filters']['field_tax_paid_value']['id'] = 'field_tax_paid_value';
  $handler->display->display_options['filters']['field_tax_paid_value']['table'] = 'field_data_field_tax_paid';
  $handler->display->display_options['filters']['field_tax_paid_value']['field'] = 'field_tax_paid_value';
  $handler->display->display_options['filters']['field_tax_paid_value']['group'] = 1;
  $handler->display->display_options['filters']['field_tax_paid_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_tax_paid_value']['expose']['operator_id'] = 'field_tax_paid_value_op';
  $handler->display->display_options['filters']['field_tax_paid_value']['expose']['label'] = 'Tax paid';
  $handler->display->display_options['filters']['field_tax_paid_value']['expose']['operator'] = 'field_tax_paid_value_op';
  $handler->display->display_options['filters']['field_tax_paid_value']['expose']['identifier'] = 'field_tax_paid_value';
  $handler->display->display_options['filters']['field_tax_paid_value']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_tax_paid_value']['expose']['remember_roles'] = array(
    2 => '2',
    3 => '3',
    4 => '4',
    1 => 0,
  );
  /* Filter criterion: Content: R&amp;D paid (field_r_d_paid) */
  $handler->display->display_options['filters']['field_r_d_paid_value']['id'] = 'field_r_d_paid_value';
  $handler->display->display_options['filters']['field_r_d_paid_value']['table'] = 'field_data_field_r_d_paid';
  $handler->display->display_options['filters']['field_r_d_paid_value']['field'] = 'field_r_d_paid_value';
  $handler->display->display_options['filters']['field_r_d_paid_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_r_d_paid_value']['expose']['operator_id'] = 'field_r_d_paid_value_op';
  $handler->display->display_options['filters']['field_r_d_paid_value']['expose']['label'] = 'R&D paid';
  $handler->display->display_options['filters']['field_r_d_paid_value']['expose']['operator'] = 'field_r_d_paid_value_op';
  $handler->display->display_options['filters']['field_r_d_paid_value']['expose']['identifier'] = 'field_r_d_paid_value';
  $handler->display->display_options['filters']['field_r_d_paid_value']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_r_d_paid_value']['expose']['remember_roles'] = array(
    2 => '2',
    3 => '3',
    4 => '4',
    1 => 0,
  );
  /* Filter criterion: Content: Has taxonomy term */
  $handler->display->display_options['filters']['tid']['id'] = 'tid';
  $handler->display->display_options['filters']['tid']['table'] = 'taxonomy_index';
  $handler->display->display_options['filters']['tid']['field'] = 'tid';
  $handler->display->display_options['filters']['tid']['value'] = '';
  $handler->display->display_options['filters']['tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['tid']['expose']['operator_id'] = 'tid_op';
  $handler->display->display_options['filters']['tid']['expose']['label'] = 'Earnings account';
  $handler->display->display_options['filters']['tid']['expose']['operator'] = 'tid_op';
  $handler->display->display_options['filters']['tid']['expose']['identifier'] = 'tid';
  $handler->display->display_options['filters']['tid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['tid']['expose']['remember_roles'] = array(
    2 => '2',
    3 => '3',
    4 => '4',
    1 => 0,
  );
  $handler->display->display_options['filters']['tid']['vocabulary'] = 'earning_account';
  $handler->display->display_options['path'] = 'income';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Income';
  $handler->display->display_options['menu']['weight'] = '5';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $export['finance_track_money'] = $view;

  $view = new view();
  $view->name = 'income_fields';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Income fields';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Income fields';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Tax type */
  $handler->display->display_options['fields']['field_tax_type']['id'] = 'field_tax_type';
  $handler->display->display_options['fields']['field_tax_type']['table'] = 'field_data_field_tax_type';
  $handler->display->display_options['fields']['field_tax_type']['field'] = 'field_tax_type';
  $handler->display->display_options['fields']['field_tax_type']['type'] = 'editable';
  $handler->display->display_options['fields']['field_tax_type']['settings'] = array(
    'click_to_edit' => 0,
    'empty_text' => '',
    'fallback_format' => 'entityreference_label',
    'fallback_settings' => array(
      'link' => 1,
    ),
  );
  /* Field: Content: R&D percentage */
  $handler->display->display_options['fields']['field_r_d_percentage']['id'] = 'field_r_d_percentage';
  $handler->display->display_options['fields']['field_r_d_percentage']['table'] = 'field_data_field_r_d_percentage';
  $handler->display->display_options['fields']['field_r_d_percentage']['field'] = 'field_r_d_percentage';
  $handler->display->display_options['fields']['field_r_d_percentage']['type'] = 'editable';
  $handler->display->display_options['fields']['field_r_d_percentage']['settings'] = array(
    'click_to_edit' => 0,
    'empty_text' => '',
    'fallback_format' => 'entityreference_label',
    'fallback_settings' => array(
      'link' => 1,
    ),
  );
  /* Field: Content: Tax paid */
  $handler->display->display_options['fields']['field_tax_paid']['id'] = 'field_tax_paid';
  $handler->display->display_options['fields']['field_tax_paid']['table'] = 'field_data_field_tax_paid';
  $handler->display->display_options['fields']['field_tax_paid']['field'] = 'field_tax_paid';
  $handler->display->display_options['fields']['field_tax_paid']['type'] = 'editable';
  $handler->display->display_options['fields']['field_tax_paid']['settings'] = array(
    'click_to_edit' => 0,
    'empty_text' => '',
    'fallback_format' => 'list_default',
  );
  /* Field: Content: R&D paid */
  $handler->display->display_options['fields']['field_r_d_paid']['id'] = 'field_r_d_paid';
  $handler->display->display_options['fields']['field_r_d_paid']['table'] = 'field_data_field_r_d_paid';
  $handler->display->display_options['fields']['field_r_d_paid']['field'] = 'field_r_d_paid';
  $handler->display->display_options['fields']['field_r_d_paid']['type'] = 'editable';
  $handler->display->display_options['fields']['field_r_d_paid']['settings'] = array(
    'click_to_edit' => 0,
    'empty_text' => '',
    'fallback_format' => 'list_default',
  );
  /* Field: Content: Tax (override) */
  $handler->display->display_options['fields']['field_tax_override']['id'] = 'field_tax_override';
  $handler->display->display_options['fields']['field_tax_override']['table'] = 'field_data_field_tax_override';
  $handler->display->display_options['fields']['field_tax_override']['field'] = 'field_tax_override';
  $handler->display->display_options['fields']['field_tax_override']['type'] = 'editable';
  $handler->display->display_options['fields']['field_tax_override']['settings'] = array(
    'click_to_edit' => 0,
    'empty_text' => '',
    'fallback_format' => 'number_decimal',
    'fallback_settings' => array(
      'thousand_separator' => ',',
      'decimal_separator' => '.',
      'scale' => '2',
      'prefix_suffix' => 1,
    ),
  );
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
    'income' => 'income',
  );
  /* Filter criterion: Content: Effective date (field_effective_date) */
  $handler->display->display_options['filters']['field_effective_date_value']['id'] = 'field_effective_date_value';
  $handler->display->display_options['filters']['field_effective_date_value']['table'] = 'field_data_field_effective_date';
  $handler->display->display_options['filters']['field_effective_date_value']['field'] = 'field_effective_date_value';
  $handler->display->display_options['filters']['field_effective_date_value']['operator'] = '>=';
  $handler->display->display_options['filters']['field_effective_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_effective_date_value']['expose']['operator_id'] = 'field_effective_date_value_op';
  $handler->display->display_options['filters']['field_effective_date_value']['expose']['label'] = 'Effective date (field_effective_date)';
  $handler->display->display_options['filters']['field_effective_date_value']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['field_effective_date_value']['expose']['operator'] = 'field_effective_date_value_op';
  $handler->display->display_options['filters']['field_effective_date_value']['expose']['identifier'] = 'field_effective_date_value';
  $handler->display->display_options['filters']['field_effective_date_value']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_effective_date_value']['default_date'] = '-8 weeks';
  $handler->display->display_options['filters']['field_effective_date_value']['year_range'] = '2011:+0';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'income-fields';
  $export['income_fields'] = $view;

  $view = new view();
  $view->name = 'payment_sources';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'payment_sources';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => '3',
    4 => '4',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'field_special',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['row_plugin'] = 'fields';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['content'] = 'No income line items are available right now. Add one and come back.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Content: Special */
  $handler->display->display_options['fields']['field_special']['id'] = 'field_special';
  $handler->display->display_options['fields']['field_special']['table'] = 'field_data_field_special';
  $handler->display->display_options['fields']['field_special']['field'] = 'field_special';
  $handler->display->display_options['fields']['field_special']['exclude'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Effective date */
  $handler->display->display_options['fields']['field_effective_date']['id'] = 'field_effective_date';
  $handler->display->display_options['fields']['field_effective_date']['table'] = 'field_data_field_effective_date';
  $handler->display->display_options['fields']['field_effective_date']['field'] = 'field_effective_date';
  $handler->display->display_options['fields']['field_effective_date']['label'] = '';
  $handler->display->display_options['fields']['field_effective_date']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_effective_date']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_effective_date']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  /* Field: Content: Amount remaining */
  $handler->display->display_options['fields']['field_remaining']['id'] = 'field_remaining';
  $handler->display->display_options['fields']['field_remaining']['table'] = 'field_data_field_remaining';
  $handler->display->display_options['fields']['field_remaining']['field'] = 'field_remaining';
  $handler->display->display_options['fields']['field_remaining']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_remaining']['type'] = 'computed_field_default';
  /* Sort criterion: Content: Special (field_special) */
  $handler->display->display_options['sorts']['field_special_value']['id'] = 'field_special_value';
  $handler->display->display_options['sorts']['field_special_value']['table'] = 'field_data_field_special';
  $handler->display->display_options['sorts']['field_special_value']['field'] = 'field_special_value';
  /* Sort criterion: Content: Effective date (field_effective_date) */
  $handler->display->display_options['sorts']['field_effective_date_value']['id'] = 'field_effective_date_value';
  $handler->display->display_options['sorts']['field_effective_date_value']['table'] = 'field_data_field_effective_date';
  $handler->display->display_options['sorts']['field_effective_date_value']['field'] = 'field_effective_date_value';
  /* Sort criterion: Content: Amount remaining (field_remaining) */
  $handler->display->display_options['sorts']['field_remaining_value']['id'] = 'field_remaining_value';
  $handler->display->display_options['sorts']['field_remaining_value']['table'] = 'field_data_field_remaining';
  $handler->display->display_options['sorts']['field_remaining_value']['field'] = 'field_remaining_value';
  $handler->display->display_options['sorts']['field_remaining_value']['order'] = 'DESC';
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
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'income' => 'income',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  /* Filter criterion: Content: Amount remaining (field_remaining) */
  $handler->display->display_options['filters']['field_remaining_value']['id'] = 'field_remaining_value';
  $handler->display->display_options['filters']['field_remaining_value']['table'] = 'field_data_field_remaining';
  $handler->display->display_options['filters']['field_remaining_value']['field'] = 'field_remaining_value';
  $handler->display->display_options['filters']['field_remaining_value']['operator'] = '!=';
  $handler->display->display_options['filters']['field_remaining_value']['value']['value'] = '0';

  /* Display: Entity Reference: Sources */
  $handler = $view->new_display('entityreference', 'Entity Reference: Sources', 'entityreference_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'entityreference_style';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'field_special',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['style_options']['search_fields'] = array(
    'title' => 'title',
    'field_special' => 0,
    'field_effective_date' => 0,
    'field_remaining' => 0,
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'entityreference_fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;

  /* Display: Entity Reference: Targets */
  $handler = $view->new_display('entityreference', 'Entity Reference: Targets', 'entityreference_3');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'entityreference_style';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'field_special',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['style_options']['search_fields'] = array(
    'title' => 'title',
    'field_special' => 0,
    'field_effective_date' => 0,
    'field_remaining' => 0,
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'entityreference_fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'expense' => 'expense',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  /* Filter criterion: Content: Amount remaining (field_remaining) */
  $handler->display->display_options['filters']['field_remaining_value']['id'] = 'field_remaining_value';
  $handler->display->display_options['filters']['field_remaining_value']['table'] = 'field_data_field_remaining';
  $handler->display->display_options['filters']['field_remaining_value']['field'] = 'field_remaining_value';
  $handler->display->display_options['filters']['field_remaining_value']['operator'] = '!=';
  $handler->display->display_options['filters']['field_remaining_value']['value']['value'] = '0';
  $export['payment_sources'] = $view;

  $view = new view();
  $view->name = 'payments';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Payments';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Payments';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'views_bulk_operations' => 'views_bulk_operations',
    'nid' => 'nid',
    'field_paid' => 'field_paid',
    'field_business_related' => 'field_business_related',
    'title_1' => 'title_1',
    'field_remaining' => 'title_1',
    'body' => 'title_1',
    'field_amount' => 'field_amount',
    'field_effective_date_1' => 'field_effective_date_1',
    'title' => 'title',
    'field_effective_date' => 'field_effective_date',
    'field_amount_1' => 'field_amount_1',
    'field_amount_transferred' => 'field_amount_transferred',
    'expression' => 'field_effective_date',
    'body_1' => 'title',
    'nothing' => 'nothing',
    'term_node_tid' => 'title_1',
    'term_node_tid_1' => 'title',
    'body_2' => 'field_amount',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'views_bulk_operations' => array(
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
    'field_paid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_business_related' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '<br />',
      'empty_column' => 0,
    ),
    'field_remaining' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'body' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_amount' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_effective_date_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '<br />',
      'empty_column' => 0,
    ),
    'field_effective_date' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '<br />',
      'empty_column' => 0,
    ),
    'field_amount_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_amount_transferred' => array(
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
    'body_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing' => array(
      'align' => '',
      'separator' => ' |',
      'empty_column' => 0,
    ),
    'term_node_tid' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'term_node_tid_1' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'body_2' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_target_line_item_target_id']['id'] = 'field_target_line_item_target_id';
  $handler->display->display_options['relationships']['field_target_line_item_target_id']['table'] = 'field_data_field_target_line_item';
  $handler->display->display_options['relationships']['field_target_line_item_target_id']['field'] = 'field_target_line_item_target_id';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_line_item_target_id']['id'] = 'field_line_item_target_id';
  $handler->display->display_options['relationships']['field_line_item_target_id']['table'] = 'field_data_field_line_item';
  $handler->display->display_options['relationships']['field_line_item_target_id']['field'] = 'field_line_item_target_id';
  /* Field: Bulk operations: Content */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'node';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = 'Select';
  $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::node_assign_owner_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 1,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::system_message_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::views_bulk_operations_script_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::node_make_sticky_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::node_make_unsticky_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 1,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'show_all_tokens' => 1,
        'display_values' => array(
          '_all_' => '_all_',
        ),
      ),
      'postpone_processing' => 0,
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
    'action::node_promote_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::node_publish_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::system_goto_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::node_unpromote_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::node_save_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::system_send_email_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::node_unpublish_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::node_unpublish_by_keyword_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
    'action::pathauto_node_update_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'postpone_processing' => 0,
    ),
  );
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['hide_alter_empty'] = FALSE;
  /* Field: Content: Paid */
  $handler->display->display_options['fields']['field_paid']['id'] = 'field_paid';
  $handler->display->display_options['fields']['field_paid']['table'] = 'field_data_field_paid';
  $handler->display->display_options['fields']['field_paid']['field'] = 'field_paid';
  $handler->display->display_options['fields']['field_paid']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_paid']['type'] = 'editable';
  $handler->display->display_options['fields']['field_paid']['settings'] = array(
    'click_to_edit' => 0,
    'empty_text' => '',
    'fallback_format' => 'list_default',
  );
  /* Field: Content: Business-related */
  $handler->display->display_options['fields']['field_business_related']['id'] = 'field_business_related';
  $handler->display->display_options['fields']['field_business_related']['table'] = 'field_data_field_business_related';
  $handler->display->display_options['fields']['field_business_related']['field'] = 'field_business_related';
  $handler->display->display_options['fields']['field_business_related']['relationship'] = 'field_target_line_item_target_id';
  $handler->display->display_options['fields']['field_business_related']['label'] = 'Biz';
  $handler->display->display_options['fields']['field_business_related']['hide_alter_empty'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['relationship'] = 'field_target_line_item_target_id';
  $handler->display->display_options['fields']['title_1']['label'] = 'Paying';
  $handler->display->display_options['fields']['title_1']['hide_alter_empty'] = FALSE;
  /* Field: Content: Amount remaining */
  $handler->display->display_options['fields']['field_remaining']['id'] = 'field_remaining';
  $handler->display->display_options['fields']['field_remaining']['table'] = 'field_data_field_remaining';
  $handler->display->display_options['fields']['field_remaining']['field'] = 'field_remaining';
  $handler->display->display_options['fields']['field_remaining']['relationship'] = 'field_target_line_item_target_id';
  $handler->display->display_options['fields']['field_remaining']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_remaining']['alter']['text'] = '[field_remaining] remaining';
  $handler->display->display_options['fields']['field_remaining']['type'] = 'computed_field_unsanitized';
  /* Field: Content: Notes */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['relationship'] = 'field_target_line_item_target_id';
  $handler->display->display_options['fields']['body']['hide_alter_empty'] = FALSE;
  /* Field: Content: Amount */
  $handler->display->display_options['fields']['field_amount']['id'] = 'field_amount';
  $handler->display->display_options['fields']['field_amount']['table'] = 'field_data_field_amount';
  $handler->display->display_options['fields']['field_amount']['field'] = 'field_amount';
  $handler->display->display_options['fields']['field_amount']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_amount']['settings'] = array(
    'thousand_separator' => ',',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Effective date */
  $handler->display->display_options['fields']['field_effective_date_1']['id'] = 'field_effective_date_1';
  $handler->display->display_options['fields']['field_effective_date_1']['table'] = 'field_data_field_effective_date';
  $handler->display->display_options['fields']['field_effective_date_1']['field'] = 'field_effective_date';
  $handler->display->display_options['fields']['field_effective_date_1']['relationship'] = 'field_target_line_item_target_id';
  $handler->display->display_options['fields']['field_effective_date_1']['label'] = 'Which is due';
  $handler->display->display_options['fields']['field_effective_date_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_effective_date_1']['alter']['text'] = '[field_effective_date_1]';
  $handler->display->display_options['fields']['field_effective_date_1']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_effective_date_1']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'field_line_item_target_id';
  $handler->display->display_options['fields']['title']['label'] = 'With';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = FALSE;
  /* Field: Content: Effective date */
  $handler->display->display_options['fields']['field_effective_date']['id'] = 'field_effective_date';
  $handler->display->display_options['fields']['field_effective_date']['table'] = 'field_data_field_effective_date';
  $handler->display->display_options['fields']['field_effective_date']['field'] = 'field_effective_date';
  $handler->display->display_options['fields']['field_effective_date']['relationship'] = 'field_line_item_target_id';
  $handler->display->display_options['fields']['field_effective_date']['label'] = 'Which arrives';
  $handler->display->display_options['fields']['field_effective_date']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_effective_date']['alter']['text'] = '[field_effective_date]';
  $handler->display->display_options['fields']['field_effective_date']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_effective_date']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  /* Field: Content: Amount */
  $handler->display->display_options['fields']['field_amount_1']['id'] = 'field_amount_1';
  $handler->display->display_options['fields']['field_amount_1']['table'] = 'field_data_field_amount';
  $handler->display->display_options['fields']['field_amount_1']['field'] = 'field_amount';
  $handler->display->display_options['fields']['field_amount_1']['relationship'] = 'field_line_item_target_id';
  $handler->display->display_options['fields']['field_amount_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_amount_1']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_amount_1']['settings'] = array(
    'thousand_separator' => ' ',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Amount transferred */
  $handler->display->display_options['fields']['field_amount_transferred']['id'] = 'field_amount_transferred';
  $handler->display->display_options['fields']['field_amount_transferred']['table'] = 'field_data_field_amount_transferred';
  $handler->display->display_options['fields']['field_amount_transferred']['field'] = 'field_amount_transferred';
  $handler->display->display_options['fields']['field_amount_transferred']['relationship'] = 'field_line_item_target_id';
  $handler->display->display_options['fields']['field_amount_transferred']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_amount_transferred']['empty'] = '0';
  $handler->display->display_options['fields']['field_amount_transferred']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_amount_transferred']['settings'] = array(
    'thousand_separator' => ',',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Global: Math expression */
  $handler->display->display_options['fields']['expression']['id'] = 'expression';
  $handler->display->display_options['fields']['expression']['table'] = 'views';
  $handler->display->display_options['fields']['expression']['field'] = 'expression';
  $handler->display->display_options['fields']['expression']['label'] = 'Percent usable';
  $handler->display->display_options['fields']['expression']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['expression']['alter']['text'] = '(Transferred: [field_amount_transferred]/[field_amount_1], [expression])';
  $handler->display->display_options['fields']['expression']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['expression']['precision'] = '0';
  $handler->display->display_options['fields']['expression']['suffix'] = '%';
  $handler->display->display_options['fields']['expression']['expression'] = '([field_amount_transferred] / [field_amount_1]) * 100';
  /* Field: Content: Notes */
  $handler->display->display_options['fields']['body_1']['id'] = 'body_1';
  $handler->display->display_options['fields']['body_1']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body_1']['field'] = 'body';
  $handler->display->display_options['fields']['body_1']['relationship'] = 'field_line_item_target_id';
  $handler->display->display_options['fields']['body_1']['hide_alter_empty'] = FALSE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Actions';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = 'Edit';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['path'] = 'node/[nid]/edit?destination=payments';
  /* Field: Content: All taxonomy terms */
  $handler->display->display_options['fields']['term_node_tid']['id'] = 'term_node_tid';
  $handler->display->display_options['fields']['term_node_tid']['table'] = 'node';
  $handler->display->display_options['fields']['term_node_tid']['field'] = 'term_node_tid';
  $handler->display->display_options['fields']['term_node_tid']['relationship'] = 'field_target_line_item_target_id';
  $handler->display->display_options['fields']['term_node_tid']['label'] = '';
  $handler->display->display_options['fields']['term_node_tid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['term_node_tid']['vocabularies'] = array(
    'paid_with' => 0,
    'earning_account' => 0,
    'tags' => 0,
  );
  /* Field: Content: All taxonomy terms */
  $handler->display->display_options['fields']['term_node_tid_1']['id'] = 'term_node_tid_1';
  $handler->display->display_options['fields']['term_node_tid_1']['table'] = 'node';
  $handler->display->display_options['fields']['term_node_tid_1']['field'] = 'term_node_tid';
  $handler->display->display_options['fields']['term_node_tid_1']['relationship'] = 'field_line_item_target_id';
  $handler->display->display_options['fields']['term_node_tid_1']['label'] = '';
  $handler->display->display_options['fields']['term_node_tid_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['term_node_tid_1']['vocabularies'] = array(
    'paid_with' => 0,
    'earning_account' => 0,
    'tags' => 0,
  );
  /* Field: Content: Notes */
  $handler->display->display_options['fields']['body_2']['id'] = 'body_2';
  $handler->display->display_options['fields']['body_2']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body_2']['field'] = 'body';
  $handler->display->display_options['fields']['body_2']['label'] = '';
  $handler->display->display_options['fields']['body_2']['element_label_colon'] = FALSE;
  /* Sort criterion: Content: Effective date (field_effective_date) */
  $handler->display->display_options['sorts']['field_effective_date_value']['id'] = 'field_effective_date_value';
  $handler->display->display_options['sorts']['field_effective_date_value']['table'] = 'field_data_field_effective_date';
  $handler->display->display_options['sorts']['field_effective_date_value']['field'] = 'field_effective_date_value';
  $handler->display->display_options['sorts']['field_effective_date_value']['relationship'] = 'field_line_item_target_id';
  $handler->display->display_options['sorts']['field_effective_date_value']['order'] = 'DESC';
  /* Sort criterion: Content: Effective date (field_effective_date) */
  $handler->display->display_options['sorts']['field_effective_date_value_1']['id'] = 'field_effective_date_value_1';
  $handler->display->display_options['sorts']['field_effective_date_value_1']['table'] = 'field_data_field_effective_date';
  $handler->display->display_options['sorts']['field_effective_date_value_1']['field'] = 'field_effective_date_value';
  $handler->display->display_options['sorts']['field_effective_date_value_1']['relationship'] = 'field_target_line_item_target_id';
  $handler->display->display_options['sorts']['field_effective_date_value_1']['order'] = 'DESC';
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
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'payment' => 'payment',
  );
  /* Filter criterion: Content: Paid (field_paid) */
  $handler->display->display_options['filters']['field_paid_value']['id'] = 'field_paid_value';
  $handler->display->display_options['filters']['field_paid_value']['table'] = 'field_data_field_paid';
  $handler->display->display_options['filters']['field_paid_value']['field'] = 'field_paid_value';
  $handler->display->display_options['filters']['field_paid_value']['value'] = array(
    0 => '0',
  );
  $handler->display->display_options['filters']['field_paid_value']['group'] = '0';
  $handler->display->display_options['filters']['field_paid_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_paid_value']['expose']['operator_id'] = 'field_paid_value_op';
  $handler->display->display_options['filters']['field_paid_value']['expose']['label'] = 'Paid';
  $handler->display->display_options['filters']['field_paid_value']['expose']['operator'] = 'field_paid_value_op';
  $handler->display->display_options['filters']['field_paid_value']['expose']['identifier'] = 'field_paid_value';
  $handler->display->display_options['filters']['field_paid_value']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_paid_value']['expose']['remember_roles'] = array(
    2 => '2',
    3 => '3',
    4 => '4',
    1 => 0,
  );
  /* Filter criterion: Content: Effective date (field_effective_date) */
  $handler->display->display_options['filters']['field_effective_date_value']['id'] = 'field_effective_date_value';
  $handler->display->display_options['filters']['field_effective_date_value']['table'] = 'field_data_field_effective_date';
  $handler->display->display_options['filters']['field_effective_date_value']['field'] = 'field_effective_date_value';
  $handler->display->display_options['filters']['field_effective_date_value']['relationship'] = 'field_line_item_target_id';
  $handler->display->display_options['filters']['field_effective_date_value']['operator'] = '>=';
  $handler->display->display_options['filters']['field_effective_date_value']['group'] = '0';
  $handler->display->display_options['filters']['field_effective_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_effective_date_value']['expose']['operator_id'] = 'field_effective_date_value_op';
  $handler->display->display_options['filters']['field_effective_date_value']['expose']['label'] = 'Effective date of source income';
  $handler->display->display_options['filters']['field_effective_date_value']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['field_effective_date_value']['expose']['operator'] = 'field_effective_date_value_op';
  $handler->display->display_options['filters']['field_effective_date_value']['expose']['identifier'] = 'field_effective_date_value';
  $handler->display->display_options['filters']['field_effective_date_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_effective_date_value']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['field_effective_date_value']['year_range'] = '-10:+0';
  /* Filter criterion: Content: Effective date (field_effective_date) */
  $handler->display->display_options['filters']['field_effective_date_value_1']['id'] = 'field_effective_date_value_1';
  $handler->display->display_options['filters']['field_effective_date_value_1']['table'] = 'field_data_field_effective_date';
  $handler->display->display_options['filters']['field_effective_date_value_1']['field'] = 'field_effective_date_value';
  $handler->display->display_options['filters']['field_effective_date_value_1']['relationship'] = 'field_target_line_item_target_id';
  $handler->display->display_options['filters']['field_effective_date_value_1']['operator'] = '<=';
  $handler->display->display_options['filters']['field_effective_date_value_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_effective_date_value_1']['expose']['operator_id'] = 'field_effective_date_value_1_op';
  $handler->display->display_options['filters']['field_effective_date_value_1']['expose']['label'] = 'Due date';
  $handler->display->display_options['filters']['field_effective_date_value_1']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['field_effective_date_value_1']['expose']['operator'] = 'field_effective_date_value_1_op';
  $handler->display->display_options['filters']['field_effective_date_value_1']['expose']['identifier'] = 'field_effective_date_value_1';
  $handler->display->display_options['filters']['field_effective_date_value_1']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_effective_date_value_1']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_effective_date_value_1']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['field_effective_date_value_1']['year_range'] = '-3:+10';
  /* Filter criterion: Content: Card/account to pay back (field_paid_with) */
  $handler->display->display_options['filters']['field_paid_with_tid']['id'] = 'field_paid_with_tid';
  $handler->display->display_options['filters']['field_paid_with_tid']['table'] = 'field_data_field_paid_with';
  $handler->display->display_options['filters']['field_paid_with_tid']['field'] = 'field_paid_with_tid';
  $handler->display->display_options['filters']['field_paid_with_tid']['relationship'] = 'field_target_line_item_target_id';
  $handler->display->display_options['filters']['field_paid_with_tid']['value'] = '';
  $handler->display->display_options['filters']['field_paid_with_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_paid_with_tid']['expose']['operator_id'] = 'field_paid_with_tid_op';
  $handler->display->display_options['filters']['field_paid_with_tid']['expose']['label'] = 'Expense-incurring account';
  $handler->display->display_options['filters']['field_paid_with_tid']['expose']['operator'] = 'field_paid_with_tid_op';
  $handler->display->display_options['filters']['field_paid_with_tid']['expose']['identifier'] = 'field_paid_with_tid';
  $handler->display->display_options['filters']['field_paid_with_tid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_paid_with_tid']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_paid_with_tid']['expose']['remember_roles'] = array(
    2 => '2',
    3 => '3',
    1 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_paid_with_tid']['vocabulary'] = 'paid_with';
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['relationship'] = 'field_target_line_item_target_id';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    3 => '3',
    1 => 0,
    4 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['path'] = 'payments';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Payments';
  $handler->display->display_options['menu']['weight'] = '15';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;

  /* Display: Income Block */
  $handler = $view->new_display('block', 'Income Block', 'block');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Used for these payments';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['relationship'] = 'field_line_item_target_id';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['nid']['exception']['value'] = '';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['nid']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['nid']['validate_options']['types'] = array(
    'income' => 'income',
  );
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'payment' => 'payment',
  );

  /* Display: Expense Block */
  $handler = $view->new_display('block', 'Expense Block', 'block_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Paid down with these payments';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['relationship'] = 'field_target_line_item_target_id';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['nid']['exception']['value'] = '';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['nid']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['nid']['validate_options']['types'] = array(
    'expense' => 'expense',
  );
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
    'payment' => 'payment',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  $export['payments'] = $view;

  return $export;
}
