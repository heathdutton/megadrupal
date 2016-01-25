<?php
/**
 * @file
 * Feed formatter implementation.
 */

/**
 * Feed format usage interface.
 */
interface PHUpdatesFormatInterface {
  /**
   * Formats a tree.
   *
   * @param PHUpdatesTreeWrapper $tree
   *   Tree wrapper object.
   * @return string|array
   *   String formatted output or render array.
   */
  public function format($tree);

  /**
   * Formats an error.
   *
   * @param PHUpdatesInfoError $error
   *   Update info error to format.
   * @return string|array
   *   String formatted output or render array.
   */
  public function formatError($error);
}

/**
 * Error for when an update feed is invalid.
 */
abstract class PHUpdatesInfoError extends Exception {
  /**
   * Info wrapper associated with this exception.
   * @var PHUpdatesInfoWrapper
   */
  protected $info;

  /**
   * HTTP status for this error.
   * @var string
   */
  protected $status;

  /**
   * Constructs the exception.
   */
  public function __construct(PHUpdatesInfoWrapper $info, $message = '', $httpStatus = '') {
    parent::__construct($message);
    $this->info = $info;
    $this->status = $httpStatus;
  }

  /**
   * Returns the associated info wrapper.
   */
  public function getInfo() {
    return $this->info;
  }

  /**
   * Returns the HTTP status for this error.
   */
  public function getStatus() {
    return $this->status;
  }
}

/**
 * Incomplete info error.
 */
class PHUpdatesInfoIncompleteError extends PHUpdatesInfoError {
  /**
   * Missing property.
   */
  protected $missingProperty;

  /**
   * Constructs the error with information about the missing property.
   *
   * @param PHUpdatesInfoWrapper $info
   * @param string $missingProperty
   * @param string $message
   */
  public function __construct(PHUpdatesInfoWrapper $info, $missingProperty, $message = '') {
    parent::__construct($info, $message ? $message : 'Missing property "' . $missingProperty . '"', '404 Not Found');
    $this->missingProperty = $missingProperty;
  }

  /**
   * Returns the missing part in question.
   */
  public function getMissingProperty() {
    return $this->missingProperty;
  }
}

/**
 * Default formatter base.
 */
abstract class PHUpdatesFormat implements PHUpdatesFormatInterface {
  protected $name = '';
  protected $info;

  /**
   * Constructs a formatter.
   */
  public function __construct() {
    $this->setUp();
  }

  /**
   * Sets up the formatter.
   */
  protected function setUp() {
    $this->info = ph_updates_get_format_info($this->name);
  }

  /**
   * Formats a tree.
   *
   * @param PHUpdatesTreeWrapper $tree
   *   Tree wrapper object.
   * @return string|array
   *   String formatted output or render array.
   */
  public function format($tree) {
    $formatInfo = $tree->getFormatInfo($this->name);
    return $this->formatTree($tree->value(), $formatInfo);
  }

  /**
   * Formats a tree with format info.
   *
   * @param array $tree
   *   Tree data.
   * @param array $formatInfo
   *   Format info.
   * @return string|array
   *   String formatted output or render array.
   */
  abstract protected function formatTree(array $tree, array $formatInfo);
}

/**
 * XML formatter.
 */
class PHUpdatesXMLFormat extends PHUpdatesFormat {
  protected $name = 'xml';

  /**
   * Formats a tree with format info.
   *
   * @param array $tree
   *   Tree data.
   * @param array $formatInfo
   *   Format info.
   * @return string|array
   *   String formatted output or render array.
   */
  protected function formatTree(array $tree, array $formatInfo) {
    $writer = $this->createFormatWriter($formatInfo);
    $output = $writer->format($tree);
    return $output;
  }

  /**
   * Formats an error.
   *
   * @param PHUpdatesInfoError $error
   *   Update info error to format.
   * @return string|array
   *   String formatted output or render array.
   */
  public function formatError($error) {
    $writer = $this->createFormatWriter(array(), 'error');
    $output = $writer->format($error->getMessage());
    return $output;
  }

  /**
   * Creates an XML writer.
   */
  protected function createFormatWriter(array $formatInfo = array(), $defaultRoot = 'root') {
    return new PHUpdatesXMLFormatWriter($formatInfo, $defaultRoot);
  }
}

/**
 * XML formatter.
 */
class PHUpdatesXMLFormatWriter {
  /**
   * XML writer instance.
   * @var XMLWriter
   */
  protected $writer;

  /**
   * Format info.
   * @var array
   */
  protected $formatInfo;

  /**
   * XML namespaces.
   * @var array
   */
  protected $namespaces = array();

  /**
   * Default root element.
   * @var string
   */
  private $defaultRoot;

  /**
   * Constructs an XML format writer.
   */
  public function __construct(array $formatInfo = array(), $defaultRoot = 'xml') {
    $this->formatInfo = $formatInfo;
    if (is_string($defaultRoot)) {
      $this->defaultRoot = $defaultRoot;
    }
    $this->setUp();
  }

  /**
   * Sets up the formatter.
   */
  protected function setUp() {
    $this->writer = $this->createWriter();
    // Prepare default root element name.
    $this->formatInfo += array('/' => array());
    $this->formatInfo['/'] += array('name' => $this->defaultRoot);
  }

