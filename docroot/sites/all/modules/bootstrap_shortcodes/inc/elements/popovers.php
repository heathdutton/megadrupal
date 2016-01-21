<?php

// Popover Top
function bs_shortcodes_shortcode_popovertop($attrs, $text) {
  $attrs = shortcode_attrs(array(
      'title' => '',
      'contents' => '',
      'popovertop' => ''
          ), $attrs
  );
  ;
  $popovertop = url($attrs['popovertop']);
  if ($text) {
    $style = empty($style) ? '' : ' style="' . $style . '"';
    $id = empty($id) ? '' : ' id="' . $id . '"';
    if ($attrs['title'] == '<none>') {
      $title = '';
    } else {
      $title = empty($attrs['title']) ? check_plain($text) : check_plain($attrs['title']);
      $title = ' title="' . $title . '"';
      $contents = empty($attrs['contents']) ? check_plain($text) : check_plain($attrs['contents']);
      $contents = $contents;
    }
    return '<a data-toggle="popover" data-container="body" data-placement="top" data-content="' . $contents . '"' . $id . $style . $title . '>' . $text . '</a>';
  }
  return $popovertop;
}

function bs_shortcodes_shortcode_popovertop_tip($format, $long) {
  $output = array();
  $output[] = '<p><strong>' . t('[popovertop title="Your popover title" contents="This is the content for the popover-top"]Trigger[/popovertop]') . '</strong>';
  if ($long) {
    $output[] = t('Inserts a popover at the top of an element, on hover with your custom text.') . '</p>';
    $output[] = '<p>&nbsp</p>';
  } else {
    $output[] = t('Inserts a popover at the top of an element, on hover with your custom text.') . '</p>';
  }

  return implode(' ', $output);
}

// Popover Right
function bs_shortcodes_shortcode_popoverright($attrs, $text) {
  $attrs = shortcode_attrs(array(
      'title' => '',
      'contents' => '',
      'popoverright' => ''
          ), $attrs
  );

  ;
  $popoverright = url($attrs['popoverright']);
  if ($text) {
    $style = empty($style) ? '' : ' style="' . $style . '"';
    $id = empty($id) ? '' : ' id="' . $id . '"';
    if ($attrs['title'] == '<none>') {
      $title = '';
    } else {
      $title = empty($attrs['title']) ? check_plain($text) : check_plain($attrs['title']);
      $title = ' title="' . $title . '"';
      $contents = empty($attrs['contents']) ? check_plain($text) : check_plain($attrs['contents']);
      $contents = $contents;
    }
    return '<a data-toggle="popover" data-container="body" data-placement="right" data-content="' . $contents . '"' . $id . $style . $title . '>' . $text . '</a>';
  }
  return $popoverright;
}

function bs_shortcodes_shortcode_popoverright_tip($format, $long) {
  $output = array();
  $output[] = '<p><strong>' . t('[popoverright title="Your popover title" contents="This is the content for the popover-right"]Trigger[/popoverright]') . '</strong>';
  if ($long) {
    $output[] = t('Inserts a popover to the right of an element, on hover with your custom text.') . '</p>';
    $output[] = '<p>&nbsp</p>';
  } else {
    $output[] = t('Inserts a popover to the right of an element, on hover with your custom text.') . '</p>';
  }

  return implode(' ', $output);
}

// Popover Bottom
function bs_shortcodes_shortcode_popoverbottom($attrs, $text) {
  $attrs = shortcode_attrs(array(
      'title' => '',
      'contents' => '',
      'popoverbottom' => ''
          ), $attrs
  );

  ;
  $popoverbottom = url($attrs['popoverbottom']);
  if ($text) {
    $style = empty($style) ? '' : ' style="' . $style . '"';
    $id = empty($id) ? '' : ' id="' . $id . '"';
    if ($attrs['title'] == '<none>') {
      $title = '';
    } else {
      $title = empty($attrs['title']) ? check_plain($text) : check_plain($attrs['title']);
      $title = ' title="' . $title . '"';
      $contents = empty($attrs['contents']) ? check_plain($text) : check_plain($attrs['contents']);
      $contents = $contents;
    }
    return '<a data-toggle="popover" data-container="body" data-placement="bottom" data-content="' . $contents . '"' . $id . $style . $title . '>' . $text . '</a>';
  }
  return $popoverbottom;
}

function bs_shortcodes_shortcode_popoverbottom_tip($format, $long) {
  $output = array();
  $output[] = '<p><strong>' . t('[popoverbottom title="Your popover title" contents="This is the content for the popover-bottom"]Trigger[/popoverbottom]') . '</strong>';
  if ($long) {
    $output[] = t('Inserts a popover to the bottom of an element, on hover with your custom text.') . '</p>';
    $output[] = '<p>&nbsp</p>';
  } else {
    $output[] = t('Inserts a popover to the bottom of an element, on hover with your custom text.') . '</p>';
  }

  return implode(' ', $output);
}

// Popover Left
function bs_shortcodes_shortcode_popoverleft($attrs, $text) {
  $attrs = shortcode_attrs(array(
      'title' => '',
      'contents' => '',
      'popoverleft' => ''
          ), $attrs
  );

  ;
  $popoverleft = url($attrs['popoverleft']);
  if ($text) {
    $style = empty($style) ? '' : ' style="' . $style . '"';
    $id = empty($id) ? '' : ' id="' . $id . '"';
    if ($attrs['title'] == '<none>') {
      $title = '';
    } else {
      $title = empty($attrs['title']) ? check_plain($text) : check_plain($attrs['title']);
      $title = ' title="' . $title . '"';
      $contents = empty($attrs['contents']) ? check_plain($text) : check_plain($attrs['contents']);
      $contents = $contents;
    }
    return '<a data-toggle="popover" data-container="body" data-placement="left" data-content="' . $contents . '"' . $id . $style . $title . '>' . $text . '</a>';
  }
  return $popoverleft;
}

function bs_shortcodes_shortcode_popoverleft_tip($format, $long) {
  $output = array();
  $output[] = '<p><strong>' . t('[popoverleft title="Your popover title" contents="This is the content for the popover-left"]Trigger[/popoverleft]') . '</strong>';
  if ($long) {
    $output[] = t('Inserts a popover to the left of an element, on hover with your custom text.') . '</p>';
    $output[] = '<p>&nbsp</p>';
  } else {
    $output[] = t('Inserts a popover to the left of an element, on hover with your custom text.') . '</p>';
  }

  return implode(' ', $output);
}

