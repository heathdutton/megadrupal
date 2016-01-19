<?php

/**
 * @file
 * Admin page callbacks for the Ajax Regions module.
 */

/**
 * Menu callback for the String Overrides module to display its administration.
 */
function ajax_regions_admin($form, $form_state) {
  $form['#attached']['css'][drupal_get_path('module', 'ajax_regions') . '/ajax_regions.admin.css'] = array();

  // Help.
  $help = '<p><b>' . t('Link') . '</b>: ' . t('CSS-selector of the link.') . '</p>';
  $help .= '<p><b>' . t('Ajax-regions (JSON)') . '</b>: ' . t('Enter {regions:selectors} which would be loaded by ajax. Examples: <br /> {"content": ".region-content"} <br /> {"header": "#header", "content": "#content", "footer": "#footer"}') . '</p>';
  $form['help'] = array(
    '#markup' => $help,
  );

  $form['string'] = array(
    '#tree' => TRUE,
    '#theme' => 'ajax_regions_strings',
  );

  $aregions = variable_get("ajax_regions_array", array());

  // Strings.
  $string_count = 0;
  if (isset($form_state['string_count'])) {
    $string_count = $form_state['string_count'];
  }
  else {
    $string_count = count($aregions) + 1;
  }

  if (!isset($form_state['del'])) {
    $form_state['del'] = array();
  }

  for ($index = 0; $index < $string_count; $index++) {
    if (!in_array($index, $form_state['del'])) {
      if (isset($aregions[$index])) {
        $string = $aregions[$index];
        $form['string'][$index] = ajax_regions_tablerow(
          $index,
          $string['link'],
          $string['regions'],
          $string['set_indicator'],
          $string['set_document_title'],
          $string['update_current_address'],
          $string['update_active_links']
        );
      }
      else {
        $form['string'][$index] = ajax_regions_tablerow($index);
      }
    }
  }

  // Link "Add row".
  $form['add_row'] = array(
    '#type' => 'submit',
    '#value' => t('Add row'),
    '#description' => t("If the amount of boxes above isn't enough, click here to add more choices."),
    '#submit' => array('ajax_regions_more_strings_submit'),
    '#ajax' => array(
      'callback' => 'ajax_regions_ajax',
      'wrapper' => 'ajax-regions-wrapper',
      'method' => 'replace',
      'effect' => 'none',
    ),
  );

  $form['ajax_regions_before_loading'] = array(
    '#type' => 'textarea',
    '#title' => t('Before loading'),
    '#default_value' => variable_get('ajax_regions_before_loading', ''),
    '#description' => t('js-function that executes before ajax-loading'),
    '#rows' => 7,
  );

  $form['ajax_regions_after_loading'] = array(
    '#type' => 'textarea',
    '#title' => t('After loading'),
    '#default_value' => variable_get('ajax_regions_after_loading', ''),
    '#description' => t('js-function that executes after ajax-loading'),
    '#rows' => 7,
  );

  // Submit.
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}

/**
 * Function to return a row of the table.
 */
function ajax_regions_tablerow($delta = 0, $link = '', $regions = '', $set_indicator = TRUE, $set_document_title = TRUE, $update_current_address = TRUE, $update_active_links = TRUE) {
  $form['#tree'] = TRUE;
  $form['link'] = array(
    '#type' => 'textfield',
    '#default_value' => $link,
  );
  $form['regions'] = array(
    '#type' => 'textfield',
    '#default_value' => $regions,
  );
  $form['set_indicator'] = array(
    '#type' => 'checkbox',
    '#default_value' => $set_indicator,
  );
  $form['set_document_title'] = array(
    '#type' => 'checkbox',
    '#default_value' => $set_document_title,
  );
  $form['update_current_address'] = array(
    '#type' => 'checkbox',
    '#default_value' => $update_current_address,
  );
  $form['update_active_links'] = array(
    '#type' => 'checkbox',
    '#default_value' => $update_active_links,
  );
  // Link "Delete row".
  $form['del_row'] = array(
    '#type' => 'submit',
    '#value' => t('X'),
    '#submit' => array('ajax_regions_del_strings_submit'),
    '#name' => 'del' . $delta,
    '#attributes' => array('index' => $delta),
    '#ajax' => array(
      'callback' => 'ajax_regions_ajax',
      'wrapper' => 'ajax-regions-wrapper',
      'method' => 'replace',
      'effect' => 'none',
    ),
  );
  return $form;
}

/**
 * Theme two text box strings.
 */
function theme_ajax_regions_strings($variables) {
  $form = $variables['form'];
  $rows = array();
  foreach (element_children($form) as $key) {
    // Build the table row.
    $rows[$key] = array(
      'data' => array(
        array('data' => drupal_render($form[$key]['link'])),
        array('data' => drupal_render($form[$key]['regions'])),
        array('data' => drupal_render($form[$key]['set_indicator'])),
        array('data' => drupal_render($form[$key]['set_document_title'])),
        array('data' => drupal_render($form[$key]['update_current_address'])),
        array('data' => drupal_render($form[$key]['update_active_links'])),
        array('data' => drupal_render($form[$key]['del_row'])),
      ),
    );
  }

  $header = array(
    array('data' => t('Link'), 'title' => t('CSS-selector of the link.')),
    array('data' => t('Ajax-regions')),
    array('data' => t('Set loading-indicator')),
    array('data' => t('Update document title')),
    array('data' => t('Update current address')),
    array('data' => t('Update active links')),
    array('data' => t('Delete row')),
  );

  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => 'ajax-regions-wrapper'),
  ));

  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Menu callback; Display a new string override.
 */
function ajax_regions_ajax($form, &$form_state) {
  return $form['string'];
}

/**
 * Triggered when the user submits the administration page.
 */
function ajax_regions_admin_submit($form, &$form_state) {
  if (!isset($form_state['del'])) {
    $form_state['del'] = array();
  }

  $aregions = array();
  if (isset($form_state['values']['string'])) {
    $i = 0;
    foreach ($form_state['values']['string'] as $index => $string) {
      if (!empty($string['link']) && !empty($string['regions']) && !in_array($index, $form_state['del'])) {
        unset($string['del_row']);
        $aregions[$i++] = $string;
      }
    }
  }

  variable_set("ajax_regions_array", $aregions);
  variable_set('ajax_regions_before_loading', $form_state['values']['ajax_regions_before_loading']);
  variable_set('ajax_regions_after_loading', $form_state['values']['ajax_regions_after_loading']);

  drupal_set_message(t('Your changes have been saved.'));
}

/**
 * Submit handler; The "Add row" button.
 */
function ajax_regions_more_strings_submit($form, &$form_state) {
  if (isset($form_state['string_count'])) {
    $form_state['string_count']++;
  }
  else {
    $form_state['string_count'] = count($form_state['values']['string']) + 1;
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler; The "Delete row" button.
 */
function ajax_regions_del_strings_submit($form, &$form_state) {
  $index = $form_state['clicked_button']['#attributes']['index'];
  if (!isset($form_state['del'])) {
    $form_state['del'] = array();
  }
  $form_state['del'][] = $index;
  $form_state['rebuild'] = TRUE;
}
