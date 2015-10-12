<?php
/**
 * @file
 *   Token module support for the Weblinks module.
 *   Drupal 7 Version.
 */

/**
 * Implements hook_token_info().
 */
function weblinks_token_info() {
  $info = array();

  $info['types']['weblinks'] = array(
    'name' => t('Web Links'),
    'description' => t('Tokens related to Web Links.'),
    'needs-data' => 'node',
  );

  $info['tokens']['weblinks'] = array(
    'url' => array(
      'name' => t('Web Link URL'),
      'description' => t('The URL of the Web Link.'),
    ),
    'url-raw' => array(
      'name' => t('Web Link URL unfiltered'),
      'description' => t('Unfiltered URL of the Web Link.  WARNING - raw user input.'),
    ),
    'title' => array(
      'name' => t('Web Link Title'),
      'description' => t('The title of the Web Link.'),
    ),
    'title-raw' => array(
      'name' => t('Web Link Title unfiltered'),
      'description' => t('Unfiltered title of the Web Link.  WARNING - raw user input.'),
    ),
    'click-count' => array(
      'name' => t('Web Link Click Count'),
      'description' => t('How many times the Web Link has been clicked.'),
    ),
    'last-click' => array(
      'name' => t('Web Link Last Click'),
      'description' => t('When the Web Link was last clicked on.'),
    ),
    'last-status' => array(
      'name' => t('Web Link Last Status'),
      'description' => t('The status of the Web Link when it was last checked.'),
    ),
    'last-checked' => array(
      'name' => t('Web Link Last Checked'),
      'description' => t('The date/time when the Web Link was last checked.'),
    ),
  );

  return $info;
}

/**
 * Implements hook_tokens().
 */
function weblinks_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
  $sanitize = !empty($options['sanitize']);
  $language_code = isset($options['language']) ? $options['language']->language : NULL;

  if ($type == 'weblinks' && !empty($data['node'])) {
    $object = $data['node'];
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'url':
          $replacements[$original] = $sanitize ? decode_entities(check_plain($object->url)) : $object->url;
          break;
        case 'url-raw':
          $replacements[$original] = $object->url;
          break;
        case 'title':
          // Panels deletes the title (http://drupal.org/node/213751).
          if (!isset($object->title)) {
            $object->title = db_query("SELECT n.title FROM {node} n WHERE n.nid=:nid", array(':nid' => $object->nid));
          }
          $replacements[$original] = $sanitize ? decode_entities(check_plain($object->title)) : $object->title;
          break;
        case 'title-raw':
          $replacements[$original] = $object->title;
          break;
        case 'click-count':
          $replacements[$original] = $object->click_count;
          break;
        case 'last-click':
          if (isset($object->last_click)) {
            $replacements[$original] = $object->last_click;
            $replacements += _weblinks_token_date($object->last_click, $type . ':clicked');
          }
          else {
            $replacements[$original] = '';
          }
          break;
        case 'last-status':
          $replacements[$original] = $object->last_status;
          break;
        case 'last-checked':
          if (isset($object->last_checked)) {
            $replacements[$original] = $object->last_checked;
            $replacements += _weblinks_token_date($object->last_checked, $type . ':checked');
          }
          else {
            $replacements[$original] = '';
          }
          break;
      }
    }
  }

  return $replacements;
}

/**
 * Helper function to return array of date parts.
 */
function _weblinks_token_date($date, $prefix = NULL) {
  if ($prefix) {
    $prefix .= '-';
  }
  $date = strtotime($date);
  $values = array(
    $prefix . 'small'  => format_date($date, 'small'),
    $prefix . 'medium' => format_date($date, 'medium'),
    $prefix . 'large'  => format_date($date, 'large'),
    $prefix . 'since'  => $date ? format_interval(time() - $date) : t('Never'),
    );
  $date = (int)$date;
  $values += array(
    $prefix . 'yyyy'  => date('Y', $date),
    $prefix . 'yy'    => date('y', $date),
    $prefix . 'month' => date('F', $date),
    $prefix . 'mon'   => date('M', $date),
    $prefix . 'mm'    => date('m', $date),
    $prefix . 'm'     => date('n', $date),
    $prefix . 'ww'    => date('W', $date),
    $prefix . 'date'  => date('N', $date),
    $prefix . 'day'   => date('l', $date),
    $prefix . 'ddd'   => date('D', $date),
    $prefix . 'dd'    => date('d', $date),
    $prefix . 'd'     => date('j', $date),
    $prefix . 'hh'    => date('h', $date),
    $prefix . 'mm'    => date('i', $date),
    $prefix . 'ss'    => date('d', $date),
    );

  return $values;
}