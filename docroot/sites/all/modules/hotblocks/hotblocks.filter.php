<?php

/**
 * Implements hook_filter_info().
 *
 */
function hotblocks_filter_info() {
  $filters['hotblocks_filter'] = array(
    'title' => t('Hotblocks filter'),
    'description' => t('Allows users to insert a hotblock into text with the format [hotblock:N] where N is the numeric ID of the hotblock.'),
    'process callback'  => 'hotblocks_filter_process',
    'default settings' => array(),
    'tips callback' => 'hotblocks_filter_tips',
  );
  
  return $filters;
}

function hotblocks_filter_tips($filter, $format, $long) {
  if ($long) {
    return t('Insert a hotblock into text with the format [hotblock:N], where N is the numeric ID of the hotblock.');
  }
  else {
    return t('Hotblocks can be embedded into content with the format [hotblock:N].');
  }
}

/**
 * Process callback for hotblocks_filter
 */
function hotblocks_filter_process($text, $filter, $format, $langcode, $cache, $cache_id) {
  $text = preg_replace_callback('/\[hotblock\:([0-9]*?)\]/', 'hotblocks_filter_process_helper', $text);
  return $text;
}

/**
 * Replace a numeric hotblock ID found by preg_replace_callback with a rendered hotblock
 * Used by hotblocks_filter_process
 */
function hotblocks_filter_process_helper($aMatches) {
  if (sizeof($aMatches) > 1) {
    if (is_numeric($aMatches[1])) {
      return '<div class="block-hotblocks">' . render(hotblocks_block_view($aMatches[1])) .'</div>';
    }
  }
}
