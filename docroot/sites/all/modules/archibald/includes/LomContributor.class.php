<?php

/**
 * @file
 * class to manage local address / vCard Database
 */

require_once dirname(__FILE__) . '/vcard.php';

/**
 * Manage local address / vCard Database
 *
 * dont be confused about ArchibaldLom_Contribute and ArchibaldLomContributor
 * This are to completly differen classes
 * ArchibaldLomContributor is part of the ArchibaldLom Class
 * @see Lom.class.php
 *
 * @author Heiko Henning <h.henning@educa.ch>
 */
class ArchibaldLomContributor {

  /**
   * identifier for vcard
   * @var string
   */
  private $id = '';

  /**
   * vcard object
   * @var ArchibaldAppDataVcard
   */
  private $vcardObject = NULL;

  /**
   * contributor constructor
   *
   * @param string $data optional
   */
  public function __construct($data = '') {
    if (!empty($data)) {
      $this->load($data);
    }
  }

  /**
   * load datastring to object
   *
   * @param string $data
   *
   * @return boolean
   */
  public function load($data) {
    if (!empty($data) && drupal_strlen($data) > 6 && drupal_strlen($data) <= 32) {
      $this->loadById($data);
      return TRUE;
    }

    if (!empty($data)) {
      if (drupal_substr(drupal_strtoupper($data), 0, 11) != 'BEGIN:VCARD') {
        return FALSE;
      }

      $parser = new ArchibaldAppDataVcard_Parser($data);
      $this->vcardObject = $parser->parse();
      if (!empty($this->vcardObject) && is_array($this->vcardObject) && $this->vcardObject[0] instanceof ArchibaldAppDataVcard) {
        $this->vcardObject = $this->vcardObject[0];
      }


      if (empty($this->id)) {
        $uid = $this->vcardObject->uid;
        if (empty($uid)) {
          $this->updateId();
        }
        else {
          $this->id = $uid;
        }
      }

      return TRUE;
    }

    return FALSE;
  }

  /**
   * load a vcard object form database
   *
   * @param string $contributor_id
   */
  public function loadById($contributor_id) {
    $sh = db_select('archibald_contributors ', 'contrers')
      ->fields(
        'contrers',
        array(
          'lastname',
          'firstname',
          'organisation',
          'country',
          'zip',
          'city',
          'address1',
          'address2',
          'email',
          'url',
          'uid',
          'tel',
          'logo',
        )
      )
      ->condition('contributor_id', $contributor_id, '=');
    $result = $sh->execute();
    $this->id = $contributor_id;
    $this->renderDataObject($result->fetchAssoc());
  }

  /**
   * return stringified vcard object
   *
   * @return string
   */
  public function getVcard() {
    if (!($this->vcardObject instanceof ArchibaldAppDataVcard)) {
      return '';
    }

    return ArchibaldAppDataVcardGenerator::generate($this->vcardObject);
  }

  /**
   * return vcard object
   *
   * @return string
   */
  public function getVcardObject() {
    return $this->vcardObject;
  }

  /**
   * generate id from vcard content to identify
   */
  private function updateId() {
    $this->id = md5(microtime());
    $this->vcardObject->setUid($this->id);
  }

  /**
   * save current object to database
   *
   * @return string
   *   id of contributor
   */
  public function save() {
    if (empty($this->id)) {
      $this->updateId();
    }

    if ($this->vcardObject->organization == '' && $this->vcardObject->firstname == '' && $this->vcardObject->lastname == '') {
      return FALSE;
    }

    db_merge('archibald_contributors')->key(array('contributor_id' => $this->id))->fields($this->getData(TRUE))->execute();

    return $this->id;
  }

  /**
   * get data object for saving
   *
   * @param boolean $inlcude_empty_elements
   *   default: TRUE
   *   include also empty elements
   *
   * @return array
   */
  public function getData($inlcude_empty_elements = FALSE) {
    $data = array();

    if ($inlcude_empty_elements == TRUE) {
      $data['lastname'] = '';
      $data['firstname'] = '';
      $data['organisation'] = '';
      $data['country'] = '';
      $data['zip'] = '';
      $data['city'] = '';
      $data['address1'] = '';
      $data['address2'] = '';
      $data['email'] = '';
      $data['url'] = '';
      $data['uid'] = '';
      $data['tel'] = '';
      $data['logo'] = '';
    }

    if ($this->vcardObject->lastname != '') {
      $data['lastname'] = $this->vcardObject->lastname;
    }

    if ($this->vcardObject->firstname != '') {
      $data['firstname'] = $this->vcardObject->firstname;
    }

    if ($this->vcardObject->organization != '') {
      $data['organisation'] = $this->vcardObject->organization;
    }

    $address = $this->vcardObject->address;
    if (!empty($address)) {
      $data['country']  = @$address[0]['country'];
      $data['zip']      = @$address[0]['zip'];
      $data['city']     = @$address[0]['city'];
      $data['address1'] = @$address[0]['street'];
      $data['address2'] = @$address[0]['extendedaddress'];
    }

    $email = $this->vcardObject->email;
    if (!empty($email)) {
      $data['email'] = @$email[0]['value'];
    }

    $url = $this->vcardObject->url;
    if (!empty($url)) {
      $data['url'] = @$url[0]['value'];
    }

    $telephone = $this->vcardObject->telephone;
    if (!empty($telephone)) {
      $data['tel'] = @$telephone[0]['value'];
    }

    if (isset($this->vcardObject->logo->uri) && drupal_strlen($this->vcardObject->logo->uri) <= 255) {
      $data['logo'] = $this->vcardObject->logo->uri;
    }

    $uid = $this->vcardObject->uid;
    if (!empty($uid)) {
      if (!is_array($uid)) {
        $data['uid'] = $uid;
      }
      else {
        $data['uid'] = $uid[0]['value'];
      }
    }

    return $data;
  }

