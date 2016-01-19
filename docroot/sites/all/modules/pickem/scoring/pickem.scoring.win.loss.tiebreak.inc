<?php
/**
 * @file
 *
 * Provides the object definitions used by the win_loss_tiebreak scoring engine.
 */

/**
 * Definition for win_loss_tiebreak scoring engine.
 */
class pickem_scoring_win_loss_tiebreak implements pickem_scoring_engine_interface {

  // Properties
  private $props = array();

  // Magic overload method.
  public function __get($name) {
    if (array_key_exists($name, $this->props)) {
      return $this->props[$name];
    }
    trigger_error(check_plain('Undefined property via __get(): ' . $name), E_USER_NOTICE);
    return NULL;
  }

  // Constructor
  public function __construct() {
    $this->props = array(
      'key' => PICKEM_TYPE_WIN_LOSS_TIEBREAK,
      'name' => t('Pick the winner with tie-breaker'),
      'description' => t('Pick the winner of each game and pick the score for the last game to break weekly ties.'),
      'features' => array('pick'=>1, 'pick_score'=>0, 'with_spread'=>0, 'with_last_weekly_score'=>1, 'with_confidence'=>0),
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
    $totals[$game->uid]['weeks'][$game->wid]['games'][$game->gid] = 0;

    // If it is the last game of the week, use the score picks.
    if ( is_last_game_of_week(array('gid' => $game->gid, 'wid' => $game->wid)) ) {

      // They didn't pick, although there shouldn't be a record in the database then.
      if (!isset($game->pick_h_score) || !isset($game->pick_v_score)) {
        return;
      }

      if (!isset($totals[$game->uid]['tiebreak_pts'])) {
        $totals[$game->uid]['tiebreak_pts'] = 0;
      }

      // Update the pick counts.
      $totals[$game->uid]['picked_games']++;
      $totals[$game->uid]['weeks'][$game->wid]['picked_games']++;

      // Picked the correct winner or tie.
      if ($game->h_score > $game->v_score && $game->pick_h_score > $game->pick_v_score ||
          $game->v_score > $game->h_score && $game->pick_v_score > $game->pick_h_score ||
          $game->v_score == $game->h_score && $game->pick_v_score == $game->pick_h_score) {
        $totals[$game->uid]['weeks'][$game->wid]['games'][$game->gid] = $game->wk_points;
        $totals[$game->uid]['weeks'][$game->wid]['total'] += $game->wk_points;
        $totals[$game->uid]['total'] += $game->wk_points;

        $totals[$game->uid]['tiebreak_pts'] += 0;  // Completeness sake.
        $totals[$game->uid]['weeks'][$game->wid]['tiebreak_pts'] = 0;
      }
      else {
        $totals[$game->uid]['tiebreak_pts'] += 10000;
        $totals[$game->uid]['weeks'][$game->wid]['tiebreak_pts'] = 10000;
      }

      // Tiebreak calculation = (Actual Points - Picked Points) + 10000 for picking the wrong winner.
      // LOWER is better.
      $score_total_pts = $game->h_score + $game->v_score;
      $pick_total_pts = $game->pick_h_score + $game->pick_v_score;
      $totals[$game->uid]['tiebreak_pts'] += abs($score_total_pts - $pick_total_pts);
      $totals[$game->uid]['weeks'][$game->wid]['tiebreak_pts'] += abs($score_total_pts - $pick_total_pts);
    }

    // If it is not the last game, use normal pick the winner id, and don't use the pick scores at all.
    else {

      // They didn't pick, although there shouldn't be a record in the database then.
      if (empty($game->winnerpick_id)) {
        return;
      }

      // Update the pick counts.
      $totals[$game->uid]['picked_games']++;
      $totals[$game->uid]['weeks'][$game->wid]['picked_games']++;

      if (isset($game->h_score) && isset($game->v_score)) {
        $totals[$game->uid]['weeks'][$game->wid]['games'][$game->gid] = 0;

        // Correctly picked home winner.
        if ($game->h_score > $game->v_score && $game->winnerpick_id == $game->h_id) {
          $totals[$game->uid]['weeks'][$game->wid]['games'][$game->gid] = $game->wk_points;
          $totals[$game->uid]['weeks'][$game->wid]['total'] += $game->wk_points;
          $totals[$game->uid]['total'] += $game->wk_points;
        }

        // Correctly picked visitor winner.
        elseif ($game->v_score > $game->h_score && $game->winnerpick_id == $game->v_id) {
          $totals[$game->uid]['weeks'][$game->wid]['games'][$game->gid] = $game->wk_points;
          $totals[$game->uid]['weeks'][$game->wid]['total'] += $game->wk_points;
          $totals[$game->uid]['total'] += $game->wk_points;
        }

        // Tie.
        elseif ($game->v_score == $game->h_score && $game->h_score <> "") {
          $totals[$game->uid]['weeks'][$game->wid]['games'][$game->gid] = $game->wk_points;
          $totals[$game->uid]['weeks'][$game->wid]['total'] += $game->wk_points;
          $totals[$game->uid]['total'] += $game->wk_points;
        }
      }
    }
  }


  /**
   * After all user totals are calculated, this function is called to wrap-up any extra calculations.
   *
   * @param $totals
   *   A deep array to store user totals (from get_user_totals() in pickem.inc)
   * @return
   *   NULL
   */
  function update_totals_final(&$totals=FALSE) {

  }

}
