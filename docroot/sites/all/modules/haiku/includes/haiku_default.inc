<?php

/**
 * @file
 * Default implementations of haiku and haiku source interfaces..
 */


/**
 * Implements the haiku source interface.
 */
class HaikuSource implements HaikuSourceInterface {

  /**
   * Useful codes representing different types of characters.
   */
  const OTHER = -1;
  const CONSONANT = 0;
  const VOWEL = 1;
  const DIGIT = 2;
  const PUNCTUATION = 3;

  /**
   * The full text of the document.
   */
  protected $text;

  /**
   * The document represented as an array of words.
   */
  protected $text_array;

  /**
   * The number of words in the document.
   */
  protected $length;

  /**
   * A list of haikus generated from this document.
   */
  protected $haikus;

  /**
   * The number of haikus generated from this document.
   */
  protected $num_haikus;

  /**
   * Constructs a HaikuSource object from a string.
   */
  public function __construct($text) {
    $this->text = preg_replace('/\s+/', ' ', strip_tags($text));
    $this->text_array = explode(' ', $this->text);
    $this->length = count($this->text_array);
    $this->parseHaikus();
  }

  /**
   * Parses the HaikuSourceDocument for all haikus.
   *
   * This method works by splitting the source text into an array of words, then
   * iterating through each word and making an array of syllable counts which
   * correspond to the original text array. Then, the syllable array is iterated
   * over with a separate method to check for the 5-7-5 syllable count. If a
   * match is found, it takes slices of the source text array and creates Haiku
   * objects from them.
   *
   * @return
   *   An array of Haiku objects.
   */
  public function parseHaikus() {
    $return_haikus = array();

    // Iterate through all words, adding each word's syllable count to an array.
    $text_syllables = array();
    foreach ($this->text_array as $n => $word) {
      $text_syllables[$n] = $this->countSyllables($word);
    }
    $text_syllables_array = $text_syllables;

    // Try to parse out a haiku using each word as a starting point.
    while (count($text_syllables) > 2) {
      // If this is a haiku, add it to the return array.
      $bounds = $this->isHaiku($text_syllables);
      if (is_array($bounds) && count($bounds) == 6) {
        // Pull each line based on the returned bounds.
        $haiku_array = array(
          // We must prepend lines with numbers to prevent duplicate array keys.
          '1. ' . implode(' ', array_slice($this->text_array, $bounds[0], $bounds[1] - $bounds[0] + 1, TRUE)) =>
            array_slice($text_syllables_array, $bounds[0], $bounds[1] - $bounds[0] + 1, TRUE),
          '2. ' . implode(' ', array_slice($this->text_array, $bounds[2], $bounds[3] - $bounds[2] + 1, TRUE)) =>
            array_slice($text_syllables_array, $bounds[2], $bounds[3] - $bounds[2] + 1, TRUE),
          '3. ' . implode(' ', array_slice($this->text_array, $bounds[4], $bounds[5] - $bounds[4] + 1, TRUE)) =>
            array_slice($text_syllables_array, $bounds[4], $bounds[5] - $bounds[4] + 1, TRUE),
        );

        $return_haikus[] = new Haiku($haiku_array);
        unset($bounds);
        unset($haiku_array);
      }
      // Re-run the check after pulling the first item off the array.
      $text_syllables = array_slice($text_syllables, 1, NULL, TRUE);
    }

    $this->haikus = $return_haikus;
    $this->num_haikus = count($return_haikus);
    return $return_haikus;
  }

  /**
   * Returns an array of haikus from the source.
   *
   * @param int $length
   *   The number of haikus to return. Defaults to all haikus.
   *
   * @return
   *   A sorted array of haikus contained within the source document.
   */
  public function getHaikus($length = -1) {
    $haikus = $this->haikus;
    usort($haikus, array('Haiku', 'compare'));
    if ($length > 0) {
      return array_slice($haikus, 0, $length);
    }
    else {
      return $haikus;
    }
  }

