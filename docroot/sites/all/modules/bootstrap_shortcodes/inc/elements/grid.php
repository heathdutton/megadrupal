<?php

function bs_shortcodes_shortcode_gridrow($attrs, $text) {
  $attrs = shortcode_attrs(array(
      'gridrow' => ''
          ), $attrs
  );

  $gridrow = url($attrs['gridrow']);
  if ($text) {
    return '<div class="row bsc-pad">' . $text . '</div>';
  } else {
    return '<div class="row bsc-pad"> </div>';
  }
  return $gridrow;
}

function bs_shortcodes_shortcode_gridrow_tip($format, $long) {
  $output = array();
  $output[] = '<p><strong>' . t('[gridrow]This is a row, it has no settings[/gridrow]') . '</strong>';
  if ($long) {
    $output[] = t('Wraps content in a grid row.') . '</p>';
    $output[] = '<p>&nbsp</p>';
  } else {
    $output[] = t('Wraps content in a grid row.') . '</p>';
  }

  return implode(' ', $output);
}

/*
 * 50% column
 */

function bs_shortcodes_shortcode_colhalf($attrs, $text) {
  $attrs = shortcode_attrs(array(
      'colhalf' => ''
          ), $attrs
  );

  $colhalf = url($attrs['colhalf']);
  if ($text) {
    return '<div class="col-md-6">' . $text . '</div>';
  } else {
    return '<div class="col-md-6"> </div>';
  }
  return $colhalf;
}

function bs_shortcodes_shortcode_colhalf_tip($format, $long) {
  $output = array();
  $output[] = '<p><strong>' . t('[colhalf]This is a row, it has no settings[/colhalf]') . '</strong>';
  if ($long) {
    $output[] = t('Wraps content in a grid row.') . '</p>';
    $output[] = '<p>&nbsp</p>';
  } else {
    $output[] = t('Wraps content in a grid row.') . '</p>';
  }

  return implode(' ', $output);
}

/*
 * THIRD column
 */

function bs_shortcodes_shortcode_colthird($attrs, $text) {
  $attrs = shortcode_attrs(array(
      'colthird' => ''
          ), $attrs
  );

  $colthird = url($attrs['colthird']);
  if ($text) {
    return '<div class="col-md-4">' . $text . '</div>';
  } else {
    return '<div class="col-md-4"> </div>';
  }
  return $colthird;
}

function bs_shortcodes_shortcode_colthird_tip($format, $long) {
  $output = array();
  $output[] = '<p><strong>' . t('[colthird]33% Column Width[/colthird]') . '</strong>';
  if ($long) {
    $output[] = t('To display a 1/3 width column.') . '</p>';
    $output[] = '<p>&nbsp</p>';
  } else {
    $output[] = t('To display a 3rd width column.') . '</p>';
  }

  return implode(' ', $output);
}

/*
 * 25% column
 */

function bs_shortcodes_shortcode_colquarter($attrs, $text) {
  $attrs = shortcode_attrs(array(
      'colquarter' => ''
          ), $attrs
  );

  $colquarter = url($attrs['colquarter']);
  if ($text) {
    return '<div class="col-md-3">' . $text . '</div>';
  } else {
    return '<div class="col-md-3"> </div>';
  }
  return $colquarter;
}

function bs_shortcodes_shortcode_colquarter_tip($format, $long) {
  $output = array();
  $output[] = '<p><strong>' . t('[colquarter]25% Column Width[/colquarter]') . '</strong>';
  if ($long) {
    $output[] = t('To display a 25% width column.') . '</p>';
    $output[] = '<p>&nbsp</p>';
  } else {
    $output[] = t('To display a 25% width column.') . '</p>';
  }

  return implode(' ', $output);
}
