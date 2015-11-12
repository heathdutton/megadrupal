<?php
/**
 * @file
 * Drush Rush file to include operations.
 * Functions for file and directory management.
 * @see operations
 */

include dirname(__FILE__) . '/rush.filesystem.inc';
include dirname(__FILE__) . '/rush.host.inc';
include dirname(__FILE__) . '/rush.repo.inc';
include dirname(__FILE__) . '/rush.site.inc';
include dirname(__FILE__) . '/rush.db.inc';
include dirname(__FILE__) . '/rush.utilities.inc';

/**
 * @addtogroup operations Operations
 * @brief Carry out operations such as to download files, create databases,
 * commit code, install Drupal sites, and run other Drush commands.
 * @details Operations parsed from a rush file are mapped to Rush operation
 * functions.  For example, the rush file syntax `[f] = create_build_directory`
 * maps to the function rush_op_create_build_directory.
 *
 * Rush operations functions accept a single parameter called
 * '$params', which is an associative array of all rush parameters.
 * The parameters in the params array may come from any combination of:
 * 1. A set of values derived automatically by Drush Rush
 * 2. Values parsed from Rush param files, which override derived parameters
 * 3. Parameters at call time, which override previous parameters either
 *    parsed or derived
 */
