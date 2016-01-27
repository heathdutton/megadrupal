<?php
/**
 * @file
 * General page displays for Lyris configuration and management.
 */
/**
 * Lyris List overview table.
 */
function lyris_page_lists() {
  $server = variable_get('lyris_mode', LYRIS_MODE_DEFAULT);

  $lists = lyris_get_lists($server);
  $field_ui = module_exists('field_ui');
  $members  = module_exists('lyris_member');
  $header = array(
    t('List Name'),
    array('data' => t('Operations'),         'colspan' => ($field_ui ? '5' : '3')),
    array('data' => t('Content & Mailings'), 'colspan' => ($members  ? '3' : '2')),
  );
  $rows = array();

  foreach ($lists as $list) {
    // Check remote user permissions
    if (!lyris_list_access('remote', $list[llf('ListName')])) {
      continue;
    }

    $url = lyris_url_str($list[llf('ListName')]);
    $creator = user_load($list['creator']);
    $changer = user_load($list['changer']);

    $row = array(theme('lyris_list_admin_overview', array('list' => $list)));

    // Ensure the list still exists in Lyris.
    if (lyris_list_name_exists($list[llf('ListName')])) {
      // OPERATIONS
      $row[] = array('data' => l(t('Overview'), 'admin/structure/lyris/lists/' . $list['eid']));
      $row[] = array('data' => l(t('Edit'), 'admin/structure/lyris/lists/' . $list['eid'] . '/edit'));
      if ($field_ui) {
        $row[] = array('data' => l(t('Manage Fields'), 'admin/structure/lyris/lists/' . $list['eid'] . '/fields'));
        $row[] = array('data' => l(t('Manage Display'), 'admin/structure/lyris/lists/' . $list['eid'] . '/display'));
      }
      $row[] = array('data' => l(t('Delete'), 'admin/structure/lyris/lists/' . $list['eid'] . '/delete'));

      // MAILINGS
      $row[] = array('data' => l(t('Set Defaults'), 'admin/structure/lyris/lists/' . $list['eid'] . '/defaults'));
      $row[] = array('data' => l(t('View Content'), 'admin/content/lyris/' . $list['eid']));
      if ($members) {
        $row[] = array('data' => l(t('View Members'), 'admin/structure/lyris/lists/' . $list['eid'] . '/members'));
      }
    }
    else {
      $row[] = array('data' => t('This list could not be found in Lyris.  It may have been renamed or deleted.'), 'colspan' => ($field_ui ? '5' : '3'));
      $row[] = array('data' => l(t('Delete'), 'admin/structure/lyris/lists/' . $list['eid'] . '/delete'), 'colspan' => ($members ? '3' : '2'));
    }

    $rows[] = $row;
  }

  $build['lyris_list_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No mailing lists available.'),
  );
  if (variable_get('lyris_mode_display', LYRIS_MODE_DISPLAY_DEFAULT)) {
    $build['lyris_list_table']['#caption'] = theme('lyris_mode_indicator');
  }

  return $build;
}

/**
 * List overview page.
 */
function lyris_list_view(LyrisList $list) {
  lyris_include('forms');
  drupal_set_title(t('!name Settings', array('!name' => $list->getDisplayName())));

  $lyris = lyris(NULL, $list->server);
  $out = '';

  $remote = $lyris->loadList(array('ListName' => $list->localVal('ListName')));

  // Show field comparison
  $out .= '<h2>' . t('Sync Status') . '</h2>';
  $form = drupal_get_form('lyris_entity_field_comparison_form', $list, $remote);
  $out .= drupal_render($form);

  // Get Sync Status
  $out .= '<h2>' . t('Update Status') . '</h2>';
  $out .= theme('lyris_entity_sync_status', array('entity' => $list));

  if ($list->hasChanges()) {
    $form = drupal_get_form('lyris_entity_quick_push_form', $list);
    $out .= drupal_render($form);
  }

  $out .= '<h2>' . t('Manage List Content') . '</h2>';
  $out .= l(t('Browse List Content'), "admin/content/lyris/{$list->eid}");

  // Show/import content

  // Show/import members

  return $out;
}

/**
 * Page listing available mailing lists to create new mailing list content.
 */
function lyris_content_add_page() {
  $item = menu_get_item();
  $content = system_admin_menu_block($item);

  // Bypass the lyris/add listing if only one list is available.
  if (count($content) == 1) {
    $item = array_shift($content);
    drupal_goto($item['href']);
  }

  return theme('lyris_add_page', array('content' => $content));
}

/**
 * Listing of Lyris content.
 *
 * @param LyrisList $list
 *   An optional lyris list entity to filter the content.
 *
 * @todo Build out better content listing table.
 */
