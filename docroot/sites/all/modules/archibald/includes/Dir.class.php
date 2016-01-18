<?php

/**
 * @file
 * Class that provides a advanced directory handler
 */

/**
 * Provide a class to search after directories or files, the hole object
 * can be used within a foreach loop thanks to implementing an iterator
 *
 * @author Christian Ackermann <c.ackermann@educa.ch>
 */
class ArchibaldDir implements Iterator {

  /**
   * The starting path
   * @var string
   */
  private $path = '';

  /**
   * Wether to search recrusive or not
   * @var boolean
   */
  private $recrusive = TRUE;

  /**
   * An array which holds regular expression to skip specified directories
   * @var array
   */
  private $skipDirs = array();

  /**
   * An array which holds regular expression to skip specified files
   * @var array
   */
  private $skipFiles = array();

  /**
   * An array which holds regular expression to skip a directory
   * which not match the expresssion
   * @var array
   */
  private $inclDirs = array();

  /**
   * An array which holds regular expression to skip a file
   * which not match the expresssion
   * @var array
   */
  private $inclFiles = array();

  /**
   * Determines if we want just directories within output
   * @var boolean
   */
  private $justDirs = FALSE;

  /**
   * Determines if we want just files within output
   * @var boolean
   */
  private $justFiles = FALSE;

  /**
   * Wether we must start the search process or use the previous found array
   * @var boolean
   */
  private $init = TRUE;

  /**
   * An integer which will be used for imeplementing the iterator
   * @var int
   */
  private $key = 0;

  /**
   * The found files
   * @var array
   */
  private $files = array();

  /**
   * This string will be used as the prefix for a directory search
   * @var string
   */
  private $chroot = '';

  /**
   * Scans a directory for files or folders
   *
   * @param string $path
   *   the directory to scan, this is the directory under chroot
   * @param boolean $recrusive
   *   (optional, default = TRUE)
   * @param string $chroot
   *   This is the main directory, the scan goes not below this dir
   *   optional, default = ARCHIBALD_PATH
   */
  public function __construct($path, $recrusive = TRUE, $chroot = ARCHIBALD_PATH) {

    //Replace all possibles to get under the chroot directory (security issue)
    $path            = preg_replace("/\.\.+\//is", "", $path);
    $this->path      = $path;
    $this->recrusive = $recrusive;

    //Pre init some usefull filter for security reasons
    $this->skipRegexp("^\.");
    $this->skipDirsRegexp('(^\.+|\/\.+)');
    $this->chroot = $chroot;
  }

  /**
   * Include file extension
   * Apply a file extension filter, just plaintext string
   *
   * @param string $ext the extension
   */
  public function fileExtension($ext) {
    $this->inclFiles[] = "/.*\." . preg_quote($ext, "/") . "$/iUs";
  }

  /**
   * Include file name
   * Apply a filename filter, just plaintext string
   * this is just the filename without extension, a file extension is needed
   * to match this filter
   *
   * @param string $filename the filename
   */
  public function fileName($filename) {
    $this->inclFiles[] = "/^" . preg_quote($filename, "/") . "\.[a-z]+$/iUs";
  }

  /**
   * Include file
   * Apply a file filter, just plaintext string
   * This is the same as fileName but here the file extension is not need
   *
   * @param string $filename the filename
   */
  public function file($filename) {
    $this->inclFiles[] = "/^" . preg_quote($filename, "/") . "$/iUs";
  }

  /**
   * Include the file expression
   * Apply a file filter pure regexp allowed
   *
   * @param string $regexp
   *   the regular expression
   */
  public function fileRegexp($regexp) {
    $this->inclFiles[] = "/" . $regexp . "/iUs";
  }

  /**
   * Skip file extension
   * Apply a file extension filter, just plaintext string
   *
   * @param string $ext
   *   the extension
   */
  public function skipExtensions($ext) {
    $this->inclFiles[] = "/.*\." . preg_quote($ext, "/") . "$/iUs";
  }

  /**
   * Skip file name
   * Apply a filename filter, just plaintext string
   * this is just the filename without extension, a file extension is needed
   * to match this filter
   *
   * @param string $filename
   *   the filename
   */
  public function skipFilename($filename) {
    $this->skipFiles[] = "/^" . preg_quote($filename, "/") . "\.[a-z]+$/iUs";
  }

