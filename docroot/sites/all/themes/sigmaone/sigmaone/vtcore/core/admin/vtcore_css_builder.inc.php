<?php
/**
 * @file
 * This file is for building dynamic.css based
 * on information found in the $form_state['values']['collective_css']
 * and combine it with hook_vtcore_css_rules_alter_process().
 *
 * If any vtcore plugin wish to use function found
 * in this file, they should include the file manually
 * because this file is not autoloaded by vtcore.
 */

/**
 * Function for collecting all information
 * about css that needed to be written
 * to custom.css
 *
 * @param $css
 *   array of collective css translation
 */
function vtcore_css_register(&$css) {
  // Give chances for all dependent module
  // to give out a rule to process their
  // specific css
  $processor = array();

  vtcore_alter_process('vtcore_css_rules', $css, $processor);

  // loop and build the combined css
  $output = '';
  foreach($css as $csskey => $cssvalue) {
    if (!isset($processor[$csskey])) {
      continue;
    }
    $output .= $processor[$csskey];
  }

  return $output;
}

/**
 * function to build custom css
 */
function vtcore_css_create(&$form_state) {
  global $vtcore;

  // Grab all the stored css and save it as a file

  if (!empty($form_state['values']['collective_css'])) {
    $css = $form_state['values']['collective_css'];

    $output = vtcore_css_register($css);

    $path = $vtcore->theme_path . '/css/dynamic.css';

    if (!empty($output)) {
      if (file_unmanaged_save_data($output, $path, FILE_EXISTS_REPLACE)) {
        drupal_set_message(t('Custom dynamic CSS saved to @filepath', array('@filepath' => $path)));
      }
    }
  }
}

/**
 * Helper function to render css3 gradient
 * background code
 *
 * @param $id
 * 	The css element id
 * @param $one
 *  Html color code for first gradient
 * @param $two
 *  Html color code for second gradient
 */
function vtcore_css_render_background ($id, $one, $two) {
  $output = "#" . $id . " { \r\n";
  $output .= "background: #" . $one . ";\r\n";

  if (!empty($two)) {
    $output .= "background: -moz-linear-gradient(top, #" . $one . " 0%, #" . $two . " 100%); /* FF3.6+ */ \r\n";
    $output .= "background: -webkit-gradient(linear, left top, left bottom, color-stop(0%,#" . $one . "), color-stop(100%,#" . $two . ")); /* Chrome,Safari4+ */ \r\n";
    $output .= "background: -webkit-linear-gradient(top, #" . $one ." 0%, #" . $two . " 100%); /* Chrome10+,Safari5.1+ */ \r\n";
    $output .= "background: -o-linear-gradient(top, #" . $one . " 0%, #" . $two . " 100%); /* Opera 11.10+ */ \r\n";
    $output .= "background: -ms-linear-gradient(top, #" . $one . " 0%, #" . $two . " 100%); /* IE10+ */ \r\n";
    $output .= "background: linear-gradient(top, #" . $one . " 0%, #" . $two . " 100%); /* W3C */ \r\n";
    $output .= "filter: progid:DXImageTransform.Microsoft.gradient( startColorstr='#" . $one . "', endColorstr='#" . $two . "',GradientType=0 ); /* IE6-9 */ \r\n";
  }
  $output .= "} \r\n";

  return $output;
}

/**
 * Helper function to render css3
 * border radius code
 *
 * @param $id
 *  CSS id element
 * @param $topleft
 * @param $topright
 * @param $bottomright
 * @param $bottomleft
 *  Border radius in pixels
 */
function vtcore_css_render_border_radius($id, $topleft = 0, $topright = 0, $bottomright = 0, $bottomleft = 0) {
  $output = "#" . $id . " { \r\n";
  $output .= "border-radius: "  . $topleft . "px "  . $topright . "px " . $bottomright . "px " . $bottomleft . "px; \r\n";
  $output .= "-moz-border-radius: "  . $topleft . "px "  . $topright . "px " . $bottomright . "px " . $bottomleft . "px; \r\n";
  $output .= "-webkit-border-radius: "  . $topleft . "px "  . $topright . "px " . $bottomright . "px " . $bottomleft . "px; \r\n";
  $output .= "-o-border-radius: "  . $topleft . "px "  . $topright . "px " . $bottomright . "px " . $bottomleft . "px; \r\n";
  $output .= "} \r\n";

  return $output;
}

/**
 * Helper function to render
 * font css code
 *
 * @param $id
 *  CSS element id
 * @param $variables
 *  Variables array of styles, variant, size, line_height, size, family
 * @param $convert
 *  convert to em or not (boolean)
 */
function vtcore_css_render_font($id, $variables, $convert = FALSE) {
  // convert to em if told to do so
  if ($convert != FALSE) {
    $variables['size'] = $variables['size'] / $convert;
    $variables['size'] .= "em";
  }
  else {
    $variables['size'] .= "px";
  }

  $output = $id . " { \r\n";
  $output .= "font-style: " . $variables['style'] . ";\r\n";
  $output .= "font-variant: " . $variables['variant'] . "; \r\n";
  $output .= "font-weight: " . $variables['weight'] . "; \r\n";
  $output .= "font-size: " . $variables['size'] . "; \r\n";
  $output .= "line-height: " . $variables['line_height'] . "%; \r\n";
  $output .= "font-family: " . $variables['family'] . "; \r\n";
  $output .= "} \r\n";

  return $output;
}