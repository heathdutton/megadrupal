<?php

function doccheck_user_debug_page($account) {
  $page = array();

  if (variable_get('doccheck_log_get_array')) {
    $page['related_logs'] = array(
      '#prefix' => '<p>',
      '#markup' => l(t('Possibly related logs'), 'admin/reports/doccheck/' . $account->uid),
      '#suffix' => '</p>',
    );
  }

  $page['profession'] = array(
    '#prefix' => '<p>',
    '#markup' => t('Profession: %profession', array(
      '%profession' => $account->dc_beruf ? $account->dc_beruf : t('!! NO PROFESSION !!')
    )),
    '#suffix' => '</p>',
  );

  $page['activity'] = array(
    '#prefix' => '<p>',
    '#markup' => t('Activity: %activity', array(
      '%activity' => $account->dc_activity ? $account->dc_activity : t('!! NO ACTIVITY !!'),
    )),
    '#suffix' => '</p>',
  );

  $roles = db_query('SELECT rid, name FROM {users_roles} NATURAL JOIN {role} WHERE uid = :uid ORDER BY weight DESC', array(':uid' => $account->uid))
    ->fetchAllKeyed();

  $page['roles'] = array(
    '#theme' => 'item_list',
    '#items' => $roles,
    '#title' => t('Roles'),
  );

  $mappings_result = db_query('SELECT rid, profession, activity FROM {doccheck_mapping} WHERE profession = :profession AND (activity = :activity OR activity = :anyactivity)', array(
    ':profession' => $account->dc_beruf,
    ':activity' => $account->dc_activity,
    ':anyactivity' => 'Any',
  ));

  $mappings = array();
  $all_roles = user_roles(TRUE);
  foreach ($mappings_result as $record) {
    $mappings[] = array(
      'role' => $all_roles[$record->rid],
      'profession' => $record->profession,
      'activity' => $record->activity,
    );
  }

  $page['mappings'] = array(
    '#theme' => 'table',
    '#header' => array(
      t('Role'),
      t('Profession'),
      t('Activity'),
    ),
    '#rows' => $mappings,
    '#title' => t('Mappings'),
    '#caption' => t('Role mappings for this user'),
  );

  return $page;
}
