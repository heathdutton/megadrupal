<?php

/**
 * @file
 * generate a vCard string from a vCard object
 */
class ArchibaldAppDataVcardGenerator {

  protected static function renderTimezone($vcard_object) {
    if ($vcard_object->timezone) {
      return "TZ:" . $vcard_object->timezone . ArchibaldAppDataVcard::LINEBREAK;
    }
  }

  protected static function renderMailer($vcard_object) {
    if ($vcard_object->mailer) {
      return "MAILER:" . $vcard_object->mailer . ArchibaldAppDataVcard::LINEBREAK;
    }
  }

  protected static function renderLogo($vcard_object) {
    if ($vcard_object->logo) {
      $obj = $vcard_object->logo;
      $return = "LOGO;";
      if ($obj->uri) {
        $return .= "VALUE:uri:" . $obj->uri;
      }
      else {
        if (file_exists($obj->binary)) {
          $obj->binary = file_get_contents($obj->binary);
        }
        $return .= "ENCODING=b;TYPE=JPEG:" . base64_encode($obj->binary);
      }

      $return .= ArchibaldAppDataVcard::LINEBREAK;
      return $return;
    }
  }

  protected static function renderGeolocation($vcard_object) {
    if ($vcard_object->geolocation) {
      return "GEO:" . $vcard_object->geolocation . ArchibaldAppDataVcard::LINEBREAK;
    }
  }

  protected static function renderBirthday($vcard_object) {
    if ($vcard_object->birthday) {
      return "BDAY:" . $vcard_object->birthday . ArchibaldAppDataVcard::LINEBREAK;
    }
  }

  protected static function renderFullname($vcard_object) {
    return "FN:" . $vcard_object->fullname . ArchibaldAppDataVcard::LINEBREAK;
  }

  protected static function renderName($vcard_object) {
    return "N:" . $vcard_object->lastname . ";" . $vcard_object->firstname . ";" . $vcard_object->additionalnames . ";" . $vcard_object->nameprefix . ";" . $vcard_object->namesuffix . ArchibaldAppDataVcard::LINEBREAK;
  }

  protected static function renderVersion($version) {
    return "VERSION:" . $version . ArchibaldAppDataVcard::LINEBREAK;
  }

  protected static function renderAddress($vcard_object) {

    if ($vcard_object->address) {
      $address_string = "";
      foreach ($vcard_object->address as $address) {
        $address_type_string = '';
        if ($address['type']) {
          foreach ($address['type'] as $address_type) {
            $address_type_string .= ";" . $address_type;
          }
        }
        $address_string .= "ADR" . $address_type_string . ":" . $address['postofficeaddress'] . ";" . $address['extendedaddress'] . ";" . $address['street'] . ";" . $address['city'] . ";" . $address['state'] . ";" . $address['zip'] . ";" . $address['country'] . ArchibaldAppDataVcard::LINEBREAK;
      }
      return $address_string;
    }
  }

  protected static function renderBegin() {
    return "BEGIN:VCARD" . ArchibaldAppDataVcard::LINEBREAK;
  }

  protected static function renderTelephone($vcard_object) {
    if ($vcard_object->telephone) {
      $telephone_string = "";
      foreach ($vcard_object->telephone as $telephone) {
        $telephone_type_string = '';

        if ($telephone['type']) {
          foreach ($telephone['type'] as $telephone_type) {
            $telephone_type_string .= ";" . $telephone_type;
          }
        }
        $telephone_string .= "TEL" . $telephone_type_string . ":" . $telephone['value'] . ArchibaldAppDataVcard::LINEBREAK;
      }
      return $telephone_string;
    }
  }

  protected static function renderEmail($vcard_object) {
    if ($vcard_object->email) {
      $email_string = "";
      foreach ($vcard_object->email as $email) {
        $email_type_string = '';
        if ($email['type']) {
          foreach ($email['type'] as $email_type) {
            $email_type_string .= ";" . $email_type;
          }
        }
        $email_string .= "EMAIL" . $email_type_string . ":" . $email['value'] . ArchibaldAppDataVcard::LINEBREAK;
      }
      return $email_string;
    }
  }

  protected static function renderTitle($vcard_object) {
    if ($vcard_object->title) {
      return "TITLE:" . $vcard_object->title . ArchibaldAppDataVcard::LINEBREAK;
    }
  }

