<?php
/**
 * Returns block content.
 * @param  [type]  $module     [description]
 * @param  [type]  $delta      [description]
 * @param  boolean $show_title [description]
 * @return [type]              [description]
 */
function block_render($module, $delta, $show_title = FALSE) {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    $block = module_invoke($module, 'block_view', $delta);

    if (isset($block['content'])) {
      $output = '';

      if ($show_title && isset($block['subject'])) {
        $output = '<h2 class="block-title">' . $block['subject'] . '</h2>';
      }

      $output .= $block['content'];

      return $output;
    }
  }

  return FALSE;
}