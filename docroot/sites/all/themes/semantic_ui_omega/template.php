<?php

/**
 * @file
 * Template overrides as well as (pre-)process and alter hooks for the
 * semantic_ui_omega theme.
 */


/**
 * @desc Implements hook_form_alter()
 * Providing better UI elements support for FORM elements.
 */
function semantic_ui_omega_form_alter(&$form, &$form_state, $form_id) {
  $form['#attributes']['class'][] = 'ui form';
  //Using Button Element API
  if (isset($form['actions'])) {
    $tmp_count = 1;
    foreach ($form['actions'] as $action_key => $action) {
      if (isset($action['#type']) && $action['#type'] == 'submit') {
        $class = 'secondary';
        if ($tmp_count == 1) {
          $class = 'primary';
        }
        $form['actions'][$action_key]['#attributes']['class'][] = 'ui button ' . $class;
        $tmp_count++;
      }
    }
  }
  switch ($form_id) {
    case 'search_form':
      if (isset($form['basic']))
        $form['basic']['submit']['#attributes']['class'][] = 'ui button small teal';
      break;

    case 'search_block_form':
      $form['search_block_form']['#attributes']['placeholder'] = t('Search...');
      $form['search_block_form']['#prefix'] = '
        <div class="item">
            <div class="ui icon input">
            <i class="search link icon"></i>';
      $form['search_block_form']['#suffix'] = '</div></div>';
      hide($form['actions']);
      break;

    case 'user_login':
    case 'user_login_block':
      unset($form['name']['#title']);
      $form['name']['#attributes']['placeholder'] = t('Username...');
      $form['name']['#prefix'] = '<div class="ui left icon input">';
      $form['name']['#suffix'] = '<i class="user icon"></i>
        </div>';
      unset($form['pass']['#title']);
      $form['pass']['#attributes']['placeholder'] = t('Password...');
      $form['pass']['#prefix'] = '<div class="ui left icon input">';
      $form['pass']['#suffix'] = '<i class="lock icon"></i>
        </div>';
      break;
  }
}

/**
 * @desc handle main menu
 * @param <type> $variables
 * @return <type>
 */
function semantic_ui_omega_menu_tree($variables) {
  return _semantic_ui_omega_semantic_menu_tree($variables);
}


/**
 * @desc handle main menu childs
 * @param <type> $variables
 * @return <type>
 */
function semantic_ui_omega_menu_link($variables) {
  return _semantic_ui_omega_semantic_menu_link($variables);
}

/**
 *
 * @param <type> $color
 * @return <type>
 */
function _semantic_ui_omega_get_color_code($color = 'purple'){
  return theme_get_setting('semantic_ui_omega_color');
}

/**
 *
 * @param <type> $variables
 * @return <type>
 */
function _semantic_ui_omega_semantic_menu_tree($variables) {
  $color = _semantic_ui_omega_get_color_code();
  return '<div class="ui compact tiered inverted menu small ' . $color . '">
        <div class="menu">
          ' . $variables['tree'] . '
        </div>
      </div>';
}
/**
 *
 * @param <type> $variables
 * @return string
 */
function _semantic_ui_omega_semantic_menu_link($variables) {
  $element = $variables['element'];
  $color = isset($variables['color'])?$variables['color']:_semantic_ui_omega_get_color_code();
  $element['#localized_options']['attributes']['class'][] = 'item '.drupal_html_class("ml-".$element['#title']);
  $link = l($element['#title'], $element['#href'], $element['#localized_options']);
  if (!empty($element['#below'])) {
    $html = '';
    foreach ($element['#below'] as $key => $child_element) {
      if (is_int($key)) {
        $child_element['#localized_options']['attributes']['class'][] = 'item';
        $html .=l($child_element['#title'], $child_element['#href'], $child_element['#localized_options']);
      }
    }
    $link = '<div class="ui dropdown inverted ' . $color . ' link item">
            ' . $link . ' <i class="dropdown icon"></i>
            <div class="menu ui transition hidden inverted ' . $color . '">
              ' . $html . '
            </div>
          </div>
          ';
  }
  return $link;
}
