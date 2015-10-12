<?php
/**
 * @file
 * Provides functions and hooks for the layouts operation.
 */

/**
 * Adds the custom regions to be use by the various layouts.
 */
function _add_custom_regions(&$info, $file, $type) {
   // Get the default theme before the theme layer is loaded,
  // and ignore the admin theme.
  $theme_key = variable_get('theme_default', 'none');
  if ($type == 'theme' && $file->name == $theme_key) {
    // Creates new regions in the current theme.    
    $info['regions']['out_of_context_1'] = 'Out Of Context 1';
    $info['regions']['out_of_context_2'] = 'Out Of Context 2';
    $info['regions']['out_of_context_3'] = 'Out Of Context 3';
    $info['regions']['out_of_context_4'] = 'Out Of Context 4';
  }
}

/**
 * Add markup for making a region editable.
 *  @see context_reaction_block::editable_region
 */
function out_of_context_editable_region(&$page) {
  if ($node = menu_get_object()) {
    if ($type = variable_get("out_of_context_page_layout_{$node->type}", NULL)) { 
      if ($layout = $type['layout']) {
        switch ($layout) {
          case 'one_column':
            $regions = array(
              'out_of_context_1' => 'Out Of Context 1',        
            );
            drupal_add_css(drupal_get_path('module', 'out_of_context') . '/theme/styles/out_of_context_one_column.css');
            break;
          
          case 'two_columns':
            $regions = array(
              'out_of_context_1' => 'Out Of Context 1',             
              'out_of_context_2' => 'Out Of Context 2',                  
            );
            drupal_add_css(drupal_get_path('module', 'out_of_context') . '/theme/styles/out_of_context_two_columns.css');
            break;
          
          case 'stack':
            $regions = array(
              'out_of_context_1' => 'Out Of Context 1',                  
              'out_of_context_2' => 'Out Of Context 2',                  
              'out_of_context_3' => 'Out Of Context 3',                  
              'out_of_context_4' => 'Out Of Context 4',                  
            );
            break;
        }
        $theme_key = variable_get('theme_default', 'none');
        $sys_regions = system_region_list($theme_key, $show = REGIONS_VISIBLE);
        $regions = array_intersect_key($regions, $sys_regions); 
        foreach ($regions as $region => $name) {
          // Context orig markup.
          $page[$region]['context']['#markup'] = "<a class='context-block-region' id='context-block-region-{$region}'>{$name}</a>";  
          // New markup. 
          $page[$region]['context']['#markup'] .= "<a class='out-of-context-block-region' id='out-of-context-block-region-{$region}'>{$name}</a>";           
        }

        // Push Our regions to content region, to be display.
        foreach ($page as $k => $v) {
          if (is_array($v) && strpos($k, 'out_of_context_') !== FALSE) {
            $page['content'][$k] = $page[$k];
            $page['content'][$k]['#prefix'] = '<div class="region region-out-of-context region-' . str_replace("_", "-", $k) . '">';
            $page['content'][$k]['#suffix'] = '</div>';
          }
        }
      }
    }
  }
}

function _out_of_context_node_view($node, $view_mode) {
  $type = variable_get("out_of_context_page_layout_{$node->type}", NULL);
  $layout = $type['layout'];
  if (user_access('administer content') && $view_mode == 'full' && (!empty($layout) && $layout != 'none')) {
    // Generate new context for each node.
    $context_name = 'out-of-context-' . $node->nid;
    $context = context_load($context_name);
    if (!$context) {
      $context = ctools_export_new_object('context');
      $context->conditions = array('path' => array('values' => array("node/{$node->nid}")));
      $context->name = $context_name;
      $context->description = "Out Of Context genetrated context for node-{$node->nid}";
      $context->tag = "Out Of Context";
      context_save($context);
    }
    context_set('context', $context_name, $context);
  }
}
