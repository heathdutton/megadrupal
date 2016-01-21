<?php

/**
 * @file
 * Expose locks data to Views.
 */

/**
 * Implements hook_views_data().
 */
function itemsessionlock_views_data() {

  $data['itemsessionlock']['table']['group'] = t('Item session locks');

  $data['itemsessionlock']['table']['base'] = array(
    'field' => array('module', 'iid'),
    'title' => t('Item session locks'),
  );
  // Join with users.
  $data['itemsessionlock']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );
  // User field.
  $data['itemsessionlock']['uid'] = array(
    'title' => t('User'),
    'help' => t('User owning the lock.'),
    'relationship' => array(
      'base' => 'users',
      'base field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('Lock owner'),
      'title' => t('Lock owner'),
      'help' => t('User owning the lock'),
    ),
  );
  // Most of our fields are text.
  $text_fields = array(
    'module' => t('Module'),
    'sid' => t('Session sid'),
    'ssid' => t('Secure session sid'),
    'type' => t('Lock item type'),
    'iid' => t('Lock item identifier'),
  );
  foreach ($text_fields as $name => $label) {
    $data['itemsessionlock'][$name] = array(
      'title' => $label,
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

  // Timestamp field.
  $data['itemsessionlock']['timestamp'] = array(
    'title' => t('Timestamp'),
    'help' => t('Time when the lock was set.'),
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

  // Break link.
  $data['itemsessionlock']['break_link'] = array(
    'title' => t('Break link'),
    'field' => array(
      'handler' => 'itemsessionlock_handler_field_break_link',
      'real field' => 'iid',
    ),
  );

  return $data;
}
