<?php

/**
 * @file
 * Custom reports for course relationships.
 */

/**
 * Make a report of all children drilled down to a certain level to show completions.
 */
function course_relationships_activity_set_report_tree($node, $toLevel = 1) {
  $select = drupal_get_form('course_relationships_activity_set_report_tree_select');
  $parent = $node->nid;
  $toLevel = intval(!empty($_REQUEST['level']) ? $_REQUEST['level'] : 1);

  // Rows that will get output to a table/csv.
  $rows = array();

  $user_fields = array(
    'uid' => 'User ID',
    'name' => 'Name',
  );

  $levelParents = array($parent => $parent);
  for ($level = 1; $level <= $toLevel; $level++) {
    $sql = "SELECT co.*, co.instance AS child FROM {node} np
    INNER JOIN {course_outline} co ON (co.nid = np.nid)
    INNER JOIN {node} n ON co.nid = n.nid
    WHERE co.nid IN (:nids) AND co.object_type = 'course'
    ORDER BY co.nid, co.weight";
    $result = db_query($sql, array(':nids' => array_keys($levelParents)));
    $levelParents = array();
    foreach ($user_fields as $key => $label) {
      $levelParents[$key] = '';
    }
    while ($row = $result->fetch()) {
      $courseObject = course_get_course_object_by_id($row->coid);
      $levelParents[$row->child] = array(
        'data' => $courseObject->getTitle(),
        'colspan' => 0,
      );
      if ($rows[$level - 1]) {
        $rows[$level - 1][$row->nid]['colspan']++;
      }
    }
    $rows[$level] = $levelParents;
  }

  $final_row = & $rows[$level - 1];
  foreach ($user_fields as $key => $label) {
    $final_row[$key] = $label;
  }

  $sql = "SELECT * FROM {course_report} cr
  LEFT JOIN {users} u ON cr.uid = u.uid
  WHERE nid IN (:nids)";
  $result = db_query($sql, array(':nids' => array_keys($final_row)));
  $users = array();
  while ($row = $result->fetch()) {
    $users[$row->uid][$row->nid] = $row;
  }

  foreach ($users as $uid => $user) {
    $row = array();
    foreach ($final_row as $child => $data) {
      if (!is_numeric($child)) {
        $row[] = reset($user)->{$child};
      }
      else {
        $row[] = $user[$child]->section_name;
      }
    }
    $rows[] = $row;
  }

  $headers = array();

  if (isset($_GET['csv']) && function_exists('theme_format_csv')) {
    header("Content-type: text/csv");
    header("Content-disposition: attachment; filename=$parent.csv");
    print theme_format_csv(array('headers' => $headers, 'rows' => $rows));
    exit;
  }
  else {
    return drupal_render($select) .
      theme_table(array('headers' => $headers, 'rows' => $rows, 'attributes' => array())) .
      l('Download CSV', $_GET['q'], array('query' => array('csv' => 1, 'level' => $toLevel)));
  }
}

function course_relationships_activity_set_report_tree_select() {
  $form = array();
  $form['#method'] = 'GET';
  $form['level'] = array(
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
    '#title' => t('Level'),
    '#type' => 'select',
    '#options' => drupal_map_assoc(range(1, 9)),
    '#description' => t('Select the level on which to report course status.'),
    '#default_value' => $_REQUEST['level'],
  );
  $form['submit'] = array(
    '#value' => t('Apply'),
    '#type' => 'submit',
  );
  return $form;
}
