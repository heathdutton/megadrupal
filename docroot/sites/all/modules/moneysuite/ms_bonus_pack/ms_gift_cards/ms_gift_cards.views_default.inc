<?php

/**
 * Implements hook_views_default_views().
 */
function ms_gift_cards_views_default_views() {
  $views['gift_cards'] = ms_gift_cards_exported_default_view();

  return $views;
}

/**
 * Exported Default View: Orders.
 */
function ms_gift_cards_exported_default_view() {
  $view = new view();
  $view->name = 'gift_cards';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'ms_gift_cards';
  $view->human_name = 'Gift Cards';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Gift Cards';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer moneysuite settings';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Relationship: Gift Card: Oid */
  $handler->display->display_options['relationships']['oid']['id'] = 'oid';
  $handler->display->display_options['relationships']['oid']['table'] = 'ms_gift_cards';
  $handler->display->display_options['relationships']['oid']['field'] = 'oid';
  /* Relationship: Gift Card: Redeemed_user */
  $handler->display->display_options['relationships']['redeemed_user']['id'] = 'redeemed_user';
  $handler->display->display_options['relationships']['redeemed_user']['table'] = 'ms_gift_cards';
  $handler->display->display_options['relationships']['redeemed_user']['field'] = 'redeemed_user';
  $handler->display->display_options['relationships']['redeemed_user']['label'] = 'Redeemer';
  /* Relationship: Gift Card: Activated_user */
  $handler->display->display_options['relationships']['activated_user']['id'] = 'activated_user';
  $handler->display->display_options['relationships']['activated_user']['table'] = 'ms_gift_cards';
  $handler->display->display_options['relationships']['activated_user']['field'] = 'activated_user';
  $handler->display->display_options['relationships']['activated_user']['label'] = 'Activator';
  /* Field: Bulk operations: Gift Card */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'ms_gift_cards';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
  $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
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
  );
  /* Field: Gift Card: Gift card ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'ms_gift_cards';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['label'] = '';
  $handler->display->display_options['fields']['id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['id']['element_label_colon'] = FALSE;
  /* Field: Gift Card: Admin_note */
  $handler->display->display_options['fields']['admin_note']['id'] = 'admin_note';
  $handler->display->display_options['fields']['admin_note']['table'] = 'ms_gift_cards';
  $handler->display->display_options['fields']['admin_note']['field'] = 'admin_note';
  $handler->display->display_options['fields']['admin_note']['label'] = 'Note';
  /* Field: Gift Card: Code */
  $handler->display->display_options['fields']['code']['id'] = 'code';
  $handler->display->display_options['fields']['code']['table'] = 'ms_gift_cards';
  $handler->display->display_options['fields']['code']['field'] = 'code';
  /* Field: Gift Card: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'ms_gift_cards';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  /* Field: Gift Card: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'ms_gift_cards';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: Gift Card: Activated */
  $handler->display->display_options['fields']['activated']['id'] = 'activated';
  $handler->display->display_options['fields']['activated']['table'] = 'ms_gift_cards';
  $handler->display->display_options['fields']['activated']['field'] = 'activated';
  $handler->display->display_options['fields']['activated']['date_format'] = 'short';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'users';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['relationship'] = 'activated_user';
  $handler->display->display_options['fields']['name_1']['label'] = 'Activated by';
  $handler->display->display_options['fields']['name_1']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['name_1']['empty_zero'] = TRUE;
  /* Field: Gift Card: Redeemed */
  $handler->display->display_options['fields']['redeemed']['id'] = 'redeemed';
  $handler->display->display_options['fields']['redeemed']['table'] = 'ms_gift_cards';
  $handler->display->display_options['fields']['redeemed']['field'] = 'redeemed';
  $handler->display->display_options['fields']['redeemed']['date_format'] = 'short';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'redeemed_user';
  $handler->display->display_options['fields']['name']['label'] = 'Redeemed by';
  $handler->display->display_options['fields']['name']['empty'] = '-';
  $handler->display->display_options['fields']['name']['empty_zero'] = TRUE;
  /* Field: Gift Card: Amount */
  $handler->display->display_options['fields']['amount']['id'] = 'amount';
  $handler->display->display_options['fields']['amount']['table'] = 'ms_gift_cards';
  $handler->display->display_options['fields']['amount']['field'] = 'amount';
  $handler->display->display_options['fields']['amount']['set_precision'] = TRUE;
  $handler->display->display_options['fields']['amount']['precision'] = '2';
  /* Field: Order: Order Number */
  $handler->display->display_options['fields']['order_number']['id'] = 'order_number';
  $handler->display->display_options['fields']['order_number']['table'] = 'views_entity_ms_order';
  $handler->display->display_options['fields']['order_number']['field'] = 'order_number';
  $handler->display->display_options['fields']['order_number']['relationship'] = 'oid';
  $handler->display->display_options['fields']['order_number']['label'] = 'Order';
  $handler->display->display_options['fields']['order_number']['link_to_entity'] = 1;
  /* Field: Gift Card: Product name */
  $handler->display->display_options['fields']['product_name']['id'] = 'product_name';
  $handler->display->display_options['fields']['product_name']['table'] = 'views_entity_ms_gift_card';
  $handler->display->display_options['fields']['product_name']['field'] = 'product_name';
  $handler->display->display_options['fields']['product_name']['label'] = 'Product';
  $handler->display->display_options['fields']['product_name']['link_to_entity'] = 0;
  /* Field: Gift Card: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'ms_gift_cards';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  /* Field: Gift Card: Scope */
  $handler->display->display_options['fields']['scope']['id'] = 'scope';
  $handler->display->display_options['fields']['scope']['table'] = 'ms_gift_cards';
  $handler->display->display_options['fields']['scope']['field'] = 'scope';
  /* Field: Gift Card: Value */
  $handler->display->display_options['fields']['value']['id'] = 'value';
  $handler->display->display_options['fields']['value']['table'] = 'ms_gift_cards';
  $handler->display->display_options['fields']['value']['field'] = 'value';
  $handler->display->display_options['fields']['value']['precision'] = '0';
  /* Field: Edit link */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['ui_name'] = 'Edit link';
  $handler->display->display_options['fields']['nothing_1']['label'] = '';
  $handler->display->display_options['fields']['nothing_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'Edit';
  $handler->display->display_options['fields']['nothing_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['path'] = 'admin/moneysuite/ms_gift_cards/edit/[id]';
  $handler->display->display_options['fields']['nothing_1']['element_label_colon'] = FALSE;
  /* Field: Delete link */
  $handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
  $handler->display->display_options['fields']['nothing_2']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_2']['ui_name'] = 'Delete link';
  $handler->display->display_options['fields']['nothing_2']['label'] = '';
  $handler->display->display_options['fields']['nothing_2']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['text'] = 'Delete';
  $handler->display->display_options['fields']['nothing_2']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['path'] = 'admin/moneysuite/ms_gift_cards/delete/[id]';
  $handler->display->display_options['fields']['nothing_2']['element_label_colon'] = FALSE;
  /* Field: Actions */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['ui_name'] = 'Actions';
  $handler->display->display_options['fields']['nothing']['label'] = 'Actions';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[nothing_1] | [nothing_2]';
  /* Filter criterion: Gift Card: Admin_note */
  $handler->display->display_options['filters']['admin_note']['id'] = 'admin_note';
  $handler->display->display_options['filters']['admin_note']['table'] = 'ms_gift_cards';
  $handler->display->display_options['filters']['admin_note']['field'] = 'admin_note';
  $handler->display->display_options['filters']['admin_note']['operator'] = 'contains';
  $handler->display->display_options['filters']['admin_note']['group'] = 1;
  $handler->display->display_options['filters']['admin_note']['exposed'] = TRUE;
  $handler->display->display_options['filters']['admin_note']['expose']['operator_id'] = 'admin_note_op';
  $handler->display->display_options['filters']['admin_note']['expose']['label'] = 'Note';
  $handler->display->display_options['filters']['admin_note']['expose']['operator'] = 'admin_note_op';
  $handler->display->display_options['filters']['admin_note']['expose']['identifier'] = 'admin_note';
  $handler->display->display_options['filters']['admin_note']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
  );
  /* Filter criterion: Gift Card: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'ms_gift_cards';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
  );
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'activated_user';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['group'] = 1;
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Activated by';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
  );
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid_1']['id'] = 'uid_1';
  $handler->display->display_options['filters']['uid_1']['table'] = 'users';
  $handler->display->display_options['filters']['uid_1']['field'] = 'uid';
  $handler->display->display_options['filters']['uid_1']['relationship'] = 'redeemed_user';
  $handler->display->display_options['filters']['uid_1']['value'] = '';
  $handler->display->display_options['filters']['uid_1']['group'] = 1;
  $handler->display->display_options['filters']['uid_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid_1']['expose']['operator_id'] = 'uid_1_op';
  $handler->display->display_options['filters']['uid_1']['expose']['label'] = 'Redeemed by';
  $handler->display->display_options['filters']['uid_1']['expose']['operator'] = 'uid_1_op';
  $handler->display->display_options['filters']['uid_1']['expose']['identifier'] = 'uid_1';
  $handler->display->display_options['filters']['uid_1']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/moneysuite/ms_gift_cards/overview';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Gift Cards';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['weight'] = '0';

  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['filename'] = '%view-%timestamp-full.csv';
  $handler->display->display_options['style_options']['parent_sort'] = 1;
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 0;
  $handler->display->display_options['style_options']['replace_newlines'] = 0;
  $handler->display->display_options['style_options']['header'] = 1;
  $handler->display->display_options['style_options']['keep_html'] = 0;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Gift Card: Gift card ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'ms_gift_cards';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['label'] = '';
  $handler->display->display_options['fields']['id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['id']['element_label_colon'] = FALSE;
  /* Field: Gift Card: Admin_note */
  $handler->display->display_options['fields']['admin_note']['id'] = 'admin_note';
  $handler->display->display_options['fields']['admin_note']['table'] = 'ms_gift_cards';
  $handler->display->display_options['fields']['admin_note']['field'] = 'admin_note';
  $handler->display->display_options['fields']['admin_note']['label'] = 'Note';
  /* Field: Gift Card: Code */
  $handler->display->display_options['fields']['code']['id'] = 'code';
  $handler->display->display_options['fields']['code']['table'] = 'ms_gift_cards';
  $handler->display->display_options['fields']['code']['field'] = 'code';
  /* Field: Gift Card: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'ms_gift_cards';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  /* Field: Gift Card: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'ms_gift_cards';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: Gift Card: Activated */
  $handler->display->display_options['fields']['activated']['id'] = 'activated';
  $handler->display->display_options['fields']['activated']['table'] = 'ms_gift_cards';
  $handler->display->display_options['fields']['activated']['field'] = 'activated';
  $handler->display->display_options['fields']['activated']['date_format'] = 'short';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'users';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['relationship'] = 'activated_user';
  $handler->display->display_options['fields']['name_1']['label'] = 'Activated by';
  $handler->display->display_options['fields']['name_1']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['name_1']['empty_zero'] = TRUE;
  /* Field: Gift Card: Redeemed */
  $handler->display->display_options['fields']['redeemed']['id'] = 'redeemed';
  $handler->display->display_options['fields']['redeemed']['table'] = 'ms_gift_cards';
  $handler->display->display_options['fields']['redeemed']['field'] = 'redeemed';
  $handler->display->display_options['fields']['redeemed']['date_format'] = 'short';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'redeemed_user';
  $handler->display->display_options['fields']['name']['label'] = 'Redeemed by';
  $handler->display->display_options['fields']['name']['empty'] = '-';
  $handler->display->display_options['fields']['name']['empty_zero'] = TRUE;
  /* Field: Gift Card: Amount */
  $handler->display->display_options['fields']['amount']['id'] = 'amount';
  $handler->display->display_options['fields']['amount']['table'] = 'ms_gift_cards';
  $handler->display->display_options['fields']['amount']['field'] = 'amount';
  $handler->display->display_options['fields']['amount']['precision'] = '0';
  /* Field: Order: Order Number */
  $handler->display->display_options['fields']['order_number']['id'] = 'order_number';
  $handler->display->display_options['fields']['order_number']['table'] = 'views_entity_ms_order';
  $handler->display->display_options['fields']['order_number']['field'] = 'order_number';
  $handler->display->display_options['fields']['order_number']['relationship'] = 'oid';
  $handler->display->display_options['fields']['order_number']['label'] = 'Order';
  $handler->display->display_options['fields']['order_number']['link_to_entity'] = 1;
  /* Field: Gift Card: Product name */
  $handler->display->display_options['fields']['product_name']['id'] = 'product_name';
  $handler->display->display_options['fields']['product_name']['table'] = 'views_entity_ms_gift_card';
  $handler->display->display_options['fields']['product_name']['field'] = 'product_name';
  $handler->display->display_options['fields']['product_name']['label'] = 'Product';
  $handler->display->display_options['fields']['product_name']['link_to_entity'] = 0;
  /* Field: Gift Card: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'ms_gift_cards';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  /* Field: Gift Card: Scope */
  $handler->display->display_options['fields']['scope']['id'] = 'scope';
  $handler->display->display_options['fields']['scope']['table'] = 'ms_gift_cards';
  $handler->display->display_options['fields']['scope']['field'] = 'scope';
  /* Field: Gift Card: Value */
  $handler->display->display_options['fields']['value']['id'] = 'value';
  $handler->display->display_options['fields']['value']['table'] = 'ms_gift_cards';
  $handler->display->display_options['fields']['value']['field'] = 'value';
  $handler->display->display_options['fields']['value']['precision'] = '0';
  $handler->display->display_options['path'] = 'admin/moneysuite/ms_gift_cards/export';
  $handler->display->display_options['displays'] = array(
    'page' => 'page',
    'default' => 0,
  );
  $translatables['gift_cards'] = array(
    t('Master'),
    t('Gift Cards'),
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
    t('Order'),
    t('Redeemer'),
    t('Activator'),
    t('- Choose an operation -'),
    t('.'),
    t(','),
    t('Note'),
    t('Code'),
    t('Status'),
    t('Created'),
    t('Activated'),
    t('Activated by'),
    t('Redeemed'),
    t('Redeemed by'),
    t('-'),
    t('Amount'),
    t('Product'),
    t('Type'),
    t('Scope'),
    t('Value'),
    t('Edit'),
    t('Delete'),
    t('Actions'),
    t('[nothing_1] | [nothing_2]'),
    t('Page'),
    t('Data export'),
  );

  return $view;
}
