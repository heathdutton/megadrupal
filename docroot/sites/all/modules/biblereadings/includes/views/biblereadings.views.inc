<?php

/**
 * Implementation of hook_views_data().
 */
function biblereadings_views_data() {
  $data = array();

  // Table names
  $data['biblereadings_subscriptions'] ['table']['group'] = t('Bible Readings Subscriptions');

  // Describe base tables
  $data['biblereadings_subscriptions']['table']['base'] = array(
      'field' => 'id',
      'title' => t('Bible Readings Subscriptions'),
      'help' => t("Table containing subscriptions to daily Bible Readings"),
      'weight' => -10,
  );

  /**
   * DESCRIBE FIELDS
   */
  $data['biblereadings_subscriptions']['id'] = _biblereadings_views_field_number(t('ID'), t('Unique ID of the subscription.'));
  $data['biblereadings_subscriptions']['name'] = _biblereadings_views_field_string(t('Subscriber Name'), t('Name of the subscriber'));
  $data['biblereadings_subscriptions']['email'] = _biblereadings_views_field_string(t('Subscriber Email'), t('Email address of the subscriber'));
  $data['biblereadings_subscriptions']['joined'] = _biblereadings_views_field_timestamp(t('Joined On'), t('Timestamp when subscription was created'));
  $data['biblereadings_subscriptions']['unsubscribed'] = _biblereadings_views_field_timestamp(t('Unsubscribed On'), t('Timestamp when subscription was unsubscribed'));


  return $data;
}

function _biblereadings_views_field_number($title, $help) {
  return array(
      'title' => $title,
      'help' => $help,
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
}

function _biblereadings_views_field_string($title, $help) {
  return array(
      'title' => $title,
      'help' => $help,
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

function _biblereadings_views_field_timestamp($title, $help) {
  return array(
      'title' => $title,
      'help' => $help,
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
}
