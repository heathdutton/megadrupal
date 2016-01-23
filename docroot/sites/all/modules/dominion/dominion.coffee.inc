<?php
/**
 * @file
 * Implements the hook_coffee_command to extend Coffee.
 */

/**
 * Implements hook_coffee_command().
 */
function dominion_coffee_command($op) {
  switch ($op) {
    case 'dom':

      $query = db_select('menu_router', 'mr');
      $query->fields('mr', array('title', 'path'));
      $query->condition('mr.path', '%dominion%', 'LIKE');
      $query->condition('mr.path', '%\%%', 'NOT LIKE');
      $query->condition('mr.path', '%autocomplete%', 'NOT LIKE');
      $query->condition('mr.title', "", 'NOT LIKE');
      $query->orderBy('mr.path', 'DESC');
      $result = $query->execute();

      $values = $result->fetchAll();

      foreach ($values as $value) {
        $return[] = array(
          'title' => $value->title,
          'path' => $value->path,
        );
      }

      break;
  }
  return $return;

}