  /**
   * Method to check the syllable array for a haiku beginning at this point.
   *
   * This works by taking an array of syllable counts (corresponding to words)
   * and summing them one-by-one. If it first can sum 5, it then attempts to sum
   * 7. If it's able to sum 7, it attempts to sum 5 again. If the sums are
   * greater than their targets, return FALSE, otherwise, keep summing. It keeps
   * track of the break points so that the returned array represents the bounds
   * of each line of the haiku.
   *
   * @param array $syllable_array
   *   An array of syllable counts corresponding to words.
   *
   * @return
   *   An array if it is a haiku, FALSE if it is not.
   */
  protected function isHaiku($syllable_array) {
    $this_line = 0;
    $return = array(key($syllable_array));
    // Test for a first line match.
    while ($syllables = current($syllable_array)) {
      // We use PHP_INT_MAX to represent non-alphanumeric strings.
      if ($syllables < PHP_INT_MAX) {
        $this_line += $syllables;
      }
      if ($this_line < 5) {
        next($syllable_array);
      }
      elseif ($this_line == 5) {
        $return[] = key($syllable_array);
        next($syllable_array);
        break;
      }
      else {
        return FALSE;
      }
    }

    // Test for a second line match.
    $this_line = 0;
    $return[] = key($syllable_array);
    while ($syllables = current($syllable_array)) {
      if ($syllables < PHP_INT_MAX) {
        $this_line += $syllables;
      }
      if ($this_line < 7) {
        next($syllable_array);
      }
      elseif ($this_line == 7) {
        $return[] = key($syllable_array);
        next($syllable_array);
        break;
      }
      else {
        return FALSE;
      }
    }

    // Test for a third line match.
    $this_line = 0;
    $return[] = key($syllable_array);
    while ($syllables = current($syllable_array)) {
      if ($syllables < PHP_INT_MAX) {
        $this_line += $syllables;
      }
      if ($this_line < 5) {
        next($syllable_array);
      }
      elseif ($this_line == 5) {
        $return[] = key($syllable_array);
        break;
      }
      else {
        return FALSE;
      }
    }

    return $return;
  }


  /**
   * Counts syllables in a word.
   *
   * In this implementation, syllable counting represents the main logic of the
   * algorithm. Natural language processing is never perfect, so this often
   * miscounts. The basic idea is to count consonant-vowel pairs in the word
   * with a slew of exceptions before and after. This assumes English language
   * input.
   *
   * @param string $word
   *   The word whose syllables we're counting.
   *
   * @return
   *   The number of syllables in the word.
   */
  protected function countSyllables($word) {
    // It's common to use & to be read as 'and' in English.
    if ($word == '&') {
      return 1;
    }

    // The way times are read is standardized, check for this.
    if ($is_time = $this->tryCountSyllablesTime($word)) {
      return $is_time;
    }

    // Otherwise, strip out all but the letters and let's begin.
    $word = trim(preg_replace("/[^a-zA-Z]+/", "", $word));

    // Set some default variables.
    $words = str_split($word);
    $length = strlen($word);
    $syllables = 0;

    // If it's a non alpha string, return 0 syllables.
    if ($length == 0) {
      return PHP_INT_MAX;
    }

    // If the word is one or two letters, assume it's one syllable.
    if ($length <= 2) {
      return 1;
    }

    // If 3rd letter vowel and 1st two letters consonant-vowel, increment.
    if ($length > 4 && $this->letterIs($words[2]) == self::VOWEL && $this->letterIs($words[0]) == self::CONSONANT
      && $this->letterIs($words[1]) == self::VOWEL) {
      $syllables++;
    }

    // Search the word for vowel-consonant pairs.
    foreach ($words as $charNum => $char) {
      if (isset($words[$charNum + 1])) {
        if ($this->letterIs($words[$charNum]) == self::VOWEL && $this->letterIs($words[$charNum + 1]) == self::CONSONANT) {
          $syllables++;
        }
      }
    }

    // Decrement a syllable if the word ends in ed but not ted, ded, or eed.
    if (substr($word, -2) == 'ed' && $length>3) {
      if (substr($word, -3, 1) != 't' && substr($word, -3, 1) != 'd' && substr($word, -3, 1) != 'e') {
        $syllables--;
      }
    }

    // Increment a syllable if the word ends in "ism" or "ble"
    if (substr($word, -3) == 'ism' || substr($word, -3) == 'ble') {
      $syllables++;
    }

    // Increment a syllable if the word ends in y.
    if ($words[$length-1] == 'y' && $this->letterIs($words[$length-3]) != self::VOWEL) {
      $syllables++;
    }

    // Catch-all.
    if ($syllables < 1) {
      $syllables = 1;
    }

    return $syllables;
  }

