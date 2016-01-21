<?php

/**
 * @file
 * Provides class for integration with the PostcodeAnywhere API.
 */

/**
 * Specifically the 'PostcodeAnywhere Interactive Find (v1.10)' service.
 *
 * This lists address records matching the specified search term.This general
 * search method can search by postcode, company or street.
 *
 * Based on existing open source projects and official API documentation.
 *
 * @see http://www.pcapredict.com/support/webservice/postcodeanywhere/interactive/find/1.1/, https://github.com/bensquire/postcodeanywhere-php-api
 */
class AddressFieldLookupPostcodeAnywhere implements AddressFieldLookupInterface {
  // Multi-dimensional array of endpoints. Firstly keyed by the name of the
  // endpoint and then keyed by the return data format.
  protected $endpoints = array(
    'Find' => array(
      'xml' => 'services.postcodeanywhere.co.uk/PostcodeAnywhere/Interactive/Find/v1.10/xmla.ws',
      'json' => 'services.postcodeanywhere.co.uk/PostcodeAnywhere/Interactive/Find/v1.10/json.ws',
    ),
    'RetrieveById' => array(
      'xml' => 'services.postcodeanywhere.co.uk/PostcodeAnywhere/Interactive/RetrieveById/v1.30/xmla.ws',
      'json' => 'services.postcodeanywhere.co.uk/PostcodeAnywhere/Interactive/RetrieveById/v1.30/json.ws',
    ),
  );

  // Format to use for the return data.
  protected $format = 'json';

  // Should API calls be made using https.
  protected $https = TRUE;

  // The key to use to authenticate to the service.
  protected $key;

  // The username associated with the Royal Mail license
  // (not required for click licenses).
  protected $userName;

  // The language version of the address to return.
  protected $preferredLanguage;

  // The filter to apply to the output.
  protected $filter;

  // List of languages support by the Postcode Anywhere API.
  protected $allowedLanguages = array('English', 'Welsh');

  // List of filters support by the Postcode Anywhere API.
  protected $allowedFilters = array('None', 'OnlyResidential', 'OnlyCommercial');

  // The search term to find. The search term can be a postcode, company name or
  // street and town (separated by commas).
  protected $searchTerm;

  // Array of results from the API.
  protected $result = array();

  /**
   * Constructor.
   *
   * @param string $key
   *   The key to use to authenticate to the service.
   * @param string $user_name
   *   The username associated with the Royal Mail license
   *   (not required for click licenses).
   * @param string $preferred_language
   *   The language version of the address to return.
   * @param string $filter
   *   The filter to apply to the output.
   */
  public function __construct($key, $user_name, $preferred_language, $filter = 'None') {
    // Set API credentials.
    $this->key = $key;
    $this->userName = $user_name;

    // Check the language is valid.
    if (in_array($preferred_language, $this->allowedLanguages)) {
      $this->preferredLanguage = $preferred_language;
    }
    else {
      // Otherwise default to English.
      $this->preferredLanguage = 'English';
    }

    // Check the filter is valid.
    if (in_array($filter, $this->allowedFilters)) {
      $this->filter = $filter;
    }
    else {
      // Not a valid filter.
      throw new Exception('Requested filter not supported by Postcode Anywhere API.');
    }
  }

  /**
   * Set the data return format.
   *
   * @param string $format
   *   String containing the required format.
   *
   * @return AddressFieldLookupInterface
   *   The called object.
   */
  public function setFormat($format) {
    // Build an array of valid formats.
    $valid_formats = array();

    // Loop through the endpoints.
    foreach ($this->endpoints as $formats) {
      foreach ($formats as $format => $url) {
        // Add to the valid format list.
        if (!in_array($format, $valid_formats)) {
          $valid_formats[] = $format;
        }
      }
    }

    // Check the requested format is valid.
    if (!in_array($format, $valid_formats)) {
      $this->format = $format;
    }
    else {
      // Not a valid format.
      throw new Exception('Requested data format not supported by Postcode Anywhere API.');
    }

    return $this;
  }

  /**
   * Set the flag to indicate if API calls be made using https.
   *
   * @param bool $https
   *   Should API calls be made using https.
   *
   * @return AddressFieldLookupInterface
   *   The called object.
   */
  public function setHttps($https) {
    // Check we have an endpoint for the requested format.
    if (is_bool($https)) {
      $this->https = $https;
    }
    else {
      throw new Exception('HTTPS flag must be a boolean.');
    }

    return $this;
  }

  /**
   * Set the term that we'll be using to lookup addresses.
   *
   * @param string $lookup_term
   *   String containing the lookup term.
   *
   * @return AddressFieldLookupInterface
   *   The called object.
   */
  public function setLookupTerm($lookup_term) {
    $this->searchTerm = $lookup_term;

    return $this;
  }

