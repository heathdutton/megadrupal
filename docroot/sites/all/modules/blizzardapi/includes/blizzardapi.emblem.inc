<?php

/**
 * @file
 * Implements emblem creation using layered base images.
 */

/**
 * Interface for renderable emblems.
 */
interface BlizzardApiRenderInterface {
  /**
   * Determines the destination path of a rendered emblem image.
   *
   * @return mixed
   *   The full path of the emblem image, or FALSE if there was a problem
   *   setting up the directory.
   */
  public static function getRenderPath($emblem, $options = array());
  
  /**
   * Renders an emblem image.
   *
   * @param array $emblem
   *   An array containing emblem creation information.
   * @param int $size
   *   The size of the rendered emblem.
   *
   * @return mixed
   *   The path of the generated emblem, or FALSE if there was a problem
   *   creating the image.
   */
  public static function render($emblem, $size, $options = array());
}

/**
 * Base class for rendering emblems.
 */
abstract class BlizzardApiEmblem implements BlizzardApiRenderInterface {
  /**
   * Blends a color with a pixel in the specified source image.
   */
  private static function colorize($src, $x, $y, $color, $intensity = 19.0, $blend = 0.33333) {
    $added_r = $color['r'] / $intensity + $color['r'] * $blend;
    $added_g = $color['g'] / $intensity + $color['g'] * $blend;
    $added_b = $color['b'] / $intensity + $color['b'] * $blend;
    $scale_r = $color['r'] / 255 + $blend;
    $scale_g = $color['g'] / 255 + $blend;
    $scale_b = $color['b'] / 255 + $blend;
    
    $pixel = imagecolorsforindex($src, imagecolorat($src, $x, $y));
    if ($pixel['alpha'] != 127) {
      $color['r'] = $pixel['red'] * $scale_r + $added_r;
	  $color['g'] = $pixel['green'] * $scale_g + $added_g;
	  $color['b'] = $pixel['blue'] * $scale_b + $added_b;
    }
    
    $color['r'] = $color['r'] > 255 ? 255 : ($color['r'] < 0 ? 0 : $color['r']);
    $color['g'] = $color['g'] > 255 ? 255 : ($color['g'] < 0 ? 0 : $color['g']);
    $color['b'] = $color['b'] > 255 ? 255 : ($color['b'] < 0 ? 0 : $color['b']);
    
    $color['a'] = $pixel['alpha'];
    return $color;
  }
  
  /**
   * "Colorizes" a greyscale image based on the specified color.
   *
   * @param resource $src
   *   A GD image handle of the source image.
   * @param mixed $color
   *   The foreground color, as a hexadecimal string, or an array with RGB keys.
   */
  protected static function colorizeImage($src, $color) {
    if (is_string($color)) {
      $color = self::unpackColor($color);
    }
    
    $width = imagesx($src);
    $height = imagesy($src);
    
    for ($x = 0; $x < $width; $x++) {
      for ($y = 0; $y < $height; $y++) {
        $colorized = self::colorize($src, $x, $y, $color);
        $colorized = imagecolorallocatealpha($src, $colorized['r'], $colorized['g'], $colorized['b'], $colorized['a']);
        imagesetpixel($src, $x, $y, $colorized);
        imagecolordeallocate($src, $colorized);
      }
    }
  }
  
  /**
   * Overlays a color on a grayscale image.
   *
   * @param resource $src
   *   A GD image handle of the source image.
   * @param mixed $color
   *   The foreground color, as a hexadecimal string, or an array with RGB keys.
   */
  protected static function overlayImage($src, $color) {
    if (is_string($color)) {
      $color = self::unpackColor($color);
    }
    
    $width = imagesx($src);
    $height = imagesy($src);
    $color = imagecolorallocatealpha($src, $color['r'], $color['g'], $color['b'], $color['a']);
    
    imagelayereffect($src, IMG_EFFECT_OVERLAY);
    imagefilledrectangle($src, 0, 0, $width, $height, $color);
  }
  
