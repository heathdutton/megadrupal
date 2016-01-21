<?php

/**
 * This class can be used to categorize tags into different vocabularies 
 * and find most relevant tags 
 */
class AutordfTag extends Autordf {
    
  // PHP5 automatically calls constructors of base class
  
  /**
   * Helper function to Categorize a content into different vocabularies
   */
  public function categorize($ajax = FALSE) {
    $value = $this->parseContent($this->text);
    if ($ajax == FALSE) {
      self::matchCountries($value);
      self::matchAcronyms($value);
      self::matchEmails($value);
      self::matchPhrase();
    }
    self::filterTags();
  }
  
  /**
   * filter tags based on threshold value 
   */
  protected function filterTags() {
    $totalwords = $this->totalwords;
    $tags = array();

    // Set a threshold score for tags
    $threshold = max(2.5, $this->threshold * $totalwords);
    
    // Get list of all possible tags
    $all_tags = $this->getTags();
    foreach($all_tags as $key => $token) {

      // Current Tag Score 
      $score = $token->getScore();
      
      // Add score based on capitalization
      $capitals = array_count_values($token->getCapitalization());

      // Boost score based on occurence of capitalization of word
      if (isset($capitals[AUTORDF_FIRSTCAP])) {
        $score += $capitals[AUTORDF_FIRSTCAP] * 1.2;
      }
      if (isset($capitals[AUTORDF_ALLCAP])) {
        $score += $capitals[AUTORDF_ALLCAP] * 1.0;
      }
      
      // Set the Final update score for tag
      $token->setScore($score);

      // If score is greater than minimum threshold then its a tag
      if($score > $threshold) {
        $tags[$key] = $score; 
      }
    }

    // Save the list of tags
    parent::setVocabularies($tags, 'tags');
  }
  
  /**
   * Named Entity Recognition (NER) seeks to locate and classify atomic elements 
   * in text into predefined categories such as the names of persons, 
   * organizations, locations, expressions of times, quantities, monetary values, 
   * percentages, etc.
   * 
   * Although we can use an existing API for better classification of NER
   * But i plan to make a simple version right nowand later when Services for 
   * Drupal7 will be stable we can set up a service to identify NER
   * 
   * We will use a huge list of lexicons which can Recognize named entities
   * But they are computational expensive and require a lot of processing.
   */
  function matchPhrase() {
    $p = $this->pos;
    $tokens = $this->tokens;
    $i = 0;
    $phrases = array();
    $prev_caps = NULL;
    
    foreach ($p as $index => $pos) {
      
      $word = $tokens[$index];
      $num = is_numeric($word);

      if ($num || $this->isNoiseWord($word) || $this->keywordExists($word)) {
        $prev_caps = NULL;
        continue;
      }
      else if (drupal_strlen($word) >= $this->min_token_length || 
              ($word == ',' && $prev_caps == AUTORDF_FIRSTCAP)) {
        // For Names Such as O Hara, Rania Al Abdulla
        if ($pos[0] == 'N' && preg_match('/^[A-Z][a-z0-9]*$/u', $word)) {
          if ($prev_caps == AUTORDF_FIRSTCAP) {
            $phrases[$i - 1] .= ' '. $word;
          }
          else {
            $phrases[$i++] = $word;
            $prev_caps = AUTORDF_FIRSTCAP;
          }
        }
        else if ($word == ',' && $prev_caps == AUTORDF_FIRSTCAP) {
          $phrases[$i - 1] .= ',';  
        }
        else {
          $prev_caps = NULL;
        }
      }
    }
    
    $v = array();
    // Make phrases
    foreach($phrases as $phrase) {
      
      if (AutordfTokenizer::endsWith($phrase) == ',') {
        $phrase = substr($phrase, 0, -1);
      }
      $v[$phrase] = isset($v[$phrase])? $v[$phrase]++: 1;
    }
    
    // Check into existing Taxonomy tree for Entity Recognition
    $r = parent::Training($v);
    
    // Save all tags
    foreach ($r as $vname => $tags) {
      parent::setVocabularies(array_flip($tags), $vname); 
    }
  }
  
  
     
  /**
   * Checks whether a word is a roman numeral
   *
   * @param string $word
   * 
   * @return boolean
   */
  protected function isRomanNumeral($word) {
    if(preg_match("/^M{0,4}(CM|CD|D?C{0,3})(XC|XL|L?X{0,3})(IX|IV|V?I{0,3})$/",$word)) {
      return true;
    } 
    else {
      return false;
    }
  }
  
