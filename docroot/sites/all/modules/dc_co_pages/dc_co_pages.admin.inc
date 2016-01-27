<?php
/**
 * @file
 * Admin functions for Drupal Commerce Checkout Pages module.
 */

/**
 * Page callback for pages management.
 */
function dc_co_pages_manage_callback() {
  $pages = commerce_checkout_pages();
  $output = drupal_get_form('dc_co_pages_manage_form', $pages);
  return $output;
}

/**
 * Form to manage checkout pages.
 */
function dc_co_pages_manage_form($form, $form_state, $pages) {
  $form['pages']['#tree'] = TRUE;
  foreach ($pages as $page) {
    $id = isset($page['id']) ? $page['id'] : $page['page_id'];
    $form['pages'][$id]['#page'] = $page;
    $form['pages'][$id]['weight'] = array(
      '#type' => 'weight',
      '#delta' => 10,
      '#default_value' => $page['weight'],
      '#attributes' => array('class' => array('dc-co-page-weight')),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Theming function for dc_co_pages_manage_form().
 */
function theme_dc_co_pages_manage_form($variables) {
  $form = $variables['form'];

  $header = array(t('Name'), t('Weight'), t('Status Cart'), t('Buttons'), array('data' => t('Operations'), 'colspan' => 2));
  $rows   = array();
  foreach (element_children($form['pages'], TRUE) as $k) {
    $page = $form['pages'][$k]['#page'];

    $row = array();
    $row[] = t('!title (Machine name: !machine) !help', array(
      '!title'   => $page['title'],
      '!machine' => '<small>' . (isset($page['id']) ? $page['id'] : $page['page_id']) . '</small>',
      '!help'    => "<div class=\"description\">{$page['help']}</div>",
    ));
    $row[] = drupal_render($form['pages'][$k]['weight']);
    $row[] = $page['status_cart'] ? t('Yes') : t('No');
    $row[] = $page['buttons'] ? t('Yes') : t('No');
    $row[] = l(t('Edit'), "admin/commerce/config/checkout/custom-pages/{$k}/edit");

    if (isset($page['custom'])) {
      $row[] = l(t('Delete'), "admin/commerce/config/checkout/custom-pages/{$k}/delete");
    }
    else {
      $row[] = l(t('Reset'), "admin/commerce/config/checkout/custom-pages/{$k}/delete");
    }

    $rows[] = array('data' => $row, 'class' => array('draggable'));
  }

  drupal_add_tabledrag('dc-co-pages-manage-form-table', 'order', 'sibling', 'dc-co-page-weight');
  $output  = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'dc-co-pages-manage-form-table')));
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Submit handler for dc_co_pages_manage_form().
 */
function dc_co_pages_manage_form_submit($form, &$form_state) {
  $v = &$form_state['values'];
  foreach (array_keys($v['pages']) as $id) {
    $weight = $v['pages'][$id]['weight'];
    if ($weight != $form['pages'][$id]['#page']['weight']) {
      if (!$page = dc_co_pages_page_load($id)) {
        $page = array('id' => $id);
      }

      $page['weight'] = $weight;
      dc_co_pages_page_save($page);
    }
  }
}

/**
 * Form to edit checkout page.
 */
function dc_co_pages_edit_page_form($form, $form_state, $page = array()) {
  $form['title'] = array(
    '#type' => 'textfield',
    '#size' => 30,
    '#title' => t('Title'),
    '#required' => TRUE,
    '#default_value' => isset($page['title']) ? $page['title'] : '',
  );

  $form['id'] = array(
    '#type' => 'machine_name',
    '#title' => t('Machine name'),
    '#maxlength' => 32,
    '#required' => TRUE,
    '#default_value' => isset($page['page_id']) ? $page['page_id'] : '',
    '#disabled' => isset($page['id']),
    '#machine_name' => array(
      'exists' => 'dc_co_pages_page_load',
      'source' => array('title'),
    ),
  );

  $form['help'] = array(
    '#type' => 'textarea',
    '#title' => t('Help'),
    '#required' => TRUE,
    '#description' => t('Help text displayed to end user on checkout page.'),
    '#default_value' => isset($page['help']) ? $page['help'] : '',
  );

  $form['status_cart'] = array(
    '#type' => 'checkbox',
    '#title' => t('Status cart'),
    '#default_value' => isset($page['status_cart']) ? $page['status_cart'] : FALSE,
  );

  $form['buttons'] = array(
    '#type' => 'checkbox',
    '#title' => t('Buttons'),
    '#default_value' => isset($page['buttons']) ? $page['buttons'] : FALSE,
  );

  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  if (isset($page['id'])) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete page'),
    );
  }

  if (!empty($page) && empty($page['custom'])) {
    $form['status_cart']['#disabled'] = TRUE;
    $form['buttons']['#disabled'] = TRUE;
  }

  return $form;
}

/**
 *
 * @param type $form
 * @param type $form_state
 */
function dc_co_pages_edit_page_form_submit($form, $form_state) {
  $v = $form_state['values'];

  $record['id']    = $v['id'];
  $record['title'] = filter_xss_admin($v['title']);
  $record['help']  = filter_xss_admin($v['help']);
  $record['status_cart'] = $v['status_cart'];
  $record['buttons'] = $v['buttons'];

  if (FALSE !== dc_co_pages_page_save($record)) {
    drupal_set_message(t('The page has been saved'));

    $form_state['redirect'] = 'admin/commerce/config/checkout/custom-pages';
  }
}

/**
 * Page delete form.
 */
function dc_co_pages_delete_page_form($form, $form_state, $page) {
  $form['#page'] = $page;
  $question = 'Are you sure you want to @action the page %title?';
  $action   = !empty($page['custom']) ? 'delete' : 'reset';
  $question = t($question, array('%title' => $page['title'], '@action' => $action));
  $path = 'admin/commerce/config/checkout/custom-pages';
  return confirm_form($form, $question, $path);
}

function dc_co_pages_delete_page_form_submit($form, &$form_state) {
  dc_co_pages_page_delete($form['#page']);
  $form_state['redirect'] = 'admin/commerce/config/checkout/custom-pages';
}
