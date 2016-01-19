<?php

define('INITIAL_RATING', 1200);
define('MIN_RATED_GAMES', 5);
define('RATING_CHANGE_MULTIPLIER', 25);

/**
 * @file
 * Functions to apply game results to rating according to ELO's formula
 */

/**
 * Compute initial rating based on average opponent strength
 */
// function vchess_get_initial_rating($uid) {
//   $player = new Player($uid);

//   if ($player->played() == 0) {
//     $rating = INITIAL_RATING;
//   }

// //  $rating = $avg_opp_rating + 700 * (($wins + 0.5 * $draws) / $played -0.5) * $played / ($played + 2);

//   return $rating;
// }

/**
 * Get probability for player to win from difference in rating
 *
 * @param $diff
 *   Difference between two players ratings
 *
 * @return
 *   Probability, between 0 and 1, that the player would win
 */
function gamer_get_win_probability($diff) {
  $absdiff = abs($diff);
  if ($diff > 735) {
    $probability = 1;
  }
  elseif ($diff < -735) {
    $probability = 0;
  }
  else {
    $probability = 0.5
      + 1.4217 * 0.001 * $diff
      - 2.4336 * 0.0000001 * $diff * $absdiff
      - 2.5140 * 0.000000001 * $diff * $absdiff * $absdiff
      + 1.9910 * 0.000000000001 * $diff * $absdiff * $absdiff * $absdiff;
  }

  return $probability;
}

/**
 * Get rating change multiplier (coefficient K) which is used to scale expected propability before
 * updating the rating.
 */
function gamer_get_rating_change_multiplier() {
//   if ($rating < 2000) {
//     $K = 30;
//   }
//   elseif ($rating > 2400) {
//     $K = 10;
//   }
//   else {
//     $K = 130 -$rating / 20;
//   }
//   return $K;

  return RATING_CHANGE_MULTIPLIER;
}

/**
 * Update stats to note that another game is in progress
 *
 * @param $uid1
 *   The user id of the first player
 * @param $uid2
 *   The user id of the second player
 */
function gamer_add_in_progress($uid1, $uid2) {
  // Load stats. Is always successful (returns zero array if not found).
  $player1_stats = gamer_load_user_stats($uid1);
  $player2_stats = gamer_load_user_stats($uid2);

//  @todo: rewrite gamer model to be more OO and then finish this function!

// Get the new stats
//   $winner_stats_new = gamer_update_stats($winner->uid, $winner_stats, $loser_stats, 1);
//   $loser_stats_new = gamer_update_stats($loser->uid, $loser_stats, $winner_stats, 0);

//   // Save changes
//   gamer_save_user_stats($winner->uid, $winner_stats_new);
//   gamer_save_user_stats($loser->uid, $loser_stats_new);
}

