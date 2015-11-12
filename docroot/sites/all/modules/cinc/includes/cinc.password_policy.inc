<?php

/**
 * Creates password policies in code.
 *
 * @code
 * $my_policy = CINC::init('PasswordPolicy')->machine_name('My password policy');
 * @endcode
 */
class CINC_PasswordPolicy extends CINC implements CINC_CRUD {

  public static $dependencies = array('password_policy');
  public $config_machine_name = 'name';
  public $cinc_yaml_prefix = 'password_policy';

  function __construct() {

    $this->config = array(
      'name' => '',
      'description' => '',
      'policy' => array(
        'alphanumeric' => '',
        'complexity' => '',
        'delay' => '',
        'digit' => '',
        'digit_placement' => '',
        'history' => '',
        'length' => '',
        'letter' => '',
        'lowercase' => '',
        'punctuation' => '',
        'uppercase' => '',
        'username' => '',
      ),
      'roles' => array(
        DRUPAL_AUTHENTICATED_RID,
      ),
      'enabled' => 1,
      'expiration' => 0,
      'warning' => '',
    );

  }

  /**
   * Saves password policy to the database.
   */
  public function create() {

    $pid = db_insert('password_policy')
      ->fields(array(
        'name' => $this->get('name'),
        'description' => $this->get('description'),
        'enabled' => $this->get('enabled'),
        'policy' => serialize($this->get('policy')),
        'expiration' => $this->get('expiration'),
        'warning' => $this->get('warning'),
      ))
      ->execute();

    $this->set('pid', $pid);

    $roles = $this->get('roles');

    foreach ($roles as $rid) {
      db_insert('password_policy_role')
        ->fields(array(
          'pid' => $pid,
          'rid' => $rid,
        ))
        ->execute();
    }

    return $this;
  }

  /**
   * Tests if password policy exists.
   */
  public function exists() {

    $row = db_select('password_policy', 'p')
      ->fields('p')
      ->condition('name', $this->get('name'))
      ->execute()
      ->fetchAssoc();

    if ($row) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Loads password policy from the database.
   */
  public function read() {

    $row = db_select('password_policy', 'p')
      ->fields('p')
      ->condition('name', $this->get('name'))
      ->execute()
      ->fetchAssoc();

    if ($row) {

      $row['policy'] = unserialize($row['policy']);
      $row['roles'] = array();

      $result = db_select('password_policy_role', 'p')
        ->fields('p', array('rid'))
        ->condition('pid', $row['pid'])
        ->execute();
      foreach ($result as $role) {
        $row['roles'][] = $role->rid;
      }

      $this->config = $row;
    }

    return $this;
  }

  /**
   * Loads list of machine names.
   */
  static public function read_list() {
    $list = array();
    return $list;
  }

  /**
   * Saves password policy to the database.
   */
  public function update() {
    $clone = clone $this;
    $clone->delete();
    $this->create();
    return $this;
  }

  /**
   * Removes password policy from the database.
   */
  public function delete() {
    $pid = $this->get('pid');
    if (empty($pid)) {
      $this->read();
      $pid = $this->get('pid');
    }
    db_delete('password_policy')->condition('pid', $pid)->execute();
    db_delete('password_policy_role')->condition('pid', $pid)->execute();
    return $this;
  }

}
