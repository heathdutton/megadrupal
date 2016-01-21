<?php


/**
 * Wildcard loader for ../%imagene_image/..
 * called from imagene_image_load()
 */
function _imagene_image_load($file, $key) {
  if (preg_match('/^(.*)\.png$/', $file, $m)) {
    list(,$slug) = $m;
    switch ($key) {
      case 'gradient':
      case 'gradient-y':
        $vertical = TRUE;
      case 'gradient-x':
        $im = _imagene_gradient_load($slug, $vertical && TRUE);
        break;
      case 'gradient-xy':
        $im = _imagene_gradient_xy_load($slug);
    }
    if ($im) {
      $filename = 'public://imagene/'. $key .'/'. $file;
      return (object)array(
        'image' => $im,
        'file' => $filename,
      );
    }
  }

  return FALSE;
}


/**
 * Helper function for the imagene_image_load() wildcard loader,
 * for one-dimensional gradient images
 * controlled by one-dimensional Bezier grids.
 */
function _imagene_gradient_load($slug, $vertical = TRUE) {
  if (preg_match('/^([1-9]\d*)-([0-9a-f\-]+)$/', strtolower($slug), $m)) {
    list(,$dim,$hex_colors_slug) = $m;
    $dim = (int)$dim;
    // discard trailing *.png and friends.
    $hex_colors = explode('-', $hex_colors_slug);
    $colors = array();
    $hasAlpha = FALSE;
    foreach ($hex_colors as $hex) {
      $rgba = imagene_color_hex_to_rgba($hex, $hasAlpha);
      if ($rgba) {
        $colors[] = $rgba;
      }
    }
    if (!empty($colors)) {
      if ($vertical) {
        return new imagene_GradientImage_y($dim, $colors, $hasAlpha);
      }
      else {
        return new imagene_GradientImage_x($dim, $colors, $hasAlpha);
      }
    }
  }
  return FALSE;
}


/**
 * Helper function for the imagene_image_load() wildcard loader,
 * for two-dimensional gradient images
 * controlled by two-dimensional Bezier grids.
 */
function _imagene_gradient_xy_load($slug) {
  if (preg_match('/^([1-9]\d*)x([1-9]\d*)--([0-9a-f\-]+)(|-(-[tbrl]+_(x|[0-9a-f]+))+)$/', strtolower($slug), $m)) {
    list(,$w,$h,$hex_colors_slug,$extend_slug) = $m;
    $w = (int)$w;
    $h = (int)$h;
    // discard trailing *.png and friends.
    $hex_colors = explode('-', $hex_colors_slug);
    $colors = array();
    $hasAlpha = FALSE;
    $i=0; $j=0; $iMax=0;
    foreach ($hex_colors as $hex) {
      if (!strlen($hex)) {
        $iMax = max($iMax, $i-1);
        ++$j; $i=0;
        continue;
      }
      $rgba = imagene_color_hex_to_rgba($hex, $hasAlpha);
      if ($rgba) {
        $colors[$i][$j] = $rgba;
      }
      ++$i;
    }
    $iMax = max($iMax, $i-1);
    $jMax = $j;
    for ($i=0; $i<=$iMax; ++$i) {
      for ($j=0; $j<=$jMax; ++$j) {
        if (!isset($colors[$i][$j])) {
          $colors[$i][$j] = array(0,0,0,0);
        }
      }
    }

    if ($extend_slug) {
      $extend = explode('-', $extend_slug);
      array_shift($extend);
      array_shift($extend);
      _imagene_grid_extend($colors, $extend, $iMax, $jMax, $hasAlpha);
    }

    if (!empty($colors)) {
      return new imagene_GradientImage_xy($w, $h, $iMax, $jMax, $colors, $hasAlpha);
    }
  }
  return FALSE;
}


function _imagene_grid_extend(&$grid, $extend, &$iMax, &$jMax, &$hasAlpha) {

  foreach ($extend as $loc_hex) {
    list($loc,$hex) = explode('_', $loc_hex);
    if ($hex !== 'x') {
      $rgba = imagene_color_hex_to_rgba($hex, $hasAlpha);
      for ($i=0; $i<strlen($loc); ++$i) {
        switch ($loc{$i}) {
          case 't':
            foreach ($grid as $iRow => $row) {
              array_unshift($grid[$iRow], $rgba);
            }
            ++$jMax;
            break;

          case 'b':
            foreach ($grid as $iRow => $row) {
              $grid[$iRow][] = $rgba;
            }
            ++$jMax;
            break;

          case 'l':
            $row = array_fill(0, $jMax+1, $rgba);
            array_unshift($grid, $row);
            ++$iMax;
            break;

          case 'r':
            $row = array_fill(0, $jMax+1, $rgba);
            $grid[] = $row;
            ++$iMax;
            break;
        }
      }
    }
    else {
      for ($i=0; $i<strlen($loc); ++$i) {
        switch ($loc{$i}) {
          case 't':
            foreach ($grid as $iRow => $row) {
              array_unshift($grid[$iRow], $grid[$iRow][0]);
            }
            ++$jMax;
            break;

          case 'b':
            foreach ($grid as $iRow => $row) {
              $grid[$iRow][] = $grid[$iRow][$jMax];
            }
            ++$jMax;
            break;

          case 'l':
            array_unshift($grid, $grid[0]);
            ++$iMax;
            break;

          case 'r':
            $grid[] = $grid[$iMax];
            ++$iMax;
            break;
        }
      }
      
    }
  }
}


/**
 * convert a hex color string to an rgb array
 */
function imagene_color_hex_to_rgba($hex, &$hasAlpha = NULL) {
  $hex = strtolower($hex);
  if (preg_match('/^[0-9a-f]+$/', $hex)) {
    switch (strlen($hex)) {
    
      case 8:
        $hasAlpha = TRUE;
      case 6:
        $rgba = array();
        foreach(str_split($hex, 2) as $i => $fragment) {
          $rgba[$i] = hexdec($fragment);
        }
        if (!isset($rgba[3])) $rgba[3] = 0;
        break;
        
      case 4:
        $hasAlpha = TRUE;
      case 3:
        $rgba = array();
        foreach (str_split($hex, 1) as $i => $fragment) {
          $rgba[$i] = hexdec($fragment.$fragment);
        }
        if (!isset($rgba[3])) $rgba[3] = 0;
        break;
      
      case 2:
        $v = hexdec($hex);
      case 1:
        $v = hexdec($hex.$hex);
        $rgba = array($v,$v,$v,0);
        break;
        
      default:
        return;
    }

    return $rgba;
  }
}
