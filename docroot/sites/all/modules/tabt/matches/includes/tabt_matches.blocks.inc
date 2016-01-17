<?php

/**
 * @file
 * Create blocks for the TabT Matches module.
 */

function _tabt_matches_get_matches_between($date_from, $date_to, $season = NULL) {
  $season = tabt_process_seasonnumber($season);
  $teams = tabt_get_teams($season);
  $teams = $teams->TeamEntries;
  usort($teams, 'tabt_teams_sort');
  $matches = array();
  foreach ($teams as $team) {
    $matchlist = tabt_matches_get_matches($team->TeamId, $season);
    foreach ($matchlist as $match) {
      $date = DateTime::createFromFormat('Y-m-d H:i:s', $match->Date . ' ' . $match->Time);
      if ($date != NULL) {
        if ($date->getTimestamp() >= $date_from && $date->getTimestamp() <= $date_to) {
          $matches[$team->TeamId] = $match;
        }
      }
    }
  }
  return $matches;
}

/**
 * Create a block showing the rank for all teams.
 */
function _tabt_matches_rankings_block() {
  $block = array();
  $block['subject'] = t('Rankings');

  $header = array(t('Team'), t('Rank', array(), array('context' => 'game')), t('Points'));
  $rows = array();

  $teams = tabt_get_teams();
  $teams = $teams->TeamEntries;
  usort($teams, 'tabt_teams_sort');
  foreach ($teams as $team) {
    $rank = tabt_matches_get_rank($team->TeamId);
    if (isset($rank->Position) && isset($rank->Points)) {
      $rows[] = array(
        l(tabt_get_teamname($team->TeamId), variable_get('tabt_matches_rankingpage', 'championship/ranking') . '/' . tabt_get_teamurl($team->TeamId)),
        $rank->Position,
        $rank->Points
      );
    }
  }
  $block['content'] = theme('table', array('header' => $header, 'rows' => $rows));
  return $block;
}

/**
 * Create a block showing the next match of each team.
 */
function _tabt_matches_next_matches_block() {
  $block = array();
  $block['subject'] = t('Next Matches', array(), array('context' => 'game'));

  $teams = tabt_get_teams();
  $teams = $teams->TeamEntries;
  usort($teams, 'tabt_teams_sort');
  $matches = array();
  foreach ($teams as $team) {
    $matches[$team->TeamId] = tabt_matches_get_next_match($team->TeamId);
  }
  $block['content'] = theme('tabt_upcoming_matches', array('matches' => $matches));
  return $block;
}

/**
 * Create a block showing the matches of the next divisionweek.
 */
function _tabt_matches_next_divisionweek_matches_block() {
  $block = array();
  $block['subject'] = t('Upcoming Matches');

  $date_from = tabt_matches_get_next_divisionweek();
  $date_to = strtotime("+1 week", $date_from);

  $matches = _tabt_matches_get_matches_between($date_from, $date_to);
  $block['content'] = theme('tabt_upcoming_matches', array('matches' => $matches));
  return $block;
}

/**
 * Create a block showing the matches of the current yearweek.
 */
function _tabt_matches_current_yearweek_matches_block() {
  $block = array();
  $block['subject'] = t('Matches this week');

  $date_from = strtotime("monday this week", time());
  $date_to = strtotime("+1 week", $date_from);

  $matches = _tabt_matches_get_matches_between($date_from, $date_to);

  $block['content'] = theme('tabt_past_matches', array('matches' => $matches));
  return $block;
}

/**
 * Create a block showing the previous match of each team.
 */
function _tabt_matches_previous_matches_block() {
  $block = array();
  $block['subject'] = t('Previous Matches', array(), array('context' => 'game'));

  $teams = tabt_get_teams();
  $teams = $teams->TeamEntries;
  usort($teams, 'tabt_teams_sort');
  $matches = array();
  foreach ($teams as $team) {
    $matches[$team->TeamId] = tabt_matches_get_last_match($team->TeamId);
  }

  $block['content'] = theme('tabt_past_matches', array('matches' => $matches));
  return $block;
}

/**
 * Create a block showing the matches of the previous divisionweek.
 */
function _tabt_matches_previous_divisionweek_matches_block() {
  $block = array();
  $block['subject'] = t('Latest Matches');

  $date_from = tabt_matches_get_previous_divisionweek();
  $date_to = strtotime("+1 week", $date_from);

  $matches = _tabt_matches_get_matches_between($date_from, $date_to);

  $block['content'] = theme('tabt_past_matches', array('matches' => $matches));
  return $block;
}

/**
 * Create a block showing the matches of the current yearweek.
 */
function _tabt_matches_previous_yearweek_matches_block() {
  $block = array();
  $block['subject'] = t('Matches last week');

  $date_to = strtotime("monday this week", time());
  $date_from = strtotime("-1 week", $date_to);

  $matches = _tabt_matches_get_matches_between($date_from, $date_to);

  $block['content'] = theme('tabt_past_matches', array('matches' => $matches));
  return $block;
}