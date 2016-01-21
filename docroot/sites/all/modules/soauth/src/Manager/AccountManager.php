<?php

namespace Drupal\soauth\Manager;

use Drupal\soauth\Common\Entity\User;
use Drupal\soauth\Common\Entity\Account;


/**
 * AccountManager
 * @author Raman Liubimau <raman@cmstuning.net>
 */
class AccountManager {
  
  /**
   * Manager owner
   * @var User
   */
  private $user;
  
  /**
   * Construct manager
   * @param User $user
   */
  public function __construct($user) {
    $this->user = $user;
  }
  
  /**
   * Get owner
   * @return User
   */
  public function getOwner() {
    return $this->user;
  }
  
  /**
   * Add account
   * @param Account $account
   * @return AccountManager
   */
  public function addAccount($account) {
    db_merge('soauth_user')
      ->key(array(
        'uid' => $this->getOwner()->getId(),
        'app' => $account->getProvider(),))
      ->fields(array(
        'app_uid' => $account->getExtUserId(),
        'mail' => $account->getMail(),))
      ->execute();
    
    return $this;
  }
  
  /**
   * Get user accounts
   * @return array
   */
  public function getAccounts() {
    // Get accounts for user
    $result = array();
    
    $query = db_select('soauth_user', 'u')
      ->condition('uid', $this->getOwner()->getId())
      ->execute();
    
    while (($info = $query->fetchAssoc())) {
      $result[$info['app']] = Account::create($info['app'])
        ->setExtUserId($info['app_uid'])
        ->setMail($info['mail']);
    }
    
    return $result;
  }
  
  /**
   * Get user account for provider
   * @param string $provider
   * @return Account|NULL
   */
  public function getAccount($provider) {
    // Build query
    $query = db_select('soauth_user', 'u')
      ->condition('uid', $this->getOwner()->getId())
      ->condition('app', $provider)
      ->execute();
    
    $info = $query->fetchAssoc();
    
    if (!empty($info)) {
      return Account::create($info['app'])
        ->setExtUserId($info['app_uid'])
        ->setMail($info['mail']);
    }
    return NULL;
  }
 
  /**
   * Delete user accounts
   * @return AccountManager
   */
  public function delAccounts() {
    // Build query
    db_delete('soauth_user')
      ->condition('uid', $this->getOwner()->getId())
      ->execute();
    
    return $this;
  }
  
  /**
   * Delete user account for provider
   * @param AbstractBaseProvider $provider
   * @return AccountManager
   */
  public function delAccount($provider) {
    // Build query
    db_delete('soauth_user')
      ->condition('uid', $this->getOwner()->getId())
      ->condition('app', $provider)
      ->execute();
    
    return $this;
  }
  
  /**
   * Check user has account
   * @param AbstarctBaseProvider $provider
   * @return boolean
   */
  public function hasAccount($provider) {
    // Build query
    $query = db_select('soauth_user', 'u')
      ->fields('u', array('aid',))
      ->condition('uid', $this->getOwner()->getId())
      ->condition('app', $provider)
      ->execute();
      
    return (bool)$query->fetchField();
  }
  
  /**
   * Get account manager
   * @param User $user
   */
  static public function getManagerFor($user) {
    return new self($user);
  }
  
}