  /**
   * Skip file
   * Apply a file  filter, just plaintext string
   * This is the same as skipFilename but here the file extension is not need
   *
   * @param string $filename
   *   the filename
   */
  public function skip($filename) {
    $this->skipFiles[] = "/^" . preg_quote($filename, "/") . "$/iUs";
  }

  /**
   * Skip the file expression
   * Apply a file filter pure regexp allowed
   *
   * @param string $regexp
   *   the regular expression
   */
  public function skipRegexp($regexp) {
    $this->skipFiles[] = "/" . $regexp . "/iUs";
  }

  /**
   * Skip a directory
   * Apply a directoriy filter, just plaintext string
   *
   * @param string $dir
   *   the directory
   */
  public function skipDirs($dir) {
    $this->skipDirs[] = "/^" . preg_quote($dir, "/") . "/iUs";
  }

  /**
   * Skip a directory
   * Apply a directoriy filter pure regexp allowed
   *
   * @param string $regexp
   *   the directory as a regular expression
   */
  public function skipDirsRegexp($regexp) {
    $this->skipDirs[] = "/" . $regexp . "/iUs";
  }

  /**
   * Include a directory
   * Apply a directoriy filter pure regexp allowed
   *
   * @param string $regexp
   *   the directory as a regular expression
   */
  public function dirRegexp($regexp) {
    $this->inclDirs[] = "/" . $regexp . "/iUs";
  }

  /**
   * Set that we want only files within returning output
   *
   * @param boolean $bool
   *  set to TRUE if your want only files within output
   *  optional, default = TRUE
   */
  public function justFiles($bool = TRUE) {
    if ($bool !== TRUE) {
      $bool = FALSE;
    }
    $this->justFiles = $bool;
  }

  /**
   * Set that we want only directories within returning output
   *
   * @param boolean $bool
   *  set to TRUE if your want only directories within output
   *  optional, default = TRUE
   */
  public function justDirs($bool = TRUE) {
    if ($bool !== TRUE) {
      $bool = FALSE;
    }
    $this->justDirs = $bool;
  }

  /**
   * Start searching
   *
   * @return array
   *   the returning result array
   */
  public function search() {
    $this->valid();
    return $this->files;
  }

  /**
   * Check if we have a valid result array, this is also the first function
   * which is called when the iterator starts, so we need to include here our
   * primary search engine
   *
   * @return boolean
   *   TRUE if valid, else FALSE
   */
  public function valid() {
    if ($this->init == TRUE) {
      //Fresh start, we must init the opendir handle
      $this->files = $this->readdir($this->path);
      $this->rewind();
      $this->init = FALSE;
    }
    return ($this->key < count($this->files));
  }

  /**
   * Main search engine
   *
   * @param string $dir
   *   the dir where we want to start search
   *
   * @return array
   *   the result array
   */
  private function readdir($dir) {
    //If we have no directories or the provided one is not a directory
    //return an empty array
    if (empty($dir) || !is_dir($this->chroot . "/" . $dir)) {
      return array();
    }
    $entries = array();

    //Loop through all current entries within the directory
    if ($handle = opendir($this->chroot . "/" . $dir)) {
      while (FALSE !== ($entry = readdir($handle))) {
        //If the directory should not handle by a skip / include directory
        //or the handle is not a ressource, skip this entry
        if (empty($handle) || $this->validateDir($dir . "/" . $entry) == FALSE) {
          continue;
        }

        //Setup the current full path for the entry
        $path = preg_replace(
          "/\/\/+/is", "/", $this->chroot . "/" . $dir . "/" . $entry
        );

        //Check if the validation of the file succeed.
        if ($this->validate($entry, $dir) !== FALSE) {
          //Add the current entry to the return result
          $tmp_class = new ArchibaldDirEntry();
          $tmp_class->directory = $dir;
          $tmp_class->path = $path;
          $tmp_class->last_modified = filemtime($path);
          $tmp_class->last_access = fileatime($path);
          $tmp_class->created = filectime($path);
          $tmp_class->filename = $entry;
          $tmp_class->is_file = is_file($path);
          $tmp_class->is_dir = (($tmp_class->is_file == TRUE) ? FALSE : TRUE);
          $tmp_class->size = filesize($path);
          if (preg_match("/\.([a-z]+)$/iUs", $entry, $match)) {
            $tmp_class->ext = $match[1];
          }
          $entries[] = $tmp_class;
          unset($tmp_class);
        }
        //If we want recursion and the current entry is a directory,
        //call readdir recrusive with the current entry
        if ($this->recrusive && is_dir($path)) {
          $entries = array_merge($entries, $this->readdir(preg_replace("/\/\/+/is", "/", $dir . "/" . $entry)));
        }
      }
      return $entries;
    }
    return $entries;
  }

