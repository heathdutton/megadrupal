<?php

$plugin = array(
    'name' => 'Expire',
    'module' => 'steroids',
    'admin_settings_form' => 'steroids_expire_admin_settings_form',

    'hook_comment_delete' => 'steroids_expire_comment_delete',
    'hook_comment_insert' => 'steroids_expire_comment_insert',
    'hook_comment_publish' => 'steroids_expire_comment_publish',
    'hook_comment_unpublish' => 'steroids_expire_comment_unpublish',
    'hook_comment_update' => 'steroids_expire_comment_update',

    'hook_node_delete' => 'steroids_expire_node_delete',
    'hook_node_insert' => 'steroids_expire_node_insert',
    'hook_node_revision_delete' => 'steroids_expire_node_revision_delete',
    'hook_node_update' => 'steroids_expire_node_update',

    'hook_steroids_expire_all' => 'steroids_expire_all',
);

/**
 * Implements the former hook_settings().
 */
function steroids_expire_admin_settings_form() {
  $settings = variable_get('steroids_expire', array());

  $form['steroids_expire'] = array(
      '#type' => 'fieldset',
      '#title' => t('Expire'),
      '#tree' => TRUE,
      '#collapsed' => TRUE,
      '#collapsible' => TRUE,
  );

  $form['steroids_expire']['expire_flush_front'] = array(
      '#type' => 'checkbox',
      '#title' => t('Expire front page'),
      '#default_value' => (isset($settings['expire_flush_front'])) ? $settings['expire_flush_front'] : TRUE,
      '#description' => t('Always expire the frontpage along an expire request.'),
  );

  $form['steroids_expire']['expire_flush_node_terms'] = array(
      '#type' => 'checkbox',
      '#title' => t('Expire node term pages'),
      '#default_value' => (isset($settings['expire_flush_node_terms'])) ? $settings['expire_flush_node_terms'] : TRUE,
      '#description' => t('When expiring a node, expire taxonomy pages for its terms.'),
  );

  $form['steroids_expire']['expire_include_base_url'] = array(
      '#type' => 'checkbox',
      '#title' => t('Include base URL in expires'),
      '#default_value' => (isset($settings['expire_include_base_url'])) ? $settings['expire_include_base_url'] : FALSE,
      '#description' => t('Include the base URL in expire requests.'),
  );

  $form['steroids_expire']['steroids_expire_custom_urls'] = array(
      '#type' => 'textarea',
      '#title' => t('Extra hostnames'),
      '#description' => t('List of hostnames to also use for expire requests when the Include base URL is set. One per line.'),
      '#default_value' =>  (isset($settings['steroids_expire_custom_urls'])) ? $settings['steroids_expire_custom_urls'] : '',
  );

  return $form;
}

/**
 * Delegation of hook_comment_insert().
 */
function steroids_expire_comment_insert($comment) {
  if (empty($comment->nid)) {
    return;
  }
  $node = node_load($comment->nid);
  if ($node) {
    steroids_expire_node($node);
  }
}

/**
 * Delegation of hook_comment_update().
 */
function steroids_expire_comment_update($comment) {
  if (empty($comment->nid)) {
    return;
  }
  $node = node_load($comment->nid);
  if ($node) {
    steroids_expire_node($node);
  }
}

/**
 * Delegation of hook_comment_publish().
 */
function steroids_expire_comment_publish($comment) {
  if (empty($comment->nid)) {
    return;
  }
  $node = node_load($comment->nid);
  if ($node) {
    steroids_expire_node($node);
  }
}

/**
 * Delegation of hook_comment_unpublish().
 */
function steroids_expire_comment_unpublish($comment) {
  if (empty($comment->nid)) {
    return;
  }
  $node = node_load($comment->nid);
  if ($node) {
    steroids_expire_node($node);
  }
}

/**
 * Delegation of hook_comment_delete().
 */
function steroids_expire_comment_delete($comment) {
  if (empty($comment->nid)) {
    return;
  }
  $node = node_load($comment->nid);
  if ($node) {
    steroids_expire_node($node);
  }
}

