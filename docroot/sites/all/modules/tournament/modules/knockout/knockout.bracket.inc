<?php

/**
 * @file
 * Handles logic for creating knockout brackets
 */

/**
 * This function builds a knockout bracket match data.
 * Nothing is stored in the database here.
 *
 * @param $node
 *   A tournament node object.
 * @param $participants
 *   An array of participants ordered by seed (ASC).
 */
function knockout_create_matches($node, $participants) {
  // Fill out missing participants with BYEs
  $missing = $node->size - count($participants);
  for ($i=1;$i<=$missing;$i++) {
    $participants[] = 0;
  }

  // Remove the array keys before ordering
  $participants = array_values($participants);
  _knockout_participants_order($participants);

  // Initialize match position
  $position = 1;

  // Create all match data
  $matches = array();
  for ($i=0;$i<count($participants);$i+=2) {
    // Initialize the match
    $match = array(
      'match_date' => $node->first_round_date,
      'position' => $position,
    );

    // Set match participants. Ensure they are not BYEs
    if ($participants[$i]) {
      $id = $participants[$i]['id'];
      $match['participants'][$id] = $participants[$i];
      $match['participants'][$id]['weight'] = 0;
    }
    if ($participants[$i+1]) {
      $id = $participants[$i+1]['id'];
      $match['participants'][$id] = $participants[$i+1];
      $match['participants'][$id]['weight'] = 1;
    }

    // If only 1 participant exists, there is a BYE
    if (count($match['participants']) == 1) {
      // Target the participant
      $id = key($match['participants']);

      // Get the participants new weight
      $weight = $match['position'] % 2 ? 0 : 1;

      // Winner match position in the next round
      $winner = _knockout_get_winning_position($node, $match['position']);

      $match['participants'][$id]['weight'] = $weight;

      if (isset($matches[$winner])) {
        // If the match was already created, simply add the participant
        $matches[$winner]->participants += $match['participants'];
        unset($match);
      }
      else {
        // Move the match to the next round
        $match['position'] = $winner;
        $match['match_date'] = $node->first_round_date;
      }
    }

    if (isset($match)) {
      $matches[$match['position']] = (object) $match;
    }

    $position++;
  }

  return $matches;
}

/**
 * Inserts bracket data into the database.
 *
 * @param $node
 *   A knockout tournament node object.
 * @param $matches
 *   An array of matches to be inserted as nodes.
 * @param $participants
 *   An array of participants.
 *
 * @see knockout_create_matches()
 */
function knockout_bracket_insert($node, $matches, $participants) {
  $transaction = db_transaction();

  try {
    // Set participants seeds
    _knockout_save_participant_seeds($node, $participants);

    // Create all match nodes
    foreach ($matches as $match) {
      knockout_create_match($match, $node);
    }
  }
  catch (Exception $e) {
    $transaction->rollback();
    watchdog_exception('knockout', $e);
    throw $e;
  }
}

/**
 * Returns structured knockout bracket data.
 *
 * @param $node
 *   A knockout tournament node
 * @param $matches
 *   Optional, array of matches to build the bracket. (e.g. when previewing)
 *
 * @return
 *   An associative array of matches, grouped and indexed by round number.
 */
