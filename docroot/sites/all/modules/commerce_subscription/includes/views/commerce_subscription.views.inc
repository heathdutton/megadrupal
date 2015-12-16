<?php

/**
 * @file
 * Views integration for the commerce_subscription module.
 */

/**
 * Implements hook_views_data().
 */
function commerce_subscription_views_data() {
  $data = array();

  $data['commerce_subscription']['table']['group']  = t('Commerce Subscription');

  $data['commerce_subscription']['table']['base'] = array(
    'field' => 'subscription_id',
    'title' => t('Commerce Subscription'),
    'help' => t('Subscriptions.'),
  );

  // Expose the subscription ID.
  $data['commerce_subscription']['subscription_id'] = array(
    'title' => t('Subscription ID'),
    'help' => t('The unique internal identifier of the subscription.'),
    'field' => array(
      'handler' => 'commerce_subscription_handler_field_subscription',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'commerce_subscription_handler_argument_subscription_id',
    ),
  );

  // Expose the subscription type.
  $data['commerce_subscription']['type'] = array(
    'title' => t('Type'),
    'help' => t('The human-readable name of the type of the subscription.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'commerce_subscription_handler_filter_subscription_type',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the created and changed timestamps.
  $data['commerce_subscription']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date the subscription was created.'),
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

  $data['commerce_subscription']['changed'] = array(
    'title' => t('Updated date'),
    'help' => t('The date the subscription was last updated.'),
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

  // Expose the user ID.
  $data['commerce_subscription']['uid'] = array(
    'title' => t('User ID'),
    'help' => t('The subscription owner\'s user ID.'),
    'field' => array(
      'handler' => 'commerce_subscription_handler_field_subscription',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  // Expose the subscription status.
  $data['commerce_subscription']['status'] = array(
    'title' => t('Status'),
    'help' => t('Whether or not the subscription is active.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
      'output formats' => array(
        'active-disabled' => array(t('Active'), t('Disabled')),
      ),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Active'),
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  /*
  // Expose the subscription SKU.
  $data['commerce_subscription']['sku'] = array(
    'title' => t('SKU'),
    'help' => t('The unique human-readable identifier of the subscription.'),
    'field' => array(
      'handler' => 'commerce_subscription_handler_field_subscription',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the subscription title.
  $data['commerce_subscription']['title'] = array(
    'title' => t('Title'),
    'help' => t('The title of the subscription used for administrative display.'),
    'field' => array(
      'handler' => 'commerce_subscription_handler_field_subscription',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the creator uid.
  $data['commerce_subscription']['uid'] = array(
    'title' => t('Creator'),
    'help' => t('Relate a subscription to the user who created it.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
      'label' => t('Subscription creator'),
    ),
  );

  // Expose links to operate on the subscription.
  $data['commerce_subscription']['view_subscription'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a simple link to the administrator view of the subscription.'),
      'handler' => 'commerce_subscription_handler_field_subscription_link',
    ),
  );
  $data['commerce_subscription']['edit_subscription'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to edit the subscription.'),
      'handler' => 'commerce_subscription_handler_field_subscription_link_edit',
    ),
  );
  $data['commerce_subscription']['delete_subscription'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to delete the subscription.'),
      'handler' => 'commerce_subscription_handler_field_subscription_link_delete',
    ),
  );

  $data['commerce_subscription']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all the available operations links for the subscription.'),
      'handler' => 'commerce_subscription_handler_field_subscription_operations',
    ),
  );
  */

  return $data;
}

