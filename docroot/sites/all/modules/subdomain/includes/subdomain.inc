<?php

/**
 * @file
 * Subdomain engine base class
 */

/**
 * Description of subdomain engine
 */
class subdomain_engine {
  var $mode;
  var $source;
  var $home_default;
  var $home_view;
  var $form_field;

  // Constructor
  function subdomain_engine() {
    $this->mode = variable_get('subdomain_mode', 'user');
    $this->source = variable_get('subdomain_source', 'default');
    $this->home_default = variable_get('subdomain_home_default', TRUE);
    $this->home_view = variable_get('subdomain_home_view', '');

    if ($this->source == 'default') {
      $this->form_field = $this->default_form_field();
    }
    else {
      $this->form_field = 'subdomain';
    }
  }

  function type() {
    return '';
  }

  function base_path($sid) {
    return '';
  }

  function get_content_subdomain($nid) {
    return NULL;
  }

  function url_alter_helper(&$subdomain, &$path, $original_path, $outbound = TRUE) {
  }

  function is_entry_form($form_id) {
    return FALSE;
  }

  function entry_form_alter(&$form, $form_id) {
  }

  function static_form_text() {
    return '';
  }

  function default_form_field() {
    return 'name';
  }

  function get_homepage_path($sid) {
    if ($this->home_default) {
      return subdomain()->base_path($sid);
    }
    else {
      return "subdomain/homepage/$sid";
    }
  }

  function get_sid($subdomain, $force = FALSE) {
    $sids = &drupal_static(__CLASS__ . '::' . __METHOD__ . '__sids', array());

    if (!isset($sids[$subdomain]) || $force) {
      $sids[$subdomain] = db_query("SELECT sid FROM {subdomain} WHERE subdomain = :subdomain", array(':subdomain' => subdomain()->clean($subdomain)))->fetchField();
    }

    return $sids[$subdomain];
  }

  function get_subdomain($sid, $force = FALSE) {
    $subdomains = &drupal_static(__CLASS__ . '::' . __METHOD__ . '__subdomains', array());

    if (!isset($subdomains[$sid]) || $force) {
      $subdomains[$sid] = db_query("SELECT subdomain FROM {subdomain} WHERE sid = :sid", array(':sid' => $sid))->fetchField();
    }

    return $subdomains[$sid];
  }

  function data_handler($op, $requireMode, $object) {
    if ($requireMode == $this->mode) {
      switch ($op) {
        case 'insert':
          if (user_access('create subdomains', ($requireMode == 'user' ? (object) $object : NULL))) {
            $sid = $this->save($object);
            module_invoke_all('subdomain_insert', $sid, $object);
          }
          break;
        case 'update':
          $subdomain = subdomain_get_subdomain($object->uid);
          if (empty($subdomain) && user_access('create subdomains')) {
            $sid = $this->save($object);
            module_invoke_all('subdomain_insert', $sid, $object);
          }
          elseif (user_access('edit subdomains')) {
            $sid = $this->save($object);
            module_invoke_all('subdomain_update', $sid, $object);
          }
          break;
        case 'delete':
          $sid = $this->delete($object);
          module_invoke_all('subdomain_delete', $sid, $object);
          break;
      }
    }
  }

  /**
   * Prepares subdomain for saving
   */
  function clean($raw, $make_unique = FALSE) {
    $clean = $raw;

    // Try to use the transliteration module if available.
    if (function_exists('transliteration_get')) {
      $clean = transliteration_get($clean, '-', language_default('language'));
    }

    // Replace spaces with dashes, and make sure we don't have multiple dashes in a row.
    $clean = str_replace(' ', '-', $clean);
    $clean = preg_replace('/--+/i', '-', $clean);

    /*
    Hostname spec:
    The original specification of hostnames in RFC 952, mandated that labels could not start with a digit or with a hyphen, and must not
    end with a hyphen. However, a subsequent specification (RFC 1123) permitted hostname labels to start with digits.
    */

    // Remove any remaining invalid characters
    $clean = preg_replace("/[^a-z0-9-]/i", "", $clean);

    // Remove any leading dash
    $clean = preg_replace("/^-/i", "", $clean);

    // Remove any trailing dash
    $clean = preg_replace("/-$/i", "", $clean);

    // Are we going to allow the subdomain to start with a numeral (RCC 1123)?
    if (!variable_get('subdomain_allow_rfc1123', FALSE)) {
      // Clean until nothing is replaced.
      $dirty = TRUE;
      while ($dirty) {
        // Remove leading digits
        $clean = preg_replace("/^[0-9]+/i", "", $clean, -1, $dirty);

        // Remove any exposed leading dash
        $clean = preg_replace("/^-/i", "", $clean);
      }
    }

    // Make sure we return a lowercase subdomain
    $clean = strtolower($clean);

    if (!empty($clean) && $make_unique) {
      // If the subdomain already exists, generate a new, hopefully unique, variant
      if (subdomain_get_sid($clean)) {
        $maxlength = 250;
        $original_clean = $clean;

        $i = 0;
        do {
          // Append an incrementing numeric suffix until we find a unique alias.
          $unique_suffix = '-' . $i;
          $clean = truncate_utf8($original_clean, $maxlength - drupal_strlen($unique_suffix, TRUE)) . $unique_suffix;
          $i++;
        } while (subdomain_get_sid($clean));
      }
    }

    return $clean;
  }

  function exists($value) {
    $sid = subdomain_get_sid($value);
    return !empty($sid);
  }

  function save($obj) {
  }

  function delete($obj) {
  }

  function save_record($id, $raw, $make_unique = FALSE) {
    $row->sid = $id;
    $row->subdomain = $this->clean($raw, $make_unique);
    $row->existing_sid = subdomain_get_sid($row->subdomain);
    if ($row->subdomain && (!$row->existing_sid || $row->existing_sid == $row->sid)) {
      db_merge('subdomain')
        ->key(array('sid' => $row->sid))
        ->fields(array(
            'subdomain' => $row->subdomain,
        ))
        ->execute();
    }
    else {
      // Delete subdomain if blank (site admins can enter blank subdomains)
      $this->delete_record($row->sid);
    }

    // Refresh caches
    subdomain_get_sid($row->subdomain, TRUE);
    subdomain_get_subdomain($row->sid, TRUE);
  }

  function delete_record($sid) {
    db_query('DELETE FROM {subdomain} WHERE sid = :sid', array(':sid' => $sid));
  }

  function bulk_update_batch_process(&$context) {
    if (!isset($context['sandbox']['current'])) {
      $context['sandbox']['count'] = 0;
      $context['sandbox']['current'] = 0;
    }

    $context['finished'] = 1;
    return;
  }
}
