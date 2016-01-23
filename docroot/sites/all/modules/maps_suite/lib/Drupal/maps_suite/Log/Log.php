<?php

/**
 * @file
 *
 * Defines the MaPS Suite Log base class.
 */

namespace Drupal\maps_suite\Log;

use Drupal\maps_suite\Log\Context\Context;
use Drupal\maps_suite\Log\Context\ContextInterface;
use Drupal\maps_suite\Log\Observer\ObserverInterface;
use Drupal\maps_suite\Exception\Exception;
use Drupal\maps_suite\Exception\LogException;

/**
 * MaPS Suite Log class.
 */
class Log implements LogInterface {

  /**
   * The DOM document.
   *
   * @var \DOMDocument
   */
  protected $domDocument;

  /**
   * The current context.
   *
   * @var ContextInterface
   */
  public $currentContext;

  /**
   * The log file name.
   *
   * @var string
   */
  protected $filename;

  /**
   * The log uri.
   *
   * @var string
   */
  protected $uri;

  /**
   * The full log path.
   *
   * @var string
   */
  protected $path;

  /**
   * The log type.
   *
   * @var string
   */
  protected $type;

  /**
   * Some contextual data related to the log.
   *
   * @var array
   */
  protected $relatedTokens = array();

  /**
   * The MaPS Suite Log obervers.
   *
   * @var \SplObjectStorage
   */
  protected $observers;

  /**
   * The current operation (in case of batch processing).
   *
   * @var int
   */
  protected $currentOperation = 0;

  /**
   * The total number of operatiopns.
   *
   * @var int
   */
  protected $totalOperations = 1;

  /**
   * Class constructor
   *
   * @param string $type
   *   The type of log.
   * @param int id
   *   The log id.
   * @param boolean test
   *   Flag indicating if we are running tests.
   */
  public function __construct($type, $id = NULL, $test = FALSE) {
    $this->type = $type;

    if (is_null($id)) {
      $id = microtime();
    }

    $this->filename = hash('crc32', $type . $id) . '.xml';
    $this->uri = ($test === FALSE) ? file_default_scheme() : 'temporary';
    $this->uri .= '://' . $this->getLogDir() . '/' . $type ;

    $this->setPath($this->uri . '/' . $this->filename);
    $this->domDocument = new \DOMDocument();
    $this->observers = new \SplObjectStorage();

    if (is_file($this->getPath())) {
      $this->domDocument->load($this->getPath());
    }
    else {
      $this->initDomDocument();
    }
  }

  /**
   * @inheritdoc
   */
  public function save() {
    if ($this->writeXML()) {
      $this->dispatch();
    }
    else {
      watchdog('maps_suite', 'MaPS Suite Log: an error occured while writing the XML file. Context data: !data', array('!data' => (string) $this), WATCHDOG_ERROR);
    }
  }

  /**
   * @inheritdoc
   */
  public function getLogDir() {
    return variable_get('maps_suite_log_path', LogInterface::LOG_DIR);
  }

  /**
   * @inheritdoc
   */
  public function addContext(ContextInterface $context, $type = 'sibling') {
    $contextNode = $this->domDocument->createElement('context');

    // Get attributes
    $attributes = $context->getAttributes();
    if (!empty($attributes)) {
      foreach ($attributes as $name => $attribute) {
        $contextNode->setAttribute($name, $attribute);
      }
    }

    // Get current element
    if (is_null($this->currentContext)) {
      $elements = $this->domDocument->getElementsByTagName(self::CONTENT_ROOT);

      if (!$elements->length) {
        throw new LogException('Content node does not exist.', 0, array());
      }
      $current = $elements->item(0);
    }
    else {
      $current = $this->currentContext;
      if ($type === 'sibling') {
        $current = $current->parentNode;
      }
    }

    $current->appendChild($contextNode)
      ->setAttribute('level', WATCHDOG_DEBUG)
        ->parentNode
      ->setAttribute('type', $context->getType());

    $this->currentContext = $contextNode;

    return $this;
  }

