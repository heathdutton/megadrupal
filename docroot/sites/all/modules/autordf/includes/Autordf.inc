<?php

class Autordf {
    
  /**
   * Minimum token length to be taken into consideration
   * 
   * @var int $min_token_length 
   */
  protected $min_token_length;
  
  /**
   * A list of words to ignore
   * 
   * @var array $noisewords
   */
  protected $noisewords = array();
  
  /**
   * Bundle to operate on e.g 'Article'
   * 
   * @var string $bundle
   */
  protected $bundle;
  
  /**
   * Threshold value for filtering tags
   * 
   * @var float $threshold
   */
  protected $threshold;
  
  /**
   * Count of total words in the document
   * 
   * @var int $totalwords 
   */
  protected $totalwords;
  
  /**
   * List with index as Vocabulary name and value as tags for that Vocabulary 
   * 
   * @var array $vocab
   */
  protected $vocab = array();
  
  /**
   * List of all the words in the document stored as indexed array
   * 
   * @var array $tokens 
   */
  public $tokens = array();
  
  /**
   * Part of speech tags for the list of words in the document
   * 
   * @var array $pos
   */
  public $pos;
  
  /**
   * List of all the words after filtering noise words
   * Each word index is an AutordfWord object
   * 
   * @var array $tags
   */
  protected $tags = array();
  
  /**
   * Document content
   *   
   * @var string $text
   */
  protected $text;
  
  /**
   * Document Language
   * 
   * @var string $lang_id
   */
  protected $lang_id;
  
  function __construct($text, $bundle = NULL, $language = LANGUAGE_NONE) {
    $this->text = $text;
    $this->bundle = $bundle;
    
    // @TODO: If not set $language identify it using stopwords 
    $this->lang_id = $language;
    // Get Minimum token length
    $this->min_token_length = variable_get('autordf_minlength', 3);
    
    // Generate a list of all the noise words from the list generated using cron,
    // from user input and from imported list    
    $this->noisewords = autordf_ignorelist_all();
    
    // Percent threshold for tags in the content
    $threshold = variable_get('autordf_tags_threshold', 1);
    $this->threshold =  $threshold/100;
  }
  
  /**
   * List Tags in content associated with different Vocabularies 
   * 
   * @param string $vname
   *   Vocabulary Name
   * 
   * @return array
   *   If $vname is set then return tags related to that vocabularies, else will return all vocabularies tags
   */
  public function getVocabularies($vname = NULL) {
    if(isset($vname)) {
      return $this->vocab[$vname];
    }
    return $this->vocab;  
  }
  
  /**
   * Set the list of Vocabularies
   * 
   * @see getVocabularies()
   * 
   * @param array $vocab
   *   Vocabulary List
   */
  public function setVocabularies($vocab, $vname = NULL) {
    if (isset($vname)) {
      $this->vocab[$vname] = $vocab;
    }
    else {
     $this->vocab = array_merge_recursive($this->vocab, $vocab);
    }
  }
  
  /**
   * List of tags
   * 
   * @return array
   *   Return a list of all the tags
   */
  public function getTags() {
    return $this->tags;
  }
  
  /**
   * Save the list of tags
   * 
   * @see getTags()
   * 
   * @param array $tags
   *   Save list of tags
   */
  public function setTags($tags) {
    $this->tags = $tags;
  }
  
  /**
   * Check whether a keyword with that name doesn't already exist in other vocabularies
   * 
   * @param string $keyword
   *   Keyword to check
   * 
   * @return bool
   *   TRUE if keyword already exist, else return FALSE  
   */
  public function keywordExists($keyword) {
    $vocab = $this->vocab;
    if ($vocab) {
      foreach($vocab as $key => $values) {
        if ($values && in_array($keyword, array_keys($values))) {
          return TRUE;
        }
      }
    }
    return FALSE;
  }  
  