  /**
   * Copies a rectangular portion of an emblem base image onto a target image.
   *
   * All coordinates refer to the upper left corner.
   * 
   * @param resource $dst
   *   A GD image handle of the target image.
   * @param string $src
   *   A GD image handle of the image to overlay on the target.
   * @param array $dst_rect
   *   The area of the destination to place the source image.
   *
   *   Requires the following keys:
   *   - x: The x-coordinate of the destination point.
   *   - y: The y-coordinate of the destination point.
   *   - width: The width of the destination area.
   *   - height: The height of the destination area.
   * @param array $src_rect
   *   (optional) The area of the source image to copy. Defaults to the entire
   *   image.
   *
   *   Requires the following keys:
   *   - x: The x-coordinate of the source point.
   *   - y: The y-coordinate of the source point.
   *   - width: The width of the source area.
   *   - height: The height of the source area.
   */
  protected static function copyResampledImage($dst, $src, $dst_rect, $src_rect = array()) {
    $width = imagesx($src);
    $height = imagesy($src);
    
    // Copy the entire source image if a rectangle wasn't provided.
    $src_rect += array(
      'x' => 0,
      'y' => 0,
      'width' => $width,
      'height' => $height
    );
    
    imagecopyresampled($dst, $src,
      $dst_rect['x'], $dst_rect['y'], $src_rect['x'], $src_rect['y'],
      $dst_rect['width'], $dst_rect['height'], $src_rect['width'], $src_rect['height']);
  }
  
  /**
   * Creates a blank PNG image of the specified size for use with GD functions.
   * 
   * Remember to destroy this resource when you're done!
   *
   * @param int $width
   *   The desired image width.
   * @param int $height
   *   The desired image height.
   *
   * @return resource
   *   A GD image handle.
   */
  protected static function createImage($width, $height) {
    $res = imagecreatetruecolor($width, $height);
    
    // PNG images only!
    imagealphablending($res, FALSE);
    $transparent = imagecolorallocatealpha($res, 0, 0, 0, 127);
    imagefill($res, 0, 0, $transparent);
    imagealphablending($res, TRUE);
    imagesavealpha($res, TRUE);
    
    return $res;
  }
  
  /**
   * Corrects base images with transparency issues that cause color overlays
   * to fail in spectacular fashion.
   *
   * @param string $filename
   *   The location of the base image to fix.
   */
  protected static function fixTransparency($filename) {
    $source = imagecreatefrompng($filename);
    
    // Quit if the image has no transparent color.
    if (imagecolortransparent($source) == -1) {
      imagedestroy($source);
      return;
    }
    
    $width = imagesx($source);
    $height = imagesy($source);
    
    // Copy the image onto a new transparent image of the same size.
    $target = self::createImage($width, $height);
    imagecopy($target, $source, 0, 0, 0, 0, $width, $height);
    
    // Overwrite the existing image.
    self::saveImage($target, $filename);
    
    // Clean up any resources.
    imagedestroy($source);
    imagedestroy($target);
  }
  
  /**
   * Checks if the emblem render directory exists and is writable.
   *
   * @param string $path
   *   The render directory (appended to the default file path).
   *
   * @return mixed
   *   The destination file path, or FALSE if there was a problem setting up the
   *   directory.
   */
  protected static function prepareRenderPath($path) {
    $destination = file_default_scheme() . '://blizzardapi/' . $path;
    $destination = file_stream_wrapper_uri_normalize($destination);
    
    $dirname = drupal_dirname($destination);
    if (!file_prepare_directory($dirname, FILE_CREATE_DIRECTORY)) {
      watchdog('blizzardapi', 'The directory %directory does not exist or is not writable.',
        array('%directory' => $dirname, WATCHDOG_ERROR));
      return FALSE;
    }
    
    return $destination;
  }
  
  /**
   * Saves an image to the specified destination (needed for stream wrapper support).
   *
   * This method is a modified version of image_gd_save().
   *
   * @param resource $res
   *   A GD image handle.
   * @param string $destination
   *   A file URI or path where the image should be saved.
   *
   * @return bool
   *   The result of the save operation, either TRUE or FALSE.
   */
  protected static function saveImage($res, $destination) {
    $scheme = file_uri_scheme($destination);
    if ($scheme && file_stream_wrapper_valid_scheme($scheme)) {
      $local_wrappers = file_get_stream_wrappers(STREAM_WRAPPERS_LOCAL);
      if (!isset($local_wrappers[$scheme])) {
        $permanent_destination = $destination;
        $destination = drupal_tempnam('temporary://', 'gd_');
      }
      $destination = drupal_realpath($destination);
    }
    
    imagealphablending($res, FALSE);
    imagesavealpha($res, TRUE);
    $success = imagepng($res, $destination);
    
    if (isset($permanent_destination) && $success) {
      return (bool) file_unmanaged_move($destination, $permanent_destination, FILE_EXISTS_REPLACE);
    }
    return $success;
  }
  
