<?php

/**
 * @file
 * Admin page and form callbacks.
 */

/**
 * Callback for the Personalize Pages list page.
 */
function personalize_pages_list() {
  $header = array(
    array('data' => t('Variation set name')),
    array('data' => t('Operations'), 'colspan' => 2),
  );
  $rows = array();

  foreach (personalize_option_set_load_by_type('pages') as $option_set) {
    $tablerow = array(
      array('data' => check_plain($option_set->label)),
      array('data' => l(t('Edit'), 'admin/structure/personalize/variations/personalize-pages/manage/'. $option_set->osid . '/edit')),
      array('data' => l(t('Delete'), 'admin/structure/personalize/variations/personalize-pages/manage/'. $option_set->osid . '/delete')),
    );
    $rows[] = $tablerow;
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('No pages available.'), 'colspan' => 3));
  }

  $build = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('id' => 'personalize_pages'),
  );
  return $build;
}

/**
 * Form callback for the Personalize Page add/edit form.
 */
function personalize_pages_form($form, &$form_state, $formtype, $option_set = NULL) {
  // When the form is in edit mode, the option set is passed as a param.
  if (!isset($option_set)) {
    $option_set = new stdClass;
    // Set our default option set with a control and one variant.
    $option_set->options = array(
      0 => array('option_label' => personalize_generate_option_label(0)),
      1 => array('option_label' => personalize_generate_option_label(1)),
    );
  }
  if (isset($form_state['option_set'])) {
    $option_set = $form_state['option_set'];
  }
  $form_state['option_set'] = $option_set;

  $form['title'] = array(
    '#title' => t('Title'),
    '#description' => t('This will be the administrative title of the variant.'),
    '#type' => 'textfield',
    '#default_value' => isset($option_set->label) ? $option_set->label : '',
    '#required' => TRUE,
  );

  $agents = array();
  foreach (personalize_agent_load_multiple() as $agent) {
    $agents[$agent->machine_name] = $agent->label;
  }
  $form['agent'] = array(
    '#type' => 'select',
    '#title' => t('Campaign'),
    '#description' => t('Select the campaign to use for this variation set.'),
    '#options' => $agents,
    '#default_value' => !empty($option_set->agent) ? $option_set->agent : NULL,
    '#required' => TRUE,
  );


  $control_option = reset($option_set->options);
  $form['control_page_label'] = array(
    '#title' => t('Control Label'),
    '#description' => t("Enter the label for the control option."),
    '#type' => 'textfield',
    '#default_value' => isset($control_option['option_label']) ? $control_option['option_label'] : '',
    '#required' => TRUE,
  );
  $form['control_page_path'] = array(
    '#title' => t('Page'),
    '#description' => t("Enter the drupal path of a page to personalize. This page will serve as the control. Use %front for the front page.", array('%front' => '<front>')),
    '#type' => 'textfield',
    '#default_value' => isset($control_option['path']) ? $control_option['path'] : '',
    '#required' => TRUE,
  );

  // Add a wrapper for the pages and Add Another Tab button.
  $form['option_sets'] = array(
    '#tree' => FALSE,
    '#theme_wrappers' => array('fieldset'),
    '#title' => t('Variations'),
    '#attributes' => array('id' => 'personalize-pages-wrapper'),
    '#tree' => TRUE,
  );

  $form['option_sets']['pages_more'] = array(
    '#type' => 'submit',
    '#value' => t('Add another page'),
    '#submit' => array('personalize_pages_add_page_submit'),
    '#ajax' => array(
      'callback' => 'personalize_pages_ajax_callback',
      'wrapper' => 'personalize-pages-wrapper',
      'effect' => 'fade',
    ),
    '#limit_validation_errors' => array(),
  );

  // If creating a new personalize_pages instance, start off with 2 empty pages.
  if (empty($option_set->options)) {
    $option_set->options = array(
      0 => array('option_label' => personalize_generate_option_label(0)),
      1 => array('option_label' => personalize_generate_option_label(1)),
    );
  }

  // If the "Add another" button was clicked, we need to increment the number of
  // pages by one.
  $num_pages = count($option_set->options);
  if (isset($form_state['num_pages']) && $form_state['num_pages'] > $num_pages) {
    $option_set->options[] = array('option_label' => personalize_generate_option_label($num_pages));
  }
  $form_state['num_pages'] = count($option_set->options);
  // Keep track of page options throughout Ajax submits.
  $form_state['option_set'] =  $option_set;

  // If the "Remove" button was clicked for a page, we need to remove that page
  // from the form.
  if (isset($form_state['to_remove'])) {
    unset($option_set->options[$form_state['to_remove']]);
    unset($form_state['to_remove']);
    // We need to re-key the array since we may have removed a value at any 
    // index.
    $option_set->options = array_values($option_set->options);
    $form_state['num_pages']--;
  }

  // Add current tabs to the form.
  foreach ($option_set->options as $oid => $page) {
    // We'll skip the control since we already rendered it above.
    if ($oid == 0) {
      continue;
    }
    $form['option_sets'][$oid]['option_label'] = array(
      '#title' => t('Variant label'),
      '#type' => 'textfield',
      '#size' => '10',
      '#default_value' => isset($page['option_label']) ? $page['option_label'] : '',
    );
    $form['option_sets'][$oid]['path'] = array(
      '#description' => t("Enter the drupal path of a page to personalize. This page will serve as the control. Use %front for the front page.", array('%front' => '<front>')),
      '#type' => 'textfield',
      '#default_value' => isset($page['path']) ? $page['path'] : '',
      '#title' => t('Enter a path'),
    );
    $form['option_sets'][$oid]['remove'] = array(
      '#type' => 'submit',
      '#value' => t('Remove'),
      '#id' => 'remove_page_' . $oid,
      '#submit' => array('personalize_pages_remove_page_submit'),
      '#ajax' => array(
        'callback' => 'personalize_pages_ajax_callback',
        'wrapper' => 'personalized-page-variations',
        'method' => 'replace',
        'effect' => 'fade',
      ),
      '#limit_validation_errors' => array(),
    );
  }
  // If there's only two pages, you shouldn't be able to remove any. To do this
  // campaign with one page only would be boneheaded.
  if (count($option_set->options) == 2) {
    $form['option_sets']['pages'][0]['remove']['#access'] = FALSE;
  }

  $form_state['option_set'] = $option_set;

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit_form'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Ajax callback for the add tab and remove tab buttons.
 */
function personalize_pages_ajax_callback($form, $form_state) {
  return $form['option_sets'];
}

/**
 * Submit handler for the "Add Tab" button.
 */
function personalize_pages_add_page_submit($form, &$form_state) {
  // Increment the number of pages to be rendered.
  $form_state['num_pages']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the "Remove Tab" button.
 */
function personalize_pages_remove_page_submit($form, &$form_state) {
  // Get the tab pid for the clicked button.
  $oid = explode('_', $form_state['triggering_element']['#id']);
  $form_state['to_remove'] = end($oid);
  $form_state['rebuild'] = TRUE;
}

/**
 * Form callback for the Personalize Pages delete form.
 */
function personalize_pages_page_delete($form, &$form_state, $personalized_page) {
  $form['osid'] = array('#type' => 'hidden', '#value' => $personalized_page->osid);
  $form['title'] = array('#type' => 'hidden', '#value' => $personalized_page->label);
  return confirm_form($form, t('Are you sure you want to delete the personalized_pages page %title?', array('%title' => $personalized_page->label)), 'admin/structure/personalize/variations/personalize-pages', '', t('Delete'), t('Cancel'));
}


/**
 * Submit handler for Personalize Pages delete form.
 */
function personalize_pages_page_delete_submit($form, &$form_state) {
  personalize_option_set_delete($form_state['values']['osid']);
  drupal_set_message(t('The personalized_pages instance %name has been removed.', array('%name' => $form_state['values']['title'])));
  cache_clear_all();
  $form_state['redirect'] = 'admin/structure/personalize/variations/personalize-pages';
}

/**
 * Submit handler for personalize_blocks admin page.
 */
function personalize_pages_form_validate($form, &$form_state) {
  $paths = array();
  $paths[] = $form_state['values']['control_page_path'];
  foreach ($form_state['values']['option_sets'] as $key => $value) {
    if (!is_array($value) || !isset($value['path'])) {
      continue;
    }
    $paths[$key] = $value['path'];
  }
  foreach ($paths as $key => $path) {
    if (!drupal_valid_path($path)) {
      form_set_error("option_sets][{$key}][path", t('@path does not exist.', array('@path' => $path)));
    }
  }
}

/**
 * Submit handler for personalize_blocks admin page.
 */
function personalize_pages_form_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
  if ($op != t('Save')) {
    return;
  }
  $options = array();
  $options[] = array(
    'option_label' => $form_state['values']['control_page_label'],
    'path' => $form_state['values']['control_page_path'],
  );

  unset($form_state['values']['option_sets']['pages_more']);
  foreach ($form_state['values']['option_sets'] as $option_set) {
    unset($option_set['remove']);
    $options[] = $option_set;
  }

  $values = $form_state['values'];
  $option_set = $form_state['option_set'];
  $option_set->label = $values['title'];
  $option_set->plugin = 'pages';
  $option_set->agent = $values['agent'];
  $option_set->options = $options;
  $option_set->executor = 'personalizePages';

  if (isset($values['osid'])) {
    $option_set->osid = $values['osid'];
  }

  personalize_option_set_save($option_set);
  drupal_set_message(t('The option set was saved.'));
  $form_state['redirect'] = 'admin/structure/personalize/variations/personalize-pages';
}
