<?php

/**
 * @file
 * Page callbacks for the Cache Bypass Path module.
 */

function cache_bypass_path_hash_page($hash) {
  $chash = variable_get('cache_bypass_path_hash');
  if ($hash != $chash) {
    return MENU_NOT_FOUND;
  }
  drupal_page_is_cacheable(FALSE);
  return t('Hello');
}
