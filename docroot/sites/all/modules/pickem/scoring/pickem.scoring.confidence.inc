<?php
/**
 * @file
 *
 * Provides the object definitions used by the confidence scoring engine.
 */

/**
 * Definition for confidence scoring engine.
 */
class pickem_scoring_confidence implements pickem_scoring_engine_interface {

  // Properties
  private $props = array();

  // Magic overload method.
  public function __get($name) {
    if (array_key_exists($name, $this->props)) {
      return $this->props[$name];
    }
    trigger_error('Undefined property via __get(): ' . $name, E_USER_NOTICE);
    return NULL;
  }

  // Constructor
  public function __construct() {
    $this->props = array(
      'key' => PICKEM_TYPE_CONFIDENCE,
      'name' => t('Pick the winner with a confidence score'),
      'description' => t('Simply pick the winner of each game and assign a confidence point value.  Assign each game below a value starting at 1 and ending with the number of games that week.  Each number must be used once and only once.  Give a 1 to the game you are least confident about and the maximum value to the game you expect to be a blowout.  If you win the game, you get the points.'),
      'features' => array('pick'=>1, 'pick_score'=>0, 'with_spread'=>0, 'with_last_weekly_score'=>0, 'with_confidence'=>1),
    );
  }

  /**
   * Calculate user totals for this scoring system.
   *
   * @param $totals
   *   A deep array to store user totals (from get_user_totals() in pickem.inc)
   * @param $row
   *   A row object consisting of game and pick data (from get_user_totals() in pickem.inc)
   * @return
   *   NULL
   */
  function update_totals(&$totals=FALSE, &$game=FALSE) {

    // They didn't pick.
    if (empty($game->winnerpick_id)) {
      return;
    }

    // Update the pick counts.
    $totals[$game->uid]['picked_games']++;
    $totals[$game->uid]['weeks'][$game->wid]['picked_games']++;

    if (isset($game->h_score) && isset($game->v_score)) {
      $totals[$game->uid]['weeks'][$game->wid]['games'][$game->gid] = 0;

      // They picked the home winner.
      if ($game->h_score > $game->v_score && $game->winnerpick_id == $game->h_id) {
        $totals[$game->uid]['weeks'][$game->wid]['games'][$game->gid] = $game->confidence;
        $totals[$game->uid]['weeks'][$game->wid]['total'] += $game->confidence;
        $totals[$game->uid]['total'] += $game->confidence;
      }

      // They picked the visitor winner.
      else if ($game->v_score > $game->h_score && $game->winnerpick_id == $game->v_id) {
        $totals[$game->uid]['weeks'][$game->wid]['games'][$game->gid] = $game->confidence;
        $totals[$game->uid]['weeks'][$game->wid]['total'] += $game->confidence;
        $totals[$game->uid]['total'] += $game->confidence;
      }

      // Tie
      else if ($game->v_score == $game->h_score && $game->h_score <> "") {
        $totals[$game->uid]['weeks'][$game->wid]['games'][$game->gid] = $game->confidence;
        $totals[$game->uid]['weeks'][$game->wid]['total'] += $game->confidence;
        $totals[$game->uid]['total'] += $game->confidence;
      }
    }
  }

  function update_totals_final(&$totals=FALSE) {

  }

}



