<?php

/**
 * @file
 * Parses a vCard string into a vCard object
 */
class ArchibaldAppDataVcard_Parser {

  protected $content = NULL;
  protected $vcardCacheObject = NULL;

  public function __construct($filename) {
    if (is_file($filename)) {
      $this->content = file_get_contents($filename);
    }
    else {
      $this->content = $filename;
    }
  }

  protected function parseFullname($value, $args = array()) {
    $this->vcardCacheObject->setFullname($value);
  }

  protected function parseName($value, $args = array()) {
    //var_dump($args);
    $value = explode(';', $value);
    if (isset($value[0])) {
      $this->vcardCacheObject->setLastname($value[0]);
    }
    if (isset($value[1])) {
      $this->vcardCacheObject->setFirstname($value[1]);
    }
    if (isset($value[2])) {
      $this->vcardCacheObject->setAdditionalNames($value[2]);
    }
    if (isset($value[3])) {
      $this->vcardCacheObject->setNamePrefix($value[3]);
    }
    if (isset($value[4])) {
      $this->vcardCacheObject->setNameSuffix($value[4]);
    }
  }

  protected function parseBirthday($value, $args = array()) {
    $this->vcardCacheObject->setBirthday($value);
  }

  protected function parseGender($value, $args = array()) {
    $this->vcardCacheObject->setGender($value);
  }

  protected function parseNickname($value, $args = array()) {
    $this->vcardCacheObject->setNickname($value);
  }

  protected function parseUid($value, $args = array()) {
    $this->vcardCacheObject->setUid($value);
  }

  protected function parseRole($value, $args = array()) {
    $this->vcardCacheObject->setRole($value);
  }

  protected function parseTitle($value, $args = array()) {
    $this->vcardCacheObject->setTitle($value);
  }

  protected function parseLogo($value, $args = array()) {
    $obj = new stdClass();
    if (preg_match('/^http:\/\//', $value, $value_parts) || preg_match('/^uri:(.+)/', $value, $value_parts)) {
      $obj->uri = (isset($value_parts[1])) ? $value_parts[1] : $value;
    }
    else {
      $obj->binary = $value;
    }
    $this->vcardCacheObject->setLogo($obj);
  }

  protected function parseOrganization($value, $args = array()) {
    $value = explode(';', $value);
    if (isset($value[0])) {
      $this->vcardCacheObject->setOrganization($value[0]);
    }
    if (isset($value[1])) {
      $this->vcardCacheObject->setDepartment($value[1]);
    }
    if (isset($value[2])) {
      $this->vcardCacheObject->setSubDepartment($value[2]);
    }
  }

  protected function parseRevision($value, $args = array()) {
    $this->vcardCacheObject->setRevision($value);
  }

  protected function parseVersion($value, $args = array()) {
    //$this->vcardCacheObject->setVersion($value, $args);
  }

  protected function parseAddress($value, $args = array()) {
    $sub_array = array();
    foreach ($args as $type_cache) {
      $type_cache = drupal_strtoupper($type_cache);
      if (strpos($type_cache, 'TYPE=') === 0) {
        $sub_array = array_merge($sub_array, explode(',', drupal_substr($type_cache, 5)));
      }
      else {
        $sub_array[] = $type_cache;
      }
    }
    $value = explode(';', $value);
    $this->vcardCacheObject->addAddress(
      array(
        'postofficeaddress' => (isset($value['0']) ? $value['0'] : ''),
        'extendedaddress' => (isset($value['1']) ? $value['1'] : ''),
        'street' => (isset($value['2']) ? $value['2'] : ''),
        'city' => (isset($value['3']) ? $value['3'] : ''),
        'state' => (isset($value['4']) ? $value['4'] : ''),
        'zip' => (isset($value['5']) ? $value['5'] : ''),
        'country' => (isset($value['6']) ? $value['6'] : ''),
        'type' => $sub_array,
      )
    );
  }

  protected function parseTelephone($value, $args = array()) {
    $sub_array = array();
    foreach ($args as $type_cache) {
      $type_cache = drupal_strtoupper($type_cache);
      if (strpos($type_cache, 'TYPE=') === 0) {
        $sub_array = array_merge($sub_array, explode(",", drupal_substr($type_cache, 5)));
      }
      else {
        $sub_array[] = $type_cache;
      }
    }
    $this->vcardCacheObject->addPhonenumber($value, $sub_array);
  }

  protected function parseEmail($value, $args = array()) {
    $sub_array = array();
    foreach ($args as $type_cache) {
      $type_cache = drupal_strtoupper($type_cache);
      if (strpos($type_cache, 'TYPE=') === 0) {
        $sub_array = array_merge($sub_array, explode(",", drupal_substr($type_cache, 5)));
      }
      else {
        $sub_array[] = $type_cache;
      }
    }

    $this->vcardCacheObject->addEmail($value, $sub_array);
  }

