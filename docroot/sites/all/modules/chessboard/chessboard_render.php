<?php

/**
 * @file
 * Theme related functions for the Chessboard Renderer module.
 */

/*================================*/
/* Configuration                  */
/*================================*/

function chessboard_image_path($path) {
  global $chessboard_image_path;
  $chessboard_image_path = $path;
}


/*================================*/
/* Routines                       */
/*================================*/

function chessboard_border_filename($border, $chessboard_image_path = NULL) {
  // $border: T, B, L, R, TL, TR, BL, BR
  if (!isset($chessboard_image_path)) {
    $chessboard_image_path = $GLOBALS['chessboard_image_path'];
  }
  switch ($border) {
    case 'T':
    case 'B':
      return $chessboard_image_path . 'h.png';
    case 'L':
    case 'R':
      return $chessboard_image_path . 'v.png';
    default:
      return $chessboard_image_path . 'c.png';
  }
}


function chessboard_piece_filename($piece, $square_color, $chessboard_image_path = NULL) {
  // $piece        : KQBNRPkqbnrp-
  // $square_color : 0 1
  if (!isset($chessboard_image_path)) {
    $chessboard_image_path = $GLOBALS['chessboard_image_path'];
  }
  switch ($piece) {
    case 'x':
      $name = 'xx';
      break;
    case '-':
      $name = '';
      break;
    default:
      $name = strtolower($piece) . (ctype_lower($piece) ? 'd' : 'l');
      break;
  }

  return $chessboard_image_path . $name . ($square_color ? 'd' : 'l') . '.png';
}

function chessboard_render($content) {
  global $chessboard_image_path;

  if (!isset($chessboard_image_path)) {
    $image_directory_path = NULL;
  }
  elseif (strpos($chessboard_image_path, base_path()) === 0) {
    $image_directory_path = trim(substr($chessboard_image_path, strlen(base_path())), '\\/');
  }
  else {
    $image_directory_path = trim($chessboard_image_path, '\\/');
  }
  $item = array(
    'value' => $content,
  );
  $node = NULL;
  $field = array();
  $items = array($item);
  chessboard_field('sanitize', $node, $field, $items, FALSE, FALSE);
  $element = array(
    '#theme' => 'chessboard',
    '#item' => $items[0],
    '#weight' => 0,
    '#image_directory_path' => $image_directory_path,
  );
  $output = drupal_render($element);
  return $output;
}

/**
 * Formats Chessboard Renderer notation syntax in HTML.
 *
 * @param $element
 *    An associative array containing items (chessboards) as children keyed by delta.
 *
 * @ingroup themable
 */
function theme_chessboard_formatter_chessboard_default($variables) {
  $element = $variables['element'];
  $items = array();
  foreach (element_children($element) as $key) {
    $items[$key] = $element[$key]['#item'];
  }
  $element = _chessboard_formatter_view($items);
  $output = drupal_render($element);
  return $output;
}