  /**
   * Check whether a word is a noise word
   * 
   * @param $word
   *   The word to check for
   * 
   * @return bool
   *   TRUE if its a noise word, else FALSE
   */
  public function isNoiseWord($word) {

    $word = drupal_strtolower($word);
    return in_array($word, $this->noisewords)? TRUE: FALSE;
  }
  
  
  /**
   * Helper function to format the string for easy parsing
   * 
   * @param string $content
   *   Passed in content to format
   * 
   * @return string
   *   Formatted content
   */
  public static function formatContent($content) {
    if ($content) {
      // Decode entities to UTF-8
      $content = decode_entities($content);
      // Chinese, Japanese, Korean handling
      if (variable_get('overlap_cjk', TRUE)) {
        $content = preg_replace_callback('/[' . PREG_CLASS_CJK . ']+/u', 'search_expand_cjk', $content);
      }

      // Replace new lines with spaces
      $content = preg_replace("/[\r\n\t]/"," ",$content);
      
      // remove excess space
      $content = preg_replace("/\s{2,}/"," ",$content);      
    }
    return $content;
  }

 
  /**
   * @todo: Make this function more clean
   * 
   * Function takes full content as input and splits it into array pieces 
   * with score based on tag associated with it
   * 
   * @param string $text
   *   The node content
   * 
   * @return string
   *   The text split
   *  
   */  
  function parseContent($text) {

    // Multipliers for scores of words inside certain HTML tags
    $tags = variable_get('autordf_tag_weights', array(
      'title'  => 1.0,
      'h1'     => 0.8,
      'a'      => 0.8,
      'strong' => 0.6,
      'b'      => 0.6,
      'h2'     => 0.6,
      'h3'     => 0.5,
      'h4'     => 0.5,
      'h5'     => 0.4,
      'h6'     => 0.4,
      'i'      => 0.3,
      'em'     => 0.3,
      'u'      => 0.2,
    ));
  
    // Strip off all ignored tags to speed up processing, but insert space before/after
    // them to keep word boundaries.
    $text = str_replace(array('<', '>'), array(' <', '> '), $text);
    $text = strip_tags($text, '<' . implode('><', array_keys($tags)) . '>');
  
    //  Split HTML tags from plain text.
    $split = preg_split('/\s*<([^>]+?)>\s*/', $text, -1, PREG_SPLIT_DELIM_CAPTURE);

    $tag = FALSE; // Odd/even counter. Tag or no tag.
    $score = 1; // Starting score per word
    $tagstack = array(); // Stack with open tags
    $text_split = '';
    foreach ($split as $value) {
      if ($tag) {
        // Increase or decrease score per word based on tag
        list($tagname) = explode(' ', $value, 2);
        $tagname = drupal_strtolower($tagname);
        // Closing or opening tag?
        if ($tagname[0] == '/') {
          $tagname = substr($tagname, 1);
          // If we encounter unexpected tags, reset score to avoid incorrect boosting.
          if (!count($tagstack) || $tagstack[0] != $tagname) {
            $tagstack = array();
            $score = 1;
          }
          else {
            // Remove from tag stack and decrement score
            $score = max(1, $score - $tags[array_shift($tagstack)]);
          }
        }
        else {
          if (isset($tagstack[0]) && $tagstack[0] == $tagname) {
            // None of the tags we look for make sense when nested identically.
            // If they are, it's probably broken HTML.
            $tagstack = array();
            $score = 1;
          }
          else {
            // Add to open tag stack and increment score
            array_unshift($tagstack, $tagname);
            $score += $tags[$tagname];
          }
        }
      }
      else {
        // Note: use of PREG_SPLIT_DELIM_CAPTURE above will introduce empty values
        if ($value != '') {
          // Format String and generate tokens
          $value = self::formatContent($value);
          //$value = $this->_cleanString($value);
          $position = self::generateTokens($value, $score, $tagstack);
          
          $text_split .= " " . $value;
        }
      }
      $tag = !$tag;
    }
    
    $path = drupal_get_path('module', 'autordf');
    try {
      // Part of Speech Tagger
      
      // @todo: Detect Language if not set
      // $lang_id = self::detectLanguage();
      
      $lang_id = 'en';
      require_once($path . '/pos/pos_' . $lang_id . '.inc');
      $pos = new PosTagger("$path/data/lexicon_$lang_id.lst");
      
      // Part of speech tag for each token
      $this->pos = $pos->tag($this->tokens);
    }
    catch (Exception $e) {
      // Exception file Not found
      if (user_access('administer autordf')) {
        drupal_set_message(t('"Lexicon" file not found, Refer <a href="!link"> README.txt</a>', array('!link' => "$path/README.txt")), 'error');
      }
    }
    
    // Set counter for total count of words
    $this->totalwords = $position;

    // Plain text we could have used strip_tags()
    // But anyway we get the same result
    return $text_split;
  }

