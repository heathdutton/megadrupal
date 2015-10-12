<?php

 /**
 * @file
 * Contains PRH.
 *
 * Client for querying PRH interface
 */
class PRH {

  /**
   * Interface configuration
   *
   * @var array
   * @see fonecta.ini
   */
  public $configuration;

  /*
   * Initialize client
   */
  public function __construct() {

    $this->configuration['association_url'] = variable_get('prh_association_search_url');
    $this->configuration['username'] = variable_get('prh_username');
    $this->configuration['password'] = str_replace('"', '', variable_get('prh_password'));
  }

  /**
   * Query PRH interface for $name and filter results by record
   *
   * @param string regnum to search
   * @return SimpleXML Object
   */
  public function getValidAssociationFromPRH($regnum, $association='') {

    $requestParameters = array(
        'kyselyversio' => '0001',
        'hakukohde' => 1,
        'nimi' => $association,
        'rekisterinumero' => $regnum,
    );

    $requestUri = $this->configuration['association_url'] . '?';
    foreach ($requestParameters as $key => $value) {
      $requestUri .= $key . '=' . urlencode($value) . '&';
    }
    $requestUri = trim($requestUri, '&');

    $username = $this->configuration['username'];
    $password = $this->configuration['password'];

    $context = stream_context_create(array(
      'http' => array(
        'header'  => "Authorization: Basic " . base64_encode("$username:$password"),
      )
    ));

    $data = file_get_contents($requestUri, FALSE, $context);

    $data = simplexml_load_string(str_replace('<?xml version="1.0" encoding="ISO-8859-1" ?>', '', $data));

    return $data->yhdistys;
  }
}
