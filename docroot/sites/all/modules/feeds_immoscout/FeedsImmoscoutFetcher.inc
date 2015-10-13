<?php

/**
 * @file
 * Home of the FeedsImmoscoutFetcher and related classes.
 */

/**
 * Definition of the import batch object created on the fetching stage by
 * FeedsImmoscoutFetcher.
 */
class FeedsImmoscoutFetcherResult extends FeedsFetcherResult {
  /**
   * Constructor.
   */
  public function __construct($accesskey, $vendorid, $multiquery, $fetchexpose, $picturetype) {
    parent::__construct('');
    $this->accesskey = $accesskey;
    $this->vendorid = $vendorid;
    $this->multiquery = $multiquery;
    $this->fetchexpose = $fetchexpose;
    $this->picturetype = $picturetype;
  }

  /**
   * Overrides parent::getRaw();
   */
  public function getRaw() {
    $element = _immoscout_api_fetch($this->accesskey, $this->vendorid,
        $this->multiquery, $this->fetchexpose, $this->picturetype);
    return $element;
  }
}

/**
 * Fetches data via HTTP.
 */
class FeedsImmoscoutFetcher extends FeedsFetcher {

  /**
   * Implements FeedsFetcher::fetch().
   */
  public function fetch(FeedsSource $source) {
    $source_config = $source->getConfigFor($this);
    return new FeedsImmoscoutFetcherResult($source_config['accesskey'], $source_config['vendorid'],
        $source_config['multiquery'], $this->config['fetchexpose'], $this->config['picturetype']);
  }

  /**
   * Source form.
   */
  public function sourceForm($source_config) {
    $form = array();
    $form['accesskey'] = array(
      '#type' => 'textfield',
      '#title' => t('Access Key'),
      '#default_value' => empty($source_config['accesskey']) ? '' : $source_config['accesskey'],
      '#description' => t('Access Key for Immoscout API session.'),
    );
    $form['vendorid'] = array(
      '#type' => 'textfield',
      '#title' => t('Vendor ID'),
      '#default_value' => empty($source_config['vendorid']) ? '' : $source_config['vendorid'],
      '#description' => t('Vendor ID for Immoscout API session.'),
    );

  //  pageSize="50"
  //   pictureType="Result">
  //  <Sorting name="LastModificationDate" order="DESC" />

    $options = array(
      'AppartmentBuyQuery' => t('Appartment Buy'),
      'AppartmentRentQuery' => t('Appartment Rent'),
      'GastronomyQuery' => t('Gastronomy'),
      'HouseBuyQuery' => t('House Buy'),
      'HouseRentQuery' => t('House Rent'),
      'HouseTypeQuery' => t('House Type'),
      'IndustryQuery' => t('Industry'),
      'InvestmentQuery' => t('Investment'),
      'MiscQuery' => t('Misc'),
      'OfficeQuery' => t('Office'),
      'SiteQuery' => t('Site'),
      'StoreQuery' => t('Store'),
      'WazQuery' => t('Wohnen auf Zeit'),
    );
    $form['multiquery'] = array(
      '#type' => 'select',
      '#title' => 'Multi Query settings',
      '#default_value' => empty($source_config['multiquery']) ? array_keys($options) : $source_config['multiquery'],
      '#options' => $options,
      '#description' => 'Multi Query settings',
      '#multiple' => TRUE,
      '#size' => count($options),
    );

    return $form;
  }

  /**
   * Override parent::sourceFormValidate().
   */
  public function sourceFormValidate(&$values) {
    if (empty($values['accesskey'])) {
      form_set_error('feeds][source', t('You need an access key.'));
    }
    elseif (empty($values['vendorid'])) {
      form_set_error('feeds][source', t('You need a vendor id.'));
    }
  }

  /**
   * Override parent::sourceSave().
   */
  public function sourceSave(FeedsSource $source) {

  }

