<?php

/**
 * @file
 * Contains Views module hooks.
 */

/**
 * Implementation of hook_views_handlers().
 */
function paypal_roles_views_handlers() {
  $path = drupal_get_path('module', 'paypal_roles');

  return array(
    'handlers' => array(
      'views_handler_field_membership_amount' => array(
        'parent' => 'views_handler_field',
        'path' => $path . '/includes/views/handlers',
      ),
      'views_handler_field_membership_button' => array(
          'parent' => 'views_handler_field',
          'path' => $path . '/includes/views/handlers',
      ),
      'views_handler_field_membership_cycles' => array(
        'parent' => 'views_handler_field',
        'path' => $path . '/includes/views/handlers',
      ),
      'views_handler_field_membership_description' => array(
          'parent' => 'views_handler_field',
          'path' => $path . '/includes/views/handlers',
      ),
      'views_handler_field_membership_length_type' => array(
        'parent' => 'views_handler_field',
        'path' => $path . '/includes/views/handlers',
      ),
      'views_handler_field_membership_link_cancel' => array(
        'parent' => 'views_handler_field',
        'path' => $path . '/includes/views/handlers',
      ),
      'views_handler_field_membership_link_edit' => array(
        'parent' => 'views_handler_field',
        'path' => $path . '/includes/views/handlers',
      ),
      'views_handler_field_payment_expire' => array(
        'parent' => 'views_handler_field',
        'path' => $path . '/includes/views/handlers',
      ),
      'views_handler_field_payment_link_status' => array(
        'parent' => 'views_handler_field',
        'path' => $path . '/includes/views/handlers',
      ),
      'views_handler_field_payment_amount' => array(
        'parent' => 'views_handler_field',
        'path' => $path . '/includes/views/handlers',
      ),
    ),
  );
}

/**
 * Implements hook_views_data().
 */