function lyris_content_listing($list = NULL) {
  $output = '';
  $lists = array();
  $server = variable_get('lyris_mode', LYRIS_MODE_DEFAULT);

  lyris_include('forms');

  $sel = db_select(LYRIS_CONTENT_TABLE, 'lc')->fields('lc');

  // Filter by the list if one was set.
  if (is_object($list)) {
    drupal_set_title(t('Mailing List Content for !title', array('!title' => $list->title)));
    $sel->condition('lc.' . llf('ListName'), $list->localVal('ListName'));
  }

  $res = $sel->execute();

  $rows = array();
  while ($content = $res->fetchAssoc()) {
    $list = lyris_list_load($content[llf('ListName')]);
    if (lyris_list_access('remote', $list) && ($server == $list->server)) {
      $row = array(
        $content[llf('Title')] . '<div class="description">' . $content[llf('Description')] . '</div>',
        l($list->getDisplayName(), "admin/structure/lyris/lists/{$list->eid}"),
        l(t('View'), 'lyris/content/' . $content['eid']),
        l(t('Edit'), 'lyris/content/' . $content['eid'] . '/edit'),
        l(t('Preview/Send Mail'), 'lyris/content/' . $content['eid'] . '/mailings'),
      );
      $rows[] = $row;
      $lists[] = $list;
    }
  }

  $headers = array(
    t('Content Title'),
    t('ListName'),
    array('data' => t('Operations'), 'colspan' => 3),
  );

  // Build the table element.
  $table['content'] = array(
    '#theme' => 'table',
    '#header' => $headers,
    '#rows' => $rows,
    '#empty' => t('No content available.'),
  );

  // Build the list filter.
  $form = drupal_get_form('lyris_content_jump_list_form', $lists);
  $output .= drupal_render($form);

  if (variable_get('lyris_mode_display', LYRIS_MODE_DISPLAY_DEFAULT)) {
    $output .= theme('lyris_mode_indicator');
  }
  $output .= drupal_render($table);

  return $output;
}

/**
 * Returns a list content submission form.
 */
function lyris_content_add($list_eid) {
  $output = '';
  $user = lyris_global_user();
  $list = lyris_list_load($list_eid);

  // Prepare initial $lyris_content item.
  $lc = lyris_content($list->eid);

  drupal_set_title(t('Create Content for @name', array('@name' => $list->getDisplayName())), PASS_THROUGH);

  if (variable_get('lyris_mode_display', LYRIS_MODE_DISPLAY_DEFAULT)) {
    $output .= theme('lyris_mode_indicator');
  }

  $form = drupal_get_form($list->localVal('ListName') . '_lyris_content_form', $lc);
  $output .= drupal_render($form);

  return $output;
}

/**
 * Returns a list-content edit form.
 */
function lyris_content_edit($lc) {
  drupal_set_title(t("Edit '@name'", array('@name' => $lc->getDisplayName())), PASS_THROUGH);
  $output = drupal_get_form($lc->localVal('ListName') . '_lyris_content_form', $lc);

  return $output;
}

/**
 * Display Lyris Content.
 *
 * @todo Setup display modes.
 */
function lyris_content_page($lc) {
  $out = theme('lyris_content_embedded', array('content' => $lc));
  return $out;
}
/**
 * Embedded view callback.
 */
function lyris_content_page_view($op = 'embedded', $lc) {
  switch ($op) {
    case 'embedded':
    default:
      lyris_content_render($lc);
      break;
  }
}

/**
 * Display the mailing form with the preview.
 */
function lyris_mailing_page($lc) {
  $out = '';
  lyris_include('forms');
  lyris_include('css');

  $list = lyris_list_load($lc->localVal('ListName'));
  $mailing = lyris_mailing($lc);
  $title = t('Deliver \'@mailing\' to \'@list\'', array('@mailing' => $lc->getDisplayName(), '@list' => $list->getDisplayName()));
  drupal_set_title($title);

  $form = drupal_get_form('lyris_mailing_form', $list, $lc, $mailing);

  $mailing->content_eid = $lc->eid;
  $mailing->list_eid = $list->eid;

  $out .= '<div id="lyris-mailing">';
  $out .= '<div class="col left">' . drupal_render($form) . '</div>';
  $out .= '<div class="col right">' . theme('lyris_mailing_preview', array('mailing' => $mailing)) . '</div>';
  $out .= '</div>';

  return $out;
}

/**
 * Print the plain body content of the mailing.
 */
function lyris_content_render($lc) {
  $content = '';
  $is_html = FALSE;
  foreach (field_get_items('lyris_content', $lc, 'lyris_content_docparts') as $instance) {
    $content .= $instance['safe_value'];
    if (lyris_textfield_mimetype($instance) == 'html') {
      $is_html = TRUE;
    }
  }

  $data = array('lyris_content' => $lc, 'lyris_list' => lyris_list_load($lc->localVal('ListName')));

  print theme('lyris_content', array('content' => token_replace($content, $data), 'is_html' => $is_html));
}
