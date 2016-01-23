<?php


class CiviCrmAddressToken extends \CiviCrmToken {

  function __construct() {
    $this->baseToken = 'addresses';
    $this->entityType = 'civicrm_address';
    $this->needsType = 'civicrm_contact';
  }

  /**
   * Should be called in the hook_tokens().
   *
   * @param $type
   * @param $tokens
   * @param array $data
   * @param array $options
   * @return mixed
   */
/*  function tokens($type, $tokens, array $data = array(), array $options = array()) {
    foreach ($tokens as $token => $original) {
      if ($this->isTargetToken($type, $token, $data)) {
        $token_pieces = $this->getTokenPieces($token);
        // The second piece contains the location type we need to lookup.
        $location_type = $token_pieces[1];
        $address = $this->getClassDataObject($location_type, $data[$this->needsType]);
        // The last piece is the property we need to fetch.
        $property = $token_pieces[count($token_pieces) - 1];
        $correction = $this->getEntityPropertyValue($address, $property);
        if (!empty($correction)) {
          $replacements[$tokens[$token]] = $correction;
        }
      }
    }

    return $replacements;
  }
*/

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
            'name' => 'Addresses',
            'description' => 'Addresses for a CiviCRM Contact. Replace "?" with the following location types: ' .
              join(', ', $location_types),
            'dynamic' => TRUE,
            'type' => $this->entityType,
          ),
        ),
      ),
    );
  }

}

