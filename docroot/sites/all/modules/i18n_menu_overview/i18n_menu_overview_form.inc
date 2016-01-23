<?php
/**
 * @file
 * The form for my menu language parts of these are from MENU core.
 * but adapted to fit my needs. This contains form + submit + theming.
 */

/**
 * Build the form.
 *
 * Array @param $form.
 * Array @param $form_state.
 * Array @param string $menu.
 * Array @param string $language_mnu_links.
 * Array @return array.
 */
function i18n_menu_overview_menulanguage_form($form, &$form_state, $menu = 'main-menu', $language_mnu_links = 'nl') {
  $form['language_id'] = array(
    '#type' => 'hidden',
    '#value' =>
      $language_mnu_links
  );

  global $menu_admin;
  $form['#attached']['css'] = array(drupal_get_path('module', 'menu') . '/menu.css');
  // Getting it with the query builder.
  $result = i18n_menu_overview_get_menu_links_by_language($language_mnu_links, $menu);

  foreach ($result as $item) {
    $links[] = get_object_vars($item);
  }

  $form['#menu'] = $menu;
  if (!empty($links)) {
    $tree = menu_tree_data($links);
    $node_links = array();
    menu_tree_collect_node_links($tree, $node_links);
    // We indicate that a menu administrator is running the menu access check.
    $menu_admin = TRUE;
    menu_tree_check_access($tree, $node_links);
    $menu_admin = FALSE;

    $form = array_merge($form, _i18n_menu_overview_menulanguage_form($tree));
    if (element_children($form)) {
      $form['actions'] = array('#type' => 'actions');
      $form['actions']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save configuration'),
      );
    }
    return $form;
  }
  else {
    drupal_set_message(t('There are no menu links yet. <a href="@link">@link_text</a>.',
      array(
        '@link' => url('admin/structure/menu/manage/' .
          $form['#menu'] . '/add'),
        '@link_text' => t('Add link')
      )));
    return $form;
  }
}

/**
 * Returns HTML table (draggable).
 *
 * Array @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @ingroup themeable
 */
function theme_i18n_menu_overview_menulanguage_form($variables) {
  $form = $variables['form'];

  $language_id = $form['language_id']['#value'];

  drupal_add_tabledrag('menu-overview', 'match', 'parent', 'menu-plid', 'menu-plid', 'menu-mlid', TRUE, MENU_MAX_DEPTH - 1);
  drupal_add_tabledrag('menu-overview', 'order', 'sibling', 'menu-weight');

  $header = array(
    t('Menu link'),
    array('data' => t('Enabled'), 'class' => array('checkbox')),
    t('Weight'),
    array('data' => t('Operations'), 'colspan' => '3'),
  );

  $rows = array();
  foreach (element_children($form) as $mlid) {
    if (isset($form[$mlid]['hidden'])) {
      $element = &$form[$mlid];
      // Build a list of operations.
      $operations = array();
      foreach (element_children($element['operations']) as $op) {
        $operations[] = array('data' => drupal_render($element['operations'][$op]), 'class' => array('menu-operations'));
      }
      while (count($operations) < 2) {
        $operations[] = '';
      }

      // Add special classes to be used for tabledrag.js.
      $element['plid']['#attributes']['class'] = array('menu-plid');
      $element['mlid']['#attributes']['class'] = array('menu-mlid');
      $element['weight']['#attributes']['class'] = array('menu-weight');

      // Change the parent field to a hidden allows any value b hides the field.
      $element['plid']['#type'] = 'hidden';

      $row = array();
      $row[] = theme('indentation', array(
          'size' => $element['#item']['depth'] - 1)) . drupal_render($element['title']);
      $row[] = array(
        'data' => drupal_render($element['hidden']), 'class' => array('checkbox', 'menu-enabled'));
      $row[] = drupal_render($element['weight']) . drupal_render($element['plid']) . drupal_render($element['mlid']);
      $row = array_merge($row, $operations);

      $row = array_merge(array('data' => $row), $element['#attributes']);
      $row['class'][] = 'draggable';
      $rows[] = $row;
    }
  }
  if (empty($rows)) {
    $rows[] = array(array('data' => t('Empty, no links, please add an translated link by selecting an language for a link.'), 'colspan' => '7'));
  }
  // My theming function.
  $the_menu = isset($variables['form']['#menu']) ? $variables['form']['#menu'] : NULL;
  if (empty($the_menu)) {
    drupal_set_message(t('Please make this menu translatable & add an translated link below.'), 'error');
  }

  $output = theme('i18n_menu_overview_page', array(
    'language_id' => $language_id,
    'menu' => $the_menu,
    'table' => theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'attributes' => array('id' => 'menu-overview')
    )),
    'remaining_form_elements' => drupal_render_children($form)
  ));

  return $output;
}

/**
 * Submit callback ==> core default.
 */
