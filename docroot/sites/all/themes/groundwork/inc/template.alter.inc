<?php

/**
 * @file
 * Performs different alterations.
 *
 * Groundwork Framework for Drupal
 * Learn more ~ http://groundwork.noceda.me
 *
 * WARNING: DO NOT MODIFY THIS FILE. Create a subtheme for your customizations.
 *
 */

/**
 * Implements hook_html_head_alter().
 */
function groundwork_html_head_alter(&$head) {
  // Simplify the meta tag for character encoding.
  if (isset($head['system_meta_content_type']['#attributes']['content'])) {
    $head['system_meta_content_type']['#attributes'] = array('charset' => str_replace('text/html; charset=', '', $head['system_meta_content_type']['#attributes']['content']));
  }
}

/**
 * Implements hook_page_alter().
 */
function groundwork_page_alter($page) {
  // Look in each visible region for blocks.
  foreach (system_region_list($GLOBALS['theme'], REGIONS_VISIBLE) as $region => $name) {
    if (!empty($page[$region])) {
      // Find the last block in the region.
      $blocks = array_reverse(element_children($page[$region]));
      while ($blocks && !isset($page[$region][$blocks[0]]['#block'])) {
        array_shift($blocks);
      }
      if ($blocks) {
        $page[$region][$blocks[0]]['#block']->last_in_region = TRUE;
      }
    }
  }
}

/**
 * Sets a class on the iframe body element for WYSIWYG editors. This allows
 * to easily override the background for the iframe body element.
 * This only works for the WYSIWYG module: http://drupal.org/project/wysiwyg
 */
function groundwork_wysiwyg_editor_settings_alter(&$settings, &$context) {
  $settings['bodyClass'] = 'wysiwyg-editor';
}

/**
 * hook_form_BASE_FORM_ID_alter()
 * Modify field classes on node forms.
 */
/**
 * Implements hook_form_BASE_FORM_ID_alter().
 *
 * Prevent user-facing field styling from screwing up node edit forms by
 * renaming the classes on the node edit form's field wrappers.
 */
function groundwork_form_node_form_alter(&$form, &$form_state, $form_id) {
  // Remove if #1245218 is backported to D7 core.
  foreach (array_keys($form) as $item) {
    if (strpos($item, 'field_') === 0) {
      if (!empty($form[$item]['#attributes']['class'])) {
        foreach ($form[$item]['#attributes']['class'] as &$class) {
          if (strpos($class, 'field-type-') === 0 || strpos($class, 'field-name-') === 0) {
            // Make the class different from that used in theme_field().
            $class = 'form-' . $class;
          }
        }
      }
    }
  }
}
