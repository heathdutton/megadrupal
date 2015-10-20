<?php
/**
 * @file
 * Functions for compatibility between ls_answ module and Views module
 */

/**
 * Implements hook_views_data().
 */
function ls_answ_views_data() {
  // @TODO Add an extra options to synchronize the selected answer nodes.
  $data = array();

  $data['ls_answ']['table']['group'] = t('LS answers');

  // tables + fields that can be used for SQL Joins
  $data['ls_answ']['table']['join'] = array(
    'node_revisions' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
    'node' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
  );

  // ls_status
  module_load_include('inc', 'ls_answ', 'ls_answ');
  $data['ls_answ']['ls_status'] = array(
    'title' => ls_answ_display_sync_headers('ls_status'),
    'help' => t('The answer status, it can be : virgin, editing or completed'),
    'field' => array(
      'handler' => 'ls_answ_views_handlers_field_ls_fields',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // ls_sync
  $data['ls_answ']['ls_sync'] = array(
    'title' => ls_answ_display_sync_headers('ls_sync'),
    'help' => t('The synchronization status, it can be : unsynchronized, synchronized, desynchronized'),
    'field' => array(
      'handler' => 'ls_answ_views_handlers_field_ls_fields',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // ls_last_import
  $data['ls_answ']['ls_last_import'] = array(
    'title' => ls_answ_display_sync_headers('ls_last_import'),
    'help' => t('Date of the last import'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_sort_date',
    ),
  );

  // ls_sid
  $data['ls_answ']['ls_sid'] = array(
    'title' => ls_answ_display_sync_headers('ls_sid'),
    'help' => t('The survey ID on the LimeSurvey site'),
    'field' => array(
      'handler' => 'ls_answ_views_handlers_field_ls_fields',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // ls_lang
  $data['ls_answ']['ls_lang'] = array(
    'title' => ls_answ_display_sync_headers('ls_lang'),
    'help' => t('The survey language on the LimeSurvey site'),
    'field' => array(
      'handler' => 'ls_answ_views_handlers_field_ls_fields',
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

  // ls_aid
  $data['ls_answ']['ls_aid'] = array(
    'title' => ls_answ_display_sync_headers('ls_aid'),
    'help' => t('The answer ID on the LimeSurvey site'),
    'field' => array(
      'handler' => 'ls_answ_views_handlers_field_ls_fields',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // ls_token
  $data['ls_answ']['ls_lang'] = array(
    'title' => ls_answ_display_sync_headers('ls_lang'),
    'help' => t('The invitation and answer token on the LimeSurvey site'),
    'field' => array(
      'handler' => 'ls_answ_views_handlers_field_ls_fields',
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


  $data['ls_answ']['table']['base'] = array(
    'field' => 'vid',
    'title' => t('LimeSurvey answers'),
    'help' => t("LimeSurvey answers nodes"),
    'weight' => 2,
  );

  // When using the new 'LimeSurvey' type you need to use relationships
  // to access fields in other tables.

  // Relationship to the 'Node' table
  $data['ls_answ']['vid'] = array(
    'title' => t('Node'),
    'help' => t('The LimeSurvey answer node revision'),
    'relationship' => array(
      'label' => t('Node'),
      'base' => 'node',
      'base field' => 'vid',
      // This allows us to not show this relationship if the base is already
      // node so users won't create circular relationships.
      //'skip base' => array('node', 'node_revisions'),
    ),
  );

  return $data;
}

