<?php
/**
 * @file
 * Provides functions and hooks for admin pages configurations.
 */

/**
 * Returns menu items to hook_menu().
 */
function _out_of_context_admin_menu() {
  $items = array();
  
  $items['admin/config/user-interface/out_of_context'] = array(
    'title' => 'Out Of Context Configurations',  
    'page callback' => 'drupal_get_form',
    'page arguments' => array('out_of_context_config_layout_form'),
    'access arguments' => array('administer content'),
    'description' => 'Configure content types layouts.',  
    'file' => 'out_of_context.admin.inc',  
    'type' => MENU_NORMAL_ITEM,
  );
  
  $items['admin/config/user-interface/out_of_context/layout'] = array(
    'title' => 'Layout Configurations',
    'description' => 'Configure content types layouts.', 
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );
  
  $items['admin/config/user-interface/out_of_context/blocks'] = array(
    'title' => 'Blocks Configurations',
    'page arguments' => array('out_of_context_config_blocks_form'),
    'access arguments' => array('administer content'),
    'description' => 'Configure blocks to be participate.', 
    'type' => MENU_LOCAL_TASK,
    'file' => 'out_of_context.admin.inc',
    'weight' => 20,
  );
  
  $items['admin/config/user-interface/out_of_context/contexts'] = array(
    'title' => 'Contexts Configurations',
    'page arguments' => array('out_of_context_config_contexts_form'),
    'access arguments' => array('administer content'),
    'description' => 'Configure contexts to be available.',   
    'type' => MENU_LOCAL_TASK,
    'file' => 'out_of_context.admin.inc',
    'weight' => 20,
  );
  return $items;
}

/**
 * Layout configuration form.
 */
function out_of_context_config_layout_form($form, &$form_state) {
  $form['group'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 99,
  );
  
  // Turn on/off the layout feature.
  $form["out_of_context_page_layout"] = array(
    '#type' => 'checkbox',
    '#title' => t('Page Content Layout'),
    '#default_value' => variable_get("out_of_context_page_layout", 0),
    '#description' => t('check this will set the page content layout (need to select a layout for at least one content type below).'),  
   );
  
  // Find all layouts.
  $layouts = array(
    'none' => 'None',  
    'one_column' => 'One Column', 
    'two_columns' => 'Two Columns'
  );
  
  // Find all node types.
  foreach (node_type_get_names() as $type => $name) {
    $default = variable_get("out_of_context_page_layout_{$type}", NULL);
    $default = $default['layout'];
    $form["out_of_context_page_layout_{$type}"] = array(
      '#type' => 'fieldset',
      '#title' => check_plain($name),
      '#tree' => TRUE,
      '#group' => 'group',  
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form["out_of_context_page_layout_{$type}"]["layout"] = array(
      '#type' => 'radios',
      '#title' => t('@page Content Layout', array('@page' => $name)),
      '#options' => $layouts,
      '#default_value' => $default,
      '#description' => t('Set the page content layout.'),  
     );
  }

  $form = system_settings_form($form);
  $form['#submit'][] = 'out_of_context_config_layout_form_submit';
  return $form;
}

/**
 * En extra submit for layout form, for flush all caches in the finala
 */
function out_of_context_config_layout_form_submit($form, &$form_state) {
  drupal_flush_all_caches();
  drupal_set_message(t('Flushed all caches')); 
}


/**
 * Configuration form for selecting which blocks will be available on the palette.
 */
function out_of_context_config_blocks_form() {
  // Find all visible system regions.
//  $theme_key = variable_get('theme_default','none');
//  $regions = system_region_list($theme_key, $show = REGIONS_VISIBLE);
//  $form['out_of_context_regions'] = array(
//    '#type' => 'select',
//    '#title' => t('Select Regions'),
//    '#options' => $regions,
//    '#multiple' => TRUE, 
//    '#default_value' => variable_get('out_of_context_regions', NULL),
//    '#description' => t('Set the regions which will marked in editing mode.'),
//  );
  
  $plugin = context_get_plugin('reaction', 'block');
  $blocks = $plugin->get_blocks();
  $modules = array();
  foreach ($blocks as $block) {
    $group = isset($block->context_group) ? $block->context_group : $block->module;
    $modules[$group][$block->bid] = $block;
  }
  
  $form = array();
  $form['group'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 99,
  );
  
  foreach ($modules as $name => $module) { 
    $form["out_of_context_blocks_{$name}"] = array(
      '#type' => 'fieldset',
      '#title' => check_plain($name),
      '#tree' => TRUE,
      '#group' => 'group',    
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $default = array();
    foreach ($module as $block => $v) {
      $default = variable_get("out_of_context_block_{$block}", 0);
      $form["out_of_context_blocks_{$name}"][$block] = array(
        '#type' => 'checkbox',
        '#title' => check_plain($block),
        '#default_value' => $default,  
       );
    }
    $blocks = array_combine(array_keys($module), array_keys($module));
  }
  
  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));

  if (!empty($_POST) && form_get_errors()) {
    drupal_set_message(t('The settings have not been saved because of the errors.'), 'error');
  }
  
  return $form;
}

/**
 * Submit blocks form.
 */
function out_of_context_config_blocks_form_submit($form, &$form_state) {
  // Exclude unnecessary elements.
  form_state_values_clean($form_state);

  foreach ($form_state['values'] as $key => $value) {
    if (is_array($value)) {
      foreach ($value as $name => $block) {
        if (!empty($block)) {
          variable_set("out_of_context_block_{$name}", 1);
        }
        else {
          variable_del("out_of_context_block_{$name}");
        }
      }
    }  
  }

  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Configuration form for selecting which context will be able to editing. 
 */
function out_of_context_config_contexts_form() {
  $form = array();
  
  $contexts = context_enabled_contexts();
  $options['on_current_page'] = 'on_current_page';
  foreach ($contexts as $context) {
    if (!empty($context->name) && $context->tag != 'Out Of Context') {
      $options[$context->name] = $context->name;
    }
  }
  $form['out_of_context_contexts'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Select Contexts'),
    '#options' => $options,
    '#multiple' => TRUE, 
    '#default_value' => variable_get('out_of_context_contexts', array()),
    '#description' => t('Set the contexts to be available.'),
  );
  
  return system_settings_form($form);
}