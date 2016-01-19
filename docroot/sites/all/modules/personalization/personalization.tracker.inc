<?php
/**
 * @file
 * This file contains functionality related to the end user and their
 * individual content scores.
 */

/**
 * Checks if we should be tracking this user.
 *
 * @return bool
 *   Whether or not the user should be tracked.
 */
function personalization_trackable() {
  return user_access('use personalization');
}

/**
 * Record the terms of the page the user has accessed.
 */
function personalization_track_page($nid = NULL) {
  $settings = variable_get('personalization_settings', array());

  // Only track if vocabs have been selected.
  if ($settings['pz_vocabularies']) {
    if (!$nid || !is_numeric($nid)) {
      $n = menu_get_object();
      if ($n) {
        $nid = $n->nid;
      }
      else {
        $nid = NULL;
      }
    }

    if ($nid) {
      // Get vid of location vocabulary if we're tracking geo.
      if (array_filter($settings['pz_geo'])) {
        $geo_v = taxonomy_vocabulary_machine_name_load(PERSONALIZATION_GEO_VOCABULARY_NAME);
        $settings['pz_vocabularies'][$geo_v->vid] = $geo_v->vid;
      }

      // Does this node have any terms from the trackable vocabularies?
      $query = db_select('taxonomy_term_data', 'ttd');
      $query->leftJoin('taxonomy_index', 'ti', 'ttd.tid = ti.tid');
      $query->fields('ttd', array(
        'tid',
        'vid',
      ));

      $query->condition('ttd.vid', $settings['pz_vocabularies'], 'IN');
      $query->condition('ti.nid', $nid, '=');
      $result = $query->execute()->fetchAll();

      if ($result) {
        // Send the terms to the scoring function.
        personalization_update_scores($result);
      }
    }
  }
}

/**
 * Updates the user's scores with for the terms supplied.
 *
 * @param array $terms
 *   The terms that need to have their scores updated.
 */
function personalization_update_scores($terms, $type = 'page') {
  $scores = array();
  $tids = array();
  $uid = personalization_get_user();
  $settings = variable_get('personalization_settings', array());

  $weight = 0;
  $pages = 0;
  $searches = 0;

  // Get the users current scores so we can increase them where appropriate.
  $result = db_query('SELECT user, tid FROM {personalization_user_scores} WHERE user = :user', array(':user' => $uid))->fetchAll();

  // Loop through the current scores and add them to a new array with a
  // concatinated key for quick lookup and update later.
  foreach ($result as $s) {
    $scores[$s->user . ':' . $s->tid] = (array) $s;
    $tids[] = $s->tid;
  }

  // Loop through the terms supplied so we can score them.
  foreach ($terms as $row) {
    if (is_object($row)) {
      // Scoring a page access.
      if ($type == 'page') {
        // Get vid of location vocabulary if we're tracking geo pages.
        if (array_filter($settings['pz_geo'])) {
          $geo_v = taxonomy_vocabulary_machine_name_load(PERSONALIZATION_GEO_VOCABULARY_NAME);
          $settings['pz_vocabulary_weights'][$geo_v->vid] = $settings['pz_geo_vocab_weight'];
        }
        $weight = isset($settings['pz_vocabulary_weights'][$row->vid]) ? $settings['pz_vocabulary_weights'][$row->vid] : 0;
        $pages = 1;
      }
      // Scoring search terms.
      elseif ($type == 'search') {
        $weight = $settings['pz_search_weight'];
        $searches = 1;
      }

      // The user already has a score for this term.
      if (isset($scores[$uid . ':' . $row->tid])) {
        // Update only the changing columns.
        $scores[$uid . ':' . $row->tid]['pages'] += $pages;
        $scores[$uid . ':' . $row->tid]['searches'] += $searches;
        $scores[$uid . ':' . $row->tid]['score'] += $weight;
        $scores[$uid . ':' . $row->tid]['changed'] = REQUEST_TIME;
      }
      // The user hasn't scored this term yet.
      else {
        $scores[$uid . ':' . $row->tid] = array(
          'user' => $uid,
          'vid' => $row->vid,
          'tid' => $row->tid,
          'pages' => $pages,
          'searches' => $searches,
          'location' => 0,
          'score' => $weight,
          'changed' => REQUEST_TIME,
        );
      }
    }
  }

  // Need to delete before INSERT as db_merge does not support multiple records.
  if ($tids) {
    db_delete('personalization_user_scores')->condition('user', $uid, '=')->condition('tid', $tids, 'IN')->execute();
  }

  $insert = db_insert('personalization_user_scores')->fields(array(
    'user',
    'vid',
    'tid',
    'pages',
    'searches',
    'location',
    'score',
    'changed',
  ));

  // Add all the new term scores to one insert query.
  foreach ($scores as $s) {
    $insert->values($s);
  }

  $insert->execute();
}

/**
 * Score terms based on an internal site search.
 *
 * Scores terms for the user where their search keywords from an internal site
 * search match one of the admin defined mappings.
 *
 * @param string $text
 *   The search term.
 *
 * @return string
 *   The same search term after we've recorded it.
 */
