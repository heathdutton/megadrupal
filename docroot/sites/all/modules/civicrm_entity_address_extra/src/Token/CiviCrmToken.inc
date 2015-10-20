<?php


abstract class CiviCrmToken extends \CiviCrmDrupalEntity{
  protected $baseToken;
  protected $needsType;

  /**
   * Check if the call from hook_tokens if related to this object or not.
   *
   * @param $type
   * @param string $token
   * @param array $data
   * @param array $options
   * @return bool
   */
  public function isTargetToken($type, $token, array $data = array(), array $options = array()) {
    // If we have a specific data type that is required, check if this is it, else, continue.
    if(empty($this->needsType)
      || (!empty($this->needsType)
        && $type == $this->needsType
        && isset($data[$this->needsType]))) {

      if(stripos($token, $this->baseToken . ':') !== false) {
        return true;
      }
    }

    return false;
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
  function tokens($type, $tokens, array $data = array(), array $options = array()) {
    foreach ($tokens as $token => $original) {
      if ($this->isTargetToken($type, $token, $data)) {
        $token_pieces = $this->getTokenPieces($token);
        // The second piece contains the location type we need to lookup.
        $location_type = $token_pieces[1];
        // The last piece is the property we need to fetch.
        $property = $token_pieces[count($token_pieces) - 1];
        $property = str_replace('-', '_', $property);
        $correction = $this->render($token,$original, $data[$this->needsType], $property, $location_type);
        if (!empty($correction)) {
          $replacements[$tokens[$token]] = $correction;
        }
      }
    }

    return $replacements;
  }

  /**
   * The render process that is called by the Drupal theming layer.
   *
   * @param $token
   * @param $original
   * @param $needs_data
   * @param $entity_property
   * @param $location_type
   * @internal param $class_data
   * @return string
   */
  function render($token, $original, $needs_data, $entity_property, $location_type) {
    $data = $this->getClassDataObject($location_type, $needs_data);
    // Use Drupal's theming system so this can be extended easily.
    $context = str_ireplace($this->baseToken . ':', '', $token);
    $context = preg_replace('/[^A-z0-9]/', '_', $context);
    $themed_output = theme('civicrm_entity_address_extra_token__' . $context, array(
      'controller' => $this,
      'token' => $token,
      'original' => $original,
      'data' => array(
        'raw' => $data,
        'needs_data' => $needs_data,
        'location_type' => $location_type,
      ),
      'entity_property' => $entity_property,
    ));

    return $themed_output;
  }

  /**
   * An abstract function to retrieve the relevant data to the class.
   *
   * @param string $location_type
   * @param object|array|mixed $needs_data The "needs_data" value defined for this token in hook_token_info().
   * @return mixed
   */
  function getClassDataObject($location_type, $needs_data) {
    $location_types = $this->getLocationTypes();

    $data = $this->genericEntityFetch(
      array(
        'location_type_id' => $location_types[$location_type],
        'contact_id' => $needs_data->id,
      ));

    return (empty($data)
      ? NULL
      : reset($data));

  }

  /**
   * Gets an array of CiviCRM Location Types. Returns an array keyed on the the name with the ID as the value.
   * Uses static caching for repeat calls.
   *
   * @return array
   */
  public static function getLocationTypes() {
    $location_types_results = \CRM_Core_BAO_LocationType::commonRetrieveAll('CRM_Core_DAO_LocationType', 'id', NULL);
    static $location_types;
    if(!$location_types) {
      $location_types = array();
      foreach ($location_types_results as $type) {
        $name = drupal_strtolower($type['name']);
        $location_types[$name] = $type['id'];
      }
    }

    return $location_types;
  }


  /**
   * Breaks a token apart, separating by ":".
   *
   * @param string $token The tokens passed in by hook_tokens. Note: Only the first token in the $tokens array is used.
   * @return array The exploded token.
   */
  protected function getTokenPieces($token) {
    $token_pieces = explode(':', $token);
    return $token_pieces;
  }

  /**
   * Gets the property of an entity via its token equivalent.
   *
   * @param object|array $entity The entity that we need to fetch a property's value from.
   * @param string $property_name The token to use when attempting to retrieve a property.
   * @return mixed
   */
  public function getEntityPropertyValue($entity, $property_name) {
    $token = $this->buildPropertyToken($property_name);
    // Get the data source from the $data by using the entity_type as the array key.
    return token_replace(
      $token,
      array(
        $this->entityType => $entity),
      array(
        'clear' => true,
        'sanitize' => false));
  }

  /**
   * Builds a token for an entity and a target property.
   * @param string $property_name The target property name of an entity.
   * @return string
   */
  protected function buildPropertyToken($property_name) {
    $property_name = str_replace('_', '-', $property_name);
    return '[' . $this->entityType . ':' . $property_name . ']';
  }


  /**
   * Define this token for hook_token_info().
   * @return array
   */
  abstract function info();
}

