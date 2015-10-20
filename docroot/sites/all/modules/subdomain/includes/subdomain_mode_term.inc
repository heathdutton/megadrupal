<?php

/**
 * @file
 * Subdomain engine term mode subclass
 */


/**
 * Description of subdomain_term
 *
 * @author setvik
 */
class subdomain_mode_term extends subdomain_engine {

  function type() {
    return 'term';
  }

  function base_path($sid) {
    return 'taxonomy/term/' . $sid;
  }

  function static_form_text() {
    return t('Term homepage');
  }

  function is_entry_form($form_id) {
    return $form_id == 'taxonomy_form_term';
  }

  function get_content_subdomain($nid) {
    static $subdomains;

    if (!isset($subdomains[$nid])) {
      // Get first group_nid
      $vid = variable_get('subdomain_vocab', '');
      $sid = db_query("SELECT tn.tid
        FROM {taxonomy_index} tn
        INNER JOIN {node} n on tn.nid = n.nid
        INNER JOIN {taxonomy_term_data} td on tn.tid = td.tid
        WHERE td.vid = :vid AND n.nid = :nid AND n.type NOT IN (:types)",
        array(':vid' => $vid, ':nid' => $nid, ':types' => _subdomain_filtered_content_types()))
      ->fetchField();
      $subdomains[$nid] = subdomain_get_subdomain($sid);
    }
    return $subdomains[$nid];
  }

  function entry_form_alter(&$form, $form_id) {
    global $user;
    if ( $form['#vocabulary']->vid == variable_get('subdomain_vocab', 0) &&
         !(isset($form['delete']) && $form['delete']['#type'] == 'value' && $form['delete']['#value'] == TRUE) ) {
      $default = subdomain_get_subdomain($form['tid']['#value']);
      if (empty($default) && user_access('create subdomains') || !empty($default) && user_access('edit subdomains')) {
        $form['subdomain_exists'] = array(
          '#type' => 'value',
          '#value' => !empty($default),
        );

        $form['#validate'][] = '_subdomain_validate_subdomain';

        if ($this->source == 'custom') {
          $form['identification']['name']['#weight'] = -5.1;

          $editable = user_access('edit subdomains');
          $form['identification']['subdomain'] = _subdomain_get_custom_form_field($default, $editable, $user->uid == 1);
        }
        _subdomain_add_js_validation($form['tid']['#value']);
      }
    }
  }

  function save($obj) {
    if ($obj->vid == variable_get('subdomain_vocab', 0)) {
      return $this->save_record($obj->tid, $obj->{$this->form_field});
    }
  }

  function delete($obj) {
    if ($obj->vid == variable_get('subdomain_vocab', 0)) {
      return $this->delete_record($obj->tid);
    }
  }

  function url_alter_helper(&$subdomain, &$path, $original_path, $outbound = TRUE) {
    if ($tid = _subdomain_id_from_path('tid', $original_path)) {
      $subdomain = subdomain_get_subdomain($tid);
      if (!empty($subdomain) && $path == $this->base_path($tid) && $outbound) {
        $path = '';
      }
    }
    elseif ($nid = _subdomain_id_from_path('nid', $original_path)) {
      $subdomain = subdomain()->get_content_subdomain($nid);
    }
  }

  /**
   * Bulk generate subdomains from term name
   */
  function bulk_update_batch_process(&$context) {
    if (!isset($context['sandbox']['current'])) {
      $context['sandbox']['count'] = 0;
      $context['sandbox']['current'] = 0;
    }

    $vid = variable_get('subdomain_vocab', 0);
    if (!$vid) {
      $context['finished'] = 1;
      return;
    }

    $query = db_select('taxonomy_term_data', 'td');
    $query->leftJoin('subdomain', 's', "s.sid = td.tid");
    $query->addField('td', 'tid');
    $query->isNull('s.sid');
    $query->condition('td.vid', $vid);
    $query->condition('td.tid', $context['sandbox']['current'], '>');
    $query->orderBy('td.tid');
    $query->addTag('subdomain_bulk_update');
    $query->addMetaData('entity', 'taxonomy_term');

    // Get the total amount of items to process.
    if (!isset($context['sandbox']['total'])) {
      $context['sandbox']['total'] = $query->countQuery()->execute()->fetchField();

      // If there are no users to update, the stop immediately.
      if (!$context['sandbox']['total']) {
        $context['finished'] = 1;
        return;
      }
    }

    $query->range(0, 25);
    $tids = $query->execute()->fetchCol();

    //pathauto_user_update_alias_multiple($uids, 'bulkupdate', array('alias blog' => FALSE));
    $terms = taxonomy_term_load_multiple($tids);
    foreach ($terms as $term) {
      //pathauto_user_update_alias($account, $op, $options);
      if (user_access('create subdomains')) {
        $this->save_record($term->tid, $term->name, TRUE);
      }
    }

    $context['sandbox']['count'] += count($tids);
    $context['sandbox']['current'] = max($tids);
    $context['message'] = t('Updated subdomain for term @tid.', array('@tid' => end($tids)));

    if ($context['sandbox']['count'] != $context['sandbox']['total']) {
      $context['finished'] = $context['sandbox']['count'] / $context['sandbox']['total'];
    }
  }
}
