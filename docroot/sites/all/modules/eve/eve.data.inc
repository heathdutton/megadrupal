<?php

/**
 * @File
 * Handles all EVE API objects and data parsing
 */

class eveRequest {
  // User Data
  protected $userID = NULL;
  protected $apiKey = NULL;
  // Api method
  protected $requestURI = NULL;
  private   $baseURI = 'http://api.eve-online.com/';

  // Post request data
  private $data = NULL;

  private $cachedUntil = NULL;
  private $currentTime = NULL;

  public function __construct($userID, $apiKey) {
    $this->userID = $userID;
    $this->apiKey = $apiKey;
  }

  public function get($fields = array()) {
    // Eve servers cache data, so we avoid querying again until cache is rebuilt
    if (time() >= $this->cachedUntil) {
      $curl_handler = curl_init();
      $fields += array(
        'userID' => $this->userID,
        'apiKey' => $this->apiKey,
      );
      curl_setopt($curl_handler, CURLOPT_URL, $this->baseURI . $this->requestURI);
      curl_setopt($curl_handler, CURLOPT_POST, 1);
      curl_setopt($curl_handler, CURLOPT_POSTFIELDS, $fields);
      curl_setopt($curl_handler, CURLOPT_RETURNTRANSFER, 1);
      $this->data = new SimpleXMLElement(curl_exec($curl_handler));
      $this->parseCommon();
    }
    return $this;
  }
  private function parseCommon() {
    if (!isset($this->data->error)) {
      // Avoid cache time flag on erroneous queries
      $this->cachedUntil = strtotime($this->data->cachedUntil);
    }
  }

  public function result($xpath = NULL) {
    if ($xpath) {
      return $this->data->xpath($xpath);
    }
    else {
      return $this->data;
    }
  }

  public function debug() {
    dsm($this->data->asXML());
  }

/**
 * Returns all current error descriptions keyed by error ID
 */
  public function getErrors() {
    $errors = $this->data->xpath('/eveapi/error');
    $return = array();
    foreach ($errors as $error) {
      foreach ($error->attributes() as $index => $value) {
        $return[(string)$value] = $value . ' : ' . $error;
      }
    }
    return $return;
  }
}

class eveAccount extends eveRequest {
  private $characters = NULL;
  public function __construct($userID = NULL, $apiKey = NULL) {
    $this->userID = $userID;
    $this->apiKey = $apiKey;
    $this->requestURI = 'account/Characters.xml.aspx';
  }

  // Parses XML and creates well structured character object
  private function parseCharacter(SimpleXMLElement $char) {
    $character = new stdClass;
    foreach ($char->attributes() as $index => $value) {
      $character->$index = (string)$value;
    }
    return $character;
  }

  // Fetches XML from EVE API and populates the characters list
  private function loadCharacters() {
    foreach ($this->get()->result('result/rowset/row') as $charData) {
      $char = $this->parseCharacter($charData);
      $this->characters[$char->characterID] = $char;
    }
  }

/**
 * Returns all available characters
 */
  public function getCharacters() {
    if (!$this->characters) {
      // Refresh character list
      $this->loadCharacters();
    }
    return isset($this->characters) ? $this->characters : array();
  }

/**
 * Returns character matching $name or NULL
 */
  public function getCharacterByName($name = NULL) {
    foreach ($this->characters as $char) {
      if ($name == $char['name']) {
        return $char;
      }
    }
    return NULL;
  }

/**
 * Returns characters matching $ID or NULL
 */
  public function getCharacterByID($ID = NULL) {
    return isset($this->characters[$ID]) ? $this->characters[$ID] : NULL;
  }
}

