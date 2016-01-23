<?php

/**
 * @file
 * Subdomain engine OG mode subclass
 */

/**
 * Description of subdomain_group
 *
 * @author setvik
 */
class subdomain_mode_group extends subdomain_engine {

  function type() {
    return 'node';
  }

  function base_path($sid) {
    return 'node/' . $sid;
  }

  function static_form_text() {
    return t('Group homepage');
  }

  function default_form_field() {
    return 'title';
  }

  function get_content_subdomain($nid) {
    static $subdomains;
    if (!isset($subdomains[$nid])) {
      $sid = db_query("
        SELECT gid FROM og_membership
        WHERE field_name = 'og_group_ref' AND etid = :nid LIMIT 1",
        array(':nid' => $nid))
        ->fetchField();
      $subdomains[$nid] = subdomain_get_subdomain($sid);
    }
    return $subdomains[$nid];
  }

  function is_entry_form($form_id) {
    // TODO update to select correct og group node type if multiple
    if (substr($form_id, -10) == '_node_form') {
      return og_is_group_type('node', substr($form_id, 0, -10));
    }
    return FALSE;
  }

  function entry_form_alter(&$form, $form_id) {
    $nid = isset($form['#node']->nid) ? $form['#node']->nid : NULL;
    $default = subdomain_get_subdomain($nid);
    $form['subdomain_exists'] = array(
      '#type' => 'value',
      '#value' => !empty($default),
    );

    $form['#validate'][] = '_subdomain_validate_subdomain';

    if ($this->source == 'custom') {
      $editable = user_access('edit subdomains');
      $form['subdomain'] = _subdomain_get_custom_form_field($default, $editable);
    }

    _subdomain_add_js_validation($nid);
  }

  function save($obj) {
    if (og_is_group_type('node', $obj->type)) {
      // URL cache clear needed b/c url is cached before subdomain is saved resulting in skewed redirect: http://subdomain.example.com/node/xxxx instead of http://subdomain.example.com/
      subdomain_url_outbound_cache_clear();

      return $this->save_record($obj->nid, $obj->{$this->form_field});
    }
  }

  function delete($obj) {
    if (og_is_group_type('node', $obj->type)) {
      return $this->delete_record($obj->nid);
    }
  }

  function url_alter_helper(&$subdomain, &$path, $original_path, $outbound = TRUE) {
    if ($nid = _subdomain_id_from_path('nid', $original_path)) {
      $subdomain = subdomain_get_subdomain($nid);
      if (empty($subdomain)) {
        $subdomain = subdomain()->get_content_subdomain($nid);
      }
      elseif ($path == $this->base_path($nid) && $outbound) {
        $path = '';
      }
    }
  }

  /**
   * Bulk generate subdomains from group name
   */
  function bulk_update_batch_process(&$context) {
    if (!isset($context['sandbox']['current'])) {
      $context['sandbox']['count'] = 0;
      $context['sandbox']['current'] = 0;
    }

    $query = db_select('node', 'n');
    $query->leftJoin('subdomain', 's', "s.sid = n.nid");
    $query->addField('n', 'nid');
    $query->isNull('s.sid');
    $query->innerJoin('og', 'g', "g.etid = n.nid AND g.entity_type = :type", array(':type' => 'node'));
    $query->condition('n.nid', $context['sandbox']['current'], '>');
    $query->condition('n.type', _subdomain_filtered_content_types(), 'NOT IN');
    $query->orderBy('n.nid');
    $query->addTag('subdomain_bulk_update');
    $query->addMetaData('entity', 'node');

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
    $nids = $query->execute()->fetchCol();

    //pathauto_user_update_alias_multiple($uids, 'bulkupdate', array('alias blog' => FALSE));
    $nodes = node_load_multiple($nids);
    foreach ($nodes as $node) {
      //pathauto_user_update_alias($account, $op, $options);
      if (user_access('create subdomains')) {
        $this->save_record($node->nid, $node->title, TRUE);
      }
    }

    $context['sandbox']['count'] += count($nids);
    $context['sandbox']['current'] = max($nids);
    $context['message'] = t('Updated subdomain for group @nid.', array('@nid' => end($nids)));

    if ($context['sandbox']['count'] != $context['sandbox']['total']) {
      $context['finished'] = $context['sandbox']['count'] / $context['sandbox']['total'];
    }
  }
}
