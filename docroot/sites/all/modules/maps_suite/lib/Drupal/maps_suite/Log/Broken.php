<?php

/**
 * @file
 * Defines the MaPS Suite Log Broken class.
 */

namespace Drupal\maps_suite\Log;

use Drupal\maps_suite\Log\Context\ContextInterface;
use Drupal\maps_suite\Log\Observer\ObserverInterface;

/**
 * Maps Suite Log Broken class.
 *
 * Used for simple watchdog log.
 */
class Broken implements LogInterface {

  /**
   * @inheritdoc
   */
  public function save() {}

  /**
   * @inheritdoc
   */
  public function getLogDir() {}

  /**
   * @inheritdoc
   */
  public function addContext(ContextInterface $context, $type = 'sibling') {
    return $this;
  }

  /**
   * @inheritdoc
   */
  public function setCurrentOperation($index) {
    return $this;
  }

  /**
   * @inheritdoc
   */
  public function incrementCurrentOperation() {}

  /**
   * @inheritdoc
   */
  public function setTotalOperations($count) {
    return $this;
  }

  /**
   * @inheritdoc
   */
  public function getPercentage() {}

  /**
   * @inheritdoc
   */
  public function addMessage($text = '', array $options = array()) {
    // Make sure there is a severity and a link defined, to prevent fatal errors.
    //if (empty($options['level'])) {
    //  $options['level'] = WATCHDOG_NOTICE;
    //}
    //if (empty($options['links'])) {
    //  $options['links'] = array();
    //}
    //if (empty($options['variables'])) {
    //  $options['variables'] = array();
    //}
    //
    //if (variable_get('maps_suite_logs_enabled', TRUE)) {
    //  watchdog('maps_suite', $text, $options['variables'], $options['level'], reset($options['links']));
    //}
    return $this;
  }

  /**
   * @inheritdoc
   */
  public function addMessageFromException(\Exception $e, array $links = array()) {
    return $this;
  }

  /**
   * @inheritdoc
   */
  public function deleteMessages() {
    return $this;
  }

  /**
   * @inheritdoc
   */
  public function getMessages() {
    return array();
  }

  /**
   * @inheritdoc
   */
  public function updateContextsLevel($context, $level) {}

  /**
   * @inheritdoc
   */
  public function moveToContentRoot() {
    return $this;
  }

  /**
   * @inheritdoc
   */
  public function deleteContext() {
    return $this;
  }

  /**
   * @inheritdoc
   */
  public function goToContext($contextName, $autoCreate = FALSE) {
    return $this;
  }

  /**
   * @inheritdoc
   */
  public function moveToParent($contextType = NULL) {
    return $this;
  }

  /**
   * @inheritdoc
   */
  public function addObserver(ObserverInterface $observer) {
    return $this;
  }

  /**
   * @inheritdoc
   */
  public function getRelatedTokens() {
    return array();
  }

  /**
   * @inheritdoc
   */
  public function setRelatedToken($key, $value) {
    return $this;
  }

  /**
   * @inheritdoc
   */
  public function setIntroductionMessage($message) {
    return $this;
  }

  /**
   * @inheritdoc
   */
  public function getIntroductionMessage() {}

  /**
   * @inheritdoc
   */
  public function getPath() {}

  /**
   * @inheritdoc
   */
  public function getType() {}

  /**
   * @inheritdoc
   */
  public function getUrl() {}

  /**
   * @inheritdoc
   */
  public function __toString() {
    return '';
  }

}