function paypal_roles_views_data() {
  $data = array();

  // paypal_roles_memberships

  $data['paypal_roles_memberships']['table']['group'] = t('PayPal Roles Memberships');

  $data['paypal_roles_memberships']['table']['base'] = array(
    'field' => 'machine_name',
    'title' => t('PayPal Roles Memberships'),
    'help' => t('PayPal Roles Memberships created on your site.'),
  );

  $data['paypal_roles_memberships']['table']['join']['paypal_roles_payments'] = array(
    'left_field' => 'item_id',
    'field' => 'machine_name',
  );

  $data['paypal_roles_memberships']['role'] = array(
    'title' => t('Role ID'),
    'help' => t('Role ID.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'label' => t('Role ID'),
      'base' => 'role',
      'base field' => 'rid',
      'field' => 'role',
      'label' => t('Relationship to Role via Membership: Role'),
    ),
  );

  $data['paypal_roles_memberships']['machine_name'] = array(
    'title' => t('Machine name'),
    'help' => t('Machine name of Membership.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['paypal_roles_memberships']['name'] = array(
    'title' => t('Name'),
    'help' => t('Name of Membership.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['paypal_roles_memberships']['amount'] = array(
    'title' => t('Amount'),
    'help' => t('Membership Amount.'),
    'field' => array(
      'handler' => 'views_handler_field_membership_amount',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['paypal_roles_memberships']['button_type'] = array(
      'title' => t('Button'),
      'help' => t('Paypal purchase button'),
      'field' => array(
          'handler' => 'views_handler_field_membership_button',
          'format' => 'full_html',
      ),
  );

  $data['paypal_roles_memberships']['currency'] = array(
    'title' => t('Currency'),
    'help' => t('Currency of Amount.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['paypal_roles_memberships']['length'] = array(
    'title' => t('Length'),
    'help' => t('Membership Length.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['paypal_roles_memberships']['length_type'] = array(
    'title' => t('Length type'),
    'help' => t('Length type.'),
    'field' => array(
      'handler' => 'views_handler_field_membership_length_type',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['paypal_roles_memberships']['cycles'] = array(
    'title' => t('Cycles'),
    'help' => t('Number of Cycles.'),
    'field' => array(
      'handler' => 'views_handler_field_membership_cycles',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['paypal_roles_memberships']['description'] = array(
      'field' => array(
          'title' => t('Description'),
          'help' => t('Membership Description'),
          'handler' => 'views_handler_field_markup',
          'format' => filter_default_format(),
      ),
  );

  $data['paypal_roles_memberships']['status'] = array(
    'title' => t('Status'),
    'help' => t('Membership Status.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Status'),
      'type' => 'yes-no',
      'use equal' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['paypal_roles_memberships']['edit_node'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to edit the membership.'),
      'handler' => 'views_handler_field_membership_link_edit',
    ),
  );

  $data['paypal_roles_memberships']['cancel_node'] = array(
    'field' => array(
      'title' => t('Cancel link'),
      'help' => t('Provide a simple link to cancel the membership.'),
      'handler' => 'views_handler_field_membership_link_cancel',
    ),
  );

  // paypal_roles_payments

  $data['paypal_roles_payments']['table']['group'] = t('PayPal Roles Payments');

  $data['paypal_roles_payments']['table']['base'] = array(
    'field' => 'txn_id',
    'title' => t('PayPal Roles Payments'),
    'help' => t('PayPal Transaction ID.'),
  );

  $data['paypal_roles_payments']['txn_id'] = array(
    'title' => t('Transaction ID.'),
    'help' => t('PayPal Transaction ID.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['paypal_roles_payments']['item_name'] = array(
    'title' => t('Membership name.'),
    'help' => t('Name of the membership by payment.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['paypal_roles_payments']['uid'] = array(
    'title' => t('UID'),
    'help' => t('User ID.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'label' => t('User ID'),
      'base' => 'users',
      'base field' => 'uid',
      'field' => 'uid',
      'label' => t('Relationship to User via Payment: UID'),
    ),
  );

  $data['paypal_roles_payments']['created'] = array(
    'title' => t('Created'),
    'help' => t('The payment was created at.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['paypal_roles_payments']['amount'] = array(
    'title' => t('Amount.'),
    'help' => t('Amount of payment.'),
    'field' => array(
      'handler' => 'views_handler_field_payment_amount',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['paypal_roles_payments']['currency'] = array(
    'title' => t('Currency.'),
    'help' => t('Currency of payment.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['paypal_roles_payments']['expire'] = array(
    'title' => t('Expire date'),
    'help' => t('Expire date for membership at this payment.'),
    'field' => array(
      'handler' => 'views_handler_field_payment_expire',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['paypal_roles_payments']['cancelled'] = array(
    'title' => t('Change status'),
    'help' => t('Provide a simple link to change status of a payment.'),
    'field' => array(
      'handler' => 'views_handler_field_payment_link_status',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_boolean_operator',
    ),
  );



  // paypal_roles_custom_payments

  $data['paypal_roles_custom_payments']['table']['group'] = t('PayPal Roles Custom Payments');

  $data['paypal_roles_custom_payments']['table']['base'] = array(
    'field' => 'pid',
    'title' => t('PayPal Roles Custom Payments'),
    'help' => t('Custom Payment ID.'),
  );

  $data['paypal_roles_custom_payments']['pid'] = array(
    'title' => t('Custom Payment ID.'),
    'help' => t('Custom Payment ID.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['paypal_roles_custom_payments']['uid'] = array(
    'title' => t('UID'),
    'help' => t('User ID.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'label' => t('User ID'),
      'base' => 'users',
      'base field' => 'uid',
      'field' => 'uid',
      'label' => t('Relationship to User via Custom Payment: UID'),
    ),
  );

  $data['paypal_roles_custom_payments']['date'] = array(
    'title' => t('Date'),
    'help' => t('Expiration date is calculated from this date.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['paypal_roles_custom_payments']['expire'] = array(
    'title' => t('Expire date'),
    'help' => t('Expire date at this payment.'),
    'field' => array(
      'handler' => 'views_handler_field_custom_payment_expire',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['paypal_roles_custom_payments']['role'] = array(
    'title' => t('Role ID'),
    'help' => t('Role ID.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'label' => t('Role ID'),
      'base' => 'role',
      'base field' => 'rid',
      'field' => 'role',
      'label' => t('Relationship to Role via Custom Payment: Role'),
    ),
  );

  $data['paypal_roles_custom_payments']['amount'] = array(
    'title' => t('Amount'),
    'help' => t('Payed Amount at Custom Payment.'),
    'field' => array(
      'handler' => 'views_handler_field_custom_payment_amount',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['paypal_roles_custom_payments']['currency'] = array(
    'title' => t('Currency'),
    'help' => t('Currency of Amount.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['paypal_roles_custom_payments']['length'] = array(
    'title' => t('Length'),
    'help' => t('Length of custom subscription.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['paypal_roles_custom_payments']['length_type'] = array(
    'title' => t('Length type'),
    'help' => t('Length type.'),
    'field' => array(
      'handler' => 'views_handler_field_custom_payment_length_type',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['paypal_roles_custom_payments']['edit_node'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to edit the custom payment.'),
      'handler' => 'views_handler_field_custom_payment_link_edit',
    ),
  );

  $data['paypal_roles_custom_payments']['cancel_node'] = array(
    'field' => array(
      'title' => t('Cancel link'),
      'help' => t('Provide a simple link to cancel the custom payment.'),
      'handler' => 'views_handler_field_custom_payment_link_cancel',
    ),
  );

  return $data;
}

/**
 * Implements hook_views_data().
 */
function paypal_roles_views_data_alter(&$data) {
  $data['user']['table']['join']['paypal_roles_payments'] = array(
    'left_field' => 'uid',
    'field' => 'uid',
  );

  $data['role']['table']['join']['paypal_roles_memberships'] = array(
    'left_field' => 'role',
    'field' => 'rid',
  );

  $data['user']['table']['join']['paypal_roles_custom_payments'] = array(
    'left_field' => 'uid',
    'field' => 'uid',
  );

  $data['role']['table']['join']['paypal_roles_custom_payments'] = array(
    'left_field' => 'role',
    'field' => 'rid',
  );

  $data['role']['table']['group'] = t('User Role');

  $data['role']['name'] = array(
    'title' => t('Name'),
    'help' => t('Name of Membership.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
}

/**
 * Implements hook_views_default_views().
 */
function paypal_roles_views_default_views() {
  $view = new view();
  $view->name = 'memberships';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'paypal_roles_memberships';
  $view->human_name = 'Memberships';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Memberships';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'better_exposed_filters';
  $handler->display->display_options['exposed_form']['options']['bef'] = array(
    'general' => array(
      'allow_secondary' => 0,
      'secondary_label' => 'Advanced options',
    ),
    'name' => array(
      'bef_format' => 'default',
      'more_options' => array(
        'is_secondary' => 0,
        'any_label' => '',
        'bef_filter_description' => '',
        'tokens' => array(
          'available' => array(
            0 => 'global_types',
          ),
        ),
        'rewrite' => array(
          'filter_rewrite_values' => '',
        ),
      ),
    ),
    'status' => array(
      'bef_format' => 'default',
      'more_options' => array(
        'bef_select_all_none' => FALSE,
        'bef_collapsible' => 0,
        'is_secondary' => 0,
        'any_label' => '',
        'bef_filter_description' => '',
        'tokens' => array(
          'available' => array(
            0 => 'global_types',
          ),
        ),
        'rewrite' => array(
          'filter_rewrite_values' => '',
        ),
      ),
    ),
  );
  $handler->display->display_options['exposed_form']['options']['input_required'] = 0;
  $handler->display->display_options['exposed_form']['options']['text_input_required_format'] = 'filtered_html';
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'counter' => 'counter',
    'name' => 'name',
    'amount' => 'amount',
    'currency' => 'amount',
    'length' => 'length',
    'length_type' => 'length',
    'cycles' => 'cycles',
    'name_1' => 'name_1',
    'status' => 'status',
    'edit_node' => 'edit_node',
    'cancel_node' => 'edit_node',
  );
  $handler->display->display_options['style_options']['default'] = 'name';
  $handler->display->display_options['style_options']['info'] = array(
    'counter' => array(
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-left',
      'separator' => '',
      'empty_column' => 0,
    ),
    'amount' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-right',
      'separator' => '&nbsp;',
      'empty_column' => 0,
    ),
    'currency' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'length' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '&nbsp;',
      'empty_column' => 0,
    ),
    'length_type' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cycles' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name_1' => array(
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
    'edit_node' => array(
      'align' => 'views-align-center',
      'separator' => '&nbsp;|&nbsp;',
      'empty_column' => 0,
    ),
    'cancel_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  /* No results behavior: Global: Unfiltered text */
  $handler->display->display_options['empty']['area_text_custom']['id'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['table'] = 'views';
  $handler->display->display_options['empty']['area_text_custom']['field'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['empty'] = TRUE;
  $handler->display->display_options['empty']['area_text_custom']['content'] = 'No memberships...';
  /* Field: Global: View result counter */
  $handler->display->display_options['fields']['counter']['id'] = 'counter';
  $handler->display->display_options['fields']['counter']['table'] = 'views';
  $handler->display->display_options['fields']['counter']['field'] = 'counter';
  $handler->display->display_options['fields']['counter']['label'] = '';
  $handler->display->display_options['fields']['counter']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['counter']['counter_start'] = '1';
  /* Field: PayPal Roles Memberships: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'paypal_roles_memberships';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  /* Field: PayPal Roles Memberships: Amount */
  $handler->display->display_options['fields']['amount']['id'] = 'amount';
  $handler->display->display_options['fields']['amount']['table'] = 'paypal_roles_memberships';
  $handler->display->display_options['fields']['amount']['field'] = 'amount';
  $handler->display->display_options['fields']['amount']['set_precision'] = TRUE;
  $handler->display->display_options['fields']['amount']['precision'] = '2';
  $handler->display->display_options['fields']['amount']['separator'] = ' ';
  $handler->display->display_options['fields']['amount']['format_plural_singular'] = '2';
  /* Field: PayPal Roles Memberships: Currency */
  $handler->display->display_options['fields']['currency']['id'] = 'currency';
  $handler->display->display_options['fields']['currency']['table'] = 'paypal_roles_memberships';
  $handler->display->display_options['fields']['currency']['field'] = 'currency';
  /* Field: PayPal Roles Memberships: Length */
  $handler->display->display_options['fields']['length']['id'] = 'length';
  $handler->display->display_options['fields']['length']['table'] = 'paypal_roles_memberships';
  $handler->display->display_options['fields']['length']['field'] = 'length';
  $handler->display->display_options['fields']['length']['label'] = 'Period';
  $handler->display->display_options['fields']['length']['separator'] = '';
  /* Field: PayPal Roles Memberships: Length type */
  $handler->display->display_options['fields']['length_type']['id'] = 'length_type';
  $handler->display->display_options['fields']['length_type']['table'] = 'paypal_roles_memberships';
  $handler->display->display_options['fields']['length_type']['field'] = 'length_type';
  $handler->display->display_options['fields']['length_type']['label'] = '';
  $handler->display->display_options['fields']['length_type']['element_label_colon'] = FALSE;
  /* Field: PayPal Roles Memberships: Cycles */
  $handler->display->display_options['fields']['cycles']['id'] = 'cycles';
  $handler->display->display_options['fields']['cycles']['table'] = 'paypal_roles_memberships';
  $handler->display->display_options['fields']['cycles']['field'] = 'cycles';
  $handler->display->display_options['fields']['cycles']['separator'] = '';
  /* Field: User Role: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'role';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['label'] = 'Grant access to';
  /* Field: PayPal Roles Memberships: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'paypal_roles_memberships';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['type'] = 'custom';
  $handler->display->display_options['fields']['status']['type_custom_true'] = 'Active';
  $handler->display->display_options['fields']['status']['type_custom_false'] = 'Inactive';
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Field: PayPal Roles Memberships: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'paypal_roles_memberships';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Options';
  $handler->display->display_options['fields']['edit_node']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['edit_node']['text'] = 'Edit';
  /* Field: PayPal Roles Memberships: Cancel link */
  $handler->display->display_options['fields']['cancel_node']['id'] = 'cancel_node';
  $handler->display->display_options['fields']['cancel_node']['table'] = 'paypal_roles_memberships';
  $handler->display->display_options['fields']['cancel_node']['field'] = 'cancel_node';
  $handler->display->display_options['fields']['cancel_node']['label'] = '';
  $handler->display->display_options['fields']['cancel_node']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['cancel_node']['text'] = 'Delete';
  /* Sort criterion: PayPal Roles Memberships: Name */
  $handler->display->display_options['sorts']['name']['id'] = 'name';
  $handler->display->display_options['sorts']['name']['table'] = 'paypal_roles_memberships';
  $handler->display->display_options['sorts']['name']['field'] = 'name';
  /* Filter criterion: PayPal Roles Memberships: Name */
  $handler->display->display_options['filters']['name']['id'] = 'name';
  $handler->display->display_options['filters']['name']['table'] = 'paypal_roles_memberships';
  $handler->display->display_options['filters']['name']['field'] = 'name';
  $handler->display->display_options['filters']['name']['operator'] = 'contains';
  $handler->display->display_options['filters']['name']['group'] = 1;
  $handler->display->display_options['filters']['name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name']['expose']['operator_id'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['label'] = 'Name';
  $handler->display->display_options['filters']['name']['expose']['operator'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['identifier'] = 'name';
  $handler->display->display_options['filters']['name']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: User Role: Name */
  $handler->display->display_options['filters']['name_1']['id'] = 'name_1';
  $handler->display->display_options['filters']['name_1']['table'] = 'role';
  $handler->display->display_options['filters']['name_1']['field'] = 'name';
  $handler->display->display_options['filters']['name_1']['operator'] = 'contains';
  $handler->display->display_options['filters']['name_1']['group'] = 1;
  $handler->display->display_options['filters']['name_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name_1']['expose']['operator_id'] = 'name_1_op';
  $handler->display->display_options['filters']['name_1']['expose']['label'] = 'Grant access to';
  $handler->display->display_options['filters']['name_1']['expose']['operator'] = 'name_1_op';
  $handler->display->display_options['filters']['name_1']['expose']['identifier'] = 'name_1';
  $handler->display->display_options['filters']['name_1']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: PayPal Roles Memberships: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'paypal_roles_memberships';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 'All';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');

  $views[$view->name] = $view;

  $view = new view();
  $view->name = 'payments';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'paypal_roles_payments';
  $view->human_name = 'Payments';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Payments';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'better_exposed_filters';
  $handler->display->display_options['exposed_form']['options']['bef'] = array(
    'general' => array(
      'allow_secondary' => 0,
      'secondary_label' => 'Advanced options',
    ),
    'txn_id' => array(
      'bef_format' => 'default',
      'more_options' => array(
        'is_secondary' => 0,
        'any_label' => '',
        'bef_filter_description' => '',
        'tokens' => array(
          'available' => array(
            0 => 'global_types',
          ),
        ),
        'rewrite' => array(
          'filter_rewrite_values' => '',
        ),
      ),
    ),
    'created' => array(
      'bef_format' => 'bef_datepicker',
      'more_options' => array(
        'is_secondary' => 0,
        'any_label' => '',
        'bef_filter_description' => '',
        'tokens' => array(
          'available' => array(
            0 => 'global_types',
          ),
        ),
        'rewrite' => array(
          'filter_rewrite_values' => '',
        ),
      ),
    ),
    'created_1' => array(
      'bef_format' => 'bef_datepicker',
      'more_options' => array(
        'is_secondary' => 0,
        'any_label' => '',
        'bef_filter_description' => '',
        'tokens' => array(
          'available' => array(
            0 => 'global_types',
          ),
        ),
        'rewrite' => array(
          'filter_rewrite_values' => '',
        ),
      ),
    ),
    'uid' => array(
      'bef_format' => 'default',
      'more_options' => array(
        'bef_select_all_none' => FALSE,
        'bef_collapsible' => 0,
        'is_secondary' => 0,
        'any_label' => '',
        'bef_filter_description' => '',
        'tokens' => array(
          'available' => array(
            0 => 'global_types',
          ),
        ),
        'rewrite' => array(
          'filter_rewrite_values' => '',
        ),
      ),
    ),
    'mail' => array(
      'bef_format' => 'default',
      'more_options' => array(
        'is_secondary' => 0,
        'any_label' => '',
        'bef_filter_description' => '',
        'tokens' => array(
          'available' => array(
            0 => 'global_types',
          ),
        ),
        'rewrite' => array(
          'filter_rewrite_values' => '',
        ),
      ),
    ),
  );
  $handler->display->display_options['exposed_form']['options']['input_required'] = 0;
  $handler->display->display_options['exposed_form']['options']['text_input_required_format'] = 'filtered_html';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'counter' => 'counter',
    'txn_id' => 'txn_id',
    'created' => 'created',
    'name' => 'name',
    'mail' => 'mail',
    'expire' => 'expire',
    'cancelled' => 'cancelled',
    'amount' => 'amount',
    'currency' => 'amount',
    'item_name' => 'item_name',
  );
  $handler->display->display_options['style_options']['default'] = 'created';
  $handler->display->display_options['style_options']['info'] = array(
    'counter' => array(
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'txn_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-left',
      'separator' => '',
      'empty_column' => 0,
    ),
    'mail' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'expire' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cancelled' => array(
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'amount' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-right',
      'separator' => '&nbsp;',
      'empty_column' => 0,
    ),
    'currency' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'item_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-left',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  /* No results behavior: Global: Unfiltered text */
  $handler->display->display_options['empty']['area_text_custom']['id'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['table'] = 'views';
  $handler->display->display_options['empty']['area_text_custom']['field'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['empty'] = TRUE;
  $handler->display->display_options['empty']['area_text_custom']['content'] = 'No payments...';
  /* Relationship: PayPal Roles Payments: UID */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'paypal_roles_payments';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: Global: View result counter */
  $handler->display->display_options['fields']['counter']['id'] = 'counter';
  $handler->display->display_options['fields']['counter']['table'] = 'views';
  $handler->display->display_options['fields']['counter']['field'] = 'counter';
  $handler->display->display_options['fields']['counter']['label'] = '';
  $handler->display->display_options['fields']['counter']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['counter']['counter_start'] = '1';
  /* Field: PayPal Roles Payments: Transaction ID. */
  $handler->display->display_options['fields']['txn_id']['id'] = 'txn_id';
  $handler->display->display_options['fields']['txn_id']['table'] = 'paypal_roles_payments';
  $handler->display->display_options['fields']['txn_id']['field'] = 'txn_id';
  /* Field: PayPal Roles Payments: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'paypal_roles_payments';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'User name';
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
  /* Field: User: E-mail */
  $handler->display->display_options['fields']['mail']['id'] = 'mail';
  $handler->display->display_options['fields']['mail']['table'] = 'users';
  $handler->display->display_options['fields']['mail']['field'] = 'mail';
  $handler->display->display_options['fields']['mail']['relationship'] = 'uid';
  $handler->display->display_options['fields']['mail']['label'] = 'User email';
  $handler->display->display_options['fields']['mail']['link_to_user'] = '0';
  /* Field: PayPal Roles Payments: Amount. */
  $handler->display->display_options['fields']['amount']['id'] = 'amount';
  $handler->display->display_options['fields']['amount']['table'] = 'paypal_roles_payments';
  $handler->display->display_options['fields']['amount']['field'] = 'amount';
  $handler->display->display_options['fields']['amount']['label'] = 'Payed Amount';
  $handler->display->display_options['fields']['amount']['set_precision'] = TRUE;
  $handler->display->display_options['fields']['amount']['precision'] = '2';
  $handler->display->display_options['fields']['amount']['separator'] = '.';
  /* Field: PayPal Roles Payments: Currency. */
  $handler->display->display_options['fields']['currency']['id'] = 'currency';
  $handler->display->display_options['fields']['currency']['table'] = 'paypal_roles_payments';
  $handler->display->display_options['fields']['currency']['field'] = 'currency';
  $handler->display->display_options['fields']['currency']['label'] = '';
  $handler->display->display_options['fields']['currency']['element_label_colon'] = FALSE;
  /* Field: PayPal Roles Payments: Membership name. */
  $handler->display->display_options['fields']['item_name']['id'] = 'item_name';
  $handler->display->display_options['fields']['item_name']['table'] = 'paypal_roles_payments';
  $handler->display->display_options['fields']['item_name']['field'] = 'item_name';
  $handler->display->display_options['fields']['item_name']['label'] = 'Payed for';
  /* Field: PayPal Roles Payments: Expire date */
  $handler->display->display_options['fields']['expire']['id'] = 'expire';
  $handler->display->display_options['fields']['expire']['table'] = 'paypal_roles_payments';
  $handler->display->display_options['fields']['expire']['field'] = 'expire';
  $handler->display->display_options['fields']['expire']['date_format'] = 'short';
  $handler->display->display_options['fields']['expire']['second_date_format'] = 'long';
  /* Field: PayPal Roles Payments: Change status */
  $handler->display->display_options['fields']['cancelled']['id'] = 'cancelled';
  $handler->display->display_options['fields']['cancelled']['table'] = 'paypal_roles_payments';
  $handler->display->display_options['fields']['cancelled']['field'] = 'cancelled';
  /* Sort criterion: PayPal Roles Payments: Created */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'paypal_roles_payments';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: PayPal Roles Payments: Transaction ID. */
  $handler->display->display_options['filters']['txn_id']['id'] = 'txn_id';
  $handler->display->display_options['filters']['txn_id']['table'] = 'paypal_roles_payments';
  $handler->display->display_options['filters']['txn_id']['field'] = 'txn_id';
  $handler->display->display_options['filters']['txn_id']['operator'] = 'contains';
  $handler->display->display_options['filters']['txn_id']['group'] = 1;
  $handler->display->display_options['filters']['txn_id']['exposed'] = TRUE;
  $handler->display->display_options['filters']['txn_id']['expose']['operator_id'] = 'txn_id_op';
  $handler->display->display_options['filters']['txn_id']['expose']['label'] = 'Transaction ID.';
  $handler->display->display_options['filters']['txn_id']['expose']['operator'] = 'txn_id_op';
  $handler->display->display_options['filters']['txn_id']['expose']['identifier'] = 'txn_id';
  $handler->display->display_options['filters']['txn_id']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: PayPal Roles Payments: Created */
  $handler->display->display_options['filters']['created']['id'] = 'created';
  $handler->display->display_options['filters']['created']['table'] = 'paypal_roles_payments';
  $handler->display->display_options['filters']['created']['field'] = 'created';
  $handler->display->display_options['filters']['created']['operator'] = '>=';
  $handler->display->display_options['filters']['created']['group'] = 1;
  $handler->display->display_options['filters']['created']['exposed'] = TRUE;
  $handler->display->display_options['filters']['created']['expose']['operator_id'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['label'] = 'Created (start)';
  $handler->display->display_options['filters']['created']['expose']['operator'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['identifier'] = 'created';
  $handler->display->display_options['filters']['created']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: PayPal Roles Payments: Created */
  $handler->display->display_options['filters']['created_1']['id'] = 'created_1';
  $handler->display->display_options['filters']['created_1']['table'] = 'paypal_roles_payments';
  $handler->display->display_options['filters']['created_1']['field'] = 'created';
  $handler->display->display_options['filters']['created_1']['operator'] = '<=';
  $handler->display->display_options['filters']['created_1']['group'] = 1;
  $handler->display->display_options['filters']['created_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['created_1']['expose']['operator_id'] = 'created_1_op';
  $handler->display->display_options['filters']['created_1']['expose']['label'] = 'Created (to)';
  $handler->display->display_options['filters']['created_1']['expose']['operator'] = 'created_1_op';
  $handler->display->display_options['filters']['created_1']['expose']['identifier'] = 'created_1';
  $handler->display->display_options['filters']['created_1']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['group'] = 1;
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'User name';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: User: E-mail */
  $handler->display->display_options['filters']['mail']['id'] = 'mail';
  $handler->display->display_options['filters']['mail']['table'] = 'users';
  $handler->display->display_options['filters']['mail']['field'] = 'mail';
  $handler->display->display_options['filters']['mail']['relationship'] = 'uid';
  $handler->display->display_options['filters']['mail']['operator'] = 'contains';
  $handler->display->display_options['filters']['mail']['group'] = 1;
  $handler->display->display_options['filters']['mail']['exposed'] = TRUE;
  $handler->display->display_options['filters']['mail']['expose']['operator_id'] = 'mail_op';
  $handler->display->display_options['filters']['mail']['expose']['label'] = 'User email';
  $handler->display->display_options['filters']['mail']['expose']['operator'] = 'mail_op';
  $handler->display->display_options['filters']['mail']['expose']['identifier'] = 'mail';
  $handler->display->display_options['filters']['mail']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');

  $views[$view->name] = $view;

  $view = new view();
  $view->name = 'custom_payments';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'paypal_roles_custom_payments';
  $view->human_name = 'Custom Payments';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Custom Payments';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'better_exposed_filters';
  $handler->display->display_options['exposed_form']['options']['bef'] = array(
    'general' => array(
      'allow_secondary' => 0,
      'secondary_label' => 'Advanced options',
    ),
  );
  $handler->display->display_options['exposed_form']['options']['input_required'] = 0;
  $handler->display->display_options['exposed_form']['options']['text_input_required_format'] = 'filtered_html';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'counter' => 'counter',
    'name' => 'name',
    'mail' => 'mail',
    'date' => 'date',
    'expire' => 'expire',
    'length' => 'length',
    'length_type' => 'length',
    'amount' => 'amount',
    'currency' => 'amount',
    'name_1' => 'name_1',
    'edit_node' => 'edit_node',
    'cancel_node' => 'edit_node',
  );
  $handler->display->display_options['style_options']['default'] = 'date';
  $handler->display->display_options['style_options']['info'] = array(
    'counter' => array(
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-left',
      'separator' => '',
      'empty_column' => 0,
    ),
    'mail' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-left',
      'separator' => '',
      'empty_column' => 0,
    ),
    'date' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'expire' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'length' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '&nbsp;',
      'empty_column' => 0,
    ),
    'length_type' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'amount' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-right',
      'separator' => '&nbsp;',
      'empty_column' => 0,
    ),
    'currency' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_node' => array(
      'align' => 'views-align-center',
      'separator' => '&nbsp;|&nbsp;',
      'empty_column' => 0,
    ),
    'cancel_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  /* No results behavior: Global: Unfiltered text */
  $handler->display->display_options['empty']['area_text_custom']['id'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['table'] = 'views';
  $handler->display->display_options['empty']['area_text_custom']['field'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['empty'] = TRUE;
  $handler->display->display_options['empty']['area_text_custom']['content'] = 'No custom payments...';
  /* Relationship: PayPal Roles Custom Payments: Role ID */
  $handler->display->display_options['relationships']['role']['id'] = 'role';
  $handler->display->display_options['relationships']['role']['table'] = 'paypal_roles_custom_payments';
  $handler->display->display_options['relationships']['role']['field'] = 'role';
  /* Relationship: PayPal Roles Custom Payments: UID */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'paypal_roles_custom_payments';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: Global: View result counter */
  $handler->display->display_options['fields']['counter']['id'] = 'counter';
  $handler->display->display_options['fields']['counter']['table'] = 'views';
  $handler->display->display_options['fields']['counter']['field'] = 'counter';
  $handler->display->display_options['fields']['counter']['label'] = '';
  $handler->display->display_options['fields']['counter']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['counter']['counter_start'] = '1';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'User';
  $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
  /* Field: User: E-mail */
  $handler->display->display_options['fields']['mail']['id'] = 'mail';
  $handler->display->display_options['fields']['mail']['table'] = 'users';
  $handler->display->display_options['fields']['mail']['field'] = 'mail';
  $handler->display->display_options['fields']['mail']['relationship'] = 'uid';
  /* Field: PayPal Roles Custom Payments: Date */
  $handler->display->display_options['fields']['date']['id'] = 'date';
  $handler->display->display_options['fields']['date']['table'] = 'paypal_roles_custom_payments';
  $handler->display->display_options['fields']['date']['field'] = 'date';
  $handler->display->display_options['fields']['date']['label'] = 'Start date';
  $handler->display->display_options['fields']['date']['date_format'] = 'custom';
  $handler->display->display_options['fields']['date']['custom_date_format'] = 'Y-m-d';
  $handler->display->display_options['fields']['date']['second_date_format'] = 'long';
  /* Field: PayPal Roles Custom Payments: Expire date */
  $handler->display->display_options['fields']['expire']['id'] = 'expire';
  $handler->display->display_options['fields']['expire']['table'] = 'paypal_roles_custom_payments';
  $handler->display->display_options['fields']['expire']['field'] = 'expire';
  $handler->display->display_options['fields']['expire']['date_format'] = 'custom';
  $handler->display->display_options['fields']['expire']['custom_date_format'] = 'Y-m-d';
  $handler->display->display_options['fields']['expire']['second_date_format'] = 'long';
  /* Field: PayPal Roles Custom Payments: Length */
  $handler->display->display_options['fields']['length']['id'] = 'length';
  $handler->display->display_options['fields']['length']['table'] = 'paypal_roles_custom_payments';
  $handler->display->display_options['fields']['length']['field'] = 'length';
  $handler->display->display_options['fields']['length']['label'] = 'Length of access';
  /* Field: PayPal Roles Custom Payments: Length type */
  $handler->display->display_options['fields']['length_type']['id'] = 'length_type';
  $handler->display->display_options['fields']['length_type']['table'] = 'paypal_roles_custom_payments';
  $handler->display->display_options['fields']['length_type']['field'] = 'length_type';
  $handler->display->display_options['fields']['length_type']['label'] = '';
  $handler->display->display_options['fields']['length_type']['element_label_colon'] = FALSE;
  /* Field: PayPal Roles Custom Payments: Amount */
  $handler->display->display_options['fields']['amount']['id'] = 'amount';
  $handler->display->display_options['fields']['amount']['table'] = 'paypal_roles_custom_payments';
  $handler->display->display_options['fields']['amount']['field'] = 'amount';
  $handler->display->display_options['fields']['amount']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['amount']['set_precision'] = TRUE;
  $handler->display->display_options['fields']['amount']['precision'] = '2';
  $handler->display->display_options['fields']['amount']['separator'] = ' ';
  /* Field: PayPal Roles Custom Payments: Currency */
  $handler->display->display_options['fields']['currency']['id'] = 'currency';
  $handler->display->display_options['fields']['currency']['table'] = 'paypal_roles_custom_payments';
  $handler->display->display_options['fields']['currency']['field'] = 'currency';
  $handler->display->display_options['fields']['currency']['label'] = '';
  $handler->display->display_options['fields']['currency']['element_label_colon'] = FALSE;
  /* Field: User Role: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'role';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['relationship'] = 'role';
  $handler->display->display_options['fields']['name_1']['label'] = 'Grant access to';
  /* Field: PayPal Roles Custom Payments: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'paypal_roles_custom_payments';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Options';
  $handler->display->display_options['fields']['edit_node']['text'] = 'Edit';
  /* Field: PayPal Roles Custom Payments: Cancel link */
  $handler->display->display_options['fields']['cancel_node']['id'] = 'cancel_node';
  $handler->display->display_options['fields']['cancel_node']['table'] = 'paypal_roles_custom_payments';
  $handler->display->display_options['fields']['cancel_node']['field'] = 'cancel_node';
  $handler->display->display_options['fields']['cancel_node']['label'] = '';
  $handler->display->display_options['fields']['cancel_node']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['cancel_node']['text'] = 'Delete';
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['group'] = 1;
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Name';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
  );
  /* Filter criterion: User: E-mail */
  $handler->display->display_options['filters']['mail']['id'] = 'mail';
  $handler->display->display_options['filters']['mail']['table'] = 'users';
  $handler->display->display_options['filters']['mail']['field'] = 'mail';
  $handler->display->display_options['filters']['mail']['relationship'] = 'uid';
  $handler->display->display_options['filters']['mail']['group'] = 1;
  $handler->display->display_options['filters']['mail']['exposed'] = TRUE;
  $handler->display->display_options['filters']['mail']['expose']['operator_id'] = 'mail_op';
  $handler->display->display_options['filters']['mail']['expose']['label'] = 'E-mail';
  $handler->display->display_options['filters']['mail']['expose']['operator'] = 'mail_op';
  $handler->display->display_options['filters']['mail']['expose']['identifier'] = 'mail';
  $handler->display->display_options['filters']['mail']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');

  $views[$view->name] = $view;

  return $views;
}
