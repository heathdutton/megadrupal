<?php

namespace Drupal\soauth\Common\Entity;

/**
 * Class Account
 * @author Raman Liubimau <raman@cmstuning.net>
 */
class Account {
  
  /**
   * Account provider
   * @var AbstractProvider
   */
  private $provider;
  
  /**
   * External user ID
   * @var string
   */
  private $ext_uid;
  
  /**
   * User email
   * @var string
   */
  private $mail;
  
  
  /**
   * Construct acount
   * @param Provider $provider
   * @param User $user
   */
  public function __construct($provider) {
    $this->provider = $provider;
  }
  
  /**
   * Get provider
   * @return AbstractBaseProvider
   */
  public function getProvider() {
    return $this->provider;
  }
  
  /**
   * Set external user id
   * @param integer $ext_uid
   * @return Account
   */
  public function setExtUserId($ext_uid) {
    $this->ext_uid = $ext_uid;
    return $this;
  }
  
  /**
   * Get external user id
   * @return integer
   */
  public function getExtUserId() {
    return $this->ext_uid;
  }
  
  /**
   * 
   * @param type $mail
   * @return \Drupal\soauth\Common\Entity\Account
   */
  public function setMail($mail) {
    $this->mail = $mail;
    return $this;
  }
  
  /**
   * Get user mail
   * @return string
   */
  public function getMail() {
    return $this->mail;
  }
  
  /**
   * Create account
   * @param AbstractBaseProvider $provider
   * @param User $user
   * @return Account
   */
  static public function create($provider) {
    return new self($provider);
  }
  
  /**
   * Create account from data
   * @param AbstractBaseProvider $provider
   * @param User $user
   * @param array $data
   * @return Account
   */
  static public function fromData($provider, $data) {
    return self::create($provider)
      ->setExtUserId($data['id'])
      ->setMail($data['mail']);
  }
}
