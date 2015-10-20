<?php
/**
 * @file
 * Provides theme functions for Project Management modules.
 */

/**
 * Provides theming for the datetime element.
 */
function theme_datetime($element) {
  return theme('form_element', $element, '<div class="container-inline">' . $element['#children'] . '</div>');
}

/**
 * Provides HTML for a Project Management dashboard.
 */
function theme_pm_dashboard($link_blocks) {
  $content = '<div id="pmdashboard">';
  if (!empty($link_blocks)) {
    $content .= '<dl class="pmdashboard clear-block">';
    foreach ($link_blocks as $link_block_array) {
      $content .= '<div class="pmdashboard">';
      if (!empty($link_block_array) AND is_array($link_block_array)) {
        foreach ($link_block_array as $link_array) {
          if (!empty($link_array['theme'])) {
            $content .= theme($link_array['theme'], $link_array);
          }
          else {
            $content .= theme('pm_dashboard_link', $link_array);
          }
        }
      }
      $content .= '</div>';
    }
    $content .= '</dl>';
  }
  else {
    $content .= t('No dashboard links available');
  }
  $content .= '</div>';

  return $content;
}

/**
 * Provides HTML for the Project Management dashboard blocks.
 */
function theme_pm_dashboard_block($link_blocks) {
  $content = '<div id="pmdashboard-block">';
  if (!empty($link_blocks)) {
    foreach ($link_blocks as $link_block_array) {
      if (!empty($link_block_array)) {
        foreach ($link_block_array as $link_array) {
          $content .= '<div class="pmdashboard-icon">';
          if (!empty($link_array['theme'])) {
            $content .= theme($link_array['theme'], $link_array);
          }
          else {
            $content .= theme('pm_dashboard_link', $link_array);
          }
          $content .= '</div>';
        }
      }
    }
  }
  else {
    // No links, hide block.
    return '';
  }
  $content .= '</div>';
  return $content;
}

/**
 * Provides HTML for Project Management links.
 */
function theme_pm_dashboard_link($link_array) {
  $content = '';

  // Default icon.
  if (empty($link_array['icon'])) {
    $dt_id = 'pmexpenses';
  }
  else {
    $dt_id = $link_array['icon'];
  }

  $params = array();
  if (!empty($link_array['nid'])) {
    $params_key = $link_array['node_type'] . '_nid';
    $params['query'] = array($params_key => $link_array['nid']);
  }

  $link = l($link_array['title'], $link_array['path'], $params);

  // ADD PLUS SIGN (node/add)
  if (!empty($link_array['add_type'])) {
    $item = new stdClass();
    $item->type = $link_array['add_type'];
    if (empty($link_array['params'])) {
      $link_array['params'] = array();
    }
    $link .= pm_icon_add('node/add/' . str_replace('_', '-', $link_array['add_type']), $item, $link_array['params']);
  }

  if (empty($link_array['nid']) || 0 == $link_array['nid']) {
    if (variable_get('pm_icons_display', TRUE)) {
      $content .= '<dt id="' . $dt_id . '" class="pmcomponent">';
    }
    else {
      $content .= '<dt class="pmcomponent">';
    }
    $content .= $link;
    $content .= '</dt>';
  }
  else {
    $content = array(
      '#prefix' => variable_get('pm_icons_display', TRUE) ? '<dt id="' . $dt_id . '" class="pmcomponent">' : '<dt class="pmcomponent">',
      '#suffix' => '</dt>',
      '#value' => $link,
      '#weight' => $link_array['weight'],
    );
  }
  return $content;
}

/**
 * Provides HTML for the Project Management dashboard weight table.
 */
function theme_pm_dashboard_links_weight_table($form = array()) {
  $type = $form['#infix'];
  $rows = array();
  foreach ($form as $id => &$value) {
    if ($id[0] == '#') {
      continue;
    }
    $value[$type . '_pm_dashboard_link_weight_' . $id]['#attributes']['class'] = $type . 'dashboard-link-table-weight';

    $row = array();
    $row[] = $value['#value'];
    $row[] = drupal_render($value[$type . '_pm_dashboard_link_active_' . $id]);
    $row[] = drupal_render($value[$type . '_pm_dashboard_link_weight_' . $id]);
    unset($value['#value']);
    if (!empty($row)) {
      $rows[] = array(
        'data' => $row,
        'class' => 'draggable',
      );
    }
  }
  $headers = array(t('Link'), t('Active'), t('Weight'));

  $output = theme('table', array(
    'headers' => $headers,
    'rows' => $rows,
    'attributes' => array('id' => $type . 'dashboard-link-table'),
  ));

  drupal_add_tabledrag($type . 'dashboard-link-table', 'order', 'sibling', $type . 'dashboard-link-table-weight');

  $output .= drupal_render($form);

  return $output;
}
