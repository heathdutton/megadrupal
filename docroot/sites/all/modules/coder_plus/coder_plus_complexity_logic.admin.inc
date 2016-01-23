<?php

/**
 * @file
 * This file holds functions for handling coder_plus complexity logic.
 *
 * @SNDCP coder_plus
 */
define('IF_CONDITION', '/(else)?if.*\(.+/');
define('TERNARY_OPERATOR', '/\?.*:/');
define('ANDAND_OPERATOR', '/&&/');
define('OR_OPERATOR', '/\|\|/');
define('CASE_CONDITION', '/case.+?:.*?break[;]/s');
define('SWITCH_CONDITION', '/switch.*\(.*\)\{?/');
define('DEFAULT_CONDITION', '/default[ ]*:/');
define('WHILE_LOOP', '/while.*\(.*\)/');
define('FOR_LOOP', '/for.*\(.*;.*;.*\)/');
define('FOREACH_LOOP', '/foreach.*\(.*as.*\)/');

/**
 * Function to check cyclometric complexity for a module.
 *
 * @param string $file
 *   File path
 *
 * @return array
 *   function with their complexity
 */
function coder_plus_check_complexity($file) {
  $functions = array();
  $in_comment = FALSE;
  $in_php = FALSE;
  $is_class = FALSE;
  $handle = fopen($file, 'r');
  $function_array = array();

  while (!feof($handle)) {
    $line = coder_plus_escape_slashes(fgets($handle));
    if (1 == preg_match('/\<\?(php)?/', $line)) {
      $in_php = TRUE;
    }
    if (1 === preg_match('/\?\>/', $line)) {
      $in_php = FALSE;
    }
    if (1 === preg_match('/\/\*/', $line)) {
      $in_comment = TRUE;
    }
    if (1 === preg_match('/\*\//', $line)) {
      $in_comment = FALSE;
    }
    if ($in_comment || !$in_php) {
      continue;
    }
    if (FALSE !== ($function_name = coder_plus_find_function($line))) {
      $function_array[] = $function_name;
    }

    if (FALSE !== coder_plus_find_class($line)) {
      $is_class = TRUE;
      break;
    }
  }

  fclose($handle);

  if (!$is_class) {
    $functions = coder_plus_process_nodes_edges($function_array, $file);
  }

  return $functions;
}

/**
 * Function to calculate cyclometric complexity for a module.
 * 
 * @param array $function_array
 *   Array of function in a file.
 * @param array $file
 *   File data in array format
 *
 * @return array
 *   Final out with calculated nodes and edges.
 */
function coder_plus_process_nodes_edges($function_array, $file) {
  include_once $file;
  $functions = array();
  foreach ($function_array as $data_function) {
    $reflector = new ReflectionFunction($data_function['fnc']);

    $body = array_slice(
        file($file), $reflector->getStartLine(), $reflector->getEndLine() - $reflector->getStartLine()
    );
    $function_line = count($body);
    $source = implode($body);
    $node_edges = coder_plus_get_nodes_edges($source);

    $functions[$data_function['fnc']]['complexity'] = $node_edges['edges'] - $node_edges['nodes'] + 2;
    $functions[$data_function['fnc']]['count'] = $node_edges['counts'];
    $functions[$data_function['fnc']]['function_line'] = $function_line;
  }

  return $functions;
}

/**
 * Function to get nodes and edges.
 *
 * @param array $source
 *   Array format of file to be processed.
 *
 * @return array
 *   Array of nodes, edges and count.
 */
function coder_plus_get_nodes_edges($source) {
  $counts = coder_plus_get_count($source);
  $nodes = 1;
  $edges = 0;

  // If, or, ternary operator, andand.
  $nodes += 3 * $counts['if'] + 3 * $counts['olif'] + 3 * $counts['andand'] + 3 * $counts['or'];
  $edges += 4 * $counts['if'] + 4 * $counts['olif'] + 4 * $counts['andand'] + 4 * $counts['or'];

  // Switch, case and default.
  $nodes += 2 * $counts['switch'] + 1 * $counts['case'] - 2 * $counts['default'];
  $edges += 2 * $counts['switch'] + 2 * $counts['case'] - 2 * $counts['default'];

  // While.
  $nodes += 3 * $counts['while'];
  $edges += 4 * $counts['while'];

  // For and foreach.
  $nodes += 3 * $counts['for'] + 3 * $counts['foreach'];
  $edges += 4 * $counts['for'] + 4 * $counts['foreach'];

  return array('nodes' => $nodes, 'edges' => $edges, 'counts' => $counts);
}

