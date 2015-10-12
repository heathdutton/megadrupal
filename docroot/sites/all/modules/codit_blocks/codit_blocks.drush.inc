<?php
/**
 * @file
 * Drush commands for Codit: Blocks.
 */

/**
 * Implements hook_drush_command().
 */
function codit_blocks_drush_command() {
  $items = array();

  // The 'add block' command.
  $items['codit-blocks-add'] = array(
    'description' => "Creates a new block directory and files in codit_local/blocks/block_bin and registers the block",
    'arguments' => array(
      'block-delta' => 'The name of the block (block delta).  Make it unique, meaningful and 32 char max.',
    ),
    'options' => array(
      'enabled' => array(
        'description' => 'If the block is enabled, it is immediately ready for placement, else it will create the directory with a leading _  making it disabled.',
        'example-value' => 'FALSE',
        'value' => 'optional',
      ),
    ),
    'examples' => array(
      'drush codit-blocks-add cafe' => 'Creates a block named "cafe" and enables it.',
      'drush codit-blocks-add cafe --enabled=TRUE' => 'Creates a block named "cafe" enables it.',
      'drush codit-blocks-add cafe --enabled=FALSE' => 'Creates a block named "cafe" but disables it with an _.',
    ),
    'scope' => 'site',
    'drupal dependencies' => array('codit', 'codit_local'),
    'aliases' => array('codit-block-add', 'codit-blocks-add'),
  );

  $items['codit-blocks-enable'] = array(
    'description' => "Enables an existing disabled Codit Block by removing the underscore from in front of the block directory and flushes the cache to register it.",
    'arguments' => array(
      'block-delta' => 'The name of the block (block delta) of an existing block.',
    ),
    'examples' => array(
      'drush codit-blocks-enable my_block_name' => 'Enables a block named "my_block_name".',
    ),
    'scope' => 'site',
    'drupal dependencies' => array('codit', 'codit_local'),
    'aliases' => array('codit-block-enable', 'codit-blocks-enable'),
  );

  $items['codit-blocks-disable'] = array(
    'description' => "Disables an existing enebled Codit Block by adding an underscore from in front of the block directory and flushes the cache to remove it.",
    'arguments' => array(
      'block-delta' => 'The name of the block (block delta) of an existing block.',
    ),
    'examples' => array(
      'drush codit-blocks-disable my_block_name' => 'Disables a block named "my_block_name".',
    ),
    'scope' => 'site',
    'drupal dependencies' => array('codit', 'codit_local'),
    'aliases' => array('codit-block-disable', 'codit-blocks-disable'),
  );

  $items['codit-blocks-cache-clear-block-list'] = array(
    'description' => "Clears the existing Codit: Blocks block list cache to register new block changes.  Unneccessary if using Drush to add, enable or disable a block.",

    'examples' => array(
      'drush codit-blocks-cache-clear-block-list' => 'Clears the Codit: Blocks cache to register or de-register a block.',
    ),
    'scope' => 'site',
    'drupal dependencies' => array('codit', 'codit_local'),
    'aliases' => array(
      'codit-blocks-cache-clear-block-list',
      'codit-block-cache-clear-block-list',
      'codit-blocks-ccbl',
      'codit-block-ccbl',
    ),
  );

  $items['codit-blocks-cache-flush'] = array(
    'description' => "Clears any Codit: Blocks cached block content. Used when cached block content needs to be regenerated.",

    'examples' => array(
      'drush codit-blocks-cache-flush' => 'Clears the Codit: Blocks cached block content.',
    ),
    'scope' => 'site',
    'drupal dependencies' => array('codit', 'codit_local'),
    'aliases' => array(
      'codit-blocks-cache-flush',
      'codit-block-cache-flush',
      'codit-blocks-cf',
      'codit-block-cf',
    ),
  );

  $items['codit-blocks-list'] = array(
    'description' => "Lists all of the currently enabled blocks by Codit.",
    'examples' => array(
      'drush codit-blocks-list' => 'Outputs a list of all enabled Codit blocks to terminal.',
    ),
    'scope' => 'site',
    'drupal dependencies' => array('codit', 'codit_local'),
    'aliases' => array(
      'codit-blocks-list',
      'codit-block-list',
    ),
  );

  $items['codit-blocks-local-init'] = array(
    'description' => "Creates the blocks storage within Codit: Local if it does not already exist.",
    'examples' => array(
      'drush codit-blocks-init' => 'Creates the blocks storage within Codit: Local if it does not already exist.',
    ),
    'scope' => 'site',
    'drupal dependencies' => array('codit', 'codit_local'),
    'aliases' => array(
      'codit-blocks-init',
      'codit-block-init',
    ),
  );

  return $items;
}


