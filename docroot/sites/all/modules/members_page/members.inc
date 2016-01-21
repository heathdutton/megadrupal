<?php
/**
 * @file
 * Code common to both members.module and members.install files. These are
 * helper functions for the Members Page module. They were once prefixed with
 * 'drupal_' instead of 'members_' as unclaimed API type functions. Maybe
 * someday they will be part of Drupal core.
 */

/**
 * Renders a drupal block without using block_page_build().
 *
 * @param array $fields
 *   Associative array of block fields required to render the block.
 *
 * @returns $block
 *   A block for rendering by theme('block', $block);
 * @see members_page()
 */
function members_render_magic_block($fields) {
  // The process basically mirrors what Drupal does when it renders a block
  // within a region. The next few lines of code were reverse-engineered.
  // Create a request object for the block we wish render.
  $request = (object) $fields;

  // The next two functions are from the block.module file. Objects are always
  // passed by reference in PHP. Drupal typically uses arrays as arguments
  // (as is the case here). In this case both arrays have just one member.
  $render_block = _block_render_blocks(array($request));
  $build = _block_get_renderable_array($render_block);

  // Extract the single theme block from the array. The array is keyed by a
  // context label derived from the original blkObj.
  $context = $fields['module'] . '_' . $fields['delta'];
  $block = $build[$context];

  return $block;
}

/**
 * Checks if a url may be added to the menu system. (look for a 404).
 *
 * This function is required for Configurable URL's -- that is, using a Drupal
 * variable in a hook_menu() implementation. When a new url is requested, it
 * must be verified that it is available -- i.e. 404 page not found.
 *
 * @param string $url
 *   The url or Drupal alias requested.  This should be a simple string without
 *   any protocol (http:) prefix and without leading or trailing '/' characters.
 *
 * @returns boolean TRUE | FALSE
 */
function members_check_url_free($url) {
  // Translation function.
  $t = get_t();

  // First check the menu_router table.
  $test = menu_get_item($url);
  if ($test === FALSE) {
    // Looks like there is nothing in the menu_router table.
    // Now check the url_alias table.
    $test = drupal_lookup_path('source', $url);
    if ($test === FALSE) {
      // Nothing in the url_alias table either.
      // One more check is performed if the curl library is installed.
      return function_exists('curl_init') ? _curl_check_url_free($url) : TRUE;
    }
    $msg = $t('The requested url: /') . $url . $t(' is currently an alias for: /') . $test;
    watchdog('members', $msg);
    drupal_set_message($msg);
    return FALSE;
  } else {
    // This path is taken by an menu_router item.
    $msg = 'The requested url: ' . $url . ' is currently used by the "' . $test['title'] . '" module';
    watchdog('members', $msg);
    drupal_set_message($msg);
    return FALSE;
  }
}

/**
 * Helper function for members_check_url_free().
 * @see members_check_url_free()
 */
function _curl_check_url_free($url) {
  global $base_url;

  // This code should execute only if the php curl library is installed and enabled.
  $url = $base_url . '/' . $url;
  $cptr = curl_init();
  curl_setopt($cptr, CURLOPT_URL, $url);

  // No body required, just the header.
  curl_setopt($cptr, CURLOPT_NOBODY, TRUE);

  // This is a curl request to the local drupal system -- should be a fast response.
  curl_setopt($cptr, CURLOPT_CONNECTTIMEOUT, 2);
  $content = curl_exec($cptr);
  $info = curl_getinfo($cptr);
  $errs = curl_error($cptr);
  if (!empty($errs)) {
    echo "<pre>"; print_r($errs); echo "</pre>";
  }
  curl_close($cptr);

  return $info['http_code'] == '404' ? TRUE : FALSE;
}
