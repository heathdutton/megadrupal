<?php

/**
 * @file
 * OpacConnector interface
 */

interface OpacConnector {
  /**
   * Tells if connector implements a feature or not.
   *
   * @param string $feature
   *   Name of feature.
   *   Can be one of: 'issues_history'
   *
   * @return bool
   *   TRUE if connector implements this feature.
   *   FALSE otherwise.
   */
  public function has($feature);

  /**
   * Sends back connector identity which must be known by OPAC module.
   *
   * Return: An array like the following:
   *  array(
   *    'id' => '<connector_id>',
   *    'name' => '<connector_name>',
   *  );
   */
  public function identity();

  /**
   * OPAC module call this method to know which patron fields are available.
   *
   * An array with fields information (label, description):
   *  array(
   *    'field_id' => array(
   *      'label' => 'Field label',
   *      'description' => 'Field description',
   *    ),
   *    '2nd_field_id' => array(
   *      'label' => '2nd field label',
   *      'description' => '2nd field description',
   *    ),
   *    // ...
   *  );
   */
  public function patronFields();

  /**
   * Same than patronFields but for item fields.
   */
  public function itemFields();

  /**
   * Same than patronFields but for biblio fields.
   */
  public function biblioFields();

  /**
   * Same than patronFields but for authority fields.
   */
  public function authFields();

  /**
   * Return available checkouts fields for display.
   *
   * @return array
   *   An associative array where each value is an associative array with the
   *   following keys:
   *   - label (mandatory): Translated label of the field.
   *   - type (optional): Type of field (at the moment, only 'date' type is
   *     supported.
   *   Example:
   *     array(
   *      ' date_due' => array(
   *         'label' => t('Date due'),
   *         'type' => 'date',
   *       ),
   *       'title' => array(
   *         'label' => t('Title'),
   *       ),
   *     )
   */
  public function patronCheckoutsFields();

  /**
   * Return available fields for issues history display.
   *
   * @return array
   *   An associative array where each value is an associative array with the
   *   following keys:
   *   - label (mandatory): Translated label of the field.
   *   - type (optional): Type of field (at the moment, only 'date' type is
   *     supported.
   *   Example:
   *     array(
   *       'date_due' => array(
   *         'label' => t('Date due'),
   *         'type' => 'date',
   *       ),
   *       'title' => array(
   *         'label' => t('Title'),
   *       ),
   *     )
   *
   *   Mandatory keys are 'recordid', 'title' and 'returndate'.
   */
  public function patronIssuesHistoryFields();

  /**
   * Return available holds fields for display.
   *
   * @return array
   *   Same return value as patronCheckoutsFields()
   */
  public function patronHoldsFields();

  /**
   * Change ILS password.
   *
   * @param string $username
   *   username for which password change must be done.
   *
   * @param string $new_password
   *   the new (unencrypted) password.
   *
   * @return int
   *   1 on success or 0 on failure
   */
  public function changePassword($username, $new_password);

  /**
   * Get informations about webservices used by connector.
   *
   * @return array
   *   array (
   *     array (
   *       // Webservice name, mandatory
   *       'name' => 'RESTWebService',
   *
   *       // Webservice url, mandatory
   *       'url' => 'http://example.com/rest/informations',
   *
   *       // HTTP Response code, mandatory
   *       'code' => '200',
   *
   *       // Additional informations about the webservice, as key/values,
   *       // optional.
   *       'infos' => array (
   *         'key1' => 'value1',
   *         'key2' => 'value2',
   *         // ...
   *       )
   *
   *       // Raw response from the webservice, optional
   *       'raw' => 'rawresponse'
   *     ),
   *     // ...
   *   )
   */
  public function getInfos();

  /**
   * Performs authentification with ILS.
   *
   * @return mixed
   *   ILS identifier of the borrower if authentification succeed or FALSE.
   */
  public function authenticateUser($login, $password);

