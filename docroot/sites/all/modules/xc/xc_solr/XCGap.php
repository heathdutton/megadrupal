<?php
/**
 * @file
 * The date facet's gap handler class. It can parse and modify date gaps (or intervals).
 *
 * @author Király Péter
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * XCGap class
 */
class XCGap {
  public $text;
  public $quantity;
  public $measure;
  public $date_part_length;
  public $minimum_measure = 'YEAR';

  /**
   * Constructor
   * @param $gap_str (String)
   *   A date gap string like +10YEARS
   */
  function __construct($gap_str) {
    return $this->parse($gap_str);
  }

  /**
   * Parses a gap string
   * @param $gap_str (String)
   *   A date gap string like +10YEARS
   */
  function parse($gap_str) {
    if (is_null($gap_str)) {
      return FALSE;
    }
    if (is_array($gap_str)) {
      do {
        $gap_str = $gap_str[0];
      }
      while (is_array($gap_str));
    }
    elseif (!is_string($gap_str)) {
      xc_util_dsm(gettype($gap_str) . ' ' . $gap_str . ' ' . var_export($gap_str, TRUE), 'gap_str');
      return FALSE;
    }
    if (preg_match('/^\+?(\d+)\s?(DAY|WEEK|MONTH|YEAR)S?$/i', $gap_str, $matches)) {
      $this->text = $gap_str;
      $this->quantity = $matches[1];
      $this->measure  = $matches[2];
      $this->date_part_length = 0;
      switch (strtoupper($this->measure)) {
        case 'WEEK':
        case 'DAY':
          $this->date_part_length = 10; break;
        case 'MONTH':
          $this->date_part_length = 7; break;
        case 'YEAR':
          $this->date_part_length = 4; break;
      }
    }
    else {
      return FALSE;
    }
  }

  function scale_down() {
    if ($this->quantity == 100 || $this->quantity == 10) {
      $this->quantity /= 10;
    }
    elseif ($this->quantity == 1) {
      if ($this->measure = 'YEAR') {
        if ($this->measure != $this->minimum_measure) {
          $this->measure = 'MONTH';
          $this->date_part_length = 7;
        }
      }
      elseif ($this->measure = 'MONTH') {
        if ($this->measure != $this->minimum_measure) {
          $this->measure = 'DAY';
          $this->date_part_length = 10;
        }
      }
    }
    $this->set_text();
  }

  /**
   * Refresh text value acording to the actual fields.
   */
  function set_text() {
    $this->text = '+' . $this->quantity . $this->measure;
  }
}
