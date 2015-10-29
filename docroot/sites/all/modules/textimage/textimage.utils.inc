<?php
/**
 * @file
 * Utility routines of Textimage.
 */

/**
 * Generate an image containing text with the given parameters.
 *
 * @return resource
 *   A GD image resource.
 */
function textimage_text_to_image($text, $fontsize, $font, $color = array('hex' => '#000000', 'opacity' => '100'), $angle = 0, $maximum_width = 0, $fixed_width = 0, $align = ALIGN_LEFT) {
  // Set rotation angle.
  $q_angle = -$angle;

  while ($q_angle > 0 || $q_angle <= -90) {
    $q_angle -= $q_angle > 0 ? 90 : -90;
  }

  while ($angle < 0 || $angle >= 360) {
    $angle += $angle < 0 ? 360 : -360;
  }

  $rotation = -(floor($angle / 90) * 90);
  $rad = deg2rad($q_angle);
  $sin = -sin($rad);
  $cos = cos($rad);

  // Perform text wrapping, if necessary.
  $max_width = $maximum_width - 1;
  if ($max_width > 0) {
    $text = textimage_wrap_text($text, $fontsize, $font, $max_width, $align);
  }

  // Get co-ordinates of text string and boundry box.
  $coords = imagettfbbox($fontsize, 0, $font, $text);

  // Set consistant heights.
  if ($coords[3] - $coords[5] < $fontsize * 2) {
    $characters = drupal_map_assoc(range(33, 122), 'chr');
    $characters = join($characters);
    $testcoords = imagettfbbox($fontsize, 0, $font, $characters);

    if ($coords[3] - $coords[5] < $testcoords[3] - $testcoords[5]) {
      $coords[1] = $testcoords[1];
      $coords[3] = $testcoords[3];
      $coords[5] = $testcoords[5];
      $coords[7] = $testcoords[7];
    }
  }

  // Fix boundary box.
  $bbox = array();
  for ($i = 0; $i < 7; $i += 2) {
    $bbox[$i] = round($coords[$i] * $cos + $coords[$i + 1] * -$sin);
    $bbox[$i + 1] = round($coords[$i + 1] * $cos - $coords[$i] * -$sin);
  }

  // Calculate dimensions of text box.
  $text_width = sqrt(pow(abs($bbox[0] - $bbox[2]), 2) + pow(abs($bbox[1] - $bbox[3]), 2));
  // Not used.
  $text_height = sqrt(pow(abs($bbox[0] - $bbox[6]), 2) + pow(abs($bbox[1] - $bbox[7]), 2));

  // Calculate dimensions of box from text box.
  $box_width = max($bbox[0], $bbox[2], $bbox[4], $bbox[6]) - min($bbox[0], $bbox[2], $bbox[4], $bbox[6]);
  $box_height = max($bbox[1], $bbox[3], $bbox[5], $bbox[7]) - min($bbox[1], $bbox[3], $bbox[5], $bbox[7]);

  // Calculate dimensions of image.
  $image_width = ($fixed_width && $maximum_width > 0) ? ($maximum_width - 1) * $cos + $text_height * $sin : $box_width;
  $image_height = ($fixed_width && $maximum_width > 0) ? ($maximum_width - 1) * $sin + $text_height * $cos : $box_height;

  // Create Image.
  $image = imagecreatetruecolor($image_width + 1, $image_height + 1);
  $back = imagecolorallocatealpha($image, 0, 0, 0, 127);
  imagefill($image, 0, 0, $back);

  // Set text alignment left.
  $x = -$bbox[0];
  $y = $box_height - $bbox[3];

  if (($fixed_width && $maximum_width > 0) && $align !== ALIGN_LEFT) {
    switch ($align) {

      // Set text alignment center.
      case ALIGN_CENTER:
        $x += ($image_width - $box_width) / 2;
        $y += ($image_height - $box_height) / 2;
        break;

      // Set text alignment right.
      case ALIGN_RIGHT:
        $x += $image_width - $box_width;
        $y += $image_height - $box_height;
        break;
    }
  }

  // Create the textimage.
  list($r, $g, $b) = _textimage_hex2rgb($color['hex']);
  $alpha = -($color['opacity'] - 100) / 100 * 127;
  $fore = imagecolorallocatealpha($image, $r, $g, $b, $alpha);

  imagettftext($image, $fontsize, $q_angle, $x, $y, $fore, $font, $text);
  if ($rotation != 0) {
    $image = imagerotate($image, $rotation, 0);
  }

  imagealphablending($image, TRUE);
  imagesavealpha($image, TRUE);

  return $image;
}