function knockout_bracket_build($node, $matches = array()) {
  if (empty($matches)) {
    $matches = knockout_load_matches($node);
  }
  
  if ($node->format == KNOCKOUT_BRACKET_SINGLE) {
    // Single Elimination brackets have n-1 matches
    $size = ($node->third_place) ? $node->size : $node->size - 1;
  }
  elseif ($node->format == KNOCKOUT_BRACKET_DOUBLE) {
    // Double Elimination brackets have 2n - 2 matches
    $size = 2 * $node->size - 2 + $node->two_round_final;
    // Don't include second round final if the tournament is finished and match wasn't needed
    if ($node->tournament_status == TOURNAMENT_STATUS_FINISHED && !isset($matches[$size])) {
      $size--;
    }
  }

  // Split bracket data into rounds
  $rounds = array();

  for ($position=1;$position<=$size;$position++) {
    // The round number for this match
    $round_num = _knockout_get_round_num($node, $position);

    // Set the tab location
    $location = _knockout_tab_location($node, $position);

    // Set match defaults
    $match = array(
      'position' => $position,
      'status' => 0,
    );
    
    $match['participants'] = _knockout_build_participants($matches, $position);
    
    // There is no match data for first round BYEs.
    if ($round_num == 1 && $location == 'main' && !isset($matches[$position])) {
      // Target the next rounds match
      $winner = _knockout_get_winning_position($node, $position);

      if (!isset($matches[$winner]->participants)) {
        $matches[$winner]->participants = array(
          0 => array('id' => 0, 'name' => '???', 'weight' => 0),
          1 => array('id' => 0, 'name' => '???', 'weight' => 1),
        );
      }

      // Target the participant in the winner match.
      if ($position % 2) {
        $weight = 0;
        reset($matches[$winner]->participants);
      }
      else {
        $weight = 1;
        end($matches[$winner]->participants);
      }

      // Pull back the winner match participant
      $id = key($matches[$winner]->participants);
      $match['participants'][$weight] = $matches[$winner]->participants[$id];
      $match['participants'][$weight]['score'] = 1;

      if (isset($match['participants'][$weight]['entity'])) {
        $label = match_participant_label($node->entity_type, $match['participants'][$weight]['entity']);
        $match['participants'][$weight]['display_name'] = $label;
      }
      else {
        $match['participants'][$weight]['display_name'] = $match['participants'][$weight]['name'];
      }

      // Flip the weight
      $weight ^= 1;

      // Set the empty participant as a BYE
      $match['participants'][$weight] = array(
        'id' => 0,
        'score' => 0,
        'name' => 'BYE',
      );

      $match['status'] = MATCH_STATUS_FINISHED;

      // Do extra checking in DE tournament as this may result in multiple BYEs in the losers bracket
      if ($node->format == KNOCKOUT_BRACKET_DOUBLE) {
        $loser = _knockout_get_losing_position($node, $position);
        $matches[$loser]->participants[] = array(
          'id' => 0,
          'score' => 0,
          'name' => 'BYE',
          'weight' => ($position % 2) ? 0 : 1,
        );
        $bye_match[$loser] = isset($bye_match[$loser]) ? 2 : 1;
      }
    }
    elseif ($round_num == 1 && $location == 'other') {
      // Advance BYEs to round 2 if both participants are BYE participants
      if (isset($bye_match[$position]) && $bye_match[$position] == 2) {
        $winner = _knockout_get_winning_position($node, $position);
        $matches[$winner]->participants[] = array(
          'id' => 0,
          'score' => 0,
          'name' => 'BYE',
          'weight' => 1,
        );
        $bye_match[$winner] =  1;
      }
    }
    elseif ($round_num == 2 && $location == 'other') {
      // If this match has a BYE and the next one has started pull back the auto-advanced participant
      if (isset($bye_match[$position])) {
        $winner = _knockout_get_winning_position($node, $position);
        if (isset($matches[$winner]->nid)) {
          $weight = $position % 2 ? 1 : 0;
          foreach ($matches[$winner]->participants as $participant) {
            if ($participant['weight'] == $weight) {
              $match['participants'][0] = $participant;
              $match['participants'][0]['weight'] = 0;
            }
          }
        }
      }
      elseif (($first_lb_match = $position - $node->size/4) && isset($matches[$first_lb_match])
        && isset($match['participants'][1]) && count($matches[$first_lb_match]->participants) == 1) {
        // If previous match is set and it doesn't have two participants, add back bottom participant
        $weight = key($rounds['other'][1][$first_lb_match - $node->size]['participants']) ? 0 : 1;
        $rounds['other'][1][$first_lb_match - $node->size]['participants'][$weight] = $match['participants'][1];
        $rounds['other'][1][$first_lb_match - $node->size]['participants'][$weight]['score'] = 1;
        $rounds['other'][1][$first_lb_match - $node->size]['participants'][$weight]['weight'] = $weight;
      }
    }

    if (isset($matches[$position]->nid)) {
        $match['nid'] = $matches[$position]->nid;
        $match['status'] = $matches[$position]->match_status;
        $match['node'] = $matches[$position];
    }

    $rounds[$location][$round_num][] = $match;
  }

  return $rounds;
}

/**
 * Populate participant seeds.
 *
 * @param $node
 *   A tournament node object.
 * @param $participants
 *   An array of participants.
 *
 * @return
 *   The $participants array with seed numbers added.
 */
