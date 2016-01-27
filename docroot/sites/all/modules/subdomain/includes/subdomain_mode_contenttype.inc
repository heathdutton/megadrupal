<?php

/**
 * @file
 * Subdomain engine content type mode subclass
 */

/**
 * Description of subdomain_term
 *
 * @author setvik
 */
class subdomain_mode_contenttype extends subdomain_engine {
  // Constructor
  function subdomain_mode_contenttype() {
    parent::__construct();
    $this->form_field = 'subdomain_contenttype';
  }

  function type() {
    return 'contenttype';
  }

  function base_path($sid) {
    return 'node';
  }

  function static_form_text() {
    return t('Content type homepage');
  }

  function default_form_field() {
    return 'type';
  }

  function is_entry_form($form_id) {
    return in_array($form_id, array('node_type_form'));
  }

  function get_homepage_path($sid) {
    if ($this->home == 'default') {
      // This should not happen, but let's fail gracefully anyway
      return $this->base_path($sid);
    }
    else {
      $types = variable_get('subdomain_contenttypes', array());

      return 'subdomain/homepage/' . array_search($sid, $types);
    }
  }

  function get_content_subdomain($nid) {
    static $subdomains;
    if (!isset($subdomains[$nid])) {
      $type = db_query("SELECT type FROM {node} WHERE nid = :nid AND type NOT IN (:types)",
        array(':nid' => $nid, ':types' => _subdomain_filtered_content_types()))
      ->fetchField();
      if ($type) {
        $sid = $this->_get_contenttype_sid($type);
        $subdomains[$nid] = subdomain_get_subdomain($sid);
      }
      else {
        $subdomains[$nid] = '';
      }
    }
    return $subdomains[$nid];
  }

  function _get_contenttype_sid($type) {
    $types = variable_get('subdomain_contenttypes', array());
    return isset($types[$type]) ? $types[$type] : NULL;
  }

  function _set_contenttype_sid($type, $sid) {
    $types = variable_get('subdomain_contenttypes', array());
    $types[$type] = $sid;
    variable_set('subdomain_contenttypes', $types);
  }

  function _get_next_contenttype_sid() {
    $types = variable_get('subdomain_contenttypes', array());
    return count($types) ? max($types) + 1 : 1;
  }

  function entry_form_alter(&$form, $form_id) {
    $sid = $this->_get_contenttype_sid($form['#node_type']->type);
    $default = variable_get('subdomain_contenttype_' . $form['#node_type']->type, '');
    if (empty($default) && user_access('create subdomains') || !empty($default) && user_access('edit subdomains')) {
      $form['#validate'][] = '_subdomain_validate_subdomain';

      if ($this->source == 'custom') {
        $editable = user_access('edit subdomains');

        $form['subdomain_settings'] = array(
          '#type' => 'fieldset',
          '#title' => t('Subdomain settings'),
          '#collapsible' => TRUE,
          '#collapsed' => FALSE,
          '#group' => 'additional_settings',
        );
        $form['subdomain_settings']['subdomain_contenttype'] = _subdomain_get_custom_form_field($default, $editable);
      }
      _subdomain_add_js_validation($sid);
    }
  }

  function save($obj) {
    // Get sid for node orig_type
    $sid = _get_contenttype_sid($obj->orig_type);
    if (!$sid) {
      $sid = _get_next_contenttype_sid();
    }

    // Save sid
    $sids[$obj->type] = $sid;
    $this->save_record($sid, variable_get('subdomain_contenttype_' . $obj->type, ''));

    if (!empty($obj->old_type) && $obj->type != $obj->old_type) {
      // We need to remove the old node type sid
      _set_contenttype_sid($obj->old_type, NULL);
    }

    // Update cache
    _set_contenttype_sid($obj->type, $sid);

    return $sid;
  }

  function delete($obj) {
    variable_del('subdomain_contenttype_' . $obj->type);

    // Clean up sid cache
    $sids = variable_get('subdomain_contenttypes', array());
    unset($sids[$obj->type]);
    variable_set('subdomain_contenttypes', $sids);

    return $this->delete_record($this->_get_contenttype_sid($obj->type));
  }

  function url_alter_helper(&$subdomain, &$path, $original_path, $outbound = TRUE) {
    if ($nid = _subdomain_id_from_path('nid', $original_path)) {
      $subdomain = subdomain()->get_content_subdomain($nid);
    }
  }

  /**
   * Bulk generate subdomains from content type name
   */
  function bulk_generate() {
    $types = array_keys(node_type_get_names());
    $subdomains = variable_get('subdomain_contenttypes', array());
    $count = 0;
    foreach ($types as $type) {
      if (!isset($subdomains[$type])) {
        if (!in_array($type, _subdomain_filtered_content_types())) {
          $sid = count($subdomains) ? max($subdomains) + 1 : 1;
          $this->save_record($sid, $type, TRUE);
          $subdomains[$type] = $sid;
          $count += 1;
        }
      }
    }
    drupal_set_message(format_plural($count, '1 new subdomain created.', '@count new subdomains created.'));
    variable_set('subdomain_contenttypes', $subdomains);
  }

  /**
   * Bulk generate subdomains from node type name
   */
  function bulk_update_batch_process(&$context) {
    if (!isset($context['sandbox']['current'])) {
      $context['sandbox']['count'] = 0;
      $context['sandbox']['current'] = 0;
    }

    $names = node_type_get_names();
    foreach ($names as $type => $name) {
      //pathauto_user_update_alias($account, $op, $options);
      if (user_access('create subdomains')) {
        // Get sid for node orig_type
        $sid = _get_contenttype_sid($type);
        if (!$sid) {
          $sid = _get_next_contenttype_sid();
        }

        $this->save_record($sid, $name, TRUE);
      }
    }

    $context['sandbox']['count'] += count($names);
    $context['sandbox']['current'] = count($names)-1;
    $context['message'] = t('Updated subdomain for node type @type.', array('@type' => end($names)));

    if ($context['sandbox']['count'] != $context['sandbox']['total']) {
      $context['finished'] = $context['sandbox']['count'] / $context['sandbox']['total'];
    }
  }
}