  /**
   * Validates a directory against the filters
   *
   * @param string $entry
   *   the directory
   *
   * @return boolean
   *   TRUE if directory is valid, else FALSE
   */
  public function validateDir($entry) {

    //Check all skipDirs filter, if one match skip this directory
    foreach ($this->skipDirs as $regexp) {
      if (preg_match($regexp, $entry)) {
        return FALSE;
      }
    }

    //Check all inclFiles filter, if one does not match skip this entry
    foreach ($this->inclDirs as $regexp) {
      if (!preg_match($regexp, $entry)) {
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Validates a file through all file filters
   *
   * @param string $entry
   *   the entry name
   * @param string $dir
   *   the complete path to the entry but without chroot
   *
   * @return boolean
   *   TRUE if valid, else FALSE
   */
  public function validate($entry, $dir) {

    //If the entry is empty return FALSE
    if (empty($entry)) {
      return FALSE;
    }

    //If we do only files and the entry is not a file retur nFALSE
    if ($this->justFiles == TRUE && !is_file($this->chroot . "/" . $dir . "/" . $entry)) {
      return FALSE;
    }

    //If we only want directories and the one provided is not a directory
    //return FALSE
    if ($this->justDirs == TRUE && !is_dir($this->chroot . "/" . $dir . "/" . $entry)) {
      return FALSE;
    }

    //Check all inclFiles filter against the entry,
    //if one does not match skip this file
    foreach ($this->inclFiles as $regexp) {
      if (!preg_match($regexp, $dir . "/" . $entry)) {
        return FALSE;
      }
    }

    //Check all skipFiles filter against the entry,
    //if one does match skip this file
    foreach ($this->skipFiles as $regexp) {
      if (preg_match($regexp, $dir . "/" . $entry)) {
        return FALSE;
      }
    }
    return $entry;
  }

  /**
   * Implements iterator key()
   *
   * @return int
   */
  public function key() {
    return $this->key;
  }

  /**
   * Implements iterator current()
   *
   * @return mixed
   */
  public function current() {
    return $this->files[$this->key];
  }

  /**
   * Implements iterator rewind()
   */
  public function rewind() {
    $this->key = 0;
  }

  /**
   * Implements iterator next()
   */
  public function next() {
    $this->key++;
  }
}

/**
 * This object will be the returning elements
 * which includes all needed information about a file
 */
class ArchibaldDirEntry {

  /**
   * the filename without the extension
   *
   * @var string
   */
  public $filename = "";

  /**
   * the directory without the filename like dirname()
   *
   * @var string
   */
  public $directory = "";

  /**
   * The full path to the file (including filename)
   *
   * @var string
   */
  public $path = "";

  /**
   * If the entry is a file
   *
   * @var boolean
   */
  public $is_file = FALSE;

  /**
   * If the entry is a directory
   *
   * @var boolean
   */
  public $is_dir = FALSE;

  /**
   * The size of the entry in bytes
   *
   * @var int
   */
  public $size = "";

  /**
   * The file extension
   * @var string
   */
  public $ext = "";

  /**
   * The last access time as a timestamp
   *
   * @var int
   */
  public $last_access = 0;

  /**
   * The last modified time as a timestamp
   *
   * @var int
   */
  public $last_modified = 0;

  /**
   * The created time as a timestamp
   *
   * @var int
   */
  public $created = 0;

  /**
   * magic methode to convert class to string
   * This can not be drupal coding standart conform:
   * http://www.php.net/manual/en/language.oop5.magic.php#object.tostring
   */
  public function __toString() {
    return $this->path;
  }
}

