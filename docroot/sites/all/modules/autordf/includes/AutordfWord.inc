<?php

/**
 * Class to store potential tags
 */
class AutordfWord {
    
  /**
   * The word as stored in lowercase
   */
  public $word;
    
  /**
   * Type Classification
   * Is the word all in Capitals or just the first word
   * AUTORDF_NOCAPS   - No caps
   * AUTORDF_FIRSTCAP - First Letter only
   * AUTORDF_ALLCAPS  - All caps
   * AUTORDF_NUMERIC  - Numeric Characters 
   * 
   * Interesting patterns can be checked for a word using this
   * If a word is in no caps and all caps mixed in document, then its not an ACRONYM 
   * 
   * @var array $capitalization
   */
  protected $capitalization;
  
  /**
   * The overall score of the word 
   * 
   * @var float $score
   */
  protected $score = 0.0;
  
  /**
   * Linguistic Features, POS tags Noun Phrase(NP) etc.
   * 
   * @var array $lin
   *   List of occurence of word with their POS tags as array with key as index and value of POS
   */
  protected $lin;

  /**
   * Location of word with respect to document or phrase
   * Whether its in beginning of a noun phrase or in a noun phrase or at end
   *  
   * @var array
   *   List of occurences of word
   */
  protected $loc = array();
  
  /**
   * Document frequency of word
   * 
   * @var int
   */
  protected $df;
  
  /**
   * Frequency of word in querylog
   * 
   * @var int
   */
  protected $querylog; 
  
  /**
   * Is the word is in title?
   * 
   * @var bool
   */
  protected $intitle = FALSE;
  
  /**
   * Is the word an anchor text?
   * 
   * @var bool
   */
  protected $inurl = FALSE;
  
  /**
   * Length of the Candidate phrase
   * Finding key phrases
   * 
   * @var array 
   */
  protected $phlen;
  
  
  function __construct($word) {
    // This should always be in lowercase
    $this->word = $word;
  }
  
  /**
   * Return word score 
   * 
   * @return
   *   Score value 
   */
  public function getScore() {
    return $this->score;
  }
  
  /**
   * @see getScore()
   */
  public function setScore($score) {
    $this->score = $score;
  }
  
  /**
   * Update parameters related to words
   * 
   * @param $word
   *   Word string
   * 
   * @param $index
   *   Position of occurence of word in content
   * 
   * @param $score
   *   Score value
   * 
   * @param @tagstack
   *   List of open tags associated with that word
   */
  public function update($word, $index, $score = 1, $tagstack = array()) {
    $this->loc[$index] = $word;
    self::wordCaps($word, $index);
    $this->score += $score;
    
    if (in_array('title', $tagstack)) {
      $this->intitle = TRUE;
    }
    if (in_array('a', $tagstack)) {
      $this->inurl = TRUE;
    }
  }
  
  /**
   * Mark word as Firstcaps, Allcaps and Nocaps
   * 
   * @param $word
   *   Word string
   */
  protected function wordCaps($word, $index) {
    $allcap_regex = '/^[A-Z0-9]+$/u';
    $firstcap_regex = '/^[A-Z][a-z0-9]+$/u';
    
    if (is_numeric($word)) {
      $this->capitalization[$index] = AUTORDF_NUMERIC;
    }
    else if (preg_match($allcap_regex, $word)) {
      $this->capitalization[$index] = AUTORDF_ALLCAP;
    }
    else if (preg_match($firstcap_regex, $word)) {
      $this->capitalization[$index] = AUTORDF_FIRSTCAP;
    }
    else {
      $this->capitalization[$index] = AUTORDF_NOCAP;
    }   
  }
  
  public function getCapitalization($index = NULL) {
    if (isset($index)) {
      return $this->capitalization[$index];
    }
    return $this->capitalization;
  }
}
