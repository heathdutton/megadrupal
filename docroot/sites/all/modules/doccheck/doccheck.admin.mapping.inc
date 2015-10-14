<?php

/**
 * Administration form for role mappings.
 */
function doccheck_admin_mapping_page() {
  $links = array();

  foreach (user_roles(TRUE) as $rid => $role) {
    if ($role == t('authenticated user')) {
      continue;
    }

    $links["role-{$rid}"] = array(
      'title' => $role,
      'href' => "admin/config/people/doccheck/mapping/{$rid}",
    );
  }

  return array(
    '#theme' => 'links',
    '#links' => $links,
  );
}

/**
 * Admin form for the mappings.
 */
function doccheck_admin_mapping_role_form($form, &$form_state, $role) {
  if ($role->rid == DRUPAL_ANONYMOUS_RID || $role->rid == DRUPAL_AUTHENTICATED_RID) {
    drupal_not_found();
  }

  $form['#role'] = $role;

  $form['#prefix'] = '<div id="doccheck-mapping-form-wrapper">';
  $form['#suffix'] = '</div>';

  $form['list'] = array(
    '#type' => 'container',
    '#theme_wrappers' => array(
      'doccheck_form_table',
    ),
  );

  $form['list']['header'] = array(
    '#type' => 'container',
    '#theme_wrappers' => array(
      'doccheck_form_tr',
    ),
  );

  $form['list']['header'][] = array(
    '#markup' => t('Profession'),
    '#theme_wrappers' => array(
      'doccheck_form_th',
    ),
  );

  $form['list']['header'][] = array(
    '#markup' => t('Activity'),
    '#theme_wrappers' => array(
      'doccheck_form_th',
    ),
  );

  $form['list']['header'][] = array(
    '#markup' => '&nbsp;',
    '#theme_wrappers' => array(
      'doccheck_form_th',
    ),
  );

  if (!isset($form_state['mappings'])) {
    $rid_mappings = _doccheck_get_professions_and_activities_by_role(array($role->rid => $role->name));
    $form_state['mappings'] = $rid_mappings[$role->rid];
  }
  foreach ($form_state['mappings'] as $mapping) {
    $hash = "{$role->rid}_{$mapping['profession']}_{$mapping['activity']}";
    $form['list'][$hash] = array(
      '#type' => 'container',
      '#theme_wrappers' => array(
        'doccheck_form_tr',
      ),
      'profession' => array(
        '#markup' => check_plain(t($mapping['profession'])),
        '#theme_wrappers' => array(
          'doccheck_form_td',
        ),
      ),
      'activity' => array(
        '#markup' => check_plain(t($mapping['activity'])),
        '#theme_wrappers' => array(
          'doccheck_form_td',
        ),
      ),
      'remove' => array(
        '#type' => 'submit',
        '#value' => t('Remove'),
        '#name' => drupal_html_class("{$hash}_remove"),
        '#mapping' => $mapping,
        '#role' => $role,
        '#theme_wrappers' => array(
          'button',
          'doccheck_form_td',
        ),
        '#submit' => array(
          '_doccheck_mapping_remove',
        ),
        '#ajax' => array(
          'callback' => '_doccheck_mapping_ajax',
          'wrapper' => 'doccheck-mapping-form-wrapper',
        ),
      ),
    );
  }

  $notification = isset($form_state['notifications']) ?
    $form_state['notifications'] : '';
  if ($notification) {
    $msg = reset($notification);
    $form['notification'] = array(
      '#type' => 'container',
    );
    $form['notification']['message'] = array(
      '#markup' => "<div class=\"messages warning\">{$msg}</div>",
    );
    $form['notification']['undo'] = array(
      '#type' => 'submit',
      '#value' => t('Undo'),
      '#name' => "undo_{$role->rid}",
      '#role' => $role,
      '#submit' => array('_doccheck_mapping_undo'),
      '#ajax' => array(
        'callback' => '_doccheck_mapping_ajax',
        'wrapper' => 'doccheck-mapping-form-wrapper',
      ),
    );
  }

  $form['addwrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add'),
  );

  $form['addwrapper']['professions'] = array(
    '#title' => t('Professions'),
    '#type' => 'select',
    '#options' => array(
        'all' => t('All'),
      ) + _doccheck_get_profession_list(),
  );

  $form['addwrapper']['activities'] = array(
    '#title' => t('Activities'),
    '#type' => 'select',
    '#options' => array(
        'all' => t('All'),
        'any' => t('Any'),
      ) + _doccheck_get_activity_list(),
  );

  $form['addwrapper']['add'] = array(
    '#type' => 'submit',
    '#submit' => array('_doccheck_mapping_add'),
    '#name' => "add_{$role->rid}",
    '#role' => $role,
    '#value' => t('Add'),
    '#ajax' => array(
      'callback' => '_doccheck_mapping_ajax',
      'wrapper' => 'doccheck-mapping-form-wrapper',
    ),
  );

  $form['disclaimer'] = array(
    '#markup' => '<p><small>' . t('Warning! Hitting submit will rewrite the user <-> role mappings.') . '</small></p>',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Submit handler for 'doccheck_admin_mapping_form'.
 */
function doccheck_admin_mapping_role_form_submit($form, &$form_state) {
  db_delete('doccheck_mapping')
    ->condition('rid', $form['#role']->rid)
    ->execute();
  drupal_static_reset('_doccheck_get_professions_and_activities_by_role');

  $operations = array();
  $operations[] = array(
    'doccheck_mapping_clear_role',
    array($form['#role']->rid)
  );

  foreach ($form_state['mappings'] as $mapping) {
    db_insert('doccheck_mapping')
      ->fields($mapping)
      ->execute();

    $operations[] = array(
      'doccheck_mapping_assign_role',
      array($form['#role']->rid, $mapping['profession'], $mapping['activity'])
    );
  }

  // Update the user <-> role mappings.
  batch_set(array(
    'title' => t('Updating user roles'),
    'operations' => $operations,
    'finished' => 'doccheck_mapping_batch_finished',
    'file' => drupal_get_path('module', 'doccheck') . '/doccheck.admin.mapping.inc',
  ));
}

/**
 * Batch task to clear all user associations with the given role.
 *
 * @param int $rid
 *   Role id
 * @param array $context
 *   The $context array supplied by the Batch API.
 */
function doccheck_mapping_clear_role($rid, array &$context) {
  $deleted = db_delete('users_roles')
    ->condition('rid', $rid)
    ->execute();
  $context['results'][] = $deleted;
}

/**
 * Batch task to assign users to a role based on their profession and activity.
 *
 * @param int $rid
 *   Role id
 * @param string $profession
 * @param string $activity
 * @param array $context
 *   The $context array supplied by the Batch API.
 */
function doccheck_mapping_assign_role($rid, $profession, $activity, array &$context) {
  if (empty($context['sandbox'])) {
    $context['sandbox']['progress'] = 0;

    $context['sandbox']['fids'] = db_query('SELECT title, fid FROM {profile_field} WHERE title IN (:titles)', array(
      ':titles' => array('Profession', 'Activity'),
    ))->fetchAllKeyed();

    $count_query = db_select('users', 'u');
    $count_query->innerJoin('profile_value', 'vp', 'u.uid = vp.uid AND vp.fid = ' . ((int) $context['sandbox']['fids']['Profession']));
    if ($activity != 'Any') {
      $count_query->innerJoin('profile_value', 'va', 'u.uid = va.uid AND va.fid = ' . ((int) $context['sandbox']['fids']['Activity']));
      $count_query->condition('va.value', $activity);
    }
    $count_query->condition('vp.value', $profession);
    $count_query->addExpression('COUNT(u.uid)'); // ->countQuery() adds COUNT(*)
    $context['sandbox']['max'] = $count_query->execute()->fetchField();
  }

  $query = db_select('users', 'u');
  $query->fields('u', array('uid', 'name'));
  $query->innerJoin('profile_value', 'vp', 'u.uid = vp.uid AND vp.fid = ' . ((int) $context['sandbox']['fids']['Profession']));
  if ($activity != 'Any') {
    $query->innerJoin('profile_value', 'va', 'u.uid = va.uid AND va.fid = ' . ((int) $context['sandbox']['fids']['Activity']));
    $query->condition('va.value', $activity);
  }
  $query->leftJoin('users_roles', 'r', 'u.uid = r.uid AND r.rid =' . ((int) $rid));
  $query->condition('vp.value', $profession);
  $query->isNull('r.rid');
  $query->range(0, 25);

  $result = $query->execute()->fetchAllKeyed();

  foreach ($result as $uid => $name) {
    db_insert('users_roles')
      ->fields(array(
        'uid' => $uid,
        'rid' => $rid,
      ))
      ->execute();
    $context['sandbox']['progress']++;
    $context['results'][] = $uid;
    $context['message'] = t('Processed user @name', array('@name' => $name));
  }

  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Finish callback for the Batch API.
 */
function doccheck_mapping_batch_finished($success, $results, $operations) {
  if ($success) {
    $message = '';
    $message .= format_plural($results[0], 'One association removed.', '@count role associations removed.');
    $message .= ' ';
    $message .= format_plural(count($results) - 1, 'One user processed', '@count users processed.');
  }
  else {
    $message = t('An error occoured.');
  }

  drupal_set_message($message);
}

/**
 * Removes a mapping.
 */
function _doccheck_mapping_remove($form, &$form_state) {
  $form_state['rebuild'] = TRUE;

  $form_state['archive'][] = $form_state['mappings'];

  $mapping = $form_state['triggering_element']['#mapping'];
  foreach ($form_state['mappings'] as $id => $data) {
    if ($data['profession'] == $mapping['profession'] && $data['activity'] == $mapping['activity']) {
      unset($form_state['mappings'][$id]);
    }
  }
  if (!isset($form_state['notifications'])) {
    $form_state['notifications'] = array();
  }
  array_unshift($form_state['notifications'],
    t('Removed %profession with %activity', array(
      '%profession' => $mapping['profession'],
      '%activity' => $mapping['activity'],
    )));
}

/**
 * Adds a mapping.
 */
function _doccheck_mapping_add($form, &$form_state) {
  $form_state['rebuild'] = TRUE;

  $form_state['archive'][] = $form_state['mappings'];

  $role = $form_state['triggering_element']['#role'];

  if (($activity = $form_state['values']['activities']) == 'all') {
    $new_activities = _doccheck_get_activity_list();
  }
  else if ($activity == 'any') {
    $new_activities = 'Any';
  }
  else {
    $new_activities = array($activity);
  }

  $new_professions = (($profession = $form_state['values']['professions']) == 'all') ?
    _doccheck_get_profession_list() :
    array($profession);

  $counter = 0;
  foreach ($new_professions as $new_profession) {
    if (is_array($new_activities)) {
      foreach ($new_activities as $new_activity) {
        $found = FALSE;
        foreach ($form_state['mappings'] as $mapping) {
          if ($mapping['profession'] == $new_profession && $mapping['activity'] == $new_activity) {
            $found = TRUE;
            break;
          }
        }
        if (!$found) {
          $counter++;
          $form_state['mappings'][] = array(
            'rid' => $role->rid,
            'profession' => $new_profession,
            'activity' => $new_activity,
          );
        }
      }
    }
    else { // the $new_activities must be 'any'
      foreach ($form_state['mappings'] as $key => $mapping) {
        if ($mapping['profession'] == $new_profession) {
          unset($form_state['mappings'][$key]);
        }
      }
      $counter++;
      $form_state['mappings'][] = array(
        'rid' => $role->rid,
        'profession' => $new_profession,
        'activity' => $new_activities,
      );
    }
  }

  if (!isset($form_state['notifications'])) {
    $form_state['notifications'] = array();
  }

  array_unshift($form_state['notifications'], $counter == 1 ?
    t('Added %profession with %activity', array(
      '%profession' => $new_professions[0],
      '%activity' => is_array($new_activities) ? $new_activities[0] : t('Any'),
    )) :
    t('Added %num professions and activities', array(
      '%num' => $counter,
    )));
}

/**
 * Undoes a mapping.
 */
function _doccheck_mapping_undo($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $form_state['mappings'] = array_pop($form_state['archive']);
  array_shift($form_state['notifications']);
}

function _doccheck_remove_mapping($mapping) {
  $cache = &drupal_static('_doccheck_get_professions_and_activities_by_role', array());
  foreach ($cache[$mapping['rid']] as $id => $item) {
    if ($item['profession'] == $mapping['profession'] && $item['activity'] == $mapping['activity']) {
      unset($cache[$mapping['rid']][$id]);
    }
  }

  db_delete('doccheck_mapping')
    ->condition('rid', $mapping['rid'])
    ->condition('profession', $mapping['profession'])
    ->condition('activity', $mapping['activity'])
    ->execute();
}
