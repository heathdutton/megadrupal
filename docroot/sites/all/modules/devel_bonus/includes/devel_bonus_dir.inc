<?php

/**
 * @file
 * This file containts class scan directory using PHP Reflection
 */

/**
 * Class Dir Reader
 * This Class create a method getTree, can read struc of dir using build-in PHP
 * object and have a filter
 * @param $path
 *   What directory to scan
 * @param $allow
 *   What type of file to scan, using "," to split. Example: .module,.php,.inc
 */
class DirReader {
  private $_dir = NULL;
  private $_allow = NULL;
  function __construct($path, $allow = '*') {
    $this->_dir = $path;
    $this->_allow = $allow;
  }
  public function getTree() {
    $items = new RecursiveDirectoryIterator($this->_dir);
    foreach (new DirFilter(new RecursiveIteratorIterator($items), $this->_allow) as $file_name => $fileInfo) {
      $return[$file_name] = $file_name;
    }
    return $return;
  }
}

/**
 * Class Dir Filter
 * Filter file and directory by Dir Reader
 */
class DirFilter extends FilterIterator  {
  private $userFilter;
  public function __construct(Iterator $iterator, $filter) {
    parent::__construct($iterator);
    if ($filter != '*') {
      $filter = explode(',', $filter);
      $filters = array();
      foreach ($filter as $f) {
        $f = trim($f);
        $filters[] = '(' . $f . ')';
      }
      $this->userFilter = '/' . implode('|', $filters) . '/';
    }
  }
  public function accept() {
    $dir = $this->getInnerIterator()->current()->getFilename();
    if ($this->userFilter) {
      if (preg_match($this->userFilter, $dir)) {
        return TRUE;
      }
      return FALSE;
    }
    else {
      if ($dir == '.' || $dir == '..') {
        return FALSE;
      }
      return TRUE;
    }
  }
}

class HookParser {
  private $_contents;
  private $_patterns = array(
    'function' => 'function ',
    'define' => 'define('
  );
  public function __construct($file) {
    $this->_contents = file_get_contents($file);
  }
  private function countFunction() {
    return substr_count($this->_contents, $this->_patterns['function']); 
  }
  private function functionList() {
    preg_match_all("/function\s(.*)\s{/", $this->_contents, $matches);
    return isset($matches[1]) ? $matches[1] : NULL;
  }
  
  public function parse() {
    $return = new stdClass();
    $return->totalFunction = $this->countFunction();
    $return->functionLists = $this->functionList(); 
    return $return;
  }
}

