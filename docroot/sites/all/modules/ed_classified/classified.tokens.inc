<?php
/**
 * @file
 * Token hooks for Classified Ads module
 *
 * @copyright (c) 2010-2011 Ouest Systemes Informatiques (OSInet)
 *
 * @license General Public License version 2 or later
 *
 * Original code implementing a feature set derived from ed_classified.
 *
 * ---- Information about ed_classified ----
 *
 * Michael Curry, Exodus Development, Inc.
 * exodusdev@gmail.com
 * for more information, please visit:
 *   http://exodusdev.com/drupal/modules/ed_classified.module
 *
 * Copyright (c) 2006, 2007 Exodus Development, Inc.  All Rights Reserved.
 *
 * Licensed under the terms of the GNU Public License (GPL) version 2.  Please
 * see LICENSE.txt for license terms.  Possession and use of this code signifies
 * acceptance of license terms.
 * ---- /ed-classified ----
 */

/**
 * Implements hook_token_info().
 *
 * Core version of D6 hook_token_list($type = 'all')
 */
function classified_token_info() {
  $ret = array();

  $ret['types'] = array(
    // 'user' is defined in Core - Required so need not be redeclared.
  );

  $ret['tokens'] = array(
    'user' => array(
      'classified-ads' => array(
        'name' => t('Classified Ads list'),
        'description' =>  t('A themed list of Classified Ads by the user (max 10)'),
      ),
      'classified-ads-plain' => array(
        'name' => t('Classified Ads list'),
        'description' =>  t('A plain text list of Classified Ads by the user (max 10). Best for email.'),
      ),
      'classified-ads-url' => array(
        'name' => t('Classified Ads URL'),
        'description' => t("The URL of a user's ads list"),
        'type' => 'url',
      ),
    ),
  );

  return $ret;
}

/**
 * Implements hook_tokens().
 *
 * D7 tokens are chainable, unlike those in D6.
 *
 * token_replace() options:
 * - language: ignored
 * - callback: unused in hook_tokens() implementations
 * - clear: unused in hook_tokens() implementations
 * - sanitized
 */
function classified_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $ret = array();
  if ($type != 'user' || empty($data['user'])) {
    return $ret;
  }
  $clear = !empty($options['clear']);
  $sanitize = !empty($options['sanitize']);
  $account = $data['user'];

  $url_options = array(
    'absolute' => TRUE,
  );
  if (isset($options['language'])) {
    $url_options['language'] = $options['language'];
    $language_code = $options['language']->language;
  }


  foreach ($tokens as $name => $original) {
    switch ($name) {
      case 'classified-ads-url':
        $ret[$original] = url('user/' . $account->uid . '/classified', $url_options);
        break;

      case 'classified-ads':
        global $user;
        $min_status = ($account->uid == $user->uid || user_access('administer nodes'))
          ? 0 : 1;
        $q = db_select('node', 'n')
          ->comment(__FUNCTION__)
          ->range(0, 10);
        $cn = $q->innerJoin('classified_node', 'cn', 'n.vid = cn.vid');
        $nid_alias = $q->addField('n', 'nid', 'id');
        $q->condition('n.type', 'classified')
          ->condition('n.uid', $account->uid)
          ->condition('n.status', $min_status, '>=')
          ->orderBy("$cn.expires")
          ->orderBy("n.changed")
          ->orderBy('n.created')
          ->addTag('node_access');
        $results = $q->execute();
        $ads = _classified_list_nodes($results);
        unset($cn, $min_status, $q);
        $ret[$original] = empty($ads['#rows'])
          ? t('No Classified Ads')
          : render($ads);
        break;

      case 'classified-ads-plain':
        global $user;
        $min_status = ($account->uid == $user->uid || user_access('administer nodes'))
          ? 0 : 1;
        $q = db_select('node', 'n')
          ->comment(__FUNCTION__)
          ->fields('n', array('nid', 'title'))
          ->range(0, 10);
        $cn = $q->innerJoin('classified_node', 'cn', 'n.vid = cn.vid');
        $q->condition('n.type', 'classified')
          ->condition('n.uid', $account->uid)
          ->condition('n.status', $min_status, '>=')
          ->orderBy("$cn.expires")
          ->orderBy("n.changed")
          ->orderBy('n.created')
          ->addTag('node_access');
        $results = $q->execute();
        unset($cn, $min_status, $q);
        $ads = '';
        foreach ($results as $result) {
          // Always sanitized.
          $ads .= t("(@nid) @title\n", array('@nid' => $result->nid, '@title' => $result->title));
        }
        $ret[$original] = empty($ads) ? t('No Classified Ads') : $ads;
        break;
    }
  }

  if ($url_tokens = token_find_with_prefix($tokens, 'classified-ads-url')) {
    $ret += token_generate('url', $url_tokens, array('path' => 'user/' . $account->uid . '/ads'), $options);
  }

  return $ret;
}
