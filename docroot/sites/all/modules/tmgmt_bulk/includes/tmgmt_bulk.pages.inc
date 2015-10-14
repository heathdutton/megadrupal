<?php

/**
 * @file
 * User interface callbacks.
 */

/**
 * Form callback: items selector for bulk job creation.
 */
function tmgmt_bulk_form($form, &$form_state) {

  // Always clear session variables here.
  unset($_SESSION['tmgmt_bulk_storage']);
  unset($_SESSION['tmgmt_bulk_target_language']);
  unset($_SESSION['tmgmt_bulk_jobs_by_source_language']);

  $languages = array();
  foreach (language_list() as $langcode => $language) {
    $languages[$langcode] = $language->name;
  }

  $label_inline = array(
    '#prefix' => '<div class="container-inline" style="margin: 0 0 10px;">',
    '#suffix' => '</div>',
  );

  $form['plugins'] = array(
    '#tree' => TRUE,
  );
  foreach (tmgmt_source_plugin_info() as $plugin_type => $plugin) {
    if (!empty($plugin['item types']) && ($selector = tmgmt_bulk_selector_plugin_controller($plugin_type))) {
      $form['plugins'][$plugin_type] = array(
        '#type' => 'fieldset',
        '#title' => $plugin['label'],
        '#tree' => TRUE,
      );
      $form['plugins'][$plugin_type]['include'] = array(
        '#type' => 'checkbox',
        '#title' => t('Include'),
      );
      $form['plugins'][$plugin_type]['item_types'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Item types'),
        '#states' => array(
          'visible' => array(
            'input[name="plugins[' . $plugin_type . '][include]"]' => array('checked' => TRUE),
          ),
        ),
        '#options' => $plugin['item types'],
        '#tree' => TRUE,
      );
      $form['plugins'][$plugin_type]['filters'] = array(
        '#type' => 'fieldset',
        '#title' => t('Filters'),
        '#states' => array(
          'visible' => array(
            'input[name="plugins[' . $plugin_type . '][include]"]' => array('checked' => TRUE),
          ),
        ),
        '#tree' => TRUE,
      );
      $form['plugins'][$plugin_type]['filters']['translated_to'] = array(
        '#type' => 'select',
        '#title' => t('Translated to'),
        '#description' => t('When language is selected: the result will only include items that are already translated to the specified target language.'),
        '#options' => array('' => '') + $languages,
      ) + $label_inline;
      $form['plugins'][$plugin_type]['filters']['not_exists_in_jobs'] = array(
        '#type' => 'select',
        '#title' => t('Does not exist in jobs with target language'),
        '#description' => t('When language is selected: the result will not include items that are already exist in jobs with the specified target language.'),
        '#options' => array('' => '') + $languages,
      ) + $label_inline;
      $form['plugins'][$plugin_type]['filters']['specific'] = array(
        '#tree' => TRUE,
      ) + $selector->getFormSpecificFilters();
    }
  }

  if (!element_children($form['plugins'])) {
    $form['description']['#markup'] = t('No plugins found.');
    return $form;
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['request_translation'] = array(
    '#type' => 'submit',
    '#value' => t('Request translation'),
  );
  $form['actions']['add_to_cart'] = array(
    '#type' => 'submit',
    '#value' => t('Add to cart'),
  );
  return $form;
}

/**
 * Form submit handler for the tmgmt_bulk_form form.
 */
function tmgmt_bulk_form_submit(&$form, &$form_state) {
  switch ($form_state['triggering_element']['#value']) {
    case t('Add to cart'):
      $batch_redirect = 'admin/tmgmt/bulk/add-to-cart/0';
      break;

    case t('Request translation'):
      $batch_redirect = 'admin/tmgmt/bulk/request-translation';
      break;

    default:
      return;
  }
  $values = isset($form_state['values']['plugins']) ? $form_state['values']['plugins'] : array();
  $operations = array();
  foreach ($values as $plugin => $state) {
    if ($state['include'] && ($item_types = array_filter($state['item_types']))) {
      foreach ($item_types as $item_type) {
        $operations[] = array(
          '_tmgmt_bulk_search_and_store', array(
            $plugin,
            $item_type,
            $state['filters'],
          ),
        );
      }
    }
  }
  if (!empty($operations)) {
    $batch = array(
      'title' => t('Searching for items...'),
      'operations' => $operations,
      'file' => drupal_get_path('module', 'tmgmt_bulk') . '/includes/tmgmt_bulk.batch.inc',
    );
    batch_set($batch);
    batch_process($batch_redirect);
  }
}

/**
 * Form callback: request translation for the items from the storage.
 */
function tmgmt_bulk_request_translation_form($form, &$form_state) {
  if (empty($_SESSION['tmgmt_bulk_storage']) || !($items_count = count($_SESSION['tmgmt_bulk_storage'], COUNT_RECURSIVE))) {
    drupal_set_message(t('No items was found.'), 'warning');
    drupal_goto('admin/tmgmt/bulk');
  }
  $languages = array();
  foreach (language_list() as $langcode => $language) {
    $languages[$langcode] = $language->name;
  }
  $form['description'] = array(
    '#markup' => format_plural($items_count, 'You are going to request translation for 1 item.', 'You are going to request translation for @count items.'),
  );
  $form['target_language'] = array(
    '#type' => 'select',
    '#title' => t('Target language'),
    '#options' => $languages,
    '#required' => TRUE,
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['request_translation'] = array(
    '#type' => 'submit',
    '#value' => t('Request translation'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'admin/tmgmt/bulk',
  );
  return $form;
}

/**
 * Form submit handler for the tmgmt_bulk_request_translation_form form.
 */
function tmgmt_bulk_request_translation_form_submit(&$form, &$form_state) {
  $_SESSION['tmgmt_bulk_target_language'] = $form_state['values']['target_language'];
  $operations = array();
  $items = array();
  foreach ($_SESSION['tmgmt_bulk_storage'] as $plugin => $_) {
    foreach ($_ as $item_type => $item_ids) {
      foreach ($item_ids as $item_id) {
        if (count($items) == 50) {
          $operations[] = array(
            '_tmgmt_bulk_request_translation', array($items),
          );
          $items = array();
        }
        else {
          $items[] = array(
            'plugin' => $plugin,
            'item_type' => $item_type,
            'item_id' => $item_id,
          );
        }
      }
    }
  }
  $operations[] = array('_tmgmt_bulk_request_translation', array($items));
  $batch = array(
    'title' => t('Creating job(s)...'),
    'operations' => $operations,
    'file' => drupal_get_path('module', 'tmgmt_bulk') . '/includes/tmgmt_bulk.batch.inc',
  );
  batch_set($batch);
  batch_process('admin/tmgmt/bulk');
}

/**
 * Page callback: adds storage items to the cart.
 */
function tmgmt_bulk_add_to_cart($force) {
  if (empty($_SESSION['tmgmt_bulk_storage']) || !($items_count = count($_SESSION['tmgmt_bulk_storage'], COUNT_RECURSIVE))) {
    drupal_set_message(t('No items was found.'), 'warning');
    drupal_goto('admin/tmgmt/bulk');
  }
  $tmgmt_cart_count = !(empty($_SESSION['tmgmt_cart'])) ? count($_SESSION['tmgmt_cart']) : 0;
  if (!$force && ($items_count + $tmgmt_cart_count) > 1000) {
    return drupal_get_form('tmgmt_bulk_add_to_cart_warning_form', $items_count, $tmgmt_cart_count);
  }
  $i = 0;
  foreach ($_SESSION['tmgmt_bulk_storage'] as $plugin => $_) {
    foreach ($_ as $item_type => $item_ids) {
      foreach ($item_ids as $item_id) {
        if (tmgmt_ui_cart_get()->addJobItem($plugin, $item_type, $item_id)) {
          $i++;
        }
      }
    }
  }
  unset($_SESSION['tmgmt_bulk_storage']);
  drupal_set_message(format_plural($i, '1 item has been added to the cart.', '@count items have been added to the cart.'));
  drupal_goto('admin/tmgmt/bulk');
}

/**
 * Form callback: warns about big number of items scheduled to be added to cart.
 */
function tmgmt_bulk_add_to_cart_warning_form($form, &$form_state, $items_count, $tmgmt_cart_count) {
  $total = $items_count + $tmgmt_cart_count;
  $args = array(
    '%items_count' => $items_count,
    '%tmgmt_cart_count' => $tmgmt_cart_count,
    '%total' => $total,
  );
  $form['description'] = array(
    '#markup' => t('WARNING: You are above to add %items_count item(s) to the cart (which already contains %tmgmt_cart_count items(s)).
<br />The total amount of the items in the cart will become %total, and this could lead to the following problems:
<br />1) the cart UI can become unresponsive,
<br />2) the operation itself can take long time, and can be interrupted by the PHP execution time limit.
<br />Are you sure you want to continue?', $args),
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['request_translation'] = array(
    '#type' => 'submit',
    '#value' => t('Request translation'),
  );
  $form['actions']['add_to_cart'] = array(
    '#type' => 'submit',
    '#value' => t('Add to cart'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'admin/tmgmt/bulk',
  );
  return $form;
}

/**
 * Form submit handler for the tmgmt_bulk_add_to_cart_warning_form form.
 */
function tmgmt_bulk_add_to_cart_warning_form_submit(&$form, &$form_state) {
  if ($form_state['triggering_element']['#value'] == t('Request translation')) {
    drupal_goto('admin/tmgmt/bulk/request-translation');
  }
  elseif ($form_state['triggering_element']['#value'] == t('Add to cart')) {
    drupal_goto('admin/tmgmt/bulk/add-to-cart/1');
  }
  else {
    drupal_goto('admin/tmgmt/bulk');
  }
}
