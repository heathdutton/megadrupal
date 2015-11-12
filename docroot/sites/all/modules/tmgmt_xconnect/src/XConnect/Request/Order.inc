<?php
/**
 * @file
 * The X-Connect order.
 */

/**
 * Class that represents the X-Connect order.
 */
class TmgmtXConnectRequestOrder {
  /**
   * The order name.
   *
   * @var string
   */
  private $orderName;

  /**
   * The source language.
   *
   * @var string
   */
  private $sourceLanguage;

  /**
   * The target languages.
   *
   * @var array
   */
  private $targetLanguages = array();

  /**
   * The order instructions.
   *
   * @var array
   */
  private $instructions = array();

  /**
   * The client reference.
   *
   * @var string
   */
  private $clientReference;

  /**
   * The configuration.
   *
   * @var array
   */
  private $config = array(
    'client_id' => '',
    'template_id' => '',
    'due_date' => 0,
    'issued_by' => '',
    'is_confidential' => 0,
    'service' => '',
    'needs_confirmation' => 1,
    'needs_quotation' => 0,
  );

  /**
   * Array of files that are part of the order.
   *
   * @var array
   */
  private $inputFiles = array();

  /**
   * The order XML wrapper.
   *
   * @todo: is the xsi:schemaLocation property correct?
   *        look like it should be:
   *        http://www.euroscript.com/escape/types/clientOrderRequestTypes.xsd
   *
   * @var string
   */
  private $xmlWrapper = <<<EOL
<?xml version="1.0" encoding="UTF-8"?>
<ClientWoRequest xmlns:tns="http://www.euroscript.com/escaepe/types"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://www.euroscript.com/escaepe/types clientOrderRequestTypes.xsd">
</ClientWoRequest>
EOL;


  /**
   * Create a new order.
   *
   * @param string $order_name
   *   The file name of the order to be send.
   * @param string $source_language
   *   The source language for the translation order.
   * @param array $config
   *   The configuration elements for the order.
   */
  public function __construct($order_name, $source_language, $config) {
    $this->orderName = $order_name;
    $this->sourceLanguage = $source_language;
    $this->setConfig($config);
  }

  /**
   * Fill in the configuration based on the given config array.
   *
   * @param array $config
   *   The config array to store.
   */
  protected function setConfig($config) {
    $config_keys = array_keys($this->config);
    foreach ($config_keys as $key) {
      if (isset($config[$key])) {
        $this->config[$key] = $config[$key];
      }
    }
  }

  /**
   * Get the XML as a string.
   *
   * @return string
   *   The XML as a string.
   */
  public function getXml() {
    $xml_string = $this->createXml()->asXML();

    // We have to add the tns namespace.
    $xml_string = str_replace(
      'ClientWoRequest',
      'tns:ClientWoRequest',
      $xml_string
    );

    return $xml_string;
  }

  /**
   * Create the order XML.
   *
   * @return SimpleXMLElement
   *   The generated XML object.
   */
  protected function createXml() {
    // Create the XML object from the data array.
    $xml = new SimpleXMLElement($this->xmlWrapper);
    $this->arrayToXml(
      $this->getXmlDataStructure(),
      $xml
    );

    return $xml;
  }

  /**
   * Get the default XML data.
   *
   * @return array
   *   The default XML data structure.
   */
  protected function getXmlDataStructure() {
    $data = array(
      'ClientId' => $this->getClientId(),
      'OrderName' => $this->getOrderName(),
      'TemplateId' => $this->getTemplateId(),
      'RequestDate' => $this->getRequestDate()->format('Y-m-d\TH:i:s'),
      'RequestedDueDate' => $this->getRequestDueDate()->format('Y-m-d'),
      'IssuedBy' => $this->getIssuedBy(),
      'ConfidentialOrder' => $this->getConfidentialOrder(),
      'SourceLanguageIsoCode' => $this->getSourceLanguage(),
      'TargetLanguages' => $this->getTargetLanguagesIsoCodes(),
      'Service' => $this->getService(),
      'ClientInstructions' => $this->getClientInstructions(),
      'ClientReference' => $this->getClientReference(),
      'ConfirmationRequested' => $this->getConfirmationRequested(),
      'QuotationRequested' => $this->getQuotationRequested(),
      'InputFiles' => $this->getInputFilesReferences(),
    );

    return $data;
  }

  /**
   * Get the client id.
   *
   * @return string
   *   The client ID.
   */
  public function getClientId() {
    return $this->config['client_id'];
  }

  /**
   * Get the template id.
   *
   * @todo: get data from where?
   *
   * @return string
   *   The template ID.
   */
  protected function getTemplateId() {
    return $this->config['template_id'];
  }

  /**
   * Get the order name.
   *
   * @return string
   *   The order name.
   */
  public function getOrderName() {
    return $this->orderName;
  }

