<?php

namespace Drupal\nicedpq;

/**
 * Wrapper for a string, with write operations for indented text.
 */
class IndentedText {

  protected $text;
  protected $indent;

  /**
   * @param string &$text
   *   Reference to a string variable to wrap as "indented text".
   * @param string $indent
   *   String to prepend to each line for indentation.
   *   Typically only consisting of spaces.
   */
  function __construct(&$text, $indent = '') {
    $this->text =& $text;
    $this->indent = $indent;
  }

  /**
   * Create a new instance with increased indentation.
   * The internal $text reference will be the same.
   *
   * @return Drupal\nicedpq\IntentedText
   *   A new instance one level deeper.
   */
  function indent() {
    return new IndentedText($this->text, $this->indent . '  ');
  }

  /**
   * Print some text with a preceding linebreak.
   *
   * @param string $str
   *   The text to print.
   */
  function println($str = '') {
    $this->text .= "\n" . $this->indent;
    $this->pr($str);
  }

  /**
   * Print a piece of text.
   * Indentation will be added to all linebreaks.
   *
   * @param string $str
   *   The text to print.
   */
  function pr($str) {
    $this->text .= str_replace("\n", "\n" . $this->indent, $str);
  }

  /**
   * Interpret a string as a list with separators,
   * and print it with added linebreaks.
   *
   * @param string $list
   *   String with separators, e.g "x = 5 AND y = 7 AND z > 2".
   * @param string $separator
   *   Separator string, e.g. "AND" or ",".
   */
  function printList($list, $separator = ',') {
    if (is_string($list)) {
      $list = explode($separator . ' ', $list);
    }
    $this->println(implode("$separator\n", $list));
  }
}
