<?php

function theme_views_block_inception_row(&$vars) {
  $view = $vars['view'];
  $viewname = $vars['view']->name . '-' . $vars['view']->current_display;

  $subject = $content = $delta = '';

  foreach($vars['options']['blockfields'] as $field => $value) {
    if ($value != '') {
      $$field = $view->render_field($value, $view->row_index);
    }
  }

  return _views_block_inception_render_block($subject, $content, $viewname, $delta);
}

function _views_block_inception_render_block($subject, $content, $region, $delta) {
  $block = new stdClass();
  $block->subject = $subject;
  $block->content = $content;
  $block->delta = $delta;
  $block->module = 'views_block_inception';
  $block->region = $region;

  $element = array('elements' => array(
    '#block' => $block,
    '#children' => $block->content,
  ));

  return theme('block', $element);
}
