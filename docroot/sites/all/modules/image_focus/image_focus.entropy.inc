<?php
/**
 * @file
 * Image entropy calculation.
 *
 * @todo Replace imagecolorat() with imagecolorsforindex() for index image
 * support (even it is slower).
 */

class ImageFocusEntropy {

  // The GD image resource.
  protected $image;

  // Coordinate of the focal point.
  protected $cx, $cy;

  public function __construct($image) {
    $this->image = $image;
  }

  /**
   * Calculates the histogram of the zone from ($x, $y) with a width of $dx and
   * height of $dy.
   */
  protected function calculateHistogram($x, $y, $dx, $dy) {
    $maxx = min(imagesx($this->image), $x + $dx);
    $maxy = min(imagesy($this->image), $y + $dy);
    $histogram = array_fill(0, 768, 0);
    for ($i = $x; $i < $maxx; $i++) {
      for ($j = $y; $j < $maxy; $j++) {
        $rgb = imagecolorat($this->image, $i, $j);
        $r = ($rgb >> 16) & 0xFF;
        $g = ($rgb >> 8) & 0xFF;
        $b = $rgb & 0xFF;
        $histogram[$r]++;
        $histogram[$g + 256]++;
        $histogram[$b + 512]++;
      }
    }
    return $histogram;
  }

  /**
   * Calculates the entropy of an image zone, defined as -sum(p.*log2(p)) where
   * p is the histogram counts of a grayscale image.
   */
  protected function calculateEntropy($x, $y, $dx, $dy) {
    $histogram = $this->calculateHistogram($x, $y, $dx, $dy);
    $histogram_size = array_sum($histogram);
    $entropy = 0;
    foreach ($histogram as $p) {
      if ($p == 0) {
        continue;
      }
      $p = $p / $histogram_size;
      $entropy += $p * log($p, 2);
    }
    return $entropy * -1;
  }

  /**
   * Calculates the focal point.
   */
  public function calculateFocalPoint() {
    // Split the image into multiple zones and calculate entropy in each one.
    // $d is the size of the zones. This value is chosen emperically.
    // Then calculate the center of mass using entropy.
    $d = (int) max((imagesx($this->image) + imagesy($this->image))/50, 10);
    $nx = floor(imagesx($this->image) / $d);
    $ny = floor(imagesy($this->image) / $d);
    $entropies = array_fill(0, $nx * $ny, 0);
    $sum = $wsum_x = $wsum_y = 0;
    for ($i = 0; $i < $nx; $i++) {
      for ($j = 0; $j < $ny; $j++) {
        $entropy = $this->calculateEntropy($i * $d, $j * $d, $d, $d);
        $entropies[$i + $j * $nx] = $entropy;
        $sum += $entropy;
        $wsum_x += $entropy * $i;
        $wsum_y += $entropy * $j;
      }
    }

    $this->cx = $sum ? $wsum_x / $sum * $d : 0;
    $this->cy = $sum ? $wsum_y / $sum * $d : 0;
  }

  public function getFocalPoint() {
    if (!isset($this->cx)) {
      $this->calculateFocalPoint();
    }

    return array($this->cx, $this->cy);
  }
}