/**
 * Helper function for wrapping text (measures width).
 */
function textimage_measure_text_width($text, $fontsize, $font) {
  $box = imagettfbbox($fontsize, 0, $font, $text);
  return abs($box[4] - $box[0]) + 4;
}

/**
 * Wrap text for rendering at a given width.
 */
function textimage_wrap_text($text, $fontsize, $font, $maximum_width, $align) {
  // State variables for the search interval.
  $end = 0;
  $begin = 0;
  $fit = $begin;
  if ($align == ALIGN_CENTER) {
    // Get width of the space character.
    $space_width = textimage_measure_text_width(' ', $fontsize, $font);
  }
  // Note: we count in bytes for speed reasons, but maintain character
  // boundaries.
  while (TRUE) {
    // Find the next wrap point (always after trailing whitespace).
    if (drupal_preg_match('/[' . PREG_CLASS_PUNCTUATION . '][' . PREG_CLASS_SEPARATOR . ']*|[' . PREG_CLASS_SEPARATOR . ']+/u', $text, $match, PREG_OFFSET_CAPTURE, $end)) {
      $end = $match[0][1] + drupal_strlen($match[0][0]);
    }
    else {
      $end = drupal_strlen($text);
    }

    // Fetch text, removing trailing white-space and measure it.
    $line  = preg_replace('/[' . PREG_CLASS_SEPARATOR . ']+$/u', '', drupal_substr($text, $begin, $end - $begin));
    $width = textimage_measure_text_width($line, $fontsize, $font);

    // See if $line extends past the available space.
    if ($width > $maximum_width) {
      // If this is the first word, we need to truncate it.
      if ($fit == $begin) {
        // Cut off letters until it fits.
        while (drupal_strlen($line) > 0 && $width > $maximum_width) {
          $line  = drupal_substr($line, 0, -1);
          $width = textimage_measure_text_width($line, $fontsize, $font);
        }
        // If no fit was found, the image is too narrow..
        $fit = drupal_strlen($line) ? $begin + drupal_strlen($line) : $end;
      }
      // We have a valid fit for the next line. Insert a line-break and reset
      // the search interval.
      $first_part = drupal_substr($text, 0, $fit);
      $last_part  = drupal_substr($text, $fit);
      if ($align == ALIGN_CENTER) {
        $text  = $first_part . "\n" . $last_part;
        // Get last line of the text's first part.
        $first_part_array = explode("\n", $first_part);
        $last_index = count($first_part_array) - 1;
        $last_line  = $first_part_array[$last_index];
        $last_line_width = textimage_measure_text_width($last_line, $fontsize, $font);
        // Get needed space for padding.
        $num_space = floor(($maximum_width - $last_line_width) / $space_width);
        // Add space padding (to center the last line of the text's first
        // part).
        for ($i = 0; $i < $num_space; ++$i) {
          $last_line = ' ' . $last_line;
        }
        $first_part_array[$last_index] = $last_line;
        $first_part = implode("\n", $first_part_array);
        $last_part_width = textimage_measure_text_width($last_part, $fontsize, $font);
        if ($last_part_width < $maximum_width) {
          // Get needed space for last part's padding.
          $num_space = floor(($maximum_width - $last_part_width) / $space_width) - 2;
          $last_part = trim($last_part);
          // Add space padding (to center the last line of the
          // text's first part).
          for ($i = 0; $i < $num_space; ++$i) {
            $last_part = ' ' . $last_part;
          }
        }
      }
      $text  = $first_part . "\n" . $last_part;
      $begin = ++$fit;
      $end   = $begin;
    }
    else {
      // We can fit this text. Wait for now.
      $fit = $end;
    }

    if ($end == drupal_strlen($text)) {
      // All text fits. No more changes are needed.
      break;
    }
  }
  return $text;
}