  /**
   * Perform a patron lookup with ILS.
   *
   * Looks up a patron in the ILS by an identifier,
   * and returns the ILS identifier for that patron, aka the patron identifier.
   *
   * @param string $login
   *   User login.
   *
   * @param string $type
   *   Login type
   *
   * @return bool
   *   Patron id if authentification succeeded or FALSE.
   */
  public function lookupUser($login, $type);

  /**
   * Sends back marc data about a given biblio record.
   *
   * @param string $id
   *   Record identifier.
   *
   * @return array
   *   An array representing marc data.
   */
  public function marc($id);

  /**
   * Get biblio records from ILS.
   *
   * @param array $ids
   *   List of record identifiers to retrieve.
   *
   * @param array $wanted_fields
   *   List of field ids (as returned by biblioFields) to return.
   *
   * @return array
   *   An array containing records. One key is required: title.
   *   Others keys depends on the $wanted_fields parameter.
   *
   *   array(
   *     'record1_id' => array(
   *       'title' => 'record1 title',
   *       'wanted_field_1' => array(
   *         0 => 'foo',
   *         1 => 'bar',
   *       ),
   *       'wanted_field_2' => array(
   *         0 => 'bar',
   *       ),
   *     ),
   *     'record2_id' => array(
   *       'title' => 'record2 title',
   *       'wanted_field_2' => array(
   *         0 => 'bar',
   *       ),
   *     ),
   *   );
   */
  public function getBiblios($ids, $wanted_fields);

  /**
   * Retrieves information about items for a given biblio record.
   *
   * @param string $id
   *   Record identifier.
   *
   * @param array $wanted_fields
   *   List of field ids (as returned by itemFields) to return.
   *
   * @param string $user_id
   *   Patron id if you want to know if items are holdable by this patron.
   *
   * @return array
   *   An array containing items with some required keys and $wanted_fields
   *
   *   array(
   *     'avail' => <number of available items>,
   *     'total' => <number of items (total)>,
   *     'items' => array(
   *       'id_item1' => array(
   *         'canhold' => 1 // Is it holdable ?
   *         'avail' => 1 // Is it available ?
   *         'wanted_field1' => 'foo',
   *         'wanted_field2' => 'bar',
   *       ),
   *       'id_item2' => array(
   *         'canhold' => 0
   *         'avail' => 0
   *         'wanted_field1' => 'foo',
   *         'wanted_field2' => 'bar',
   *       ),
   *     ),
   *   );
   */
  public function itemsStatus($id, $wanted_fields, $user_id);

  /**
   * Retrieves patron information for a given patron.
   *
   * @param string $patron_id
   *   Patron identifier.
   *
   * @param array $wanted_fields
   *   List of field ids (as returned by patronFields) to return.
   *
   * @return array
   *   An array with $wanted_fields.
   *
   *   array(
   *     'wanted_field1' => 'foo',
   *     'wanted_field2' => 'bar',
   *   );
   */
  public function patronInfo($patron_id, $wanted_fields);

  /**
   * Get checkouts for a given patron.
   *
   * @param string $patron_id
   *   Patron identifier.
   *
   * @return array
   *   array(
   *     0 => array(
   *       'recordid' => <ils_record_id>,
   *       'barcode' => '<item_barcode>',
   *       'title' => '<document_title>',
   *       'duedate' => <TIMESTAMP>, // Date due
   *       'date' => <TIMESTAMP>, // Issue date
   *       'itemid' => <ils_item_id>,
   *       'renewable' => <is the issue renewable?>,
   *       // only present if 'renewable' is FALSE
   *       'reasons_not_renewable' =>
   *         '<string describing reasons why the issue is not renewable>',
   *     ),
   *     //...
   *   );
   */
  public function patronCheckouts($patron_id);

