<?php

/**
 * @file
 * provides vCard Object
 * http://en.wikipedia.org/wiki/VCard
 */
require_once dirname(__FILE__) . '/Vcard/Generator.php';
require_once dirname(__FILE__) . '/Vcard/Parser.php';

/**
 * Documentation
 * @example http://code.google.com/p/zendvcard/source/browse/trunk/test/library/App/Data/ArchibaldAppDataVcardTest.php?r=3
 */
class ArchibaldAppDataVcard {
  const VERSION21 = '2.1';
  const VERSION30 = '3.0';
  const LINEBREAK = "\r\n";

  protected $uid = NULL;
  protected $uri = NULL;
  protected $binary = NULL;
  protected $title = NULL;
  protected $fullname = NULL;
  protected $lastname = NULL;
  protected $firstname = NULL;
  protected $nickname = NULL;
  protected $additionalnames = NULL;
  protected $nameprefix = NULL;
  protected $namesuffix = NULL;
  protected $birthday = NULL;
  protected $organization = NULL;
  protected $department = NULL;
  protected $subdepartment = NULL;
  protected $role = NULL;
  protected $revision = NULL;
  protected $geolocation = NULL;
  protected $mailer = NULL;
  protected $timezone = NULL;
  protected $logo = NULL;

  /**
   * @var array array(postofficeaddress, extendedaddress, street, city, state, zip, country, type=array())
   */
  protected $address = array();
  const ADDRESSDOMESTIC = 'DOM';
  const ADDRESSINTERNATIONAL = 'INTL';
  const ADDRESSSPOSTAL = 'POSTAL';
  const ADDRESSPARCEL = 'PARCEL';
  const ADDRESSHOME = 'HOME';
  const ADDRESSWORK = 'WORK';
  protected $addressConstantArray = array(
    self::ADDRESSDOMESTIC,
    self::ADDRESSHOME,
    self::ADDRESSINTERNATIONAL,
    self::ADDRESSPARCEL,
    self::ADDRESSSPOSTAL,
    self::ADDRESSWORK,
  );

  /**
   * @var array array(value, type=array())
   */
  protected $telephone = array();
  const TELEPHONEPREF = 'PREF';
  const TELEPHONEWORK = 'WORK';
  const TELEPHONEHOME = 'HOME';
  const TELEPHONEOTHER = 'OTHER';
  const TELEPHONEVOICE = 'VOICE';
  const TELEPHONEFAX = 'FAX';
  const TELEPHONEMSG = 'MSG';
  const TELEPHONECELL = 'CELL';
  const TELEPHONECAR = 'CAR';
  protected $telephoneConstantArray = array(
    self::TELEPHONECAR,
    self::TELEPHONECELL,
    self::TELEPHONEFAX,
    self::TELEPHONEHOME,
    self::TELEPHONEMSG,
    self::TELEPHONEOTHER,
    self::TELEPHONEPREF,
    self::TELEPHONEVOICE,
    self::TELEPHONEWORK,
  );

  /**
   * @var array array(value, type=array())
   */
  protected $email = array();
  const EMAILINTERNET = 'INTERNET';
  const EMAILWORK = 'WORK';
  const EMAILHOME = 'HOME';
  const EMAILOTHER = 'OTHER';
  const EMAILPREF = 'PREF';
  protected $emailConstantArray = array(
    self::EMAILHOME,
    self::EMAILINTERNET,
    self::EMAILOTHER,
    self::EMAILPREF,
    self::EMAILWORK,
  );

  /**
   * @var array array(value, type(array())
   */
  protected $url = array();
  const URLWORK = 'WORK';
  const URLHOME = 'HOME';
  const URLOTHER = 'OTHER';
  const URLPREF = 'PREF';
  protected $urlConstantArray = array(
    self::URLHOME,
    self::URLOTHER,
    self::URLPREF,
    self::URLWORK,
  );

