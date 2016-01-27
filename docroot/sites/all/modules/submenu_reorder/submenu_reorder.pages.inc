<?php

/**
 * @file
 * provides the drag and drop interface for menu items.
 */

/**
 * Callback function for reorder tab.
 */
function submenu_reorder_node_page_reorder($form, &$form_state, $node) {

  $form['menu_items']['#tree'] = TRUE;
  // Finding mlid of current node path.
  $menu_record = db_select('menu_links', 'ml')
      ->condition('ml.link_path', 'node/' . $node->nid)
      ->fields('ml', array('mlid', 'menu_name'))
      ->execute()
      ->fetchObject();

  // Finding childrens.
  $menu_name = $menu_record->menu_name;

  $menu_all_childrens = db_select('menu_links', 'ml')
      ->condition('ml.plid', $menu_record->mlid)
      ->fields('ml', array('mlid', 'plid', 'weight', 'link_title'))
      ->orderBy('ml.weight', 'ASC')
      ->execute()
      ->fetchAll();
  $rows = '';
  foreach ($menu_all_childrens as $row) {
    $form['menu_items'][$row->mlid] = array(
      'link_title' => array(
        '#markup' => check_plain($row->link_title),
      ),
      'weight' => array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#default_value' => $row->weight,
        '#delta' => 10,
        '#title-display' => 'invisible',
      ),
    );
  }
  $form['menu_name'] = array(
    '#type' => 'hidden',
    '#value' => $menu_name,
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Changes'),
  );
  return $form;
}

/**
 * Returns HTML for a table.
 *
 * @param array $variables
 *   An associative array containing:
 *   - form: An array of form fields for reorder menu items.
 */
function theme_submenu_reorder_node_page_reorder($variables) {
  $form = $variables['form'];

  $rows = array();

  foreach (element_children($form['menu_items']) as $id) {

    $form['menu_items'][$id]['weight']['#attributes']['class'] = array(
      'menu-item-weight',
    );

    $rows[] = array(
      'data' => array(
        drupal_render($form['menu_items'][$id]['link_title']),
        drupal_render($form['menu_items'][$id]['weight']),
      ),
      'class' => array('draggable', 'menu-item-row-' . $id),
    );
  }

  $header = array(t('Menu Items'), t('Weight'));

  $table_id = 'menu-items-reorder-table';
  $output = '';
  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'caption' => t('Please drag and drop the menus to reorder.'),
    'empty' => t('No menu items found!'),
    'attributes' => array('id' => $table_id),
      )
  );

  $output .= drupal_render_children($form);

  drupal_add_tabledrag($table_id, 'order', 'sibling', 'menu-item-weight');

  return $output;
}

/**
 * Implements hook_submit().
 */
function submenu_reorder_node_page_reorder_submit($form, &$form_state) {
  foreach ($form_state['values']['menu_items'] as $mlid => $item) {
    db_update('menu_links')
        ->fields(array(
          'weight' => $item['weight'],
        ))
        ->condition('mlid', $mlid)
        ->execute();
  }
  drupal_set_message(t('The menu items order has been successfully saved.'));
  menu_cache_clear($form_state['values']['menu_name']);
}
