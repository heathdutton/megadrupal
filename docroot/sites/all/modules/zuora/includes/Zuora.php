<?php

interface ZuoraInterface {
  /**
   * @return ZuoraInterface
   */
  public static function instance();
  public function isSandboxed();
}

/**
 * Class Zuora.
 */
abstract class Zuora implements ZuoraInterface {
  /**
   * @var null
   */
  protected $tenant_user_id;

  /**
   * @var null
   */
  protected $tenant_password;

  /**
   * @var null
   */
  protected $is_sandbox;

  /**
   * Sets the API credential information.
   *
   * @throws ZuoraException
   *
   */
  public function __construct() {
    $this->tenant_user_id = variable_get('zuora_api_access_key_id', NULL);
    $this->tenant_password = variable_get('zuora_api_secret_access_key', NULL);

    if ($this->tenant_user_id == NULL || $this->tenant_password == NULL) {
      throw new ZuoraException(t('You must set your Zuora credentials'));
    }
    $this->is_sandbox = variable_get('zuora_api_environment', TRUE);
  }

  /**
   * Returns if the API is running in sandbox mode.
   *
   * @return boolean
   */
  public function isSandboxed() {
    return $this->is_sandbox;
  }

  /**
   * Returns tenant ID
   *
   * @return null|string
   */
  public function getTenantId() {
    return variable_get('zuora_api_tenant_id', NULL);
  }

}