  /**
   * @var array array(value, type=array(), $messenger=NULL)
   */
  protected $im = array();
  const IMHOME = 'HOME';
  const IMWORK = 'WORK';
  const IMPREF = 'PREF';
  const IMOTHER = 'OTHER';
  const IMAIM = 'X-AIM';
  const IMICQ = 'X-ICQ';
  const IMJABBER = 'X-JABBER';
  const IMMSN = 'X-MSN';
  const IMYAHOO = 'X-YAHOO';
  const IMSKYPE = 'X-SKYPE';
  const IMSKYPEALTERNATIVE = 'X-SKYPE-USERNAME';
  const IMGADUGADU = 'X-GADUGADU';

  protected $imMessengerConstantArray = array(
    self::IMAIM,
    self::IMICQ,
    self::IMJABBER,
    self::IMMSN,
    self::IMYAHOO,
    self::IMSKYPE,
    self::IMSKYPEALTERNATIVE,
    self::IMGADUGADU,
  );
  protected $imConstantArray = array(
    self::IMHOME,
    self::IMOTHER,
    self::IMPREF,
    self::IMWORK,
  );

  protected $gender = NULL;

  const GENDERMALE = 'Male';
  const GENDERFEMALE = 'Female';

  public function __call($function, $args) {
    $function = drupal_strtolower($function);
    $property = drupal_substr($function, 3);


    if (drupal_substr($function, 0, 3) == 'set' && property_exists($this, $property) && !is_array($this->{$property})) {
      if (is_string($args[0]) || is_int($args[0]) || is_object($args[0])) {
        $this->$property = $args[0];

        return $this;
      }
    }

    throw new Exception("Unknown property $property");
  }

  public function __get($name) {
    if (property_exists($this, $name)) {
      return $this->{$name};
    }
    else {
      throw new Exception("Unknown property $name");
    }
  }

  public function __set($name, $value) {
    if (property_exists($this, $name) && !is_array($this->{$name})) {
      $this->{$name} = $value;
    }
    else {
      throw new Exception("Unknown property $name");
    }
  }

  public function __unset($name) {
    if (property_exists($this, $name)) {
      if (is_array($this->{$name})) {
        $this->{$name} = array();
      }
      else {
        $this->{$name} = NULL;
      }
    }
    else {
      throw new Exception("Unknown property $name");
    }
  }

  /**
   * add Telephone number to vCard
   *
   * @param mixed $phonenumber_array
   * @param string $type
   * @return ArchibaldAppDataVcard
   *
   * @example
   * $v_card->addPhonenumber('123456');
   * $v_card->addPhonenumber('234567', ArchibaldAppDataVcard::TELEPHONEHOME);
   * $v_card->addPhonenumber(
   *   array(
   *     array(
   *       'value' => '345678',
   *       'type' => array(ArchibaldAppDataVcard::TELEPHONEHOME)
   *     ),
   *     array(
   *       'value' => '456789',
   *       'type' => array(
   *         ArchibaldAppDataVcard::TELEPHONEWORK,
   *         ArchibaldAppDataVcard::TELEPHONEVOICE
   *       )
   *     )
   *   )
   * );
   */
  public function addPhonenumber($phonenumber_array, $type = NULL) {
    if (!is_array($phonenumber_array)) {
      $phonenumber_cache_array = array(array("value" => $phonenumber_array));

      if (is_array($type)) {
        $phonenumber_cache_array[0]['type'] = $type;
      }
      else {
        $phonenumber_cache_array[0]['type'][] = $type;
      }

      $phonenumber_array = $phonenumber_cache_array;
    }

    foreach ($phonenumber_array as $value) {
      $type_array = array();
      foreach ($value['type'] as $telephone_type) {
        if (in_array($telephone_type, $this->telephoneConstantArray)) {
          $type_array[] = $telephone_type;
        }
      }
      $this->telephone[] = array('value' => $value['value'], 'type' => $type_array);
    }
    return $this;
  }

