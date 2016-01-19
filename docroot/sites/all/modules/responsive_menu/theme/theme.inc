<?php

/**
 * Returns HTML for a menu link and submenu.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: Structured array data for a menu link.
 *
 * @ingroup themeable
 */
function theme_responsive_menu_link(array $variables) {
  $element = $variables['element'];
  $sub_menu = '';

  $output = l($element['#title'], $element['#href'], $element['#localized_options']);

  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);

    $mlid = $element['#original_link']['mlid'];

    $output .= '<label for="' . $mlid . '" class="toggle-sub" onclick="">&#9658;</label>';
    $output .= '<input type="checkbox" id="' . $mlid . '" class="sub-nav-check"/>';
  }

  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}

/**
 * Implements template_preprocess_HOOK() for theme_responsive_menu_tree().
 */
function template_preprocess_responsive_menu_tree(&$variables) {
  $tree = reset($variables['tree']);
  $depth = $tree['#original_link']['depth'];
  $parent = !empty($tree['#original_link']['plid']) ? menu_link_load($tree['#original_link']['plid']) : FALSE;
  $variables['depth'] = $depth;
  $variables['mlid'] = $tree['#original_link']['mlid'];
  $variables['parent_title'] = $parent ? $parent['link_title'] : FALSE;
  $variables['plid'] = $parent ? $parent['mlid'] : FALSE;
}

/**
 * Returns HTML for a wrapper for a menu sub-tree.
 *
 * @param $variables
 *   An associative array containing:
 *   - tree: An HTML string containing the tree's items.
 *
 * @see template_preprocess_responsive_menu_tree()
 * @ingroup themeable
 */
function theme_responsive_menu_tree($variables) {
  $plid = $variables['plid'];
  $parent_title = $variables['parent_title'];

  if ($variables['depth'] > 1) {
    $content = '<ul id="menu-tree-mlid-' . $plid . '" class="sub-nav">';
    $content .= '<li class="sub-heading">' . $parent_title . ' <label for="' . $plid . '" class="toggle" onclick="" title="Back">&#9664;</label></li>';
    $content .= $variables['tree']['#children'] . '</ul>';
    return $content;
  }
  else {
    return '<ul>' . $variables['tree']['#children'] . '</ul>';
  }
}

/**
 * Returns HTML for the menu overview form into a table.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @ingroup themeable
 */
function theme_responsive_menu_overview_form($variables) {
  $form = $variables['form'];

  drupal_add_tabledrag('menu-overview', 'match', 'parent', 'menu-plid', 'menu-plid', 'menu-mlid', TRUE, MENU_MAX_DEPTH - 1);
  drupal_add_tabledrag('menu-overview', 'order', 'sibling', 'menu-weight');

  $header = array(
    t('Menu link'),
    array('data' => t('Enabled'), 'class' => array('checkbox')),
    array('data' => t('Fly-left'), 'class' => array('checkbox')),
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

      // Change the parent field to a hidden. This allows any value but hides the field.
      $element['plid']['#type'] = 'hidden';

      $row = array();
      $row[] = theme('indentation', array('size' => $element['#item']['depth'] - 1)) . drupal_render($element['title']);
      $row[] = array('data' => drupal_render($element['hidden']), 'class' => array('checkbox', 'menu-enabled'));
      $row[] = array('data' => drupal_render($element['flyleft']), 'class' => array('checkbox', 'flyleft'));
      $row[] = drupal_render($element['weight']) . drupal_render($element['plid']) . drupal_render($element['mlid']);
      $row = array_merge($row, $operations);

      $row = array_merge(array('data' => $row), $element['#attributes']);
      $row['class'][] = 'draggable';
      $rows[] = $row;
    }
  }
  $output = '';
  if (empty($rows)) {
    $rows[] = array(array('data' => $form['#empty_text'], 'colspan' => '7'));
  }
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'menu-overview')));
  $output .= drupal_render_children($form);
  return $output;
}