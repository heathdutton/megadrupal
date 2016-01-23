<?php

class AutordfTokenizer {
  /**
   * Delimiter value
   */
  protected $delim;
  
  /**
   * Generated tokens for the string
   */
  protected $tokens;
  
 /**
   * Split a text separated by space into array of words
   * 
   * @param string $content
   *   Content separated by spaces that will be splitted into array of words
   * 
   * @return array
   *   Array of words
   */  
  function __construct($string, $delim = ' ') {
    $this->delim = $delim;
    $this->tokens = self::stringTokenizer($string, $delim);
  }
  
  /**
   * @return array
   *   List of generated tokens
   */
  public function getTokens() {
    return $this->tokens;
  }
  
  /**
   * Tokenize a given string.
   * 
   * @param string $string
   *   String to tokenize
   * @param char $delim
   *   Delimiter character
   * @return array
   *   List of generated tokens
   */
  public static function stringTokenizer($string, $delim = ' ') {
    $words = explode($delim, $string);
    $tokens = array();
    
    foreach ($words as $word) {
      $word = strtok($word, '\'');
      // If word ends with . and its Not an abbrevation M.S.
      $endswith = self::endsWith($word);
      if ($endswith == '.' && strpos($word, '.') != strlen($word) -1) {
        array_push($tokens, $word);
      }
      // If word ends with .
      else if ($endswith == '.') {
        array_push($tokens, drupal_substr($word, 0, -1));
        array_push($tokens, '.');  
      }
      // If word ends with comma ,
      else if ($endswith == ',') {
        array_push($tokens, drupal_substr($word, 0, -1));
        array_push($tokens, ',');
      }
      else if ($endswith == ';') {
        array_push($tokens, drupal_substr($word, 0, -1));
        array_push($tokens, ';');
      }
      else if ($endswith == '?') {
        array_push($tokens, drupal_substr($word, 0, -1));
        array_push($tokens, ';');
      }
      else if ($endswith == ':') {
        array_push($tokens, drupal_substr($word, 0, -1));
        array_push($tokens, ':'); 
      }
      else {
        array_push($tokens, $word);        
      }
    }
    return $tokens;
  }
  
  /**
   * Return last parts of string
   */
  public static function endsWith($haystack, $len = 1) {
    return substr($haystack, strlen($haystack) - $len, $len);
  }
}