/**
 * Implements hook_drush_help().
 */
function codit_blocks_drush_help($section) {
  switch ($section) {
    case 'drush:codit-blocks-list':
      return dt("Lists all enabled Codit: Blocks.");

    case 'drush:codit-blocks-local-init':
      return dt("Creates the storage /blocks within Codit: Local.");

    case 'drush:codit-blocks-cache-clear-block-list':
      return dt("This clears the Codit: Blocks list cache, to update available blocks.");

    case 'drush:codit-blocks-flush-cache':
      return dt("This clears the Codit: Blocks block content cache.");

    case 'drush:codit-blocks-enable':
      return dt("This enables a disabled block by removing an _ from in front of the directory.");

    case 'drush:codit-blocks-disable':
      return dt("This disables an enabled block by placing an _ in front of the directory.");

    case 'drush:codit-blocks-add':
      return dt("This command will create the directory, template file and callback file in codit_local/blocks/block_bin");

    // The 'title' meta is used to name a group of commands in `drush help`.
    case 'meta:codit_blocks:title':
      return dt("Commands for making custom blocks.");

    // The 'summary' meta item is displayed in `drush help --filter`.
    case 'meta:codit_blocks:summary':
      return dt("Assists in making custom code blocks for this site.");
  }
}

/**
 * Implements drush_hook_COMMAND_validate().
 */
function codit_blocks_validate() {
  if (drush_is_windows()) {
    // $name = drush_get_username();
    // TODO: implement check for elevated process using w32api
    // as sudo is not available for Windows
    // http://php.net/manual/en/book.w32api.php
  }
  else {
    $name = posix_getpwuid(posix_geteuid());
    if ($name['name'] !== 'root') {
      return drush_set_error('CODIT_BLOCKS', dt('There was an error processing your block due to permissions.'));
    }
  }
}

/**
 * Drush command callback to create a block.
 *
 * @param string $blockname
 *   The name of the block to be created.
 * @param bool $enabled
 *   Option passed to the block.
 */