function personalization_track_search($text) {
  // Get all the mappings for this search term.
  $result = db_query('SELECT tid, vid FROM {personalization_keyword_mapping} WHERE keyword  = :keyword', array(':keyword' => trim($text)))->fetchAll();

  if ($result) {
    // Send the mapped terms to the scoring function.
    personalization_update_scores($result, 'search');
  }

  return $text;
}

/**
 * Score terms based on an external search.
 *
 * Scores terms for the user where their search keywords from a Google or Bing
 * search match one of the admin defined mappings.
 */
function personalization_track_external_search() {
  $keywords = array();
  $q = array();

  $referer = parse_url($_SERVER['HTTP_REFERER']);
  if (isset($referer['host']) && (stristr($referer['host'], 'google.') || stristr($referer['host'], 'bing.'))) {
    if (isset($referer['query'])) {
      parse_str($referer['query'], $q);
    }
    elseif (isset($referer['fragment'])) {
      parse_str($referer['fragment'], $q);
    }

    if (isset($q['q'])) {
      $keywords = explode(' ', $q['q']);
    }

    foreach ($keywords as $k) {
      personalization_track_search($k);
    }
  }
}

/**
 * Deletes all expired scores. This function is called by cron.
 */
function personalization_expire_scores() {
  $settings = variable_get('personalization_settings', array());
  db_delete('personalization_user_scores')->condition('changed', time() - $settings['pz_user_score_ttl'], '<')->execute();
}

/**
 * Returns a list nodes relevant to the user's behaviour.
 *
 * Returns a list of nodes which have been scored based on the users
 * interactions with the site. The list is sorted descending so that the most
 * relevant content is first.
 *
 * @param int $limit
 *   The number of items to return.
 * @param int $offset
 *   The offset for the query.
 * @param bool $matches_only
 *   Return only matched content rather than all content.
 * @param bool $paginate
 *   Should the list be paginated?
 *
 * @return array
 *   The array of matched nodes.
 */
function personalization_match_content($limit = 10, $offset = 0, $matches_only = FALSE, $paginate = FALSE) {
  $uid = personalization_get_user();
  $settings = variable_get('personalization_settings', array());
  $content = &drupal_static(__FUNCTION__);

  // Create a cache key so that we're only caching for this exact query.
  $cache_key = $limit . ':' . $offset . ':' . ($matches_only ? 1 : 0) . ':' . ($paginate ? 1 : 0);

  // Has this function already been called and we have a static cache?
  if (!isset($content) || !isset($content[$cache_key])) {
    // Does the user have their results cached in the database?
    $cache = cache_get('content:' . $uid . ':' . $cache_key, 'cache_personalization');
    if ($cache && property_exists($cache, 'data') && $cache->data) {
      $content = array($cache_key => $cache->data);
    }
    else {
      // No cache find so let's do the heavy lifting.
      // Start our select query by selecting the tables and creating a SUM().
      $query = db_select('node', 'n');
      $query->leftJoin('taxonomy_index', 'ti', "n.nid = ti.nid");
      $query->leftJoin('personalization_user_scores', 'pzs', "ti.tid = pzs.tid");
      $query->addExpression('SUM(pzs.score)', 'score');
      $query->fields('n', array(
        'nid',
        'title',
        'type',
      ))
      ->isNotNull('n.nid');

      // Only get this user's matches.
      $query->condition('pzs.user', $uid, '=');

      // If we only want content that matches the user's behaviour we need to
      // check the match is NOT NULL.
      if ($matches_only) {
        $query->isNotNull('pzs.score')->condition('pzs.score', 0, '>');
      }

      // Group the results by nid so the scores are combined and ensure the
      // user has access to this content.
      $query->groupBy('n.nid')->orderBy('score', 'DESC')->addTag('node_access');

      if ($paginate) {
        // Extend the query with the Drupal pager if requested.
        $result = $query->extend('PagerDefault')->limit((int) $limit)->execute();
        if ($result) {
          $content[$cache_key] = $result->fetchAllAssoc('nid');
        }
        else {
          $content[$cache_key] = array();
        }
      }
      else {
        // No pagination required, just need to return the lot.
        $query->range((int) $offset, (int) $limit);
        $content[$cache_key] = $query->execute()->fetchAllAssoc('nid');
      }

      // Cache the results of this query for next time.
      $ttl = REQUEST_TIME + ($settings['pz_user_content_ttl'] * 60);
      cache_set('content:' . $uid . ':' . $cache_key, $content[$cache_key], 'cache_personalization', $ttl);
    }
  }

  if (isset($content[$cache_key])) {
    return $content[$cache_key];
  }
  return array();
}

/**
 * Returns the personalisation user ID.
 *
 * @return Int
 *   Get the user id (generated if anonymous).
 */
function personalization_get_user() {
  $settings = variable_get('personalization_settings', array());

  $uid = NULL;

  if (user_is_logged_in()) {
    global $user;
    $uid = $user->uid;
  }
  else {
    if (isset($_COOKIE['pz_user_id']) && strlen($_COOKIE['pz_user_id']) == 23) {
      $uid = $_COOKIE['pz_user_id'];
    }
    else {
      $uid = uniqid(NULL, TRUE);
    }
  }

  if ($uid) {
    setcookie('pz_user_id', $uid, time() + ($settings['pz_user_score_ttl'] * 60));
  }

  return $uid;
}
