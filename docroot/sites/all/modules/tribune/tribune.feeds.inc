<?php

/**
 * Retrieves the latest posts and returns them in RSS format
 */
function tribune_rss($tribune) {
  if (!$tribune) {
    header('HTTP/1.0 403 Access denied');
    exit();
  }

  $count = $tribune->tribune_display_posts;

  $last_id = 0;
  if (isset($_REQUEST['last'])) {
    $last_id = (int)$_REQUEST['last'];
  } else if (isset($_REQUEST['last_id'])) {
    $last_id = (int)$_REQUEST['last_id'];
  }

  $return = tribune_posts($tribune, $count, $last_id, FALSE);

  $xml_tree = array(
    array(
      'key' => 'rss',
      'attributes' => array(
        'version' => '2.0',
      ),
      'value' => array(
        array(
          'key' => 'channel',
          'value' => array(
            array(
              'key' => 'title',
              'value' => check_plain($tribune->title),
            ),
            array(
              'key' => 'link',
              'value' => url('node/' . $tribune->nid, array('absolute' => TRUE)),
            ),
            array(
              'key' => 'description',
              'value' => '',
            ),
            array(
              'key' => 'ttl',
              'value' => '30',
            ),
          ),
        ),
      ),
    ),
  );

  foreach ($return['posts'] as $post_id => $post) {
    if ($post['moderated']) {
      continue;
    }

    $user = user_load($post['uid']);

    if ($user && $user->name) {
      $login = $user->name;
    } else if (!empty($post['username'])) {
      $login = $post['username'];
    } else {
      $login = '';
    }

    $xml_tree[0]['value'][0]['value'][] = array(
      'key' => 'item',
      '#post' => $post,
      'value' => array(
        'author' => $login,
        'title' => truncate_utf8(check_plain($post['text']), 100, TRUE, TRUE, 80),
        'description' => filter_xss($post['text']),
        'pubDate' => tribune_format_time($tribune, $post['timestamp'], 'r'),
        'link' => url('node/' . $tribune->nid, array('absolute' => TRUE, 'fragment' => 'tribune-' . $post_id)),
        'guid' => url('node/' . $tribune->nid, array('absolute' => TRUE, 'fragment' => 'tribune-' . $post_id)),
      ),
    );
  }

  drupal_alter('tribune_posts_rss', $tribune, $xml_tree);

  return _tribune_format_xml_elements($xml_tree);
}

/**
 * Retrieves the latest posts and returns them in XML format
 */
function tribune_xml($tribune, $last_id = NULL) {
  if (!$tribune) {
    header('HTTP/1.0 403 Access denied');
    exit();
  }

  $count = $tribune->tribune_display_posts;

  if ($last_id === NULL) {
    $last_id = 0;

    if (isset($_REQUEST['last'])) {
      $last_id = (int)$_REQUEST['last'];
    } else if (isset($_REQUEST['last_id'])) {
      $last_id = (int)$_REQUEST['last_id'];
    }
  }

  $return = tribune_posts($tribune, $count, $last_id, FALSE);

  $xml_tree = array(
    array(
      'key' => 'board',
      'attributes' => array(
        'site' => url('node/' . $tribune->nid, array('absolute' => TRUE)),
      ),
      'value' => array(),
    ),
  );

  foreach ($return['posts'] as $post_id => $post) {
    if ($post['moderated']) {
      continue;
    }

    $user = user_load($post['uid']);

    if ($user && $user->name) {
      $login = $user->name;
    } else if (!empty($post['username'])) {
      $login = $post['username'];
    } else {
      $login = '';
    }

    $xml_tree[0]['value'][] = array(
      'key' => 'post',
      '#post' => $post,
      'attributes' => array(
        'id' => $post_id,
        'time' => $post['timestamp'],
      ),
      'value' => array(
        'info' => htmlspecialchars($post['info'], ENT_NOQUOTES),
        'login' => $login,
        'message' => filter_xss($post['text']),
      ),
    );
  }

  drupal_alter('tribune_posts_xml', $tribune, $xml_tree);

  return _tribune_format_xml_elements($xml_tree);
}

/**
 * Retrieves the latest posts and returns them in tab-separated format
 */
