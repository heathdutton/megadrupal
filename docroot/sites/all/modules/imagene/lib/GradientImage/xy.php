<?php


/**
 * Two-dimensional gradient image
 */
class imagene_GradientImage_xy {

  protected $w, $h;
  protected $hasAlpha;
  protected $matrix = array();

  function __construct($w, $h, $nx, $ny, $colors, $hasAlpha) {
    $this->w = $w;
    $this->h = $h;
    $this->hasAlpha = $hasAlpha;
    $matrix_x = array();
    for ($x = 0; $x < $w; ++$x) {
      $Bx = imagene_bernstein(($x + 0.5) / $w, $nx);
      for ($i = 0; $i <= $nx; ++$i) {
        $Bxi = $Bx[$i];
        for ($j = 0; $j <= $ny; ++$j) {
          foreach ($colors[$i][$j] as $channel => $v) {
            @$matrix_x[$x][$j][$channel] += $Bxi * $v;
          }
        }
      }
    }
    for ($y = 0; $y < $h; ++$y) {
      $By = imagene_bernstein(($y + .5) / $h, $ny);
      for ($j = 0; $j <= $ny; ++$j) {
        $Byj = $By[$j];
        for ($x = 0; $x < $w; ++$x) {
          foreach ($matrix_x[$x][$j] as $channel => $v) {
            @$this->matrix[$x][$y][$channel] += $Byj * $v;
          }
        }
      }
    }
  }

  function getDimensions() {
    return array($this->w, $this->h);
  }

  function hasAlpha() {
    return $this->hasAlpha;
  }

  function getPixelRGBA($x, $y) {
    return $this->matrix[$x][$y];
  }
}