  /**
   * Converts a hexadecimal color string into its aRGB components.
   */
  protected static function unpackColor($hex, $normalize = FALSE) {
    $rgb = array();
    
    if (strlen($hex) == 8) {
      // Split the segments.
      $alpha = substr($hex, 0, 2);
      $hex = substr($hex, 2);
      
      // GD functions expect a 7-bit alpha (0-127).
      $rgb['a'] = abs(hexdec($alpha) - 255) >> 1;
    }
    else {
      $rgb['a'] = 0;
    }
    
    // This is borrowed from _color_unpack().
    $c = hexdec($hex);
    for ($i = 16; $i >= 0; $i -= 8) {
      $out[] = (($c >> $i) & 0xFF) / ($normalize ? 255 : 1);
    }
    
    $rgb += array(
      'r' => $out[0],
      'g' => $out[1],
      'b' => $out[2]
    );
    
    return $rgb;
  }
}

/**
 * A World of Warcraft guild tabard renderer.
 */
class BlizzardApiWowTabard extends BlizzardApiEmblem {
  /**
   * The generated tabard's maximum size: 240 pixels.
   */
  const TABARD_MAX_SIZE = 240;
  /**
   * The generated tabard's minimum size: 16 pixels.
   */
  const TABARD_MIN_SIZE = 16;
  
  protected static $backgroundColorMap = array(
    array(215,32,112),
    array(171,0,76),
    array(87,0,0),
    array(225,105,26),
    array(180,56,0),
    array(133,11,0),
    array(237,151,22),
    array(205,110,0),
    array(155,61,0),
    array(239,207,20),
    array(207,162,0),
    array(158,113,0),
    array(226,216,20),
    array(183,177,0),
    array(133,128,0),
    array(206,209,24),
    array(159,161,3),
    array(112,115,0),
    array(153,206,27),
    array(108,154,3),
    array(65,108,0),
    array(30,210,96),
    array(4,157,63),
    array(0,110,11),
    array(29,206,169),
    array(4,152,122),
    array(0,107,74),
    array(33,177,214),
    array(3,109,139),
    array(0,81,111),
    array(72,125,193),
    array(38,85,145),
    array(0,39,98),
    array(188,75,195),
    array(145,42,155),
    array(108,8,128),
    array(202,17,191),
    array(173,0,162),
    array(124,0,116),
    array(219,30,160),
    array(149,0,97),
    array(121,0,68),
    array(160,108,44),
    array(108,66,15),
    array(53,16,0),
    array(15,26,31),
    array(117,124,120),
    array(136,145,139),
    array(156,166,159),
    array(211,211,198),
    array(229,107,140)
  );
  
  protected static $borderColorMap = array(
    array(97,42,44),
    array(109,69,46),
    array(119,101,36),
    array(118,114,36),
    array(108,118,36),
    array(85,108,48),
    array(76,109,48),
    array(48,108,66),
    array(48,105,107),
    array(48,80,108),
    array(55,60,100),
    array(87,54,100),
    array(100,55,76),
    array(103,51,53),
    array(153,159,149),
    array(38,46,38),
    array(155,94,28)
  );
  
  protected static $iconColorMap = array(
    array(102,0,32),
    array(103,35,0),
    array(103,69,0),
    array(103,86,0),
    array(98,102,0),
    array(80,102,0),
    array(54,102,0),
    array(0,102,30),
    array(0,102,86),
    array(0,72,102),
    array(9,42,94),
    array(86,9,94),
    array(93,10,79),
    array(84,54,10),
    array(177,183,176),
    array(16,20,22),
    array(221,163,90)
  );
  
