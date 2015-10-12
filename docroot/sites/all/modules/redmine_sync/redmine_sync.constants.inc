<?php

  define('REDMINE_SYNC_HTML_BR', '<br/>');
  define('REDMINE_SYNC_MODULE_PATH', drupal_get_path('module', 'redmine_sync'));

  define('REDMINE_SYNC_URL',                             'admin/redmine_sync');
  define('REDMINE_SYNC_URL_PAGE_SYNC',                   'admin/redmine_sync/synchronization');
  define('REDMINE_SYNC_URL_PAGE_SETTINGS',               'admin/redmine_sync/settings');
  define('REDMINE_SYNC_URL_PAGE_SETTINGS_DATABASE',      'admin/redmine_sync/settings/database');
  define('REDMINE_SYNC_URL_PAGE_SETTINGS_REST_API',      'admin/redmine_sync/settings/rest_api');
  define('REDMINE_SYNC_URL_CONFIRM_CLEAR_ALL_RECORDS',   'admin/redmine_sync/confirm/clear_all_records');
  define('REDMINE_SYNC_URL_PAGE_PROJECT_ADD',            'node/add/redmine_project');
  define('REDMINE_SYNC_URL_PAGE_PROJECT_MEMBERSHIP_ADD', 'node/add/redmine_project_membership');
  define('REDMINE_SYNC_URL_PAGE_ISSUE_ADD',              'node/add/redmine_issue');
  define('REDMINE_SYNC_URL_PAGE_TIME_ENTRY_ADD',         'node/add/redmine_time_entry');
  define('REDMINE_SYNC_URL_JS_PROJECT_ID_ON_CHANGE',     'redmine_sync_js_project_id_on_change');

  define('REDMINE_SYNC_DEF_VAL_SRC_DB_DRIVER', 'mysql');
  define('REDMINE_SYNC_DEF_VAL_SRC_DB_HOST', '127.0.0.1');
  define('REDMINE_SYNC_DEF_VAL_SRC_DB_PORT', '3306');
  define('REDMINE_SYNC_DEF_VAL_SRC_DB_NAME', 'redmine');
  define('REDMINE_SYNC_DEF_VAL_ITEMS_IMPORT_PER_BATCH', 10000);
  define('REDMINE_SYNC_DEF_VAL_ITEMS_PER_RANGE', 20000);
  define('REDMINE_SYNC_DEF_VAL_NID_FIELD_ID', 21);
  define('REDMINE_SYNC_DEF_DATE_FORMAT',      'Y-m-d');       // Never change this constant!!! p.s. ISO 8601 "YYYY-MM-DD"
  define('REDMINE_SYNC_DEF_DATE_TIME_FORMAT', 'Y-m-d H:i:s'); // Never change this constant!!! p.s. ISO 8601 "YYYY-MM-DDThh:mm:ss"
  define('REDMINE_SYNC_DB_INIT_OK', 1);
  define('REDMINE_SYNC_DB_INIT_EMPTY_PARAMETERS', -1);
  define('REDMINE_SYNC_DB_INIT_INVALID_PARAMETERS_OR_SERVER_NOT_AVAILABLE', -2);

  define('REDMINE_SYNC_REST_API_INIT_OK', 1);
  define('REDMINE_SYNC_REST_API_INIT_EMPTY_PARAMETERS', -1);
  define('REDMINE_SYNC_REST_API_INIT_INVALID_PARAMETERS_OR_SERVER_NOT_AVAILABLE', -2);
  define('REDMINE_SYNC_REST_API_INIT_NO_USER_KEY', -3);
  define('REDMINE_SYNC_REST_API_ADMIN_KEY_EXAMPLE', '0123456789abcdefghijklmnopqrstuvwxyz0123');
  define('REDMINE_SYNC_REST_API_AUTH_MODE_ADMIN_KEY', 0);
  define('REDMINE_SYNC_REST_API_AUTH_MODE_USER_KEYS', 1);

  define('REDMINE_SYNC_RET_VAL_TYPE_DEFAULT', 0);
  define('REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST', 1);
  define('REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST_GROUPED', 2);

  $GLOBALS['REDMINE_SYNC_MODULE_NAMES_LIST'] = array(
    'issue_tracking' => t('Issue tracking'),
    'time_tracking'  => t('Time tracking'),
    'news'           => t('News'),
    'documents'      => t('Documents'),
    'files'          => t('Files'),
    'wiki'           => t('Wiki'),
    'repository'     => t('Repository'),
    'forums'         => t('Forums'),
    'calendar'       => t('Calendar'),
    'gantt'          => t('Gantt'),
  );