  /**
   * Get holds for a given patron.
   *
   * @param string $patron_id
   *   Patron identifier.
   *
   * @return array
   *   array(
   *     0 => array(
   *       'date' => <TIMESTAMP>, // Reserve date
   *       'title' => '<document_title>',
   *       'recordid' => <ils_record_id>,
   *       'barcode' => '<item_barcode>',
   *       'pickuploc' => '<pickup_location>',
   *       'canceldate' => '<cancel_date>',
   *       'cancelid' => '<cancel_id>', // Identifier used for cancellation
   *       'status' => '<status of reserve>',
   *     ),
   *     //...
   *   );
   */
  public function patronHolds($patron_id);

  /**
   * Performs an issue renewal for a given patron and item.
   *
   * @param string $patron_id
   *   Patron identifier.
   *
   * @param string $item_id
   *   Item identifier.
   *
   * @return array
   *   array(
   *    'success' => <bool>,
   *    'message' => 'what you want', // Message describing success or failure
   *  );
   */
  public function renewItem($patron_id, $item_id);

  /**
   * Cancel a specific hold.
   *
   * @param string $patron_id
   *   Patron identifier.
   *
   * @param string $hold_id
   *   Hold identifier ('cancelid' of patronHolds response)
   *
   * @return bool
   *   TRUE if success, FALSE otherwise.
   */
  public function cancelHold($patron_id, $hold_id);

  /**
   * Provide a web form adapted to what the ILS expect for holding a document.
   *
   * @param string $patron_id
   *   Patron identifier
   *
   * @param array $record
   *   Contains keys 'recordid', 'record_title' and 'nid'.
   *
   * @param string $item_id
   *   Item identifier, if item-level hold is requested.
   */
  public function placeHoldForm($patron_id, $record, $item_id);

  /**
   * Place an hold for an item or record.
   *
   * @param array $values
   *   Comes from the submited form of placeHoldForm.
   *   So values contains form elements values + :
   *     itemid: if holding an item,
   *     recordid: if holding a record,
   *     opacuserid: ILS identifier of the borrower.
   */
  public function placeHold($values);

  /**
   * Check if a biblio is holdable by a patron.
   *
   * @param int $record_id
   *   ILS identifier of record
   *
   * @param int $patron_id
   *   ILS identifier of patron
   *
   * @return int
   *   TRUE if biblio is holdable, FALSE otherwise.
   */
  public function biblioIsHoldable($record_id, $patron_id);

  /**
   * Provide a web form for processing a registration to ILS.
   *
   * @param array $user
   *   Drupal user.
   *
   * @return array
   *   A drupal form adapted to what the ILS expects.
   */
  public function patronRegistrationForm($user);

  /**
   * Get issues history of a patron.
   *
   * @param string $patron_id
   *   Patron ILS identifier
   *
   * @return array
   *   Array of issues
   */
  public function patronIssuesHistory($patron_id);

  /**
   * Create a user in ILS by webservice.
   *
   * @param array $data
   *   Everything needed for a user creation as a key/value array.
   */
  public function createUser($data);

  /**
   * Modify an existing user in ILS.
   *
   * @param string $user_name
   *   User name
   *
   * @param array $data
   *   An associative array where keys are names of fields to modify and values
   *   are new values for those fields.
   *
   * @return array
   *   An associative array with the following keys:
   *   - success: A boolean that indicates if modification succeeded or not.
   *   - modified_fields: An associative array where keys are names of modified
   *     fields and values are new values for those fields.
   */
  public function modifyUser($user_name, $data);

  /**
   * Does the given login exists in the ILS?.
   *
   * @param array $login
   *   The login of the user.
   */
  public function loginExists($login);

  /**
   * Create a purchase suggestion in ILS by webservice.
   *
   * @param array $data
   *   Everything needed for a suggestion creation as a key/value array.
   */
  public function createSuggestion($values);

  /**
   * Return purchase suggestions.
   */
  public function getSuggestions($patron_id);

  /**
   * Delete an existing suggestion.
   */
  public function deleteSuggestion($suggestionid);
}

