<?php

/**
 * Apply an convolution to an image given a matrix kernel and a divisor using 
 * ImageMagick.
 *
 * @param $image
 *   An image object. The $image->resource will be modified by this call.
 * @param $matrix
 *   The matrix kernel to be applied.
 * @param $divisor
 *   The divisor to be applied.
 *  
 * @return
 *   TRUE
 *
 * @see imagemagick convert()
 */
function image_imagemagick_filtersie_convolution(stdClass $image, $matrix, $divisor, $offset) {
  $matrix_s = '';
  foreach ($matrix as $vector) {
      $vector = array_map(create_function('$e', "{return (\$e/$divisor)+$offset;}"), $vector);
      $matrix_s .= implode(',',$vector) . " ";
  }
  $matrix_s = substr($matrix_s, 0, -1);
  $image->ops[] = "-morphology Convolve '3x3:" . $matrix_s . "'";
  return TRUE;
}

/**
 * Sharpen an image using the Unsharp masking technique using ImageMagick.
 *
 * @param $img
 *   An image object. The $image->resource will be modified by this call.
 * @param $amount, $radius and $threshols
 *   @see http://en.wikipedia.org/wiki/Unsharp_masking#Digital_unsharp_masking
 * 
 * @return
 *   TRUE
 *
 * @see http://en.wikipedia.org/wiki/Unsharp_masking 
 * @see http://www.imagemagick.org/script/command-line-options.php?#unsharp
 *
 * @todo Figure out how to make gd and imagemagick implementations output the 
 * same result form the same inputs.
 */
function image_imagemagick_filtersie_UnsharpMask(stdClass $image, $amount, $radius, $threshold) {
  $sigma = 1;
  $image->ops[] = '-unsharp ' . $radius . 'x' . $sigma . '+' . $amount / 100  . '+' . $threshold / 100;
  return TRUE;  
}
