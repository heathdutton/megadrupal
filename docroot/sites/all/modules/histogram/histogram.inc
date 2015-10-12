<?php

/**
 * @file
 * Returns histogram filepath and array of values.
 */

/**
 * Makes PNG file histogram.
 */
function _histogram_make_histogram($image, $color = "#000000", $force_bw = FALSE, $hist_type = 1, $hist_width = 256) {
  if (!isset($image)) {
    drupal_set_message(t('An error occurred and processing did not complete.'), 'error');
    return array();
  }

  // Variables.
  $source_file = file_create_url($image);
  $is_color = FALSE;
  $path = str_replace('public:/', variable_get('file_public_path', 'sites/default/files'), $image);
  $file_info = pathinfo($path);
  $directory = $file_info['dirname'];
  $directory .= '/histograms';
  if (!is_dir($directory)) {
    drupal_mkdir($directory);
  };
  // Array of histogram values saved when generating a merged histogram.
  // TODO Use these values to create histogram from js graphing library.
  $histogram_computed = array();

  // Load image into memory.
  $img = _histogram_image_create_from_file($source_file);
  // Count pixel data in image.
  $histogram = _histogram_count_pixel_values($img);
  // Destroy image in memory.
  imagedestroy($img);

  // Automatic Greyscale Detection.
  if ($force_bw != TRUE) {
    for ($a = 0; $a < count($histogram[0]); $a++) {
      // If at any point the values are not the same, the image is color.
      if ($histogram[0][$a] != $histogram[1][$a] || $histogram[1][$a] != $histogram[2][$a]) {
        $is_color = TRUE;
        break;
      }
    }
  }

  // Create Histogram Image.
  $histogram_img = _histogram_render($histogram, $histogram_computed, $color, $is_color, $hist_type, $hist_width);

  // Save Histogram.
  if (is_writable($directory)) {
    touch($directory . "/hist_" . $file_info['filename'] . ".png");
    // Save Histogram to PNG file.
    imagepng($histogram_img, $directory . "/hist_" . $file_info['filename'] . ".png");
    // Prepare $histogram_complete array for returning.
    $histogram_complete[0] = $directory . "/hist_" . $file_info['filename'] . ".png";
    $histogram_complete[1] = $histogram_computed;
    // After saving we can destroy the image in memory.
    imagedestroy($histogram_img);
    // Set success message.
    drupal_set_message(t('Histogram Created.'));
    // Return completed array.
    return $histogram_complete;

  }
  else {
    drupal_set_message(t('The folder is not writable.'), 'error');
    return array();
  }
}

/**
 * Returns RGB values from hexdec color.
 */
function _histogram_html2rgb($color) {
  if ($color[0] == '#') {
    $color = substr($color, 1);
  }
  if (strlen($color) == 6) {
    list($r, $g, $b) = array(
      $color[0] . $color[1],
      $color[2] . $color[3],
      $color[4] . $color[5],
    );
  }
  elseif (strlen($color) == 3) {
    list($r, $g, $b) = array($color[0], $color[1], $color[2]);
  }
  else {
    return FALSE;
  }
  $r = hexdec($r);
  $g = hexdec($g);
  $b = hexdec($b);
  return array($r, $g, $b);
}

/**
 * Returns image from jpeg, png or gif.
 */
function _histogram_image_create_from_file($filename) {
  switch (strtolower(pathinfo($filename, PATHINFO_EXTENSION))) {
    case 'jpeg':
    case 'jpg':
      $img = imagecreatefromjpeg($filename);
      break;

    case 'png':
      $img = imagecreatefrompng($filename);
      break;

    case 'gif':
      $img = imagecreatefromgif($filename);
      break;

    default:
      drupal_set_message(t('File selected for histogram processing is not a valid jpg, png or gif image.'), 'error');
      return FALSE;
  }
  return $img;
}

/**
 * Loops through an image pixel by pixel and counts the rgb values.
 */
function _histogram_count_pixel_values($img) {
  // Step value determines number of pixels to skip during histogram processing.
  // 4 seems to be a good balance of performance and quality.
  $step = 4;

  $histogram_r = array();
  $histogram_g = array();
  $histogram_b = array();
  $histogram_rgb = array();

  // ZERO OUT HISTOGRAM VALUES.
  for ($i = 0; $i < 256; $i++) {
    $histogram_r[$i] = 0;
    $histogram_g[$i] = 0;
    $histogram_b[$i] = 0;
    $histogram_rgb[$i] = 0;
  }

  // COUNT HISTOGRAM VALUES.
  // Go through image line by line and count the pixel values.
  $img_width = imagesx($img);
  $img_height = imagesy($img);
  for ($i = 0; $i < $img_width; $i += $step) {
    for ($j = 0; $j < $img_height; $j += $step) {
      // Get the pixel values at a given point.
      $rgb = imagecolorat($img, $i, $j);
      $r = ($rgb >> 16) & 0xFF;
      $g = ($rgb >> 8) & 0xFF;
      $b = $rgb & 0xFF;

      // Increment the count of the total number of pixels at a given value.
      $v = round($r * 1);
      $histogram_r[$v] += 1;
      $v = round($g * 1);
      $histogram_g[$v] += 1;
      $v = round($b * 1);
      $histogram_b[$v] += 1;
      $v = round(($r + $g + $b) / 3);
      $histogram_rgb[$v] += 1;
    }
  }

  return array($histogram_r, $histogram_g, $histogram_b, $histogram_rgb);
}


