<?php

/**
 * @file
 * Whatsapp controller class implementation.
 */

require_once 'lib/whatsapp.class.inc';

/**
 * Class implementation.
 */
class WhatsappController extends WhatsApp {

  /**
   * Constant declarations.
   */
  const undefined = 01;
  const missing = 02;
  
  /**
   * Destionation where the messages will be sent to.
   */
  protected $destination;

  /**
   * Empty constructor.
   */
  function __construct() {
    $this->setUp();
    parent::__construct(
      $this->getPhoneCountryCode(),
      $this->getPhoneNumber(),
      $this->getIdentity(),
      $this->getUserName(),
      $this->getCountryCode(),
      $this->getLangCode(),
      $this->getDebug()
    );
  }

  /**
   * Sets up the oject to be initialized.
   * 
   * @return WhatsappController
   *   Returns itself.
   * 
   * @throws ErrorException
   */
  public function setUp() {
    // Set default values if they have not been set.
    if (empty($this->phoneCountryCode) || empty($this->phoneNumber)) {
      $phone = variable_get('whatsapp_sitewide_phone', '');
      $dissectPhone = $this->get_container('util')->dissect($phone);
      $this
        ->setPhoneCountryCode($dissectPhone['cc'])
        ->setPhoneNumber($dissectPhone['phone']);
    }
    if (empty($this->identity)) {
      $identifier = variable_get('whatsapp_sitewide_identifier', '');
      $this->setIdentity($identifier);
    }
    if (empty($this->phoneCountryCode) ||
        empty($this->phoneNumber) ||
        empty($this->identity)) {
      throw new ErrorException(t('Missing parameter required parameter.'), WhatsappController::missing, WATCHDOG_ERROR);
    }
    // Set default values if they have not been set.
    if (empty($this->userName)) {
      $this->setUserName(variable_get('whatsapp_sitewide_username', variable_get('site_name')));
    }
    if (empty($this->userStatus)) {
      $this->setUserStatus(variable_get('whatsapp_sitewide_status', 'Using Drupal WhatsApp!'));
    }
    if (empty($this->countryCode)) {
      $this->setCountryCode('US');
    }
    if (empty($this->langCode)) {
      $this->setLangCode('en');
    }
    if (empty($this->debug)) {
      $this->setDebug(FALSE);
    }
    return $this;
  }

  /**
   * Initializes the object.
   */
  public function init() {
    // Exclude setters from parent constructor.
    $dict = getDictionary();
    $this->writer = new BinTreeNodeWriter($dict);
    $this->reader = new BinTreeNodeReader($dict);
    $this->loginStatus = $this->disconnectedStatus;
  }
  
  /**
   * Sends a message.
   * 
   * This method is a wrapper that allows performing some setup and data caching
   * 
   * @param string $message
   *   Short text message.
   * 
   * @return WhatsappController
   *   Returns itself.
   */
  public function sendMessage($message) {
    if ($this->prepare()) {
      $this->Message($this->getDestination(), $message);
      return $this;
    }
    return FALSE;
  }
  
  /**
   * Sends an image.
   * 
   * This method is a wrapper that allows performing some setup and data caching
   * 
   * @param stdClass $image
   *   Image object as returned from image_load.
   * 
   * @return WhatsappController
   *   Returns itself.
   */
  public function sendImage($image) {
    if ($this->prepare()) {
      $this->MessageImage(
        $this->getDestination(),
        // file_create_url($image->source),
        'http://farm9.staticflickr.com/8327/8429584484_891030bedc_b.jpg',
        drupal_basename($image->source),
        // $image->info['file_size'],
        260626,
        $this->get_container('util')->createIconB64($image)
      );
      return $this;
    }
    return FALSE;
  }
  
