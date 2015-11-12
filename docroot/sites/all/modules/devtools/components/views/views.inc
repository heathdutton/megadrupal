<?php

/**
 * Generates the block structure for a particular View + display + arguments combination
 * @param  [type] $view_id   [description]
 * @param  [type] $display   [description]
 * @param  string $subject   [description]
 * @param  array  $arguments [description]
 * @return [type]            [description]
 */
function build_views_block($view_id, $display, $subject = '', $arguments = array()) {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    $block = array();
  
    if ($view = views_get_view($view_id)) {
      if ($view->access($display)) {
        $view_output = $view->preview($display, $arguments);
        if (!empty($view->result) || $view->display_handler->get_option('empty') || !empty($view->style_plugin->definition['even empty'])) {
          $view_title = $view->get_title();
          if (empty($subject) && !empty($view_title)) {
            $subject = $view_title;
          }

          $block['subject'] = check_plain($subject);
          $block['content']['#markup'] = $view_output;
          $block['module'] = 'views';
          $block['delta'] = $view_id . '-' . $display;
          $block['region'] = '';
        }
      }
      $view->destroy();
    }
    
    return $block;
  }
}

/**
 * Renders a Views block.
 * @param  [type] $block [description]
 * @return [type]        [description]
 */
function render_views_block($block) {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    $block = (object) $block;

    $args = array(
      'elements' => array(
        '#block' => $block,
        '#children' => $block->content['#markup'],
      ),
      'block' => $block,
    );

    $output = theme('block', $args);

    return $output;
  }
}