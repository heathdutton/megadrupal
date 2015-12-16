<?php

/**
 * @file
 * Basic haiku interface definitions.
 */


/**
 * Interface for a haiku source.
 *
 * This is where all of the haiku parsing logic should reside. Additional
 * private/protected methods will almost certainly be required.
 */
interface HaikuSourceInterface {

  /**
   * Parses the source text for all haikus.
   *
   * @return
   *   A sorted array of haikus contained within the source document.
   */
  public function parseHaikus();

  /**
   * Returns an array of haikus from the source.
   *
   * @param int $length
   *   The number of haikus to return. Defaults to all haikus.
   *
   * @return
   *   A sorted array of haikus contained within the source document.
   */
  public function getHaikus($length = -1);
}


/**
 * Interface for a haiku.
 *
 * While not required for the above to work, this is useful for sorting haikus.
 * Implementing a private method for scoring haikus allows easy sorting, which
 * makes returning the "best" haiku very simple.
 */
interface HaikuInterface {

  /**
   * Returns the haiku's score.
   *
   * @return
   *   An integer representing the haiku's score, used for sort ordering.
   */
  public function getScore();

  /**
   * A PHP callable method to help sort haikus. This should be based on the
   * haiku's score.
   *
   * @param HaikuInterface $a
   *   First haiku to compare.
   *
   * @param HaikuInterface $b
   *   Second haiku to compare.
   *
   * @return
   *   -1 if a > b, 1 if b > a, 0 if equal.
   */
  public static function compare($a, $b);

  /**
   * Returns the haiku as an array.
   *
   * @return
   *   An array in which each line corresponds to a line of the haiku.
   */
  public function toArray();

  /**
   * Returns the haiku as a delimited string.
   *
   * @param string $delimiter
   *   String used to separate each line. Defaults to a newline.
   *
   * @return
   *   A string representing the haiku.
   */
  public function toString($delimiter = "\n");
}
