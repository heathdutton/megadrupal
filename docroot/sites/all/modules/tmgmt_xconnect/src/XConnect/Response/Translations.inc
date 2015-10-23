<?php
/**
 * @file
 * Collection of translations.
 */

/**
 * Collection of translations.
 */
class TmgmtXConnectResponseTranslations implements Iterator, Countable {
  /**
   * The current position in the collection.
   *
   * @var int
   */
  private $position = 0;

  /**
   * The elements in the array.
   *
   * @var array
   */
  private $array = array();

  /**
   * The archive that holds the translations.
   *
   * @var TmgmtXConnectResponseFile
   */
  private $file;


  /**
   * Construct collection by passing the archive containing the translations.
   *
   * @param TmgmtXConnectResponseFile $file
   *   The Archive object.
   */
  public function __construct(TmgmtXConnectResponseFile $file) {
    $this->file = $file;
    $this->array = array_values($file->getDeliveryInfo()->getFilesInfo());
  }

  /**
   * Rewind the pointer to the first element.
   */
  public function rewind() {
    $this->position = 0;
  }

  /**
   * Get the current file.
   *
   * @return TmgmtXConnectResponseTranslation
   *   The current file.
   */
  public function current() {
    $info = $this->array[$this->position];
    return new TmgmtXConnectResponseTranslation($this->file, $info);
  }

  /**
   * Get the current key.
   *
   * @return int
   *   The current position in the collection.
   */
  public function key() {
    return $this->position;
  }

  /**
   * Go to the next element in the collection.
   */
  public function next() {
    ++$this->position;
  }

  /**
   * Check if there is an element on the current position.
   *
   * @return bool
   *   Is valid TRUE/FALSE.
   */
  public function valid() {
    return isset($this->array[$this->position]);
  }

  /**
   * Count the number of items in the collection.
   *
   * @return int
   *   The number of files in the collection.
   */
  public function count() {
    return count($this->array);
  }

}
