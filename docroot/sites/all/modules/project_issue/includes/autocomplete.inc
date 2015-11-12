<?php


/**
 * @file
 * Autocomplete callback functions for the Project issue tracking module.
 *
 * Each function returns a JSON string for use with JS autocomplete fields.
 */

/**
 * Return valid issue-enabled project names for comma-separated input.
 */
function project_issue_autocomplete_issue_project($string = '') {
  $matches = array();

  // The user enters a comma-separated list of projects. We only autocomplete
  // the last one.
  $array = drupal_explode_tags($string);
  $last_string = trim(array_pop($array));

  if ($last_string != '') {
    $query = new EntityFieldQuery;
    $result = $query
      // Projects are currently restricted to nodes!
      ->entityCondition('entity_type', 'node')
      // If there is a field_project_type, this is a project.
      // This part is what we care about.
      ->fieldCondition('field_project_type', 'value', NULL, 'IS NOT NULL')
      ->fieldCondition('field_project_has_issue_queue', 'value', 1)
      ->propertyCondition('title', '%' . db_like($last_string) . '%', 'like')
      ->range(0, 10)
      ->execute();

    $prefix = count($array) ? implode(', ', $array) .', ' : '';
    $projects = isset($result['node']) ? $result['node'] : array();
    $projects = node_load_multiple(array_keys($projects));
    foreach ($projects as $project) {
      $title = $project->title;
      // Commas and quotes in terms are special cases, so encode 'em.
      if (strpos($title, ',') !== FALSE || strpos($title, '"') !== FALSE) {
        $title = '"'. str_replace('"', '""', $project->title) .'"';
      }
      $matches[$prefix . $title] = check_plain($project->title);
    }
  }

  drupal_json_output($matches);
}


/**
 * Return valid issue-enabled project names based on a user's own projects.
 *
 * Only returns matches for project titles from issues the user has either
 * submitted or commented on.
 */
function project_issue_autocomplete_user_issue_project($uid, $string = '') {
  $matches = array();

  // The user enters a comma-separated list of projects. We only autocomplete
  // the last one.
  $array = drupal_explode_tags($string);
  $last_string = trim(array_pop($array));

  if ($last_string != '') {
    $query = new EntityFieldQuery;
    $result = $query
      // Projects are currently restricted to nodes!
      ->entityCondition('entity_type', 'node')
      // If there is a field_project_type, this is a project.
      // This part is what we care about.
      ->fieldCondition('field_project_type', 'value', NULL, 'IS NOT NULL')
      ->fieldCondition('field_project_has_issue_queue', 'value', 1)
      ->propertyCondition('title', '%' . db_like($last_string) . '%', 'like')
      //->range(0, 10)
      ->execute();

    $prefix = count($array) ? implode(', ', $array) .', ' : '';
    $projects = isset($result['node']) ? $result['node'] : array();

    foreach ($projects as $nid => $project) {
      // Catch created or edited by.
      $query = new EntityFieldQuery;
      $result = $query
        ->entityCondition('entity_type', 'node')
        // Limit to issues on this project.
        ->fieldCondition('field_project', 'target_id', $nid)
        ->propertyCondition('uid', $uid)
        ->range(0, 1)
        ->execute();
      if (!empty($result['node'])) {
        // Found.
        continue;
      }
      // Catch commented on.
      // @@@ TODO figure out how to do this in terms of EFQ!
      $result = db_query_range('SELECT 1 FROM {comment} c INNER JOIN {field_data_field_project} p ON c.nid = p.entity_id WHERE p.entity_type = :type AND p.field_project_target_id = :project AND c.uid = :uid', 0, 1, array(':type' => 'node', ':project' => $nid, ':uid' => $uid))->rowCount();
      if ($result) {
        // Found.
        continue;
      }
      // Not found, remove from results.
      unset($projects[$nid]);
    }

    $prefix = count($array) ? implode(', ', $array) .', ' : '';
    $projects = node_load_multiple(array_keys($projects));
    foreach ($projects as $project) {
      $title = $project->title;
      // Commas and quotes in terms are special cases, so encode 'em.
      if (strpos($title, ',') !== FALSE || strpos($title, '"') !== FALSE) {
        $title = '"'. str_replace('"', '""', $project->title) .'"';
      }
      $matches[$prefix . $title] = check_plain($project->title);
    }
  }

  drupal_json_output($matches);
}

