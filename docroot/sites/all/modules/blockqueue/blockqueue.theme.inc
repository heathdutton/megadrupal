<?php

/**
 * @file
 * Block Queue Theme File.
 */

/**
 * Implements hook_theme().
 */
function blockqueue_theme() {
  return array(
    'blockqueue_export_ui_form_table' => array(
      'render element' => 'form',
    ),
    'blockqueue'                      => array(
      'render element' => 'elements',
      'template'       => 'blockqueue',
    ),
    'blockqueue_block'                => array(
      'render element' => 'elements',
      'template'       => 'blockqueue-block',
    ),
  );
}

/**
 * Blockqueue form callback.
 */
function theme_blockqueue_export_ui_form_table(&$variables) {
  $form = $variables['form'];
  $rows = array();

  foreach (element_children($form) as $id) {
    $form[$id]['weight']['#attributes']['class'] = array('item-weight');

    $rows[] = array(
      'data'  => array(
        drupal_render($form[$id]['name']),
        drupal_render($form[$id]['module']),
        drupal_render($form[$id]['delta']),
        drupal_render($form[$id]['remove']),
        drupal_render($form[$id]['weight']),
      ),
      'class' => array('draggable'),
    );
  }

  $table_id = 'items-table';
  $headers  = array(
    t('Block name'),
    t('Module'),
    t('Delta'),
    t('Operation'),
    t('Weight'),
  );

  drupal_add_tabledrag($table_id, 'order', 'sibling', 'item-weight');

  return theme(
    'table',
    array(
      'header'     => $headers,
      'rows'       => $rows,
      'attributes' => array('id' => $table_id),
      'empty'      => t('Empty block list. Use the form below to add a new block to the list.'),
    )
  );
}

/**
 * Render the blockqueue block itself.
 *
 * @param array $variables
 *   Variables.
 *
 * @return string
 *   Content of the blockqueue.
 *
 * @throws \Exception
 */
function theme_blockqueue($variables) {
  $content = '';

  foreach ($variables['blocks'] as $token => $block) {
    $content .= theme(
      'blockqueue_block',
      array(
        'elements' => array(
          '#block'    => (object) $block,
          '#children' => drupal_render($block->content),
        ),
      )
    );
  }

  return $content;
}

/**
 * Implements hook_preprocess().
 */
function blockqueue_preprocess(&$variables, $hook) {
  if ($hook == 'block' && $variables['elements']['#block']->module == 'blockqueue') {
    // Removes first class: "block".
    unset($variables['classes_array'][0]);
    $variables['classes_array'] = array_values($variables['classes_array']);
  }
}

/**
 * Implements template_preprocess_HOOK().
 */
function template_preprocess_blockqueue(&$variables) {
  $variables['blockqueue'] = $variables['elements']['#blockqueue'];

  // Create the $content variable that templates expect.
  $blocks = _blockqueue_load_blocks($variables['blockqueue']);
  $blocks = _block_render_blocks($blocks);
  $blocks = _block_get_renderable_array($blocks);

  // Replace the last theme wrapper by blockqueue_block instead of block.
  foreach ($blocks as $key => $block) {
    if (isset($block['#theme_wrappers']) && ($wrappers = $block['#theme_wrappers'])) {
      if ($wrappers[count($wrappers) - 1] == 'block') {
        $wrappers[count($wrappers) - 1] = 'blockqueue_block';

        $blocks[$key]['#theme_wrappers'] = $wrappers;
        $blocks[$key]['#blockqueue']     = $variables['blockqueue'];
      }
    }
  }

  $variables['blocks'] = $blocks;

  $variables['classes_array'] = array(
    drupal_html_class('blockqueue'),
    drupal_html_class('blockqueue-' . $variables['blockqueue']->name),
  );

  $variables['theme_hook_suggestions'][] = 'blockqueue__' . $variables['blockqueue']->name;

  // Create a valid HTML ID and make sure it is unique.
  $variables['block_html_id'] = drupal_html_id('blockqueue');
}

/**
 * Implements template_preprocess_HOOK().
 */
function template_preprocess_blockqueue_block(&$variables) {
  // Reset to default block class.
  $variables['classes_array'] = array(drupal_html_class('block'));

  template_preprocess_block($variables);

  $blockqueue = $variables['elements']['#blockqueue'];

  array_shift($variables['theme_hook_suggestions']);

  $variables['theme_hook_suggestions'][] = 'blockqueue_block__blockqueue__' . $blockqueue->name;
  $variables['theme_hook_suggestions'][] = 'blockqueue_block__' . $variables['block']->module;
  $variables['theme_hook_suggestions'][] = 'blockqueue_block__' . $variables['block']->module . '__' . strtr(
      $variables['block']->delta,
      '-',
      '_'
    );
}