  /**
   * Match from the list of countries
   * 
   * @params $content
   *   The content to match against
   * 
   * @params $vocab
   *   A passed by reference variable to list possible matches
   */
  public function matchCountries($content) {

    // Fetch list of all countries
    $path = drupal_get_path('module', 'autordf').'/data/country.lst';
    $list_countries = AutordfTraining::getEntities($path, 'Country');
    
    $tags_countries = array();
    preg_match_all("/\s($list_countries)[\s.,?]/i",$content,$matches, PREG_SET_ORDER);
    if($matches){
      foreach($matches as $match) {
        $pat = $match[1];
        if (!$this->keywordExists($pat)) {
          $tags_countries[$pat] = isset($tags_countries[$pat])? $tags_countries[$pat]++: 1;
        }
      }
    }
    parent::setVocabularies($tags_countries, 'Country');
  }
  
  /**
   * Matches content for email addresses
   * 
   * @params $content
   *   The content to match email addresses
   * 
   * @params $vocab
   *   A passed by reference variable to list possible matches
   */
  public function matchEmails($content) {
    
    $regexp = "/([_a-z0-9-]+)(\.[_a-z0-9-]+)*@([\w\d-]+)(\.[\w\d-9-]+)*(\.[A-Za-z]{2,4})/";
    
    $emails = array();
    preg_match_all($regexp, $content, $matches, PREG_SET_ORDER);
    
    if($matches) {
      foreach($matches as $match){
        $pat = $match[0];
        $emails[$pat] = isset($emails[$pat])? $emails[$pat]++: 1;
      }
    }
    parent::setVocabularies($emails, 'EmailAddress');
  }
  
  /**
   * Searches the passed in content looking for Acronyms to add to the search tags array
   * The Acronym word is always in capitals throughout the content if a word is in ALLCAPS 
   * and in lowercaps in rest of content, its not a potential word for acronym
   * 
   * @TODO: Filter other words used that are probably used in ALLCAPS to emphasis or for Quotation  
   * 
   * @param $content
   *  Passed in content to match the Acronyms
   * 
   * @param $vocab
   *  A passed by reference variable to list possible matches
   */
  public function matchAcronyms($content) {
    $searchtags = array();
    // easiest way to look for keywords without some sort of list is to look for Acronyms like CIA, AIG, JAVA etc.
    // so use a regex to match all words that are pure capitals 2 chars or more to skip over I A etc
    preg_match_all("/\b([A-Z]{3,}[0-9]*)\b/u",$content,$matches,PREG_SET_ORDER);
  
    if($matches) {
    
      foreach($matches as $match) {    
        $pat = $match[1];
        
      // ignore noise words who someone has capitalised as well as roman numerals which may be part of something else e.g World War II
        if(!$this->IsRomanNumeral($pat) && !$this->isNoiseWord($pat) && !$this->keywordExists($pat)) {
          // add in the format key=value to make removing items easy and quick plus we don't waste overhead running
          // array_unique to remove duplicates!         
          $searchtags[$pat] = isset($searchtags[$pat])?$searchtags[$pat]++: 1;
        }
      }
    }
    
    parent::setVocabularies($searchtags, 'Acronyms');
  }
  
  /**
   * List possible matches for Anniversary 
   */
  protected function matchAnniversary($content, &$vocab) {
    $anniversary = &$vocab['Anniversary'];
    $content = preg_replace("/$stopwords/", '',$content);
    $content = preg_replace("/\s[A-Za-z]{1,3}\s/", ' ',$content);
    print $content;
    $id = "[A-Z]+[a-z]*\s";
    preg_match_all("/(?:$id)*[0-9]{1,3}\s?(?:st|nd|rd|th)\s(?:$id)*/", $content, $matches);
    print print_r($matches,1);

  }

  /**
   * @TODO: Plan to Implement Sentiment Analysis using predefined lexicons
   * @see ace.cs.ohio.edu/~razvan/papers/icdm2003.pdf
   */
  public function getSentiments() {
    $tokens = $this->tokens;
    $pos = $this->pos;
    foreach ($tokens as $index => $token) {
      // JJ NN adjective noun pairs
      if ($pos[$index] == 'JJ' && $pos[$index + 1] == 'NN') {
        
      }
    }
  } 
    
}