<?php

/**
 * @file
 * Various module functions.
 */

/**
 * Parse content.
 * @param $text
 * @param $terms
 * @param $type
 * @param $configs
 * @return mixed
 */
function _taxonomy_autolink_links($text, $terms, $configs = array()) {
  // Read configs.
  $limit = $configs['limit'];
  // Start processing.
  foreach ($terms as $term_name => $term) {
    $rule = _taxonomy_autolink_links_rule($term_name, $configs);
    $text = preg_replace_callback($rule, function ($match) use ($term) {
      if (!empty($match[0])) {
        return theme('taxonomy_autolink_link', array(
          'tid' => $term->tid,
          'text' => $match[0]
        ));
      }
    }, $text, $limit);
  }
  return $text;
}

/**
 * @param $word
 * @param array $configs
 * @return string
 */
function _taxonomy_autolink_links_rule($needle, $configs = array()) {
  $case = isset($configs['case']) ? $configs['case'] : 0;
  $mode = isset($configs['mode']) ? $configs['mode'] : 0;
  $rule = '';
  // @todo Allow user to define this number.
  if (strlen($needle) > 150) {
    $needle = substr($needle, 0, strpos(wordwrap($needle, 150), "\n"));
  }
  // Quote regular expression characters.
  $needle = preg_quote($needle, '/');
  switch ($mode) {
    case 0:
      $rule = '/(?<=\s|^|<li>)' . $needle . '(?=\s|$|\<|<\/li>)/';
      break;
    case 1:
      $rule = '/(?<=\s|^|<li>)\b(' . $needle . '[ieds]*)\b(?=\s|$|\<|<\/li>)/';
      break;
  }
  // If case not sensitive, set to lowercase.
  if ($case === 0) {
    $rule .= 'i';
  }
  return $rule;
}
