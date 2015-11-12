<?php

function _stop_spam_form_validate($values, $entity_type) {
  list($id, $body_field, $subfields) = _stop_spam_entity_type_info($entity_type, $values);

  // Skip this expensive checks if there are other errors.
  $errors = form_get_errors();
  if (!empty($errors)) {
    return;
  }

  // Skip protection for privileged users.
  if (user_access('skip stop spam protection')) {
    return;
  }

  $minutes = 1;
  if (_stop_spam_is_node_bruteforce($minutes) || _stop_spam_is_comment_bruteforce($minutes)) {
    form_set_error($body_field, t('You are posting too quickly! Please wait %minutes minute(s).', array('%minutes' => $minutes)));
    return;
  }

  // Skip external links check if links posting is allowed to current user.
  if (_stop_spam_is_external_links_posting_allowed($entity_type, $id)) {
    return;
  }

  $previous = NULL;
  if (!empty($id) && variable_get('stop_spam_skip_existing_on_edit', TRUE)) {
    // Load original entity to compare links and skip existing on edit.
    $entities = entity_load($entity_type, array($id));
    if (!empty($entities[$id])) {
      $previous = $entities[$id];
    }
  }

  // Render entity body with selected format and extract attributes to check
  // for external links.
  $formats = filter_formats();
  
  foreach ($values[$body_field] as $lang => $items) {
    foreach ($items as $index => $field) {
      // Check access to the input format.
      $format_name = $field['format'];
      if (!filter_access($formats[$format_name])) {
        form_set_error($body_field . '][' . $lang . '][' . $index . '][format', t("You don't have access to the specified format."));
        continue;
      }

      foreach ($subfields as $subfield) {
        if (empty($field[$subfield])) {
          continue;
        }

        // Render content and extract URLs from links and images.
        $content = check_markup($field[$subfield], $format_name);
        $attributes = _stop_spam_extract_attributes($content, array('img' => 'src', 'a' => 'href'));
        $messages = array();

        if (isset($previous->{$body_field}[$lang][$index]['safe_' . $subfield])) {
          _stop_spam_ignore_urls_from_content($attributes, $previous->{$body_field}[$lang][$index]['safe_' . $subfield]);
        }

        // Check for external links.
        foreach ($attributes['a'] as $href) {
          try {
            if (_stop_spam_is_external_url($href)) {
              $messages[] = t("External links are not permitted.");
              break;
            }
          }
          catch (Exception $e) {
            $messages[] = t('Incorrect URL: @url', array('@url' => $href));
            break;
          }
        }

        // Check for external images.
        foreach ($attributes['img'] as $href) {
          try {
            if (_stop_spam_is_external_url($href)) {
              $messages[] = t("External images are not permitted.");
              break;
            }
          }
          catch (Exception $e) {
            $messages[] = t('Incorrect image URL: @url', array('@url' => $href));
            break;
          }
        }

        // Report form validation errors.
        foreach ($messages as $message) {
          form_set_error($body_field . '][' . $lang . '][' . $index . '][' . $subfield, $message);
        }
      }
    }
  }
}

/**
 * Returns the list of entity info values.
 *
 * @param type $entity_type
 * @param type $values
 *
 * @return array
 *   Following values are returned in the list:
 *   - primary key value
 *   - body field name
 *   - list of columns with text to check.
 */
function _stop_spam_entity_type_info($entity_type, $values) {
  $result = array();
  
  switch ($entity_type) {
    case 'node':
      $result[] = !empty($values['nid']) ? $values['nid'] : NULL;
      $result[] = 'body';
      break;

    case 'comment':
      $result[] = !empty($values['cid']) ? $values['cid'] : NULL;
      $result[] = 'comment_body';
      break;

    default:
      $result[] = NULL;
      $result[] = NULL;
  }
  
  $result[] = _stop_spam_body_subfields($entity_type);
  
  return $result;
}

function _stop_spam_is_comment_bruteforce($minutes = 1) {
  //@TODO: check bruteforce of comments.
}

