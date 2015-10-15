<?php

function bs_shortcodes_shortcode_jumbotron($attrs, $text) {
  $attrs = shortcode_attrs(array(
      'jumbotron' => ''
          ), $attrs
  );

  $jumbotron = url($attrs['jumbotron']);
  if ($text) {
    return '<div class="jumbotron">' . $text . '</div>';
  } else {
    return $text;
  }
  return $jumbotron;
}

function bs_shortcodes_shortcode_jumbotron_tip($format, $long) {
  $output = array();
  $output[] = '<p><strong>' . t('[jumbotron]This is your normal jumbotron area, it has no settings[/jumbotron]') . '</strong>';
  if ($long) {
    $output[] = t('Wraps content in a styled jumbotron.') . '</p>';
    $output[] = '<p>&nbsp</p>';
  } else {
    $output[] = t('Wraps content in a styled jumbotron.') . '</p>';
  }

  return implode(' ', $output);
}
