<?php
/**
 * @file
 * The load_results() function to load matchdetails.
 */

/**
 * Load the match results and copy them in the TabTMatch-object.
 *
 * @param match
 *   The TabTMatch-object.
 */
function _tabt_matches_load_results($match) {
  // Don't get the resutls if we don't have an id or if we shouldn't get them
  // in the first place.
  $match->err = TRUE;
  $match->errinfo = "no data";
  if (!$match->id || $match->id == '') {
    $match->err = TRUE;
    $match->errinfo = 'no id';
    return;
  }
  if (!variable_get('tabt_matches_get_results')) {
    return;
  }

  // Load the first page based on the Matchnumber.
  $contents = file_get_contents('http://' . variable_get('tabt_site', 'competitie.vttl.be') . '/index.php?sel=' . $match->id . '&detail=1&list=1&menu=4');
  $match_results = '';
  // The page will tell if the Matchnumber does not exist.
  if (strpos($contents, 'is not a valid match!') !== FALSE) {
    $match->err = TRUE;
    $match->errinfo = t('invalid match.', array(), array('context' => 'game'));
    return;
  }
  // The page will tell if there are no details available yet for the match
  // (so we shouldn't show this page).
  if (strpos($contents, 'There is no detail available for this match') !== FALSE) {
    $match->err = TRUE;
    $match->errinfo = t('no details available.');
    return;
  }
  // We get redirected to another page. The Results-site uses it's own
  // internal ID's which we can't calculate, so we just grab the URL.
  if (strpos($contents, '<div id="mainarea"><SCRIPT>location.replace(') !== FALSE) {
    if (preg_match("/\<SCRIPT>location\.replace\('([^']+)'\);\<\/SCRIPT>/", $contents, $matches)) {
      $match_results = 'http://' . variable_get('tabt_site', 'competitie.vttl.be') . $matches[1] . '&lang=en';
    }
    else {
      $match_results = '';
    }
  }
  // See if we got a new URL to go to.
  if ($match_results == '') {
    $match->err = TRUE;
    $match->errinfo = 'cannot find relocation address.';
    return;
  }
  // Get the contents of the actual resultspage.
  $contents = file_get_contents($match_results);
  // The site uses a different encoding than we are using, so change it or
  // we lose names.
  $contents = iconv('Windows-1252', 'UTF-8', $contents);
  $lines = explode("\n", $contents);
  $current_data = 'season';
  $current_data2 = '';
  $data = array();
  $breakfor = FALSE;
  $resulttype = 1;
  foreach ($lines as $line) {
    if ($breakfor) {
      break;
    }
    switch ($current_data) {
      case 'season':
        if (strpos($line, '<span class="Interclubs_Title_2ndLine">Season') !== FALSE) {
          $current_data = 'week';
          if (preg_match('/\<span class="Interclubs_Title_2ndLine">Season (....\-....)\<\/span>/', $line, $matches)) {
            $match->Season = tabt_process_seasonnumber($matches[1]);
          }
        }
      case 'week':
        // The first thing we need to get, is the weekname.
        if (strpos($line, '<tr><td class="Interclubs_Subtitle">Week&nbsp;') !== FALSE) {
          $current_data = 'start';
          if (preg_match('/\<OPTION value="(.|..)" SELECTED>(.|..)\<\/OPTION>/', $line, $matches)) {
            $match->WeekName = $matches[1];
          }
        }
        break;
      case 'start':
        // Let's wait with processing untill we get the title of the
        // match-info.
        if (strpos($line, '<tr><td class="Interclubs_Subtitle">Match') !== FALSE) {
          $current_data = 'info';
          $current_data2 = 'start';
          $data_list = array();
          $data_entry = new stdClass();
        }
        break;
      case 'info':
        if (strpos($line, '<TR VALIGN=top>') !== FALSE) {
          // We've reached the end, move on to the hometeam-members.
          $current_data = 'hometeam';
          $current_data2 = 'start';
          $data_list = array();
          $data_entry = new stdClass();
        }
        else {
          switch ($current_data2) {
            case 'start':
              // First we need to get the MatchID. We actually already have
              // this one, but let's get it again anyway
              // should this one be different.
              if (strpos($line, '<td class="DBTable_first">') !== FALSE) {
                $match->MatchId = trim(strip_tags($line));
                $current_data2 = 'date';
              }
              break;
            case 'date':
              // Get the date. We also already have this one, but get this
              // one too. If there is a difference, this one
              // will be the correct one.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $match->Date = trim(strip_tags($line));
                $current_data2 = 'home';
              }
              break;
            case 'home':
              // Get the hometeam. Same as before: we already got it, but
              // if there is a difference, this one will be correct.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $hometeam = explode(' ', trim(strip_tags($line)));
                $match->HomeClub = $hometeam[0];
                unset($hometeam[0]);
                $match->HomeTeam = implode(' ', $hometeam);
                if (strpos($match->HomeClub, variable_get('tabt_club_id')) !== FALSE) {
                  $match->isHome = TRUE;
                }
                $current_data2 = 'away';
              }
              break;
            case 'away':
              // Get the awayteam. Same as before: we already got it, but
              // if there is a difference, this one will be correct.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $awayteam = explode(' ', trim(strip_tags($line)));
                $match->AwayClub = $awayteam[0];
                unset($awayteam[0]);
                $match->AwayTeam = implode(' ', $awayteam);
                if (strpos($match->AwayClub, variable_get('tabt_club_id')) !== FALSE) {
                  $match->isAway = TRUE;
                }
                if (!$match->isHome && !$match->isAway) {
                  // We only show matches played by one of our teams. So if
                  // neither of the teams is one of ours, don't show the page.
                  $breakfor = TRUE;
                }
                $current_data2 = 'score';
              }
              break;
            case 'score':
              // Get the score. Same as before: we already got it, but if
              // there is a difference, this one will be correct.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $score = explode('&nbsp;-&nbsp;', trim(strip_tags($line)));
                $match->HomeScore = $score[0];
                $match->AwayScore = $score[1];
                $score2 = explode('&nbsp;', $score[1]);
                if (isset($score2[1])) {
                  $match->AwayScore = $score2[0];
                  if ($match->AwayScore == 0 && $match->HomeScore == 0) {
                    $match->Withdrawal = 2;
                  }
                  elseif ($match->AwayScore == 0) {
                    $match->Withdrawal = 1;
                  }
                  else {
                    $match->Withdrawal = 0;
                  }
                }
                $match->Score = $match->HomeScore . '-' . $match->AwayScore;
                $current_data2 = 'end';
                $match->Cache = TRUE;
                $match->err = FALSE;
              }
              break;
          }
        }
        break;
      case 'hometeam':
      case 'awayteam':
        if (strpos($line, '<TR VALIGN=top>') !== FALSE) {
          // We've reached the end, move on to the matcheslist or awayteam.
          $current_data2 = 'start';
          if ($current_data == 'hometeam') {
            $current_data = 'awayteam';
            $match->HomeTeamMembers = $data_list;
          }
          else {
            $current_data = 'matches';
            $match->AwayTeamMembers = $data_list;
          }
          $data_list = array();
          $data_entry = new stdClass();
        }
        else {
          switch ($current_data2) {
            case 'start':
              // First we get the number of the player. We can actually
              // calculate this ourselfs since it's the arraykey + 1, but
              // we'll just grab every bit of info from the page.
              if (strpos($line, '<td class="DBTable_first">') !== FALSE) {
                $data_entry->Id = trim(strip_tags($line));
                $current_data2 = 'index';
              }
              break;
            case 'index':
              // Get the indexnumber of the player.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $data_entry->Index = trim(strip_tags($line));
                if ($data_entry->Index == '?') {
                  $data_entry->Index = t('N/A');
                }
                $current_data2 = 'name';
              }
              break;
            case 'name':
              // Get the name.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $data_entry->Name = ucwords(drupal_strtolower(trim(str_replace(array('&nbsp;', '(J)'), '', strip_tags($line)))));
                if ($data_entry->Name == '?no Last Name? ?no First Name?') {
                  $data_entry->Name = t('N/A');
                }
                // Let's also get the Link to the player's profile.
                if (preg_match('/\<A class=DBTable href="([^"]+)">/', $line, $matches)) {
                  $data_entry->Link = 'http://' . variable_get('tabt_site', 'competitie.vttl.be') . $matches[1] . '&season=' . $match->Season;
                }
                $current_data2 = 'rank';
              }
              break;
            case 'rank':
              // Get the rank.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $data_entry->Rank = trim(strip_tags($line));
                if ($data_entry->Rank == '??') {
                  $data_entry->Rank = t('N/A');
                }
                $current_data2 = 'wins';
              }
              break;
            case 'wins':
              // And get the number of wins.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $data_entry->Wins = trim(strip_tags($line));
                // This is the last bit of info, so we can store it and get
                // everything ready for another player.
                $data_list[] = $data_entry;
                $data_entry = new stdClass();
                $current_data2 = 'start';
              }
              break;
          }
        }
        break;
      case 'doubles-home':
      case 'doubles-away':
        if (strpos($line, '<TR VALIGN=top>') !== FALSE) {
          // We've reached the end, move on to the matcheslist or awayteam.
          $current_data2 = 'start';
          if ($current_data == 'doubles-home') {
            $current_data = 'doubles-away';
            $match->Doubles = new stdClass();
            $match->Doubles->Home = $data_entry;
          }
          else {
            $current_data = 'matches';
            $match->Doubles->Away = $data_entry;
          }
          $data_list = array();
          $data_entry = new stdClass();
        }
        else {
          switch ($current_data2) {
            case 'start':
              if (strpos($line, '<td class="DBTable_first">') !== FALSE) {
                $data_entry->Id = trim(strip_tags($line));
                $current_data2 = 'players';
              }
              break;
            case 'players':
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $players = explode(' - ', trim(strip_tags($line)));
                if (!isset($players[0]) || $players[0] == '?/?') {
                  $players = array();
                }
                $data_entry->Players = $players;
                $current_data2 = 'wins';
              }
              break;
            case 'wins':
              // And get the number of wins.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $data_entry->Wins = trim(strip_tags($line));
                // This is the last bit of info.
                $current_data2 = 'end';
              }
              break;
          }
        }
        break;
      case 'matches':
        if (strpos($line, '<TR VALIGN=top>') !== FALSE) {
          // We've reached the end. There is nothing left after this, so we
          // can just stop processing the page.
          $current_data = 'end';
          $current_data2 = '';
          $match->Matches = $data_list;
          $data_list = array();
          $data_entry = new stdClass();
          $breafor = TRUE;
        }
        else {
          switch ($current_data2) {
            case 'start':
              if (strpos($line, '</TR></TABLE></TD></TABLE>Doubles') !== FALSE) {
                $current_data = 'doubles-home';
              }
              elseif (strpos($line, '<td class="DBTable_first">') !== FALSE) {
                // First we get the match ID.
                $data_entry->Id = trim(strip_tags($line));
                $current_data2 = 'homeplayer';
              }
              elseif (strpos($line, '<th class=DBTable>Set 1</th>') !== FALSE) {
                // Some matches also have the setresults listed, this means
                // the number of cells is higher than usual.
                $resulttype = 2;
                // We'll already get the array ready for the first one.
                $data_entry->Sets = array();
              }
              break;
            case 'homeplayer':
              // Get the name of the homeplayer.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $data_entry->HomePlayer = ucwords(drupal_strtolower(trim(str_replace('J', '', strip_tags($line)))));
                // Let's also get his id.
                if (preg_match('/\<span id="home_player_([0-9])">/', $line, $matches)) {
                  $data_entry->HomePlayerId = $matches[1];
                }
                $current_data2 = 'awayplayer';
              }
              break;
            case 'awayplayer':
              // Get the name and id of the awayplayer.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $data_entry->AwayPlayer = ucwords(drupal_strtolower(trim(str_replace('J', '', strip_tags($line)))));
                if (preg_match('/\<span id="away_player_([0-9])">/', $line, $matches)) {
                  $data_entry->AwayPlayerId = $matches[1];
                }
                // The next thing we encounter depends on whether we have
                // setresults or not.
                if ($resulttype == 1) {
                  $current_data2 = 'sets';
                }
                else {
                  $current_data2 = 'set 1';
                }
              }
              break;
            case 'set 1':
            case 'set 2':
            case 'set 3':
            case 'set 4':
            case 'set 5':
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $setparts = explode(' ', $current_data2);
                $score = trim(strip_tags($line));
                $set = new stdClass();
                // Let's see if this is in fact a number, if not, we have
                // no setresult (meaning, there weren't that many sets).
                if ($score && is_numeric($score)) {
                  // If the number is positive, it indicates the number of
                  // points the awayplayer had and the homeplayer won.
                  if ($score > 0) {
                    // You win a set with at least a 2 points difference,
                    // so if they don't have more than 9, the winner got
                    // 11.
                    if ($score <= 9) {
                      $set->HomeScore = 11;
                    }
                    else {
                      $set->HomeScore = $score + 2;
                    }
                    $set->AwayScore = $score;
                  }
                  else {
                    // Else if the number is negative, it indicates the
                    // number of points the homeplayer had and the
                    // awayplayer won. So first, get the absolute value of
                    // the score.
                    $score *= -1;
                    $set->HomeScore = $score;
                    // Same setup as before: you win with a 2 point
                    // difference.
                    if ($score <= 9) {
                      $set->AwayScore = 11;
                    }
                    else {
                      $set->AwayScore = $score + 2;
                    }
                  }
                }
                // Store the set.
                $data_entry->Sets[] = $set;
                $setparts[1]++;
                // We have 5 cells with sets, so keep on going untill we
                // reach 5.
                if ($setparts[1] <= 5) {
                  $current_data2 = 'set ' . $setparts[1];
                }
                else {
                  $current_data2 = 'sets';
                }
              }
              break;
            case 'sets':
              // Get the overall setscore.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                if (trim(strip_tags($line)) != 'W-O') {
                  $sets = explode('/', trim(strip_tags($line)));
                  $data_entry->HomeSets = $sets[0];
                  $data_entry->AwaySets = $sets[1];
                }
                else {
                  $data_entry->HomeSets = 'W-O';
                  $data_entry->AwaySets = '';
                }
                $current_data2 = 'score';
              }
              break;
            case 'score':
              // Get the teamscore.
              if (strpos($line, '<td class=DBTable>') !== FALSE) {
                $sets = explode('-', trim(strip_tags($line)));
                $data_entry->HomeScore = $sets[0];
                $data_entry->AwayScore = $sets[1];
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
  if ($match->err) {
    $match->Cache = FALSE;
  }
  // Check if one of th teams was really from our club.
  if (!$match->isHome && !$match->isAway) {
    $match->err = TRUE;
    $match->errinfo = t('not a match played by @club.', array('@club' => variable_get('tabt_club_name')));
  }
  $match->Results = TRUE;

}