  protected function parseUrl($value, $args = array()) {
    $sub_array = array();
    foreach ($args as $type_cache) {
      $type_cache = drupal_strtoupper($type_cache);
      if (strpos($type_cache, 'TYPE=') === 0) {
        $sub_array = array_merge($sub_array, explode(",", drupal_substr($type_cache, 5)));
      }
      else {
        $sub_array[] = $type_cache;
      }
    }
    $this->vcardCacheObject->addUrl($value, $sub_array);
  }

  protected function parseIm($value, $messenger, $args = array()) {
    $sub_array = array();
    foreach ($args as $type_cache) {
      $type_cache = drupal_strtoupper($type_cache);
      if (strpos($type_cache, 'TYPE=') === 0) {
        $sub_array = array_merge($sub_array, explode(",", drupal_substr($type_cache, 5)));
      }
      else {
        $sub_array[] = $type_cache;
      }
    }

    $this->vcardCacheObject->addInstantmessenger($value, $messenger, $sub_array);
  }

  protected function parseGeo($value, $args = array()) {
    $this->vcardCacheObject->setGeolocation($value);
  }

  protected function parseMailer($value, $args = array()) {
    $this->vcardCacheObject->setMailer($value);
  }

  protected function parseTimezone($value, $args = array()) {
    $this->vcardCacheObject->setTimezone($value);
  }

  // Returns Vcard Object
  public function parse() {
    $vcard_object_array = array();
    $commands = array(
      "FN" => "fullname",
      "N" => "name",
      "BDAY" => "birthday",
      "ADR" => "address",
      "TEL" => "telephone",
      "EMAIL" => "email",
      "X-GENDER" => 'gender',
      "REV" => 'revision',
      "VERSION" => 'version',
      "ORG" => 'organization',
      "UID" => 'uid',
      "URL" => 'url',
      "TITLE" => 'title',
      "ROLE" => 'role',
      "LOGO" => 'logo',
      "NICKNAME" => 'nickname',
      "X-AIM" => 'im',
      "X-ICQ" => 'im',
      "X-JABBER" => 'im',
      "X-MSN" => 'im',
      "X-YAHOO" => 'im',
      "X-SKYPE" => 'im',
      "X-SKYPE-USERNAME" => 'im',
      "X-GADUGADU" => 'im',
      "GEO" => 'geo',
      "MAILER" => 'mailer',
      "TZ" => 'timezone',
    );
    $this->content = str_replace(array(ArchibaldAppDataVcard::LINEBREAK, "\r"), "\n", $this->content);

    // RFC2425 5.8.1.  Line delimiting and folding
    // Unfolding is accomplished by regarding CRLF immediately
    //followed by a white space character (namely HTAB ASCII decimal 9 or
    //SPACE ASCII decimal 32) as equivalent to no characters at all (i.e.,
    //the CRLF and single white space character are removed).
    $this->content = preg_replace("/\n(?:[ \t])/", "", $this->content);
    //var_dump($this->content);
    $lines = explode("\n", $this->content);

    foreach ($lines as $line) {
      $line = trim($line);
      //echo $f . $line;
      if (drupal_strtoupper($line) == "BEGIN:VCARD") {
        //echo 'Begin';
        $this->vcardCacheObject = new ArchibaldAppDataVcard();
      }
      elseif (drupal_strtoupper($line) == "END:VCARD") {
        //echo 'End!!!';
        $vcard_object_array[] = $this->vcardCacheObject;
      }
      elseif ($line != NULL) {
        $type = '';
        $value = '';
        //var_dump(explode(':', $line, 2));
        @list($type, $value) = explode(':', $line, 2);
        $types = explode(';', $type);

        $command = drupal_strtoupper($types[0]);

        array_shift($types);
        $i = 0;

        foreach ($types as $type) {

          if (preg_match("/base64/", drupal_strtolower($type))) {
            $value = base64_decode($value);
            unset($types[$i]);
          }
          elseif (preg_match("/encoding=b/", drupal_strtolower($type))) {
            $value = base64_decode($value);
            unset($types[$i]);
          }
          elseif (preg_match("/quoted-printable/", drupal_strtolower($type))) {
            $value = quoted_printable_decode($value);
            unset($types[$i]);
          }
          elseif (strpos(drupal_strtolower($type), 'charset=') === 0) {
            try {
              $value = mb_convert_encoding(
                $value, "UTF-8", drupal_substr($type, 8)
              );
            }
            catch (Exception $e) {

            }

            unset($types[$i]);
          }
          $i++;
        }
        if (in_array(drupal_strtoupper($command), array_keys($commands))) {
          if ($commands[$command] == 'im') {
            call_user_func(array($this, "parse" . drupal_ucfirst($commands[$command])), $value, $command, $types);
          }
          elseif (isset($types[0])) {
            call_user_func(array($this, "parse" . drupal_ucfirst($commands[$command])), $value, $types);
          }
          else {
            call_user_func(array($this, "parse" . drupal_ucfirst($commands[$command])), $value);
          }
        }
      }
    }

    return $vcard_object_array;
  }
}

