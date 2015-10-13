<?php
/**
 * @file
 * Page callbacks for nodequeue populator.
 */

/**
 * Ajax callback function that returns subnodeques.
 *
 * @param string $string
 *   The callback string provided by the ajax call.
 *
 * @return string
 *   returns a string of subnodeques formattet as json.
 */
function nodequeue_populator_subnodequeue_autocomplete($string = "") {
  $matches = array();

  if ($string) {
    $query = db_select('nodequeue_subqueue', 's');

    $query->join('nodequeue_queue', 'q', 'q.qid = s.qid');
    $query->fields('s', array('sqid', 'title'));
    $query->fields('q', array('title'));
    $query->condition('s.title', db_like($string) . '%', 'LIKE');

    $result = $query->execute();

    foreach ($result as $subqueue) {
      $matches[$subqueue->title . " [$subqueue->sqid]"] = check_plain($subqueue->title . " (" . $subqueue->q_title . ")");
    }
  }
  drupal_json_output($matches);
}