function _knockout_participants_seeds($node, &$participants) {
  $seeds = &drupal_static(__FUNCTION__);

  if (!isset($seeds[$node->nid])) {
    $result = db_select('tournament_knockout_participants', 'kp')
      ->fields('kp', array('id', 'seed'))
      ->condition('nid', $node->nid)
      ->execute();

    foreach ($result as $row) {
      $seeds[$node->nid][$row->id] = $row->seed;
    }
  }

  foreach ($participants as $id => &$participant) {
    $id = $participant['id'];
    if (isset($seeds[$node->nid][$id])) {
      $participant['seed'] = $seeds[$node->nid][$id];
    }
  }
}

/**
 * Prepare match participant data for building brackets.
 *
 * @param $matches
 *   An associative array of matches indexed by position
 * @param $position
 *   The position of the match we wish to target
 *
 * @return
 *   An array of match participants, keyed by weight.
 */
function _knockout_build_participants($matches, $position) {
  $participants = array();

  if (!empty($matches[$position]->participants)) {
    // Participants must be indexed by weight
    $ids = array_keys($matches[$position]->participants);

    foreach ($ids as $id) {
      $weight = $matches[$position]->participants[$id]['weight'];
      $participants[$weight] = $matches[$position]->participants[$id];
      if (!isset($participants[$weight]['score'])) {
        $participants[$weight]['score'] = '-';
      }

      // Return entity label as a link
      if (isset($participants[$weight]['entity'])) {
        $label = match_participant_label($matches[$position]->entity_type, $participants[$weight]['entity']);
        $participants[$weight]['display_name'] = $label;
      }
    }
  }

  return $participants;
}

/**
 * Determines the tab location that a match will appear.
 *
 * @param $node
 *   A knockout tournament node
 * @param $position
 *   Mmatch position in a bracket
 *
 * @return
 *   The tab location as string: 'main', 'other' or 'final'
 */
function _knockout_tab_location($node, $position) {
  $location = $position < $node->size ? 'main' : 'other';

  if ($position < $node->size) {
    return 'main';
  }
  elseif ($position < $node->size * 2 - 2) {
    return 'other';
  }
  else {
    return 'final';
  }
}

/**
 * Loads knockout matches, order them by bracket position and add
 * knockout participant data.
 *
 * @param $node
 *   A knockout tournament node object.
 *
 * @return
 *   An array of matches indexed by bracket position.
 *
 * @see knockout_bracket_build()
 */
function knockout_load_matches($node) {
  $matches = match_load_tournament_matches($node);

  // Re-index matches by their bracket position
  $return = array();
  foreach ($matches as $match) {
    $return[$match->position] = $match;

    // Add participant seed numbers
    _knockout_participants_seeds($node, $match->participants);
  }

  return $return;
}

/**
 * Stores tournament participant seeds in our DB.
 *
 * @param $node
 *   A knockout tournament node object.
 *
 * @param $participants
 *   An array of participant ids, already ordered by seed (ASC).
 */
function _knockout_save_participant_seeds($node, $participants) {
  foreach ($participants as $participant) {
    $values[] = array(
      'nid' => $node->nid,
      'id' => $participant['id'],
      'seed' => $participant['seed'],
    );
  }

  $query = db_insert('tournament_knockout_participants')
    ->fields(array('nid', 'id', 'seed'));

  foreach ($values as $record) {
    $query->values($record);
  }

  $query->execute();
}

/**
 * Orders a given participants array based on their seeds.
 * The resulting order reflects that of a tournament brackets first round.
 *
 * @param $participants
 *   Array of tournament participants ordered by seed
 *
 * @return array
 *   The vertically ordered $participants array
 */
function _knockout_participants_order(&$participants) {
  $size = count($participants);
  $a = range(0, $size - 1);
  $l = log($size, 2);

  foreach ($a as &$e) {
    $order[] = bindec(strrev(sprintf("%0{$l}b", $e)));
  }

  $matches = $size / 2;

  $return = array();
  for ($i=0; $i<$matches; $i++) {
    $top = array_shift($order);
    $bottom = array_pop($order);
    $return[] = $participants[$top];
    $return[] = $participants[$bottom];
  }

  // Replace the original order
  $participants = $return;
}

/**
 * Returns knockout participants for a given tournament, ordered by seed.
 *
 * @param $tournament
 *   A knockout tournament node object.
 */
