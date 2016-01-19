<?php
/**
 * @file Views hooks
 */

/**
 * Implements hook_views_pre_view().
 */
function og_entityform_views_pre_view(&$view, &$display_id, &$args) {
  if ($view->base_table == 'entityform' && strpos($display_id, 'autofields_') === 0) {
    if (empty($view->display[$display_id]->entityform_settings['autofields'])) {
      $type = $view->args[2];
      $all_types = entityform_get_types();
      if (is_string($type) && in_array($type, array_keys($all_types))) {
        _entityform_view_add_all_fields($view, $display_id, $type);
      }
    }

  }
  return;
}

/**
 * Implements hook_views_query_alter().
 */
function og_entityform_views_query_alter(&$view, &$query){
  // Restrict the default group form views to forms with OG audience field.
  if (in_array($view->name, array('og_entityform_group_form_admin', 'og_entityform_forms_list', 'og_entityform_group_admin_list'))) {
    $group_forms = og_entityform_get_group_forms();
    $query->add_where(0, 'entityform_type.type', $group_forms, 'IN');
  }
}
