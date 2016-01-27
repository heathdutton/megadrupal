<?php

function paas_metric_blocks_call($value) {
  if (is_numeric($value)) {
    $sql = 'SELECT module, delta, theme FROM {blocks} WHERE bid = :bid';
    $res = db_query($sql, array(':bid' => $value));
    $block = $res->fetchObject();
    if (!$block) {
      exit;
    }
    drupal_theme_initialize();
    timer_start('paas_blocks_build');
    $object = module_invoke($block->module, 'block_view', $object->delta);
    $block->build_time = timer_read('paas_blocks_build');
    timer_start('paas_blocks_render');
    if (is_array($object) && !empty($object['content'])) {
      isset($object['subject']) or $object['subject'] = '';
      render($object);
    }
    $block->render_time = timer_read('paas_blocks_render');
    if (function_exists('memory_get_peak_usage')) {
      $block->memory_usage = memory_get_peak_usage();
    }
    else {
      $block->memory_usage = 0;
    }
    
    echo serialize($block);
    exit;
  }
}