/**
 * Unicode-safe preg_match().
 *
 * Search subject for a match to the regular expression given in pattern,
 * but return offsets in characters, where preg_match would return offsets
 * in bytes.
 *
 * @see http://php.net/manual/en/function.preg-match.php
 * @see http://drupal.org/node/465638
 */
if (!function_exists('drupal_preg_match')) {
  /**
   * Todo.
   */
  function drupal_preg_match($pattern, $subject, &$matches, $flags = NULL, $offset = 0) {
    // Convert the offset value from characters to bytes.
    // NOTE - strlen is used on purpose here, instead of drupal_strlen
    $offset = strlen(drupal_substr($subject, 0, $offset));

    $return_value = preg_match($pattern, $subject, $matches, $flags, $offset);

    if ($return_value && ($flags & PREG_OFFSET_CAPTURE)) {
      foreach ($matches as &$match) {
        // Convert the offset returned by preg_match from bytes back to
        // characters.
        // NOTE - substr is used on purpose here, instead of drupal_substr
        $match[1] = drupal_strlen(substr($subject, 0, $match[1]));
      }
    }
    return $return_value;
  }
}

/**
 * Todo.
 */
if (!function_exists('imagerotate')) {
  /**
   * Todo.
   */
  function imagerotate($im, $angle, $bgcolor) {
    if ($angle === 0) {
      return $im;
    }

    // imagerotate() in php's libgd rotates the image counterclockwise,
    // this implementation rotates clockwise. The angle needs to be
    // inverted to give the same behaviour between these implementations.
    $angle = 360 + $angle;

    $width  = imagesx($im);
    $height = imagesy($im);

    // Background color.
    list($r, $g, $b, $a) = _textimage_hex2rgb($bgcolor);

    switch ($angle) {
      case 270:
      case 90:
        // Flip dimensions.
        $rot_width = $height;
        $rot_height = $width;
        break;

      case 180:
        // Maintain dims.
        $rot_width = $width;
        $rot_height = $height;
        break;
    }

    $rotate = imagecreatetruecolor($rot_width, $rot_height);
    $bg = imagecolorallocatealpha($rotate, $r, $g, $b, $a);
    imagefilledrectangle($rotate, 0, 0, $rot_width, $rot_height, $bg);
    imagealphablending($rotate, FALSE);
    imagesavealpha($rotate, TRUE);

    switch ($angle) {
      case 270:
        $rot_width--;
        for ($y = 0; $y < $height; ++$y) {
          for ($x = 0; $x < $width; ++$x) {
            imagesetpixel($rotate, $rot_width - $y, $x, imagecolorat($im, $x, $y));
          }
        }
        break;

      case 90:
        $rot_height--;
        for ($y = 0; $y < $height; ++$y) {
          for ($x = 0; $x < $width; ++$x) {
            imagesetpixel($rotate, $y, $rot_height - $x, imagecolorat($im, $x, $y));
          }
        }
        break;

      case 180:
        $rot_width--;
        $rot_height--;
        for ($y = 0; $y < $height; ++$y) {
          for ($x = 0; $x < $width; ++$x) {
            imagesetpixel($rotate, $rot_width - $x, $rot_height - $y, imagecolorat($im, $x, $y));
          }
        }
        break;
    }
    return $rotate;
  }
}

/**
 * This function adds a margin (or border) around an existing image resource.
 */
function textimage_image_add_margin($img, $margin) {
  $width = imagesx($img);
  $height = imagesy($img);

  // Create a new image for the background.
  $back_img = imagecreatetruecolor($width + $margin['right'] + $margin['left'], $height + $margin['top'] + $margin['bottom']);
  $back = imagecolorallocatealpha($back_img, 0, 0, 0, 127);
  imagefill($back_img, 0, 0, $back);

  // Apply the source image ontop the background with the new margin.
  imagecopy($back_img, $img, $margin['left'], $margin['top'], 0, 0, $width, $height);
  imagealphablending($back_img, TRUE);
  imagesavealpha($back_img, TRUE);

  return $back_img;
}

