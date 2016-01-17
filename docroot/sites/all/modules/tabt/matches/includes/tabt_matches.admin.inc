<?php
/**
 * @file
 * Adminpage for the TabT Matches-module.
 */

/**
 * Make an admin-page to set some settings for the this module.
 */
function tabt_matches_admin() {
  $form['tabt_note'] = array(
      '#markup' => '<strong>Note:</strong> You have to clear all caches after changing these settings.',
  );
  $form['tabt_matches_matchespage'] = array(
      '#type' => 'textfield',
      '#title' => t('TabT Calendarpage link'),
      '#default_value' => variable_get('tabt_matches_matchespage', 'championship/calendar'),
      '#field_suffix' => '/A',
      '#size' => 20,
      '#description' => t('The path to the calendar of a team. The teamletter (for example, A) or teamid (for example, 1120-3) will be appended.'),
      '#required' => TRUE,
      '#element_validate' => array('_tabt_validate_path'),
  );
  $form['tabt_matches_matchespage_title'] = array(
      '#type' => 'textfield',
      '#title' => t('TabT Calenderpage title'),
      '#default_value' => variable_get('tabt_matches_matchespage_title', 'Calendar @club @team - @season'),
      '#size' => 40,
      '#description' => t('The pagetitle of the calendar of a team. <em>@club</em> will be replace by the clubname, <em>@team</em> by the teamletter, <em>@division</em> by the divisionname and <em>@season</em> by the seasonname (for example, 2011-2012)'),
      '#required' => TRUE,
  );
  $form['tabt_matches_rankingpage'] = array(
      '#type' => 'textfield',
      '#title' => t('TabT Rankingpage link'),
      '#default_value' => variable_get('tabt_matches_rankingpage', 'championship/ranking'),
      '#field_suffix' => '/A',
      '#size' => 20,
      '#description' => t('The path to the ranking of a team. The teamletter (for example, A) or teamid (for example, 1120-3) will be appended.'),
      '#required' => TRUE,
      '#element_validate' => array('_tabt_validate_path'),
  );
  $form['tabt_matches_rankingpage_title'] = array(
      '#type' => 'textfield',
      '#title' => t('TabT Rankingpage title'),
      '#default_value' => variable_get('tabt_matches_rankingpage_title', 'Ranking @club @team - @season'),
      '#size' => 40,
      '#description' => t('The pagetitle of the ranking of a team. <em>@club</em> will be replace by the clubname, <em>@team</em> by the teamletter, <em>@division</em> by the divisionname and <em>@season</em> by the seasonname (for example, 2011-2012)'),
      '#required' => TRUE,
  );
  $form['tabt_matches_matchpage'] = array(
      '#type' => 'textfield',
      '#title' => t('TabT Matchpage link'),
      '#default_value' => variable_get('tabt_matches_matchpage', 'championship/match'),
      '#field_suffix' => '/201937',
      '#size' => 20,
      '#description' => t('The path to a match. The matchnumber (for example, 201937) will be appended.'),
      '#required' => TRUE,
      '#element_validate' => array('_tabt_validate_path'),
  );
  $form['tabt_matches_matchpage_title'] = array(
      '#type' => 'textfield',
      '#title' => t('TabT Matchpage title'),
      '#default_value' => variable_get('tabt_matches_matchpage_title', 'Match @matchid: @hometeam - @awayteam'),
      '#size' => 40,
      '#description' => t('The pagetitle of a match.<br /><em>@matchid</em> will be replaced by the Match-ID (for example, VlB01/234), <em>@hometeam</em> and <em>@awayteam</em> by the 2 teams, <em>@score</em> by the score and <em>@week</em> by the weekname.'),
      '#required' => TRUE,
  );
  $form['tabt_matches_reportview'] = array(
      '#type' => 'textfield',
      '#title' => t('Reportslist viewname'),
      '#default_value' => variable_get('tabt_matches_reportview'),
      '#size' => 40,
      '#description' => t('You can make a view with a block that lists all reports for a match. The first argument should be the Match Unique ID (for example, 201937).'),
  );

  $form['tabt_matches_get_results'] = array(
      '#type' => 'checkbox',
      '#title' => t('Load match details'),
      '#default_value' => variable_get('tabt_matches_get_results'),
      '#size' => 10,
      '#description' => t('Choose whether or not to load additional details from the TabT-site.<br />Note that his can increase loadtimes.'),
  );

  $form['tabt_matches_show_sets'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show setscores'),
      '#default_value' => variable_get('tabt_matches_show_sets'),
      '#size' => 10,
      '#description' => t('Show the scores of each set if available.'),
  );
  return system_settings_form($form);
}