function tribune_tsv($tribune, $last_id = NULL) {
  if (!$tribune) {
    header('HTTP/1.0 403 Access denied');
    exit();
  }

  $count = $tribune->tribune_display_posts;

  if ($last_id === NULL) {
    $last_id = 0;
    if (isset($_REQUEST['last'])) {
      $last_id = (int)$_REQUEST['last'];
    } else if (isset($_REQUEST['last_id'])) {
      $last_id = (int)$_REQUEST['last_id'];
    }
  }

  $return = tribune_posts($tribune, $count, $last_id, FALSE);

  $document = array(
    array(
      'id',
      'time',
      'info',
      'login',
      'message',
    ),
  );

  foreach ($return['posts'] as $post_id => $post) {
    if ($post['moderated']) {
      continue;
    }

    $user = user_load($post['uid']);

    if ($user && $user->name) {
      $login = $user->name;
    } else if (!empty($post['username'])) {
      $login = $post['username'];
    } else {
      $login = '';
    }

    $line = array(
      $post_id,
      $post['timestamp'],
      $post['info'],
      $login,
      $post['text'],
    );

    $document[] = array_map('_tribune_tsv_sanitize', $line);
  }

  $document = array_reverse($document);

  drupal_alter('tribune_posts_tsv', $tribune, $document);

  $lines = array();
  foreach ($document as $line) {
    $lines[] = implode("\t", $line);
  }

  return implode("\n", $lines);
}

/**
 * Replaces all control characters and tabs with spaces.
 */
function _tribune_tsv_sanitize($string) {
  return preg_replace('/\P{C}\t/u', ' ', $string);
}

/**
 * Returns a tribune's configuration information in XML format
 */
function tribune_config($tribune) {
  if (!$tribune) {
    header('HTTP/1.0 403 Access denied');
    exit();
  }

  $info = tribune_info($tribune);

  if ($tribune->tribune_public) {
    $backend_path = file_create_url(tribune_static_backend($tribune, 'xml'));
  } else {
    $backend_path = url('node/' . $tribune->nid . '/xml', array('absolute' => TRUE));
  }

  $xml_tree = array(
    array(
      'key' => 'site',
      'attributes' => array(
        'name' => $info['machine_name'],
        'title' => $info['title'],
        'baseurl' => $info['url'],
        'version' => '1.0',
      ),
      'value' => array(
        array(
          'key' => 'board',
          'attributes' => array(
            'name' => 'board',
            'title' => 'Tribune',
          ),
          'value' => array(
            array(
              'key' => 'backend',
              'attributes' => array(
                'path' => $backend_path,
                'public' => tribune_access('read tribune', $tribune, user_load(0)) ? 'true' : 'false',
                'tags_encoded' => 'false',
                'refresh' => $info['reload_rate'],
              ),
            ),
            array(
              'key' => 'post',
              'attributes' => array(
                'method' => 'post',
                'path' => url('node/' . $tribune->nid . '/post', array('absolute' => TRUE)),
                'anonymous' => tribune_access('post in tribune', $tribune, user_load(0)) ? 'true' : 'false',
                'max_length' => $info['max_length'],
              ),
              'value' => array(
                array(
                  'key' => 'field',
                  'attributes' => array(
                    'name' => 'message',
                  ),
                  'value' => '$m',
                ),
              ),
            ),
          ),
        ),
      ),
    ),
  );

  return _tribune_format_xml_elements($xml_tree);
}

/**
 * Sends a tribune's posts in JSON format
 */
function tribune_json_posts($tribune, $last_id = NULL, $echo = TRUE) {
  $GLOBALS['conf']['cache'] = FALSE;

  $count = $tribune->tribune_display_posts;

  $posts = array('posts' => array(), 'moderated' => array());

  if ($last_id === NULL) {
    $last_id = 0;
    foreach (array('last', 'last_id') as $parameter) {
      if (isset($_REQUEST[$parameter])) {$last_id = (int)$_REQUEST[$parameter]; break;}
    }
  }

  $return = tribune_posts($tribune, $count, $last_id);
  foreach (array_reverse($return['posts']) as $post) {
    if ($post['moderated']) {
      $posts['moderated'][] = $post['id'];
    } else {
      $posts['posts'][$post['id']] = theme('tribune_post', array('post' => $post, 'node' => $tribune));
    }
  }

  if ($return['error']) {
    $posts['error'] = $return['error'];
  }

  if ($echo) {
    drupal_json_output($posts);
  } else {
    return drupal_json_encode($posts);
  }
}

/**
 * Sends a tribune's historical posts in JSON format
 */
function tribune_json_posts_history($tribune, $until_id = NULL, $count = 30, $echo = TRUE) {
  $GLOBALS['conf']['cache'] = FALSE;

  $count = $tribune->tribune_display_posts;

  $posts = array('posts' => array());

  if ($until_id === NULL) {
    $until_id = $_REQUEST['until'];
  }

  if (!empty($_REQUEST['count'])) {
    $count = $_REQUEST['count'];
  }

  $return = tribune_posts_history($tribune, $count, $until_id);
  foreach (array_reverse($return['posts']) as $post) {
    if (!$post['moderated']) {
      $posts['posts'][$post['id']] = theme('tribune_post', array('post' => $post, 'node' => $tribune));
    }
  }

  if ($return['error']) {
    $posts['error'] = $return['error'];
  }

  if ($echo) {
    drupal_json_output($posts);
  } else {
    return drupal_json_encode($posts);
  }
}