function drush_codit_blocks_add($blockname = '', $enabled = TRUE) {
  $o_block = new stdClass();
  // Any error, messages, or success set will be output.
  $o_block->error = array();
  $o_block->msg = array();
  $o_block->success = array();
  // All systems are go unless one or more of the checks says otherwise.
  $o_block->build = TRUE;
  $o_block->name = trim($blockname);
  $o_block->length = strlen($o_block->name);

  // Check to see if the local environment is present.
  $codit_local_dir = drupal_get_path('module', 'codit_local') . '/blocks/';
  if (!file_exists($codit_local_dir)) {
    drush_codit_blocks_local_init();
  }

  // Start the case race.  First one to trigger TRUE aborts the block creation.
  switch (TRUE) {
    // Check that we have a blockname.
    case empty($o_block->name):
      $o_block->error[] = dt("You must include a block delta in your request, example: drush codit-blocks-add my_new_block");
      break;

    // Check that length is not longer than 32 chars.
    case $o_block->length > 32:
      $o_block->error[] = dt("The block delta '!blockname' is !length chars, but the limit is 32 chars.  Please shorten it.", array('!blockname' => $blockname, '!length' => $o_block->length));
      break;

    // Check that it contains no hyphens.
    case strpos($o_block->name, '-') !== FALSE:
      $o_block->error[] = dt("The block delta '!blockname' contains hyphens and can not.  Please use underscores.", array('!blockname' => $blockname));
      break;

    // Check that it contains no spaces.
    case strpos($o_block->name, ' ') !== FALSE:
      $o_block->error[] = dt("The block delta '!blockname' contains spaces and can not.  Please use underscores.", array('!blockname' => $blockname));
      break;

    // Check to see if the block already exists as an enabled block.
    case codit_blocks_dir_exists($o_block):
      if ($o_block->enabled) {
        // The block exists and is enabled.
        $o_block->error[] = dt("The block delta '!blockname' already exists as an enabled block.  Remember, block names must be unique.", array('!blockname' => $o_block->name));
      }
      else {
        // The block exists and is disabled.
        $o_block->error[] = dt("The block delta '!blockname' already exists as a disabled block.  Remember, block names must be unique.", array('!blockname' => $o_block->name));
      }
      break;

    default:
      $o_block->msg[] = dt("The block delta '!blockname' has met the criteria to be a block and is being built.", array('!blockname' => $blockname));
      break;

  }

  if (!empty($o_block->error) || !empty($o_block->errorFlag)) {
    // There was an error so set the flag.
    $o_block->errorFlag = TRUE;
  }

  if (empty($o_block->errorFlag)) {
    // Passed all checks, proceed to build the block.

    // Made it this far, carry on and make the new block directory.
    _codit_blocks_create_new_block($o_block);

    // Check to see if the enable was already set by another check.
    if (!isset($o_block->enable_it)) {
      // This was not previously set by the name of the block, so read options.
      $o_block->enable_it = codit_sanitize_boolean(drush_get_option('enabled', TRUE), 'boolean', TRUE);
    }

    // The block is already enabled, process the disable if there is one.
    if (!$o_block->enable_it) {
      // Disable the block.
      drush_codit_blocks_codit_block_disable($o_block->name);
    }
    else {
      // The block was built and by default is enabled.
      $o_block->enabled = TRUE;
    }

    if (empty($o_block->errorFlag)) {
      // No errors, and it made it this far.  Assume it was built.
      if ($o_block->enabled) {
        // This block is enabled.
        $o_block->success[] = dt("The block delta '!blockname' was built and should be ready for placement.", array('!blockname' => $o_block->name));
      }
      else {
        // The block is disabled.
        $o_block->success[] = dt("The block delta '!blockname' was built, but is currently disabled.  Use 'drush codit-blocks-enable !blockname'", array('!blockname' => $o_block->name));
      }
      // Clear the Codit Blocks registered block cache to pick up an changes.
      cache_clear_all('codit_blocks_block_list', 'cache');
    }
  }
  else {
    // Block failed so issue error and do nothing else.
    $o_block->error[] = "This block was not built.";
  }
  codit_drush_process_messages($o_block);
}


/**
 * Checks to see if the block exists and reads/saves enabled property of block.
 *
 * @param object $o_block
 *   The block object that is being built.
 *
 * @return bool
 *   TRUE if the block exists, FALSE otherwise.
 */
function codit_blocks_dir_exists(&$o_block) {
  // Adjust name in case requested disabled block.
  $trimmed = ltrim($o_block->name, '_');
  if ($o_block->name != $trimmed) {
    // Had an initial underscore so make it disabled.
    $o_block->name = $trimmed;
    $o_block->enable_it = FALSE;
  }

  // Is this block present?

  // Verify block storage in codit_local.
  if (codit_path_to_local('blocks')) {
    // Codit: Local blocks storage is available.
    $o_block->location  = codit_path_to_local('blocks') . 'block_bin/' . $o_block->name;
    $o_block->location_disabled = codit_path_to_local('blocks') . 'block_bin/_' . $o_block->name;

    if (file_exists($o_block->location)) {
      // Yes the block exists as enabled.
      $o_block->exists = TRUE;
      $o_block->enabled = TRUE;
    }
    elseif (file_exists($o_block->location_disabled)) {
      // The block exists but is disabled.
      $o_block->exists = TRUE;
      $o_block->enabled = FALSE;
    }
    else {
      // The block does not exist at all.
      $o_block->exists = FALSE;
      $o_block->enabled = FALSE;
    }

    return $o_block->exists;
  }
  else {
    // Storage is not available.  Throw an error.
    $o_block->error[] = dt('Storage in Codit: Local does not seem to be available.  Is the module Codit: Local enabled?');
    return FALSE;
  }
}

/**
 * Creates an enabled block from boilerplate files.
 *
 * @param object $o_block
 *   The block object with block details.
 */
