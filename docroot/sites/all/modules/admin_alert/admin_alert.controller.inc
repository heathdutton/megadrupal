<?php
// $Id$

/**
 * @file
 *
 */

interface AdminAlertControllerInterface extends DrupalEntityControllerInterface {
  public function create();
  public function save($alert);
  public function delete($alert);
}

class AdminAlertController
extends DrupalDefaultEntityController
implements AdminAlertControllerInterface {

  public function create($type = 'info') {
    $info = entity_get_info('admin_alert');
    if (!in_array($type, array_keys($info['bundles']))) {
      $type = 'info';
    }
    global $user;
    $alert = (object) array(
      'aaid' => 0,
      'name' => uniqid("alert_{$user->uid}_"),
      'type' => $type,
      'path' => NULL,
      'send' => FALSE,
      'translate' => FALSE,
      'replacements' => NULL,
      'closed' => 0,
      'created' => 0,
    );
    return $alert;
  }

  public function save($alert) {
    if ($roles = variable_get('admin_alert_roles')) { // intentional assignment of $roles
      $users = db_query('SELECT uid, COUNT(rid) rid_count FROM {users_roles} WHERE rid IN (:rids) GROUP BY uid', array(':rids' => $roles))->fetchAll();
      if (!empty($users)) {

        if (empty($alert->aaid)) {
          $alert->created = time();
        }
        field_attach_presave('admin_alert', $alert);
        module_invoke_all('entity_presave', 'admin_alert', $alert);
        $primary_keys = $alert->aaid ? 'aaid' : array();
        drupal_write_record('admin_alert', $alert, $primary_keys);

        if (empty($primary_keys)) {
          field_attach_insert('admin_alert', $alert);
          module_invoke_all('entity_insert', 'admin_alert', $alert);

          $save_query = db_insert('admin_alert_users')->fields(array('aaid', 'uid'));
          foreach ($users as $account) {
            if (!variable_get('admin_alert_exclude_user_1', FALSE) || $account->uid != 1) {
              $save_query->values(array($alert->aaid, $account->uid));
              $save_query->execute();
            }
          }
          if ($alert->send) {
            admin_alert_send($alert);
          }

        }
        else {
          field_attach_update('admin_alert', $alert);
          module_invoke_all('entity_update', 'admin_alert', $alert);
        }
        cache_clear_all('admin_alert:block:user', 'cache_block', TRUE);
        return $alert;
      }
    }
    drupal_set_message('There were no users with administrative roles.');
    return FALSE;
  }

  public function delete($alert) {
    $this->delete_multiple(array($alert));
  }

  public function delete_multiple($alerts) {
    if (!empty($alerts)) {
      $aaids = array();
      $transaction = db_transaction();
      try {
        foreach ($alerts as $alert) {
          module_invoke_all('entity_delete', $alert, 'admin_alert');
          field_attach_delete('admin_alert', $alert);
          $aaids[] = $alert->aaid;
        }
        db_delete('admin_alert')->condition('aaid', $aaids, 'IN')->execute();
        db_delete('admin_alert_users')->condition('aaid', $aaids, 'IN')->execute();
      }
      catch (Exception $e) {
        $transaction->rollback();
        watchdog_exception('admin_alert', $e);
        throw $e;
        return FALSE;
      }
      cache_clear_all('admin_alert:block:user', 'cache_block', TRUE);
      return TRUE;
    }
  }

  protected function buildQuery($ids, $conditions = array(), $revision_id = FALSE) {
    if (!empty($conditions)) {
      $user_conditions = array_intersect_key($conditions, array('uid' => 1, 'user_read' => 1, 'count' => 1));
      if (!empty($user_conditions)) {
        $conditions = array_diff_key($conditions, $user_conditions);
        $query = parent::buildQuery($ids, $conditions, $revision_id);
        $query->innerJoin('admin_alert_users', 'aau', 'base.aaid = aau.aaid');
        $query->addField('aau', 'uid');
        $query->addField('aau', 'user_read');
        if (!empty($user_conditions['count'])) {
          $query->addExpression('COUNT(aau.uid)', 'count');
          $query->addExpression('COUNT(aau.user_read)', 'count_read');
          $query->groupBy('base.aaid');
          unset($user_conditions['count']);
        }
        foreach ($user_conditions as $field => $value) {
          $query->condition('aau.' . $field, $value);
        }
        return $query;
      }
    }
    return parent::buildQuery($ids, $conditions, $revision_id);
  }

}