  /**
   * @inheritdoc
   */
  public function addMessage($text, array $options = array()) {
    $options += array(
      'level' => WATCHDOG_DEBUG,
      'variables' => array(),
      'backtrace' => FALSE,
      'links' => array(),
      'context' => NULL,
      'root' => FALSE,
      'autocreate' => TRUE,
      'delete_messages' => FALSE,
    );

    if (!empty($options['context'])) {
      if (!empty($options['root'])) {
        $this->moveToContentRoot();
      }

      $this->goToContext($options['context'], !empty($options['autocreate']));
    }

    if ($this->currentContext->tagName != 'context') {
      watchdog('maps_suite', 'Try to write a log message while not in a valid context: @name.', array('@name' => $this->currentContext->tagName), WATCHDOG_ERROR);
      return $this;
    }

    if (!empty($options['delete_messages'])) {
      $this->deleteMessages();
    }

    // @todo is that really necessary?
    if (is_array($text)) {
      $text = serialize($text);
    }

    $messageNode = $this->domDocument->createElement('message');
    $messageNode->setAttribute('level', $options['level'])
      ->parentNode
      ->appendChild($this->domDocument->createElement('text'))
      ->appendChild($this->domDocument->createCDATASection($text));

    // Variables.
    if ($options['variables']) {
      $messageNodeVariables = $this->domDocument->createElement('variables');

      foreach ($options['variables'] as $name => $variable) {
        $messageNodeVariables->appendChild($this->domDocument->createElement('variable'))
          ->setAttribute('name', $name)
          ->parentNode
          // @todo should we use highlight_string?
          ->appendChild($this->domDocument->createCDATASection($variable));
      }

      $messageNode->appendChild($messageNodeVariables);
    }

    // Display a backtrace?
    if ($options['backtrace']) {
      // @todo improve the backtrace display.
      // @todo Causes some problems. Need step by step debugging.
//      $backtrace = debug_backtrace();
//      $messageNode->appendChild($this->domDocument->createElement('backtrace', $backtrace));
    }

    // Links
    if ($options['links']) {
      $messageNodeLinks = $this->domDocument->createElement('links');

      foreach ($options['links'] as $link) {
        $messageNodeLinks->appendChild($this->domDocument->createElement('link'))
          ->appendChild($this->domDocument->createCDATASection($link));
      }

      $messageNode->appendChild($messageNodeLinks);
    }

    $this->currentContext->appendChild($messageNode);
    $this->updateContextsLevel($this->currentContext, $options['level']);
    return $this;
  }

  /**
   * @inheritdoc
   */
  public function deleteMessages() {
    if (!empty($this->currentContext->childNodes)) {
      foreach ($this->currentContext->childNodes as $child) {
        $this->currentContext->removeChild($child);
      }
    }

    return $this;
  }

  /**
   * @inheritdoc
   */
  public function deleteContext() {
    $current = $this->currentContext;
    $this->moveToParent();
    $this->currentContext->removeChild($current);
    return $this;
  }

  /**
   * @inheritdoc
   */
  public function goToContext($contextName, $autoCreate = FALSE) {
    if (!empty($this->currentContext->childNodes)) {
      foreach ($this->currentContext->childNodes as $child) {
        if ($child->getAttribute('type') == $contextName) {
          $this->currentContext = $child;
          return $this;
        }
      }
    }

    if ($autoCreate) {
      $this->addContext(new Context($contextName), 'child');
    }

    return $this;
  }

  /**
   * @inheritdoc
   */
  public function addMessageFromException(\Exception $e, array $links = array()) {
    return $this->addMessage($e->getMessage(), array(
      'level' => $e->getCode(),
      'variables' => ($e instanceof Exception) ? $e->getContext() : array(),
      'backtrace' => TRUE,
      'links' => $links,
    ));
  }

  /**
   * @inheritdoc
   */
  public function updateContextsLevel($context, $level) {
    if ($context->getAttribute('level') >= $level) {
      $context->setAttribute('level', $level);

      if ($context->parentNode->tagName === 'context') {
        $this->updateContextsLevel($context->parentNode, $level);
      }
    }
  }

  /**
   * @inheritdoc
   */
  public function moveToContentRoot() {
    $this->currentContext = $this->domDocument->getElementsByTagName(self::CONTENT_ROOT)->item(0);
    return $this;
  }

  /**
   * @inheritdoc
   */
  public function moveToParent($contextType = NULL) {
    if (!is_null($contextType)) {
      if (!$this->currentContext->parentNode) {
        return FALSE;
      }

      if ($this->currentContext->getAttribute('type')) {
        do {
          $this->currentContext = $this->currentContext->parentNode;

          if ($this->currentContext->tagName === self::CONTENT_ROOT) {
            break;
          }
        } while ($contextType != $this->currentContext->getAttribute('type'));
      }
    }
    else {
      $this->currentContext = $this->currentContext->parentNode;
    }

    return $this;
  }

  /**
   * @inheritdoc
   */
  public function addObserver(ObserverInterface $observer) {
    $this->observers->attach($observer);
    return $this;
  }