  /**
   * Determines the destination path for a rendered tabard image.
   *
   * Note: You should use file_exists() to determine if the file named in this
   * path actually exists.
   *
   * @param array $guild
   *   (optional) Information on the owner of a tabard. If not provided, the path
   *   of a default tabard image will be returned. Requires the following keys:
   *   - name: The guild name. If transliteration is available, it will be used
   *     to generate a prettier file name.
   *   - realm: The name, or slug, of the realm that the guild is on (used to
   *     generate a unique tabard path). You must also specify a valid
   *     Battle.net locale, using the $options parameter, for this realm.
   * @param array $options
   *   (optional) An array containing any combination of the following keys:
   *   - region: The Battle.net region to request data from. Defaults to 'us'.
   *   - language: The language to request data in. Defaults to 'en'.
   *
   * @return mixed
   *   The full path of the tabard image, or FALSE if there was a problem
   *   setting up the directory.
   */
  public static function getRenderPath($guild = array(), $options = array()) {
    if (empty($guild)) {
      return self::prepareRenderPath('tabards/render/default.png');
    }
    
    $options += array('region' => BlizzardApi::REGION_US, 'language' => BlizzardApi::LANGUAGE_DEFAULT);
    $slug = blizzardapi_realm_slug($guild['realm'], $options, TRUE);
    if (empty($slug)) {
      if ($slug == '') {
        watchdog('blizzardapi', 'Cannot determine tabard destination path: realm not found (%region: %name).',
          array('%region' => $options['region'], '%name' => $guild['realm']), WATCHDOG_DEBUG);
      }
      return FALSE;
    }
    
    // Convert the guild name into a safe file name.
    if (function_exists('transliteration_clean_filename')) {
      $name = transliteration_clean_filename($guild['name'], language_default('language'));
    }
    else {
      $name = drupal_hash_base64($guild['name']);
    }
    
    return self::prepareRenderPath("tabards/render/$slug/$name.png");
  }
  
  /**
   * Renders a World of Warcraft guild tabard image.
   *
   * If the tabard image already exists, it will be overwritten.
   *
   * @param array $tabard
   *   (optional) An array containing tabard creation information (found in
   *   character and guild profiles). If empty, a blank default tabard will be
   *   created.
   *
   *   The following keys are required:
   *   - name: The guild name.
   *   - realm: The name, or slug, of the realm that the guild is on (used to
   *     generate a unique tabard path). You must also specify a valid
   *     Battle.net locale, using the $options parameter, for this realm.
   *   - side: The guild faction ID (0: Alliance, 1: Horde).
   *   - emblem: An array containing the following emblem information.
   *     - icon
   *     - iconColor
   *     - border
   *     - borderColor
   *     - backgroundColor
   * @param int $size
   *   (optional) The size of the rendered tabard image. This value must be
   *   within the TABARD_MIN_SIZE and TABARD_MAX_SIZE range.
   * @param array $options
   *   (optional) Tabard rendering options.
   *   - region: The Battle.net region to request data from. Defaults to 'us'.
   *   - language: The language to request data in. Defaults to 'en'.
   *   - colorize: Uses preset emblem colors to "colorize" the tabard like the
   *     generated Battle.net armory images. This must be an array, keyed by
   *     layer, containing color indexes.
   *
   * @return mixed
   *   The path of the generated tabard image, or FALSE if there was a problem
   *   creating the image.
   */
  public static function render($tabard, $size = self::TABARD_MAX_SIZE, $options = array()) {
    if ($size < self::TABARD_MIN_SIZE) {
      $size = self::TABARD_MIN_SIZE;
    }
    elseif ($size > self::TABARD_MAX_SIZE) {
      $size = self::TABARD_MAX_SIZE;
    }
    
    $options += array(
      'region' => BlizzardApi::REGION_US,
      'language' => BlizzardApi::LANGUAGE_DEFAULT
    );
    
    // If Drupal's GD image toolkit is available, then so is the GD library.
    $toolkits = image_get_available_toolkits();
    if (!isset($toolkits['gd'])) {
      drupal_set_message(t('Cannot create tabard image: GD library missing'), 'error');
      return FALSE;
    }
    
    $images = array(
      'ring' => array(
        'file' => 'ring-alliance.png',
        'position' => array('x' => 0, 'y' => 0)
      ),
      'shadow' => array(
        'file' => 'shadow_00.png',
        'position' => array('x' => 18, 'y' => 27, 'height' => 216)
      ),
      'background' => array(
        'file' => 'bg_00.png',
        'position' => array('x' => 18, 'y' => 27),
        'map' => self::$backgroundColorMap
      ),
      'overlay' => array(
        'file' => 'overlay_00.png',
        'position' => array('x' => 18, 'y' => 27)
      ),
      'border' => array(
        'file' => '',
        'position' => array('x' => 31, 'y' => 40),
        'map' => self::$borderColorMap
      ),
      'icon' => array(
        'file' => '',
        'position' => array('x' => 33, 'y' => 57),
        'map' => self::$iconColorMap
      ),
      'hooks' => array(
        'file' => 'hooks.png',
        'position' => array('x' => 18, 'y' => 27)
      )
    );
    
    if (isset($tabard['emblem'])) {
      $images['background']['color'] = $tabard['emblem']['backgroundColor'];
      $images['border']['color'] = $tabard['emblem']['borderColor'];
      $images['icon']['color'] = $tabard['emblem']['iconColor'];
      
      if ($tabard['side'] == 1) {
        $images['ring']['file'] = 'ring-horde.png';
      }
      
      // If a tabard has never been customized, the API will return -1 for these
      // values. In that case, fallback to the default tabard.
      if ($tabard['emblem']['icon'] < 0 || $tabard['emblem']['border'] < 0) {
        unset($tabard['emblem']);
      }
      else {
        $images['border']['file'] = sprintf('border_%02d.png', $tabard['emblem']['border']);
        $images['icon']['file'] = sprintf('emblem_%02d.png', $tabard['emblem']['icon']);
      }
    }
    
    // Generate a default tabard without a border or icon.
    if (empty($tabard['emblem'])) {
      unset($images['border'], $images['icon']);
    }
    
    $target = self::createImage($size, $size);
    
    foreach ($images as $layer => $info) {
      $source = self::retrieveBaseImage($info['file'], $options['region']);
      if ($source == FALSE) {
        imagedestroy($target);
        return FALSE;
      }
      
      $source = imagecreatefrompng($source);
      $s_width = isset($info['position']['width']) ? $info['position']['width'] : imagesx($source);
      $s_height = isset($info['position']['height']) ? $info['position']['height'] : imagesy($source);
      
      $dst_rect = array(
        'x' => $info['position']['x'] * $size / self::TABARD_MAX_SIZE,
        'y' => $info['position']['y'] * $size / self::TABARD_MAX_SIZE,
        'width' => $s_width * $size / self::TABARD_MAX_SIZE,
        'height' => $s_height * $size / self::TABARD_MAX_SIZE
      );
      
      if (isset($info['color'])) {
        if (isset($options['colorize'])) {
          $color = $info['map'][$options['colorize'][$layer]];
          $color = array('r' => $color[0], 'g' => $color[1], 'b' => $color[2], 'a' => 0);
          self::colorizeImage($source, $color);
        }
        else {
          self::overlayImage($source, $info['color']);
        }
      }
      
      self::copyResampledImage($target, $source, $dst_rect);
      imagedestroy($source);
    }
    
    $destination = self::getRenderPath($tabard, $options);
    if (empty($destination)) {
      return FALSE;
    }
    
    $success = self::saveImage($target, $destination);
    imagedestroy($target);
    
    return $success ? $destination : FALSE;
  }
  
