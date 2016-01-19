<?php

/**
 * Profession based authentication admin.
 */
function doccheck_admin_profession_auth_form($form, &$form_state) {
  $form['#prefix'] = '<div id="doccheck-profauth-form-wrapper">';
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
    '#markup' => t('User'),
    '#theme_wrappers' => array(
      'doccheck_form_th',
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

  if (!isset($form_state['profauth'])) {
    $form_state['profauth'] = variable_get('doccheck_profauth', array());
  }

  module_load_include('data.inc', 'doccheck');
  user_load_multiple(array_values($form_state['profauth'])); // Cache warmup

  foreach ($form_state['profauth'] as $profession_and_activity_id => $uid) {
    list($profid, $activityid) = explode('_', $profession_and_activity_id);
    $profession = _doccheck_get_profession_by_id((int) $profid);
    $activity = _doccheck_get_activity_by_id((int) $activityid);
    $account = user_load($uid);
    $form['list'][$profession_and_activity_id] = array(
      '#type' => 'container',
      '#theme_wrappers' => array(
        'doccheck_form_tr',
      ),
      'account' => array(
        '#markup' => check_plain($account->name),
        '#theme_wrappers' => array(
          'doccheck_form_td',
        ),
      ),
      'profession' => array(
        '#markup' => check_plain($profession),
        '#theme_wrappers' => array(
          'doccheck_form_td',
        ),
      ),
      'activity' => array(
        '#markup' => check_plain($activity),
        '#theme_wrappers' => array(
          'doccheck_form_td',
        ),
      ),
      'remove' => array(
        '#type' => 'submit',
        '#value' => t('Remove'),
        '#name' => drupal_html_class("{$profession_and_activity_id}_{$uid}_remove"),
        '#profession_and_activity_id' => $profession_and_activity_id,
        '#theme_wrappers' => array(
          'button',
          'doccheck_form_td',
        ),
        '#submit' => array(
          '_doccheck_profauth_remove',
        ),
        '#ajax' => array(
          'callback' => '_doccheck_mapping_ajax',
          'wrapper' => 'doccheck-profauth-form-wrapper',
        ),
      ),
    );
  }

  $form['addwrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add'),
  );

  $all = array(
    'all' => t('All'),
  );

  $form['addwrapper']['profession'] = array(
    '#title' => t('Profession'),
    '#type' => 'select',
    '#options' => $all + _doccheck_get_profession_list(TRUE),
  );

  $form['addwrapper']['activity'] = array(
    '#title' => t('Activity'),
    '#type' => 'select',
    '#options' => $all + _doccheck_get_activity_list(TRUE),
  );

  $form['addwrapper']['account'] = array(
    '#title' => t('User'),
    '#type' => 'textfield',
    '#autocomplete_path' => 'user/autocomplete',
  );

  $form['addwrapper']['add'] = array(
    '#type' => 'submit',
    '#submit' => array('_doccheck_profauth_add'),
    '#value' => t('Add'),
    '#ajax' => array(
      'callback' => '_doccheck_mapping_ajax',
      'wrapper' => 'doccheck-profauth-form-wrapper',
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

function _doccheck_profauth_remove($form, &$form_state) {
  $form_state['rebuild'] = TRUE;

  $profession_and_activity_id = $form_state['triggering_element']['#profession_and_activity_id'];
  unset($form_state['profauth'][$profession_and_activity_id]);
}

/**
 * Returns the list of profession or activity ids.
 *
 * @param string $type
 *   The string 'profession' or 'activity'.
 *
 * @return array
 */
function _doccheck_fetch_ids($type) {
  $function = "_doccheck_get_{$type}_list";
  if (is_callable($function)) {
    $items = $function(TRUE);
    return array_unique(array_keys($items));
  }
  return array();
}

function _doccheck_profauth_add($form, &$form_state) {
  $form_state['rebuild'] = TRUE;

  $profid = $form_state['values']['profession'];
  $activityid = $form_state['values']['activity'];
  $account = user_load_by_name($form_state['values']['account']);

  if ($account) {
    $profids = $profid === 'all' ? _doccheck_fetch_ids('profession') : array($profid);
    $activityids = $activityid === 'all' ? _doccheck_fetch_ids('activity') : array($activityid);
    foreach ($profids as $profid) {
      foreach ($activityids as $activityid) {
        $form_state['profauth']["{$profid}_{$activityid}"] = $account->uid;
      }
    }
  }
}

function doccheck_admin_profession_auth_form_submit($form, &$form_state) {
  variable_set('doccheck_profauth', $form_state['profauth']);
}