  /**
   * Initialize the dom document with main necessary tags
   */
  protected function initDomDocument() {
    global $base_url;

    $xslt = $this->domDocument->createProcessingInstruction('xml-stylesheet', 'type="text/xsl" href="' . url('admin/maps-suite/log.xsl', array('absolute' => TRUE)) . '" version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"');

    $this->domDocument->appendChild($xslt)
      ->parentNode
      ->appendChild($this->domDocument->createElement('root'))
        ->appendChild($this->domDocument->createElement(self::CONTENT_ROOT));
  }

  /**
   * @inheritdoc
   */
  public function __toString() {
    return file_create_url($this->getPath());
  }

  /**
   * @inheritdoc
   */
  public function setIntroductionMessage($message) {
    $xpath = new \DOMXPath($this->domDocument);
    $root = $this->domDocument->getElementsByTagName('root')->item(0);
    $content = $this->domDocument->getElementsByTagName(self::CONTENT_ROOT)->item(0);

    $elements = $xpath->query('//root/introduction_message');
    if ($elements->length > 1) {
      throw new LogException('Multiple introduction message node: %introductionMessage', 0, array('%introductionMessage' => $message));
    }
    else if ($elements->length == 1) {
      $elements->item(0)->parentNode->removeChild($elements->item(0));
    }

    // Introduction Message
    $introductionMessageNode = $this->domDocument->createElement('introduction_message');

    // The introduction message is supposed to be valid HTML. But...
    if (module_exists('filter')) {
      $message = _filter_htmlcorrector($message);
    }

    $introductionMessageXml = $this->domDocument->createDocumentFragment();
    $introductionMessageXml->appendXML('<html>' . $message . '</html>');

    $introductionMessageNode->appendChild($introductionMessageXml);
    $content->parentNode->insertBefore($introductionMessageNode, $content);

    return $this;
  }

  /**
   * @inheritdoc
   */
  public function getMessages() {
    $messages = array();

    if (!empty($this->currentContext->childNodes)) {

      foreach ($this->currentContext->childNodes as $child) {
        if ($child->tagName === 'message') {
          $messages[] = $child->nodeValue;
        }
      }
    }

    return $messages;
  }

  /**
   * Set log path
   *
   * @param $path.
   *  The path to log files.
   */
  protected function setPath($path) {
    $this->path = $path;
  }

  /**
   * @inheritdoc
   */
  public function getPath() {
    return $this->path;
  }

  /**
   * Return the dom document
   *
   * @return DOMDocument
   *
   * @todo make this method protected and add the necessary mock classes
   *   for the functional tests (Log and Logger).
   */
  public function getDomDocument() {
    return $this->domDocument;
  }

  /**
   * @inheritdoc
   */
  public function getType() {
    return $this->type;
  }

  /**
   * @inheritdoc
   */
  public function setCurrentOperation($index) {
    $this->currentOperation = (int) $index;
    return $this;
  }

  /**
   * @inheritdoc
   */
  public function setTotalOperations($count) {
    // Prevent division by zero.
    if (!$count) {
      $count = 1;
    }
    
    $this->totalOperations =  ceil($count);
    return $this;
  }

  /**
   * @inheritdoc
   */
  public function getPercentage() {
    return (int) $this->currentOperation / $this->totalOperations;
  }

  /**
   * @inheritdoc
   */
  public function getIntroductionMessage() {
    $xpath = new \DOMXPath($this->domDocument);
    $elements = $xpath->query('//root/introduction_message');

    if (!$elements->length) {
      return '';
    }

    $introductionMessageNode = $elements->item(0);

    return $introductionMessageNode->nodeValue;
  }

  /**
   * @inheritdoc
   */
  public function getRelatedTokens() {
    return $this->relatedTokens;
  }

  /**
   * @inheritdoc
   */
  public function setRelatedToken($key, $value) {
    $this->relatedTokens[$key] = $value;
    return $this;
  }

  /**
   * Dispatch the current log to the observers.
   */
  protected function dispatch() {
    $percentage = $this->getPercentage();
    $method = $percentage < 1 ? 'dispatchPartialLog' : 'dispatchLog';

    foreach ($this->observers as $observer) {
      $observer->{$method}($this);
    }
  }

  /**
   * Save the data to the XML file.
   *
   * @return $boolean
   *    Indicates if the writting has been successfull.
   */
  protected function writeXML() {
    if (!file_exists($this->uri)) {
      drupal_mkdir($this->uri, NULL, TRUE);
    }

    if ($path = drupal_realpath($this->getPath())) {
      return file_put_contents($path, $this->domDocument->saveXML());
    }

    return FALSE;
  }

  /**
   * @inheritdoc
   */
  public function incrementCurrentOperation() {
    $this->currentOperation++;
  }

  /**
   * @inheritdoc
   */
  public function getUrl() {
    return file_create_url($this->getPath());
  }

}
