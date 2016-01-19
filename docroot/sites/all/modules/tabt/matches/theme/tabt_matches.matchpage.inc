<?php
/**
 * @file
 * Create the matchpage.
 */

function _tabt_theme_matchpage_team_members($team_members) {
  $rows[] = array();
  foreach ($team_members as $team_member) {
    $row = array(
      array(
        'style' => 'text-align: center;',
        'data' => check_plain($team_member->Id)
      ),
      array(
        'style' => 'text-align: center;',
        'data' => check_plain($team_member->Index)
      )
    );
    if (isset($team_member->Link)) {
      $row[] = l($team_member->Name, $team_member->Link);
    }
    else {
      $row[] = check_plain($team_member->Name);
    }
    array_push(
      $row,
      array(
        'style' => 'text-align: center;',
        'data' => check_plain($team_member->Rank)
      ),
      array(
        'style' => 'text-align: center;',
        'data' => check_plain($team_member->Wins)
      )
    );
    $rows[] = $row;
  }
  return $rows;
}

function _tabt_theme_matchpage_team_members_double($team_members, $double_players) {
  $row = array();
  $row[] = array('colspan' => '2', 'style' => 'text-align: center;', 'data' => t('Double'));
  if (isset($double_players->Players[0])) {
    if (isset($team_members[$double_players->Players[0] - 1]->Name)) {
      $player1 = check_plain($team_members[$double_players->Players[0] - 1]->Name);
    }
    else {
      $player1 = check_plain($double_players->Players[0]);
    }
    if (isset($double_players->Players[1])) {
      if (isset($team_members[$double_players->Players[1] - 1]->Name)) {
        $player2 = check_plain($team_members[$double_players->Players[1] - 1]->Name);
      }
      else {
        $player2 = check_plain($double_players->Players[1]);
      }
      $double = check_plain($player1) . '<br />' . check_plain($player2);
    }
    else {
      $double = check_plain($player1) . '<br />' . t('Unknown');
    }
  }
  else {
    $double = t('Unknown');
  }
  array_push(
    $row,
    $double,
    '',
    array(
      'style' => 'text-align: center;',
      'data' => check_plain($double_players->Wins)
    )
  );
  return $row;
}