/**
 * Delegation of hook_node_insert().
 */
function steroids_expire_node_insert($node) {
  if (empty($node->nid)) {
    return;
  }
  else {
    steroids_expire_node($node);
  }
}

/**
 * Delegation of hook_node_update().
 */
function steroids_expire_node_update($node) {
  if (empty($node->nid)) {
    return;
  }
  else {
    steroids_expire_node($node);
  }
}

/**
 * Delegation of hook_node_delete().
 */
function steroids_expire_node_delete($node) {
  if (empty($node->nid)) {
    return;
  }
  else {
    steroids_expire_node($node);
  }
}

/**
 * Delegation of hook_node_revision_delete().
 */
function steroids_expire_node_revision_delete($node) {
  if (empty($node->nid)) {
    return;
  }
  else {
    steroids_expire_node($node);
  }
}

/**
 * Delegation of hook_user_insert().
 */
function steroids_expire_user_insert(&$edit, $account, $category = NULL) {
  if (empty($account->uid)) {
    return;
  }
  else {
    steroids_expire_user($account);
  }
}


/**
 * Delegation of hook_user_delete().
 */
function steroids_expire_user_delete($account) {
  if (empty($account->uid)) {
    return;
  }
  else {
    steroids_expire_user($account);
  }
}

/**
 * Delegation of hook_user_update().
 */
function steroids_expire_user_update(&$edit, $account, $category = NULL) {
  if (empty($account->uid)) {
    return;
  }
  else {
    steroids_expire_user($account);
  }
}

/**
 * Expires a user page.
 */
function steroids_expire_user($account) {
  $paths[] = 'user/' . $account->uid;
  $flushed = steroids_expire_cache_derivative($paths, $account);
}

/**
 * Expires a node from the cache; including related pages.
 */
function steroids_expire_node(&$node) {
  $paths = array();
  $settings = variable_get('steroids_expire', array());

  // Check node object
  if (empty($node->nid)) {
    return FALSE;
  }

  // Expire this node
  $paths['node'] = 'node/' . $node->nid;

  // If promoted to front page, expire front page
  if (isset($settings['expire_flush_front']) && $settings['expire_flush_front']) {
    $paths['front'] = '<front>';
  }

  // Get taxonomy terms and flush
  if (module_exists('taxonomy') && isset($settings['expire_flush_node_terms']) && $settings['expire_flush_node_terms']) {
    $terms = steroids_expire_taxonomy_node_get_tids($node->nid);
    foreach ($terms as $term) {
      $uri = taxonomy_term_uri($term);
      $paths['term' . $term->tid] = $uri['path'];
    }
  }

  // Flush array of paths
  if (!empty($paths)) {
    $flushed = steroids_expire_cache_derivative($paths, $node);
  }
}

/**
  * Expire the entire cache bin
  */
function steroids_expire_all() {
  $urls = array('');
  return steroids_expire_cache_derivative($urls, $node = NULL, TRUE);
}

/**
 *
 * Returns all tids related to the give node with $nid
 *
 * @param integer $nid
 */
function steroids_expire_taxonomy_node_get_tids($nid) {
  $tids = db_select('taxonomy_index', 'ti')
            ->fields('ti', array('tid'))
            ->condition('ti.nid', $nid)
            ->execute()
            ->fetchCol();
  return taxonomy_term_load_multiple($tids);
}

/**
 * Finds all possible paths/redirects/aliases given the root path.
 *
 * @param $paths
 *   Array of current URLs
 * @param $node
 *   node object
 */