  /**
   * render database object to vcard
   *
   * @param array $data
   */
  public function renderDataObject($data) {
    if (empty($data) && !is_array($data)) {
      return FALSE;
    }

    $this->vcardObject = new ArchibaldAppDataVcard();
    if (isset($data['uid']) && !empty($data['uid'])) {
      $this->vcardObject->setUid($data['uid']);
      $this->id = $data['uid'];
    }
    elseif (!empty($this->id)) {
      $this->vcardObject->setUid($this->id);
    }

    if (isset($data['firstname'])) {
      $this->vcardObject->setFirstname($data['firstname']);
    }

    if (isset($data['lastname'])) {
      $this->vcardObject->setLastname($data['lastname']);
    }

    if (isset($data['firstname']) || isset($data['lastname'])) {
      $this->vcardObject->setFullname( $data['firstname'] . ' ' . $data['lastname']);
    }

    if (isset($data['organisation'])) {
      $this->vcardObject->setOrganization($data['organisation']);
    }

    if (isset($data['address1']) || isset($data['city'])) {
      if (empty($data['address1'])) {
        $data['address1'] = '';
      }
      if (empty($data['address2'])) {
        $data['address2'] = '';
      }
      if (empty($data['city'])) {
        $data['city'] = '';
      }
      if (empty($data['zip'])) {
        $data['zip'] = '';
      }
      if (empty($data['country'])) {
        $data['country'] = '';
      }

      $this->vcardObject->addAddress(
        array('street' => $data['address1'],
          'extendedaddress' => $data['address2'],
          'city' => $data['city'],
          'zip' => $data['zip'],
          'country' => $data['country'],
          'type' => array(ArchibaldAppDataVcard::ADDRESSWORK),
        )
      );
    }
    else {
      unset($this->vcardObject->address);
    }


    if (isset($data['tel'])) {
      if (empty($data['tel'])) {
        unset($this->vcardObject->telephone);
      }
      else {
        $this->vcardObject->addPhonenumber($data['tel'], array(ArchibaldAppDataVcard::TELEPHONEWORK, ArchibaldAppDataVcard::TELEPHONEVOICE));
      }
    }

    if (isset($data['email'])) {
      if (empty($data['email'])) {
        unset($this->vcardObject->email);
      }
      else {
        $this->vcardObject->addEmail($data['email'], array(ArchibaldAppDataVcard::EMAILPREF, ArchibaldAppDataVcard::EMAILINTERNET));
      }
    }

    if (isset($data['url'])) {
      if (empty($data['url'])) {
        unset($this->vcardObject->url);
      }
      else {
        $this->vcardObject->addUrl($data['url'], array(ArchibaldAppDataVcard::URLPREF));
      }
    }

    if (!empty($data['logo'])) {
      if (is_object($data['logo'])) {
        $this->vcardObject->setLogo($data['logo']);
      }
      else {
        $this->vcardObject->setLogo((object)array('uri' => $data['logo']));
      }
    }
    else {
      unset($this->vcardObject->logo);
    }

    if (empty($this->id)) {
      $this->updateId();
    }
  }

  /**
   * get name of vcardet person for display within the form
   *
   * @return string
   */
  public function getFormName() {
    $form_name = '';
    if ($this->vcardObject->firstname != '') {
      $form_name .= $this->vcardObject->firstname . ' ';
    }

    if ($this->vcardObject->lastname != '') {
      $form_name .= $this->vcardObject->lastname . ' ';
    }

    if ($this->vcardObject->organization != '') {
      $form_name .= $this->vcardObject->organization;
    }
    return trim($form_name);
  }

  /**
   * return current id
   *
   * @return string
   */
  public function getId() {
    return $this->id;
  }
}

