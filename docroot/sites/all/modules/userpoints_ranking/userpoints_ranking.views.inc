<?php

/**
 * @file
 * Implementaion of the views hooks.
 */

/**
 * Implementation of hook_views_plugins().
 */
function userpoints_ranking_views_plugins() {
  return array(
    'module' => 'userpoints_ranking',
    'display' => array(
      'userpoints_ranking' => array(
        'title' => t('Userpoints Ranking'),
        'help' => t('Outputs the view as a messages list. Use this for mass messaging.'),
        'handler' => 'userpoints_ranking_plugin_display',
        'uses hook menu' => FALSE,
        'use ajax' => FALSE,
        'use pager' => FALSE,
        'use_more' => FALSE,
        'accept attachments' => FALSE,
        'admin' => t('Userpoints Ranking Display'),
      ),
    ),
    'style' => array(
      'userpoints_ranking_style' => array(
        'title' => t('Userpoints Ranking Style'),
        'help' => t('Generates .'),
        'handler' => 'userpoints_ranking_plugin_style',
        'parent' => 'default',
        'uses row plugin' => FALSE,
        'uses fields' => FALSE,
        'uses options' => FALSE,
        'type' => 'userpoints_ranking',
      ),
    ),

  );
}

/**
 * Implements hook_views_data().
 */
function userpoints_ranking_views_data() {
  
  ## Period table
  $data['userpoints_ranking_period']['table']['group'] = t('Userpoints Ranking Periods');

  $data['userpoints_ranking_period']['table']['base'] = array(
    'field' => 'period_id',
    'title' => t('Userpoints Ranking Period'),
    'help' => t('!Points ranking periods.', userpoints_translation()),
  );

  $data['userpoints_ranking_period']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
    'taxonomy_term_data' => array(
      'left_field' => 'tid',
      'field' => 'tid',
    ),
  );

  // Describe the tid column of the userpoints ranking period table.
  $data['userpoints_ranking_period']['tid'] = array(
    'title' => t('Category'),
    'help' => t('The categories (terms) of userpoints used for this period'), // The help that appears on the UI,
    'field' => array(
      'handler' => 'userpoints_ranking_handler_field_category',
    ),
    'argument' => array(
      'handler' => 'userpoints_ranking_handler_argument_category',
      'numeric' => TRUE,
      'name field' => 'category', // display this field in the summary
    ),
    'filter' => array(
      'handler' => 'userpoints_ranking_handler_filter_category',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['userpoints_ranking_period']['period_id'] = array(
    'title' => t('Period ID'),
    'help' => t('The ID of the period.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );
  
  $data['userpoints_ranking_period']['period_start_date'] = array(
    'title' => t('Start Date'),
    'help' => t('The start date of the period.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );
  
  $data['userpoints_ranking_period']['period_end_date'] = array(
    'title' => t('End Date'),
    'help' => t('The start date of the period.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );
  
  $data['userpoints_ranking_period']['period_edited'] = array(
    'title' => t('Changed'),
    'help' => t('The change timestamp for the period, for when the period is updated.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  ); 
  
  $data['userpoints_ranking_period']['period_created'] = array(
    'title' => t('Created'),
    'help' => t('The create timestamp for the period, for when the period is created.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['userpoints_ranking_period']['state'] = array(
    'title' => t('State'),
    'help' => t('The state of this period.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Reverse join information. This should not be necessary.
  $data['taxonomy_term_data']['table']['join'] = array(
    'userpoints_ranking_period' => array(
      'left_field' => 'tid',
      'field' => 'tid',
    ),
  );

  
  
  
  
  
  ## Ranking table
  $data['userpoints_ranking']['table']['group'] = t('Userpoints Ranking');

  $data['userpoints_ranking']['table']['base'] = array(
    'field' => 'ranking_id',
    'title' => t('Userpoints Ranking'),
    'help' => t('!Points ranking.', userpoints_translation()),
  );

  $data['userpoints_ranking']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
    'taxonomy_term_data' => array(
      'left_field' => 'tid',
      'field' => 'tid',
    ),
    'userpoints_ranking_period' => array(
      'left_field' => 'period_id',
      'field' => 'period_id',
    ),
  );

  // Describe the tid column of the userpoints ranking period table.
  $data['userpoints_ranking']['tid'] = array(
    'title' => t('Category'),
    'help' => t('The categories (terms) of this ranking'), // The help that appears on the UI,
    'field' => array(
      'handler' => 'userpoints_ranking_handler_field_category',
    ),
    'argument' => array(
      'handler' => 'userpoints_ranking_handler_argument_category',
      'numeric' => TRUE,
      'name field' => 'category', // display this field in the summary
    ),
    'filter' => array(
      'handler' => 'userpoints_ranking_handler_filter_category',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Add relationship to ranking period table.
  $data['userpoints_ranking']['period_id'] = array(
    'title' => t('Userpoint Ranking Period'),
    'help' => t('Relate the ranking table period table.'),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
    'relationship' => array(
      'base' => 'userpoints_ranking_period',
      'field' => 'period_id',
      'label' => t('Userpoint Ranking Period'),
      'handler' => 'views_handler_relationship',
    ),
  );
  
  // Add relationship to user table.
  $data['userpoints_ranking']['uid'] = array(
    'title' => t('User'),
    'help' => t('Relate the userpoints ranking table to the user table.'),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
    'relationship' => array(
      'base' => 'users',
      'field' => 'uid',
      'label' => t('Users'),
      'handler' => 'views_handler_relationship',
    ),
  );
  
  // Add relationship to user table.
  $data['userpoints_ranking']['uid'] = array(
    'title' => t('User'),
    'help' => t('Relate the userpoints table to the user table.'),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
    'relationship' => array(
      'base' => 'users',
      'field' => 'uid',
      'label' => t('Users'),
      'handler' => 'views_handler_relationship',
    ),
  );
  
  $data['userpoints_ranking']['lifetime_points'] = array(
    'title' => t('Lifetime Points'),
    'help' => t('The sum of all user points from creation of the user till end of the period. (Earned and redeemed transactions)'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );
  
  $data['userpoints_ranking']['period_points'] = array(
    'title' => t('Period Points'),
    'help' => t('The sum of all points earned in this period. (only earned transactions)'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );
  
  $data['userpoints_ranking']['period_points_balanced'] = array(
    'title' => t('Period Points Balanced'),
    'help' => t('The sum of all points earned and redeemed points in this period. (Earned and redeemed transactions)'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );
    
  $data['userpoints_ranking']['lifetime_ranking'] = array(
    'title' => t('Lifetime Rank'),
    'help' => t('The rank over all users by points since the creation of the user.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );
    
  $data['userpoints_ranking']['period_ranking'] = array(
    'title' => t('Period Rank'),
    'help' => t('The rank of users by points earned in this period.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );
      
  $data['userpoints_ranking']['period_ranking_balanced'] = array(
    'title' => t('Period Rank Balanced'),
    'help' => t('The rank of users by points earned and redeemed this period.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );
      
  $data['userpoints_ranking']['period_id'] = array(
    'title' => t('Last Ranked Period'),
    'help' => t('Filter the result set to the last ranked period.'),
    'filter' => array(
      'handler' => 'userpoints_ranking_handler_filter_last_period',
    ),
  );
  
  
  // Reverse join information.
  $data['taxonomy_term_data']['table']['join'] = array(
    'userpoints_ranking' => array(
      'left_field' => 'tid',
      'field' => 'tid',
    ),
  );

  return $data;
}
