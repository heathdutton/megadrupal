<?php
/**
 * @file
 * Theme methods for the tether_stats module.
 */

/**
 * Implements hook_theme().
 */
function _tether_stats_theme() {

  $path = drupal_get_path('module', 'tether_stats');

  $base = array(
    'file' => 'tether_stats.theme.inc',
    'path' => $path,
  );

  $items = array();

  $items['tether_stats_admin_derivative_entity_form'] = $base + array(
    'render element' => 'form',
  );

  return $items;
}

/**
 * Theme callback for the derivative management form.
 *
 * @param array $vars
 *   An associative array containing:
 *   - form: The renderable form array.
 *
 * @ingroup themeable
 *
 * @see tether_stats_admin_manage_derivatives_form()
 */
function theme_tether_stats_admin_derivative_entity_form(array $vars) {

  $form = $vars['form'];
  $output = '';

  $children = element_children($form);

  if (!empty($children)) {

    $header = array(t('Derivative Name'), t('Description'), t('Actions'));

    $rows = array();

    foreach ($children as $key) {

      $links = array();
      $links[] = l(t('delete'), "admin/config/system/tether_stats/derivatives/delete/{$form[$key]['#value']}");

      if (!empty($form[$key]['#derivative']->disabled)) {

        $links[] = l(t('enable'), "admin/config/system/tether_stats/derivatives/enable/{$form[$key]['#value']}");
      }
      else {

        $links[] = l(t('disable'), "admin/config/system/tether_stats/derivatives/disable/{$form[$key]['#value']}");
      }

      $row = array(
        $form[$key]['#name'],
        $form[$key]['#description'],
        implode(' | ', $links),
      );

      $rows[] = $row;
      drupal_render($form[$key]);
    }

    $output .= theme('table', array('header' => $header, 'rows' => $rows));
  }
  else {

    $output .= '<p>' . t('No derivatives currently exist for this entity type.') . '</p>';
  }
  return $output;
}