  /**
   * Tries to count syllables as if they were in a time format.
   *
   * This is a hacked-together (but fairly accurate) way to count syllables in
   * common time formats (e.g. 4:55pm). It assumes American formatting and
   * pronunciation (sorry).
   *
   * @param string $time
   *   The string which may represent time (it also may not).
   *
   * @return
   *   FALSE if $time is not a time, otherwise the number of syllables.
   */
  protected function tryCountSyllablesTime($time) {
    $length = strlen($time);
    $syllables = 0;

    if (substr($time, 1, 1) == ':') {
      $hour = substr($time, 0, 1);
      $minute = substr($time, 2, 2);
      if ($length > 4) {
        $suffix = substr($time, 4);
      }
    }
    elseif (substr($time, 2, 1) == ':') {
      $hour = substr($time, 0, 2);
      $minute = substr($time, 3, 2);
      if ($length > 5) {
        $suffix = substr($time, 5);
      }
    }
    else {
      // This is probably not a time.
      return FALSE;
    }

    // We're only going to handle American times. Sorry!
    switch ($hour) {
      case 7:
        $syllables += 2;
        break;

      case 11:
        $syllables += 3;
        break;

      default:
        $syllables++;
    }

    // Silent.
    if ($minute == '00') {
      goto afterhours;
    }

    // The only single-syllable minute marker.
    if ($minute == '10') {
      $syllables++;
      goto afterhours;
    }

    // Otherwise, default to 2 and ++ for 7, ++ again for > 21 or 11
    $syllables += 2;
    if (substr($minute, 1, 1) == '7') {
      $syllables++;
    }
    if ($minute > 21 || $minute == '11') {
      $syllables++;
    }

    // Used for goto (man what a horrible idea).
    afterhours:

    // If there's a suffix, it may be AM or PM.
    if (isset($suffix)) {
      $acceptable_suffixes = array(
        'am',
        'pm',
        'a.m.',
        'p.m.',
      );
      if (in_array(strtolower($suffix), $acceptable_suffixes)) {
        $syllables += 2;
      }
    }

    return $syllables;
  }

  /**
   * Helper method to determine if a character is a vowel, consonant, or
   * otherwise.
   *
   * @param char $character
   *
   * @return
   *   VOWEL, CONSONANT, DIGIT, or PUNCTUATION
   */
  protected function letterIs($character) {
    // Character is numeric.
    if (ctype_digit($character)) {
      return self::DIGIT;
    }

    // Character is punctuation.
    if (ctype_punct($character)) {
      return self::PUNCTUATION;
    }

    // Character is alphabetic.
    if (ctype_alpha($character)) {
      if (strpos('aeiouyAEIOUY', $character) !== FALSE) {
        return self::VOWEL;
      }
      else {
        return self::CONSONANT;
      }
    }

    return self::OTHER;
  }

  /**
   * Implements PHP's magic method __toString().
   */
  public function __toString() {
    return $this->text;
  }
}


/**
 * Implements the haiku interface.
 */
class Haiku implements HaikuInterface {

  /**
   * The first line of the haiku, which should have 5 syllables.
   */
  protected $first_line;
  protected $first_line_count;

  /**
   * The second line of the haiku, which should have 7 syllables.
   */
  protected $second_line;
  protected $second_line_count;

  /**
   * The third line of the haiku, which should have 5 syllables.
   */
  protected $third_line;
  protected $third_line_count;

  /**
   * The score associated with this haiku.
   */
  protected $score;

  /**
   * Constructs a Haiku object by using one of two constructors based on how
   * many arguments are passed to it.
   */
  public function __construct() {
    $argv = func_get_args();
    $argc = func_num_args();

    switch ($argc) {
      case 1:
        call_user_func_array(array($this, '__construct_array'), $argv);
        break;

      case 3:
        call_user_func_array(array($this, '__construct_lines'), $argv);
        break;
    }
    $this->score = $this->score();
  }

  /**
   * Constructs a Haiku object from an array.
   *
   * @param array $lines
   *   An array of length three.
   */
  private function __construct_array($lines) {
    if (is_array($lines) && count($lines) == 3) {
      $n = 0;
      foreach ($lines as $line => $count) {
        if ($n == 0) {
          $this->first_line = substr($line, 3);
          $this->first_line_count = $count;
        }
        elseif ($n == 1) {
          $this->second_line = substr($line, 3);
          $this->second_line_count = $count;
        }
        elseif ($n == 2) {
          $this->third_line = substr($line, 3);
          $this->third_line_count = $count;
        }
        $n++;
      }
    }
  }

  /**
   * Constructs a Haiku object from three separate lines.
   *
   * @param string $line1
   *   The first line of the haiku.
   * @param string $line2
   *   The second line of the haiku.
   * @param string $line3
   *   The third line of the haiku.
   */
  private function __construct_lines($line1, $line2, $line3) {
    $this->first_line = $line1;
    $this->second_line = $line2;
    $this->third_line = $line3;
  }

