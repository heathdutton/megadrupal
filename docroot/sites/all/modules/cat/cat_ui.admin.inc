<?php

/**
 * @file Admin zone and Settings of CAT module.
 *
 * @author Oleksiy Kalinichenko <aexchecker@gmail.com>
 */

/**
 * Page callback for module settings page.
 */
function cat_ui_cat_list($form, &$form_state) {
  $cat_items = cat_load_all();

  $js = !empty($form_state['input']['js']);

  $form_state['storage']['objects'] = $cat_items;

  $form['#tree'] = TRUE;

  $form['top row'] = array(
    '#prefix' => '<div class="ctools-export-ui-row ctools-export-ui-top-row clearfix">',
    '#suffix' => '</div>',
  );

  $form['bottom row'] = array(
    '#prefix' => '<div class="ctools-export-ui-row ctools-export-ui-bottom-row clearfix">',
    '#suffix' => '</div>',
  );

  $category_options = array(
    'all' => t('- All -'),
  );

  $form['top row']['category'] = array(
    '#type' => 'select',
    '#title' => t('Category'),
    '#options' => $category_options,
    '#default_value' => 'all',
    '#attributes' => array(
      'class' => array(
        'ctools-auto-submit',
      ),
    ),
  );

  if ($js && !empty($form_state['input']['top_row']['category'])) {
    $form['#js_category'] = $form_state['input']['top_row']['category'];
  }
  else {
    $form['#js_category'] = FALSE;
  }

  foreach ($cat_items as $cat_item) {
    if (!empty($cat_item->category)) {
      $category_options[$cat_item->category] = $cat_item->category;
    }

    if ($form['#js_category'] && $form['#js_category'] !== 'all') {
      if (empty($cat_item->category) || $cat_item->category != $form['#js_category']) {
        continue;
      }
    }

    $form[$cat_item->machine_name]['title']['#markup'] = $cat_item->name;
    if ($cat_item->is_overwritten) {
      $form[$cat_item->machine_name]['title']['#markup'] .= ' <sup>(' . $cat_item->type . ')</sup>';
    }
    $form[$cat_item->machine_name]['name']['#markup'] = $cat_item->machine_name;
    $form[$cat_item->machine_name]['path']['#markup'] = $cat_item->path_original;
    $form[$cat_item->machine_name]['category']['#markup'] = $cat_item->category;
    $form[$cat_item->machine_name]['#parents'] = array('paths', $cat_item->machine_name);

    // Create operations
    $operations = array();
    if ($cat_item->disabled == TRUE) {
      $operations[] = array(
        'title' => t('Enable'),
        'href' => "admin/structure/cat/enable/{$cat_item->machine_name}",
      );
    }
    $operations[] = array(
      'title' => t('Edit'),
      'href' => "admin/structure/cat/edit/{$cat_item->machine_name}",
    );
    $operations[] = array(
      'title' => t('Clone'),
      'href' => "admin/structure/cat/clone/{$cat_item->machine_name}",
    );
    $operations[] = array(
      'title' => t('Export'),
      'href' => "admin/structure/cat/export/{$cat_item->machine_name}",
    );

    if (empty($cat_item->in_code_only)) {
      $operations[] = array(
        'title' => $cat_item->is_overwritten ? t('Revert') : t('Delete'),
        'href' => "admin/structure/cat/delete/{$cat_item->machine_name}",
      );
    }

    if ($cat_item->disabled == FALSE) {
      $operations[] = array(
        'title' => t('Disable'),
        'href' => "admin/structure/cat/disable/{$cat_item->machine_name}",
      );
    }

    $form[$cat_item->machine_name]['actions'] = array(
      '#theme' => 'links__ctools_dropbutton',
      '#links' => $operations,
      '#attributes' => array(
        'class' => array(
          'links',
          'inline',
        ),
      ),
    );

    $form[$cat_item->machine_name]['weight'] = array(
      '#type' => 'weight',
      '#default_value' => $cat_item->weight,
      '#delta' => 100,
      '#attributes' => array(
        'class' => array(
          'cat-ui-table-weight',
        ),
      ),
    );

    $form[$cat_item->machine_name]['disabled'] = array(
      '#type' => 'value',
      '#value' => $cat_item->disabled,
    );
  }

  $form['actions'] = array(
    '#type' => 'action',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save'),
    ),
  );

  $form['top row']['category']['#options'] = $category_options;

  $form['bottom row']['apply'] = array(
    '#type' => 'submit',
    '#value' => t('Apply'),
    '#submit' => array(
      'cat_ui_cat_list_submit_ajax',
    ),
    '#attributes' => array(
      'class' => array(
        'use-ajax-submit',
        'ctools-auto-submit-click',
      ),
    ),
  );

  $form['bottom row']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array(
      'cat_ui_cat_list_submit_ajax',
    ),
    '#attributes' => array(
      'class' => array(
        'use-ajax-submit',
      ),
    ),
  );

  $form['#prefix'] = '<div class="clearfix">';
  $form['#suffix'] = '</div>';

  $form['#attached']['js'] = array(ctools_attach_js('auto-submit'));
  $form['#attached']['library'][] = array('system', 'drupal.ajax');
  $form['#attached']['library'][] = array('system', 'jquery.form');
  $form['#attached']['css'][] = drupal_get_path('module', 'cat_ui') . '/cat_ui.css';

  $form['#attributes']['class'][] = 'ctools-auto-submit-full-form';

  return $form;
}