function i18n_menu_overview_menulanguage_form_submit($form, &$form_state) {
  // Updates the weight column for each element in
  // our tabletaking intoaccount that item's new order after the drag and
  // drop actions have beenperformed.
  // When dealing with saving menu items, the order in which these items are
  // saved is critical. If a changed child item is saved before its parent,
  // the child item could be saved with an invalid path past its immediate
  // parent. To prevent this, save items in the form in the same order they
  // are sent by $_POST, ensuring parents are saved first, then their children.
  // See http://drupal.org/node/181126#comment-632270
  // Get the $_POST order.
  $order = array_flip(array_keys($form_state['input']));
  // Update our original form with the new order.
  $form = array_merge($order, $form);

  $updated_items = array();
  $fields = array('weight', 'plid');
  foreach (element_children($form) as $mlid) {
    if (isset($form[$mlid]['#item'])) {
      $element = $form[$mlid];
      // Update any fields that have changed in this menu item.
      foreach ($fields as $field) {
        if ($element[$field]['#value'] != $element[$field]['#default_value']) {
          $element['#item'][$field] = $element[$field]['#value'];
          $updated_items[$mlid] = $element['#item'];
        }
      }
      // Hidden is a special case, the value needs to be reversed.
      if ($element['hidden']['#value'] != $element['hidden']['#default_value']) {
        // Convert to integer rather than boolean due to PDO cast to string.
        $element['#item']['hidden'] = $element['hidden']['#value'] ? 0 : 1;
        $updated_items[$mlid] = $element['#item'];
      }
    }
  }

  // Save all our changed items to the database.
  foreach ($updated_items as $item) {
    $item['customized'] = 1;
    menu_link_save($item);
  }
  drupal_set_message(t('Your configuration has been saved.'));
}

/**
 * Recursive helper function for menu_overview_form().
 *
 * menuTree @param $tree The menu_tree retrieved by menu_tree_data.
 */
function _i18n_menu_overview_menulanguage_form($tree) {
  // Generic codee.
  $base_url_admin_menu_operations = 'admin/structure/menu/item/';
  $options_menu_destination = array('query' => array('destination' => 'admin/structure/' . arg(2)));

  $form = &drupal_static(__FUNCTION__, array('#tree' => TRUE));

  foreach ($tree as $data) {
    $item = $data['link'];
    // Don't show callbacks; these have $item['hidden'] < 0.
    if ($item && $item['hidden'] >= 0) {
      $mlid = 'mlid:' . $item['mlid'];
      // Update the hidden field as i18n menu will disable not current language menus.
      $hidden = db_select('menu_links', 'm')
          ->fields('m', array('hidden'))
          ->condition('mlid', $item['mlid'],'=')
          ->execute()
          ->fetchField();
      $item['hidden'] = $hidden;
      $form[$mlid]['#item'] = $item;
      $form[$mlid]['#attributes'] = $item['hidden'] ? array('class' => array('menu-disabled')) : array('class' => array('menu-enabled'));
      $form[$mlid]['title']['#markup'] = l($item['title'], $item['href'], $item['localized_options']) . ($item['hidden'] ? ' (' . t('disabled') . ')' : '');
      $form[$mlid]['hidden'] = array(
        '#type' => 'checkbox',
        '#title' => t('Enable @title menu link', array('@title' => $item['title'])),
        '#title_display' => 'invisible',
        '#default_value' => !$item['hidden'],
      );
      $form[$mlid]['weight'] = array(
        '#type' => 'weight',
        '#delta' => 50,
        '#default_value' => $item['weight'],
        '#title_display' => 'invisible',
        '#title' => t('Weight for @title', array('@title' => $item['title'])),
      );
      $form[$mlid]['mlid'] = array(
        '#type' => 'hidden',
        '#value' => $item['mlid'],
      );
      $form[$mlid]['plid'] = array(
        '#type' => 'hidden',
        '#default_value' => $item['plid'],
      );
      // Change the options for the links.
      $operations = array();
      // My translate link.
      // Comes in handy and quick for translating things.
      $operations['translate'] = array(
        '#type' => 'link', '#title' => t('translate'), '#href' => $base_url_admin_menu_operations . $item['mlid'] . '/translate' , '#options' => $options_menu_destination);
      // Edit link.
      $operations['edit'] = array(
        '#type' => 'link', '#title' => t('edit'), '#href' => $base_url_admin_menu_operations . $item['mlid'] . '/edit' , '#options' => $options_menu_destination);
      // Only items created by the menu module can be deleted.
      if ($item['module'] == 'menu' || $item['updated'] == 1) {
        $operations['delete'] = array(
          '#type' => 'link', '#title' => t('delete'), '#href' => $base_url_admin_menu_operations . $item['mlid'] . '/delete', '#options' => $options_menu_destination);
      }
      // Set the reset column.
      elseif ($item['module'] == 'system' && $item['customized']) {
        $operations['reset'] = array(
          '#type' => 'link', '#title' => filter_xss(t('Reset')), '#href' => $base_url_admin_menu_operations . $item['mlid'] . '/reset');
      }
      $form[$mlid]['operations'] = $operations;
    }

    if ($data['below']) {
      _i18n_menu_overview_menulanguage_form($data['below']);
    }
  }
  return $form;
}