function _codit_blocks_create_new_block(&$o_block) {
  // Check for required properties to build the block.
  // Must not exist and have no errors set.
  if ((empty($o_block->exists)) && (empty($o_block->error))) {
    // Criteria met, so create from boilerplate.
    $boilerplate_dir = drupal_get_path('module', 'codit_blocks') . '/boilerplate/blocks/block_bin/_a_sample_block';
    // Make the directory in the enabled state.
    $o_block->location = codit_path_to_local('blocks') . 'block_bin/' . $o_block->name;
    // Copy and rename the entire boilerplate.
    $o_block->built = drush_copy_dir($boilerplate_dir, $o_block->location);

    if (empty($o_block->built)) {
      // The block was not built.
      $o_block->error[] = dt("The block delta '!blockname' could not be created because they already exist at !location.", array('!blockname' => $o_block->name, '!location' => $o_block->location));
    }
    else {
      // The block was built.
      $o_block->msg[] = dt("The block files for '!blockname' were created at !location.", array('!blockname' => $o_block->name, '!location' => $o_block->location));

      // Modify the template to be specific to the block.
      // Read boilerplate template file.
      $template_uri = "$o_block->location/a_sample_block.tpl.php";
      $template_text = file_get_contents($template_uri);
      // Replace token with block delta.
      $template_text = str_replace('{BLOCK_DELTA}', $o_block->name, $template_text);
      // Save data to temp file.
      $temp_url = drush_save_data_to_temp_file($template_text);

      // Copy temp file to the actual tpl file.
      $o_block->tpl_completed = drush_copy_dir($temp_url, "{$o_block->location}/codit_blocks_{$o_block->name}.tpl.php");

      // The temp file gets removed automatically by Drush.
      // Register the orignal tpl for deletion.
      drush_register_file_for_deletion($template_uri);

      if (empty($o_block->tpl_completed)) {
        // The block was not renamed and rebuilt.
        $o_block->error[] = dt("The block tpl '!blockname.tpl.php' was not created for some reason at !location.", array('!blockname' => $o_block->name, '!location' => $o_block->location));
      }
      else {
        // The tpl was built.
        $o_block->msg[] = dt("The block tpl '!blockname.tpl.php' was created at !location.", array('!blockname' => $o_block->name, '!location' => $o_block->location));
      }
    }
  }
}

/**
 * Enables an existing disabled Codit:block.
 *
 * @param string $blockname
 *   The block delta of the block to enable.
 *
 * @return bool
 *   Return True if the block is enabled.
 */
function drush_codit_blocks_enable($blockname = '') {
  // Try to find the block.
  if (!empty($blockname)) {
    $o_block = new stdClass();
    $o_block->name = $blockname;
    if (codit_blocks_dir_exists($o_block)) {
      // The block exists, so check its state.
      if ($o_block->enabled) {
        // The block is already enabled.
        $o_block->msg[] = dt("The block '!blockname' is enabled.", array('!blockname' => $o_block->name));
        $return = TRUE;
      }
      else {
        // The block exists but is disabled, so enable it by changing the
        // directory name to not start with an underscore.
        $enabled = drush_move_dir($o_block->location_disabled, $o_block->location);
        if ($enabled) {
          // Means the enable was successful.
          $return = TRUE;
          $o_block->enabled = TRUE;
          $o_block->success[] = dt("The block '!blockname' has been enabled.", array('!blockname' => $o_block->name));
          // Clear the Codit Blocks registered block cache to pick up changes.
          cache_clear_all('codit_blocks_block_list', 'cache');
        }
        else {
          // For some reason the move failed.
          $return = FALSE;
          $o_block->enabled = FALSE;
          $o_block->err[] = dt("The block '!blockname' failed to enable. Perhaps a file permission issue?", array('!blockname' => $o_block->name));
        }
      }
    }
    else {
      // This block does not exist.
      $return = FALSE;
      $o_block->err[] = dt("The block '!blockname' Does not exist.", array('!blockname' => $o_block->name));
      $o_block->msg[] = dt("Suggest using 'drush codit-blocks-add !blockname' to create this block.", array('!blockname' => $o_block->name));
    }
  }
  else {
    // No block name specified.
    $return = TRUE;
    $o_block->err[] = dt("You must specify a block name to enable.  Example: 'drush enable my_block'");
  }

  codit_drush_process_messages($o_block);

  return $return;
}


/**
 * Disables an existing Codit:block.
 *
 * @param string $blockname
 *   The block delta of the block to disable.
 *
 * @return bool
 *   Return True if the block is disabled.
 */