/**
 * Submit callback for cat_ui_cat_list() form.
 */
function cat_ui_cat_list_submit($form, &$form_state) {
  if (!empty($form_state['values']['paths'])) {
    foreach ($form_state['values']['paths'] as $machine_name => $value) {
      if (isset($value['weight'])) {
        $cat_item = $form_state['storage']['objects'][$machine_name];
        $cat_item->weight = $value['weight'];
        cat_save($cat_item);
      }
    }

    drupal_set_message(t('CAT item was updated.'));
  }
}

/**
 * AJAX (CTools) Submit callback for cat_ui_cat_list() form.
 */
function cat_ui_cat_list_submit_ajax($form, &$form_state) {
  $commands = array();
  $commands[] = ajax_command_replace('#cat-ui-table-list-ajax', render($form));
  print ajax_render($commands);
  ajax_footer();
  exit();
}


/**
 * Form for object import.
 */
function cat_ui_import_form($form, $form_state) {
  $form['import'] = array(
    '#type' => 'textarea',
    '#title' => t('Paste code here to import CAT item'),
  );

  $form['actions'] = array(
    '#type' => 'action',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );

  return $form;
}

/**
 * Submit callback for CAT item import form.
 */
function cat_ui_import_form_submit($form, &$form_state) {
  $code = $form_state['values']['import'];
  ctools_include('export');

  $cat_item_raw = ctools_export_crud_import('cat_items', $code);
  if (is_string($cat_item_raw)) {
    form_error($form['import'], t('Unable to get an import from the code. Errors reported: @errors', array('@errors' => $cat_item_raw)));
    $form_state['rebuild'] = TRUE;
  }
  else {
    // Because we get raw object from Ctools we should prepare it before saving.
    $cat_item = cat_load_prepare($cat_item_raw);
    // If there is CAT item object with the same machine name,
    // we delete the original so that this one writes properly.
    if ($cat_item_original = cat_load_by_name($cat_item->machine_name)) {
      cat_delete($cat_item_original->machine_name);
    }
    if (cat_save($cat_item)) {
      if (!empty($cat_item_original)) {
        drupal_set_message(t('The CAT item "!name" was successfully updated.', array('!name' => filter_xss($cat_item->name))));
      }
      else {
        drupal_set_message(t('The CAT item "!name" was successfully imported.', array('!name' => filter_xss($cat_item->name))));
      }
      $form_state['redirect'] = 'admin/structure/cat';
    }
    else {
      form_error($form['import'], t('Could not import CAT item.'));
    }
  }
}

/**
 * CAT settings form.
 */
function cat_ui_settings($form, $form_state) {
  // cat_settings
  $form['cat_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
  );

  $form['cat_settings']['cat_load_prepare_unset_data'] = array(
    '#type' => 'checkbox',
    '#title' => t('Unset %data property', array('%data' => '$cat_item->data')),
    '#description' => t('Use only on production sites.'),
    '#default_value' => variable_get('cat_load_prepare_unset_data', TRUE),
  );

  $form['cat_settings']['cat_show_single_tab'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show single tab'),
    '#description' => t('Single tab will be showed on the page.'),
    '#default_value' => variable_get('cat_show_single_tab', TRUE),
  );

  // cat_cache
  $form['cat_cache'] = array(
    '#type' => 'fieldset',
    '#title' => t('Performance settings'),
  );
  
  $form['cat_cache']['cat_cache_cat_items'] = array(
    '#type' => 'checkbox',
    '#title' => t('Cache CAT items'),
    '#default_value' => variable_get('cat_cache_cat_items', FALSE),
  );

  // cat_propeties
  $cat_info = cat_get_info('fields');
  $cat_info_rows = array();
  $cat_info_header = array(
    t('Name'),
    t('Title'),
    t('Description'),
    t('Module'),
  );

  foreach ($cat_info as $cat_info_name => $cat_info_more) {
    $cat_info_rows[] = array(
      $cat_info_name,
      $cat_info_more['name'],
      $cat_info_more['description'],
      $cat_info_more['module']
    );
  }

  $form['cat_propeties'] = array(
    '#type' => 'fieldset',
    '#title' => t('CAT propeties'),
  );

  $form['cat_propeties']['table'] = array(
    '#markup' => theme('table', array(
      'header' => $cat_info_header,
      'rows' => $cat_info_rows,
      'attributes' => array(
        'id' => 'cat-propeties-table',
      ),
    )),
  );

  $form['#submit'][] = 'cat_ui_settings_submit';

  $form['#attached']['css'][] = drupal_get_path('module', 'cat_ui') . '/cat_ui.css';

  return system_settings_form($form);
}

/**
 * Submit function for cat_ui_settings form.
 */
function cat_ui_settings_submit($form, &$form_state) {
  // Truncate all cached data.
  cat_clear_cache();
}

