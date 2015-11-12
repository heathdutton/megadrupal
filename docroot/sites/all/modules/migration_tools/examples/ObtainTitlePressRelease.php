<?php
/**
 * @file
 * ObtainTitlePressRelease.
 */

class ObtainTitlePressRelease extends ObtainTitle {

  /**
   * {@inheritdoc}
   */
  public function validateString($text) {
    // If the text it grabbed was 'News And Press Releases' then try again.
    if (strcasecmp(trim($text), "News And Press Releases") == 0) {
      return FALSE;
    }
    // If the text it grabbed was 'FOR IMMEDIATE RELEASE' then try again.
    if (stristr($text, "for immediate release")) {
      return FALSE;
    }
    // Make sure we didn't grab a date.
    $possible_date = strtotime($text);
    if ($possible_date) {
      return FALSE;
    }

    // Made it this far.  Send it to the parent for further validations.
    return parent::validateString($text);
  }
}
