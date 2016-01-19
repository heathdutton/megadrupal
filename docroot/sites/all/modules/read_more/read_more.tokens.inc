<?php

/**
 * @file
 * Defines the tokens which can be used for the Read more module
 */

/**
 * Implements hook_token_info().
 */
function read_more_token_info() {
  $types = $tokens = array();

  $types['read-more'] = array(
    'name' => t("Read More Link"),
    'description' => t('Tokens related to the read-more link.'),
    'needs-data' => 'node',
  );

  $tokens['read-more'] = array(
    'link' => array(
      'name' => t('Link'),
      'description' => t('The %link_text linked to %link_url with title attribute %link_title.  Does not include the wrapper text.',
        array('%link_text' => t('Link text'), '%link_url' => t('Link URL'), '%link_title' => t('Link title'))
      ),
    ),
    'link-text' => array(
      'name' => t('Link text'),
      'description' => t('The text wrapped by the link.'),
    ),
    'link-url' => array(
      'name' => t('Link URL'),
      'description' => t('The target URL.'),
    ),
    'link-full-url' => array(
      'name' => t('Link Absolute URL'),
      'description' => t('The absolute target URL.'),
    ),
    'link-title' => array(
      'name' => t('Link title'),
      'description' => t('The value of the link\'s title attribute.'),
    ),
  );

  // Make it a sub-token of node tokens.
  $tokens['node']['read-more'] = array(
    'name' => t('Read More Link'),
    'description' => t('Read More Link'),
    'type' => 'read-more',
  );

  $token_info = array(
    'types' => $types,
    'tokens' => $tokens,
  );

  return $token_info;
}

/**
 * Implements hook_tokens().
 */
function read_more_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
  $url_options = array();
  $sanitize = !empty($options['sanitize']);
  $view_mode = empty($options['read_more_view_mode']) ? '' : $options['read_more_view_mode'];

  if (($type == 'read-more') && !empty($data['node'])) {
    $node = $data['node'];
    foreach ($tokens as $name => $original) {
      switch ($name) {

        case 'link':
          $replacements[$original] = _read_more_link($node, $view_mode);
          break;

        case 'link-text':
          $replacements[$original] = _read_more_link_text($node);
          break;

        case 'link-title':
          $link_title = _read_more_link_title($node);
          $replacements[$original] = $sanitize ? check_plain($title) : $title;
          break;

        case 'link-full-url':
          $url_options['absolute'] = TRUE;
          // fallthrough

        case 'link-url':
          if (!empty($options['language'])) {
            $url_options['language'] = $options['language'];
          }
          $replacements[$original] = _read_more_link_url($node, $url_options);
          break;
      }
    }
  }

  // Pass thorugh for node subtokens.
  if ($type == 'node' && !empty($data['node'])) {
    if ($read_more_tokens = token_find_with_prefix($tokens, 'read-more')) {
        $replacements += read_more_tokens('read-more', $read_more_tokens, $data, $options);
    }
    foreach ($tokens as $name => $original) {
      if ($name == 'read-more') {
        $replacements[$original] = read_more_link($node, 'inline');
      }
    }
  }

  return $replacements;
}
