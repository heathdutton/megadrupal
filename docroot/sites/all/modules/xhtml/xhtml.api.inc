<?php /* -*- mode: php; indent-tabs-mode: nil; tab-width: 2; -*- */

/**
 * @file
 * The hooks which the xhtml module uses.
 */

/**
 * Disable outputting XHTML for the current page.
 *
 * Called by _xhtml_use_xhtml(). Called during hook_init() (this makes
 * detecting certain conditions where XHTML output should be disabled
 * harder).
 *
 * @return
 *   If any implementation of this hook returns TRUE, the page will be
 *   delivered using Drupal's default of text/html.
 *
 * @ingroup hooks
 * @see hook_init()
 */
function hook_xhtml_disable() {
  /*
   * Provide a way for users to view pages even if they have broken
   * XHTML and thus cause the users' browsers to display a
   * well-formedness error message instead of the page.
   */
  if (isset($_GET['html'])) {
    return TRUE;
  }
}
