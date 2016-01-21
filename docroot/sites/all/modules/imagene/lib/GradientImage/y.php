<?php


/**
 * Vertical gradient image
 */
class imagene_GradientImage_y {

  protected $dim;
  protected $colors;
  protected $hasAlpha;

  function __construct($height, $colors, $hasAlpha) {
    $this->dim = $height;
    $this->colors = $colors;
    $this->hasAlpha = $hasAlpha;
  }

  function getDimensions() {
    return array(1, $this->dim);
  }

  function hasAlpha() {
    return $this->hasAlpha;
  }

  function getPixelRGBA($x, $y) {
    $B = imagene_bernstein(($y + 0.5) / $this->dim, count($this->colors) - 1);
    $result_rgba = array();
    foreach ($this->colors as $i => $rgba) {
      foreach ($rgba as $channel => $v) {
        $result_rgba[$channel] += $B[$i] * $v;
      }
    }
    return $result_rgba;
  }
}
