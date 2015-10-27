<?php

/**
 * @file
 * Admin page callbacks for the Block export widget module.
 */

/**
 * Form constructor for the administration form.
 *
 * @param array $form
 *   Form object.
 * @param array $form_state
 *   Form state object.
 *
 * @return array
 *   An array representing the form definition.
 *
 * @ingroup forms
 *
 * @see block_export_widget_admin_configure_form_submit()
 */
function block_export_widget_admin_configure_form(array $form, array &$form_state) {
  $blocks = _block_rehash();

  $form['blocks'] = array();
  $form['#tree'] = TRUE;

  $form['block_modules'] = array(
    '#type' => 'value',
    '#value' => array(),
  );

  foreach ($blocks as $block) {
    $key = $block['module'] . '_' . $block['delta'];

    if (in_array($key, block_export_widget_excluded_blocks())) {
      continue;
    }

    $block_config = block_export_widget_get_config($block['module'], $block['delta']);

    $form['block_modules']['#value'][$block['module']] = ucfirst($block['module']);

    $form['blocks'][$key]['module'] = array(
      '#type' => 'value',
      '#value' => $block['module'],
    );
    $form['blocks'][$key]['delta'] = array(
      '#type' => 'value',
      '#value' => $block['delta'],
    );
    $form['blocks'][$key]['export'] = array(
      '#type' => 'checkbox',
      '#title_display' => 'invisible',
      '#title' => t('Enable export for @block block', array('@block' => $block['info'])),
      '#default_value' => isset($block_config['export']) ? $block_config['export'] : 0,
      '#attributes' => array(
        'title' => t('Enable export for @block block', array('@block' => $block['info'])),
      ),
    );
    $form['blocks'][$key]['info'] = array(
      '#markup' => '<label for="edit-blocks-' . drupal_clean_css_identifier($key) . '-export">' . check_plain($block['info']) . '</label>',
    );
    $form['blocks'][$key]['format'] = array(
      '#type' => 'select',
      '#title_display' => 'invisible',
      '#title' => t('Export format for @block block', array('@block' => $block['info'])),
      '#default_value' => isset($block_config['format']) ? $block_config['format'] : 'format_default',
      '#options' => block_export_widget_formats($block['module'], $block['delta']),
    );
    if (!empty($block_config['export'])) {
      $form['blocks'][$key]['link'] = array(
        '#type' => 'link',
        '#title' => t('export'),
        '#href' => 'block/export/' . $block['module'] . '/' . $block['delta'],
      );
    }
  }

  $form['actions'] = array(
    '#tree' => FALSE,
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Form submission handler for block_export_widget_admin_configure_form().
 *
 * @param array $form
 *   Form object.
 * @param array $form_state
 *   Form state object.
 *
 * @see block_export_widget_admin_configure_form()
 */
function block_export_widget_admin_configure_form_submit(array $form, array &$form_state) {
  variable_set('block_export_widget', $form_state['values']['blocks']);
  drupal_set_message(t('Block export configuration saved.'));
}

/**
 * Processes variables for block-export-widget-admin-configure-form.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $form
 *
 * @see block-export-widget-admin-configure-form.tpl.php
 */
function template_preprocess_block_export_widget_admin_configure_form(&$variables) {
  $variables['block_modules'] = $variables['form']['block_modules']['#value'];
  asort($variables['block_modules']);

  foreach ($variables['block_modules'] as $key => $value) {
    // Initialize an empty array for the module.
    $variables['block_listing'][$key] = array();
  }

  // Add each block in the form to the appropriate place in the block listing.
  foreach (element_children($variables['form']['blocks']) as $i) {
    $block = &$variables['form']['blocks'][$i];

    // Fetch the module for the current block.
    $module = $block['module']['#value'];

    $variables['block_listing'][$module][$i] = new stdClass();
    $variables['block_listing'][$module][$i]->export_checkbox = drupal_render($block['export']);
    $variables['block_listing'][$module][$i]->block_title = drupal_render($block['info']);
    $variables['block_listing'][$module][$i]->format_select = drupal_render($block['format']);
    $variables['block_listing'][$module][$i]->export_link = !empty($block['link']) ? drupal_render($block['link']) : '';
    $variables['block_listing'][$module][$i]->printed = FALSE;
  }

  $variables['form_submit'] = drupal_render_children($variables['form']);
}