  /**
   * Creates an XML writer with the root tree.
   */
  protected function createWriter() {
    $writer = new XMLWriter();
    $this->setUpWriter($writer);
    return $writer;
  }

  /**
   * Sets up an XML writer.
   */
  protected function setUpWriter(XMLWriter $writer) {
    $writer->openMemory();
    $writer->setIndent(TRUE);
    $writer->setIndentString('  ');
  }

  /**
   * Formats data for output.
   */
  public function format($data) {
    $this->writer->startDocument('1.0', 'UTF-8');
    $this->writeTree($data, '/', $this->formatInfo['/']);
    $this->writer->endDocument();
    $output = $this->writer->outputMemory(FALSE);

    // Process output.
    $output = $this->processOutput($output);

    return $output;
  }

  /**
   * Processes formatted output.
   */
  protected function processOutput($xml) {
    // Normalize line endings.
    $xml = str_replace(array("\r\n", "\r"), "\n", $xml);
    // Trim whitespace.
    $xml = trim($xml);

    return $xml;
  }

  /**
   * Writes data array.
   */
  protected function writeArray(array $data, $basePath, array $info = array()) {
    $pathPrefix = rtrim($basePath, '/');
    $info += array('list item' => 'item');
    foreach ($data as $key => $item) {
      // Determine item path.
      $itemBasePath = $pathPrefix . '/' . $key;
      // Prepare item info.
      $itemInfo = isset($this->formatInfo[$itemBasePath]) ? $this->formatInfo[$itemBasePath] : array();
      $itemInfo += array('name' => $key);
      if (is_int($key)) {
        $itemInfo['name'] = $info['list item'];
      }
      // Write tree.
      $this->writeTree($item, $itemBasePath, $itemInfo);
    }
  }

  /**
   * Writes a data tree, i.e. data with an element name.
   */
  protected function writeTree($tree, $basePath, array $info = array()) {
    // Write tree with root element.
    $this->writeElement($info['name'], $tree, $basePath, $info);
  }

  /**
   * Writes a named element.
   */
  protected function writeElement($name, $data, $basePath, array $info = array()) {
    if (!isset($data)) {
      return;
    }

    $info += array(
      'xmlns' => array(),
      'prefix' => NULL,
    );

    // Push namespaces.
    $namespaces = is_array($info['xmlns']) ? $info['xmlns'] : array();
    $namespaces = $this->pushNamespaces($namespaces);

    // Determine namespace parameters.
    $prefix = $uri = NULL;
    $addedNamespaces = $namespaces;
    if (is_string($info['xmlns']) || isset($info['xmlns'][''])) {
      $uri = is_string($info['xmlns']) ? $info['xmlns'] : $info['xmlns'][''];
    }
    if (isset($info['prefix'])) {
      $prefix = $info['prefix'];
      if (isset($namespaces[$prefix])) {
        // Set URI for namespace declaration to be added.
        $uri = $namespaces[$prefix];
        unset($addedNamespaces[$prefix]);
      }
    }

    // Start element.
    if (isset($prefix) || isset($uri)) {
      $success = @$this->writer->startElementNs($prefix, $name, $uri);
    }
    else {
      $success = @$this->writer->startElement($name);
    }
    if ($success) {
      $this->writeNamespaces($addedNamespaces);

      // Write data.
      $this->writeElementData($data, $basePath, $info);

      // End element.
      $this->writer->endElement();
    }

    // Pop namespaces.
    $this->popNamespaces($namespaces);
  }

  /**
   * Writes a data tree using XMLWriter.
   */
  protected function writeElementData($data, $basePath, array $info = array()) {
    // Write subtree.
    if (is_array($data)) {
      $this->writeArray($data, $basePath, $info);
    }
    // Write simple value.
    elseif (is_scalar($data)) {
      $this->writer->text($data);
    }
  }

  /**
   * Writes namespace attributes.
   *
   * @param array $namespaces
   *   Namespace URIs keyed by prefixes.
   */
  protected function writeNamespaces(array $namespaces) {
    foreach ($namespaces as $prefix => $uri) {
      if (!empty($prefix)) {
        $this->writer->writeAttributeNs('xmlns', $prefix, NULL, $uri);
      }
    }
  }

  /**
   * Registers namespaces.
   *
   * Only prefixes that are not mapped to matching URIs are added.
   *
   * @param array $namespaces
   *   Namespace URIs keyed by prefixes.
   * @return array
   *   Registered namespaces.
   */
  protected function pushNamespaces(array $namespaces) {
    $added = array();
    foreach ($namespaces as $prefix => $uri) {
      if (!empty($prefix)) {
        $this->namespaces += array($prefix => array());
        if ($this->lookupNamespace($prefix) !== $uri) {
          $this->namespaces[$prefix][] = $uri;
          $added[$prefix] = $uri;
        }
      }
    }
    return $added;
  }

  /**
   * Pops a namespace.
   */
  protected function popNamespaces(array $namespaces) {
    foreach (array_keys($namespaces) as $prefix) {
      if (!empty($prefix) && !empty($this->namespaces[$prefix])) {
        array_pop($this->namespaces[$prefix]);
      }
    }
  }

  /**
   * Selects a namespace URI based on declared prefixes in the format info.
   */
  protected function lookupNamespace($prefix) {
    return !empty($this->namespaces[$prefix]) ? end($this->namespaces[$prefix]) : NULL;
  }
}