  /**
   * Get the request date.
   *
   * @return DateTime
   *   The date time presentation of the request date.
   */
  public function getRequestDate() {
    $date = new DateTime();
    return $date;
  }

  /**
   * Get the request due date.
   *
   * The date is calculated based on the dueDate value in the settings.
   *
   * @return DateTime
   *   The date time presentation of the due date.
   */
  public function getRequestDueDate() {
    $due_date = new DateTime();
    if (empty($this->config['due_date'])) {
      return $due_date;
    }

    $interval = new DateInterval(
      'P' . $this->config['due_date'] . 'D'
    );

    $due_date->add($interval);
    return $due_date;
  }

  /**
   * Get the issuer identifier.
   *
   * @return string
   *   The email address.
   */
  public function getIssuedBy() {
    return $this->config['issued_by'];
  }

  /**
   * Get if the translation request is confidential.
   *
   * @return string
   *   Confidential Y(1) / N(0).
   */
  public function getConfidentialOrder() {
    return (int) $this->config['is_confidential'];
  }

  /**
   * Get the source language.
   *
   * @return string
   *   The source language of the translation.
   */
  public function getSourceLanguage() {
    return $this->sourceLanguage;
  }

  /**
   * Add a target language.
   *
   * @param string $language
   *   The target language to add to the order.
   */
  public function addTargetLanguage($language) {
    if (!in_array($language, $this->targetLanguages)) {
      $this->targetLanguages[] = $language;
    }
  }

  /**
   * Get the target languages.
   *
   * @return array
   *   The target language codes.
   */
  public function getTargetLanguages() {
    return $this->targetLanguages;
  }

  /**
   * Get the target language.
   *
   * @return array
   *   Array of target languages.
   */
  protected function getTargetLanguagesIsoCodes() {
    $languages = array();
    foreach ($this->targetLanguages as $language) {
      $languages[] = array(
        'IsoCode' => $language,
      );
    }

    return $languages;
  }

  /**
   * Get te service name.
   *
   * @return string
   *   The Service name.
   */
  public function getService() {
    return $this->config['service'];
  }

  /**
   * Add instructions to the order.
   *
   * @param string $instructions
   *   The instructions.
   */
  public function addInstructions($instructions) {
    $this->instructions[] = $instructions;
  }

  /**
   * Get the client instructions.
   *
   * @return string
   *   The client instructions regarding the translation.
   */
  public function getClientInstructions() {
    return (empty($this->instructions))
      ? 'None'
      : implode(PHP_EOL, $this->instructions);
  }

  /**
   * Set the client reference.
   *
   * @param string $reference
   *   The reference to use in the order.
   */
  public function setClientReference($reference) {
    $this->clientReference = $reference;
  }

  /**
   * Get the client reference.
   *
   * @return string
   *   The client reference.
   */
  public function getClientReference() {
    return $this->clientReference;
  }

  /**
   * Get if the confirmation is requested.
   *
   * @return string
   *   Requested Y(1) / N(0).
   */
  public function getConfirmationRequested() {
    return (int) $this->config['needs_confirmation'];
  }

  /**
   * Get if an quotation is required before translation is processed.
   *
   * @return string
   *   Requested Y(1) / N(0).
   */
  public function getQuotationRequested() {
    return (int) $this->config['needs_quotation'];
  }

  /**
   * Add an input file to the order.
   *
   * @param string $file_name
   *   The file name of the file to add to the order.
   */
  public function addInputFile($file_name) {
    if (!in_array($file_name, $this->inputFiles)) {
      $this->inputFiles[] = $file_name;
    }
  }

  /**
   * Get the input files.
   *
   * @return array
   *   The input file names part of the translation order.
   */
  public function getInputFiles() {
    return $this->inputFiles;
  }

  /**
   * Get the input files.
   *
   * @return array
   *   Array of input files.
   */
  protected function getInputFilesReferences() {
    $files = array();
    foreach ($this->inputFiles as $file_name) {
      $files[] = array(
        'InputFile' => array(
          'FileName' => $file_name,
          'FileReference' => 'Input/' . $file_name,
        ),
      );
    }

    return $files;
  }

  /**
   * Transforms an array into an XML object.
   *
   * @param array $array
   *   Array with values.
   * @param SimpleXMLElement $xml
   *   XML object.
   */
  protected function arrayToXml(array $array, SimpleXMLElement $xml) {
    foreach ($array as $key => $value) {
      $element_key = (is_numeric($key))
        ? 'item' . $key
        : $key;

      if (!is_array($value)) {
        $xml->addChild($element_key, htmlspecialchars($value));
        continue;
      }

      // Support numeric keyed array values.
      if (is_numeric($key)) {
        $this->arrayToXml($value, $xml);
        continue;
      }

      $sub_node = $xml->addChild($element_key);
      $this->arrayToXml($value, $sub_node);
    }
  }

}
