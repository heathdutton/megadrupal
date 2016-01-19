<?php
/**
 * @file
 *
 * Provides the object definitions used by the point_spread scoring engine.
 */

/**
 * Definition for point_spread scoring engine.
 */
class pickem_scoring_point_spread implements pickem_scoring_engine_interface {

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
      'key' => PICKEM_TYPE_SPREAD,
      'name' => t('Pick the winner with point spread'),
      'description' => t('Pick the winner of each game based on point spread.'),
      'features' => array('pick'=>1, 'pick_score'=>0, 'with_spread'=>1, 'with_last_weekly_score'=>0, 'with_confidence'=>0),
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
  function update_totals(&$totals=FALSE, &$row=FALSE) {

    // They didn't pick.
    if (empty($row->winnerpick_id)) {
      return;
    }

    // Update the pick counts.
    $totals[$row->uid]['picked_games']++;
    $totals[$row->uid]['weeks'][$row->wid]['picked_games']++;

    if (isset($row->h_score) && isset($row->v_score)) {
      $totals[$row->uid]['weeks'][$row->wid]['games'][$row->gid] = 0;

      // They picked the home winner.
      if ( ($row->h_score + $row->h_spread) > $row->v_score && $row->winnerpick_id == $row->h_id) {
        $totals[$row->uid]['weeks'][$row->wid]['games'][$row->gid] = $row->wk_points;
        $totals[$row->uid]['weeks'][$row->wid]['total'] += $row->wk_points;
        $totals[$row->uid]['total'] += $row->wk_points;
      }

      // They picked the visitor winner.
      elseif ( ($row->v_score + $row->v_spread) > $row->h_score && $row->winnerpick_id == $row->v_id) {
        $totals[$row->uid]['weeks'][$row->wid]['games'][$row->gid] = $row->wk_points;
        $totals[$row->uid]['weeks'][$row->wid]['total'] += $row->wk_points;
        $totals[$row->uid]['total'] += $row->wk_points;
      }

      // Tie
      elseif ( ($row->v_score + $row->v_spread) == $row->h_score && $row->h_score <> "") {
        $totals[$row->uid]['weeks'][$row->wid]['games'][$row->gid] = $row->wk_points;
        $totals[$row->uid]['weeks'][$row->wid]['total'] += $row->wk_points;
        $totals[$row->uid]['total'] += $row->wk_points;
      }
    }

  }


  function update_totals_final(&$totals=FALSE) {

  }
}