function _knockout_tournament_participants($tournament) {
  $query = db_select('tournament_knockout_participants', 'k');
  $query->join('users', 'u', 'k.id = u.uid');

  $result = $query->fields('k', array('id', 'seed'))
    ->fields('u', array('name'))
    ->condition('nid', $tournament->nid)
    ->orderBy('seed')
    ->execute()
    ->fetchAllAssoc('id', PDO::FETCH_ASSOC);

  return $result;
}

/**
 * Implements logic for finishing a knockout match.
 * Winners and losers are moved to their next matches.
 *
 * @param $node
 *   A match node object.
 */
function knockout_bracket_match_finish($node) {
  $tournament = $node->tournament_node;

  // We don't care about Single Elimination third place or final matches
  if ($tournament->format == KNOCKOUT_BRACKET_SINGLE && $node->position > ($tournament->size - 2)) {
    return;
  }

  // We don't care about the final match in Double Elimination tournaments
  if ($tournament->format == KNOCKOUT_BRACKET_DOUBLE && $node->position == 2 * $tournament->size - 2 + $tournament->two_round_final) {
    return;
  }

  // Determine the match loser
  foreach ($node->participants as $id => $participant) {
    if ($id != $node->winner) {
      $loser = $node->participants[$id];
      $loser['score'] = 0;
      $loser['id'] = $id;
    }
  }

  // For Double Elimination tournaments, create a losers match
  if ($tournament->format == KNOCKOUT_BRACKET_DOUBLE && $node->position < $tournament->size) {
    // Initialize losers match
    $loser_match = _knockout_load_next_match($node, 'loser');

    // Set loser to match's bottom position. Alternate in first round since both participants are losers
    $loser['weight'] = ($node->position <= $tournament->size / 2) ? abs(($node->position % 2 ) - 1) : 0;

    if (is_object($loser_match)) {
      // If the match already exists, update it
      $loser_match->participants[$loser['id']] = $loser;
      _knockout_reset_participants($loser_match);
      node_save($loser_match);
    }
    else {
      if ($node->position <= $tournament->size / 2) {
        // If corresponding first round match doesn't exist (Had a BYE), advance to second round
        $parent_match = ($node->position % 2 == 0) ? $node->position - 1 : $node->position + 1;
        if (!knockout_load_match_from_position($tournament, $parent_match)) {
          $fake_match = new stdClass();
          $fake_match->position = $loser_match;
          $fake_match->tournament_node = $tournament;
          $loser_match = _knockout_load_next_match($fake_match, 'winner');
          $loser['weight'] = 1;
        }
      }
      elseif ($node->position <= $tournament->size * (3/4)) {
        // If we're coming from winners bracket second round, the loser is going to the losers bracket second round

        // Get order within this round since it matches order of first round of winners bracket
        $order = $loser_match - ($tournament->size / 4) - ($tournament->size - 1);

        // Match positions of first round of winners bracket
        $positions = array(2 * $order, 2 * $order - 1);

        $nid = db_select('tournament_knockout_match', 'k')
          ->fields('k', array('nid'))
          ->condition('position', $positions, 'IN')
          ->condition('tid', $tournament->nid)
          ->execute()
          ->fetchField();

        // If no matches exist, then bother were BYEs, so advance this match to the third round
        if (empty($nid)) {
          $fake_match = new stdClass();
          $fake_match->position = $loser_match;
          $fake_match->tournament_node = $tournament;
          $loser_match = _knockout_load_next_match($fake_match, 'winner');
          $loser['weight'] = ($fake_match->position % 2 ) ? 1 : 0;
        }
      }

      if (is_object($loser_match)) {
        // If the match already exists, update it
        $loser_match->participants[$loser['id']] = $loser;
        _knockout_reset_participants($loser_match);
        node_save($loser_match);
      }
      else {
        // Create the losers match, which is the
        $next_match = new stdClass();
        $next_match->position = $loser_match;
        $next_match->match_date = REQUEST_TIME;
        $next_match->participants = array($loser['id'] => $loser);
        _knockout_reset_participants($next_match);
        knockout_create_match($next_match, $tournament);
      }
    }
  }
  elseif (!empty($tournament->third_place) && in_array($tournament->size - $node->position, array(2, 3))) {
    // Losers of Semi Final matches drop to the third place playoff
    $participants[$loser['id']]['weight'] = $node->position + 3 - $tournament->size;

    if ($third_place_match = knockout_load_match_from_position($tournament, $tournament->size)) {
      $third_place_match->participants += $participants;
      _knockout_reset_participants($third_place_match);
      node_save($third_place_match);
    }
    else {
      // Create the third place match
      $third_place_match = new stdClass();
      $third_place_match->match_date = REQUEST_TIME;
      $third_place_match->position = $tournament->size;
      $third_place_match->participants = $participants;
      _knockout_reset_participants($third_place_match);
      knockout_create_match($third_place_match, $tournament);
    }
  }

  // A winner always advances to the next match if the tournament is not finished
  $winner_match = _knockout_load_next_match($node, 'winner');

  // Initalize winning participant
  $winner = $node->participants[$node->winner];
  $winner['score'] = 0;

  // Determine the winners weight
  if ($node->position < $tournament->size) {
    $winner['weight'] = $node->position % 2 ? 0 : 1;
  }
  elseif (_knockout_get_round_num($tournament, $node->position) % 2 == 1) {
    $winner['weight'] = 1;
  }
  else {
    $winner['weight'] = $node->position % 2 ? 1 : 0;
  }

  if (is_object($winner_match)) {
    // If the match already exists, update it
    $winner_match->participants[$node->winner] = $winner;
    _knockout_reset_participants($winner_match);
    node_save($winner_match);
    return;
  }
  elseif ($node->position == $tournament->size * 2 - 2 && !empty($tournament->two_round_final)) {
    // This is the first of a two round final for a Double Elimination bracket
    $previous_match = knockout_load_match_from_position($tournament, $node->position - 1);

    // If winner came from losers bracket, the tournament continues, so reset scores and repeat participants
    if (!empty($previous_match->participants[$node->winner])) {
      $participants = $node->participants;
      foreach ($participants as $id => $participant) {
        $participants[$id]['score'] = 0;
      }
    }
    else {
      // If the participant with no losses won, the tournament is over
      return;
    }
  }
  else {
    // Default: Grab the winning participant to create the new match
    $participants = array($node->winner => $winner);
  }

  // Create the winners match
  $next_match = new stdClass();
  $next_match->position = $winner_match;
  $next_match->match_date = REQUEST_TIME;
  $next_match->participants = $participants;
  _knockout_reset_participants($next_match);
  knockout_create_match($next_match, $tournament);
}