function steroids_expire_cache_derivative($paths, &$node = NULL, $wildcard = FALSE) {
  global $base_path;
  global $base_url;

  $settings = variable_get('steroids_expire', array());
  $urls = array();

  // No paths.
  if (empty($paths)) {
    return FALSE;
  }

  $site_frontpage = variable_get('site_frontpage', 'node');

  foreach ($paths as $path) {

    // Special front page handling
    if ($path == $site_frontpage || $path == '' || $path == '<front>') {
      $urls[] = $base_path;
      $urls[] = $base_path . 'rss.xml';
      $urls[] = $base_path . $site_frontpage;
    }

    // Add given path
    if ($path != '<front>') {
      $urls[] = $base_path . $path;
    }

    // Path alias
    $path_alias = url($path, array('absolute' => FALSE));
    $urls[] = $path_alias;
  }

  // Allow other modules to modify the list prior to expiring
  drupal_alter('steroids_expire_list_alter', $urls, $node, $paths);

  // No paths.
  if (empty($urls)) {
    return FALSE;
  }

  $urls = array_unique($urls);

  // Add on the url to these paths
  $hostnames = array();
  if (isset($settings['expire_include_base_url']) && $settings['expire_include_base_url']) {
    $hostnames = steroids_expire_get_hostnames($node);
  }

  // Purge urls.
  $modules = module_implements('steroids_purge');
  foreach ($modules as $module) {
    module_invoke($module, 'steroids_purge', $urls, $wildcard, $hostnames);
  }

  return count($urls);
}

/**
 * Get all base url's where this node can appear. Domain access support.
 *
 * @param $node
 *   node object
 * @return array
 *   array(0 => array($base_url . '/'))
 */
function steroids_expire_get_hostnames(&$node) {
  global $base_url, $base_path;

  $base_urls = array();
  $domains = array();

  // the url the site is currently running on
  $base_urls[] = $_SERVER['HTTP_HOST'];

  // the list of urls defined in steroids' settings page
  $settings = variable_get('steroids_expire', array());
  $custom_urls = isset($settings['steroids_expire_custom_urls']) ? $settings['steroids_expire_custom_urls'] : array();
  if (!empty($custom_urls)) {
    foreach(explode("\n", $custom_urls) as $url) {
      $base_urls[] = trim($url); // trim so we don't get newlines included
    }
  }

  // Get list of URL's if using domain access
  if (module_exists('domain') && isset($node->domains)) {
    // Get domains from node/user object
    foreach ($node->domains as $key => $domain_id) {
      if ($key != $domain_id) {
        continue;
      }
      $domains[$domain_id] = $domain_id;
    }
    // Get domains from database
    foreach (steroids_expire_get_domains($node) as $domain_id) {
      $domains[$domain_id] = $domain_id;
    }
    // Get aliases and set base url
    foreach ($domains as $domain_id) {
      $domain = domain_lookup($domain_id);
      if ($domain['valid'] == 1) {
        if (isset($domain['path'])) {
          $base_urls[] = $domain['path'];
        }
        if (is_array($domain['aliases'])) {
          foreach ($domain['aliases'] as $alias) {
            if ($alias['redirect'] != 1) {
              $temp_domain = array('scheme' => $domain['scheme'], 'subdomain' => $alias['pattern']);
              $base_urls[] = domain_get_path($temp_domain);
            }
          }
        }
      }
    }
  }

  // remove duplicates if needed
  $base_urls = array_map('strtolower', array_unique($base_urls));

  // @todo This can be written a lot nicer: remove the trailing slash if needed
  foreach($base_urls as &$url) {
    if (substr($url, -1) == '/') {
      $url = substr($url, 0, -1);
    }
  }

  return $base_urls;
}

/**
 * Get domains the node is currently published to
 *
 * @param $node
 *   node object
 * @return array
 *   array('$gid' => $gid)
 */
function steroids_expire_get_domains(&$node) {
  $domains = array();

  if ($node->nid) {
    $gids = $query = db_select('domain_access', 'da')
              ->fields('da', array('gid'))
              ->condition('da.nid', $node->nid)
              ->execute()
              ->fetchCol();
    foreach($gids as $gid) {
      $domains[$gid] = $gid;
    }
  }
  elseif ($node->mail && $node->name) {
    $gids = $query = db_select('domain_editor', 'de')
              ->fields('de', array('domain_id'))
              ->condition('de.uid', $node->uid)
              ->execute()
              ->fetchCol();
    foreach($gids as $gid) {
      $domains[$gid] = $gid;
    }
  }
  return $domains;
}
