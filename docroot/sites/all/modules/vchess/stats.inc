<?php

/**
 * @file
 * Functions for dealing with player statistics
 */

/**
 * Calculate the number of games won
 */
function vchess_stats_games_won($uid) {
  $sql = "SELECT count(gid) FROM {vchess_games} WHERE " . 
    "(white_uid = '" . $uid . "' AND status = '" . STATUS_WHITE_WIN . ") " .
    "OR";
    "(black_uid = '" . $uid . "' AND status = '" . STATUS_BLACK_WIN . ") ";
  
  $result = db_query($sql);
  $count = $result->fetchColumn();
  
  return $count;
}

/**
 * Calculate the number of games lost
 */
function vchess_stats_games_lost($uid) {
  $sql = "SELECT count(gid) FROM {vchess_games} WHERE " .
      "(white_uid = '" . $uid . "' AND status = '" . STATUS_BLACK_WIN . ") " .
      "OR";
  "(black_uid = '" . $uid . "' AND status = '" . STATUS_WHITE_WIN . ") ";

  $result = db_query($sql);
  $count = $result->fetchColumn();

  return $count;
}

/**
 * Calculate the number of games in progress
 */
function vchess_stats_games_current($uid) {
  $result = db_query("SELECT count(gid) FROM {vchess_games} WHERE status = :status " . 
      "AND (white_uid = :white_uid OR black_uid = :black_uid)" ,
      array(
        ':status' => STATUS_IN_PROGRESS,
        ':white_uid' => $uid,
        ':black_uid' => $uid
      )
  );
  $count = $result->fetchColumn();

  return $count;
}