/**
 * Checks histogram values for exceptionally high spikes.
 */
function _histogram_clip($histogram) {
  sort($histogram);
  $lerp = min(max(($histogram[255] / $histogram[250] - 1.15) / 2.0, 0.0), 1.0);
  $histogram_clip = (1.0 - $lerp) * $histogram[255] + $lerp * $histogram[250];
  return $histogram_clip;
}

/**
 * Render Histogram Image.
 */
function _histogram_render($histogram, &$histogram_computed, $color, $is_color, $hist_type, $hist_width) {
  $max_height = 100;
  $histogram_r = $histogram[0];
  $histogram_g = $histogram[1];
  $histogram_b = $histogram[2];
  $histogram_rgb = $histogram[3];

  if (($hist_type == '1') or ($is_color == FALSE)) {
    // Merged or Greyscale Histogram.
    $hist_height = 100;
  }
  else {
    // Stacked Histogram.
    $hist_height = 300;
  }

  // Create Image.
  $histogram_img = imagecreatetruecolor($hist_width, $hist_height)
  or die("Cannot Initialize new GD image stream");

  // Calculate Background Color.
  if (empty($color)) {
    // Transparent background.
    imagealphablending($histogram_img, FALSE);
    imagesavealpha($histogram_img, TRUE);
    $bg_color = imagecolorallocatealpha($histogram_img, 0, 0, 0, 127);
  }
  else {
    $rgb_color = _histogram_html2rgb($color);
    $bg_color = imagecolorallocate($histogram_img, $rgb_color[0], $rgb_color[1], $rgb_color[2]);
  }

  // Fill Background Color.
  imagefilledrectangle($histogram_img, 0, 0, $hist_width, $hist_height, $bg_color);
  imagefill($histogram_img, 0, 0, $bg_color);

  // Make sure histogram color is different from background color.
  if (empty($color) || (($rgb_color[0] + $rgb_color[0] + $rgb_color[0]) / 3) < 127) {
    $graphcolor = 255;
  }
  else {
    $graphcolor = 0;
  }

  // Check histogram values for exceptionally high spikes and clip them.
  $histogram_clip_r = _histogram_clip($histogram_r);
  $histogram_clip_g = _histogram_clip($histogram_g);
  $histogram_clip_b = _histogram_clip($histogram_b);

  // Use only the highest value out of all three color channels.
  $histogram_clip = max($histogram_clip_r, $histogram_clip_g, $histogram_clip_b);

  // CREATE GRAPH.
  for ($a = 0; $a < 256; $a++) {
    $line_heights = array(
      min($histogram_r[$a] / $histogram_clip, 1.0) * $max_height,
      min($histogram_g[$a] / $histogram_clip, 1.0) * $max_height,
      min($histogram_b[$a] / $histogram_clip, 1.0) * $max_height,
    );
    // Save computed values to array.
    // TODO Use this value to create graph using JS instead of image file.
    $histogram_computed[$a] = $line_heights;

    if ($is_color) {
      if ($hist_type == '1') {
        // COMBINED RGB HISTOGRAM.
        $line_order = array(0, 1, 2);
        array_multisort($line_heights, $line_order);

        // Draws up from the bottom.
        // White line where all three channels overlap.
        $line_rgb = array(255, 255, 255);
        $start = $max_height - $line_heights[0];
        $end = $max_height;
        _histogram_draw_line($histogram_img, $line_rgb, $a, $start, $end);

        // Two channels overlap. In Middle.
        $line_rgb[$line_order[0]] = 0;
        $start = $max_height - $line_heights[1];
        $end = $max_height - $line_heights[0];
        _histogram_draw_line($histogram_img, $line_rgb, $a, $start, $end);

        // Single channel. At Top.
        $line_rgb[$line_order[1]] = 0;
        $start = $max_height - $line_heights[2];
        $end = $max_height - $line_heights[1];
        _histogram_draw_line($histogram_img, $line_rgb, $a, $start, $end);
      }
      else {
        // STACKED HISTOGRAM.

        // Red.
        $line_rgb = array(255, 0, 0);
        $start = $max_height - $line_heights[0];
        $end = $max_height;
        _histogram_draw_line($histogram_img, $line_rgb, $a, $start, $end);

        // Green.
        $line_rgb = array(0, 255, 0);
        $start = $max_height - $line_heights[1];
        $end = $max_height;
        _histogram_draw_line($histogram_img, $line_rgb, $a, $start + 100, $end + 100);

        // Blue.
        $line_rgb = array(0, 0, 255);
        $start = $max_height - $line_heights[2];
        $end = $max_height;
        _histogram_draw_line($histogram_img, $line_rgb, $a, $start + 200, $end + 200);
      }
    }
    else {
      // SINGLE CHANNEL HISTOGRAM.
      $line_height = min($histogram_rgb[$a] / $histogram_clip, 1.0) * $max_height;
      $line_rgb = array($graphcolor, $graphcolor, $graphcolor);
      $start = $max_height - $line_height;
      $end = $max_height;
      _histogram_draw_line($histogram_img, $line_rgb, $a, $start, $end);
    }
  }

  return $histogram_img;
}

/**
 * Draws a line of the histogram.
 */
function _histogram_draw_line(&$histogram_img, $line_rgb, $a, $start, $end) {
  $line_color = imagecolorallocatealpha($histogram_img, $line_rgb[0], $line_rgb[1], $line_rgb[2], 0);
  imageline($histogram_img, ($a + 1), $start, ($a + 1), $end, $line_color);
}
