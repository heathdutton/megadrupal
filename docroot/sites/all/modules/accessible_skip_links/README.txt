Accessible Skip Links generates skip links to any configured block on a page.
Skip links are a way to provide visually impaired users the ability to skip to
relevant content, which aligns with Section 508 best practices.

Using this module, you can specify a "Skip Link Title" for any block. When a
block with this configuration is displayed, an anchor link will be added to the
start of it's content, and a link to it will appear in the "Skip links block",
which should be configured and added to the first region of a site's theme.

The module also offers up the list of links it generates to drupal alter through
drupal_alter().

To add or modify the links before they are displayed, use the following:
/**
 * Implements hook_skip_links_alter().
 */
YOUR_MODULE_skip_links_alter(&$links, &$page) {
  // Do things.
}