  /**
   * Gets the path to a tabard base image. If there is no local copy of the
   * image, it will be downloaded from the specified region's Battle.net site.
   *
   * @param string $filename
   *   The name of the tabard base image.
   * @param string $region
   *   (optional) The Battle.net region to download the image from.
   *   Valid regions include:
   *   - us: Americas (default)
   *   - eu: Europe
   *   - kr: Korea
   *   - tw: Taiwan
   *   - cn: China
   *
   * @return string
   *   The location that the image was saved to, or FALSE if there was a problem
   *   retrieving the file.
   */
  protected static function retrieveBaseImage($filename, $region = BlizzardApi::REGION_US) {
    $destination = self::prepareRenderPath('tabards/base/' . drupal_basename($filename));
    if ($destination && file_exists($destination)) {
      return $destination;
    }
    
    $region = BlizzardApiWow::isSupportedRegion($region) ? $region : BlizzardApi::REGION_US;
    $host = ($region == BlizzardApi::REGION_CN) ? BlizzardApi::URL_HOST_CHINA : BlizzardApi::URL_HOST;
    $url = 'http:' . $host . BlizzardApiWow::STATIC_IMAGE_PATH . '/guild/tabards/' . $filename;
    $url = str_replace('{region}', $region, $url);
    
    $image = system_retrieve_file($url, $destination, FALSE, FILE_EXISTS_ERROR);
    if ($image !== FALSE) {
      self::fixTransparency($image);
    }
    
    return $image;
  }
}
