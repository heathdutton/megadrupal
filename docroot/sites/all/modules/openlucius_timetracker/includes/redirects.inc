<?php
/**
 * @file
 * This file contains function for redirecting for the timetracker.
 */

/**
 * Function to redirect to own reports page.
 */
function openlucius_timetracker_my_time() {
  global $user;

  drupal_goto('reports', array(
    'query' => array(
      'user' => $user->uid,
    ),
  ));
}
