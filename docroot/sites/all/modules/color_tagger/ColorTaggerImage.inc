<?php

/**
 * @file
 * Color Tagger ColorTaggerImage Class.
 */

/**
 * Class ColorTaggerImage.
 *
 * Processes an image to find colors matching those in a given taxonomy.
 */
class ColorTaggerImage {

  public $filename;
  public $image;
  public $height;
  public $width;
  public $colorLimit;
  public $matchedColors;
  private $useLab;
  private $step;
  private $colorPalette = array();
  private $backgroundColor;
  private $threshold;

  public function __construct($filename, $color_palette) {
    $this->colorPalette = $color_palette;
    $this->useLab = color_tagger_get_record('use_lab', 1);
    $this->colorLimit = color_tagger_get_record('color_limit', '2');
    $this->threshold = color_tagger_get_record('threshold', '10');
    if ($filename) {
      $this->filename = file_create_url($filename);
      $this->createImageFromFile();
    }
  }

  /**
   * Creates image from jpeg, png or gif file.
   */
  private function createImageFromFile() {
    $filename = $this->filename;

    switch (strtolower(pathinfo($filename, PATHINFO_EXTENSION))) {
      case 'jpeg':
      case 'jpg':
        $this->image = imagecreatefromjpeg($filename);
        break;

      case 'png':
        $this->image = imagecreatefrompng($filename);
        break;

      case 'gif':
        $this->image = imagecreatefromgif($filename);
        break;

      default:
        drupal_set_message(t('File "@filename" could not be loaded. Check that image file exists and it is a valid jpg, png or gif file.', array('@filename' => $filename)), 'error');
    }
  }

  /**
   * Gets the size of the image.
   */
  public function getImageSize() {
    $width = imagesx($this->image);
    $height = imagesy($this->image);
    $this->height = $height;
    $this->width = $width;
    return "Image width = " . $width . ", height = " . $height;
  }

  /**
   * Calculate the step value depending on the size of the image.
   *
   * Step value determines number of pixels to skip during processing.
   * Aprox. 15000 pixels per image will be processed.
   */
  private function calculateStepValue() {
    $this->step = floor(max($this->width, $this->height) / 150);
  }

  /**
   * Loops through an image pixel by pixel and counts the rgb values.
   */
  private function countPixelValues() {
    // Check if image has been loaded.
    if (!$this->image) {
      drupal_set_message(t('File "@filename" could not be loaded. Check that image file exists and it is a valid jpg, png or gif file.', array('@filename' => $this->filename)), 'error');
      return FALSE;
    }

    // Setup parameters.
    $this->getImageSize();
    $this->calculateStepValue();
    if (color_tagger_get_record('ignore_background', 0)) {
      $this->getBackgroundColor();
    }

    // Search image pixel by pixel.
    $matched_colors = array();
    for ($i = 0; $i < $this->width; $i += $this->step + 1) {
      for ($j = 0; $j < $this->height; $j += $this->step + 1) {
        // Get the pixel values at a given point.
        $pixel_value = $this->getPixelValue($i, $j);

        // Check for color match.
        $matched_color_tid = $this->findColorMatch($pixel_value);

        // Increment count of how many times this value has been matched.
        array_key_exists($matched_color_tid, $matched_colors) ? $matched_colors[$matched_color_tid]++ : $matched_colors[$matched_color_tid] = 1;
      }
    }

    // Sort array by highest values.
    arsort($matched_colors);

    $this->matchedColors = $matched_colors;

    return TRUE;
  }

  /**
   * Get the RGB or LAB values for a pixel at a given coordinate.
   */
  public function getPixelValue($x, $y) {
    $pixel_value = imagecolorat($this->image, $x, $y);

    $r = ($pixel_value >> 16) & 0xFF;
    $g = ($pixel_value >> 8) & 0xFF;
    $b = $pixel_value & 0xFF;

    $rgb = array($r, $g, $b);

    if ($this->useLab) {
      return _color_tagger_rgb_to_lab($rgb);
    }
    else {
      return $rgb;
    }
  }

  /**
   * Determines the background color in an image.
   */
  public function getBackgroundColor() {
    $background_color = NULL;

    $colors['top_left'] = $this->getPixelValue(0, 0);
    $colors['top_right'] = $this->getPixelValue($this->width - 1, 0);
    $colors['bottom_left'] = $this->getPixelValue(0, $this->height - 1);
    $colors['bottom_right'] = $this->getPixelValue($this->width - 1, $this->height - 1);

    foreach ($colors as $name => $color) {
      $colors[$name] = $this->findColorMatch($color);
    }

    if (count(array_unique($colors)) == 1) {
      $background_color = $colors['top_left'];
    }

    $this->backgroundColor = $background_color;
  }

  /**
   * Finds the closest matching color for a given RGB or LAB value.
   *
   * @param array $pixel_value
   *   An array of RGB or LAB color values.
   *
   * @return int
   *   Returns the TID of the closest matching term.
   */
  public function findColorMatch($pixel_value) {
    $color_palette = $this->colorPalette;
    $min_distance = NULL;

    foreach ($color_palette as $palette_color) {
      if ($this->useLab) {
        $distance = _color_tagger_calculate_color_distance_lab_2000($pixel_value, $palette_color['lab']);
      }
      else {
        $distance = _color_tagger_calculate_color_distance_rgb($pixel_value, $palette_color['rgb']);
      }
      if ($min_distance === NULL || $distance < $min_distance) {
        $min_distance = $distance;
        $closest_match = $palette_color['tid'];
      }
    }

    return $closest_match;
  }

  /**
   * Gets the color matches for the image.
   *
   * @param array $blacklist_colors
   *   Array of colors that have been blacklisted and will not be ignored in the returned results.
   *
   * @param bool $debug
   *  Toggles output of color matches for debugging.
   *
   * @return array
   *   Returns array of colors limited by the number defined in the threshold parameter.
   */
  public function getMatchedColors($blacklist_colors = array(), $debug = FALSE) {
    // Process image.
    $count_success = $this->countPixelValues();
    if (!$count_success) {
      // Abort.
      return FALSE;
    }

    $matched_colors = $this->matchedColors;
    if (!$matched_colors) {
      drupal_set_message(t('No color matches were found. Check that the taxonomy terms are properly configured with valid hexadecimal color values.'), 'error');
      return FALSE;
    }

    // Print values with color name for debugging.
    if ($debug) {
      $debug_output['Total Pixels'] = 0;
      foreach ($matched_colors as $tid => $count) {
        $term = taxonomy_term_load($tid);
        $debug_output[$term->name] = $count;
        $debug_output['Total Pixels'] += $count;
      }
      dpm($debug_output);
    }

    // Remove colors below threshold percentage.
    if (!empty($this->threshold)) {
      $total_num_pixels = array_sum($matched_colors);
      foreach ($matched_colors as $tid => $pixel_count) {
        $percentage = (($pixel_count / $total_num_pixels) * 100);
        if ($percentage < $this->threshold) {
          unset($matched_colors[$tid]);
        }
      }
    }

    // Add background color to blacklist.
    if ($this->backgroundColor) {
      $blacklist_colors[] = $this->backgroundColor;
    }

    // Remove blacklisted colors.
    foreach ((array) $blacklist_colors as $tid) {
      unset($matched_colors[$tid]);
    }

    $top_colors = array_slice($matched_colors, 0, $this->colorLimit, TRUE);

    return $top_colors;
  }

}
