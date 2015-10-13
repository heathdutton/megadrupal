<?php

function bs_shortcodes_shortcode_tooltiptop($attrs, $text) {
  $attrs = shortcode_attrs(array(
      'title' => '',
      'tooltiptop' => ''
          ), $attrs
  );

  $tooltiptop = url($attrs['tooltiptop']);
  if ($text) {
    $style = empty($style) ? '' : ' style="' . $style . '"';
    $id = empty($id) ? '' : ' id="' . $id . '"';
    if ($attrs['title'] == '<none>') {
      $title = '';
    } else {
      $title = empty($attrs['title']) ? check_plain($text) : check_plain($attrs['title']);
      $title = ' title="' . $title . '"';
    }
    return '<a href="#" rel="tooltip" class="hint" data-placement="top"' . $id . $style . $title . '>' . $text . '</a>';
  }
  return $tooltiptop;
}

function bs_shortcodes_shortcode_tooltiptop_tip($format, $long) {
  $output = array();
  $output[] = '<p><strong>' . t('[tooltiptop title="Your tooltip text"]text[/tooltiptop]') . '</strong>';
  if ($long) {
    $output[] = t('Inserts a tooltip at the top of an element, on hover with your custom text.') . '</p>';
    $output[] = '<p>&nbsp</p>';
  } else {
    $output[] = t('Inserts a tooltip at the top of an element, on hover with your custom text.') . '</p>';
  }

  return implode(' ', $output);
}

function bs_shortcodes_shortcode_tooltipright($attrs, $text) {
  $attrs = shortcode_attrs(array(
      'title' => '',
      'tooltipright' => ''
          ), $attrs
  );

  $tooltipright = url($attrs['tooltipright']);
  if ($text) {
    $style = empty($style) ? '' : ' style="' . $style . '"';
    $id = empty($id) ? '' : ' id="' . $id . '"';
    if ($attrs['title'] == '<none>') {
      $title = '';
    } else {
      $title = empty($attrs['title']) ? check_plain($text) : check_plain($attrs['title']);
      $title = ' title="' . $title . '"';
    }
    return '<a href="#" rel="tooltip" class="hint" data-placement="right"' . $id . $style . $title . '>' . $text . '</a>';
  }
  return $tooltipright;
}

function bs_shortcodes_shortcode_tooltipright_tip($format, $long) {
  $output = array();
  $output[] = '<p><strong>' . t('[tooltipright title="Your tooltip text"]text[/tooltipright]') . '</strong>';
  if ($long) {
    $output[] = t('Inserts a tooltip at the right of an element, on hover with your custom text.') . '</p>';
    $output[] = '<p>&nbsp</p>';
  } else {
    $output[] = t('Inserts a tooltip at the right of an element, on hover with your custom text.') . '</p>';
  }

  return implode(' ', $output);
}

function bs_shortcodes_shortcode_tooltipbottom($attrs, $text) {
  $attrs = shortcode_attrs(array(
      'title' => '',
      'tooltipbottom' => ''
// ...etc
          ), $attrs
  );

  $tooltipbottom = url($attrs['tooltipbottom']);
  if ($text) {
    $style = empty($style) ? '' : ' style="' . $style . '"';
    $id = empty($id) ? '' : ' id="' . $id . '"';
    if ($attrs['title'] == '<none>') {
      $title = '';
    } else {
      $title = empty($attrs['title']) ? check_plain($text) : check_plain($attrs['title']);
      $title = ' title="' . $title . '"';
    }
    return '<a href="#" rel="tooltip" class="hint" data-placement="bottom"' . $id . $style . $title . '>' . $text . '</a>';
  }
  return $tooltipbottom;
}

function bs_shortcodes_shortcode_tooltipbottom_tip($format, $long) {
  $output = array();
  $output[] = '<p><strong>' . t('[tooltipbotttom title="Your tooltip text"]text[/tooltipbottom]') . '</strong>';
  if ($long) {
    $output[] = t('Inserts a tooltip at the bottom of an element, on hover with your custom text.') . '</p>';
    $output[] = '<p>&nbsp</p>';
  } else {
    $output[] = t('Inserts a tooltip at the bottom of an element, on hover with your custom text.') . '</p>';
  }

  return implode(' ', $output);
}

function bs_shortcodes_shortcode_tooltipleft($attrs, $text) {
  $attrs = shortcode_attrs(array(
      'title' => '',
      'tooltipleft' => ''
// ...etc
          ), $attrs
  );

  $tooltipleft = url($attrs['tooltipleft']);
  if ($text) {
    $style = empty($style) ? '' : ' style="' . $style . '"';
    $id = empty($id) ? '' : ' id="' . $id . '"';
    if ($attrs['title'] == '<none>') {
      $title = '';
    } else {
      $title = empty($attrs['title']) ? check_plain($text) : check_plain($attrs['title']);
      $title = ' title="' . $title . '"';
    }
    return '<a href="#" rel="tooltip" class="hint" data-placement="left"' . $id . $style . $title . '>' . $text . '</a>';
  }
  return $tooltipleft;
}

function bs_shortcodes_shortcode_tooltipleft_tip($format, $long) {
  $output = array();
  $output[] = '<p><strong>' . t('[tooltipleft title="Your tooltip text"]text[/tooltipleft]') . '</strong>';
  if ($long) {
    $output[] = t('Inserts a tooltip at the left of an element, on hover with your custom text.') . '</p>';
    $output[] = '<p>&nbsp</p>';
  } else {
    $output[] = t('Inserts a tooltip at the left of an element, on hover with your custom text.') . '</p>';
  }

  return implode(' ', $output);
}