  /**
   * add E-Mail address to vCard
   *
   * @param mixed $email_array
   * @param string $type
   * @return ArchibaldAppDataVcard
   *
   * @example
   * $v_card->addEmail('schaaf@komola.de');
   * $v_card->addEmail('thomas.schaaf@komola.de', ArchibaldAppDataVcard::EMAILHOME);
   * $v_card->addEmail(
   *   array(
   *     array(
   *       'value' => 'thomas@komola.de',
   *       'type' => array(ArchibaldAppDataVcard::EMAILINTERNET)
   *     ),
   *     array(
   *       'value' => 'asd@komola.de',
   *       'type' => array(
   *         ArchibaldAppDataVcard::EMAILOTHER,
   *         ArchibaldAppDataVcard::EMAILINTERNET
   *       )
   *     )
   *   )
   * );
   */
  public function addEmail($email_array, $type = NULL) {
    if (!is_array($email_array) && is_string($email_array)) {
      $email_cache_array = array();
      $email_cache_array[0]['value'] = $email_array;
      if (is_array($type)) {
        $email_cache_array[0]['type'] = $type;
      }
      else {
        $email_cache_array[0]['type'][] = $type;
      }
      $email_array = $email_cache_array;
    }
    foreach ($email_array as $value) {
      $type_array = array();
      foreach ($value['type'] as $email_type) {
        if (in_array($email_type, $this->emailConstantArray)) {
          $type_array[] = $email_type;
        }
      }
      $this->email[] = array('value' => $value['value'], 'type' => $type_array);
    }
    return $this;
  }

  /**
   * add url to vCard
   *
   * @param mixed $url_array
   * @param string $type
   * @return ArchibaldAppDataVcard
   *
   * @example
   * $v_card->addUrl('http://vsphp.de');
   * $v_card->addUrl('http://suplify.me', ArchibaldAppDataVcard::URLWORK);
   * $v_card->addUrl(
   *   array(
   *     array(
   *       'value' => 'http://komola.de',
   *       'type' => array(ArchibaldAppDataVcard::URLWORK)
   *     ),
   *     array(
   *       'value' => 'http://www.komola.de',
   *       'type' => array(
   *         ArchibaldAppDataVcard::URLPREF,
   *         ArchibaldAppDataVcard::URLWORK
   *       )
   *     )
   *   )
   * );
   */
  public function addUrl($url_array, $type = NULL) {
    if (!is_array($url_array) && is_string($url_array)) {
      $url_cache_array = array();
      $url_cache_array[0]['value'] = $url_array;
      if (is_array($type)) {
        $url_cache_array[0]['type'] = $type;
      }
      else {
        $url_cache_array[0]['type'][] = $type;
      }
      $url_array = $url_cache_array;
    }
    foreach ($url_array as $value) {
      $type_array = array();
      foreach ($value['type'] as $url_type) {
        if (in_array($url_type, $this->urlConstantArray)) {
          $type_array[] = $url_type;
        }
      }
      $this->url[] = array('value' => $value['value'], 'type' => $type_array);
    }
    return $this;
  }

  /**
   * Add new logo url to vCard.
   *
   * @param string $url
   *   The url to file.
   */
  public function setLogoUri($url) {
    if (empty($this->logo)) {
      $this->logo = new stdClass();
    }

    $this->logo->uri = $url;
  }

  /**
   * Add new binary logo to vCard.
   *
   * @param string $binary
   *   The jpg binary string.
   */
  public function setLogoBinary($binary) {
    if (empty($this->logo)) {
      $this->logo = new stdClass();
    }

    $this->logo->binary = $binary;
  }

