<?php

/**
 * @file
 * Access plugin to determine if page is not in a pager.
 * @todo It would be nice to configure the identifier for the pager GET variable name.
 */

$plugin = array(
  'title' => t("★ Not Paged"),
  'description' => t('Control access by pager indicator in URL.'),
  'callback' => 'hap_not_paged_access_check',
  'summary' => 'hap_not_paged_access_summary',
);

/**
 * Access callback.
 */
function hap_not_paged_access_check($conf, $context) {
  if (isset($_GET['page'])) {
    // If identifier is 0 or FALSE, it will return TRUE. Any other value will
    // imply that it is using a pager, either numeric or with strings.
    return !($_GET['page']);
  }

  // If the identifier isn't set, then there is no pager.
  return TRUE;
}

/**
 * Summary callback.
 */
function hap_not_paged_access_summary($conf, $context) {
  return t('there is no pager');
}
