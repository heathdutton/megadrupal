<?php
/**
 * @file
 * Theme the memberpage.
 */

function _tabt_members_theme_replace_match_type($original_string) {
  $original_types = array(
    'Champ.',
    'Tourn.',
    'Men',
    'Women'
  );
  $replacements = array(
    t('Champ.', array(), array('context' => 'abbreviation championship')),
    t('Tourn.', array(), array('context' => 'abbreviation tournament')),
    t('Men'),
    t('Women')
  );
  return str_replace($original_types, $replacements, $original_string);
}

/**
 * Theme the memberpage.
 */
function theme_tabt_memberpage($variables) {
  $member = $variables['member'];
  $output = '';
  $member_seasons = tabt_members_get_seasons($member->UniqueIndex);

  $output .= '<center>' . tabt_get_seasons_form($variables['season'], variable_get('tabt_members_memberspage', 'members') . '/' . $member->UniqueIndex, $member_seasons) . '</center>';

  $rows = array();
  if (!$member->UserID) {
    if ($member->ProfilePic) {
      $rows[] = array(
        array(
          'style' => 'text-align: center;',
          'data' => theme('image', array('path' => $member->ProfilePic))
        )
      );
    }
    elseif ($member->DefaultPic) {
      $rows[] = array(
        array(
          'style' => 'text-align: center;',
          'data' => theme('image', array('path' => $member->DefaultPic))
        )
      );
    }
  }

  $rows[] = array(
    array(
      'style' => 'text-align: center;',
      'data' => '<strong>' . t('Player Id') . '</strong>: ' . check_plain($member->UniqueIndex)
    )
  );
  $rows[] = array(
    array(
      'style' => 'text-align: center;',
      'data' => '<strong>' . t('Rank', array(), array('context' => 'game')) . '</strong>: ' . str_replace('NC', t('NC', array(), array('context' => 'abbreviation Not Classified')), check_plain($member->Ranking))
    )
  );
  if ($member->Gender == 'F') {
    if (isset($member->WomenResults->Ranking)) {
      $rows[] = array(
        array(
          'style' => 'text-align: center;',
          'data' => '<strong>' . t('Rank (Women)') . '</strong>: ' . str_replace('NC', t('NC', array(), array('context' => 'abbreviation Not Classified')), check_plain($member->WomenResults->Ranking))
        )
      );
    }
  }
  $rows[] = array(
    array(
      'style' => 'text-align: center;',
      'data' => '<strong>' . t('VTTL URL') . '</strong>: ' .
                l(
                  t('Men'),
                  $member->VTTLLink,
                  array('attributes' => array('target' => '_blank'))
                ) .
                ($member->Gender == 'F' ? ' - ' .
                  l(
                    t('Women'),
                    'http://' . variable_get('tabt_site', 'competitie.vttl.be') . '/index.php?result=1&index=' . $member->UniqueIndex . '&category=2&season=' . $member->Season,
                    array('attributes' => array('target' => '_blank'))
                  ) : ''
                )
    )
  );
  switch ($member->Status) {
    case 'A': $status = t('Active Member'); break;
    case 'R': $status = t('Recreatif Member'); break;
    case 'V': $status = t('Reserve Member'); break;
  }
  $rows[] = array(
    array(
      'style' => 'text-align: center;',
      'data' => '<strong>' . t('Status') . '</strong>: ' . $status
    )
  );
  $rows[] = array(
    array(
      'style' => 'text-align: center;',
      'data' => '<strong>' . t('Age') . '</strong>: ' . check_plain($member->Category)
    )
  );

  $output .= theme('table', array('rows' => $rows, 'style' => 'width: auto;')) . "\n";

  if (isset($member->Results->RankSum)) {
    if ($member->Gender = 'F' && isset($member->WomenResults->RankSum)) {
      $output .= '<div style="text-align: center; width: 100%;"><a href="#womenresults">' . t('Jump to Women-results.') . '</a></div>' . "\n";
    }
    $output .= '<div style="text-align: center; width: 100%;"><h2 style="font-size: 200%">' . t('Men') . '</h2></div>' . "\n";
    $output .= theme('tabt_memberpage_results', array('results' => $member->Results)) . "\n";
  }

  if ($member->Gender = 'F' && isset($member->WomenResults->RankSum)) {
    $output .= '<a name="womenresults"></a>' . "\n";
    $output .= '<div style="text-align: center; width: 100%;"><h2 style="font-size: 200%">' . t('Women') . '</h2></div>' . "\n";
    $output .= theme('tabt_memberpage_results', array('results' => $member->WomenResults)) . "\n";
  }
  $output .= '<small>' . (t('Information obtained from !link', array('!link' => l('http://' . variable_get('tabt_site', 'competitie.vttl.be'), 'http://' . variable_get('tabt_site', 'competitie.vttl.be'))))) . '</small>' . "\n";
  return $output;
}

