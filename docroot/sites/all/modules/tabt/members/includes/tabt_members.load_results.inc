<?php
/**
 * @file
 * The load_results() function to load individual results.
 */

function _tabt_members_replace_match_type($original_string) {
  $original_types = array(
    'Dames',
    'Heren',
    'Hommes',
    'Filles',
    'Femmes'
  );
  $replacements = array(
   'Women',
   'Men',
   'Men',
   'Women',
   'Women'
  );
  return str_replace($original_types, $replacements, $original_string);
}

/**
 * Load the individual results and store them in the TabTMember-object.
 *
 * @param member
 *   The TabTMember-object
 * @param women
 *   Load women results.
 */
function _tabt_members_load_results($member, $women = FALSE) {
  $member->ResultsLoaded = FALSE;
  if (!$member->UniqueIndex || $member->UniqueIndex == '' || !variable_get('tabt_members_get_results'))
    return;
  // Load up the site with the individual results.
  $contents = file_get_contents('http://' . variable_get('tabt_site', 'competitie.vttl.be') . '/index.php?result=1&index=' . $member->UniqueIndex . '&menu=6&lang=en&season=' . $member->Season . ($women ? '&category=2' : ''));
  // The site uses a different encoding than we are using, so change it or
  // we lose names.
  $contents = iconv('Windows-1252', 'UTF-8', $contents);
  $lines = explode("\n", $contents);
  $current_data = 'pic';
  $current_data2 = '';
  $data = new stdClass();
  $break_for = FALSE;
  foreach ($lines as $line) {
    if ($break_for) {
      break;
    }
    switch ($current_data) {
      case 'pic':
        // We first try to get the picture.
        if (strpos($line, '<td class="DBTable_first"><IMG alt="') !== FALSE) {
          if (preg_match('/\<td class="DBTable_first"><IMG alt="[^"]+" src="([^"]+)" border="0">/', $line, $matches)) {
            $data->Pic = 'http://' . variable_get('tabt_site', 'competitie.vttl.be') . $matches[1];
          }
          $current_data = 'ranking';
        }
        break;
      case 'ranking':
        if (strpos($line, '<td class="DBTable_first">Ranking') !== FALSE) {
          $parts = explode('&nbsp;:&nbsp;', trim(strip_tags($line)));
          if (isset($parts[1])) {
            $data->Ranking = $parts[1];
          }
          $current_data = 'start';
        }
        break;
      case 'start':
        // First we get the overview per rank.
        if (strpos($line, '</TR></TABLE>Summary per ranking') !== FALSE) {
          $current_data = 'ranksum';
          $current_data2 = 'start';
          $data_list = array();
          $data_entry = new stdClass();
        }
        break;
      case 'ranksum':
        if (strpos($line, '</TR></TABLE></TD><TD width=50% valign="top">Summary - Match') !== FALSE) {
          // We've reached the end. Store the info and move on to the
          // match-summary.
          $current_data = 'matchsum';
          $current_data2 = 'perc';
          $data->RankSum = $data_list;
          $data_list = array();
          $data_entry = new stdClass();
        }
        else {
          switch ($current_data2) {
            case 'start':
              // First we get the rank.
              if (strpos($line, '<td class="DBTable_first">') !== FALSE) {
                $data_entry->Rank = trim(strip_tags($line));
                $current_data2 = 'win';
              }
              break;
            case 'win':
              // Get the number of wins.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $data_entry->Win = trim(strip_tags($line));
                $current_data2 = 'lose';
              }
              break;
            case 'lose':
              // Then get the number of losses.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $data_entry->Lose = trim(strip_tags($line));
                $current_data2 = 'perc';
              }
              break;
            case 'perc':
              // And as last we get the percentage of wins.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $perc = explode(' ', trim(strip_tags($line)));
                $data_entry->Perc = $perc[0];
                // This is the last bit of info, so store it and get ready
                // for the next rank.
                $data_list[] = $data_entry;
                $data_entry = new stdClass();
                $current_data2 = 'start';
              }
              break;
          }
        }
        break;
      case 'matchsum':
        if (strpos($line, '</TR></TABLE>Summary - Sets') !== FALSE) {
          // We've reached the end, so store the info and get ready for the
          // setsummary.
          $current_data = 'setsum';
          $current_data2 = 'perc';
          $data->MatchSum = $data_entry;
          $data_list = array();
          $data_entry = new stdClass();
        }
        else {
          switch ($current_data2) {
            case 'perc':
              // We first get the percentage of wins.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $perc = explode(' ', trim(strip_tags($line)));
                $data_entry->Perc = $perc[0];
                $current_data2 = 'play';
              }
              break;
            case 'play':
              // Next we get the number of matches played.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $data_entry->Play = trim(strip_tags($line));
                $current_data2 = 'win';
              }
              break;
            case 'win':
              // Get the number of wins.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $data_entry->Win = trim(strip_tags($line));
                $current_data2 = 'lose';
              }
              break;
            case 'lose':
              // Get the number of losses.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $data_entry->Lose = trim(strip_tags($line));
                $current_data2 = 'abs';
              }
              break;
            case 'abs':
              // Get the absolute points.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $data_entry->Abs = trim(strip_tags($line));
                $current_data2 = 'rel';
              }
              break;
            case 'rel':
              // Get the relative points.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $data_entry->Rel = trim(strip_tags($line));
                $current_data2 = 'elo';
              }
              break;
            case 'elo':
              // Finally get the ELO-points.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $data_entry->Elo = trim(strip_tags($line));
                $current_data2 = 'end';
              }
              break;
          }
        }
        break;
      case 'setsum':
        if (strpos($line, '</TR></TABLE>Distribution of scores (Sets):') !== FALSE) {
          // We've reached the end, so store the info and get ready for
          // setdistribution.
          $current_data = 'setdistrib';
          $current_data2 = 'start';
          $data->SetSum = $data_entry;
          $data_list = array();
          $data_entry = new stdClass();
        }
        else {
          switch ($current_data2) {
            case 'perc':
              // First we get the percentage of wins.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $perc = explode(' ', trim(strip_tags($line)));
                $data_entry->Perc = $perc[0];
                $current_data2 = 'play';
              }
              break;
            case 'play':
              // Get the number of sets played.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $data_entry->Play = trim(strip_tags($line));
                $current_data2 = 'win';
              }
              break;
            case 'win':
              // Get the number of wins.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $data_entry->Win = trim(strip_tags($line));
                $current_data2 = 'lose';
              }
              break;
            case 'lose':
              // Get the number of losses.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $data_entry->Lose = trim(strip_tags($line));
                $current_data2 = 'avg';
              }
              break;
            case 'avg':
              // Get the average number of sets per match.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $data_entry->Avg = trim(strip_tags($line));
                $current_data2 = 'end';
              }
              break;
          }
        }
        break;
      case 'setdistrib':
        if (strpos($line, '</TR></TABLE>Last set results:') !== FALSE) {
          // We've reached the end, so store the info and move on to the
          // last-set results.
          $current_data = 'bellesum';
          $current_data2 = 'start';
          $data->SetDistrib = $data_list;
          $data_list = array();
          $data_entry = new stdClass();
        }
        elseif (strpos($line, '</script>ELO points evolution') !== FALSE) {
          // We've reached the end, but we seem to have reached ELO-points
          // right away.
          $current_data = 'elo';
          $current_data2 = 'start';
          $data->SetDistrib = $data_list;
          $data_list = array();
          $data_entry = new stdClass();
        }
        elseif (strpos($line, '</TR></TABLE>Match list') !== FALSE) {
          // We've reached the end, but we seem to have reached ELO-points
          // right away.
          $current_data = 'matches';
          $current_data2 = 'start';
          $data->SetDistrib = $data_list;
          $data_list = array();
          $data_entry = new stdClass();
          $columns = array();
        }
        else {
          switch ($current_data2) {
            case 'start':
              // Get the setscore.
              if (strpos($line, '<td class="DBTable_first">') !== FALSE) {
                $score = explode('&nbsp;-&nbsp;', trim(strip_tags($line)));
                $data_entry->Win = $score[0];
                $data_entry->Lose = $score[1];
                $current_data2 = 'number';
              }
              break;
            case 'number':
              // Get the amount.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $data_entry->Number = trim(strip_tags($line));
                $data_list[] = $data_entry;
                $data_entry = new stdClass();
                $current_data2 = 'start';
              }
              break;
          }
        }
        break;
      case 'bellesum':
        // We don't always get this information. Only if the player already
        // had a match with 5 sets.
        if (strpos($line, '</script>ELO points evolution') !== FALSE) {
          // We've reached the end, so store the info and move on to
          // ELO Points.
          $current_data = 'elo';
          $current_data2 = 'start';
          $data->BelleSum = $data_entry;
          $data_list = array();
          $data_entry = new stdClass();
        }
        elseif (strpos($line, '</TR></TABLE>Match list') !== FALSE) {
          // We've reached the end, so store the info and move on to the
          // matchlist.
          $current_data = 'matches';
          $current_data2 = 'start';
          $data->BelleSum = $data_entry;
          $data_list = array();
          $data_entry = new stdClass();
          $columns = array();
        }
        else {
          switch ($current_data2) {
            case 'start':
              // We first get the amount.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $data_entry->Play = trim(strip_tags($line));
                $current_data2 = 'playperc';
              }
              break;
            case 'playperc':
              // Get the percentage in relation to amount of sets.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $perc = explode(' ', trim(strip_tags($line)));
                $data_entry->PlayPerc = $perc[0];
                $current_data2 = 'win';
              }
              break;
            case 'win':
              // Get the number of wins.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $data_entry->Win = trim(strip_tags($line));
                $current_data2 = 'winperc';
              }
              break;
            case 'winperc':
              // Get the percentage of wins in relation to amount of
              // 5 set-matches.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $perc = explode(' ', trim(strip_tags($line)));
                $data_entry->WinPerc = $perc[0];
                $current_data2 = 'lose';
              }
              break;
            case 'lose':
              // Get the number of losses.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $data_entry->Lose = trim(strip_tags($line));
                $current_data2 = 'loseperc';
              }
              break;
            case 'loseperc':
              // Get the percentage of losses in relation to amount of
              // 5 set-matches.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $perc = explode(' ', trim(strip_tags($line)));
                $data_entry->LosePerc = $perc[0];
                $current_data2 = 'end';
              }
              break;
          }
        }
        break;
      case 'elo':
        if (strpos($line, '</TR></TABLE>Match list') !== FALSE) {
          // We've reached the end, so store the info and move on to the
          // matchlist.
          $current_data = 'matches';
          $current_data2 = 'start';
          $data->Elo = $data_entry;
          $data_list = array();
          $data_entry = new stdClass();
          $columns = array();
        }
        else {
          switch ($current_data2) {
            case 'start':
              $current_data3 = 'start';
              // Eeverything is on one line here...
              if (strpos($line, '<td class="DBTable_first" valign="top"><table width="100%">') !== FALSE) {
                // Split it into rows.
                $parts = explode('<tr>', $line);
                $break_for2 = FALSE;
                foreach ($parts as $part) {
                  if ($break_for2) {
                    break;
                  }
                  switch ($current_data3) {
                    case 'start':
                      // First we search for the season start.
                      if (strpos($part, 'Season start') !== FALSE) {
                        $parts2 = explode('</td><td>', $part);
                        $data_entry->Start = trim(strip_tags($parts2[1]));
                        $current_data3 = 'min';
                      }
                      break;
                    case 'min':
                      // Then the minimum.
                      if (strpos($part, 'Minimum on') !== FALSE) {
                        $parts2 = explode('</td><td>', $part);
                        $parts3 = explode(' ', trim(strip_tags($parts2[0])));
                        $data_entry->MinDate = $parts3[2];
                        $data_entry->Min = trim(strip_tags($parts2[1]));
                        $current_data3 = 'max';
                      }
                      break;
                    case 'max':
                      // Then the maximum.
                      if (strpos($part, 'Maximum on') !== FALSE) {
                        $parts2 = explode('</td><td>', $part);
                        $parts3 = explode(' ', trim(strip_tags($parts2[0])));
                        $data_entry->MaxDate = $parts3[2];
                        $data_entry->Max = trim(strip_tags($parts2[1]));
                        $current_data3 = 'current';
                      }
                      break;
                    case 'current':
                      // Finally, the current amount.
                      if (strpos($part, 'Today') !== FALSE) {
                        $parts2 = explode('</td><td>', $part);
                        $data_entry->Current = trim(strip_tags($parts2[1]));
                        $current_data3 = 'end';
                        $break_for2 = TRUE;
                      }
                      break;
                  }
                }
                $current_data2 = 'link';
              }
              break;
            case 'link';
            // Let's also try to get the image.
            if (strpos($line, '<IMG id="elo_graph" src="') !== FALSE) {
              if (preg_match('/\<IMG id="elo_graph" src="([^"]+)" border="0" align="absmiddle">/', $line, $matches)) {
                $data_entry->Link = 'http://' . variable_get('tabt_site', 'competitie.vttl.be') . '/' . $matches[1];
              }
              $current_data2 = 'end';
            }
            break;
          }
        }
        break;
      case 'matches':
        if (strpos($line, '<TR VALIGN=top>') !== FALSE) {
          // We've reached the end, so store it and stop processing.
          $current_data = 'end';
          $data->Matches = $data_list;
          $data_list = array();
          $data_entry = new stdClass();
          $break_for = TRUE;
        }
        elseif (strpos($line, '<tr class="DBTable">') !== FALSE) {
          // We've reached the end, so store it and stop processing.
          if ($current_data2 != 'start') {
            $current_data2 = 'start';
            $data_list[] = $data_entry;
            $data_entry = new stdClass();
          }
        }
        else {
          switch ($current_data2) {
            case 'start':
              // First we get the date.
              if (strpos($line, '<td class="DBTable_first">') !== FALSE) {
                if (isset($columns['date']) && $columns['date']) {
                  $data_entry->Date = trim(strip_tags($line));
                  $current_data2 = 'type';
                }
                else {
                  $data_entry->Type = trim(strip_tags($line));
                  if ($data_entry->Type == '') {
                    $data_entry->Type = 'Champ.';
                  }
                  $data_entry->Type = _tabt_members_replace_match_type($data_entry->Type);
                  $current_data2 = 'matchid';
                }
              }
              elseif (strpos($line, '<th class=DBTable>Date</th>') !== FALSE) {
                $columns['date'] = TRUE;
              }
              elseif (strpos($line, '<th class=DBTable>ELO<br>Opp.</th>') !== FALSE) {
                $columns['elo'] = TRUE;
              }
              break;
            case 'type':
              // Get the type of match.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $data_entry->Type = trim(strip_tags($line));
                if ($data_entry->Type == '') {
                  $data_entry->Type = 'Champ.';
                }
                $data_entry->Type = _tabt_members_replace_match_type($data_entry->Type);
                $current_data2 = 'matchid';
              }
              break;
            case 'matchid':
              // Get the MatchID and the link to the match.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $data_entry->Matchid = trim(strip_tags($line));
                if (preg_match('/\<A class=DBTable href="([^"]+)">/', $line, $matches)) {
                  $data_entry->MatchLink = 'http://' . variable_get('tabt_site', 'competitie.vttl.be') . '/index.php' . $matches[1];
                }
                $current_data2 = 'team';
              }
              break;
            case 'team':
              // Get the team or series.
              // Also try to get the link to the team if it's available.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $data_entry->Team = trim(strip_tags($line));
                if (preg_match("/\<A class=DBTable href='([^']+)'>/", $line, $matches)) {
                  $data_entry->TeamLink = 'http://' . variable_get('tabt_site', 'competitie.vttl.be') . $matches[1] . '&season=' . $member->Season;
                }
                $current_data2 = 'oppteam';
              }
              break;
            case 'oppteam':
              // Get the opposing team.
              // Also try to get the teamlink.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $data_entry->OppTeam = trim(strip_tags($line));
                if (preg_match("/\<A class=DBTable href='([^']+)'>/", $line, $matches)) {
                  $data_entry->OppTeamLink = 'http://' . variable_get('tabt_site', 'competitie.vttl.be') . $matches[1] . '&season=' . $member->Season;
                }
                $current_data2 = 'opp';
              }
              break;
            case 'opp':
              // Get the opponent and their link.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $data_entry->Opp = ucwords(drupal_strtolower(trim(strip_tags($line))));
                if (preg_match("/\<A class=DBTable href='([^']+)'>/", $line, $matches)) {
                  $data_entry->OppLink = 'http://' . variable_get('tabt_site', 'competitie.vttl.be') . $matches[1] . '&season=' . $member->Season;
                }
                $current_data2 = 'opprank';
              }
              break;
            case 'opprank':
              // Get the opponents rank.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $data_entry->OppRank = trim(strip_tags($line));
                if (isset($columns['elo']) && $columns['elo']) {
                  $current_data2 = 'oppelo';
                }
                else {
                  $current_data2 = 'score';
                }
              }
              break;
            case 'oppelo':
              // Get the opponents ELO points.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $data_entry->OppElo = trim(strip_tags($line));
                $current_data2 = 'score';
              }
              break;
            case 'score':
              // Get the matchscore.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $score = explode('&nbsp;-&nbsp;', trim(strip_tags($line)));
                $data_entry->Win = $score[0];
                $data_entry->Lose = $score[1];
                $current_data2 = 'elo';
              }
              break;
            case 'elo':
              // Get the players elo-points before the match.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $data_entry->Elo = trim(strip_tags($line));
                $current_data2 = 'elodiff';
              }
              break;
            case 'elodiff':
              // Finally, get the ELO-points that were added/removed.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $data_entry->EloDiff = trim(strip_tags($line));
                // This is the last part, so store the data and get ready
                // for another match.
                $data_list[] = $data_entry;
                $data_entry = new stdClass();
                $current_data2 = 'start';
              }
              break;
          }
        }
        break;
    }
  }
  if (isset($data->Ranking)) {
    $member->ResultsLoaded = TRUE;
  }
  if ($women) {
    $member->WomenResults = $data;
  }
  else {
    $member->Results = $data;
  }
}