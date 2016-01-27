<?php
/**
 * @file
 * Theme the calendar.
 */


/**
 * Theme the calendar.
 */
function theme_tabt_calendar($variables) {
  $output = '';
  $matches = $variables['matches'];
  $team = check_plain($variables['team']);
  $teaminfo = $variables['teaminfo'];
  $team_letter = check_plain($teaminfo->Team);
  if (variable_get('tabt_locale')) {
    setlocale(LC_ALL, variable_get('tabt_locale'));
  }

  if (isset($teaminfo->Alias)) {
    $team_seasons = tabt_get_seasons_teams($teaminfo->Alias);
    $output .= '<center>' . tabt_get_seasons_form($variables['season'], variable_get('tabt_matches_matchespage', 'championship/calendar') . '/' . $teaminfo->Alias, $team_seasons) . '</center>';
  }

  // Loop through received entries of the ranking and prepare
  // the table to be displayed.
  $header = array(
    t('Week'),
    t('Date') . ', ' . t('Time'),
    t('Hometeam'),
    t('Away', array(), array('context' => 'game')),
    t('Score')
  );
  $rows = array();

  foreach ($matches as $match) {
    $date = DateTime::createFromFormat('Y-m-d H:i:s', $match->Date . ' ' . $match->Time);
    if ($date) {
      $date_string = strftime('%a %e/%b, %Hu%M', $date->getTimestamp());
    }
    else {
      $date_string = '';
    }
    $row = array(check_plain($match->WeekName), $date_string);
    $home_style = '';
    $away_style = '';
    if ($match->Withdrawal == 0 || $match->Withdrawal == 2) {
      $home_style .= 'text-decoration: line-through;';
    }
    elseif ($match->Withdrawal == 1 || $match->Withdrawal == 2) {
      $away_style .= 'text-decoration: line-through;';
    }
    if ($match->isHome && drupal_strtoupper(drupal_substr($match->HomeTeam, -1)) == drupal_strtoupper($team_letter)) {
      $home_style .= 'font-weight: bold;';
    }
    else {
      $away_style .= 'font-weight: bold;';
    }
    array_push(
      $row,
      array(
        'style' => $home_style,
        'data' => check_plain($match->HomeTeam)
      ),
      array(
        'style' => $away_style,
        'data' => check_plain($match->AwayTeam)
      )
    );
    if ($match->Link) {
      $row[] = l($match->Score, $match->Link);
    }
    elseif ($match->Score) {
      $row[] = $match->Score;
    }
    else {
      $row[] = '';
    }
    $rows[] = $row;
  }
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= _tabt_get_footer();
  return $output;
}
