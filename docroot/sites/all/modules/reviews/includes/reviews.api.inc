<?php

/**
 * @file
 * This file contains API functions for the reviews system
 */

/**
 * Helper function returns TRUE or FALSE based on whether the current user
 * has reviewed the node being displayed. If admin setting is set to allow
 * multiple reviews from a single user then we return FALSE immediately.
 */
function reviews_check_user_review($nid, $uid) {
  if (variable_get('reviews_allow_multiple', 0)) {
    return FALSE;
  }

  $results = db_select('reviews', 'r')
    ->fields('r')
    ->condition('uid', $uid, '=')
    ->condition('nid', $nid, '=')
    ->execute()
    ->fetchAll();
  if (count($results) == 0) {
    return FALSE;
  }
  else {
    return TRUE;
  }
}

/**
 * Helper function return TRUE or FALSE based on whether reviews are enabled.
 */
function reviews_reviews_enabled() {
  if (variable_get('reviews_enabled', 0) == 0) {
    return FALSE;
  }
  else {
    return TRUE;
  }
}

/**
 * Helper function to return the number of approved reviews for the
 * current node.
 */
function reviews_reviews_count($nid) {
  $results = db_select('reviews', 'r')
    ->fields('r')
    ->condition('nid', $nid, '=')
    ->condition('status', 1, '=')
    ->execute()
    ->fetchAll();
  return count($results);
}

/**
 * Helper function to return the number of pending reviews for the
 * current node.
 */
function reviews_reviews_pending_count($nid) {
  $results = db_select('reviews', 'r')
    ->fields('r')
    ->condition('nid', $nid, '=')
    ->condition('status', 0, '=')
    ->execute()
    ->fetchAll();
  return count($results);
}

/**
 * Helper function to return a username from a user ID.
 */
function reviews_get_username($uid) {
  $user = user_load($uid);
  if ($user) {
    return $user->name;
  }
  else {
    return 'Anonymous';
  }
}

/**
 * Helper function to return a node title from a node ID.
 */
function reviews_get_node_title($nid) {
  $node = node_load($nid);
  if ($node) {
    return $node->title;
  }
  else {
    return 'Unknown';
  }
}

/**
 * Helper function to return the review status name form the ID.
 */
function reviews_get_review_status($sid) {
  $statuses = array(
    0 => '<span class="marker">' . t('Awaiting moderation') . '</span>',
    1 => t('Published'),
    2 => t('Rejected'),
  );
  return $statuses[$sid];
}

/**
 * Helper function to return an array of available content types.
 */
function reviews_get_node_types() {
  $types = node_type_get_types();
  $tmp = array();
  foreach ($types as $key => $type) {
    $tmp[$key] = $type->name;
  }
  return $tmp;
}

/**
 * Helper function to return the nid from the reviews ID.
 */
function reviews_get_nid_from_rid($rid) {
  $result = db_select('reviews', 'r')
    ->fields('r', array('nid'))
    ->condition('rid', $rid, '=')
    ->execute()
    ->fetchAll();

  if (count($result) == 0) {
    return FALSE;
  }
  else {
    return $result[0]->nid;
  }
}

/**
 * Helper function to update the review count table for a node.
 */
function reviews_update_review_count_for_node($nid) {
  db_delete('reviews_count')
    ->condition('nid', $nid, '=')
    ->execute();
  $result = db_select('reviews', 'r')
    ->fields('r', array('nid'))
    ->condition('nid', $nid, '=')
    ->execute()
    ->fetchAll();
  $result2 = db_insert('reviews_count')
    ->fields(array(
      'nid' => $nid,
      'count' => count($result),
    ))
    ->execute();
}

/**
 * Helper function to load a review into an object.
 */
function reviews_load($rid) {
  $review = db_select('reviews', 'r')
    ->fields('r')
    ->condition('rid', $rid, '=')
    ->execute()
    ->fetchAll();

  if (count($review) != 0) {
    $tmp = unserialize($review[0]->review);
    $rvw = new stdClass();
    $rvw->rid = $rid;
    $rvw->nid = $review[0]->nid;
    $rvw->uid = $review[0]->uid;
    $rvw->review = array(
      'value' => $tmp['value'],
      'format' => $tmp['format'],
    );
    $rvw->rating = $review[0]->rating;
    $rvw->created = $review[0]->created;
    return $rvw;
  }
  else {
    return new stdClass();
  }
}

function reviews_get_title($nid) {
  return t('Reviews (!count)', array('!count' => reviews_reviews_count($nid)));
}
