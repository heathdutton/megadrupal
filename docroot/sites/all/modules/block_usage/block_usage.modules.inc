<?php

/**
 * @file
 * Possibly include 3rd party module hooks that should be in those modules and
 * might be, that's why they're includes with a nasty function_exists() around it.
 */

// blockreference.module
if (!function_exists('blockreference_block_usage')) {
  require __DIR__ . '/modules/block_usage.blockreference.inc';
}

// context.module
if (!function_exists('context_block_usage')) {
  require __DIR__ . '/modules/block_usage.context.inc';
}

// node_level_blocks.module
if (!function_exists('node_level_blocks_block_usage')) {
  require __DIR__ . '/modules/block_usage.node_level_blocks.inc';
}