/**
 * Reset participant confirmations.
 *
 * @param $match
 *   A match node object.
 */
function _knockout_reset_participants(&$match) {
  // Reset participant confirmations
  foreach ($match->participants as &$participant) {
    $participant['confirm'] = 0;
  }
}

/**
 * @param $size
 *  Number of participants
 * @param $position
 *  Match position as ordered sequentially starting from the top of the first round, heading south each round
 *
 * @return float
 *  returns the round that the given match is in
 */
function _knockout_get_round_num($tournament, $position) {
  // Treat DE and SE the same position wise until after what would be the final in an SE tournament
  if ($tournament->format == KNOCKOUT_BRACKET_SINGLE || $position < $tournament->size) {
    // Return the same round for third place match as the final match
    if ($tournament->third_place == 1  && $position == $tournament->size) {
      $position--;
    }
    return ceil(log($tournament->size/($tournament->size - $position), 2));
  }
  else {
    $divisor = 2 * $tournament->size - ($position + 1);
    // Divisor multiple will equal 0 in the second round of finals in DE bracket
    if ($divisor == 0) {
      // Provides last round plus one and avoids dividing by zero
      return _knockout_get_round_num($tournament, $position - 1) + 1;
    }
    else {
      $base_round = ceil(log($tournament->size/($divisor), 2));
      $round = 2 * $base_round - 1;
      $round_match_count = ($tournament->size)/pow(2,$base_round);
      $fraction = (pow(2,$base_round-1) - 1)/pow(2,$base_round-1);
      $round_position = $position - ($tournament->size + ($tournament->size * $fraction)) + 1;
      $round =  (2 * ($round_position / $round_match_count) > 1 ) ? $round + 1 : $round;
      return ($position == 2 * $tournament->size - 2) ? $round - 1 : $round;
    }
  }
}

/**
 * Loads the next match in a knockout bracket.
 *
 * @param $node
 *   A match node object
 * @param $type
 *   String, 'winner' or 'loser'.
 *
 * @return mixed
 *   The next knockout match node. If no match is found, the position of the
 *   next match is returned instead.
 */