function template_preprocess_chessboard(&$variables) {
  static $tables_xhtml = array();

  if (!isset($variables['element']['#image_directory_path'])) {
    $variables['element']['#image_directory_path'] = drupal_get_path('module', 'chessboard') . '/default';
  }
  $image_directory_path = $variables['element']['#image_directory_path'];
  if (!isset($tables_xhtml[$image_directory_path])) {
    $groups = array('square', 'border-h', 'border-v', 'border-c');
    $test_paths = array(
      'square' => chessboard_piece_filename('-', 0, $image_directory_path . '/'),
      'border-h' => chessboard_border_filename('T', $image_directory_path . '/'),
      'border-v' => chessboard_border_filename('R', $image_directory_path . '/'),
      'border-c' => chessboard_border_filename('TR', $image_directory_path . '/'),
    );
    $class_attributes = array(
      'square' => array(
        'class' => 'chessboard-square',
      ),
      'border-h' => array(
        'class' => 'chessboard-border-h',
      ),
      'border-v' => array(
        'class' => 'chessboard-border-v',
      ),
      'border-c' => array(
        'class' => 'chessboard-border-c',
      ),
    );
    $default_image_directory_path = drupal_get_path('module', 'chessboard') . '/default';
    if ($image_directory_path === $default_image_directory_path) {
      $size_attributes = array(
        'square' => array(
          'width' => 40,
          'height' => 40,
        ),
        'border-h' => array(
          'width' => 40,
          'height' => 4,
        ),
        'border-v' => array(
          'width' => 4,
          'height' => 40,
        ),
        'border-c' => array(
          'width' => 4,
          'height' => 4,
        ),
      );
    }
    foreach ($test_paths as $group => $test_path) {
      if (!isset($size_attributes[$group]) && is_file($test_path) && (list($width, $height) = @getimagesize($test_path))) {
        $size_attributes[$group] = array(
          'width' => $width,
          'height' => $height,
        );
      }
    }

    foreach ($groups as $group) {
      if (!isset($size_attributes[$group])) {
        $size_attributes[$group] = array();
      }
      if (!isset($class_attributes[$group])) {
        $class_attributes[$group] = array();
      }
    }

    // Construct the translation table

    // Empty and marked squares
    $table_xhtml = array(
      '-0' => theme('image', array('path' => chessboard_piece_filename('-', 0, $image_directory_path . '/'), 'width' => $size_attributes['square']['width'], 'height' => $size_attributes['square']['height'], 'alt' => '-', 'title' => '', 'attributes' => $class_attributes['square'])),
      '-1' => theme('image', array('path' => chessboard_piece_filename('-', 1, $image_directory_path . '/'), 'width' => $size_attributes['square']['width'], 'height' => $size_attributes['square']['height'], 'alt' => '-',  'title' => '', 'attributes' => $class_attributes['square'])),
      'x0' => theme('image', array('path' => chessboard_piece_filename('x', 0, $image_directory_path . '/'), 'width' => $size_attributes['square']['width'], 'height' => $size_attributes['square']['height'], 'alt' => 'x', 'title' => '', 'attributes' => $class_attributes['square'])),
      'x1' => theme('image', array('path' => chessboard_piece_filename('x', 1, $image_directory_path . '/'), 'width' => $size_attributes['square']['width'], 'height' => $size_attributes['square']['height'], 'alt' => 'x', 'title' => '', 'attributes' => $class_attributes['square'])),
    );

    // Pieces
    $pieces = 'kqbnrp';
    for ($i=0; $i<6; $i++) {
      $piece = $pieces[$i];
      $table_xhtml += array(
        strtoupper($piece) . '0' => theme('image', array('path' => chessboard_piece_filename(strtoupper($piece), 0, $image_directory_path . '/'), 'width' => $size_attributes['square']['width'], 'height' => $size_attributes['square']['height'], 'alt' => strtoupper($piece), 'title' => '', 'attributes' => $class_attributes['square'])),
        strtoupper($piece) . '1' => theme('image', array('path' => chessboard_piece_filename(strtoupper($piece), 1, $image_directory_path . '/'), 'width' => $size_attributes['square']['width'], 'height' => $size_attributes['square']['height'], 'alt' => strtoupper($piece), 'title' => '', 'attributes' => $class_attributes['square'])),
        $piece . '0' => theme('image', array('path' => chessboard_piece_filename($piece, 0, $image_directory_path . '/'), 'width' => $size_attributes['square']['width'], 'height' => $size_attributes['square']['height'], 'alt' => $piece, 'title' => '', 'attributes' => $class_attributes['square'])),
        $piece . '1' => theme('image', array('path' => chessboard_piece_filename($piece, 1, $image_directory_path . '/'), 'width' => $size_attributes['square']['width'], 'height' => $size_attributes['square']['height'], 'alt' => $piece, 'title' => '', 'attributes' => $class_attributes['square'])),
      );
    }

    // Borders
    $table_xhtml += array(
      'T' => theme('image', array('path' => chessboard_border_filename('T', $image_directory_path . '/'), 'width' => $size_attributes['border-h']['width'], 'height' => $size_attributes['border-h']['height'], 'title' => '', 'attributes' => $class_attributes['border-h'])),
      'B' => theme('image', array('path' => chessboard_border_filename('B', $image_directory_path . '/'), 'width' => $size_attributes['border-h']['width'], 'height' => $size_attributes['border-h']['height'], 'title' => '', 'attributes' => $class_attributes['border-h'])),
      'L' => theme('image', array('path' => chessboard_border_filename('L', $image_directory_path . '/'), 'width' => $size_attributes['border-v']['width'], 'height' => $size_attributes['border-v']['height'], 'title' => '', 'attributes' => $class_attributes['border-v'])),
      'R' => theme('image', array('path' => chessboard_border_filename('R', $image_directory_path . '/'), 'width' => $size_attributes['border-v']['width'], 'height' => $size_attributes['border-v']['height'], 'title' => '', 'attributes' => $class_attributes['border-v'])),
      'TL' => theme('image', array('path' => chessboard_border_filename('TL', $image_directory_path . '/'), 'width' => $size_attributes['border-c']['width'], 'height' => $size_attributes['border-c']['height'], 'title' => '', 'attributes' => $class_attributes['border-c'])),
      'TR' => theme('image', array('path' => chessboard_border_filename('TR', $image_directory_path . '/'), 'width' => $size_attributes['border-c']['width'], 'height' => $size_attributes['border-c']['height'], 'title' => '', 'attributes' => $class_attributes['border-c'])),
      'BL' => theme('image', array('path' => chessboard_border_filename('BL', $image_directory_path . '/'), 'width' => $size_attributes['border-c']['width'], 'height' => $size_attributes['border-c']['height'], 'title' => '', 'attributes' => $class_attributes['border-c'])),
      'BR' => theme('image', array('path' => chessboard_border_filename('BR', $image_directory_path . '/'), 'width' => $size_attributes['border-c']['width'], 'height' => $size_attributes['border-c']['height'], 'title' => '', 'attributes' => $class_attributes['border-c'])),
    );
    $tables_xhtml[$image_directory_path] = $table_xhtml;
  }
  $variables['table_xhtml'] = $tables_xhtml[$image_directory_path];
  // For convenience.
  $variables['file_max'] = $variables['element']['#item']['file_max'];
  $variables['square_color_first'] = $variables['element']['#item']['square_color_first'];
  $variables['border'] = $variables['element']['#item']['border'];
  $variables['board'] = $variables['element']['#item']['board'];
}
