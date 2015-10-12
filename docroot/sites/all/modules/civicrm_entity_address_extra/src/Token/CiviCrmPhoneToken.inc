<?php

class CiviCrmPhoneToken extends \CiviCrmToken {

  function __construct() {
    $this->baseToken = 'phone';
    $this->entityType = 'civicrm_phone';
    $this->needsType = 'civicrm_contact';
  }

  /**
   * Define this token for hook_token_info().
   * @return array
   */
  function info() {
    $location_types = $this->getLocationTypes();
    $location_types = array_keys($location_types);

    return array(
      'tokens' => array(
        $this->needsType => array(
          $this->baseToken => array(
            'name' => 'Phone Number',
            'description' => 'Phone number for a CiviCRM Contact. Replace "?" with the following location types: ' .
              join(', ', $location_types),
            'dynamic' => TRUE,
            'type' => $this->entityType,
          ),
        ),
      ),
    );
  }
}