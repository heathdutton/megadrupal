<?php

/**
 * @file
 * theme functions for Comment Easy Reply module.
 */

/**
 * Returns HTML for the module's status report.
 *
 * @param array $variables
 *   An associative array containing:
 *   - comment_easy_reply_node_types: An array of node types with Comment Easy
 *     Reply enabled on.
 *
 * @ingroup themeable
 */
function theme_comment_easy_reply_status_report_page($variables) {
  $rows = array();
  $headers = array(
    'Node type',
    'Current status',
    'Default settings overrided',
    'Settings details',
    'Actions');
  $node_types = node_type_get_names();
  foreach ($node_types as $node_type => $label) {
    $override = '';
    $links = array();
    if (in_array($node_type, $variables['comment_easy_reply_node_types'])) {
      $title = t('OK');
      $class = 'ok';
      $active = '<strong>' . t('enabled') . '</strong>';
      if (_comment_easy_reply_is_node_type_override_active($node_type)) {
        $override = t('Node type specific settings used.');
      }
      else {
        $override = t('Default settings used.');
        $override .= '<br />(' . l(t('Override default settings'), 'admin/structure/types/manage/' . $node_type) . ')';
      }
      $links[] = l(t('Disable'), 'admin/config/content/comment-easy-reply/status', array('query' => array('disable' => $node_type)));
      $links[] = l(t('Edit'), 'admin/structure/types/manage/' . $node_type);
    }
    else {
      $title = t('Warning');
      $class = 'warning';
      $active = t('disabled');
      $links[] = l(t('Enable'), 'admin/config/content/comment-easy-reply/status', array('query' => array('enable' => $node_type)));
    }
    $row = array(
      array(
        'data' => $label,
      ),
      array(
        'data' => $active,
      ),
      array(
        'data' => $override,
      ),
      array(
        'data' => theme('comment_easy_reply_node_type_settings_report', array('node_type' => $node_type)),
      ),
      array(
        'data' => theme('item_list', array('items' => $links)),
      ),
    );
    $rows[] = array('data' => $row, 'class' => array($class));
  }
  return theme('table', array(
    'rows' => $rows,
    'sticky' => FALSE,
    'attributes' => array('class' => array('system-status-report')),
    'header' => $headers,
  ));
}

/**
 * Returns HTML for a node type specific settings configuration.
 *
 * @param array $variables
 *   An associative array containing:
 *   - node_type: The node type machine name.
 */
function theme_comment_easy_reply_node_type_settings_report($variables) {
  $settings = array();
  if (!_comment_easy_reply_is_node_type_active($variables['node_type'])) {
    return '';
  }
  $names = _comment_easy_reply_get_settings_names();
  foreach ($names as $name => $values) {
    if (!is_array($values) || !isset($values['report label']) || !isset($values['report callback'])) {
      continue;
    }
    $label = $values['report label'];
    $value = _comment_easy_reply_get_settings_value($name, $variables['node_type']);
    $value = call_user_func($values['report callback'], $name, $value);
    $item = $label . ' <strong>' . $value . '</strong>';
    $settings[] = array(
      'data' => $item,
    );
  }
  return theme('item_list', array('items' => $settings));
}

/**
 * Adds Comment Easy Reply tooltip markup to a quote output.
 *
 * @param array &$variables
 *   An associative array with the following structure:
 *   'comment' - The comment.
 *
 * @return string
 *   Themed quote eventually containing Comment Easy Reply toltip markup.
 */
function theme_comment_easy_reply_quote(&$variables) {
  $quote_output = _comment_easy_reply_get_quote_text($variables);
  if (isset($variables['comment_easy_reply_quote_processed']) && $variables['comment_easy_reply_quote_processed']) {
    $classes = array('comment-easy-reply-added-quote');
    $variables = drupal_array_merge_deep($variables, array(
        'override_text' => $quote_output,
        'more_class' => $classes,
    ));
    _comment_easy_reply_get_tooltip('comment_referrer_tips', $variables);
    $variables['more_class'] = array('comment-easy-reply-add-quote');
    _comment_easy_reply_get_tooltip('comment_referrer_link', $variables);
    $quote_output = $variables['referrer_link'] . $variables['referrer_tips'];
    $js = comment_easy_reply_add_js(
      array(
        'comment' => $variables['comment'],
        'comment_easy_reply_comment_num' => $variables['comment_easy_reply_comment_num'],
      )
    );
  }
  return $quote_output;
}

/**
 * Returns an HTML list of recent comments to be displayed on comment block.
 *
 * @ingroup themeable
 * @see theme_comment_block()
 */
function theme_comment_easy_reply_comment_block() {
  $items = array();
  $number = variable_get('comment_block_count', 10);
  foreach (comment_get_recent($number) as $comment) {
    $options['tag'] = array();
    $options['tag_link'] = FALSE;
    $matches = _comment_easy_reply_get_matches($comment->subject);
    if (!empty($matches)) {
      foreach ($matches as $num) {
        $parent = NULL;
        $parent_cid = _comment_easy_reply_get_comment_cid_from_num($num, $comment->nid);
        $parent = comment_load($parent_cid);
        if (!empty($parent)) {
          $options['tag'][$num] = _comment_easy_reply_get_tag($parent, TRUE);
        }
      }
    }
    $comment->subject = _comment_easy_reply_comment_body_replace($comment->subject, $comment->nid, FALSE, $options);
    $items[] = l($comment->subject, 'comment/' . $comment->cid, array('fragment' => 'comment-' . $comment->cid)) . '&nbsp;<span>' . t('@time ago', array('@time' => format_interval(REQUEST_TIME - $comment->changed))) . '</span>';
  }
  if ($items) {
    return theme('item_list', array('items' => $items));
  }
  else {
    return t('No comments available.');
  }
}
