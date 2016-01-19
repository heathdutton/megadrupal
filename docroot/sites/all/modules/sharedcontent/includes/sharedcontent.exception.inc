<?php

/**
 * @file
 * Shared Content Exception classes.
 */


/**
 * No parent index record
 *
 * This error is thrown then trying to index a chapter where the parent
 * index record does not exists.
 *
 * @see sharedcontent_chapters_sharedcontent_index_presave().
 */
define('SHAREDCONTENT_ERROR_NO_PARENT_INDEX', 7);

class SharedContentCountIndexException extends Exception {

  function __construct($connection_name, $time_start, $time_end, Exception $previous = NULL) {
    $message = format_string('Could not count index for time frame !start – !end using connection !name.', array(
      '!name' => $connection_name,
      '!start' => $time_start,
      '!end' => $time_end,
    ));
    parent::__construct($message, 0, $previous);
  }
}

/**
 * Connection not found
 *
 * To push an assignment to a remote system using a connection name for
 * which no connection is configured.
 *
 * To prevent this exception to occur again, either remove the connection
 * name from all index records that does contain them or create a valid
 * connection using this connection mane.
 */
class SharedContentConnectionNotFoundException extends Exception {

  public function __construct($connection_name, Exception $previous = NULL) {
    $message = format_string('Could not find connection with name !name.', array(
      '!name' => $connection_name
    ));
    parent::__construct($message, 0, $previous);
  }
}

/**
 * Push index exception
 *
 * This exception is thrown on errors while pushing an index record to
 * a remote system. Refer to the encapsulated exception for further
 * details.
 *
 * @todo Provide a mechanism to push the index record and its associated
 *       assignments manually.
 */
class SharedContentPushIndexException extends Exception {

  public function __construct($connection_name, SharedContentIndex $index, Exception $previous = NULL) {
    $message = format_string('Could not push index !uuid to connection !name.', array(
      '!uuid' => isset($index->uuid) ? $index->uuid : NULL,
      '!name' => $connection_name
    ));
    parent::__construct($message, 0, $previous);
  }
}

/**
 * Synchronization error
 *
 * Indicates errors that occurred during synchronization.
 * Use the encapsulated exception to figure out what has gone wrong and
 * the arguments to see the associated data.
 *
 * Use drush sc-sync or the resynchronization button to fix inconsistent
 * data that might have emerged from this error.
 */
class SharedContentSynchronizeException extends Exception {

  private $arguments;

  function __construct($connection_name, array $arguments = array(), Exception $previous = NULL) {
    $this->arguments = $arguments;
    $message = format_string('Failed to synchronize index with !name', array(
      '!name' => $connection_name,
    ));
    parent::__construct($message, 0, $previous);
  }

  public function getArguments() {
    return $this->arguments;
  }
}

/**
 * Indexing errors
 *
 * This exception is thrown during errors in the indexing process.
 *
 * @see SHAREDCONTENT_ERROR_NO_PARENT_INDEX
 */
class SharedContentIndexingException extends Exception {

  public $data = array();

  public function __construct($code, array $data = NULL, Exception $previous = NULL) {
    $this->data = $data;
    switch ($code) {
      case SHAREDCONTENT_ERROR_NO_PARENT_INDEX:
        $message = 'Parent index record does not exists for chapter.';
        break;
      default:
        $message = 'Failed to update index.';
    }
    parent::__construct($message, $code, $previous);
  }
}

