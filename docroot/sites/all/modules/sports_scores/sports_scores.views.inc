<?php

/**
 * @file
 * sports_scores_views.inc - Views2 support for sports scores.
 */

/**
 * Implementation of hook_views_data().
 */
function sports_scores_views_data() {
  $data['sports_scores_games']['table']['group'] = t('Sports Scores');

  $data['sports_scores_games']['table']['base'] = array(
    'field' => 'gid', // Required field for a base table.  Must be the primary key.
    'title' => t('Sports Scores Games'),
    'help' => t('The sports scores table contains various sports games and their scores.'),
    'weight' => -10,
  );

 // Expose the GID.
  $data['sports_scores_games']['gid'] = array(
    'title' => t('GID'),
    'help' => t('This is a calculated GUID.  It probably will not look nice:)'),
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

   // Fields.
  $data['sports_scores_games']['sport'] = array(
    'title' => t('Sport'),
    'help' => t('This is the sport for this game.'),
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

  $data['sports_scores_games']['home_team'] = array(
    'title' => t('Home Team'),
    'help' => t('The name or abbreviation of the home team.'),
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
  );

  $data['sports_scores_games']['home_score'] = array(
    'title' => t('Home score'),
    'help' => t('The score for the home team.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  $data['sports_scores_games']['visiting_team'] = array(
    'title' => t('Visiting Team'),
    'help' => t('The name or abbreviation of the visiting team.'),
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
  );

  $data['sports_scores_games']['visiting_score'] = array(
    'title' => t('Visiting score'),
    'help' => t('The score for the visiting team.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );


  $data['sports_scores_games']['gamestart'] = array(
    'title' => t('Start time'),
    'help' => t('The schedule game start time.'),
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

  $data['sports_scores_games']['status_code_text'] = array(
    'title' => t('Final'),
    'help' => t('"F" when game is final.'),
    'real field' => 'status_code',
    'field' => array(
      'handler' => 'sports_scores_games_handler_field_game_status',
      'click sortable' => TRUE,
    ),
  );

  return $data;
}


/**
 * A handler to change the db final field value (0,1) to ('', 'F') for display.
 */
class sports_scores_games_handler_field_game_status extends views_handler_field {
  function render($values) {
    $value = $values->{$this->field_alias};
    return $value == 2 ? 'F' : '';
  }
}
