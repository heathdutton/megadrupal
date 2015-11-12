<?php
/**
 * @file
 * This will automagically flip your LTR styles to RTL and vice versa.
 * Usage: @flip {file: "filename.scss"};
 * Started as a wordpress plugin by Louy Alakkad (http://wordpress.org/extend/plugins/rtler/).
 * @see css_flipper_css_alter()
 */

class Flipper {

  /**
   * extract blocks from css file, then $this->parse_block() on each.
   */
  function parse_css($css) {

    $return = '';
    $is_media = FALSE;

    // Strip Sass debug info (firesass info) to save media queries
    $css = preg_replace('/@media -sass-debug-info.*?\}\}/', '', $css);

    // Break the css into media queries
    $css = preg_split('/(@media.*?\}\})/', $css, 0, PREG_SPLIT_DELIM_CAPTURE);

    foreach ($css as $section) {
      // Break the css into blocks
      $section = explode('}', $section);

      // Loop through blocks.
      foreach ($section as $block) {

        // Explode to selector and code.
        $block = explode('{', $block);

        // Check header to see if it's @media.
        $h = $this->remove_comments($block[0]);

        // If the section is wrapped in a @media query
        if( preg_match( '/@media/', $h ) ) {
          $is_media = TRUE;
          $return .= "\n" . $block[0] . "{\n";
          array_shift($block);
        }

        // parse code
        if (!empty($block[1]) && !preg_match('/no-flip-block/', $block[1])) {
          if ($t = $this->parse_block($block[1])) {
            $return .= "\n" . trim($block[0]) . " {\n$t\n}\n";
          }
        }

        // Close the @media block
        if ($is_media && count($block) <= 1) {
          $return .= "\n}\n";
          // Reset value
          $is_media = FALSE;
        }
      }
    }

    return $return;
  }

  /**
   * Explode block to lines, call $this->parse_line on each,
   * then add neccesary code to the end of block;
   */
  function parse_block($block) {
    // Reset some vars
    $this->has   = FALSE;
    $this->has_p = FALSE;
    $this->has_m = FALSE;
    $this->has_b = FALSE;

    // Explode to lines
    $block = explode(";", $block);

    // Prepare return array
    $return = array();

    // Loop
    foreach ($block as $line) {
      if (!preg_match('/no-flip/', $line)) {
        $line = preg_replace('/\\/\\*.*\\*\\//', '', $line); // remove comments
        if (!$line)
          continue;
        $line = trim($line) . ';';
        preg_replace('/^[\s\t]*([a-z\-]+)\:[\s\t]*(.+)[\s\t]*;/', '$1: $2;', $line);
        $c = $this->parse_line($line);
        if ($c) {
          $return[] = '  ' . $c;
        }
      }
    }

    // Check for right/left
    if ($this->has) {
      $t = ($this->has === 'right' ) ? $this->has : 'left';
      $return[] = "  $t: auto;";
    }

    // Check for padding
    if ($this->has_p) {
      $t = ($this->has_p === 'right' ) ? $this->has_p : 'left';
      $return[] = "  padding-$t: 0;";
    }

    // Check for margin
    if ($this->has_m) {
      $t = ($this->has_m === 'right' ) ? $this->has_m : 'left';
      $return[] = "  margin-$t: auto;";
    }

    // Check for border
    if ($this->has_b) {
      $t = ($this->has_b === 'right' ) ? $this->has_b : 'left';
      $return[] = "  border-$t: none;";
    }

    // Return
    return count($return) ? implode("\n", $return) : FALSE;
  }

  /**
   * These bools are used to see if we add a (|padding-|margin-|border-)(right|left) so we can
   * set the other direction's value. if both directions are added then we add nothing.
   *
   * has   => right or left
   * has_p => padding
   * has_m => margin
   * has_b => border
   */
  var $has   = FALSE;
  var $has_p = FALSE;
  var $has_m = FALSE;
  var $has_b = FALSE;

