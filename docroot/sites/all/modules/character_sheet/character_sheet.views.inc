<?php

/**
 * @file
 * Basic views integration for character sheetss.
 */

/**
 * Implements hook_views_data(). From Views module.
 */
function character_sheet_views_data() {
  // Character sheet log.
  $data['character_sheet_log']['table']['group'] = t('Character sheet log');
  $data['character_sheet_log']['table']['base'] = array(
    'field' => 'lid',
    'title' => t('Character sheet log table'),
    'help' => t("Character sheet log table contains log content."),
    'weight' => -10,
  );
  $data['character_sheet_log']['nid'] = array(
    'title' => t('Character sheet log node'),
    'help' => t('Node referenced by Character sheet log.'),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Character sheet node'),
    ),
  );
  $data['character_sheet_log']['uid'] = array(
    'title' => t('Character sheet log user'),
    'help' => t('User referenced by Character sheet log.'),
    'relationship' => array(
      'base' => 'users',
      'field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('Character sheet user'),
    ),
  );
  $data['character_sheet_log']['lid'] = array(
    'title' => t('Log ID'),
    'help' => t(''),
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
  $data['character_sheet_log']['type'] = array(
    'title' => t('Log type'),
    'help' => t(''),
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
  $data['character_sheet_log']['message'] = array(
    'title' => t('Message'),
    'help' => t(''),
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
  $data['character_sheet_log']['hostname'] = array(
    'title' => t('Hostname'),
    'help' => t(''),
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
  $data['character_sheet_log']['timestamp'] = array(
    'title' => t('Timestamp'),
    'help' => t(''),
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

  // XP type table.
  $data['character_sheet_xp']['table']['group'] = t('Character sheet xp');
  $data['character_sheet_xp']['nid'] = array(
    'title' => t('Character sheet xp node'),
    'help' => t('Node referenced by Character sheet xp.'),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Character sheet node'),
    ),
  );
  $data['character_sheet_xp']['xp'] = array(
    'title' => t('Character sheet xp amount'),
    'help' => t('Xp amount.'),
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
  $data['character_sheet_xp']['type'] = array(
    'title' => t('Character sheet xp type'),
    'help' => t('Xp type.'),
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
  return $data;
}