  protected static function renderRole($vcard_object) {
    if ($vcard_object->role) {
      return "ROLE:" . $vcard_object->role . ArchibaldAppDataVcard::LINEBREAK;
    }
  }

  protected static function renderOrganization($vcard_object) {
    if ($vcard_object->organization || $vcard_object->department || $vcard_object->subdepartment) {
      return "ORG:" . $vcard_object->organization . ";" . $vcard_object->department . ";" . $vcard_object->subdepartment . ArchibaldAppDataVcard::LINEBREAK;
    }
  }

  protected static function renderRevision($vcard_object) {
    if ($vcard_object->revision) {
      return "REV:" . $vcard_object->revision . ArchibaldAppDataVcard::LINEBREAK;
    }
  }

  protected static function renderUrl($vcard_object) {
    if ($vcard_object->url) {
      $url_tring = "";
      foreach ($vcard_object->url as $url) {
        $email_type_string = '';
        if ($url['type']) {
          foreach ($url['type'] as $email_type) {
            $email_type_string .= ";" . $email_type;
          }
        }
        $url_tring .= "URL" . $email_type_string . ":" . $url['value'] . ArchibaldAppDataVcard::LINEBREAK;
      }
      return $url_tring;
    }
  }

  protected static function renderUid($vcard_object) {
    if ($vcard_object->uid) {
      return "UID:" . $vcard_object->uid . ArchibaldAppDataVcard::LINEBREAK;
    }
  }

  protected static function renderGender($vcard_object) {
    if ($vcard_object->gender) {
      return "X-GENDER:" . $vcard_object->gender . ArchibaldAppDataVcard::LINEBREAK;
    }
  }

  protected static function renderNickname($vcard_object, $version) {
    if ($version == ArchibaldAppDataVcard::VERSION30) {
      if ($vcard_object->nickname) {
        return "NICKNAME:" . $vcard_object->nickname . ArchibaldAppDataVcard::LINEBREAK;
      }
    }
  }

  protected static function renderInstantmessenger($vcard_object) {
    if ($vcard_object->im) {
      $im_string = "";
      foreach ($vcard_object->im as $im) {
        $im_type_string = '';
        if ($im['type']) {
          foreach ($im['type'] as $im_type) {
            $im_type_string .= ";" . $im_type;
          }
        }
        $im_string .= $im['messenger'] . $im_type_string . ":" . $im['value'] . ArchibaldAppDataVcard::LINEBREAK;
      }
      return $im_string;
    }
  }

  protected static function renderEnd() {
    return "END:VCARD";
  }

  public static function generate($vcard_objects = NULL, $version = ArchibaldAppDataVcard::VERSION30) {
    if (!is_array($vcard_objects)) {
      $vcard_objects = array($vcard_objects);
    }

    $vcard_string = "";

    foreach ($vcard_objects as $vcard_object) {
      if (!empty($vcard_string)) {
        $vcard_string .= ArchibaldAppDataVcard::LINEBREAK;
      }
      $vcard_string .= self::renderBegin();
      $vcard_string .= self::renderVersion($version);
      $vcard_string .= self::renderFullname($vcard_object);
      $vcard_string .= self::renderName($vcard_object);
      $vcard_string .= self::renderBirthday($vcard_object);
      $vcard_string .= self::renderAddress($vcard_object);
      $vcard_string .= self::renderTelephone($vcard_object);
      $vcard_string .= self::renderEmail($vcard_object);
      $vcard_string .= self::renderInstantmessenger($vcard_object);
      $vcard_string .= self::renderTitle($vcard_object);
      $vcard_string .= self::renderRole($vcard_object);
      $vcard_string .= self::renderLogo($vcard_object);
      $vcard_string .= self::renderOrganization($vcard_object);
      $vcard_string .= self::renderRevision($vcard_object);
      $vcard_string .= self::renderUrl($vcard_object);
      $vcard_string .= self::renderUid($vcard_object);
      $vcard_string .= self::renderNickname($vcard_object, $version);
      $vcard_string .= self::renderGender($vcard_object);
      $vcard_string .= self::renderGeolocation($vcard_object);
      $vcard_string .= self::renderMailer($vcard_object);
      $vcard_string .= self::renderTimezone($vcard_object);
      $vcard_string .= self::renderEnd();
    }

    return $vcard_string;
  }
}

