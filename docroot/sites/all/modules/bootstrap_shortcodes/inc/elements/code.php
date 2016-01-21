<?php

function bs_shortcodes_shortcode_inlinecode($attrs, $text) {
  $attrs = shortcode_attrs(array(
      'inlinecode' => ''
          ), $attrs
  );

  if ($text) {
    return '<code>' . $text . '</code>';
  }
  return $inlinecode;
}

function bs_shortcodes_shortcode_inlinecode_tip($format, $long) {
  $output = array();
  $output[] = '<p><strong>' . t('[inlinecode]See the icon?[/inlinecode]') . '</strong>';
  if ($long) {
    $output[] = t('Adds code inline to text') . '</p>';
    $output[] = '<p>&nbsp</p>';
  } else {
    $output[] = t('Adds code inline to text.') . '</p>';
  }

  return implode(' ', $output);
}

/* Code - Keyboard Input */

function bs_shortcodes_shortcode_kbd($attrs, $text) {
  $attrs = shortcode_attrs(array(
      'kbd' => ''
          ), $attrs
  );

  if ($text) {
    return '<kbd>' . $text . '</kbd>';
  }
  return $kbd;
}

function bs_shortcodes_shortcode_kbd_tip($format, $long) {
  $output = array();
  $output[] = '<p><strong>' . t('[kbd]ESC[/kbd]') . '</strong>';
  if ($long) {
    $output[] = t('Formats the content in keyboard style') . '</p>';
    $output[] = '<p>&nbsp</p>';
  } else {
    $output[] = t('Formats the content in keyboard style') . '</p>';
  }

  return implode(' ', $output);
}

/* Code - Pre Normal */

function bs_shortcodes_shortcode_pre($attrs, $text) {
  $attrs = shortcode_attrs(array(
      'pre' => ''
          ), $attrs
  );

  if ($text) {
    return '<pre>' . $text . '</pre>';
  }
  return $pre;
}

function bs_shortcodes_shortcode_pre_tip($format, $long) {
  $output = array();
  $output[] = '<p><strong>' . t('[pre]ESC[/pre]') . '</strong>';
  if ($long) {
    $output[] = t('Provides a safe wrapper for single line code input') . '</p>';
    $output[] = '<p>&nbsp</p>';
  } else {
    $output[] = t('Provides a safe wrapper for single line code input') . '</p>';
  }

  return implode(' ', $output);
}

/* Code - Pre Block */

function bs_shortcodes_shortcode_preblock($attrs, $text) {
  $attrs = shortcode_attrs(array(
      'preblock' => ''
          ), $attrs
  );

  if ($text) {
    return '<pre class="pre-scrollable">' . $text . '</pre>';
  }
  return $preblock;
}

function bs_shortcodes_shortcode_preblock_tip($format, $long) {
  $output = array();
  $output[] = '<p><strong>' . t('[preblock]ESC[/preblock]') . '</strong>';
  if ($long) {
    $output[] = t('Provides a safe wrapper for multi line code input') . '</p>';
    $output[] = '<p>&nbsp</p>';
  } else {
    $output[] = t('Provides a safe wrapper for multi line code input') . '</p>';
  }

  return implode(' ', $output);
}
