<?php
/**
 * @file
 * Code for og_content.pages.inc.
 */

/**
 * Menu callback: Administrative page for groups.
 *
 * @param $group_type
 * @param $gid
 * @return string
 */
function og_content_content_page($group_type, $gid) {
  og_set_breadcrumb($group_type, $gid, array(l(t('Group'), "$group_type/$gid/group")));

  return views_embed_view('og_content_contents', 'default', $gid);
}

/**
 * Menu callback: Display a list of content types that can be added to the
 * current group.
 *
 * @param $group_type
 * @param $gid
 * @return string
 */
function og_content_content_add_page($group_type, $gid) {
  $group_content_page = "group/$group_type/$gid/admin/content";
  $group = node_load($gid);

  og_set_breadcrumb($group_type, $gid, array(
    l(t('Group'), "$group_type/$gid/group"),
    l(t('Content'), $group_content_page)
  ));

  // Simulate node add page.
  $item = array(
    'path' => 'node/add',
    'tab_root' => 'node/add'
  );
  $content = system_admin_menu_block($item);
  $types = og_content_get_available_node_types();

  // Get all available types for this group.
  foreach ($types as $type_name) {
    if (!og_is_group_content_type('node', $type_name) || !og_user_access($group_type, $gid, "create $type_name content")) {
      unset($types[$type_name]);
    }
  }

  foreach ($content as $i => $content_type) {
    $arguments = unserialize($content_type['page_arguments']);
    $type_name = reset($arguments);
    $audience_fields = og_get_group_audience_fields($group_type, $type_name);
    $audience_field = key($audience_fields);
    $info = field_info_field($audience_field);

    if (!empty($info['settings']['handler_settings']['target_bundles'])) {
      $target_bundles = array_keys($info['settings']['handler_settings']['target_bundles']);
    }

    // Remove content types not allowed for this group.
    if (!in_array($type_name, $types) || (!empty($target_bundles) && !in_array($group->type, $target_bundles))) {
      unset($content[$i]);
    }
    else {
      $instance = field_info_instance('node', $audience_field, $type_name);
      if (!empty($instance['settings']['behaviors']['prepopulate']['status'])) {
        // Instance allows prepopulating.
        $content[$i]['localized_options']['query'][OG_AUDIENCE_FIELD] = $gid;
      }

      $content[$i]['localized_options']['query']['destination'] = $group_content_page;
    }
  }

  return theme('node_add_list', array('content' => $content));
}
