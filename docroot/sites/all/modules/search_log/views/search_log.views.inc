<?php
/**
 * @file
 *  Views integration for the Search log module.
 */

/**
 * Implement hook_views_data().
 */
function search_log_views_data() {
  $data = array();

  $data['search_log'] = array(
    'table'    => array(
      'group'    => 'Search log',
      'base'     => array(
        'field'    => 'qid',
        'title'    => t('Search log queries'),
        'help'     => t('List of search terms by day, module, result and language.'),
      ),
    ),
    'qid'   => array(
      'title'    => t('Query ID'),
      'help'     => t('Primary key.'),
      'field'    => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
    'q'   => array(
      'title'    => t('Keys'),
      'help'     => t('Search term(s) in the query.'),
      'field'    => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
    ),
    'module' => array(
      'title'    => t('Module'),
      'help'     => t('Module implementing search.'),
      'field'    => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_search_log_module',
      ),
    ),
    'language' => array(
      'title'    => t('Language'),
      'help'     => t('Language code of the query.'),
      'field'    => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => module_exists('locale') ? 'views_handler_filter_locale_language' : 'views_handler_filter_search_log_language',
      ),
    ),
    'day'       => array(
      'title'    => t('Day'),
      'help'     => t('Timestamp of the day search was performed.'),
      'field'    => array(
        'handler'        => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'filter'   => array(
        'handler'        => 'views_handler_filter_date',
      ),
      'sort'     => array(
        'handler'        => 'views_handler_sort_date',
      ),
    ),
    'counter' => array(
      'title'    => t('Counter'),
      'help'     => t('Number of times query performed on day.'),
      'field'    => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
    'result' => array(
      'title'    => t('Result'),
      'help'     => t('Indicator of failed or successful queries.'),
      'field'    => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_search_log_result',
      ),
    ),
  );

  return $data;
}