function _knockout_load_next_match($node, $type) {
  $function = $type == 'winner' ? '_knockout_get_winning_position' : '_knockout_get_losing_position';
  $position = $function($node->tournament_node, $node->position);

  // Don't do lookup if resulting match will never be created
  if (empty($position)) {
    return NULL;
  }

  // Load the match node
  $match = knockout_load_match_from_position($node->tournament_node, $position);

  return empty($match) ? $position : $match;
}

/**
 * Creates a knockout match node and inserts relevant data into
 * our database
 *
 * @see match_create_match().
 */
function knockout_create_match($match, $node) {
  // Create match node
  match_create_match($match, $node);

  // Reference match position in bracket
  drupal_write_record('tournament_knockout_match', $match);
}

/**
 * @param $tournament
 *  A tournament node
 * @param $position
 *  The position within the node
 *
 * @return float
 *  The match that the winner of given position will arrive at
 */
function _knockout_get_winning_position($tournament, $position) {
  if ($position < $tournament->size) {
    // Main bracket
    if ($position == $tournament->size - 1) {
      if ($tournament->format == KNOCKOUT_BRACKET_SINGLE) {
        return NULL;
      }
      else {
        return 2 * $position;
      }
    }
    else {
      // Double the match number if it's a win to go to the championship, otherwise it remains in the winners bracket
      return ceil($position / 2) + ($tournament->size / 2);
    }
  }
  elseif ($tournament->format == KNOCKOUT_BRACKET_DOUBLE && $position < 2 * $tournament->size - 2 + $tournament->two_round_final) {
    // Loser bracket or final
    $this_round = _knockout_get_round_num($tournament, $position);
    $divisor_multiplier = ceil($this_round / 2 )  + 1;
    $divisor = pow(2, $divisor_multiplier);
    $matches_in_this_round = ceil($tournament->size / $divisor);

    // For odd numbered rounds we just add the number of matches in this round to the position
    if ($this_round % 2 == 1) {
      return $position +  $matches_in_this_round;
    }
    else {
      // For even numbered rounds we determine what position the match is in the round and add accordingly
      $first_position =  $tournament->size + ($tournament->size * (($divisor / 4) - 1) / ($divisor / 4)) + $matches_in_this_round;
      $order = floor(($position - $first_position) / 2) ;
      return $first_position + $order + $matches_in_this_round;
    }
  }

  return NULL;
}

/**
 * This is used for placing a participant from the winners bracket into the proper losers bracket match
 * @param $tournament
 *  A tournament node
 * @param $position
 *  The position within the node
 *
 * @return float
 *  The match that the loser of given position will arrive at
 */
function _knockout_get_losing_position($tournament, $position) {
  $this_round = _knockout_get_round_num($tournament, $position);
  $corresponding_lb_round = ($this_round == 1) ? 1 : 2 * $this_round - 2;

  $divisor_multiplier = ceil($corresponding_lb_round / 2 )  + 1;
  $divisor = pow(2, $divisor_multiplier);
  $match_count = $tournament->size / $divisor;
  $divisor /= 2;

  $last_position = $tournament->size * ($divisor - 1) / $divisor;
  $last_position -= ($corresponding_lb_round == 1) ? $match_count : 0;

  $positions = range($last_position - $match_count + $tournament->size, $last_position + $tournament->size - 1);

  if ($this_round == 1) {
    return $positions[ceil(($position / 2) -1)];
  }
  elseif (!(($corresponding_lb_round / 2 ) % 2)) {
    $placements = $positions;
  }
  elseif ($tournament->size == 8 && $this_round == 2) {
    $placements = array_reverse($positions);
  }
  else {
    $placements = array();
    while (!empty($positions)) {
      $one = array_pop($positions);
      $two = array_pop($positions);
      // Account for count(positions) == 1.  This occurs for the loser of the last winners bracket match.
      $placements[] = isset($two) ? $two : $one;
      $placements[] = $one;
    }
  }

  $divisor = pow(2, $this_round-1);
  $first_position_in_this_round =  ($tournament->size * ($divisor - 1)/$divisor) + 1;

  return $placements[$position - $first_position_in_this_round];

}

/**
 * Loads a knockout tournament match node given a match position.
 *
 * @param $node
 *   A tournament node object.
 * @param $position
 *   The position of the match we wish to load.
 *
 * @return
 *   A match node object.
 */
function knockout_load_match_from_position($node, $position) {
  $nid = db_select('tournament_knockout_match', 'k')
    ->fields('k', array('nid'))
    ->condition("k.position", $position)
    ->condition('k.tid', $node->nid)
    ->execute()
    ->fetchField();

  return empty($nid) ? NULL : node_load($nid);
}

