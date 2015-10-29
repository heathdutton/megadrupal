<?php
/**
 * @file
 * Provide user tid argument handler.
 */

/**
 * Argument handler to accept a team id.
 *
 * @ingroup views_argument_handlers
 */
class team_handler_argument_team_tid extends views_handler_argument_numeric {
  /**
   * Override the behavior of title(). Get the name of the team.
   *
   * @return array
   *    A list of team names.
   */
  function title_query() {
    $result = db_select('teams')
      ->fields('teams', array('name'))
      ->condition('tid', $this->value)
      ->execute();

    $titles = array();
    foreach ($result as $record) {
      $titles[] = check_plain($record->name);
    }

    return $titles;
  }
}