  /**
   * Override parent::sourceDelete().
   */
  public function sourceDelete(FeedsSource $source) {

  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    return array(
      'fetchexpose' => TRUE,
      'picturetype' => 'Expose',
    );
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $form = array();
    $form['fetchexpose'] = array(
      '#type' => 'checkbox',
      '#title' => t('Fetch each Expose'),
      '#description' => t('Fetch the Expose page for each Result.'),
      '#default_value' => $this->config['fetchexpose'],
    );
    $options = array(
      'Result' => t('Result - Kleines Bild für die Ergebnisliste (62x62px)'),
      'Original' => t('Original - Original Bild vom Anbieter'),
      'OdW' => t('OdW - Objekt der Woche Bild'),
      'Large' => t('Large - Grosses Bild'),
      'Expose' => t('Expose - Bild für Exposé'),
    );
    $form['picturetype'] = array(
      '#type' => 'select',
      '#title' => 'Request this picture size',
      '#default_value' => $this->config['picturetype'],
      '#options' => $options,
      '#description' => 'Picture size to request',
      '#multiple' => FALSE,
      '#size' => count($options),
    );
    return $form;
  }

}

/**
 * Helper. XML API.
 */

function _immoscout_api_xml_request($url, $request) {
  $host = 'api.immobilienscout24.de';
  $port = 80;

  $fp = fsockopen($host, $port, $errno, $errstr);
  $query = "POST {$url} HTTP/1.0\nUser_Agent: Drupal Feeds\nHost: {$host}\nContent-Type: text/xml\nContent-Length: " . strlen($request) . "\n\n{$request}\n";
  // use CRLF?

  if (!fputs($fp, $query, strlen($query))) {
    $errstr = "Write error";
    return 0;
  }

  $response = '';
  while (!feof($fp)) {
    $response .= fgets($fp);
  }
  fclose($fp);

  // strip header
  return substr($response, strpos($response, "\r\n\r\n") + 4);
}

function _immoscout_api_fetch($accesskey, $vendorid, $multiquery, $fetchexpose=TRUE, $picturetype='Expose') {
  $result = array();
  $session_service = 'http://api.immobilienscout24.de/api/xmlhttp/SessionService';
  $content = '<?xml version="1.0" encoding="UTF-8" ?>
  <CreateSessionRequest xmlns="http://www.immobilienscout24.de/api/schema/general/1.0">
    <accesskey>' . $accesskey . '</accesskey>
    <vendor>' . $vendorid . '</vendor>
  </CreateSessionRequest>';

  $xml = _immoscout_api_xml_request($session_service, $content);
  $result['session'] = $xml;
  try {
    $session = @ new SimpleXMLElement($xml);
  }
  catch (Exception $ignore) {
    return FALSE;
  }
  // root is CreateSessionResult
  $request_service = $session->RequestService;

  foreach ($multiquery as $key => $val) {
    $multiquery_xml .= '<MultiQuery><' . $val . ' /></MultiQuery>';
  }
  $content = '<?xml version="1.0" encoding="UTF-8" ?>
  <QueryRequest xmlns="http://www.immobilienscout24.de/api/schema/request/1.0">
    <ResultPage pageNumber="0" pageSize="50" pictureType="' . $picturetype . '">
      <Sorting name="LastModificationDate" order="DESC" />
    </ResultPage>
    ' . $multiquery_xml . '
  </QueryRequest>';

  $xml = _immoscout_api_xml_request($request_service, $content);
  $result['request'] = $xml;
  try {
    $request = @ new SimpleXMLElement($xml);
  }
  catch (Exception $ignore) {
    return FALSE;
  }
  // root is QueryResult

  if ($fetchexpose) {
    foreach ($request->ResultLine as $resultline) {
      $proxy = $resultline->children();
      $a = $proxy[0]; // e.g. AppartmentRentProxy

      $expose_uuid = (string) $a['uuid'];
      $expose_code = "expose-{$expose_uuid}";
//        $cached_data = _immoscout_api_cache_get($expose_code);
//        if (is_object($cached_data)
//       && $a->LastModificationDate == $cached_data->LastModificationDate) {
//          $cached_data->from_cache = TRUE;
//          $result[$expose_uuid] = $cached_data;
//        }
//        else {
      $content = '<?xml version="1.0" encoding="UTF-8" ?>
      <ExposeRequest xmlns="http://www.immobilienscout24.de/api/schema/expose/1.0" uuid="' . $expose_uuid . '" pictureType="Original"></ExposeRequest>';
      $xml = _immoscout_api_xml_request($request_service, $content);
      $result[$expose_uuid] = $xml;
//          _immoscout_api_cache_set($expose_code, $xml);
//        }
    }
  }

  return serialize($result);
}
