<?php
/**
 * @file
 * Variable Realm controller.
 */

/**
 * Controller for Language realms.
 */
class MobileDomainVariableRealmController extends VariableRealmDefaultController {
  /**
   * Implementation of VariableRealmControllerInterface::getDefaultKey().
   */
  public function getDefaultKey() {
    return NULL;
  }
  /**
   * Implementation of VariableRealmControllerInterface::getRequestKey().
   */
  public function getRequestKey() {
    return mobile_domain_current_mode();
  }
  /**
   * Implementation of VariableRealmControllerInterface::getAllKeys().
   */
  public function getAllKeys() {
    return mobile_domain_mode_list();
  }
}