/**
 * Generates css to be added with each tournament bracket computed by its size
 *
 * @param $rounds
 *  An array of rounds
 * @param format
 *  Type of tournament (i.e. Single or Double elimination)
 *
 * @return string
 *  CSS string of classes to create proper layout of bracket
 */
function knockout_get_tournament_css($node, $rounds) {

  $main_round_count =  count($rounds['main']);
  // computed_css stores calculated properties and values and is turned into a string at the end of the function
  $computed_css = array();

  // Set the width and height of each bracket pane to limit extra space and make entire area dragscrollable
  foreach ($rounds as $type => $data) {
    $computed_css["width: " . ((count($rounds[$type]) * 250) + 10) . "px"][] = '#knockout-bracket .' . $type;
    $round = key($rounds[$type]);
    $computed_css["height: " . ((count($rounds[$type][$round]) * 81) + 35) . "px"][] = '#knockout-bracket .' . $type;
  }

  // p (property) array that will be reused each pass through of the rounds with new calculations
  $p = array(
    '.round-[round] .round-heading',
    '.round-[round] .match',
    '.line-column-[round] .first',
    '.line-column-[round] .bracket-line-top',
    '.line-column-[round] .bracket-line-bottom'
  );

  for ($i = 1; $i <= $main_round_count; $i++) {
    // Do calculations that we'll use as values
    $multiplier = pow(2, ($i-1));
    $heading_margin_bottom =  37.5  * $multiplier - 23;
    $match_margin_bottom = 81 * $multiplier  - 61;
    $margin_top =  37.5 * $multiplier + 42.5;
    $top_height = 37.5 * $multiplier + .5;
    $bottom_height =  43.5 * $multiplier - 4.5;
    $margin_bottom = 81 * $multiplier - .5;

    // Adjust to inexplicable mathematic deviations from BB brackets
    if ($i == 1) {
      $margin_top = 79.5;
      $top_height = 37;
      $bottom_height = 39;
      $match_margin_bottom = 20;
    }

    // CSS values array that is paired with properties array above each round
    $v = array(
      "margin-bottom: " . $heading_margin_bottom . "px",
      "margin-bottom: " . $match_margin_bottom . "px",
      "margin-top: " . $margin_top . "px",
      "height: " . $top_height . "px",
      "margin-bottom: " . $margin_bottom . "px; height: " . $bottom_height . "px"
    );

    foreach ($v as $index => $values) {
      $computed_css[$values][] = "#knockout-bracket " . str_replace('[round]', $i, $p[$index]);
    }

    // Add losers bracket css computations, and exclude last round as it is accounted for in the previous round
    if (isset($rounds['other']) && count($rounds['other']) > 1 && $i < $main_round_count) {
      // Provide straight line css for match connectors
      $r = (int)(2 * $i - 1); // Round number
      $computed_css["margin-bottom: " . (int)(81 * $multiplier - 2) . "px"][] =
        "#knockout-bracket .line-column-" . $r . " .straight-line";
      $computed_css["margin-top: " . (int)(37.5 * $multiplier + 40) . "px"][] =
        "#knockout-bracket .line-column-" . $r . " .straight-line .first";

      // Provide css for round and connectors for round (int)(2 * $i -1)
      foreach ($v as $index => $values) {
        $computed_css[$values][] = "#knockout-bracket .other " . str_replace('[round]', $r, $p[$index]);
      }

      // Provide css for round and connectors for round (int)(2 * $i)
      $r++; // Equivalent to (int)(2 * $i)
      foreach ($v as $index => $values) {
        $computed_css[$values][] = "#knockout-bracket .other " . str_replace('[round]', $r, $p[$index]);
      }

      // If it's second to last round provide css for round and connectors for round (int)(2 * $i +1)
      if ($i == $main_round_count - 1) {
        $r++; // Equivalent to (int)(2 * $i +1)
        foreach ($v as $index => $values) {
          $computed_css[$values][] = "#knockout-bracket .other " . str_replace('[round]', $r, $p[$index]);
        }
      }
    }
  }

  $ret = "";

  // Here we glue together the computed_css array into a string of css to add to the page
  foreach ($computed_css as $values => $properties) {
    $all_properties = implode(",", $properties);
    $ret .= $all_properties . " { " . $values . "; }";
  }

  return $ret;
}