function drush_codit_blocks_disable($blockname = '') {
  // Try to find the block.
  if (!empty($blockname)) {
    $o_block = new stdClass();
    $o_block->name = $blockname;
    if (codit_blocks_dir_exists($o_block)) {
      // The block exists, so check its state.
      if (!$o_block->enabled) {
        // The block is already disabled.
        $o_block->msg[] = dt("The block '!blockname' is disabled.", array('!blockname' => $o_block->name));
        $o_block->enabled = FALSE;
        $return = TRUE;
      }
      else {
        // The block exists but is enabled, so disable it by changing the
        // directory name to start with an underscore.
        $o_block->enabled = !drush_move_dir($o_block->location, $o_block->location_disabled);
        if (!$o_block->enabled) {
          // Means the disable was successful.
          $return = TRUE;
          $o_block->success[] = dt("The block '!blockname' has been disabled.", array('!blockname' => $o_block->name));
          // Clear the Codit Blocks registered block cache to pick up changes.
          cache_clear_all('codit_blocks_block_list', 'cache');
        }
        else {
          // For some reason the move failed.
          $return = FALSE;
          $o_block->err[] = dt("The block '!blockname' failed to disable. Perhaps a file permission issue?", array('!blockname' => $o_block->name));
        }
      }
    }
    else {
      // This block does not exist.
      $return = FALSE;
      $o_block->err[] = dt("The block '!blockname' Does not exist.", array('!blockname' => $o_block->name));
      $o_block->msg[] = dt("Suggest using 'drush codit-blocks-add !blockname --enable=false' to create a disabled block.", array('!blockname' => $o_block->name));
    }
  }
  else {
    // No block name specified.
    $return = TRUE;
    $o_block->err[] = dt("You must specify a block name to enable.  Example: 'drush codit-blocks-enable my_block'");
  }

  codit_drush_process_messages($o_block);

  return $return;
}

/**
 * Clears the Codit:Blocks cache and re-registers the enabled blocks.
 */
function drush_codit_blocks_cache_clear_block_list() {
  // Clear the Codit Blocks registered block cache to pick up any changes.
  cache_clear_all('codit_blocks_block_list', 'cache');
  $count = count(codit_blocks_get_blocks());
  drush_log("Codit: Blocks cache has been cleared and $count blocks re-registered.", 'success');
}

/**
 * Lists all the blocks currently enabled by Codit Blocks.
 */
function drush_codit_blocks_list() {
  drush_codit_blocks_cache_clear_block_list();
  // Get the list of registered blocks from cache.
  $blocks = codit_blocks_get_blocks();
  if (!empty($blocks)) {
    drush_print(dt('The following blocks are enabled by Codit Blocks: '));
    foreach (is_array($blocks) ? array_keys($blocks) : array() as $block) {
      // Output each block delta.
      drush_print("-  $block");
    }
  }
}

/**
 * Initializes the local storage if it does not already exist.
 */
function drush_codit_blocks_local_init() {
  $codit_local_dir = drupal_get_path('module', 'codit_local') . '/blocks/';
  $msg_vars = array(
    '@path' => $codit_local_dir,
  );
  // Check to see if the directory already exists.
  if (!file_exists($codit_local_dir)) {
    // Does not exists, so init the directory and sample by copying boilerplate.
    $boilerplate_dir = drupal_get_path('module', 'codit_blocks') . '/boilerplate/blocks';
    $success = drush_copy_dir($boilerplate_dir, $codit_local_dir, FILE_EXISTS_ABORT);
    // Make sure it worked.
    if ($success) {
      // It worked.
      drush_log(dt('The Codit: Blocks local storage was created at: @path', $msg_vars), 'success');
    }
    else {
      // It fails.
      drush_log(dt('The Codit: Blocks local storage Failed to create at: @path', $msg_vars), 'error');
    }
  }
  else {
    // The blocks directory already exists in codit local.
    drush_log(dt('The Codit: Blocks local storage already exists at: @path', $msg_vars), 'error');
  }
}


/**
 * Adds a cache clear option for Codit: Blocks.
 */
function codit_blocks_drush_cache_clear(&$types) {
  $types['codit_blocks block content'] = 'drush_codit_blocks_cache_flush';
  $types['codit_blocks block list'] = 'drush_codit_blocks_cache_clear_block_list';
}

/**
 * Clears the cache of all block content cached by Codit: Blocks.
 */
function drush_codit_blocks_cache_flush() {
  cache_clear_all('codit_blocks_', 'cache', TRUE);
  drush_log(dt('Codit: Blocks -> The caches for any Codit Blocks have been cleared.', array()), 'success');
}