function _stop_spam_is_node_bruteforce($minutes = 1) {
  global $user;

  $created = db_select('node', 'node')
    ->fields('node', array('created'))
    ->orderBy('created', 'DESC')
    ->condition('node.uid', $user->uid)
    ->execute()->fetchField();

  $diff = time() - (int)$created;

  return ($diff < ($minutes * 60)) ? TRUE : FALSE;
}

/**
 * Returns TRUE if posting of external links is allowed for current user.
 *
 * @param string $entity_type
 *   Entity type being added/edited.
 * @param int|null $entity_id
 *   Entity ID being edited. Leave it empty when entity is added.
 *
 * @return bool
 */
function _stop_spam_is_external_links_posting_allowed($entity_type, $entity_id) {
  if (user_is_anonymous()) {
    return FALSE;
  }

  $conditions = variable_get('stop_spam_allow_ext_links', array());
  if (empty($conditions)) {
    return FALSE;
  }

  global $user;
  
  if (!empty($conditions['authmap'])) {
    $aid = db_query_range('SELECT aid FROM {authmap} WHERE uid = :uid', 0, 1, array(':uid' => $user->uid))->fetchField();
    if (!empty($aid)) {
      return TRUE;
    }
  }
  
  if (!empty($conditions['posts'])) {
    // Search for published nodes/comments created by current user.
    $limit = (int) variable_get('stop_spam_min_posts_num', 5);
    $args = array(':uid' => $user->uid);
    
    $comments_sql = "(SELECT c.cid AS id FROM {comment} c WHERE c.status != 0 AND c.uid = :uid";
    
    if (!empty($entity_id) && $entity_type === 'comment') {
      $comments_sql .= ' AND c.cid != :cid';
      $args[':cid'] = $entity_id;
    }
    
    $comments_sql .= " LIMIT $limit)";
    $nodes_sql = "(SELECT n.nid AS id FROM {node} n WHERE n.status != 0 AND n.uid = :uid";
    
    if (!empty($entity_id) && $entity_type === 'node') {
      $nodes_sql .= ' AND n.nid != :nid';
      $args[':nid'] = $entity_id;
    }
    
    $nodes_sql .= " LIMIT $limit)";
    
    $sql = 'SELECT COUNT(*) FROM (' . $comments_sql . ' UNION ALL ' . $nodes_sql . ') AS u';
    
    $posts_count = db_query($sql, $args)->fetchField();
    
    if (!empty($posts_count) && $posts_count >= $limit) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
 * Removes links found in content from the attributes array.
 *
 * @param array $attributes
 * @param string $content
 */
function _stop_spam_ignore_urls_from_content(&$attributes, $content) {
  $ignored = _stop_spam_extract_attributes($content, array('img' => 'src', 'a' => 'href'));
  
  foreach ($ignored as $tag => $ignored_links) {
    if (!empty($attributes[$tag]) && !empty($ignored_links)) {
      $ignored_links = array_combine($ignored_links, array_fill(0, count($ignored_links), TRUE));
  
      foreach ($attributes[$tag] as $index => $link) {
        if (!empty($ignored_links[$link])) {
          unset($attributes[$tag][$index]);
        }
      }
    }
  }
}

function _stop_spam_is_external_url($href) {
  $url = @parse_url($href);
  if (!$url) {
    throw new Exception();
  }

  $schemes = array('http', 'https');
  if (isset($url['scheme']) && $url['scheme'] != '' && !in_array(drupal_strtolower($url['scheme']), $schemes)) {
    throw new Exception();
  }

  if (isset($url['host']) && $url['host'] != '') {
    static $hosts = NULL;
    
    if (!isset($hosts)) {
      $hosts = variable_get('stop_spam_local_hosts', array());
      $current_host = _stop_spam_current_host();
      
      if ($current_host) {
        $hosts[] = $current_host;
      }
    }

    if (empty($hosts)) {
      throw new Exception();
    }

    $url_host = _stop_spam_remove_www($url['host']);
    if (!in_array($url_host, $hosts)) {
      return TRUE;
    }
  }

  return FALSE;
}
