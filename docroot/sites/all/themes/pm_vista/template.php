<?php

/**
 * @file
 * Provides theme functions for Project Management modules
 * TODO: make a template file
 */

/**
 * Implements hook_theme
 */
function pm_vista_theme() {
  return array(
    'user_login' => array(
      'render element' => 'form',
      'template' => 'user-login',
      'arguments' => array('form' => NULL),
      'path' => drupal_get_path('theme', 'pm_vista') . '/templates',
    ),
    'user_register_form' => array(
      'render element' => 'form',
      'template' => 'user-register',
      'arguments' => array('form' => NULL),
      'path' => drupal_get_path('theme', 'pm_vista') . '/templates',
    ),
    'user_pass' => array(
      'render element' => 'form',
      'template' => 'user-pass',
      'arguments' => array('form' => NULL),
      'path' => drupal_get_path('theme', 'pm_vista') . '/templates',
    ),
  );
}

function pm_vista_preprocess_user_login(&$variables) {
  $variables['intro_text'] = t('Login');
  pm_vista_user_form_links($variables);
}

function pm_vista_preprocess_user_register_form(&$variables) {
  $variables['intro_text'] = t('Register');
  pm_vista_user_form_links($variables);
}

function pm_vista_preprocess_user_pass(&$variables) {
  $variables['intro_text'] = t('Forgot Password');
  pm_vista_user_form_links($variables);
}

function pm_vista_user_form_links(&$variables) {
  $variables['rendered'] = drupal_render_children($variables['form']);

  $variables['forgot_password_link'] = (drupal_valid_path('user/password')) ? l(t('Forgot password?'), 'user/password') : '';
  $variables['new_user_register_link'] = (drupal_valid_path('user/register')) ? l(t('Register?'), 'user/register') : '';
  $variables['user_login_link'] = (drupal_valid_path('user/login')) ? l(t('Login?'), 'user/login') : '';
}
/**
 * Catch the theme_pm_dashboard_link function.
 */
function pm_vista_pm_dashboard_link($link_array) {

  $content = '';

  // DEFAULT ICON.
  if (empty($link_array['icon'])) {
    $dt_id = 'pmexpenses';
  }
  else {
    $dt_id = $link_array['icon'];
  }

  $params = array(
    'attributes' => array(
      'class' => array('dashboard-link-title'),
    ),
  );
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
      $link_array['params'] = array(
        'attributes' => array(
          'class' => array('dashboard-link-add'),
        ),
      );
    }
    $link .= l(t('[+]'), 'node/add/' . str_replace('_', '-',
      $link_array['add_type']), $link_array['params']);
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
      '#prefix' => variable_get('pm_icons_display', TRUE) ? '<dt id="' . $dt_id
      . '" class="pmcomponent">' : '<dt class="pmcomponent">',
      '#suffix' => '</dt>',
      '#value' => $link,
      '#weight' => $link_array['weight'],
    );
  }
  return $content;
}
/**
 * Catch the theme_pm_dashboard function.
 */
function pm_vista_pm_dashboard($link_blocks) {
  $content = '<div id="pm-dashboard">';
  if (!empty($link_blocks)) {
    $content .= '<dl class="pm-dashboard clearfix">';
    $flag = TRUE;
    foreach ($link_blocks as $block_id => $link_block_array) {
      $classes = ($flag) ? 'first' : 'last';
      $flag = FALSE;
      $content .= "<div class='pm-dashboard $classes'>";
      if (!empty($link_block_array)) {
        foreach ($link_block_array as $key => $link_array) {
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
 * Catch the theme_pm_form_group function.
 */
function pm_vista_pm_form_group($variables) {
  $element = $variables['element'];
  $row = array();
  foreach (element_children($element) as $key) {
    $val = drupal_render($element[$key]);
    array_push($row, $val);
  }
  if (empty($row)) {
    return '';
  }
  $out[] = "<div class ='pm-vista-form-group clearfix'>";
  foreach ($row as $r) {
    if (empty($r)) {
      continue;
    }
    $out[] = "<div class='pm-vista-form-group-item'>$r</div>";
  }
  $out[] = "</div>";
  return implode('', $out);
}

/**
 * Overriding theme_form_element_label.
 */
function pm_vista_form_element_label($variables) {
  $element = $variables['element'];
  // This is also used in the installer, pre-database setup.
  $t = get_t();

  // If title and required marker are both empty, output no label.
  if ((!isset($element['#title']) || $element['#title'] === '') && empty($element['#required'])) {
    return '';
  }

  // If the element is required, a required marker is appended to the label.
  $required = !empty($element['#required']) ? theme('form_required_marker', array('element' => $element)) : '';

  $title = filter_xss_admin($element['#title']);

  $attributes = array();
  // Style the label as class option to display inline with the element.
  if ($element['#title_display'] == 'after') {
    $attributes['class'] = 'option';
  }
  // Show label only to screen readers to avoid disruption in visual flows.
  elseif ($element['#title_display'] == 'invisible') {
    $attributes['class'] = 'element-invisible';
  }

  if (!empty($element['#id'])) {
    $attributes['for'] = $element['#id'];
  }

  // The leading whitespace helps visually separate fields from inline labels.
  return ' <label' . drupal_attributes($attributes) . '><span class="icon"></span>' . $t('!title !required', array('!title' => $title, '!required' => $required)) . "</label>\n";
}
