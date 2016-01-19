<?php
/**
 * @file ./drush/go1_cache.inc
 */

function drush_go1_cache_clear() {
  $tags = func_get_args();
  go1_cache_flush_by_tags($tags);
}
