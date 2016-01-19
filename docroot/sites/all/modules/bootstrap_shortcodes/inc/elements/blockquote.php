<?php

function bs_shortcodes_shortcode_blockquote($attrs, $text) {
  $attrs = shortcode_attrs(array(
      'blockquote' => '',
      'source' => '',
          ), $attrs
  );

  $blockquote = url($attrs['blockquote']);
  if ($text) {
    $source = empty($attrs['source']) ? check_plain($text) : check_plain($attrs['source']);
    $source = '<footer>- <cite title="Source Title">' . $source . '</cite></footer>';
    return '<blockquote">' . $text . $source . '</blockquote>';
  } else {
    $text = empty($attrs['text']) ? check_plain($text) : check_plain($attrs['text']);
    $text = $text;
    $source = empty($attrs['source']) ? check_plain($text) : check_plain($attrs['source']);
    $source = '<footer>- <cite title="Source Title">' . $source . '</cite></footer>';
    return '<blockquote">' . $text . $source . '</blockquote>';
  }
  return $blockquote;
}

function bs_shortcodes_shortcode_blockquote_tip($format, $long) {
  $output = array();
  $output[] = '<p><strong>' . t('[blockquote]This is your normal blockquote area, it has no settings[/blockquote]') . '</strong>';
  if ($long) {
    $output[] = t('Wraps content in a styled blockquote.') . '</p>';
    $output[] = '<p>&nbsp</p>';
  } else {
    $output[] = t('Wraps content in a styled blockquote.') . '</p>';
  }

  return implode(' ', $output);
}

//Blockquote Reverse
function bs_shortcodes_shortcode_blockquote_reverse($attrs, $text) {
  $attrs = shortcode_attrs(array(
      'blockquoter' => '',
      'sourcer' => '',
          ), $attrs
  );

  $blockquoter = url($attrs['blockquoter']);
  if ($text) {
    $sourcer = empty($attrs['sourcer']) ? check_plain($text) : check_plain($attrs['sourcer']);
    $sourcer = '<footer><cite title="Source Title">' . $sourcer . ' -</cite></footer>';
    return '<blockquote class="blockquote-reverse">' . $text . $sourcer . '</blockquote>';
  } else {
    return $text;
    //  $text = empty($attrs['text']) ? check_plain($text) : check_plain($attrs['text']);
    //  $text = $text;
    //  $sourcer = empty($attrs['sourcer']) ? check_plain($text) : check_plain($attrs['sourcer']);
    //  $sourcer = '<footer><cite title="Source Title">' . $sourcer . ' -</cite></footer>';
    //  return '<blockquote class="blockquote-reverse">' . $text . $sourcer . '</blockquote>';
  }
  return $blockquote_reverse;
}

function bs_shortcodes_shortcode_blockquote_reverse_tip($format, $long) {
  $output = array();
  $output[] = '<p><strong>' . t('[blockquote_reverse]This is your normal blockquote area, it has no settings[/blockquote_reverse]') . '</strong>';
  if ($long) {
    $output[] = t('Wraps content in a styled blockquote.') . '</p>';
    $output[] = '<p>&nbsp</p>';
  } else {
    $output[] = t('Wraps content in a styled blockquote.') . '</p>';
  }

  return implode(' ', $output);
}
