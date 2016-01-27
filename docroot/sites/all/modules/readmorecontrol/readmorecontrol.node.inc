<?php

/**
 * @file
 * This has been deliberately left empty.
 *
 * The module was incorrectly using hook_hook_info() as a method of loading
 * external files, when this hook is actually for modules that define new groups
 * of hooks that any module can autoload without explicitly including these.
 *
 * If another module had defined this group, the duel definition would have
 * resulted in Drupal ignoring the group definition.
 */
