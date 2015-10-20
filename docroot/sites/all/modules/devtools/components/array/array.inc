<?php

/**
 * Populates $tree with an array of paths to all child elements.
 * @param  [type] $data [description]
 * @param  [type] $tree [description]
 * @param  string $path [description]
 * @return [type]       [description]
 */
function flatten_tree($data, &$tree, $path = '') {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    if (!is_array($data)) {
      $path .= $data;

      $tree[] = explode('|', $path);
    }
    else {
      foreach ($data as $key => $value) {
        $path_tree = $path . $key . '|';
        flatten_tree($value, $tree, $path_tree);
      }
    }
  }
}

/**
 * Finds the point of divergence of two flat, numerically-indexed arrays.
 * @param  [type] $tree1 [description]
 * @param  [type] $tree2 [description]
 * @return [type]        [description]
 */
function array_flat_divergence($tree1, $tree2) {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    $size = sizeof($tree1);
    for ($i = 0; $i < $size; $i++) {
      if ($tree1[$i] !== $tree2[$i]) {
        return $tree1[$i - 1];
      }
    }

    return FALSE;
  }
}