/**
 * Theme the results on the memberpage.
 */
function theme_tabt_memberpage_results($variables) {
  $results = $variables['results'];

  $output = '';

  if (isset($results->RankSum)) {
    $output .= '<div style="float: left; clear: both; width: 50%;">' . "\n";
    $output .= '<h2>' . t('Summary per rank') . '</h2>' . "\n";
    $header = array(
      t('Rank', array(), array('context' => 'game')),
      t('Won'),
      t('Lost'),
      t('Percentage')
    );
    $rows = array();
    foreach ($results->RankSum as $rank) {
      if ($rank->Rank == 'Total') {
        $rows[] = array(
          'style' => 'font-weight: bold;',
          'data' => array(
            t('Total'),
            check_plain($rank->Win),
            check_plain($rank->Lose),
            check_plain($rank->Perc) . "%"
          )
        );
      }
      else {
        $rows[] = array(
          str_replace('NC', t('NC', array(), array('context' => 'abbreviation Not Classified')), check_plain($rank->Rank)),
          check_plain($rank->Win),
          check_plain($rank->Lose),
          check_plain($rank->Perc) . "%"
        );
      }
    }

    $output .= theme('table', array('header' => $header, 'rows' => $rows)) . "\n";
    $output .= '</div>' . "\n";

    $output .= '<div style="float: right; width: 50%;">' . "\n";
    $output .= '<h2>' . t('Setresults') . '</h2>' . "\n";
    $header = array(t('Set', array(), array('context' => 'game')), t('Amount'));
    $rows = array();
    foreach ($results->SetDistrib as $set) {
      $rows[] = array(check_plain($set->Win . ' - ' . $set->Lose), check_plain($set->Number));
    }
    if (isset($results->BelleSum)) {
      $item = $results->BelleSum;
      $rows[] = array(array('header' => TRUE, 'data' => t('Final Set Summary'), 'colspan' => 2));
      $rows[] = array(t('Played'), check_plain($item->Play . ' (' . $item->PlayPerc . '%)'));
      $rows[] = array(t('Won'), check_plain($item->Win . ' (' . $item->WinPerc . '%)'));
      $rows[] = array(t('Lost'), check_plain($item->Lose . ' (' . $item->LosePerc . '%)'));
    }
    $output .= theme('table', array('header' => $header, 'rows' => $rows)) . "\n";

    $output .= '</div>' . "\n";

    $output .= '<div style="float: left; clear: both; width: 50%;">' . "\n";
    $item = $results->MatchSum;
    $output .= '<h2>' . t('Match Summary') . '</h2>' . "\n";
    $rows = array();
    $rows[] = array(t('Played'), check_plain($item->Play));
    $rows[] = array(t('Won'), check_plain($item->Win . ' (' . $item->Perc . '%)'));
    $rows[] = array(t('Lost'), check_plain($item->Lose . ' (' . (100 - $item->Perc) . '%)'));
    $rows[] = array(t('Absolute points'), check_plain($item->Abs));
    $rows[] = array(t('Relative points'), check_plain($item->Rel));

    $output .= theme('table', array('rows' => $rows)) . "\n";
    $output .= '</div>' . "\n";

    $output .= '<div style="float: right; width: 50%;">' . "\n";
    $output .= '<h2>' . t('Set Summary') . '</h2>' . "\n";

    $rows = array();
    $item = $results->SetSum;
    $rows[] = array(t('Played'), check_plain($item->Play));
    $rows[] = array(t('Average'), check_plain($item->Avg));
    $rows[] = array(t('Won'), check_plain($item->Win . ' (' . $item->Perc . '%)'));
    $rows[] = array(t('Lost'), check_plain($item->Lose . ' (' . (100 - $item->Perc) . '%)'));

    $output .= theme('table', array('rows' => $rows)) . "\n";
    $output .= '</div>' . "\n";

    $output .= '<div style="clear: both;">' . "\n";
    if (isset($results->Elo)) {
      $output .= '<h2>' . t('ELO-points') . '</h2>' . "\n";
      $item = $results->Elo;

      $rows2 = array();
      $rows = array();
      $rows2[] = array(t('Seasonstart'), check_plain($item->Start));
      $rows2[] = array(t('Current points'), check_plain($item->Current));
      $rows2[] = array(t('Minimum this season'), check_plain($item->Min) . ' (' . t('on !date', array('!date' => $item->MinDate)) . ')');
      $rows2[] = array(t('Maximum this season'), check_plain($item->Max) . ' (' . t('on !date', array('!date' => $item->MaxDate)) . ')');
      $rows[] = array(theme('table', array('rows' => $rows2)));
      $rows[] = array(array('style' => 'text-align: center;', 'data' => theme('image', array('path' => $item->Link)), 'colspan' => 2));

      $output .= theme('table', array('rows' => $rows)) . "\n";
    }

    if (isset($results->Matches) && count($results->Matches) > 0) {
      $output .= '<h2>' . t('Match List', array(), array('context' => 'game')) . '</h2>' . "\n";
      $item = $results->Matches;
      $columns = array();
      $columns['date'] = (isset($item[0]->Date));
      $columns['elo'] = (isset($item[0]->Elo));
      $header = array();
      if ($columns['date']) {
        $header[] = t('Date');
      }
      array_push($header,
        t('Type'),
        t('Match', array(), array('context' => 'game')),
        t('Team/Serie'),
        t('Awayteam'),
        t('Opponent'),
        t('Rk.', array(), array('context' => 'abbreviation rank')),
        t('Result')
      );
      if ($columns['elo']) {
        $header[] = array('data' => t('ELO-points'), 'colspan' => '3', 'style' => 'text-align: center;');
      }

      $rows = array();
      if ($columns['elo']) {
        $rows[] = array(
          array(
            'header' => '',
            'data' => '',
            'colspan' => '8'
          ),
          array(
            'header' => TRUE,
            'data' => t('Before')
          ),
          array(
            'header' => TRUE,
            'data' => t('Opp.')
          ),
          array(
            'header' => TRUE,
            'data' => t('Diff.')
          )
        );
      }

      foreach ($item as $match) {
        $row = array();
        if ($columns['date']) {
          $row[] = check_plain($match->Date);
        }
        array_push(
          $row,
          _tabt_members_theme_replace_match_type(check_plain($match->Type)),
          l($match->Matchid, $match->MatchLink)
        );
        if (isset($match->TeamLink)) {
          $row[] = l($match->Team, $match->TeamLink, array('attributes' => array('target' => '_blank')));
        }
        else {
          $row[] = check_plain($match->Team);
        }
        if (isset($match->OppTeamLink)) {
          $row[] = l($match->OppTeam, $match->OppTeamLink, array('attributes' => array('target' => '_blank')));
        }
        else {
          $row[] = check_plain($match->OppTeam);
        }
        array_push(
          $row,
          l($match->Opp, $match->OppLink, array('attributes' => array('target' => '_blank'))),
          check_plain($match->OppRank),
          check_plain($match->Win . ' - ' . $match->Lose)
        );
        if ($columns['elo']) {
          array_push(
            $row, check_plain($match->Elo),
            check_plain($match->OppElo),
            check_plain($match -> EloDiff)
          );
        }
        $rows[] = $row;
      }

      $output .= theme('table', array('header' => $header, 'rows' => $rows)) . "\n";
      $output .= '</div>' . "\n";
      $output .= _tabt_get_footer();
    }
  }
  return $output;
}
