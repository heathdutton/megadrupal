This module extends Drupal's block visibility settings, for ease of
configuration and deployment.


FEATURES
--------

 * Allows PHP block visibility to be defined in code.
 * Allows theme specific block visibility code.
 * Removes reliance on core php module and eval() for defining
   block visibility.


USAGE
-----

1. Edit a block.
2. Pick one or more of the applicable callback functions from the
   list provided.
3. Implement the functions as follows:

<?php
/**
 * Block visibility callback for the search form block.
 *
 * @param object $block
 *   The block object.
 *
 * @return bool
 *   TRUE if the block should be displayed on the current page.
 */
function _search_form_block_visibility($block) {
  if ($block->theme == 'my_theme') {
    return !drupal_is_front_page();
  }
  return arg(0) == 'node' && is_numeric(arg(1));
}
?>

Note that if multiple callbacks have been implemented for a block, the most
specific match will always be used.

POSSIBLE EXTENSIONS
-------------------

 * Allow users to specify their own function names for each block.
 * Create a hook which can be implemented by one or more modules to determine
   block visibility (similar to hook_file_download()).
 * Create an info hook which can be used to specify callback names and override
   other block visibility settings, including user roles.

SIMILAR PROJECTS
----------------

 * Features Extra: Allows exporting of block visibility settings for
   deployment, but still requires eval() and php code entry through the
   admin interface.
 * Block Page Visibility: Allows visibility options to be defined through a
   hook (D5/D6 only). This module provides similar functionality to
   hook_block_info() in Drupal 7 and doesn't provide php-based visibility.

CREDITS

Current maintainer: Mark Pavlitski

This project has been sponsored by:

 * Microserve
   Microserve is a Drupal web development agency based in Bristol, UK,
   specialising in Drupal Development, Drupal Training, Hosting and
   Peer Reviews.
