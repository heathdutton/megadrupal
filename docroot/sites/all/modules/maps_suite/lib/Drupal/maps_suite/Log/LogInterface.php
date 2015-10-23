<?php

/**
 * @file
 * Defines the MaPS Suite Log interface.
 */

namespace Drupal\maps_suite\Log;

use Drupal\maps_suite\Log\Context\ContextInterface;
use Drupal\maps_suite\Log\Observer\ObserverInterface;

/**
 * MaPS Suite Log Interface
 */
interface LogInterface {

  /**
   * Default log directory.
   */
  const LOG_DIR = 'maps_suite/log';

  /**
   * Tag name of the "content" tag, which contain all contexts
   */
  const CONTENT_ROOT = '__content__';

  /**
   * Save the log as an XML file and dispatch the log to the observers.
   */
  public function save();

  /**
   * Get the log directory relative to Drupal file system.
   *
   * @return string
   */
  public function getLogDir();

  /**
   * Add a new context.
   *
   * @param $definition
   *   The new context.
   * @param $type
   *
   * @return LogInterface
   *   The current object.
   */
  public function addContext(ContextInterface $context, $type = 'sibling');

  /**
   * Set the current operation count.
   *
   * @param int
   *    The number of the current operation
   *
   * @return LogInterface
   *   The current object.
   */
  public function setCurrentOperation($index);

  /**
   * Increment the current operation counter.
   */
  public function incrementCurrentOperation();

  /**
   * Set the total opeartions number.
   *
   * @param int $count
   *   The total number of operations.
   *
   * @return LogInterface
   *   The current object.
   */
  public function setTotalOperations($count);

  /**
   * Get the percentage of completion of the log related tasks.
   *
   * @return int
   */
  public function getPercentage();

  /**
   * Add a log message.
   *
   * @param $text
   *   The text of the message. It may contain placeholders.
   * @param array $options
   *   An array of options that may contain:
   *   - level: (optional) The log level (see watchdog error levels). Default to
   *     WATCHDOG_DEBUG.
   *   - variables: (optional) An array of variables to display for debugging purposes.
   *   - backtrace: Whether to generate a backtrace or not. Default to FALSE.
   *   - links: (optional) A list of URLs to display in the log, in order to give some details
   *     on the context.
   *   - context: (optional) The context for which remove and add the message. If not specified,
   *     the current context is used. If the specified context does not exists, it will be
   *     created. Default to NULL.
   *   - root: (optional) If TRUE, the pointer is moved to the root element, before the context
   *     selection (see the 'context' option above).
   *   - autocreate: (optional) Flag indicating whether the context should be created if it does
   *     not exist. Default to FALSE.
   *   - delete_messages: (optional) Clear the existing messages in the specified context before
   *     adding the new message.
   *
   * @return LogInterface
   *   The current object.
   *
   * @see watchdog()
   */
  public function addMessage($text, array $options = array());

  /**
   * Add a message from a catched exception
   *
   * @param $e
   *    The catched exception.
   * @param $links
   *    An array of links.
   *
   * @return LogInterface
   *   The current object.
   */
  public function addMessageFromException(\Exception $e, array $links = array());

  /**
   * Delete all messages for the current context.
   *
   * @return LogInterface
   *   The current object.
   */
  public function deleteMessages();

  /**
   * Get all messages for the current context
   *
   * @return $messages.
   *   An array of all messages for this context.
   */
  public function getMessages();

  /**
   * Update the error level of a given context recursively.
   *
   * All parents of the context may have their log level updated if necessary.
   *
   * @param $context
   *   The current context.
   * @param $level
   *   The error level.
   */
  public function updateContextsLevel($context, $level);

  /**
   * Set the internal pointer to the content root.
   *
   * @return LogInterface
   *   The current object.
   */
  public function moveToContentRoot();

  /**
   * Delete the current context and set the internal pointer to its parent.
   *
   * @return LogInterface
   *   The current object.
   */
  public function deleteContext();

  /**
   * Move the internal pointer to the given child context.
   *
   * @param $contextName
   *   The name of the context we are looking for.
   * @param $autoCreate
   *   Flag indicating whether the context should be created if it does not exist as a
   *   child of the current context. Default to FALSE.
   *
   * @return LogInterface
   *   The current object.
   */
  public function goToContext($contextName, $autoCreate = FALSE);

  /**
   * Set the internal cursor to a parent context.
   *
   * @param $contexttype
   *   If specified, the cursor is set to the closest parent of the given context type.
   *
   * @return LogInterface
   *   The current object.
   */
  public function moveToParent($contextType);

  /**
   * Add a Log Observer.
   *
   * @param $observer
   *   A ObserverInterface object.
   *
   * @return LogInterface
   *   The current object.
   */
  public function addObserver(ObserverInterface $observer);

  /**
   * Get the log contextual data.
   *
   * @return array
   */
  public function getRelatedTokens();

  /**
   * Set a contextual data.
   *
   * @param $key
   *   The data key.
   * @param $value
   *   The data.
   *
   * @return LogInterface
   *   The current object.
   *
   * @todo Check if the token type exists.
   */
  public function setRelatedToken($key, $value);

  /**
   * Set the Log introduction message.
   *
   * @param $message
   *  The content of the introduction message.
   *
   * @return LogInterface
   *   The current object.
   */
  public function setIntroductionMessage($message);

  /**
   * Get the introduction message.
   *
   * @return string
   */
  public function getIntroductionMessage();

  /**
   * Get the log Path.
   *
   * @return string
   *   The log path.
   */
  public function getPath();

  /**
   * Get the log file URL.
   *
   * @return string
   */
  public function getUrl();

  /**
   * Get the log type.
   *
   * @return string
   *   The log type.
   */
  public function getType();

  /**
   * Magic method; cast to string.
   *
   * @return string
   *   The log URL.
   */
  public function __toString();

}
