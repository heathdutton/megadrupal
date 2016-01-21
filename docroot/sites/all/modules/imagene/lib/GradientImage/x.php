<?php


/**
 * Horizontal gradient image
 */
class imagene_GradientImage_x extends imagene_GradientImage_y {

  function getDimensions() {
    return array($this->dim, 1);
  }

  function getPixelRGBA($x, $y) {
    return parent::getPixelRGBA($y, $x);
  }
}