/**
 * Return valid issue-enabled project names based on a user's own projects.
 *
 * Only returns matches for project titles for projects the user owns.
 */
function project_issue_autocomplete_project_user($uid, $string = '') {
  $matches = array();

  // The user enters a comma-separated list of projects. We only autocomplete
  // the last one.
  $array = drupal_explode_tags($string);
  $last_string = trim(array_pop($array));

  if ($last_string != '') {
    $query = new EntityFieldQuery;
    $result = $query
      // Projects are currently restricted to nodes!
      ->entityCondition('entity_type', 'node')
      // If there is a field_project_type, this is a project.
      // This part is what we care about.
      ->fieldCondition('field_project_type', 'value', NULL, 'IS NOT NULL')
      ->fieldCondition('field_project_has_issue_queue', 'value', 1)
      ->propertyCondition('uid', $uid)
      ->propertyCondition('title', '%' . db_like($last_string) . '%', 'like')
      ->range(0, 10)
      ->execute();

    $prefix = count($array) ? implode(', ', $array) .', ' : '';
    $projects = isset($result['node']) ? $result['node'] : array();
    $projects = node_load_multiple(array_keys($projects));
    foreach ($projects as $project) {
      $title = $project->title;
      // Commas and quotes in terms are special cases, so encode 'em.
      if (strpos($title, ',') !== FALSE || strpos($title, '"') !== FALSE) {
        $title = '"'. str_replace('"', '""', $project->title) .'"';
      }
      $matches[$prefix . $title] = check_plain($project->title);
    }
  }

  drupal_json_output($matches);
}


/**
 * Handles the auto-complete callback for the nodereference widget.
 *
 * Instead of returning a value, this function sends it to the browser by
 * calling drupal_json(). The returned value is a JSON-encoded array of matches,
 * where the array keys and values are both '#NID: TITLE', where NID is the node
 * ID, and title is the node title. In the values, the title is run through
 * check_plain, but not in the keys.
 *
 * @param $string
 *   String the user typed.
 */
function project_issue_autocomplete_issues_nodereference($string) {
  $matches = array();
  $results = project_issue_autocomplete_issues_search($string);
  foreach ($results as $nid => $title) {
    // NID here is coming from the node table, so doesn't need to be
    // sanitized. In the array key, we don't sanitize the title either,
    // because that is what the nodereference module is expecting, but we
    // do want it sanitized in the output.
    $matches["#$nid: " . $title] = "#$nid: " . check_plain($title);
  }
  drupal_json($matches);
}

/**
 * Matches issues against input (partial node ID or partial title).
 *
 * @param $string
 *   User submitted text to match against the issue node ID or title.
 * @param $items
 *   Number of matches to return.
 *
 * @return
 *   Associative array of issues that match the input string, with node ID as
 *   the key, and title as the value. If there are no matches, an empty array
 *   is returned.
 */
function project_issue_autocomplete_issues_search($string, $items = 10) {
  $matches = array();

  // Match against node IDs first.
  if (is_numeric($string)) {
    // Try to find issues whose ID starts with this number.
    $result = db_query_range(db_rewrite_sql("SELECT DISTINCT(n.nid), n.title FROM {node} n WHERE n.status = 1 AND n.type = 'project_issue' AND n.nid LIKE '%s%'"), $string, 0, $items);
  }
  while ($issue = db_fetch_object($result)) {
    $matches[$issue->nid] = $issue->title;
  }

  // If we don't have the required number of items, match against the title,
  // using a full-text match of whatever was entered.
  if (count($matches) < $items) {
    $needed = $items - count($matches);
    $values = array();
    // Make sure that any matches that we've already found are excluded.
    if (!empty($matches)) {
      $values = array_keys($matches);
      $sql = "SELECT n.nid, n.title FROM {node} n WHERE n.status = 1 AND n.type = 'project_issue' AND n.nid NOT IN (" . db_placeholders($values) . ") AND n.title LIKE '%%%s%%'";
    }
    else {
      $sql = "SELECT n.nid, n.title FROM {node} n WHERE n.status = 1 AND n.type = 'project_issue' AND n.title LIKE '%%%s%%'";
    }
    // We need the string to match against.
    $values[] = $string;
    $result = db_query_range(db_rewrite_sql($sql), $values, 0, $needed);
  }
  while ($issue = db_fetch_object($result)) {
    $matches[$issue->nid] = $issue->title;
  }

  return $matches;
}
