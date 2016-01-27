<?php

/**
 * @file
 * Page callbacks for the System Status Ajax module.
 */

/**
 * Themes the status message.
 *
 * @param array $variables
 *   An associative array containing:
 *   - message: The status message text.
 *   - type: The status type, i.e. "status", "update". This value is usually
 *     arg(2) of admin/report/* paths rendering the full status check.
 *
 * @ingroup themeable
 */
function theme_system_status_ajax_message(array $variables) {

  // Build CSS ID and classes.
  $type = drupal_clean_css_identifier($variables['type']);
  $attributes = array(
    'id' => drupal_html_id("system-status-ajax-message"),
    'class' => array(
      'system-status-ajax-message',
      'system-status-ajax-message-' . $type,
    ),
  );

  // Sanitize the message, wrap in a span tag.
  $message = filter_xss_admin($variables['message']);
  return '<span' . drupal_attributes($attributes) . '>' . $message . '</span>';
}

/**
 * Sets the initial status message.
 *
 * @param string $message
 *   The message text.
 * @param string $type
 *   The status type, i.e. "status", "update". This value is usually arg(2) of
 *   admin/report/* paths rendering the full status check.
 */
function system_status_ajax_set_message($message, $type) {
  $variables = array(
    'message' => $message,
    'type' => $type,
  );
  $message = theme('system_status_ajax_message', $variables);
  drupal_set_message($message, 'warning system-status-ajax-messages');
}

/**
 * Loads relevant CSS and JavaScript files.
 */
function system_status_ajax_attach_files() {
  $path = drupal_get_path('module', 'system_status_ajax');
  drupal_add_js('misc/ajax.js');
  drupal_add_js($path . '/system_status_ajax.js');
  drupal_add_css($path . '/system_status_ajax.css');
}

/**
 * Gets the system status data.
 *
 * @return array
 *   An associative array of data containing:
 *   - status: The class that the wrapper will be updated with, i.e. "status",
 *     "warning", "error".
 *   - message: The status message text.
 */
function system_status_ajax_status_data() {
  module_load_include('inc', 'system', 'system.admin');

  // Set the message based on the status.
  $args = array('@status' => url('admin/reports/status'));
  if (!system_status(TRUE)) {
    $status = 'status';
    $message = t('There were no problems detected with your Drupal installation. Check the <a href="@status">status report</a> for more information.', $args);
  }
  else {
    $status = 'error';
    $message = t('One or more problems were detected with your Drupal installation. Check the <a href="@status">status report</a> for more information.', $args);
  }

  return array(
    'status' => $status,
    'message' => $message,
  );
}

/**
 * Menu callback; Return system status and messages.
 */
function system_status_ajax_status_page() {
  return array(
    '#type' => 'ajax',
    '#commands' => array(
      array(
        'command' => 'systemStatusAjax',
        'data' => system_status_ajax_status_data(),
      ),
    ),
  );
}

/**
 * Menu callback; Provide the administration overview page.
 *
 * @see system_admin_config_page()
 */
function system_status_ajax_admin_config_page() {
  module_load_include('inc', 'system', 'system.admin');
  system_status_ajax_attach_files();

  // Set placeholder for status message.
  if (user_access('administer site configuration')) {
    system_status_ajax_set_message(t('Checking system status...'), 'status');
  }

  $blocks = array();
  if ($admin = db_query("SELECT menu_name, mlid FROM {menu_links} WHERE link_path = 'admin/config' AND module = 'system'")->fetchAssoc()) {
    $result = db_query("
      SELECT m.*, ml.*
      FROM {menu_links} ml
      INNER JOIN {menu_router} m ON ml.router_path = m.path
      WHERE ml.link_path <> 'admin/help' AND menu_name = :menu_name AND ml.plid = :mlid AND hidden = 0", $admin, array('fetch' => PDO::FETCH_ASSOC));
    foreach ($result as $item) {
      _menu_link_translate($item);
      if (!$item['access']) {
        continue;
      }
      // The link description, either derived from 'description' in hook_menu()
      // or customized via menu module is used as title attribute.
      if (!empty($item['localized_options']['attributes']['title'])) {
        $item['description'] = $item['localized_options']['attributes']['title'];
        unset($item['localized_options']['attributes']['title']);
      }
      $block = $item;
      $block['content'] = '';
      $block['content'] .= theme('admin_block_content', array('content' => system_admin_menu_block($item)));
      if (!empty($block['content'])) {
        $block['show'] = TRUE;
      }

      // Prepare for sorting as in function _menu_tree_check_access().
      // The weight is offset so it is always positive, with a uniform 5-digits.
      $blocks[(50000 + $item['weight']) . ' ' . $item['title'] . ' ' . $item['mlid']] = $block;
    }
  }
  if ($blocks) {
    ksort($blocks);
    return theme('admin_page', array('blocks' => $blocks));
  }
  else {
    return t('You do not have any administrative items.');
  }
}