/**
 * Function to get count of different complexities.
 *
 * @param array $source
 *   Array format of file to be processed
 *
 * @return array
 *   Array of count for different complexities.
 */
function coder_plus_get_count($source) {
  $counts = array();
  $counts['if'] = preg_match_all(IF_CONDITION, $source, $matches);
  $counts['olif'] = preg_match_all(TERNARY_OPERATOR, $source, $matches);
  $counts['andand'] = preg_match_all(ANDAND_OPERATOR, $source, $matches);
  $counts['or'] = preg_match_all(OR_OPERATOR, $source, $matches);
  $counts['case'] = preg_match_all(CASE_CONDITION, $source, $matches);
  $counts['default'] = preg_match_all(DEFAULT_CONDITION, $source, $matches);
  $counts['switch'] = preg_match_all(SWITCH_CONDITION, $source, $matches);
  $counts['while'] = preg_match_all(WHILE_LOOP, $source, $matches);
  $counts['for'] = preg_match_all(FOR_LOOP, $source, $matches);
  $counts['foreach'] = preg_match_all(FOREACH_LOOP, $source, $matches);

  $counts['conditions'] = $counts['if'] + $counts['olif'] + $counts['switch'] + $counts['case'] + $counts['default'];
  $counts['loops'] = $counts['while'] + $counts['for'] + $counts['foreach'];
  $counts['operator'] = $counts['andand'] + $counts['or'];

  return $counts;
}

/**
 * Function to perform regular expression search and replace.
 *
 * @param string $line
 *   Line written in function.
 *
 * @return string
 *   Formatted string.
 */
function coder_plus_escape_slashes($line) {
  $pattern = array(
      "/\\\'/",
      '/\\\"/',
      '/\'{1}[.]*[^\']*\'{1}/',
      '/"{1}[.]*[^"]*"{1}/',
      '/\/\*.*\*\//',
      '/(#|\/\/).*/');

  $replacement = array(
      'ESCAPED_SINGLE_QUOTE',
      'ESCAPED_DOUBLE_QUOTE',
      'SINGLE_QUOTE',
      'DOUBLE_QUOTE',
      'SLST_COMMENT',
      'ONLI_COMMENT');

  return preg_replace($pattern, $replacement, $line);
}

/**
 * Function to find function in a file.
 * 
 * @param string $line
 *   Line written in function.
 */
function coder_plus_find_function($line) {
  if (0 === preg_match('/function[\s]+&?([a-zA-Z_\x7f-\xff][a-zA-Z0-9_\x7f-\xff]*)\(.*\)(?!;)/', $line, $match)) {
    return FALSE;
  }

  $units['fnc'] = $match[1];
  return $units;
}

/**
 * Function to find class in a file.
 *
 * @param string $line
 *   Line written in function.
 *
 * @return bool
 *   True or false.
 */
function coder_plus_find_class($line) {
  if (0 === preg_match('#^(\s*)((?:(?:abstract|final|static)\s+)*)class\s+([-a-zA-Z0-9_]+)(?:\s+extends\s+([-a-zA-Z0-9_]+))?(?:\s+implements\s+([-a-zA-Z0-9_,\s]+))?#', $line, $match)) {
    return FALSE;
  }

  return TRUE;
}

/**
 * Function to retrive cyclometric complexity image.
 *
 * @param int $count
 *   Count of cyclometric complexity.
 *
 * @param int $is_drush
 *   is_drush contains flag whether return image or severity label.
 *   0 => Return severity image 
 *   1 => Return Severity label for drush output.
 *
 * @return array
 *   Array theme of image
 */
function coder_plus_get_complexity_image($count, $is_drush = 0) {
  if ($count > variable_get('critical_to')) {
    $severity_name = 'blocker';
  }
  elseif ($count >= variable_get('critical_from') && $count <= variable_get('critical_to')) {
    $severity_name = 'critical';
  }
  elseif ($count >= variable_get('major_from') && $count <= variable_get('major_to')) {
    $severity_name = 'major';
  }
  elseif ($count >= variable_get('minor_from') && $count <= variable_get('minor_to')) {
    $severity_name = 'minor';
  }
  else {
    $severity_name = 'right';
  }
  if ($is_drush) {
    return $severity_name;
  }
  else {
    $path = drupal_get_path('module', 'coder_plus');
    $title = t('severity: @severity', array('@severity' => ucfirst($severity_name)));
    $img = theme('image', array(
        'path' => $path . "/images/$severity_name.png",
        'alt' => $title,
        'title' => $title,
        'attributes' => array('align' => 'right', 'class' => 'coder'),
        'getsize' => FALSE,
    ));
    return $img;
  }
}
