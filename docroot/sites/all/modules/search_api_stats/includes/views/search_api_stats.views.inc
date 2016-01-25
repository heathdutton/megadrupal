<?php
/**
 * Implements of hook_views_data().
 */
function search_api_stats_views_data() {
  $data['search_api_stats']['table']['group']  = t('Search API stats');
  // For other base tables, explain how we join
  $data['search_api_stats']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );

  $default_int = array(
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $default_string = array(
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $default_language = array(
    'field' => array(
      'handler' => 'views_handler_field_locale_language',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_locale_language',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_locale_language',
    ),
  );

  $default_date = array(
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
  );

  // ----------------------------------------------------------------
  // search_api_stats table -- fields

  $data['search_api_stats']['qid'] = array(
    'title' => t('Primary Key'),
    'help' => t('The Search Query ID.'),
  ) + $default_int;

  $data['search_api_stats']['s_name'] = array(
    'title' => t('Server name'),
    'help' => t('Server Name.'),
  ) + $default_string;


  $data['search_api_stats']['i_name'] = array(
    'title' => t('Index machine name'),
    'help' => t('Index machine name'),
  ) + $default_string;


  $data['search_api_stats']['timestamp'] = array(
    'title' => t('Timestamp'),
    'help' => t('Unix timestamp of when query occurred.'),
  ) + $default_date;

  $data['search_api_stats']['numfound'] = array(
    'title' => t('Number of found results'),
    'help' => t('Number of results.'),
  ) + $default_int;



  $data['search_api_stats']['total_time'] = array(
    'title' => t('Total time for the query'),
    'help' => t('Number of results.'),
  ) + $default_int;

  $data['search_api_stats']['prepare_time'] = array(
    'title' => t('Prepare time'),
    'help' => t('Time taken by Search API prepare phase for this query (miliseconds).'),
  ) + $default_int;

  $data['search_api_stats']['process_time'] = array(
    'title' => t('Process time'),
    'help' => t('Time taken by Search API process phase for this query (miliseconds).'),
  ) + $default_int;

  $data['search_api_stats']['uid'] = array(
    'title' => t('User ID'),
    'help' => t('The {users}.uid of the user who triggered the query.'),
  ) + $default_int;

  $data['search_api_stats']['sid'] = array(
    'title' => t('Session ID'),
    'help' => t('Session ID of user who triggered the query.'),
  ) + $default_string;

  $data['search_api_stats']['showed_suggestions'] = array(
    'title' => t('Showed suggestions'),
    'help' => t('Indicates whether a spelling suggestion was shown.'),
  ) + $default_int;

  $data['search_api_stats']['page'] = array(
    'title' => t('Page'),
    'help' => t('Current results page.'),
  ) + $default_string;

  $data['search_api_stats']['keywords'] = array(
    'title' => t('Keywords'),
    'help' => t('Query keywords arguments.'),
  ) + $default_string;

  $data['search_api_stats']['filters'] = array(
    'title' => t('Filters'),
    'help' => t('Query filter arguments.'),
  ) + $default_string;

  $data['search_api_stats']['sort'] = array(
    'title' => t('Sort'),
    'help' => t('Query sort arguments.'),
  ) + $default_string;

  $data['search_api_stats']['language'] = array(
    'title' => t('Language'),
    'help' => t('Search Language In.'),
  ) + $default_language;
  
  $data['search_api_stats']['table']['base'] = array(
    'field' => 'qid',
    'title' => t('Search API stats'),
    'help' => t("Search API stats."),
    'weight' => -9,
  );

  // Relationship to the 'Users' table
  $data['search_api_stats']['uid'] = array(
    'title' => t('User ID'),
    'help' => t('Relationship to users.'),
    'relationship' => array(
      'label' => t('User ID'),
      'base' => 'users',
      'base field' => 'uid',
      // This allows us to not show this relationship if the base is already
      // user so users won't create circular relationships.
      'skip base' => array('users'),
    ),
  );

  return $data;
}

