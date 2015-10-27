<?php

/**
 * @file
 * Contains page callbacks of Node Admin module
 */

/**
 * Page callback.
 *
 * @param $type
 *
 * @return bool
 */
function node_admin_page($type) {
  return views_embed_view('node_admin', 'block_1', $type);
}
