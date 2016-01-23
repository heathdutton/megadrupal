<?php

/**
 * @file
 * Subdomain engine user mode subclass
 */


/**
 * Description of subdomain_user
 *
 * @author setvik
 */
class subdomain_mode_user extends subdomain_engine {

  function type() {
    return 'user';
  }

  function base_path($sid) {
    return 'user/' . $sid;
  }

  function static_form_text() {
    return t('My homepage');
  }

  function is_entry_form($form_id) {
    return in_array($form_id, array('user_register_form', 'user_profile_form'));
  }

  function get_content_subdomain($nid) {
    $subdomains = &drupal_static(__CLASS__ . '__' . __METHOD__ . '__filtered', NULL);

    if (!isset($subdomains[$nid])) {
      $subdomains[$nid] = db_query("SELECT subdomain
          FROM {subdomain} s
          INNER JOIN {node} n ON s.sid = n.uid
          WHERE nid = :nid AND type NOT IN (:types)",
        array(':nid' => $nid, ':types' => _subdomain_filtered_content_types()))
      ->fetchField();
    }

    return $subdomains[$nid];
  }

  function entry_form_alter(&$form, $form_id) {
    $default = subdomain_get_subdomain($form['#user']->uid);
    $form['subdomain_exists'] = array(
      '#type' => 'value',
      '#value' => !empty($default),
    );

    $form['#validate'][] = '_subdomain_validate_subdomain';

    if ($this->source == 'custom' && !($form_id == 'user_register' && !user_access('create subdomains'))) {
      if (isset($form['account'])) {
        $parent = &$form['account'];
      }
      else {
        $parent = &$form;
      }
      $parent['name']['#weight'] = -5.1;

      $editable = $form_id == 'user_register' && user_access('create subdomains') || user_access('edit subdomains');

      $parent['subdomain'] = _subdomain_get_custom_form_field($default, $editable);
    }
    _subdomain_add_js_validation($form['#user']->uid);
  }

  function save($obj) {
    return $this->save_record($obj['uid'], $obj[$this->form_field]);
  }

  function delete($obj) {
    return $this->delete_record($obj->uid);
  }

  function url_alter_helper(&$subdomain, &$path, $original_path, $outbound = TRUE) {
    if ($uid = _subdomain_id_from_path('uid', $original_path)) {
      $subdomain = subdomain_get_subdomain($uid);
      if (!empty($subdomain) && $path == $this->base_path($uid) && $outbound) {
        $path = '';
      }
    }
    elseif ($nid = _subdomain_id_from_path('nid', $original_path)) {
      $subdomain = subdomain()->get_content_subdomain($nid);
    }
  }

  /**
   * Bulk generate subdomains from user name
   */
  function bulk_update_batch_process(&$context) {
    if (!isset($context['sandbox']['current'])) {
      $context['sandbox']['count'] = 0;
      $context['sandbox']['current'] = 0;
    }

    if (!isset($context['sandbox']['rids'])) {
      // Find roles that have the 'create subdomains' permission.
      $role_query = db_select('role_permission', 'rp');
      $role_query->addField('rp', 'rid');
      $role_query->condition('rp.module', 'subdomain', '=');
      $role_query->condition('rp.permission', 'create subdomains', '=');
      $role_query->distinct();
      $context['sandbox']['rids'] = $role_query->execute()->fetchCol();
    }

    $query = db_select('users', 'u');
    $query->leftJoin('subdomain', 's', 's.sid = u.uid');
    $query->addField('u', 'uid');
    $query->isNull('s.sid');
    $query->condition('u.status', 1, '=');
    $query->condition('u.uid', $context['sandbox']['current'], '>');
    $query->orderBy('u.uid');
    $query->addTag('subdomain_bulk_update');
    $query->addMetaData('entity', 'user');

    // If 'create subdomains' permission is not on the authenticated user role
    // we need to search the users_roles table.
    if (!in_array(DRUPAL_AUTHENTICATED_RID, $context['sandbox']['rids'])) {
      $query->innerJoin('users_roles', 'ur', 'ur.uid = u.uid');
      $query->condition('ur.rid', $context['sandbox']['rids'], 'IN');
      $query->distinct();
    }

    // Get the total amount of items to process.
    if (!isset($context['sandbox']['total'])) {
      $context['sandbox']['total'] = $query->countQuery()->execute()->fetchField();

      // If there are no users to update, the stop immediately.
      if (!$context['sandbox']['total']) {
        $context['finished'] = 1;
        return;
      }
    }

    $query->range(0, 25);
    $uids = $query->execute()->fetchCol();

    //pathauto_user_update_alias_multiple($uids, 'bulkupdate', array('alias blog' => FALSE));
    $accounts = user_load_multiple($uids);
    foreach ($accounts as $account) {
      //pathauto_user_update_alias($account, $op, $options);
      //if (user_access('create subdomains', $account)) {
        $this->save_record($account->uid, $account->name, TRUE);
      //}
    }

    $context['sandbox']['count'] += count($uids);
    $context['sandbox']['current'] = max($uids);
    $context['message'] = t('Updated subdomain for user @uid.', array('@uid' => end($uids)));

    if ($context['sandbox']['count'] != $context['sandbox']['total']) {
      $context['finished'] = $context['sandbox']['count'] / $context['sandbox']['total'];
    }
  }
}
