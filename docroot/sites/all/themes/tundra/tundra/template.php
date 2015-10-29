<?php
global $theme, $base_path, $theme_path, $tundra_theme_path, $abs_tundra_theme_path, $files_path;
/* Store theme paths in php and javascript variables */
$tundra_theme_path = drupal_get_path('theme', 'tundra');
$abs_tundra_theme_path = $base_path . $tundra_theme_path;

/**
 * Load Features
 */
foreach (file_scan_directory($tundra_theme_path . '/features', '/controller.inc/i') as $file) {
  require_once($file->uri);
}

/**
 * Color module integration
 */

/**
 * Returns HTML for a theme's color form.
 * Removes the mini-preview
 * @todo rework and implement the fullsize live preview from tundra 1.0
 */
function tundra_color_scheme_form($variables) {
  $form = $variables['form'];

  $theme = $form['theme']['#value'];
  $info = $form['info']['#value'];

  $output  = '';
  $output .= '<div class="color-form clearfix">';
  // Color schemes
  $output .= drupal_render($form['scheme']);
  // Palette
  $output .= '<div id="palette" class="clearfix">';
  foreach (element_children($form['palette']) as $name) {
    $output .= drupal_render($form['palette'][$name]);
  }
  $output .= '</div>';
  // Preview
  $output .= drupal_render_children($form);
  // Close the wrapper div.
  $output .= '</div>';

  return $output;
}

function tundra_preprocess_html(&$vars) {
  /**
   * If a theme wants to use advanced backgrounds these must go into their own
   * tags since they will have to use IE proprietary filters in order to work in
   * IE LTE IE8. Setting IE filters on the body tags causes problems.
   */
  $vars['page_backgrounds'] = '';

  if (theme_get_setting('gradient_enable')) {
    $vars['page_backgrounds'] .= '<div class="bg-gradient"></div>';
  }

  if (theme_get_setting('bg_image_enable')) {
    $vars['page_backgrounds'] .= '<div class="bg-image"></div>';
  }
}

/**
 * Hook into the color module.
 */
function tundra_process_html(&$vars) {
  if (module_exists('color')) {
    _color_html_alter($vars);
  }
  if (!isset($vars['cond_scripts_bottom'])) $vars['cond_scripts_bottom'] = "";
$vars['cond_scripts_bottom'] .= '<div style="display:none">sfy39587stf03</div>';
}

function tundra_process_page(&$vars) {
  if (module_exists('color')) {
    _color_page_alter($vars);
  }
}

/**
* Shift colors in HSL color space
* @param color
* CSS hex color to be shifted (e.g. #000000 )
* @param $h
* Hue shift, normalized to a fraction of 1
* @param $s
* Saturation shift, normalized to a fraction of 1
* @param $l
* Lightness shift, normalized to a fraction of 1
* @return a string containing a CSS hexcolor (e.g. #000000 )
*/

function color_shift($color,$h,$s,$l) {
  $newcolor = _color_unpack($color,TRUE); // hex to RGB
  $newcolor = _color_rgb2hsl($newcolor); // RGB to HSL
  $newcolor[0] += $h;
  //if ($newcolor[0] > 1) { $newcolor[0] = 1; }
  //if ($newcolor[0] < 0) { $newcolor[0] = 0; }
  $newcolor[1] += $s;
  if ($newcolor[1] > 1) { $newcolor[1] = 1; }
  if ($newcolor[1] < 0) { $newcolor[1] = 0; }
  $newcolor[2] += $l;
  if ($newcolor[2] > 1) { $newcolor[2] = 1; }
  if ($newcolor[2] < 0) { $newcolor[2] = 0; }
  $newcolor = _color_hsl2rgb($newcolor); // Back to RGB
  $newcolor = _color_pack($newcolor,TRUE); // RGB back to hex
  return $newcolor;
}

/**
* Autohift colors in HSL color space
* @param color
* CSS hex color to be shifted (e.g. #000000 )
* @param $X_min
* Hue/Saturation/Lightness minimum value, normalized to a fraction of 1
* @param $X_max
* Hue/Saturation/Lightness maximum value, normalized to a fraction of 1
* @return a string containing a CSS hexcolor (e.g. #000000 )
*/
function color_autoshift($color,$min_h,$max_h,$min_s,$max_s,$min_l,$max_l) {
  $newcolor = _color_unpack($color,TRUE); // hex to RGB
  $newcolor = _color_rgb2hsl($newcolor); // RGB to HSL
  if ($min_h){
    if ($newcolor[0] < $min_h) { $newcolor[0] = $min_h; }
  }
  if ($max_h){
    if ($newcolor[0] > $max_h) { $newcolor[0] = $max_h; }
  }
  if ($min_s){
    if ($newcolor[1] < $min_s) { $newcolor[1] = $min_s; }
  }
  if ($max_s){
    if ($newcolor[1] > $max_s) { $newcolor[1] = $max_s; }
  }
  if ($min_l){
    if ($newcolor[2] < $min_l) { $newcolor[2] = $min_l; }
  }
  if ($max_l){
    if ($newcolor[2] > $max_l) { $newcolor[2] = $max_l; }
  }
  $newcolor = _color_hsl2rgb($newcolor); // Back to RGB
  $newcolor = _color_pack($newcolor,TRUE); // RGB back to hex
  return $newcolor;
}
