<?php

/**
 * @file
 * dgrep - Drush commands to grep through content using PCREs.
 *
 * Copyright (C) 2010  Antonio Ospite <ospite@studenti.unina.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

/**
 * Implementation of hook_drush_command().
 */
function dgrep_drush_command() {
  $items = array();

  $items['grep'] = array(
    'description' => "grep through content using PCREs.",
    'arguments' => array(
      'pattern' => 'The PCRE-style regular expression to match.',
    ),
    'options' => array(
      'content-types' => 'Comma delimited list of content types to search into (node,comment).',
      'replace' => array(
        'description' => 'Do a preg_replace instead of preg_match.  Value is used as the replacement for the preg_replace; use \1 as a back-reference to the first match.',
        'value' => 'required',
        'example-value' => 'replacement',
      ),
    ),
    'examples' => array(
      'drush grep \'#https://[^"]*#\' --content-types=node' => 'Search for URLs in nodes',
    ),
    'aliases' => array('dgrep'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL, // XXX: maybe something less.
  );

  return $items;
}

/**
 * Implementation of hook_drush_help().
 */
function dgrep_drush_help($section) {
  switch ($section) {
    case 'drush:grep':
      return dt("This command will allow you to grep through content using PCREs.");
  }
}

/*
 * Implementation of drush_hook_COMMAND_validate().
 */
function drush_dgrep_grep_validate($pattern) {
  $ret = @preg_match($pattern, '');
  if ($ret === FALSE) {
    return drush_set_error(dt('Error: pattern should be a valid PCRE.'));
  }
}

/**
 * This is where the action takes place.
 */
function drush_dgrep_grep($pattern) {
  $replace = drush_get_option('replace', FALSE);
  $content_types = drush_get_option('content-types');
  if ($content_types) {
    $list = explode(',', $content_types);
  }
  else {
    // TODO: Add 'block' to list
    $list = array('node');
    if (drush_drupal_major_version() == 6) {
      $list[] = 'comment';
    }
  }

  if (in_array('node', $list)) {
    $sql = 'SELECT nid FROM {node}';
    $set_nids = array();
    if (function_exists('db_result')) {
      $nids = db_query($sql);
      while ($nid = db_result($nids)) {
        $set_nids[] = $nid;
      }
    }
    else {
      $set_nids = db_query($sql)->fetchCol();
    }
    foreach ($set_nids as $nid) {
      $node = node_load($nid);
      $body = FALSE;
      if (isset($node->body)) {
        $body_var = &$node->body;
        $body = $node->body;
        if (is_array($body)) {
          $language = isset($node->language) ? $node->language : LANGUAGE_NONE;
          if (isset($body[$language][0]['value'])) {
            $body = $body[$language][0]['value'];
            $body_var = &$node->body[$language][0]['value'];
          }
        }
      }
      if (is_string($body)) {
        $ret = preg_match($pattern, $body, $matches);
        if ($ret) {
          $nid_str = sprintf("%-4d", $nid);
          drush_print('Node: '. $nid_str . "\tTitle: " . $node->title);
          drush_print("\t\tURL: " . drupal_get_path_alias('node/'.$nid));
          drush_print("\t\tMatch: " . $matches[0]);

          if ($replace) {
            $updated = preg_replace($pattern, $replace, $body);
            if ($updated != $body) {
              $body_var = $updated;
              node_save($node);
            }
          }
          drush_print();
        }
      }
    }
  }

  if (in_array('comment', $list)) {
    $sql = 'SELECT cid FROM {comments}';
    $cids = db_query($sql);

    while ($cid = db_result($cids)) {
      $comment = _comment_load($cid);

      $ret = preg_match($pattern, $comment->comment, $matches);
      if ($ret) {
        $cid_str = sprintf("%-4d", $cid);
        drush_print('Comment: '. $cid_str . "\tTitle: " . $comment->title);
        drush_print("\t\tURL: " . drupal_get_path_alias('node/'.$comment->nid) . '#comment-' . $cid );
        drush_print("\t\tMatch: " . $matches[0]);
        drush_print();
      }
    }
  }

  if (in_array('block', $list)) {
        drush_print('TODO: grepping blocks is not supported yet');
  }

}
