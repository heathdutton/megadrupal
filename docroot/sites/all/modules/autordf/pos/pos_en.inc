<?php

class PosTagger {
  private $dict;

  public function __construct($lexicon) {
    if (!file_exists($lexicon)) {
    	throw new Exception('Lexicon file not found'); 
    }
    $fh = fopen($lexicon, 'r');
    while($line = fgets($fh)) {
      $tags = explode(' ', $line);
      $this->dict[strtolower(array_shift($tags))] = $tags;
    }
    fclose($fh);
  }

  public function tag($tokens) {
    $nouns = array('NN', 'NNS');

    $return = array();
    $i = 0;
    foreach($tokens as $token) {
      // default to a common noun
      $return[$i] = 'NN'; 

      // remove trailing full stops
      if(substr($token, -1) == '.') {
        $token = preg_replace('/\.+$/', '', $token);
      }

      // get from dict if set
      if(isset($this->dict[strtolower($token)])) {
        $return[$i] = $this->dict[strtolower($token)][0];
      }      

      // Converts verbs after 'the' to nouns
      //  rule 1: DT, {VBD | VBP | VB} --> DT, NN
      if($i > 0) {
        if($return[$i - 1] == 'DT' &&
            in_array($return[$i],
              array('VBD', 'VBP', 'VB'))) {
          $return[$i] = 'NN';
        }
      }

      // Convert noun to number if . appears
      // rule 2: Convert Noun to number (CD) if dot(.) appears in the word
      if($return[$i][0] == 'N' && strpos($token, '.') !== false) {
        $return[$i] = 'CD';
      }

      // rule 3: convert a noun to a past participle if ends with "ed"
      if($return[$i][0] == 'N' && substr($token, -2) == 'ed') {
        $return[$i] = 'VBN';
      }

      // rule 4: convert any type to adverb that ends in 'ly'
      if(substr($token, -2) == 'ly') {
        $return[$i] = 'RB';
      }

      // rule 5: convert a common noun (NN or NNS) to a adjective if it ends with "al"
      if(in_array($return[$i], $nouns)
          && substr($token, -2) == 'al') {
        $return[$i] = 'JJ';
      }

      // rule 6: convert a noun to a verb if the preceeding word is "would"
      if($i > 0) {
        if($return[$i] == 'NN'
            && strtolower($tokens[$i-1]) == 'would') {
          $return[$i] = 'VB';
        }
      }

      // rule 7: if a word has been categorized as a common noun and it ends with "s",
      //         then set its type to plural common noun (NNS)
      
      if($return[$i] == 'NN' && substr($token, -1) == 's') {
        $return[$i] = 'NNS';
      }

      // Convert common noun to gerund
      // rule 8: convert a common noun to a present participle verb (i.e., a gerand)
      if(in_array($return[$i], $nouns)
          && substr($token, -3) == 'ing') {
        $return[$i] = 'VBG';
      }

      // If we get noun noun, and the second can be a verb, convert to verb
      if($i > 0) {
        if(in_array($return[$i], $nouns)
            && in_array($return[$i-1], $nouns)
            && isset($this->dict[strtolower($token)])) {
          if(in_array('VBN', $this->dict[strtolower($token)])) {
            $return[$i] = 'VBN';
          } else if(in_array('VBZ',
                $this->dict[strtolower($token)])) {
            $return[$i] = 'VBZ';
          }
        }
      }

      $i++;
    }

    return $return;
  }
}