/**
 * Convert a hex color representation to it's rgb integer components.
 *
 * @param string $hex
 *   Hex representation of the color.
 *   Can be in the formats: '#ABC','ABC','#AABBCC','AABBCC'
 *
 * @return array
 *   Array with three components RGB.
 */
function _textimage_hex2rgb($hex) {
  $r = $g = $b = '';

  $hex = ltrim($hex, '#');
  if (preg_match('/^[0-9a-f]{3}$/i', $hex)) {
    // 'FA3' is the same as 'FFAA33' so r=FF, g=AA, b=33
    $r = str_repeat($hex{0}, 2);
    $g = str_repeat($hex{1}, 2);
    $b = str_repeat($hex{2}, 2);
  }
  elseif (preg_match('/^[0-9a-f]{6}$/i', $hex)) {
    // #FFAA33 or r=FF, g=AA, b=33
    $r = drupal_substr($hex, 0, 2);
    $g = drupal_substr($hex, 2, 2);
    $b = drupal_substr($hex, 4, 2);
  }

  $r = hexdec($r);
  $g = hexdec($g);
  $b = hexdec($b);
  return array($r, $g, $b);
}

/**
 * Utility function for image_stroke.
 *
 * Analyzes surrounding pixels and determines opacity of a pixel at that
 * x-y coordinate.
 */
function textimage_image_stroke_change_pixels(&$img, &$border_img, $thickness, $color, $x, $y, $width, $height) {
  list($r, $g, $b) = _textimage_hex2rgb($color);

  $pixel = imagecolorsforindex($img, imagecolorat($img, $x, $y));

  // Preform a radial analysis of all pixels within the radius of $thickness
  // pixels.
  $degree_increment = (90 / $thickness);
  $radial_coords = array();

  for ($degrees = 0; $degrees <= 90; $degrees += $degree_increment) {
    $x_offset = round(cos($degrees) * $thickness);
    $y_offset = round(sin($degrees) * $thickness);

    // Add the coordinates for the corresponding pixel in each 90° quadrant.
    $radial_coords[] = array('x' => $x + $x_offset, 'y' => $y + $y_offset);
    $radial_coords[] = array('x' => $x - $x_offset, 'y' => $y + $y_offset);
    $radial_coords[] = array('x' => $x + $x_offset, 'y' => $y - $y_offset);
    $radial_coords[] = array('x' => $x - $x_offset, 'y' => $y - $y_offset);
  }

  // Generate a total alpha level for all analyzed pixels.
  $total_alpha = 0;
  $total_colors = 0;

  foreach ($radial_coords as $coords) {
    if ($coords['x'] >= 0 && $coords['y'] >= 0 && $coords['x'] < $width && $coords['y'] < $height) {
      $xy_color = imagecolorsforindex($img, imagecolorat($img, $coords['x'], $coords['y']));
    }
    else {
      // This analized pixel is outside the dimensions of the image,
      // record as transparent.
      $xy_color = array('alpha' => '127');
    }

    $total_alpha += $xy_color['alpha'];
    $total_colors++;
  }

  // Check that we're not in the middle of the image or in a blonk area.
  if ($total_alpha < (127 * $total_colors) && $total_alpha > 0) {
    // If we're on a semi-transparent pixel, blend the remaining amount
    // with our border color.
    if ($pixel['alpha'] < 127) {
      $alpha = 127 - $pixel['alpha'];
    }
    else {
      // We're on a completely transparent pixel where we'll
      // use a generated transparency.
      $alpha = 127 - ((127 * $total_colors) - $total_alpha);
    }
    $alpha = ($alpha < 0) ? 0 : $alpha;
    $alpha = ($alpha > 127) ? 127 : $alpha;
    // Apply the color to the border overlay image.
    $color = imagecolorallocatealpha($border_img, $r, $g, $b, $alpha);
    imagesetpixel($border_img, $x, $y, $color);
  }
}
