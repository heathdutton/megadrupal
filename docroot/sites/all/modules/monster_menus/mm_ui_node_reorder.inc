<?php

/**
 * @file
 * User interface routines for reordering nodes on a page
 */

function mm_ui_node_reorder($form, &$form_state, $mmtid) {
  $form['#attached']['css'][] = drupal_get_path('module', 'monster_menus') . '/css/mm.css';
  $form['#attached']['js'][] = array('data' => mm_static('mm_ui_node_reorder'), 'type' => 'inline', 'scope' => 'footer');

  $where = '';
  $hidden = mm_get_node_info(MM_NODE_INFO_NO_REORDER);
  if ($hidden) {
    $where = " AND n.type NOT IN('" . join("', '", $hidden) . "')";
  }
  $node_sql = mm_content_get_accessible_nodes_by_mmtid_query($mmtid, $count_sql, ', n.type, nr.title, n.region, n.weight', ' LEFT JOIN {node_revision} nr ON nr.vid = n.vid', $where);
  $count = db_query($count_sql)->fetchColumn();

  mm_content_add_breadcrumb(t('Reorder contents'));
  $form['#tree'] = TRUE;
  if ($count) {
    $form['mmtid'] = array(
      '#type' => 'value',
      '#value' => $mmtid,
    );
    $form['actions'] = array('#type' => 'actions');
    if ($count > MM_UI_MAX_REORDER_ITEMS) {
      $form['empty_menu'] = array('#markup' => t('<p>There are too many pieces of content on this @thing to make reordering feasible.</p>', mm_ui_strings(FALSE)));
    }
    else {
      $alt = t('drag handle');
      $clear = drupal_get_path('module', 'monster_menus') . '/images/clear.gif';
      $form['#prefix'] = t('<div id="help"><p>To reorder a piece of content, grab the !image and drag it to a new location. Be sure to <em>Save configuration</em> when done.</p></div>', array('!image' => '<a href="#" class="tabledrag-handle" style="text-decoration: none; cursor: text"><span class="handle">' . theme('image', array('path' => $clear, 'width' => 13, 'height' => 13, 'alt' => $alt)) . '</span></a>'));
      $form['#suffix'] = t('<p></p><div id="help"><p>If a custom content order is being used, any new contents you add will appear at the top of the list. You may want to return here to change their location, afterward.</p><p>Use the <em>Remove custom ordering and save</em> button to revert to the default, reverse-chronological order. Any changes in region are unaffected by this button.</p></div>');

      global $theme;
      $form['edited_theme'] = array(
        '#type' => 'value',
        '#value' => $theme,
      );

      $all_regions = system_region_list($theme, REGIONS_VISIBLE);
      $form['regions'] = array(
        '#type' => 'value',
        '#value' => $all_regions,
      );

      $q = db_query($node_sql);

      foreach ($q as $row) {
        if (empty($row->region) || !isset($all_regions[$row->region])) {
          $row->region = MM_UI_REGION_CONTENT;
        }
        $rows[] = $row;
      }
      _mm_ui_node_reorder_sort();   // Initialize.
      uasort($rows, '_mm_ui_node_reorder_sort');

      $new_weight = 1;
      foreach ($rows as $row) {
        $id = 'nid:' . $row->nid;
        $form['nodes'][$id] = array();
        $form['nodes'][$id]['#attributes'] = array('class' => array('menu-enabled'));
        $title = trim($row->title);
        if (empty($title)) {
          $title = t('Title not provided');
        }
        $form['nodes'][$id]['title']['#markup'] = l($title, 'node/' . $row->nid);
        $form['nodes'][$id]['type']['#markup'] = node_type_get_name($row);
        $form['nodes'][$id]['weight'] = array(
          '#type' => 'weight',
          '#delta' => $count,
          '#default_value' => isset($form_state[$id]['weight']) ? $form_state[$id]['weight'] : $new_weight++,
        );
        $allowed_regions = array_intersect_key($all_regions, array_flip(mm_content_get_allowed_regions_for_user(NULL, $row->type)));
        if (!isset($allowed_regions[$row->region])) {
          // Current region is not allowed, so allow only it (can't leave region)
          $allowed_regions = array($row->region => $all_regions[$row->region]);
        }
        $form['nodes'][$id]['region'] = array(
          '#type' => 'select',
          '#default_value' => $row->region,
          '#title_display' => 'invisible',
          '#title' => t('Region for %title', array('@title' => $title)),
          '#options' => $allowed_regions,
        );
        $form['nodes'][$id]['nid'] = array(
          '#type' => 'hidden',
          '#value' => $row->nid,
        );
        if ($row->stuck) $form['nodes'][$id]['stuck'] = array(
          '#type' => 'value',
          '#value' => TRUE,
        );
      }

      $form['actions']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save configuration'),
      );
      $form['actions']['spacer'] = array(
        '#markup' => '&nbsp;&nbsp;&nbsp;&nbsp;',
      );
    }
    $form['actions']['reorder_normal'] = array(
      '#type' => 'submit',
      '#value' => t('Remove custom ordering and save'),
    );
  }
  else {
    $form['empty_menu'] = array('#markup' => t('There is no content on this @thing to reorder.', mm_ui_strings(FALSE)));
  }
  return $form;
}

function _mm_ui_node_reorder_sort($a = NULL, $b = NULL) {
  static $regions;

  if (empty($a)) {
    // Initialize.
    global $theme;
    $regions = array_keys(system_region_list($theme, REGIONS_VISIBLE));
    return;
  }

  if ($a->stuck != $b->stuck) {
    return $b->stuck - $a->stuck;
  }
  if ($a->region != $b->region) {
    return array_search($b->region, $regions) - array_search($a->region, $regions);
  }
  if ($a->weight != $b->weight) {
    return $a->weight - $b->weight;
  }
  return $b->created - $a->created;
}

function mm_ui_node_reorder_submit($form, &$form_state) {
  $mmtid = $form_state['values']['mmtid'];
  // Remove everything.
  db_delete('mm_node_reorder')
    ->condition('mmtid', $mmtid)
    ->execute();
  if ($form_state['clicked_button']['#id'] == 'edit-actions-reorder-normal') {
    foreach ($form_state['values']['nodes'] as $entry) {
      // make sure these are integers
      if (is_array($entry) && isset($entry['nid']) && intval($entry['nid']) && $entry['region'] != MM_UI_REGION_CONTENT) {
        db_insert('mm_node_reorder')
          ->fields(array(
            'mmtid' => $mmtid,
            'nid' => intval($entry['nid']),
            'weight' => 0,
            'region' => $entry['region'],
          ))
          ->execute();
      }
    }
    drupal_set_message(t('Custom ordering has been removed.'));
  }
  else {
    foreach ($form_state['values']['nodes'] as $entry) {
      // make sure these are integers
      if (is_array($entry) && isset($entry['nid']) && intval($entry['nid'])) {
        db_insert('mm_node_reorder')
          ->fields(array(
            'mmtid' => $mmtid,
            'nid' => intval($entry['nid']),
            'weight' => $entry['weight'],
            'region' => $entry['region'] == MM_UI_REGION_CONTENT ? NULL : $entry['region'],
          ))
          ->execute();
      }
    }
    drupal_set_message(t('The contents have been reordered.'));
  }
  // Make sure the page draws with the new order
  mm_content_clear_caches();
}