  /**
   * Perform the lookup based on the current term.
   *
   * @return bool
   *   Was the lookup successful or not?
   */
  public function lookup() {
    // Build array of parameters to pass to the API.
    $lookup_params = array(
      'SearchTerm' => $this->searchTerm,
      'Filter' => $this->filter,
    );

    // Get the raw API result.
    $api_response = $this->callApi('Find', $lookup_params);

    // Parse the API repsonse.
    if (isset($api_response)) {
      $parsed_api_response = $this->parseApiResponse($api_response);

      // Build the format we need.
      if (!empty($parsed_api_response)) {
        foreach ($parsed_api_response as $parsed_api_response_item) {
          $this->result[] = array(
            'id' => $parsed_api_response_item->Id,
            'street' => $parsed_api_response_item->StreetAddress,
            'place' => $parsed_api_response_item->Place,
          );
        }

        return TRUE;
      }
      else {
        return FALSE;
      }
    }
    else {
      // No result.
      return FALSE;
    }
  }

  /**
   * Get the results of the lookup.
   *
   * @return array
   *   Array of lookup results in the format:
   *     id - Address ID
   *     street - Street (Address Line 1)
   *     place - Remainder of address.
   */
  public function getLookupResult() {
    // Check we have some data to return.
    if (!empty($this->result)) {
      return $this->result;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Get the full details for a given address.
   *
   * @param mixed $address_id
   *   ID of the address to get details for.
   *
   * @return array $address_details
   *   Array of details for the given address in the format:
   *     id - Address ID
   *     sub_premise - The sub_premise of this address
   *     premise - The premise of this address. (i.e. Apartment / Suite number).
   *     thoroughfare - The thoroughfare of this address. (i.e. Street address).
   *     dependent_locality - The dependent locality of this address.
   *     locality - The locality of this address. (i.e. City).
   *     postal_code - The postal code of this address.
   *     administrative_area - The administrative area of this address.
   *     (i.e. State/Province)
   *     organisation_name - Contents of a primary OrganisationName element
   *     in the xNL XML.
   */
  public function getAddressDetails($address_id) {
    // Build array of parameters to pass to the API.
    $address_details_params = array(
      'Id' => $address_id,
    );

    // Get the raw API result.
    $api_response = $this->callApi('RetrieveById', $address_details_params);

    // Parse the API repsonse.
    if (isset($api_response)) {
      $parsed_api_response = $this->parseApiResponse($api_response);

      // Build the format we need.
      if (!empty($parsed_api_response[0])) {
        // Address details array.
        $address_details = array(
          'id' => $address_id,
        );

        // Sub premise.
        if (!empty($parsed_api_response[0]->SubBuilding)) {
          $address_details['sub_premise'] = $parsed_api_response[0]->SubBuilding;
        }

        // Premise.
        $address_details['premise'] = '';

        // Premise: Check for building number.
        if (!empty($parsed_api_response[0]->BuildingNumber)) {
          $address_details['premise'] .= $parsed_api_response[0]->BuildingNumber . ' ';
        }

        // Premise: Check for building name.
        if (!empty($parsed_api_response[0]->BuildingName)) {
          $address_details['premise'] .= $parsed_api_response[0]->BuildingName . ' ';
        }

        // Premise: Remove white space.
        $address_details['premise'] = trim($address_details['premise']);

        // Thoroughfare.
        $address_details['thoroughfare'] = '';

        // Thoroughfare: Street 1.
        if (!empty($parsed_api_response[0]->PrimaryStreet)) {
          $address_details['thoroughfare'] .= $parsed_api_response[0]->PrimaryStreet . ' ';
        }

        // Thoroughfare: Street 2.
        if (!empty($parsed_api_response[0]->SecondaryStreet)) {
          $address_details['thoroughfare'] .= $parsed_api_response[0]->SecondaryStreet . ' ';
        }

        // Premise: Remove white space.
        $address_details['thoroughfare'] = trim($address_details['thoroughfare']);

        // Dependent locality.
        $address_details['dependent_locality'] = '';

        // Dependent locality: Initial dependent locality.
        if (!empty($parsed_api_response[0]->DependentLocality)) {
          $address_details['dependent_locality'] .= $parsed_api_response[0]->DependentLocality . ' ';
        }

        // Dependent locality: Double dependent locality.
        if (!empty($parsed_api_response[0]->DoubleDependentLocality)) {
          $address_details['dependent_locality'] .= $parsed_api_response[0]->DoubleDependentLocality . ' ';
        }

        // Dependent locality: Remove white space.
        $address_details['dependent_locality'] = trim($address_details['dependent_locality']);

        // Locality.
        $address_details['locality'] = isset($parsed_api_response[0]->PostTown) ? $parsed_api_response[0]->PostTown : '';

        // Postal code.
        $address_details['postal_code'] = isset($parsed_api_response[0]->Postcode) ? $parsed_api_response[0]->Postcode : '';

        // Administrative area.
        $address_details['administrative_area'] = isset($parsed_api_response[0]->County) ? $parsed_api_response[0]->County : '';

        // Organisation name.
        $address_details['organisation_name'] = isset($parsed_api_response[0]->Company) ? $parsed_api_response[0]->Company : '';

        return $address_details;
      }
      else {
        return FALSE;
      }
    }
    else {
      // No result.
      return FALSE;
    }
  }

  /**
   * Call the API endpoint and return the result.
   *
   * @param string $endpoint_name
   *   Name of the API endpoint to call.
   * @param array $params
   *   Array of parameters to pass to the API.
   *
   * @return string $api_response
   *   Raw API response.
   */
  protected function callApi($endpoint_name, $params = array()) {
    // Check the endpoint name is valid.
    if (!in_array($endpoint_name, array_keys($this->endpoints))) {
      throw new Exception('Requested endpoint is not supported by the Postcode Anywhere API');
    }

    // Array parameters that we need on every request.
    $default_params = array(
      'Key' => $this->key,
      'PreferredLanguage' => $this->preferredLanguage,
      'UserName' => $this->userName,
    );

    $params += $default_params;

    // Build the API url.
    $api_url = $this->buildApiUrl($endpoint_name, $params);

    // Call the API.
    $ch = curl_init($api_url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($ch, CURLOPT_TIMEOUT, 10);

    // Get the API response.
    $api_response = curl_exec($ch);

    // Get the response code.
    $api_response_code = curl_getinfo($ch, CURLINFO_HTTP_CODE);
    curl_close($ch);

    // Throw an exception if the repsonse code was not 200.
    if ($api_response_code != 200) {
      throw new Exception('Could not reach the Postcode Anywhere API.');
    }

    return $api_response;
  }

  /**
   * Build the URL required for the API call.
   *
   * @param string $endpoint_name
   *   Name of the API endpoint to call.
   * @param array $params
   *   Array of parameters to pass to the API.
   *
   * @return string $api_url
   *   URL for the API call.
   */
  protected function buildApiUrl($endpoint_name, $params = array()) {
    // Build the URL string.
    $api_url = $this->https ? 'https' : 'http';

    // Determine the endpoint to use based on the format.
    if (in_array($this->format, array_keys($this->endpoints[$endpoint_name]))) {
      $api_url .= '://' . $this->endpoints[$endpoint_name][$this->format];
    }
    else {
      throw new Exception('Requested data format not supported by Postcode Anywhere API.');
    }

    // Build the query string.
    if (!empty($params)) {
      $api_url .= '?' . http_build_query($params, '', '&');
    }

    return $api_url;
  }

  /**
   * Parse the response from the API based on the required format.
   *
   * @param string $api_response
   *   The raw resonse from the API.
   *
   * @return array $parsed_api_response
   *   Array containing the API response items in the requested format.
   */
  protected function parseApiResponse($api_response) {
    // Check we have some data to parse.
    if (empty($api_response)) {
      return FALSE;
    }

    // Array for the parsed response.
    $parsed_api_response = array();

    // Parse based on the requested format.
    switch ($this->format) {
      case 'json':
        // Decode the json.
        $parsed_api_response = json_decode($api_response);

        // Check for any errors.
        if (isset($parsed_api_response[0]->Error)) {
          throw new Exception('Error ' . $parsed_api_response[0]->Error . ' (' . $parsed_api_response[0]->Description . '). ' . $parsed_api_response[0]->Cause . '. Resolution: ' . $parsed_api_response[0]->Resolution);
        }

        break;

      case 'xml':
        // Decode the XML.
        $api_xml_response = simplexml_load_string($api_response);

        // Check for any errors.
        if ($api_xml_response->Columns->Column->attributes()->Name == "Error") {
          throw new Exception('Error ' . $api_xml_response->Rows->Row->attributes()->Error . ' (' . $api_xml_response->Rows->Row->attributes()->Description . '). ' . $api_xml_response->Rows->Row->attributes()->Cause . '. Resolution: ' . $api_xml_response->Rows->Row->attributes()->Resolution);
        }

        // Parse the XML.
        if (!empty($api_xml_response->Rows)) {
          $parsed_api_response = array();

          $row_count = 0;

          // Loop through each XML row.
          foreach ($api_xml_response->Rows->Row as $api_xml_row) {
            // Initialize a new object for this item.
            $parsed_api_response[$row_count] = new stdClass();

            // Loop through each XML column.
            foreach ($api_xml_response->Columns->Column as $api_xml_column) {
              // Get the column name.
              $xml_column_name = $api_xml_column->attributes()->Name;

              // Use the column name to set the relevant field from the row on
              // our parsed API object array.
              $parsed_api_response[$row_count]->{$xml_column_name} = $api_xml_row->attributes()->{$xml_column_name}->__toString();
            }

            $row_count++;
          }
        }

        break;
    }

    return $parsed_api_response;
  }

}
