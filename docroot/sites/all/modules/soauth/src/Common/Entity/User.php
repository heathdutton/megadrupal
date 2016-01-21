<?php

namespace Drupal\soauth\Common\Entity;


/**
 * Class User
 * @author Raman Liubimau <raman@cmstuning.net>
 */
class User {

  /**
   * Drupals user object
   * @var stdClass
   */
  private $object;
  
  /**
   * Construct User
   * @param integer $uid
   */
  public function __construct($uid=0) {
    $this->object = (object)array(
      'uid' => $uid,
    );
  }
  
  /**
   * Get field value
   * @param string $name
   * @return mixed
   */
  public function __get($name) {
    return $this->object->{$name};
  }
  
  /**
   * Set field value
   * @param string $name
   * @param mixed $value
   */
  public function __set($name, $value) {
    $this->object->{$name} = $value;
  }
  
  /**
   * Get user id
   * @return integer
   */
  public function getId() {
    return $this->uid;
  }
  
  /**
   * Get Drupal's user object
   * @return stdClass
   */
  public function getObject() {
    return $this->object;
  }
  
  /**
   * Login
   */
  public function login() {
    $form_state = array(
        'uid' => $this->uid,
    );
    
    user_login_submit(array(), $form_state);
  }
  
  /**
   * Save user
   * @return User
   */
  public function save() {
    $this->object = user_save($this->object);
    
    return $this;
  }
  
  /**
   * Check if user is anonymous
   * @return boolean
   */
  public function isAnonymous() {
    return (bool)$this->uid;
  }
  
  /**
   * Get current user
   * @global stdClass $user
   * @return User
   */
  static public function getCurrent() {
    // From globals
    global $user;
    
    return new self($user->uid);
  }
  
  /**
   * Find user
   * @param string $provider
   * @param string $ext_uid
   * @param string $mail
   * @return integer
   */
  static public function find($provider, $ext_uid, $mail) {
    // Build query to try find user. Data from {users}, {soauth_user} tables 
    // are used.
    
    // Try find user by mail
    $query = db_select('users', 'u')
      ->fields('u', array('uid',))
      ->condition('mail', $mail);
    
    // By provider condition
    $by_provider = db_and()
      ->condition('app', $provider)
      ->condition('app_uid', $ext_uid);
    
    // Try find user by provider and external user ID, then merge results
    $query->union(db_select('soauth_user', 'k')
      ->fields('k', array('uid',))
      ->condition(db_or()
        ->condition($by_provider)
        ->condition('mail', $mail)));
    
    $result = $query->execute()
      ->fetchAssoc();
    
    return (empty($result) ? NULL : new self($result['uid']));
  }
  
}
