<?php
/**
 * @file
 * og_content.admin.inc
 */

/**
 * OG Content Administration Menu Links Settings Form.
 */
function og_content_menu_links_settings_form($form, &$form_state) {
  $group = _og_content_get_sample_group();
  if (empty($group)) {
    return $form;
  }

  $items = og_ui_get_group_admin($group->entity_type, $group->entity_id);
  $stored = variable_get('og_content_menu_links_weight', array());
  $form['og_menu']['#tree'] = TRUE;

  foreach ($items as $key => $item) {
    $form['og_menu'][$key] = array(
      'title' => array(
        '#markup' => check_plain($item['title']),
      ),
      'override_title' => array(
        '#type' => 'textfield',
        '#default_value' => !empty($stored[$key]) ? $stored[$key]['override_title'] : NULL,
      ),
      'description' => array(
        '#markup' => check_plain($item['description'])
      ),
      'enabled' => array(
        '#type' => 'checkbox',
        '#default_value' => !empty($stored[$key]) ? $stored[$key]['enabled'] : TRUE,
      ),
      'weight' => array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#default_value' => !empty($stored[$key]) ? $stored[$key]['weight'] : 0,
        '#delta' => 10,
        '#title-display' => 'invisible',
      ),
    );
  }

  uasort($form['og_menu'], 'sort_og_menu_form_links');

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Changes')
  );

  return $form;
}

function sort_og_menu_form_links($a, $b) {
  if ($a['weight']['#default_value'] == $b['weight']['#default_value']) {
    return 0;
  }
  return ($a['weight']['#default_value'] < $b['weight']['#default_value']) ? -1 : 1;
}

/**
 * Submit callback.
 *
 * @see og_content_menu_links_settings_form()
 */
function og_content_menu_links_settings_form_submit($form, &$form_state) {
  $stored = variable_get('og_content_menu_links_weight', array());

  foreach ($form_state['values']['og_menu'] as $id => $item) {
    $stored[$id]['override_title'] = $item['override_title'];
    $stored[$id]['enabled'] = $item['enabled'];
    $stored[$id]['weight'] = $item['weight'];
  }

  variable_set('og_content_menu_links_weight', $stored);
  drupal_set_message(t('Menu links settings saved'));
}

/**
 * Helper function: load a sample group for menu link extraction.
 */
function _og_content_get_sample_group() {
  return db_select('field_data_group_group', 'g')
    ->fields('g')
    ->range(0, 1)
    ->execute()
    ->fetchObject();
}

/**
 * Theme callback for the og_content_menu_links_settings_form form.
 *
 * @see og_content_menu_links_settings_form()
 */
function theme_og_content_menu_links_settings_form($variables) {
  $form = $variables['form'];
  $rows = array();

  if (!empty($form['og_menu'])) {
    foreach (element_children($form['og_menu']) as $id) {
      $form['og_menu'][$id]['weight']['#attributes']['class'] = array('og_menu-item-weight');
      $rows[] = array(
        'data' => array(
          drupal_render($form['og_menu'][$id]['title']),
          drupal_render($form['og_menu'][$id]['override_title']),
          drupal_render($form['og_menu'][$id]['description']),
          drupal_render($form['og_menu'][$id]['enabled']),
          drupal_render($form['og_menu'][$id]['weight']),
        ),
        'class' => array('draggable'),
      );
    }
  }

  $header = array(
    t('Title'),
    t('Override title'),
    t('Description'),
    t('Enabled'),
    t('Weight')
  );
  $table_id = 'og_menu-items-table';

  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => $table_id),
    'empty' => t('In order to manage menu links you have to create at least one group. It will be used for menu links extraction.')
  ));

  $output .= drupal_render_children($form);
  drupal_add_tabledrag($table_id, 'order', 'sibling', 'og_menu-item-weight');

  return $output;
}