  /**
   * add address to vCard
   *
   * @param array $address_array
   * @return ArchibaldAppDataVcard
   *
   * @example
   * $v_card->addAddress(
   *   array(
   *     'postofficeaddress' => '',
   *     'extendedaddress' => '',
   *     'street' => 'Examplestreet',
   *     'city' => 'City',
   *     'state' => 'Somestate',
   *     'zip' => '12341',
   *     'country' => 'Country'
   *   )
   * );
   *
   * $v_card->addAddress(
   *   array(
   *     'postofficeaddress' => '',
   *     'extendedaddress' => 'first Floor',
   *     'street' => 'Examplestreet',
   *     'city' => 'City',
   *     'state' => 'Somestate',
   *     'zip' => '12341',
   *     'country' => 'Country',
   *     'type' => array(ArchibaldAppDataVcard::ADDRESSHOME, ArchibaldAppDataVcard::ADDRESSINTERNATIONAL)
   *   )
   * );
   *
   * $v_card->addAddress(
   *   array(
   *     array(
   *      'postofficeaddress' => 'fgh',
   *      'extendedaddress' => 'first Floor',
   *      'street' => 'Examplestreet',
   *      'city' => 'City',
   *      'zip' => '12341',
   *      'country' => 'Country',
   *      'type' => ArchibaldAppDataVcard::ADDRESSHOME
   *    ),
   *    array(
   *      'country' => 'Country',
   *      'type' => array(ArchibaldAppDataVcard::ADDRESSHOME, ArchibaldAppDataVcard::ADDRESSSPOSTAL))
   *    )
   * );
   */
  public function addAddress($address_array) {
    if (isset($address_array['type']) || !isset($address_array[0])) {
      $address_cache_array   = array();
      $address_cache_array[] = $address_array;
      $address_array        = $address_cache_array;
    }
    foreach ($address_array as $value) {
      $type_cache_array = array();
      if (!isset($value['type'])) {
        $value['type'][] = NULL;
      }
      elseif (!is_array($value['type'])) {
        $type_cache_array[] = $value['type'];

        $value['type'] = $type_cache_array;
      }

      $type_array = array();
      foreach ($value['type'] as $address_type) {
        if (in_array($address_type, $this->addressConstantArray)) {
          $type_array[] = $address_type;
        }
      }


      $this->address[] = array(
        'postofficeaddress' => (isset($value['postofficeaddress']) ? $value['postofficeaddress'] : ''),
        'extendedaddress' => (isset($value['extendedaddress']) ? $value['extendedaddress'] : ''),
        'street' => (isset($value['street']) ? $value['street'] : ''),
        'city' => (isset($value['city']) ? $value['city'] : ''),
        'state' => (isset($value['state']) ? $value['state'] : ''),
        'zip' => (isset($value['zip']) ? $value['zip'] : ''),
        'country' => (isset($value['country']) ? $value['country'] : ''),
        'type' => $type_array,
      );
    }
    return $this;
  }

  /**
   * add instant messanger to vCard
   * @param mixed $im_array
   * @param string $messenger
   * @param string $type
   * @return ArchibaldAppDataVcard
   *
   * @example
   * $v_card->addInstantmessenger('thomas@schaafs.net', ArchibaldAppDataVcard::IMMSN);
   * $v_card->addInstantmessenger('thomas@schaafs.net', ArchibaldAppDataVcard::IMMSN, ArchibaldAppDataVcard::IMWORK);
   * $v_card->addInstantmessenger(
   *   array(
   *     array(
   *       'value' => 'thomaschaaf',
   *       'messenger' => ArchibaldAppDataVcard::IMSKYPE,
   *       'type' => array(ArchibaldAppDataVcard::IMHOME)
   *     ),
   *     array(
   *       'value' => '149795107',
   *       'messenger' => ArchibaldAppDataVcard::IMICQ,
   *       'type' => array(ArchibaldAppDataVcard::IMHOME, ArchibaldAppDataVcard::IMPREF)
   *     )
   *   )
   * );
   */
  public function addInstantmessenger($im_array, $messenger = NULL, $type = NULL) {
    if (!is_array($im_array) && is_string($im_array) && isset($messenger)) {
      $im_cache_array = array();
      $im_cache_array[0]['value'] = $im_array;
      $im_cache_array[0]['messenger'] = $messenger;
      if (is_array($type)) {
        $im_cache_array[0]['type'] = $type;
      }
      else {
        $im_cache_array[0]['type'][] = $type;
      }
      $im_array = $im_cache_array;
    }

    foreach ($im_array as $value) {
      $type_array = array();
      foreach ($value['type'] as $im_array) {
        if (in_array($im_array, $this->imConstantArray)) {
          $type_array[] = $im_array;
        }
      }
      if (in_array($value['messenger'], $this->imMessengerConstantArray)) {
        $this->im[] = array('value' => $value['value'], 'type' => $type_array, 'messenger' => $value['messenger']);
      }
    }
    return $this;
  }
}

