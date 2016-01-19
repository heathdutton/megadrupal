<?php
/**
 * @file
 * Special plugin to
 */
$plugin = array (
  'title' => XU_NAME . t(' administration'),
  'description' => t('Library providing admin functions.'),
  'xu_type' => 'library'
);

/**
 * Callback to provide the Xu admin page
 */
function xu_admin_page_plugins() {
  xu('plugin_tools');

  $output = '';

  $output .= '<h2>Installed helpers</h2>';

  $options = array(
    'extra_keys' => array(
      'xu_type' => t('Type'),
    )
  );
  $output .= xu_ctools_plugins_list('xu', 'helper', $options);

  return $output;
}
