<?php

/**
 * @file
 * nodeownership.views_default.inc
 */

/**
 * Implementation of hook_views_default_views().
 */
function nodeownership_views_default_views() {
  $export = array();

  $view = new view;
  $view->name = 'nodeownership_claims';
  $view->description = 'List of node ownership claims';
  $view->tag = 'default';
  $view->base_table = 'nodeownership';
  $view->human_name = 'Node Ownership claims';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Node ownership claims';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer nodeownership';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'uid' => 'uid',
    'nid' => 'nid',
    'contact' => 'contact',
    'notes' => 'notes',
    'timestamp' => 'timestamp',
    'status' => 'status',
    'accept_link' => 'accept_link',
    'decline_link' => 'decline_link',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'uid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
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
    'contact' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'notes' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'timestamp' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'accept_link' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'decline_link' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 1;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* Field: Bulk operations: Node ownership */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'nodeownership';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['external'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['html'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['hide_empty'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['empty_zero'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['operations'] = array(
    'action::nodeownership_approve_claim_action' => array(
      'selected' => 1,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::nodeownership_decline_claim_action' => array(
      'selected' => 1,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 1,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
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
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['display_result'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['merge_single_action'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['force_single'] = 0;
  /* Field: Node ownership: User uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'nodeownership';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['uid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['uid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['uid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['uid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['uid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['uid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['uid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['uid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['uid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['uid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['uid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['uid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['uid']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['uid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['uid']['empty'] = 'Anonymous';
  $handler->display->display_options['fields']['uid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['uid']['empty_zero'] = 1;
  $handler->display->display_options['fields']['uid']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['uid']['link_to_user'] = 1;
  /* Field: Node ownership: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'nodeownership';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['nid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nid']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['nid']['link_to_node'] = 0;
  /* Field: Node ownership: Contact */
  $handler->display->display_options['fields']['contact']['id'] = 'contact';
  $handler->display->display_options['fields']['contact']['table'] = 'nodeownership';
  $handler->display->display_options['fields']['contact']['field'] = 'contact';
  $handler->display->display_options['fields']['contact']['label'] = 'Contact Information';
  $handler->display->display_options['fields']['contact']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['contact']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['contact']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['contact']['alter']['external'] = 0;
  $handler->display->display_options['fields']['contact']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['contact']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['contact']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['contact']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['contact']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['contact']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['contact']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['contact']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['contact']['alter']['html'] = 0;
  $handler->display->display_options['fields']['contact']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['contact']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['contact']['hide_empty'] = 0;
  $handler->display->display_options['fields']['contact']['empty_zero'] = 0;
  $handler->display->display_options['fields']['contact']['hide_alter_empty'] = 1;
  /* Field: Node ownership: Notes */
  $handler->display->display_options['fields']['notes']['id'] = 'notes';
  $handler->display->display_options['fields']['notes']['table'] = 'nodeownership';
  $handler->display->display_options['fields']['notes']['field'] = 'notes';
  $handler->display->display_options['fields']['notes']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['notes']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['notes']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['notes']['alter']['external'] = 0;
  $handler->display->display_options['fields']['notes']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['notes']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['notes']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['notes']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['notes']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['notes']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['notes']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['notes']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['notes']['alter']['html'] = 0;
  $handler->display->display_options['fields']['notes']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['notes']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['notes']['hide_empty'] = 0;
  $handler->display->display_options['fields']['notes']['empty_zero'] = 0;
  $handler->display->display_options['fields']['notes']['hide_alter_empty'] = 1;
  /* Field: Node ownership: Timestamp */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'nodeownership';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['label'] = 'Claim timestamp';
  $handler->display->display_options['fields']['timestamp']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['external'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['timestamp']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['timestamp']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['html'] = 0;
  $handler->display->display_options['fields']['timestamp']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['timestamp']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['timestamp']['hide_empty'] = 0;
  $handler->display->display_options['fields']['timestamp']['empty_zero'] = 0;
  $handler->display->display_options['fields']['timestamp']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'privatemsg_current_day';
  /* Field: Node ownership: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'nodeownership';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['status']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['status']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['status']['alter']['external'] = 0;
  $handler->display->display_options['fields']['status']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['status']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['status']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['status']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['status']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['status']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['status']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['status']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['status']['alter']['html'] = 0;
  $handler->display->display_options['fields']['status']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['status']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['status']['hide_empty'] = 0;
  $handler->display->display_options['fields']['status']['empty_zero'] = 0;
  $handler->display->display_options['fields']['status']['hide_alter_empty'] = 1;
  /* Field: Node ownership: Approve link */
  $handler->display->display_options['fields']['approve_link']['id'] = 'approve_link';
  $handler->display->display_options['fields']['approve_link']['table'] = 'nodeownership';
  $handler->display->display_options['fields']['approve_link']['field'] = 'approve_link';
  $handler->display->display_options['fields']['approve_link']['label'] = 'Approve';
  $handler->display->display_options['fields']['approve_link']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['approve_link']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['approve_link']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['approve_link']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['approve_link']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['approve_link']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['approve_link']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['approve_link']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['approve_link']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['approve_link']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['approve_link']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['approve_link']['alter']['html'] = 0;
  $handler->display->display_options['fields']['approve_link']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['approve_link']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['approve_link']['hide_empty'] = 0;
  $handler->display->display_options['fields']['approve_link']['empty_zero'] = 0;
  $handler->display->display_options['fields']['approve_link']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['approve_link']['text'] = 'Approve';
  /* Field: Node ownership: Decline link */
  $handler->display->display_options['fields']['decline_link']['id'] = 'decline_link';
  $handler->display->display_options['fields']['decline_link']['table'] = 'nodeownership';
  $handler->display->display_options['fields']['decline_link']['field'] = 'decline_link';
  $handler->display->display_options['fields']['decline_link']['label'] = 'Decline';
  $handler->display->display_options['fields']['decline_link']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['decline_link']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['decline_link']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['decline_link']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['decline_link']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['decline_link']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['decline_link']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['decline_link']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['decline_link']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['decline_link']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['decline_link']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['decline_link']['alter']['html'] = 0;
  $handler->display->display_options['fields']['decline_link']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['decline_link']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['decline_link']['hide_empty'] = 0;
  $handler->display->display_options['fields']['decline_link']['empty_zero'] = 0;
  $handler->display->display_options['fields']['decline_link']['hide_alter_empty'] = 1;
  /* Filter criterion: Node ownership: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'nodeownership';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '0';
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['multiple'] = FALSE;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/content/claims';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Node Ownership Claims';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;


  $export[$view->name] = $view;

  return $export;
}