  /**
   * Looks at the haiku and attempts to assign it a score.
   *
   * @return
   *   An integer representing score. Lower is better.
   */
  protected function score() {
    $score = 0;
    $last_words = explode(' ', $this->third_line);
    $last_word = trim(preg_replace("/[^a-zA-Z]+/", "", $last_words[count($last_words)-1]));
    $first_words = explode(' ', $this->first_line);
    $first_word = trim(preg_replace("/[^a-zA-Z]+/", "", $first_words[0]));

    // Demote haikus that end poorly.
    $bad_ends = array('as', 'our', 'he', 'and', 'but', 'a', 'an', 'the', 'its', 'or', 'they', 'their', 'his', 'her', 'to', 'of', 'who', 'what', 'when', 'where', 'why', 'how', 'whether', 'because', 'those', 'for', 'that', 'on', 'at', 'in', 'said', 'was', 'has', 'is', 'nor', 'so', 'yet', 'which', 'it');
    if (in_array($last_word, $bad_ends)) {
      $score -= 10;
    }

    // Demote haikus that begin poorly.
    if (in_array($first_word, array('of'))) {
      $score -= 5;
    }

    // Demote haikus whose first words end in a hard stop.
    if (in_array($first_words[0][strlen($first_words[0]) - 1], array('!', '.', '?', ';'))) {
      $score -= 5;
    }

    // Lines ending in punctuation are good.
    $punctuation = array('!', ',', ';', '.', '?');
    if (strlen($this->first_line) > 0 && in_array($this->first_line[strlen($this->first_line) - 1], $punctuation)) {
      $score += 5;
    }
    if (strlen($this->second_line) > 0 && in_array($this->second_line[strlen($this->second_line) - 1], $punctuation)) {
      $score += 5;
    }
    if (strlen($this->third_line) > 0 && in_array($this->third_line[strlen($this->third_line) - 1], $punctuation)) {
      $score += 10;
    }

    // A couple of bonus points if it begins with an article.
    $good_beginnings = array('an', 'a', 'the');
    if (in_array(strtolower($first_word), $good_beginnings)) {
      $score += 5;
    }

    // Words with single syllables are almost never wrong, reward one point for each.
    $full_count = array_merge($this->first_line_count, $this->second_line_count, $this->third_line_count);
    foreach ($full_count as $syllables) {
      if ($syllables == 1) {
        $score++;
      }
    }

    // Varied syllable structure is often more interesting. Deduct for too many of the same syllable.
    $first_line_counts = array_count_values($this->first_line_count);
    foreach ($first_line_counts as $count) {
      if ($count == 5) {
        $score -= 10;
      }
    }
    $second_line_counts = array_count_values($this->second_line_count);
    foreach ($second_line_counts as $count) {
      if ($count > 4) {
        $score -= 14;
      }
    }
    $third_line_counts = array_count_values($this->third_line_count);
    foreach ($third_line_counts as $count) {
      if ($count == 5) {
        $score -= 10;
      }
    }

    return $score;
  }

  /**
   * Returns the haiku as an array.
   *
   * @return
   *   An array in which each line corresponds to a line of the haiku.
   */
  public function toArray() {
    return array(
      $this->first_line,
      $this->second_line,
      $this->third_line,
    );
  }

  /**
   * Returns the haiku as a delimited string.
   *
   * @param string $delimiter
   *   String used to separate each line. Defaults to a newline.
   *
   * @return
   *   A string representing the haiku.
   */
  public function toString($delimiter = "\n") {
    $return = $this->first_line . $delimiter;
    $return .= $this->second_line . $delimiter;
    $return .= $this->third_line . $delimiter;
    return $return;
  }

  /**
   * Implements PHP's magic method __toString().
   */
  public function __toString() {
    return $this->toString();
  }

  /**
   * Returns the haiku's score.
   */
  public function getScore() {
    return $this->score;
  }

  /**
   * A PHP callable function to help sort haikus by score.
   *
   * @param Haiku $a
   *   First haiku to compare.
   *
   * @param Haiku $b
   *   Second haiku to compare.
   *
   * @return
   *   -1 if a > b, 1 if b > a, 0 if equal.
   */
  static function compare($a, $b) {
    if (is_object($a) && is_object($b)) {
      $score_a = $a->getScore();
      $score_b = $b->getScore();

      if ($score_a == $score_b) {
        return 0;
      }
      else {
        return ($score_a > $score_b) ? -1 : 1;
      }
    }
  }
}
