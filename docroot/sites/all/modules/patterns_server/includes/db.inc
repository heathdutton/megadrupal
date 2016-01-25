<?php

/**
 * @file
 * all function operating db in patters_server module.
 */

/**
 * latest patterns
 */
function patterns_server_get_pattern_timeDESC_from_db($num) {
  $num = is_numeric($num)?$num:10;
  $result = db_select('patterns', 'p')
                  ->fields('p', array())
                  ->orderBy('updated', 'DESC')
                  ->range(0, $num)
                  ->execute()
                  ->fetchAll();
  return $result;
}

/**
 * one pattern defined by pid.
 */
function patterns_server_get_pattern_pid_from_db($pid) {
  $result = db_select('patterns', 'p')
                  ->fields('p', array())
                  ->condition('p.pid', $pid)
                  ->execute()
                  ->fetchAll();
  return $result;
}

/**
 * Used by search page.
 */
function patterns_server_search_pattern($key, $type) {
  $or = db_or();
  switch ($type) {
    case 0:
      $or->condition('title', '%' . db_like($key) . '%', 'LIKE');
      break;
    case 1:
      $or->condition('description', '%' . db_like($key) . '%', 'LIKE');
      break;
    case 2:
      $or->condition('author', '%' . db_like($key) . '%', 'LIKE');
      break;
    case 3:
      $or->condition('uuuid', '%' . db_like($key) . '%', 'LIKE');
      break;
    case 4:
      $or->condition('pid', $key, '=');
      break;
  }
  $result = db_select('patterns', 'p')
                  ->fields('p', array())
                  ->condition($or)
                  ->orderBy('updated', 'DESC')
                  ->execute()
                  ->fetchAll();
  $patterns = array();
  foreach ($result as $pattern) {
    $patterns[$pattern->pid] = $pattern;
    $data = unserialize($pattern->pattern);
    $patterns[$pattern->pid]->pattern = $data;
    $patterns[$pattern->pid]->info = $data['info'];
  }
  return $patterns;
}

/**
 * Use by patterns server page.
 */
function patterns_server_get_pattern_from_db($numorid, $sign = TRUE) {
  if ($sign == TRUE) {
    $result = patterns_server_get_pattern_timeDESC_from_db($numorid);
  }
  else {
    $result = patterns_server_get_pattern_pid_from_db($numorid);
  }
  $patterns = array();

  foreach ($result as $pattern) {
    $patterns[$pattern->pid] = $pattern;
    $data = unserialize($pattern->pattern);
    $patterns[$pattern->pid]->pattern = $data;
    $patterns[$pattern->pid]->info = $data['info'];
  }

  return $patterns;
}