  /**
   * Parse a given content separated by space into tokens. Generate tokens from 
   * string and save tags  
   * 
   * @param string $content
   * 
   * @param float $score
   *   Word score  associated with content
   * 
   * @param array $tagstack
   *   array of tags associated with content
   * 
   * @return int
   *   Counter for total number of tokens 
   */
  protected function generateTokens($content, $score = 1, $tagstack = array()) {
    static $position = 0;
    
    $focus = 1; 
    
    // Get a list of available tags
    $tags = $this->getTags();
    
    // Generate tokens for content separated by space
    $tokenizer = new AutordfTokenizer($content);
    // Return all tokens
    $words = $tokenizer->getTokens();
    foreach ($words as $word) {
      $this->tokens[$position] = $word;
      
      // Convert token to lowercase
      $lword = drupal_strtolower($word);
  
      $num = is_numeric($word);
      // Filter potential candidates for tags using minimum token length, and 
      // non numeric strings, and noise words
      if (!$num && drupal_strlen($lword) >= $this->min_token_length && !$this->isNoiseWord($word)) {
        // If tag is not set generate an AutordfWord Object 
        if (!isset($tags[$lword])) {
          $tags[$lword] = new AutordfWord($lword);
        }
        // Update score  
        $tags[$lword]->update($word, $position, $score * $focus, $tagstack);
        
        //  Focus is a decaying value in terms of the amount of unique words up to this point.
        //  From 100 words and more, it decays, to e.g. 0.5 at 500 words and 0.3 at 1000 words.
        $focus = min(1, .01 + 3.5 / (2 + $position * .015));
      }
      
      // Increment total no of tokens
      $position++;
    }
    
    // Save all tags
    $this->setTags($tags);
    
    // Return total no of tokens
    return $position;
  }
  
  /**
   * Learn from already existing taxonomy tags
   * 
   * @param array $phrases
   *   List of phrases
   * 
   * @return array
   *   Possible vocabulary name
   */
  public function Training($phrases) {
    $r = array();
    
    // Fetch list of Available Vocabularies
    $vocabulary = variable_get('autordf_vocabulary_names', FALSE);
    $entity_vids = array_flip($vocabulary);
    
    foreach (array_keys($phrases) as $phrase) {
      $query = db_select("autordf_term", 'a');
      $query->fields('a', array('vid'));
      $query->condition('name', $phrase);
      $vids = $query->execute()->fetchCol();
      
      // We will recieve an array of Vids. We will choose the vocabulary that was
      // tagged most times with this phrase
      $v = array_count_values($vids);
      $max = 0;
      $vname_max = NULL;
      foreach ($v as $vid => $count) {
        if ($count > $max) {
          $max = $count;
          $vname_max = $entity_vids[$vid];
        }
      }
      
      if (!$vname_max) {
        $unclassified[] = $phrase;
      }
      else {
        $r[$vname_max][] = $phrase; 
      }      
    }
    
    // If tag is not found, search in known list of tags
    $entities = AutordfTraining::TrainLocalTagger();
    foreach ($unclassified as $phrase) {
            
      if (isset($entities[$phrase])) {
        $vname = $entities[$phrase];
        $r[$vname][] = $phrase;
      }
      else {
        if (strrpos($phrase, ' ')) {
          $r['Person'][] = $phrase; 
        }
      }
    }
    return $r;
  } 

  /**
   * clean a string from the diacritics
   * 
   * @param  string string with accents
   * 
   * @return string clean string
   */
  public function cleanString($string) {
    $diac =
    /* A */   chr(192).chr(193).chr(194).chr(195).chr(196).chr(197).
    /* a */   chr(224).chr(225).chr(226).chr(227).chr(228).chr(229).
    /* O */   chr(210).chr(211).chr(212).chr(213).chr(214).chr(216).
    /* o */   chr(242).chr(243).chr(244).chr(245).chr(246).chr(248).
    /* E */   chr(200).chr(201).chr(202).chr(203).
    /* e */   chr(232).chr(233).chr(234).chr(235).
    /* Cc */  chr(199).chr(231).
    /* I */   chr(204).chr(205).chr(206).chr(207).
    /* i */   chr(236).chr(237).chr(238).chr(239).
    /* U */   chr(217).chr(218).chr(219).chr(220).
    /* u */   chr(249).chr(250).chr(251).chr(252).
    /* yNn */ chr(255).chr(209).chr(241);
      return strtolower(strtr($string, $diac, 'AAAAAAaaaaaaOOOOOOooooooEEEEeeeeCcIIIIiiiiUUUUuuuuyNn'));
  }
  
  
  function detectLanguage() {
    // Incomplete function, Use n-gram model n = 3 or 4 and stopwords for detection
    // If language is und then detect else return node language
    // return 'en' for testing purpose 
    return $this->lanugage == LANGUAGE_NONE ? 'en': $this->language;
  }
  
}
