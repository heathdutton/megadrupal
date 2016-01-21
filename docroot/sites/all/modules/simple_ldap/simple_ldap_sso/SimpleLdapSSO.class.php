<?php

/**
 * @file
 * SimpleLdapSSO object.
 */

class SimpleLdapSSO {
  /**
   * The dn of the user.
   *
   * @var string
   */
  protected $dn;

  // This attribute is intentionally private.
  private $hashedSid;

  /**
   * The hashed sid, as stored on LDAP.
   *
   * @var string
   */
  private function getHashedSid() {
    if (!isset($this->server) || !$this->server instanceof SimpleLdapServer) {
      throw new Exception(t('Unable to get hashed SID. LDAP server does not exist.'));
    }
    if (!isset($this->hashedSid)) {
      // Search for this attribute.
      $result = $this->server->search($this->dn, 'objectclass=*', 'base', array($this->getSidAttribute()));
      // Clean up the result.
      $clean = SimpleLdap::clean($result);
      // Only set the value if we actually got a result.
      if (isset($clean[$this->dn][$this->getSidAttribute()][0])) {
        $this->hashedSid = $clean[$this->dn][$this->getSidAttribute()][0];
      }
    }
    return $this->hashedSid;
  }

  /**
   * The LDAP server.
   *
   * @var SimpleLdapServer
   */
  protected $server;

  // This attribute is intentionally private.
  private $sidAttribute;

  /**
   * The attribute name where the sid will be stored.
   *
   * @var string
   */
  public function getSidAttribute() {
    if (!isset($this->sidAttribute)) {
      $this->sidAttribute = variable_get('simple_ldap_sso_attribute_sid', FALSE);
    }
    return $this->sidAttribute;
  }

  /**
   * An array of singletons per user.
   *
   * @var array
   */
  protected static $users = array();

  /**
   * Return a SimpleLdapSSO object for the given username.
   */
  public static function singleton($name) {
    if (!isset(self::$users[$name])) {
      self::$users[$name] = new SimpleLdapSSO($name);
    }

    return self::$users[$name];
  }

  /**
   * Constructor.
   *
   * @param string $name
   *   The Drupal username.
   */
  public function __construct($name) {
    $parameters = array(
      'binddn' => variable_get('simple_ldap_sso_binddn'),
      'bindpw' => variable_get('simple_ldap_sso_bindpw'),
      'readonly' => FALSE,
    );
    // If this site is in RO mode, use a separate server connection with the
    // above RW credentials.
    $this->server = variable_get('simple_ldap_readonly') ?
      new SimpleLdapServer($parameters) : SimpleLdapServer::singleton();

    // Get the LDAP configuration.
    $ldap_user = SimpleLdapUser::singleton($name);
    $this->dn = $ldap_user->dn;
  }

  /**
   * Save a plain text sid to LDAP, hashing first.
   */
  public function saveSid($sid) {
    $hashed = $this->hashSid($sid);
    $attributes[$this->getSidAttribute()] = $hashed;
    if (!$this->server->modify($this->dn, $attributes, 'replace')) {
      throw new Exception('Unable to save session id to LDAP.');
    }
  }

  /**
   * Delete the sid from LDAP.
   */
  public function deleteSid() {
    $attributes[$this->getSidAttribute()] = array();
    if (!$this->server->modify($this->dn, $attributes, 'delete')) {
      throw new Exception('Unable to delete session id from LDAP.');
    }
    $this->hashedSid = NULL;
  }

  /**
   * Validate an sid against the stored value on LDAP.
   */
  public function validateSid($sid) {
    return $this->getHashedSid() == $this->hashSid($sid);
  }

  /**
   * Hash an sid, using the current hashing method.
   *
   * This method is intentionally private.
   */
  private function hashSid($sid) {
    $algorithm = variable_get('simple_ldap_sso_hashing_algorithm', 'sha');
    return SimpleLdap::hash($sid, $algorithm);
  }
}
