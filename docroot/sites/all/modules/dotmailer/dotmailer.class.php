<?php

/**
 * @file
 *
 * Dotmailer API
 *
 * This file provides the connectivity functions for calling
 * the dotmailer API over a standard SOAP call
 *
 * Currently this include provides the following API calls
 *
 * - getContactByEmail
 * - listAddressBooksForContact
 * - listAddressBooks
 * - listContactDataLabels
 * - listContactsInAddressBook
 * - addContactToAddressBook
 * - updateContact
 * - removeContactFromAddressBook
 *
 * TODO - This class should extend SoapClient rather than just use it.
 * TODO - This class should cache the credentials at construction - not pass them
 *        in each function.
 */
class dotMAPI {
  var $version = "1.0";
  var $errorMessage;
  var $errorCode;

  /**
   * Default to a 300 second timeout on server calls
   */
  var $timeout = 300;

  /**
   * Default to a 8K chunk size
   */
  var $chunkSize = 8192;

  /**
   * Connect to the DotMailer API for a given list. All DMAPI calls require login before functioning
   *
   * @param string $username Your Dotmailer login user name - always required
   * @param string $password Your Dotmailer login password - always required
   */
  function dotMAPI() {
    $client = new SoapClient('http://apiconnector.com/api.asmx?WSDL');
    return $client;
  }

  /**
   * Obtain a full contact by email address. This will return a full
   * dotmailer contact, with the flag to include an array of address
   * books values is requested
   *
   * @param object $login
   * @param object $email
   * @param object $inc_addressbooks [optional]
   *
   * @return array
   */
  function getContactByEmail($login, $email, $inc_addressbooks = TRUE) {
    $client = $this->dotMAPI();

    $params = $login;
    $params['email'] = $email;

    try {
      $result = $client->GetContactByEmail($params);
    }
    catch (Exception $e) {
      return $e;
    }

    $contact_details = array();

    $contact = $result->GetContactByEmailResult;
    $contact_details['email'] = $contact->Email;
    $contact_details['emailId'] = $contact->ID;
    $dataFields = $contact->DataFields;
    $keys = $dataFields->Keys;
    $string = $keys->string;

    $values = $dataFields->Values;
    $anyType = $values->anyType;

    foreach ($anyType as $id => $value) {
      $contact_details[$string[$id]] = $value;
    }

    // Attach the addressbook request on top
    if ($inc_addressbooks) {

      $contact_details['addressBooks'] = array();

      $params['contact']['ID'] = $contact->ID;
      $params['contact']['Email'] = $email;
      $params['contact']['AudienceType'] = $contact->AudienceType;
      $params['contact']['OptInType'] = $contact->OptInType;
      $params['contact']['EmailType'] = $contact->EmailType;
      $params['contact']['Notes'] = $contact->Notes;
      try {
        $result = $client->ListAddressBooksForContact($params);
        self::makeArray($result->ListAddressBooksForContactResult->APIAddressBook);
      }
      catch (Exception $e) {
        return $e;
      }

      $addressbooks = $result->ListAddressBooksForContactResult->APIAddressBook;

      if ($addressbooks) {
        foreach ($addressbooks as $book) {
          $books[$book->ID] = $book->Name;
        }
        $contact_details['addressBooks'] = $books;
      }
    }
    return $contact_details;
  }

  /**
   * Obtain a list of the address books for a specfic contact
   *
   * This is used for the getContact function to add address books
   * to a contact
   *
   * @param object $login
   * @param object $email
   * @param object $id
   *
   * @return
   */
  function listAddressBooksForContact($login, $email, $id) {
    $client = $this->dotMAPI();

    $params = $login;
    $params['contact']['ID'] = $id;
    $params['contact']['Email'] = $email;
    $params['contact']['AudienceType'] = NULL;
    $params['contact']['OptinType'] = NULL;
    $params['contact']['EmailType'] = NULL;
    $params['contact']['Notes'] = NULL;

    try {
      $result = $client->ListAddressBooksForContact($params);
    }
    catch (Exception $e) {
      return $e;
    }

    $addressbooks = $result->ListAddressBooksForContactResult;
    foreach ($addressbooks as $book) {
      $books[$book->ID] = $book->Email;
    }
    return $books;
  }

  /**
   * List the address books for an account
   *
   * @param object $login
   *
   * @return
   */
  function listAddressBooks($login) {
    $client = $this->dotMAPI();

    try {
      $result = $client->ListAddressBooks($login);
      self::makeArray($result->ListAddressBooksResult->APIAddressBook);
      return $result->ListAddressBooksResult->APIAddressBook;
    }
    catch (Exception $e) {
      return $e;
    }
  }

  /**
   * List the datafields for an account (same fields currently
   * apply across all address books)
   *
   * @param object $login
   *
   * @return
   */
  function listContactDataLabels($login) {
    $client = $this->dotMAPI();

    $result = $client->ListContactDataLabels($login);
    self::makeArray($result->ListContactDataLabelsResult->ContactDataLabel);
    return $result->ListContactDataLabelsResult;
  }

