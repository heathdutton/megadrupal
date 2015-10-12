<?php
/**
 * @file
 * Provides functions and hooks for the palette operation.
 */

/**
 * Returns themes to hook_theme().
 */
function _out_of_context_palette_theme() {
  $themes = array();
  
  $themes['out_of_context_block_browser'] = array(
    'variables' => array('blocks' => array(), 'context' => array()),
    'path' => drupal_get_path('module', 'out_of_context') . '/theme',
    'template' => 'out-of-context-block-browser',
    'file' => 'out_of_context_reaction_block.theme.inc',
  );

  $themes['out_of_context_block_browser_item'] = array(
    'variables' => array('block' => array()),
    'path' => drupal_get_path('module', 'out_of_context') . '/theme',
    'template' => 'out-of-context-block-browser-item',
    'file' => 'out_of_context_reaction_block.theme.inc',
  );
  
  return $themes;
}


function add_pallete_to_dom() {
  // Add the Palette to the dom.
  drupal_add_js(drupal_get_path('module', 'out_of_context') . '/theme/scripts/out_of_context.js');
  drupal_add_library('system', 'ui.dialog');
  $block = module_invoke('out_of_context', 'block_view', 'out_of_context_palette');

  $settings = array(
    'palette' => render($block['content']),
    'editbar' => '<div class="edit-out-of-context">Edit Layout</div>'  
  );
  drupal_add_js($settings, array('type' => 'setting'));  
}

/**
 * Clones and overrides form built by
 * context_ui_editor().
 */
function out_of_context_editor($form, &$form_state, $all_contexts) {
  $form = array();  

  // Build the list of selected contexts.
  $selected = variable_get('out_of_context_contexts', array());
  if (!empty($selected)) { 
    foreach ($all_contexts as $k => $context) {
      if ((!empty($selected['on_current_page']) && $context->tag == 'Out Of Context') || !empty($selected[$k])) {
        $contexts[$k] = $context;
      }
    }
  }
  
  // If there are no available context return a message.
  if (empty($contexts)) {
    $form['empty'] = array(
      '#type' => 'markup',
      '#markup' => t("There are NO available contexts to edit")
    );
    return $form;
  }
  if (isset($contexts) && !empty($contexts)) {
    // Clone the context_ui_editor form and make some changes.
    $form = context_ui_editor($form, $form_state, $contexts);
    
    foreach ($contexts as $context) {
      $edit = l(t('Edit'), $_GET['q'], array('fragment' => $context->name, 'attributes' => array('class' => array('edit'))));
      $done = l(t('Done'), $_GET['q'], array('fragment' => $context->name, 'attributes' => array('class' => array('done'))));
      $items[] = array(
        'data' => "<div class='label'>" . (empty($context->description) ? $context->name : check_plain($context->description)) . "</div><div class='links'>{$edit} {$done}</div>",
        'class' => array('context-editable clearfix'),
        'id' => "context-editable-trigger-{$context->name}",
      );

      // Remove all conditions.  
      unset($form['contexts'][$context->name]['condition']);

      // Hide all reactions other then blocks.
      foreach (array_keys(context_reactions()) as $reaction) {
        if ($reaction !== 'block' && isset($form['contexts'][$context->name]["reaction-{$reaction}"])) {
          $form['contexts'][$context->name]["reaction-{$reaction}"]['#access'] = FALSE;
        }
      }
      // Override the editor browser markup with out_of_context_block_browser.tpl.php
      $plugin = context_get_plugin('reaction', 'block');
      $list = $plugin->get_blocks();
      $form['contexts'][$context->name]['reaction-block']['browser'] = array(
        '#markup' => theme('out_of_context_block_browser', array(
          'blocks' => $list,
          'context' => $context
        )),
      );  
    }
    // Override Display editable contexts in list.
    $form['editables']['#markup'] = theme('item_list', array('items' => $items));
  }

  return $form;
}

/**
 * Palette block callback.
 */
function out_of_context_palette_block() {
  if (user_access('administer site configuration') && strpos($_GET['q'], 'admin') === FALSE && $contexts = context_active_contexts()) {
    return drupal_get_form('out_of_context_editor', $contexts);
  }
}

/**
 * Returns blocks info to hook_block_info()
 */
function _out_of_context_palette_block_info() {
  $blocks = array();
  $blocks['out_of_context_palette'] = array(
    'info' => 'Palette block',
  );
  return $blocks;
}

/**
 * Returns block view to hook_block_view()
 */
function _out_of_context_palette_block_view($delta) {
  $block = array();
  switch ($delta) {
    case 'out_of_context_palette':
      $block['subject'] = 'Palette block';
      $block['content'] = out_of_context_palette_block();
      break; 
  }
  return $block;
}