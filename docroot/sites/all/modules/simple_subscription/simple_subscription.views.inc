<?php
/**
 * Views 3 support for Simple subscription.
 */

function simple_subscription_views_data() {

  // ----------------------------------------------------------------
  // imple subscription table -- basic table information.

  // Define the base group of this table. Fields that don't
  // have a group defined will go into this field by default.

  $data['simple_subscription_subscriptions']['table']['group'] = t('Subscriptions');

  // Advertise this table as a possible base table
  $data['simple_subscription_subscriptions']['table']['base'] = array(
    'field' => 'sid',
    'title' => t('Subscription'),
    'help' => t("Subscriptions are information about newsletter's subscribers."),
    'weight' => -10,
  );

  $data['simple_subscription_subscriptions']['table']['join'] = array(
    // Subscription links to users via uid.
    'users' => array(
      'left_table' => 'simple_subscription_subscriptions',
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );

  // ----------------------------------------------------------------
  // simple_subscription table -- fields

  // sid
  $data['simple_subscription_subscriptions']['sid'] = array(
    'title' => t('Sid'),
    'help' => t('The subscription ID of the subscription.'), // The help that appears on the UI,
    // Information for displaying the sid
    'field' => array(
      'handler' => 'views_handler_field', // @@@
      'click sortable' => TRUE,
    ),
    // Information for accepting a sid as an argument
    /*
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'name field' => 'title', // the field to display in the summary.
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
    */
    // Information for accepting a sid as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => TRUE,
    ),
    // Information for sorting on a sid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['simple_subscription_subscriptions']['mail'] = array(
    'title' => t('E-mail'),
    'help' => t('The e-mail address of the subscriber.'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['simple_subscription_subscriptions']['hash'] = array(
    'title' => t('Hash'),
    'help' => t('The hash used in mail for this subscription.'),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['users']['table']['join']['simple_subscription_subscriptions'] = array(
    'left_table' => 'users',
    'left_field' => 'uid',
    'field' => 'uid',
  );

  return $data;
}