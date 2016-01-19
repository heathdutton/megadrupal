<?php

/**
 * @file
 * Provides a default administration overview using views.
 */

/**
 * Implements hook_views_default_views().
 */
function ideal_advanced_views_default_views() {
  // Begin copy and paste of output from the Export tab of a view.
  $view = new view();
  $view->name = 'ideal_transactions';
  $view->description = 'Displays all iDEAL transactions in the administrator interface.';
  $view->tag = 'default';
  $view->base_table = 'ideal_advanced_transaction';
  $view->human_name = 'iDEAL Transactions';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE;

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'iDEAL Transactions';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view ideal transactions';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'views_bulk_operations' => 'views_bulk_operations',
    'transaction_id_1' => 'transaction_id_1',
    'created_1' => 'created_1',
    'consumer_iban_1' => 'consumer_iban_1',
    'consumer_bic_1' => 'consumer_bic_1',
    'purchase_id_1' => 'purchase_id_1',
    'formatted_amount' => 'formatted_amount',
    'consumer_name_1' => 'consumer_name_1',
    'status_1' => 'status_1',
  );
  $handler->display->display_options['style_options']['default'] = 'transaction_id_1';
  $handler->display->display_options['style_options']['info'] = array(
    'views_bulk_operations' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'transaction_id_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'consumer_iban_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'consumer_bic_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'purchase_id_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'formatted_amount' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'consumer_name_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );

  /* Field: Bulk operations: IDEAL transaction */
  if(module_exists('views_bulk_operations')) {
    $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
    $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'ideal_advanced_transaction';
    $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
    $handler->display->display_options['fields']['views_bulk_operations']['label'] = 'iDEAL Transaction';
    $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
    $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
    $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
    $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
    $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
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
      'action::views_bulk_operations_delete_item' => array(
        'selected' => 1,
        'postpone_processing' => 0,
        'skip_confirmation' => 0,
        'override_label' => 0,
        'label' => '',
      ),
      'action::views_bulk_operations_delete_revision' => array(
        'selected' => 1,
        'postpone_processing' => 0,
        'skip_confirmation' => 0,
        'override_label' => 0,
        'label' => '',
      ),
    );
  }
  /* Field: IDEAL transaction: Transaction ID */
  $handler->display->display_options['fields']['transaction_id_1']['id'] = 'transaction_id_1';
  $handler->display->display_options['fields']['transaction_id_1']['table'] = 'ideal_advanced_transaction';
  $handler->display->display_options['fields']['transaction_id_1']['field'] = 'transaction_id';
  $handler->display->display_options['fields']['transaction_id_1']['separator'] = '';
  /* Field: IDEAL transaction: Created */
  $handler->display->display_options['fields']['created_1']['id'] = 'created_1';
  $handler->display->display_options['fields']['created_1']['table'] = 'ideal_advanced_transaction';
  $handler->display->display_options['fields']['created_1']['field'] = 'created';
  $handler->display->display_options['fields']['created_1']['date_format'] = 'short';
  $handler->display->display_options['fields']['created_1']['second_date_format'] = 'long';
  $handler->display->display_options['fields']['created_1']['format_date_sql'] = 0;
  /* Field: IDEAL transaction: Consumer IBAN */
  $handler->display->display_options['fields']['consumer_iban_1']['id'] = 'consumer_iban_1';
  $handler->display->display_options['fields']['consumer_iban_1']['table'] = 'ideal_advanced_transaction';
  $handler->display->display_options['fields']['consumer_iban_1']['field'] = 'consumer_iban';
  /* Field: IDEAL transaction: Consumer BIC */
  $handler->display->display_options['fields']['consumer_bic_1']['id'] = 'consumer_bic_1';
  $handler->display->display_options['fields']['consumer_bic_1']['table'] = 'ideal_advanced_transaction';
  $handler->display->display_options['fields']['consumer_bic_1']['field'] = 'consumer_bic';
  /* Field: IDEAL transaction: Purchase ID */
  $handler->display->display_options['fields']['purchase_id_1']['id'] = 'purchase_id_1';
  $handler->display->display_options['fields']['purchase_id_1']['table'] = 'ideal_advanced_transaction';
  $handler->display->display_options['fields']['purchase_id_1']['field'] = 'purchase_id';
  $handler->display->display_options['fields']['purchase_id_1']['separator'] = '';
  /* Field: IDEAL transaction: Formatted Amount */
  $handler->display->display_options['fields']['formatted_amount']['id'] = 'formatted_amount';
  $handler->display->display_options['fields']['formatted_amount']['table'] = 'views_entity_ideal_advanced_transaction';
  $handler->display->display_options['fields']['formatted_amount']['field'] = 'formatted_amount';
  $handler->display->display_options['fields']['formatted_amount']['set_precision'] = TRUE;
  $handler->display->display_options['fields']['formatted_amount']['precision'] = '2';
  $handler->display->display_options['fields']['formatted_amount']['decimal'] = ',';
  $handler->display->display_options['fields']['formatted_amount']['separator'] = '.';
  $handler->display->display_options['fields']['formatted_amount']['prefix'] = '€ ';
  $handler->display->display_options['fields']['formatted_amount']['link_to_entity'] = 0;
  /* Field: IDEAL transaction: Consumer name */
  $handler->display->display_options['fields']['consumer_name_1']['id'] = 'consumer_name_1';
  $handler->display->display_options['fields']['consumer_name_1']['table'] = 'ideal_advanced_transaction';
  $handler->display->display_options['fields']['consumer_name_1']['field'] = 'consumer_name';
  /* Field: IDEAL transaction: Status */
  $handler->display->display_options['fields']['status_1']['id'] = 'status_1';
  $handler->display->display_options['fields']['status_1']['table'] = 'ideal_advanced_transaction';
  $handler->display->display_options['fields']['status_1']['field'] = 'status';

  /* Filter criterion: IDEAL transaction: Transaction ID */
  $handler->display->display_options['filters']['transaction_id']['id'] = 'transaction_id';
  $handler->display->display_options['filters']['transaction_id']['table'] = 'ideal_advanced_transaction';
  $handler->display->display_options['filters']['transaction_id']['field'] = 'transaction_id';
  $handler->display->display_options['filters']['transaction_id']['group'] = 1;
  $handler->display->display_options['filters']['transaction_id']['exposed'] = TRUE;
  $handler->display->display_options['filters']['transaction_id']['expose']['operator_id'] = 'transaction_id_op';
  $handler->display->display_options['filters']['transaction_id']['expose']['label'] = t('Transaction ID');
  $handler->display->display_options['filters']['transaction_id']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['transaction_id']['expose']['operator'] = 'transaction_id_op';
  $handler->display->display_options['filters']['transaction_id']['expose']['identifier'] = 'transaction_id';
  /* Filter criterion: Date: Date (ideal_advanced_transaction) */
  if(module_exists('date_views')) {
    $handler->display->display_options['filters']['date_filter']['id'] = 'date_filter';
    $handler->display->display_options['filters']['date_filter']['table'] = 'ideal_advanced_transaction';
    $handler->display->display_options['filters']['date_filter']['field'] = 'date_filter';
    $handler->display->display_options['filters']['date_filter']['operator'] = 'between';
    $handler->display->display_options['filters']['date_filter']['group'] = 1;
    $handler->display->display_options['filters']['date_filter']['exposed'] = TRUE;
    $handler->display->display_options['filters']['date_filter']['expose']['operator_id'] = 'date_filter_op';
    $handler->display->display_options['filters']['date_filter']['expose']['operator'] = 'date_filter_op';
    $handler->display->display_options['filters']['date_filter']['expose']['identifier'] = 'date_filter';
    $handler->display->display_options['filters']['date_filter']['default_date'] = '-6 month';
    $handler->display->display_options['filters']['date_filter']['default_to_date'] = 'now';
    $handler->display->display_options['filters']['date_filter']['date_fields'] = array(
      'ideal_advanced_transaction.created' => 'ideal_advanced_transaction.created',
      'ideal_advanced_transaction.changed' => 'ideal_advanced_transaction.changed',
    );
  }
  /* Filter criterion: IDEAL transaction: Consumer IBAN */
  $handler->display->display_options['filters']['consumer_iban']['id'] = 'consumer_iban';
  $handler->display->display_options['filters']['consumer_iban']['table'] = 'ideal_advanced_transaction';
  $handler->display->display_options['filters']['consumer_iban']['field'] = 'consumer_iban';
  $handler->display->display_options['filters']['consumer_iban']['operator'] = 'starts';
  $handler->display->display_options['filters']['consumer_iban']['group'] = 1;
  $handler->display->display_options['filters']['consumer_iban']['exposed'] = TRUE;
  $handler->display->display_options['filters']['consumer_iban']['expose']['operator_id'] = 'consumer_iban_op';
  $handler->display->display_options['filters']['consumer_iban']['expose']['label'] = t('Consumer IBAN');
  $handler->display->display_options['filters']['consumer_iban']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['consumer_iban']['expose']['operator'] = 'consumer_iban_op';
  $handler->display->display_options['filters']['consumer_iban']['expose']['identifier'] = 'consumer_iban';
  /* Filter criterion: IDEAL transaction: Amount */
  $handler->display->display_options['filters']['amount']['id'] = 'amount';
  $handler->display->display_options['filters']['amount']['table'] = 'ideal_advanced_transaction';
  $handler->display->display_options['filters']['amount']['field'] = 'amount';
  $handler->display->display_options['filters']['amount']['group'] = 1;
  $handler->display->display_options['filters']['amount']['exposed'] = TRUE;
  $handler->display->display_options['filters']['amount']['expose']['operator_id'] = 'amount_op';
  $handler->display->display_options['filters']['amount']['expose']['label'] = t('Amount');
  $handler->display->display_options['filters']['amount']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['amount']['expose']['operator'] = 'amount_op';
  $handler->display->display_options['filters']['amount']['expose']['identifier'] = 'amount';
  /* Filter criterion: IDEAL transaction: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'ideal_advanced_transaction';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['label'] = t('Status');
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['multiple'] = TRUE;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'ideal_advanced_transactions');
  $handler->display->display_options['path'] = 'admin/config/services/ideal/transactions';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = t('iDEAL Transactions');
  $handler->display->display_options['menu']['description'] = t('Provides a list of iDEAL Transactions');
  $handler->display->display_options['menu']['weight'] = '10';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['weight'] = '0';

  // Add view to list of views to provide.
  $views [$view->name] = $view;

  return $views;
}
