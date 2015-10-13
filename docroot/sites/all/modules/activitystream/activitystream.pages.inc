<?php

/**
 * @file
 * Page callbacks for the Activity Stream module.
 */

/**
 * Menu callback; show the default activity stream page.
 */
function activitystream_default_page($account = NULL, $count = 20) {
  $uid = is_object($account) ? $account->uid : NULL;
  if (!empty($uid)) { // We need a purdier title for user pages.
    drupal_set_title(t("@name's Activity Stream", array('@name' => format_username($account))));
  } // "Pikachu's Activity Stream" sounds more "active" than "Activity Stream for Pikachu."

  $services = activitystream_services_load();
  $nodes = activitystream_items_load($uid, $count);

  $build['activitystream_items']['#theme_wrappers'][]  = 'activitystream_items_wrapper';
  $build['activitystream_items']['#attached']['css'][] = drupal_get_path('module', 'activitystream') . '/activitystream.css';

  foreach ($nodes as $node) {
    $service = $node->activitystream_service[LANGUAGE_NONE][0]['value'];
    $build['activitystream_items'][$node->nid] = array(
      '#node'     => $node,
      '#service'  => $services[$service],
      '#theme'    => 'activitystream_item__' . $service,
    );
  }

  if (count($nodes) == 0) {
    $build['activitystream_sadness'] = array(
      '#markup' => t('No activity items have been saved for this stream yet.'),
    );
  }

  $build['pager'] = array(
    '#theme' => 'pager',
  );

  return $build;
}

/**
 * Block callback; show the default activity stream block.
 */
function activitystream_default_block($account = NULL, $count = 10, $type = 'wide') {
  // Piggy-back off the default page, but without pagers.
  $build = activitystream_default_page($account, $count);
  unset($build['pager']); // Breaks with multiple pages.

  $build['activitystream']['see-more'] = array(
    '#href'   => is_object($account) ? 'user/' . $account->uid . '/activity-stream' : 'activity-stream',
    '#prefix' => '<div class="activitystream-see-more">',
    '#title'  => t('View full activity stream »'),
    '#suffix' => '</div>',
    '#type'   => 'link',
  );

  return $build;
}

/**
 * Default theme for the wrapper around all Activity Stream items.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: A render containing the user's achievements page.
 */
function theme_activitystream_items_wrapper($variables) {
  return '<div id="activitystream-items">' . $variables['element']['#children'] . '</div>';
}