  /**
   * Obtain a list of all contacts for the account
   *
   * @param object $login
   * @param object $addressBookId
   * @param object $select
   * @param object $skip
   *
   * @return
   */
  function listContactsInAddressBook($login, $addressBookId, $select, $skip) {
    $client = $this->dotMAPI();

    $params = $login;
    $params['addressBookId'] = $addressBookId;
    $params['select'] = $select;
    $params['skip'] = $skip;

    $result = $client->ListContactsInAddressBook($params);

    $output = '';
    $contact = $result->ListContactsInAddressBookResult;

    $con = $contact->APIContact;
    $email = $con->Email;
    $dataFields = $con->DataFields;
    $keys = $dataFields->Keys;
    $string = $keys->string;

    $values = $dataFields->Values;
    $anyType = $values->anyType;

    $output .= $email . ' <br />';
    foreach ($anyType as $id => $value) {
      $output .= $string[$id] . ' : ' . $value . ' <br />';
    }
    $output = 'YES';

    return $output;
  }

  /**
   * Add a new contact to an address book
   *
   * @param object $login
   * @param object $addressbookId
   * @param object $email
   * @param object $user_datafields [optional]
   * @param object $optintype [optional]
   * @param object $emailtype [optional]
   * @param object $audiencetype [optional]
   *
   * @return
   */
  function addContactToAddressBook($login, $addressbookId, $email, $user_datafields = array(), $optintype = 1, $emailtype = 'Html', $audiencetype = 'Unknown') {
    $client = $this->dotMAPI();
    $data_fields = array();
    if (!empty($user_datafields)) {
      foreach ($user_datafields AS $id => $var) {
        switch ($var['type']) {
          case 'string':
            $type = XSD_STRING;
            break;

          case 'date':
            $type = XSD_DATE;
            break;
        }
        $keys[] = $id;
        $values[] = new SoapVar($var['data'], $type, $var['type'], 'http://www.w3.org/2001/XMLSchema');
      }
      $data_fields = array('Keys' => $keys, 'Values' => $values);
    }
    $params = $login;
    $params['addressbookId'] = $addressbookId;
    $params['contact'] = array(
      'Email' => $email,
      'OptInType' => $optintype == 2 ? 'Double' : 'Single',
      'EmailType' => $emailtype,
      'AudienceType' => $audiencetype,
      'ID' => -1,
      'DataFields' => $data_fields,
    );

    try {
      $result = $client->AddContactToAddressBook($params);
    }
    catch (Exception $e) {
      return $e;
    }
    return $result;
  }

  /**
   * Update a contact in an address book
   *
   * @param object $login
   * @param object $addressbookId
   * @param object $contactId
   * @param object $email
   * @param object $user_datafields [optional]
   * @param object $optintype [optional]
   * @param object $emailtype [optional]
   * @param object $audiencetype [optional]
   *
   * @return
   */
  function updateContact($login, $addressbookId, $contactId, $email, $user_datafields, $optintype = 1, $emailtype = 'Html', $audiencetype = 'Unknown') {
    $client = $this->dotMAPI();
    foreach ($user_datafields AS $key => $var) {
      switch ($var['type']) {
        case 'string':
          $type = XSD_STRING;
          break;

        case 'date':
          $type = XSD_DATE;
          break;
      }
      $keys[] = $key;
      $values[] = new SoapVar($var['data'], $type, $var['type'], 'http://www.w3.org/2001/XMLSchema');
    }

    $data_fields = array('Keys' => $keys, 'Values' => $values);

    $params = $login;
    $params['addressbookId'] = $addressbookId;
    $params['contact'] = array(
      'Email' => $email,
      'OptInType' => $optintype == 2 ? 'Double' : 'Single',
      'EmailType' => $emailtype,
      'AudienceType' => $audiencetype,
      'ID' => $contactId,
      'DataFields' => $data_fields,
    );

    try {
      $result = $client->AddContactToAddressBook($params);
    }
    catch (Exception $e) {
      return $e;
    }
    return $result;
  }

  /**
   * Removes a contact from an address book
   *
   * @param object $login
   * @param object $bookId
   * @param object $email
   * @param object $resubscribe [optional]
   * @param object $totalunsub [optional]
   *
   * @return
   */
  function removeContactFromAddressBook($login, $bookId, $email, $resubscribe = FALSE, $totalunsub = FALSE) {
    $client = $this->dotMAPI();

    $params = $login;
    $params['email'] = $email;

    try {
      $result = $client->GetContactByEmail($params);
    }
    catch (Exception $e) {
      return $e;
    }
    $contact = $result->GetContactByEmailResult;
    $params['contact']['ID'] = $contact->ID;
    $params['contact']['Email'] = $email;
    $params['contact']['AudienceType'] = $contact->AudienceType;
    $params['contact']['OptInType'] = $contact->OptInType;
    $params['contact']['EmailType'] = $contact->EmailType;
    $params['contact']['Notes'] = $contact->Notes;
    $params['addressBookId'] = $totalunsub === TRUE ? -1 : $bookId;
    $params['preventAddressbookResubscribe'] = $resubscribe;
    $params['totalUnsubscribe'] = $totalunsub;

    try {
      $response = $client->RemoveContactFromAddressBook($params);
    }
    catch (Exception $e) {
      return $e;
    }
    return $response;
  }

  /**
   * Allows you to ensure that 'arrays' returned by SOAP are returned as arrays
   * in PHP. This is because PHP/SOAP doesn't correcty communicate the idea of a
   * collection so PHP assumes if you have one of something there will only be one
   *
   * This means if you request an 'array' from the server that happens to only have
   * one item it will be converted to an object in PHP world. This function wraps
   * those one object elements into arrays with one element.
   *
   * If that didn't make sense, then throw a kpr() inside the 'if' to see what I
   * mean
   *
   * @param type $array
   */
  public static function makeArray(&$array) {
    if ($array && !is_array($array)) {
      $array = array($array);
    }
  }
}

