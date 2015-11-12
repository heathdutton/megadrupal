<?php

/**
 * @file
 * Profile2 Moderation admin pages.
 */

/**
 * Page with table of all unmoderated profile revisions.
 */
function profile2_moderation_pages_moderation() {
  $header = array(
    array('data' => t('User'), 'width'=> '10%'), 
    array('data' => t('Current revision'), 'width'=> '5%'), 
    array('data' => t('Latest revision to moderate'), 'width'=> '5%'), 
    array('data' => t('Difference'), 'width'=> '60%'), 
    array('data' => t('Operations'), 'colspan' => 2, 'width' => '20%'),
  );
  $revisions = profile2_moderation_unmoderate_revision_list();
  $rows = array();

  $delete_permission = FALSE;
  if ((user_access('delete profile revisions') || user_access('administer profiles')) ) {
    $delete_permission = TRUE;
  }

  foreach ($revisions as $revision) {
    $row = array();
    $operations = array();

    $row[] = $revision->username;
    $row[] = $revision->current_vid;
    $row[] = $revision->vid;

    $current_profile = profile2_load($revision->pid);
    $latest_profile = profile2_load($revision->pid, $revision->vid);

    $diff = profile2_moderation_get_revisions_diff($current_profile, $latest_profile);

    $diff_header = array(
      array('data' => t('Field'), 'width'=> '30%'), 
      array('data' => t('Current'), 'width'=> '35%'), 
      array('data' => t('Latest'), 'width'=> '35%')
    );
    $diff_rows = array();
    foreach ($diff as $key => $value) {
      $diff_row = array();
      $diff_row[] = '<em>' . $key . '</em>: ';
      $diff_row[] = '<span style="color:#f00">' . $value[0] . '</span>';
      $diff_row[] = '<span style="color:#0f0">' . $value[1] . '</span>';
      $diff_rows[] = $diff_row;
    }

    $row[] = theme_table(array(
      'header' => $diff_header,
      'rows' => $diff_rows,
      'attributes' => array(),
      'caption' => NULL,
      'colgroups' => NULL,
      'sticky' => FALSE,
      'empty' => FALSE,
    ));

    // Moderate link.
    $operations[] = l(t('Apply latest'), "admin/profile/$revision->pid/revisions/$revision->vid/moderate");

    // Delete link.
    if ($delete_permission) {
      $operations[] = l(t('Delete latest'), "admin/profile/$revision->pid/revisions/$revision->vid/delete");
    } else {
      $operations[] = '-';
    }

    $rows[] = array_merge($row, $operations);
  }

  $build = array();
  if (count($rows) > 0) {
    $build['profile2_moderation_revisions_moderation_table'] = array(
      '#theme' => 'table',
      '#rows' => $rows,
      '#header' => $header,
    );
  } else {
    $build['profile2_moderation_no_revisions_found'] = array(
      '#markup' => '<p>' . t('No revisions to moderate.') . '</p>',
    );
  }

  return $build;
}

/**
 * Return a list of all the existing revision numbers.
 */
function profile2_moderation_unmoderate_revision_list() {
  $revisions = array();
  $result = db_query('
    SELECT 
      p.pid,
      MAX(r.vid) AS vid, 
      p.vid AS current_vid, 
      u.name AS username
    FROM {profile_revision} r 
    LEFT JOIN {profile} p ON p.pid = r.pid 
    INNER JOIN {users} u ON u.uid = p.uid 
    WHERE r.status = 0 AND r.vid > p.vid
    GROUP BY p.pid, p.vid, u.name
    ORDER BY r.vid DESC
    ', array());

  foreach ($result as $revision) {
    $revisions[$revision->vid] = $revision;
  }
  return $revisions;
}

/**
 * Diffs two profile revisions.
 */
function profile2_moderation_get_revisions_diff($profile1, $profile2) {
  $properties = get_object_vars($profile1);
  $result = array();
  foreach ($properties as $key => $value) {
    if ($profile2->{$key} != $value) {
      // Just fields please.
      if (isset($value[LANGUAGE_NONE][0]) || isset($profile2->{$key}[LANGUAGE_NONE][0])) {
        $result[$key] = array();

        if (isset($value[LANGUAGE_NONE][0])) {
          $result[$key][] = $value[LANGUAGE_NONE][0]['value'];
        } else {
          $result[$key][] = "'" . t('Empty') . "'";
        }

        if (isset($profile2->{$key}[LANGUAGE_NONE][0])) {
          $result[$key][] = $profile2->{$key}[LANGUAGE_NONE][0]['value'];
        } else {
          $result[$key][] = "'" . t('Empty') . "'";
        }
      }
    }
  }

  return $result;
}