function _tabt_theme_matchpage_matches($match) {
  $rows = array();

  $set_results = (isset($match->Matches[0]->Sets) && variable_get('tabt_matches_show_sets'));
  $output = '';
  if ($set_results) {
    drupal_add_js(drupal_get_path('module', 'tabt_matches') . '/js/tabt_matches.togglesets.js');
    drupal_add_js('var tabt_matches_togglesets_show = \'' . t('Show Sets') . '\';' . "\n" .
    'var tabt_matches_togglesets_hide = \'' . t('Hide Sets') . '\';', 'inline');
    $output .= '
      <script type="text/javascript">
        document.write(\'<a href="#" onclick="togglesets(); return false;" id="settoggle"></a>\');
        togglesets();
      </script>
    ';
  }

  $header = array(
      array(
        'colspan' => 2,
        'style' => 'text-align: center; width: 12%;',
        'data' => '#'
      ),
      array(
        'colspan' => 2,
        'style' => 'text-align: right; width: 49% - 52px;',
        'data' => t('Hometeam')
      ),
      array(
        'colspan' => 2,
        'style' => 'width: 49% - 53px;',
        'data' => t('Awayteam')
      ),
      array(
        'style' => 'width: 5%; text-align: center;',
        'data' => t('Score')
      )
  );

  foreach ($match->Matches as $game) {
    if ($game->HomePlayer == 'Double') {
      $row = array(
        array(
          'style' => 'width: 20px; text-align: center;',
          'rowspan' => ($set_results ? '2' : '1'),
          'data' => check_plain($game->Id)
        ),
        array(
          'style' => 'width: 30px; text-align: center;',
          'rowspan' => ($set_results ? '2' : '1'),
          'data' => t('Double')
        )
      );
      if (isset($home_double)) {
        $home = $home_double;
      }
      else {
        $home = t('Double');
      }
      if (isset($away_double)) {
        $away = $away_double;
      }
      else {
        $away = t('Double');
      }
    }
    else {
      $row = array(
        array(
          'style' => 'width: 20px; text-align: center;',
          'rowspan' => ($set_results ? '2' : '1'),
          'data' => check_plain($game->Id)
        ),
        array(
          'style' => 'width: 30px; text-align: center;',
          'rowspan' => ($set_results ? '2' : '1'),
          'data' => check_plain($game->HomePlayerId . ' - ' . $game->AwayPlayerId)
        )
      );
      if (isset($match->HomeTeamMembers[$game->HomePlayerId - 1]->Name)) {
        $home = check_plain($match->HomeTeamMembers[$game->HomePlayerId - 1]->Name);
      }
      else {
        $home = check_plain($game->HomePlayer);
      }
      if (isset($match->AwayTeamMembers[$game->AwayPlayerId - 1]->Name)) {
        $away = check_plain($match->AwayTeamMembers[$game->AwayPlayerId - 1]->Name);
      }
      else {
        $away = check_plain($game->AwayPlayer);
      }
    }
    if ($game->HomeSets != 'W-O') {
      array_push(
        $row,
        array(
          'style' => ($game->HomeSets > $game->AwaySets ? 'font-weight: bold; width: 40%;' : '') .
          'text-align: right;',
          'data' => $home
        ),
        array(
          'style' => ($game->HomeSets > $game->AwaySets ? 'font-weight: bold; width: 20px;' : '') .
          'text-align: right;',
          'data' => check_plain($game->HomeSets)
        )
      );
      array_push(
        $row,
        array(
          'style' => ($game->HomeSets < $game->AwaySets ? 'font-weight: bold; width: 20px;' : ''),
          'data' => check_plain($game->AwaySets)
        ),
        array(
          'style' => ($game->HomeSets < $game->AwaySets ? 'font-weight: bold; width: 44%;' : ''),
          'data' => $away
        )
      );
    }
    else {
      if ($match->HomeTeamMembers[$game->HomePlayerId - 1]->Wins == 'W-O') {
        array_push(
          $row,
          array(
            'style' => 'text-decoration: line-through; text-align: right;',
            'data' => $home
          ),
          array(
            'style' => 'text-align: right; width: 20px;',
            'data' => 'W-O'
          ),
          array(
            'style' => 'font-weight: bold;',
            'data' => ''
          ),
          array(
            'style' => 'font-weight: bold; width: 44%;',
            'data' => $away
          )
        );
      }
      else {
        array_push(
          $row,
          array(
            'style' => 'font-weight: bold; text-align: right;',
            'data' => $home
          ),
          array(
            'style' => 'font-weight: bold; text-align: right;',
            'data' => ''
          ),
          array(
            'style' => 'width: 20px;',
            'data' => 'W-O'
          ),
          array(
            'style' => 'text-decoration: line-through;',
            'data' => $away
          )
        );
      }
    }
    $row[] = array(
      'style' => 'text-align: center',
      'rowspan' => ($set_results ? '2' : '1'),
      'data' => check_plain($game->HomeScore . ' - ' . $game->AwayScore)
    );
    $rows[] = $row;
    if ($set_results) {
      $row2 = array();
      foreach ($game->Sets as $set) {
        if (isset($set->HomeScore)) {
          if ($set->HomeScore > $set->AwayScore) {
            $row2[] = array(
              'style' => 'width: 60px; text-align: center; border-top: 0px; border-bottom: 0px;',
              'data' => '<strong>' . check_plain($set->HomeScore) . '</strong> - ' . check_plain($set->AwayScore)
            );
          }
          else {
            $row2[] = array(
              'style' => 'width: 60px; text-align: center; border-top: 0px; border-bottom: 0px;',
              'data' => check_plain($set->HomeScore) . ' - <strong>' . check_plain($set->AwayScore) . '</strong>'
            );
          }
        }
        else {
          $row2[] = array(
            'style' => 'width: 60px; text-align: center; border-top: 0px; border-bottom: 0px;',
            'data' => '&nbsp;'
          );
        }

      }
      $table2 = theme('table', array(
        'rows' => array(
          array(
            'no_striping' => TRUE,
            'style' => 'border: 0px;',
            'data' => $row2
          )
        ),
        'attributes' => array(
          'style' => 'width: auto; margin: 0; padding-bottom: 10spx; text-align: center;'
        )
      ));
      $rows[] = array(
        'no_striping' => TRUE,
        'class' => array(($game->Id % 2 == 1 ? 'odd' : 'even')),
        'data' => array(
          array(
            'class' => array('set'),
            'style' => 'text-align: center; padding: 0%; padding-right: 4%;',
            'colspan' => 4,
            'data' => '<center>' . $table2 . '</center>'
          )
        )
      );
    }
  }
  return $output . "\n" . theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Theme the matchpage.
 */
function theme_tabt_matchpage($variables) {
  $match = $variables['match'];
  $output = '';

  if ($match->Results) {

    if (variable_get('tabt_locale')) {
      setlocale(LC_ALL, variable_get('tabt_locale'));
    }
    $header = array(
      array(
        'style' => 'width: 15%;',
        'data' => t('Match', array(), array('context' => 'game'))
      ),
      array(
        'colspan' => 2,
        'style' => 'text-align: right; width: 35%;',
        'data' => t('Hometeam')
      ),
      array(
        'style' => 'width: 35%;',
        'colspan' => 2,
        'data' => t('Awayteam')
      ),
      array(
        'style' => 'width: 15%;',
        'data' => t('Date') . '/' . t('Time')
      )
    );
    $row = array(l($match->MatchId, $match->VTTLLink));
    $row[] = array(
      'style' => ($match->isHome ? 'font-weight: bold; ' : '') .
      ($match->Withdrawal == 0 || $match->Withdrawal == 2 ? 'text-decoration: line-through; ' : '') .
      'text-align: right; width: auto;',
      'data' => check_plain($match->HomeClub . ' ' . $match->HomeTeam)
    );
    $row[] = array(
      'style' => ($match->isHome ? 'font-weight: bold; ' : '') .
      'text-align: right; width: 5%;',
      'data' => check_plain($match->HomeScore)
    );
    $row[] = array(
      'style' => ($match->isAway ? 'font-weight: bold; ' : '') .
      'width: 5%;',
      'data' => check_plain($match->AwayScore)
    );
    $row[] = array(
      'style' => ($match->isAway ? 'font-weight: bold; ' : '') .
      ($match->Withdrawal == 1 || $match->Withdrawal == 2 ? 'text-decoration: line-through; ' : '') .
      'width: auto;',
      'data' =>  check_plain($match->AwayClub . ' ' . $match->AwayTeam)
    );
    $date_string = '';
    if ($match->Results) {
      $date_type = 'd-m-y';
    }
    else {
      $date_type = 'y-m-d';
    }
    if ($match->Date) {
      if ($match->Time) {
        $date = DateTime::createFromFormat($date_type . ' H:i:s', $match->Date . ' ' . $match->Time);
        if ($date) {
          $date_string = $date->format('D j/M Y, H\ui');
        }
      }
      else {
        $date = DateTime::createFromFormat($date_type, $match->Date);
        if ($date) {
          $date_string = $date->format('D j/M Y');
        }
      }
    }

    $row[] = $date_string;

    $output .= theme('table', array('header' => $header, 'rows' => array($row))) . "\n";
    if (variable_get('tabt_matches_get_results')) {
      $output .= '<div style="float: left; width: 50%;">' . "\n";
      $output .= '<h2>' . t('Hometeam') . '</h2>' . "\n";

      $header = array(
        array(
          'style' => 'text-align: center; width: 20px;', 'data' => '#'
        ),
        array(
          'style' => 'text-align: center; width: 40px;',
          'data' => t('Index')
        ),
        t('Name'),
        array(
          'style' => 'text-align: center; width: 20px;',
          'data' => t('Rk.', array(), array('context' => 'abbreviation rank'))
        ),
        array(
          'style' => 'text-align: center; width: 20px;',
          'data' => t('Wins')
        )
      );
      $rows = _tabt_theme_matchpage_team_members($match->HomeTeamMembers);
      if (isset($match->Doubles->Home)) {
        $rows[] = _tabt_theme_matchpage_team_members_double($match->HomeTeamMembers, $match->Doubles->Home);
      }
      $output .= theme('table', array('header' => $header, 'rows' => $rows)) . "\n";
      $output .= '</div>' . "\n";

      $output .= '<div style="float: right; width: 50%;">' . "\n";
      $output .= '<h2>' . t('Awayteam') . '</h2>' . "\n";
      $rows = _tabt_theme_matchpage_team_members($match->AwayTeamMembers);
      if (isset($match->Doubles->Away)) {
        $rows[] = _tabt_theme_matchpage_team_members_double($match->AwayTeamMembers, $match->Doubles->Away);
      }
      $output .= theme('table', array('header' => $header, 'rows' => $rows)) . "\n";
      $output .= '</div>' . "\n";

      $output .= '<div style="clear: both;">' . "\n";
      $output .= '<h2>' . t('Match List', array(), array('context' => 'game')) . '</h2>' . "\n";

      $output .=  _tabt_theme_matchpage_matches($match) . "\n";
      $output .= '</div>' . "\n";
    }
  }

  $output .= _tabt_get_footer();

  if (variable_get('tabt_matches_reportview') && ($match->id || $match->MatchUniqueId)) {
    $output .= '<h2>' . t('Match Reports') . '</h2>' . "\n";
    $output .= views_embed_view(variable_get('tabt_matches_reportview'), 'block', ($match->id ? $match->id : $match->MatchUniqueId)) . "\n";
  }

  return $output;
}
