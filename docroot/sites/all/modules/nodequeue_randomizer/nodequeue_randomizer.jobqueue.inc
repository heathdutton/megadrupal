<?php // $Id$
/**
 * @file nodequeue_randomizer.jobqueue.inc
 *
 * @author Greg Field
 * @since Dec 1st, 2009
 **/

/**
 * Takes the specified subqueue and randomizes the order of nodes within it using the node's weightings
 *
 * @param $sqid
 *   Integer identifier for a subqueue of the specified nodequeue
 * @param $last_run
 *   Integer timestamp of the last time the queue was randomized
 * @param $period
 *   Integer value representing the period between randomizations in seconds
 */
function nodequeue_randomizer_randomize_queue($sqid, $last_run, $period) {
  $results = db_query(
    "SELECT nid, weight
    FROM {nodequeue_nodes}
    WHERE sqid = %d",
    $sqid
  );

  $range = 0;
  $nids = array();
  $end_nids = array(); //Nodes with weight of 0 are set aside and stuck at the back of the queue
  while ($node_info = db_fetch_object($results)) {
    if ($node_info->weight) {
      $nids[$node_info->nid] = $node_info->weight;
      $range = $range + $node_info->weight;
    }
    else {
      $end_nids[$node_info->nid] = 1;
    }
  }

  $nids = (count($nids) == 0) ? array() : nodequeue_randomizer_weighted_randomize($nids, $range);
  $end_nids = (count($end_nids) == 0) ? array() : nodequeue_randomizer_weighted_randomize($end_nids, count($end_nids));

  $node_list = array_merge($nids, $end_nids);
  $position = 1;
  foreach ($node_list as $nid) {
    db_query(
      "UPDATE {nodequeue_nodes}
      SET position = %d
      WHERE sqid = %d AND nid = %d",
      $position, $sqid, $nid
    );

    $position++;
  }

  db_query(
    "UPDATE {nodequeue_randomizer}
    SET last_run = %d
    WHERE sqid = %d",
    $last_run + $period, $sqid
  );
}

/**
 * Given an array of weighted items and the range, returns the array in randomized order
 *
 * @param $nids
 *   An array keyed by the nids and having the weights for each node as values
 * @param $range
 *   The sum of the weights of all nids in the given array

 * @return
 *   An randomly ordered array with the nids as values
 */
function nodequeue_randomizer_weighted_randomize($nids, $range) {
  if (count($nids) == 1) {
    return array(key($nids));
  }

  reset($nids);
  $selection = rand(1, $range);
  $count = 0;
  while ($selection > current($nids) + $count) {
    $count = current($nids) + $count;
    next($nids);
  }

  $range = $range - current($nids);
  $nid = key($nids);
  unset($nids[$nid]);

  return array_merge(array($nid), nodequeue_randomizer_weighted_randomize($nids, $range));
}
