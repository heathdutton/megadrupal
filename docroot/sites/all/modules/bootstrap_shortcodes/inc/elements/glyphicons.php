<?php

function bs_shortcodes_shortcode_glyphicons($attrs, $text) {
  $attrs = shortcode_attrs(array(
      'name' => '',
      'glyphicons' => ''
          ), $attrs
  );

  if ($text) {
    $name = empty($attrs['name']) ? check_plain($text) : check_plain($attrs['name']);
    $name = $name;
    return '<span class="glyphicon glyphicon-' . $name . '"></span>' . $text;
  }
  return $glyphicons;
}

function bs_shortcodes_shortcode_glyphicons_tip($format, $long) {
  $output = array();
  $output[] = '<p><strong>' . t('[glyphicons]See the icon?[/glyphicons]') . '</strong>';
  if ($long) {
    $output[] = t('Prefixes content with an icon.') . '</p>';
    $output[] = '<p>&nbsp</p>';
  } else {
    $output[] = t('Prefixes content with an icon.') . '</p>';
  }

  return implode(' ', $output);
}
