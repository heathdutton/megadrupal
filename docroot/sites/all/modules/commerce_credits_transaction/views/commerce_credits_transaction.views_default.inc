<?php
/**
 * @file
 * Commerce Credits Transaction Default Views
 */


/**
 * Implements hook_views_default_views().
 */
function commerce_credits_transaction_views_default_views() {
  $views = array();
  
  // Begin credit_transactions view.
  $view = new view();
  $view->name = 'credit_transactions';
  $view->description = 'Summary of commerce credit transactions';
  $view->tag = 'default';
  $view->base_table = 'commerce_credits_transaction';
  $view->human_name = 'Credit Transactions';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Commerce Credits Transactions';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'manage commerce credits transaction';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'views_bulk_operations' => 'views_bulk_operations',
    'from_entity_id' => 'from_entity_id',
    'to_entity_id' => 'to_entity_id',
    'transaction' => 'transaction',
    'status' => 'status',
    'credits' => 'credits',
    'transaction_date' => 'transaction_date',
  );
  $handler->display->display_options['style_options']['default'] = 'transaction_date';
  $handler->display->display_options['style_options']['info'] = array(
    'views_bulk_operations' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'from_entity_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'to_entity_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'transaction' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'credits' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-right',
      'separator' => '',
      'empty_column' => 0,
    ),
    'transaction_date' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => 'views-align-left',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'There were no results for your specified criteria.';
  $handler->display->display_options['empty']['area']['format'] = 'plain_text';
  /* Relationship: Commerce Credits Transaction: From Credit Entity */
  $handler->display->display_options['relationships']['from_cid']['id'] = 'from_cid';
  $handler->display->display_options['relationships']['from_cid']['table'] = 'commerce_credits_transaction';
  $handler->display->display_options['relationships']['from_cid']['field'] = 'from_cid';
  $handler->display->display_options['relationships']['from_cid']['label'] = 'From';
  /* Relationship: Commerce Credits Transaction: To Credit Entity */
  $handler->display->display_options['relationships']['to_cid']['id'] = 'to_cid';
  $handler->display->display_options['relationships']['to_cid']['table'] = 'commerce_credits_transaction';
  $handler->display->display_options['relationships']['to_cid']['field'] = 'to_cid';
  $handler->display->display_options['relationships']['to_cid']['label'] = 'To';
  /* Field: Bulk operations: Commerce Credits Transaction */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'commerce_credits_transaction';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '1';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_result'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 1,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 1,
      'label' => 'Remove',
    ),
    'action::system_message_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_script_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 1,
      'label' => 'Modify transaction',
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
    'action::system_goto_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::system_send_email_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
  /* Field: Commerce Credits Transaction: From Entity Id */
  $handler->display->display_options['fields']['from_entity_id']['id'] = 'from_entity_id';
  $handler->display->display_options['fields']['from_entity_id']['table'] = 'commerce_credits_transaction';
  $handler->display->display_options['fields']['from_entity_id']['field'] = 'from_entity_id';
  $handler->display->display_options['fields']['from_entity_id']['label'] = 'From Entity';
  /* Field: Commerce Credits Transaction: To Entity Id */
  $handler->display->display_options['fields']['to_entity_id']['id'] = 'to_entity_id';
  $handler->display->display_options['fields']['to_entity_id']['table'] = 'commerce_credits_transaction';
  $handler->display->display_options['fields']['to_entity_id']['field'] = 'to_entity_id';
  $handler->display->display_options['fields']['to_entity_id']['label'] = 'To Entity';
  /* Field: Commerce Credits Transaction: Transaction */
  $handler->display->display_options['fields']['transaction']['id'] = 'transaction';
  $handler->display->display_options['fields']['transaction']['table'] = 'commerce_credits_transaction';
  $handler->display->display_options['fields']['transaction']['field'] = 'transaction';
  /* Field: Commerce Credits Transaction: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'commerce_credits_transaction';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  /* Field: Commerce Credits Transaction: Credits */
  $handler->display->display_options['fields']['credits']['id'] = 'credits';
  $handler->display->display_options['fields']['credits']['table'] = 'commerce_credits_transaction';
  $handler->display->display_options['fields']['credits']['field'] = 'credits';
  $handler->display->display_options['fields']['credits']['separator'] = '';
  $handler->display->display_options['fields']['credits']['format_plural'] = TRUE;
  $handler->display->display_options['fields']['credits']['format_plural_singular'] = '1 credit';
  $handler->display->display_options['fields']['credits']['format_plural_plural'] = '@count credits';
  /* Field: Commerce Credits Transaction: Transaction Date */
  $handler->display->display_options['fields']['transaction_date']['id'] = 'transaction_date';
  $handler->display->display_options['fields']['transaction_date']['table'] = 'commerce_credits_transaction';
  $handler->display->display_options['fields']['transaction_date']['field'] = 'transaction_date';
  $handler->display->display_options['fields']['transaction_date']['date_format'] = 'time ago';
  /* Filter criterion: Commerce Credits Transaction: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'commerce_credits_transaction';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
  );
  /* Filter criterion: Commerce Credits Transaction: Transaction */
  $handler->display->display_options['filters']['transaction']['id'] = 'transaction';
  $handler->display->display_options['filters']['transaction']['table'] = 'commerce_credits_transaction';
  $handler->display->display_options['filters']['transaction']['field'] = 'transaction';
  $handler->display->display_options['filters']['transaction']['group'] = 1;
  $handler->display->display_options['filters']['transaction']['exposed'] = TRUE;
  $handler->display->display_options['filters']['transaction']['expose']['operator_id'] = 'transaction_op';
  $handler->display->display_options['filters']['transaction']['expose']['label'] = 'Transaction';
  $handler->display->display_options['filters']['transaction']['expose']['operator'] = 'transaction_op';
  $handler->display->display_options['filters']['transaction']['expose']['identifier'] = 'transaction';
  $handler->display->display_options['filters']['transaction']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['transaction']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['transaction']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
  );
  /* Filter criterion: Commerce Credits Transaction: Transaction Date */
  $handler->display->display_options['filters']['transaction_date']['id'] = 'transaction_date';
  $handler->display->display_options['filters']['transaction_date']['table'] = 'commerce_credits_transaction';
  $handler->display->display_options['filters']['transaction_date']['field'] = 'transaction_date';
  $handler->display->display_options['filters']['transaction_date']['operator'] = 'between';
  $handler->display->display_options['filters']['transaction_date']['group'] = 1;
  $handler->display->display_options['filters']['transaction_date']['exposed'] = TRUE;
  $handler->display->display_options['filters']['transaction_date']['expose']['operator_id'] = 'transaction_date_op';
  $handler->display->display_options['filters']['transaction_date']['expose']['label'] = 'Transaction Date is between';
  $handler->display->display_options['filters']['transaction_date']['expose']['operator'] = 'transaction_date_op';
  $handler->display->display_options['filters']['transaction_date']['expose']['identifier'] = 'transaction_date';
  $handler->display->display_options['filters']['transaction_date']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
  );
  /* Filter criterion: Commerce Credits Transaction: From Entity Type */
  $handler->display->display_options['filters']['from_entity_type_11']['id'] = 'from_entity_type_11';
  $handler->display->display_options['filters']['from_entity_type_11']['table'] = 'commerce_credits_transaction';
  $handler->display->display_options['filters']['from_entity_type_11']['field'] = 'from_entity_type';
  $handler->display->display_options['filters']['from_entity_type_11']['value'] = array(
    'node' => 'node',
    'og_membership' => 'og_membership',
    'taxonomy_term' => 'taxonomy_term',
    'user' => 'user',
  );
  $handler->display->display_options['filters']['from_entity_type_11']['group'] = 1;
  $handler->display->display_options['filters']['from_entity_type_11']['exposed'] = TRUE;
  $handler->display->display_options['filters']['from_entity_type_11']['expose']['operator_id'] = 'from_entity_type_11_op';
  $handler->display->display_options['filters']['from_entity_type_11']['expose']['label'] = 'From Entity Type';
  $handler->display->display_options['filters']['from_entity_type_11']['expose']['operator'] = 'from_entity_type_11_op';
  $handler->display->display_options['filters']['from_entity_type_11']['expose']['identifier'] = 'from_entity_type_11';
  $handler->display->display_options['filters']['from_entity_type_11']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['from_entity_type_11']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
  );
  $handler->display->display_options['filters']['from_entity_type_11']['expose']['reduce'] = TRUE;
  /* Filter criterion: Commerce Credits Transaction: To Entity Type */
  $handler->display->display_options['filters']['to_entity_type_11']['id'] = 'to_entity_type_11';
  $handler->display->display_options['filters']['to_entity_type_11']['table'] = 'commerce_credits_transaction';
  $handler->display->display_options['filters']['to_entity_type_11']['field'] = 'to_entity_type';
  $handler->display->display_options['filters']['to_entity_type_11']['value'] = array(
    'node' => 'node',
    'og_membership' => 'og_membership',
    'taxonomy_term' => 'taxonomy_term',
    'user' => 'user',
  );
  $handler->display->display_options['filters']['to_entity_type_11']['group'] = 1;
  $handler->display->display_options['filters']['to_entity_type_11']['exposed'] = TRUE;
  $handler->display->display_options['filters']['to_entity_type_11']['expose']['operator_id'] = 'to_entity_type_11_op';
  $handler->display->display_options['filters']['to_entity_type_11']['expose']['label'] = 'To Entity Type';
  $handler->display->display_options['filters']['to_entity_type_11']['expose']['operator'] = 'to_entity_type_11_op';
  $handler->display->display_options['filters']['to_entity_type_11']['expose']['identifier'] = 'to_entity_type_11';
  $handler->display->display_options['filters']['to_entity_type_11']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['to_entity_type_11']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
  );
  $handler->display->display_options['filters']['to_entity_type_11']['expose']['reduce'] = TRUE;
  
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'admin/commerce/credits/transaction/summary';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'Summary';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'tab';
  $handler->display->display_options['tab_options']['title'] = 'Transaction';
  $handler->display->display_options['tab_options']['weight'] = '0';
  
  $views['credit_transactions'] = $view;
  // End credit_transactions view

  return $views;
}
