<?php
/**
 * @file
 * Configuration settings for Codit Blocks module.
 */

global $_codit_blocks_block_title_override;

/*
 * Block title handling:
 * It is often desirable to remove all block titles and have only specific
 * block titles show.  Or show them all but override only specific block titles.
 * - To hide all block titles except overridden: set
 *   'show-block-titles' => FALSE,
 * - To show all block titles and overridden: set 'show-block-titles' => TRUE,
 *
 * To override a title you have three choices:
 *  1) Output the desired title directly in the tpl for the block (preferred)
 *  2) Add the 'blockdelta' => 'Desired Title' to the
 *    $_codit_blocks_block_title_override array below.
 *  3) Allow all blocks from a specific module to use their names (needed if the
 *    block has dynamic block deltas). Add the 'module_name' => 'all' to the
 *    $_codit_blocks_block_title_override array below
 *
 * Having trouble determining the delta of a block? Look in the "block"
 * table in the db.
 */

$_codit_blocks_block_title_override = array(
  'show-block-titles' => TRUE,
);
