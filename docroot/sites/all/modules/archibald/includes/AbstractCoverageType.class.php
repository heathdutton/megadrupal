<?php

/**
 * @file
 * abstract class with basic features used for all kinds of
 * coverage sync`s
 *
 */

abstract class ArchibaldAbstractCoverageType {
  const ERROR_CHARSET_NOT_FOUND = 1;
  const ERROR_CHARSET_VARIABLE_NOT_SET = 2;
  const ERROR_SOURCE_NOT_FOUND = 3;

  /**
   * Charset type (iso5426|utf-8|...)
   * @var string
   */
  protected $charsetType = '';

  /**
   * A replacement array which will be convert the charset to utf-8
   * @var array
   */
  protected $charset = array();

  /**
   * The original file content
   * @var string
   */
  protected $sourceContent = "";

  /**
   * The last error number (use one of ArchibaldAbstractCoverageType::ERROR_*)
   * @var integer
   */
  public $error = 0;

  /**
   * Setup the charset to convert the characters
   *
   * @param string $charset_type
   *   the charset code (iso5426|utf-8|...)
   *
   * @return boolean
   *   TRUE on success
   *   else FALSE
   */
  protected function getCharset($charset_type) {
    $file = dirname(__FILE__) . "/coverage_sync/charsets/" . $charset_type . ".inc";

    // Check if file exists
    if (!is_file($file)) {
      $this->error = self::ERROR_SOURCE_NOT_FOUND;
      return FALSE;
    }

    //Include file, charset file must be a php file
    //with an array $charset('search'=>'replace')
    include($file);
    if (!isset($charset)) {
      //Check if variable $charset was set
      $this->error = self::ERROR_CHARSET_VARIABLE_NOT_SET;
      return FALSE;
    }

    $this->charsetType = $charset_type;
    $this->charset = $charset;
    return TRUE;
  }

  /**
   * Load the keyword source content file
   *
   * @param string $filename
   *   the absolute filename
   *
   * @return boolean
   *   TRUE on success
   *   else FALSE
   */
  protected function loadFile($filename) {
    // Check if file exists
    if (!is_file($filename)) {
      $this->error = self::ERROR_SOURCE_NOT_FOUND;
      return FALSE;
    }
    //Load the content into sourceContent
    $this->sourceContent = file_get_contents($filename);

    // Convert custom type
    $this->convert($this->sourceContent);

    // Loop through charset
    foreach ($this->charset as $k => $v) {
      //Replace all chars within charset table to html entities values
      $this->sourceContent = preg_replace("/" . $k . "/", $v, $this->sourceContent);
    }

    //Converting from generated html_entities back to utf-8
    $this->sourceContent = trim(mb_convert_encoding($this->sourceContent, 'UTF-8', 'HTML-ENTITIES'));

    //Return wether sourceContent is empty or not
    return !empty($this->sourceContent);
  }

  /**
   * Returns an array with unique keywords
   * array('md5 from keyword' => 'keyword')
   *
   * @param string $filename
   *   The filename for the source
   * @param string $charset
   *   The charset to be used
   *
   * @return array
   */
  public function buildKeywords($filename, $charset = 'utf-8') {

    if (!$this->getCharset($charset)) {
      return FALSE;
    }
    if (!$this->loadFile($filename)) {
      return FALSE;
    }

    $keywords = array();

    //Get all records
    $records = $this->getRecords();

    $record_count = count($records);
    echo "Got " . $record_count . " records\n";

    //Loop through all records
    $output_time = microtime(TRUE);
    foreach ($records as $k => $record) {

      //Adding keywords
      $returned_keywords = $this->getFields($record);
      if (is_array($returned_keywords)) {
        $keywords = array_merge($keywords, $returned_keywords);
      }

      if ((microtime(TRUE) - $output_time) >= 1) {
        $output_time = microtime(TRUE);
        echo "Get keywords for record: " . $k . " to " . $record_count . " - done\n";
      }
    }

    echo "Cleaning double entries\n";
    $tmp = array();
    foreach ($keywords as $v) {
      $tmp[md5($v)] = $v;
    }
    $keywords = $tmp;
    unset($tmp);

    //Return
    return $keywords;
  }

  abstract public function getLanguage();

  abstract protected function getRecords();

  abstract protected function getFields($source);

  abstract protected function convert(&$source);
}