  /**
   * Prepares the connection to send a node.
   * 
   * @return WhatsappController
   *   Returns itself.
   */
  public function prepare() {
    if (empty($this->password)) {
      $credentials = $this->checkCredentials();
      if ($credentials->status == 'ok') {
        $this->setPassword($credentials->pw);
      }
    }
    if ($this->loginStatus == $this->disconnectedStatus) {
      $this->Connect();
      $this->Login();
    }
    return $this;
  }

  /**
   * Factory method.
   * 
   * @param string
   *   Container name.
   * 
   * @return mixed
   *   Container class.
   */
  public function get_container($name) {
    switch ($name) {
      case 'util':
        return new WhatsappUtils();
        break;
    }
  }

  // Setters and getters

  /**
   * Global setter.
   * 
   * @param string $key
   *   Name of the property.
   * @param mixed $value
   *   Value to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  protected function set($key, $value) {
    // Check if the specified property is defined.
    if (!property_exists($this, $key)) {
      throw new ErrorException(t('The property %property is not defined.', array('%property' => $key)), WhatsappController::undefined, WATCHDOG_ERROR);
    }
    if ($this->getDebug()) {
      watchdog('WhatsApp', t('Variable %var has been set to: %val.', array('%var' => $key, '%val' => $value)), array(), WATCHDOG_DEBUG);
    }
    $this->{$key} = $value;
    return $this;
  }

  /**
   * Setter for WhatsApp::phoneCountryCode property
   * 
   * @param $phoneCountryCode
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setPhoneCountryCode($phoneCountryCode) {
    return $this->set('phoneCountryCode', $phoneCountryCode);
  }

  /**
   * Getter for WhatsApp::phoneCountryCode property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getPhoneCountryCode() {
    return $this->phoneCountryCode;
  }

  /**
   * Setter for WhatsApp::phoneNumber property
   * 
   * @param $phoneNumber
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setPhoneNumber($phoneNumber) {
    return $this->set('phoneNumber', $phoneNumber);
  }

  /**
   * Getter for WhatsApp::phoneNumber property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getPhoneNumber() {
    return $this->phoneNumber;
  }

  /**
   * Setter for WhatsApp::identity property
   * 
   * @param $identity
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setIdentity($identity) {
    return $this->set('identity', $identity);
  }

  /**
   * Getter for WhatsApp::identity property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getIdentity() {
    return $this->identity;
  }

  /**
   * Setter for WhatsApp::userName property
   * 
   * @param $userName
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setUserName($userName) {
    return $this->set('userName', $userName);
  }

  /**
   * Getter for WhatsApp::userName property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getUserName() {
    return $this->userName;
  }

  /**
   * Setter for WhatsApp::userStatus property
   * 
   * @param $userStatus
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setUserStatus($userStatus) {
    return $this->set('userStatus', $userStatus);
  }

  /**
   * Getter for WhatsApp::userStatus property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getUserStatus() {
    return $this->userStatus;
  }

  /**
   * Setter for WhatsApp::countryCode property
   * 
   * @param $countryCode
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setCountryCode($countryCode) {
    return $this->set('countryCode', $countryCode);
  }

  /**
   * Getter for WhatsApp::countryCode property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getCountryCode() {
    return $this->countryCode;
  }

  /**
   * Setter for WhatsApp::langCode property
   * 
   * @param $langCode
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setLangCode($langCode) {
    return $this->set('langCode', $langCode);
  }

  /**
   * Getter for WhatsApp::langCode property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getLangCode() {
    return $this->langCode;
  }

  /**
   * Setter for WhatsApp::debug property
   * 
   * @param $debug
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setDebug($debug) {
    return $this->set('debug', $debug);
  }

  /**
   * Getter for WhatsApp::debug property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getDebug() {
    return $this->debug;
  }

  /**
   * Setter for WhatsApp::destination property
   * 
   * @param $destination
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setDestination($destination) {
    return $this->set('destination', $destination);
  }

  /**
   * Getter for WhatsApp::destination property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getDestination() {
    return $this->destination;
  }
}