  /**
   * Parse one line of css, if it has something that can be RTLed then do our job.
   * anyway, if not, return false.
   */
  function parse_line($line) {
    // Check if it has right or left word.
    if (preg_match('/(right|left)/', $line)) {

      // If it's right; 5px for example, we set $has to 'right'
      // if $has is left, then we reset it to false.
      if (preg_match('/^([\s\t]*)(right|left)\:/', $line)) {
        $is_right = preg_match('/^([\s\t]*)right\:/', $line);
        if ($this->has === ($is_right ? 'left' : 'right')) {
          $this->has = FALSE;
        }
        else {
          $this->has = $is_right ? 'right' : 'left';
        }
      }

      // Same as the above code
      if (preg_match('/^([\s\t]*)padding-(right|left)\:/', $line)) {
        $is_right = preg_match('/^([\s\t]*)padding-right\:/', $line);
        if ($this->has_p === ($is_right ? 'left' : 'right')) {
          $this->has_p = FALSE;
        }
        else {
          $this->has_p = $is_right ? 'right' : 'left';
        }
      }

      // Same as the above code
      if (preg_match('/^([\s\t]*)margin-(right|left)\:/', $line)) {
        $is_right = preg_match('/^([\s\t]*)margin-right\:/', $line);
        if ($this->has_m === ($is_right ? 'left' : 'right')) {
          $this->has_m = FALSE;
        }
        else {
          $this->has_m = $is_right ? 'right' : 'left';
        }
      }

      // Same as the above code
      if (preg_match('/^([\s\t]*)border-(right|left)\:/', $line)) {
        $is_right = preg_match('/^([\s\t]*)border-right\:/', $line);
        if ($this->has_b === ($is_right ? 'left' : 'right')) {
          $this->has_b = FALSE;
        }
        else {
          $this->has_b = $is_right ? 'right' : 'left';
        }
      }

      // Flip right and left.
      $line = $this->right_to_left($line);
    }
    elseif (preg_match('/(padding|margin):(([\s\t]*)([^\s\t]+)([\s\t]+)([^\s\t]+)([\s\t]+)([^\s\t]+)([\s\t]+)([^\s\t]*)([\s\t]*)(!important)?([\s\t]*);)/', $line, $matches)) {
      // If it's <code>padding: 1 2 3 4;</code> we'll flip the 2nd and the 4th values.
      // if they are equal, return false.
      if ($matches[6] == $matches[10]) {
        $line = FALSE;
      }
      else {
        // Flip
        $line = str_replace($matches[2], $matches[3] . $matches[4] . $matches[5] . $matches[10] . $matches[7] . $matches[8] . $matches[9] . $matches[6] . $matches[11] . $matches[11] . ';', $line);
      }
    }

    // Check if it has rtl or ltr word.
    elseif (preg_match('/(rtl|ltr)/', $line)) {
      // Flip
      $line = $this->rtl_to_ltr($line);
    }

    else { // No RTL to do, return false
      $line = FALSE;
    }

    // Return the result.
    return $line;
  }

  /**
   * Remove the css comments from a string.
   */
  function remove_comments($string) {

    // First, remove the //comments
    $s = explode("\n", $string);
    $r = array();
    foreach ($s as $_s) {
      $_s = trim($_s);
      if (substr($_s, 0, 2) != '//') {
        $r[] = $_s;
      }
    }
    $s = implode("\n", $r);

    // Now, remove the /*comments*/
    $s = explode('*/', $s);
    $r = array();
    foreach ($s as $_s) {
      $t = explode('/*', $_s);
      if (!empty($t[0])) {
        $r[] = $t[0];
      }
    }

    return implode("\n", $r);
  }

  /**
   * Remove everything except the comments from a string.
   */
  function keep_comments($string) {

    // Look for /*comments*/
    $s = explode('*/', $string);
    $x = '';

    foreach ($s as $_s) {
      $t = explode('/*', $_s);
      if (count($t) > 1) {
        $x .= "/*{$t[1]}*/\n";
      }
    }

    return $x;
  }

  /**
   * Replace "right" with "left" and vice versa
   */
  function right_to_left($str) {

    // Replace left with a TMP string.
    $s = str_replace('left', 'TMP_LEFT_STR', $str);

    // Flip right to left.
    $s = str_replace('right', 'left', $s);

    // Flip left to right.
    $s = str_replace('TMP_LEFT_STR', 'right', $s);

    return $s;
  }

  /**
   * Replace "rtl" with "ltr" and vice versa
   */
  function rtl_to_ltr($str) {

    // Replace left with a TMP string.
    $s = str_replace('ltr', 'TMP_LTR_STR', $str);

    // Flip right to left.
    $s = str_replace('rtl', 'ltr', $s);

    // Flip left to right.
    $s = str_replace('TMP_LTR_STR', 'rtl', $s);

    return $s;
  }

}
