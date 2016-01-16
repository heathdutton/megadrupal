<?php
/**
 * @file
 * Menu related theme functions.
 */


/**
 * Overrides theme_menu_local_tasks().
 */
function strapped_menu_local_tasks(&$variables) {
  $output = '';

  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<div class="element-invisible">' . t('Primary tabs') . '</div>';
    $variables['primary']['#prefix'] .= '<ul class="tabs--primary nav nav-tabs">';
    $variables['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['primary']);
  }

  if (!empty($variables['secondary'])) {
    $variables['secondary']['#prefix'] = '<div class="element-invisible">' . t('Secondary tabs') . '</div>';
    $variables['secondary']['#prefix'] .= '<ul class="tabs--secondary nav nav-pills">';
    $variables['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['secondary']);
  }

  return $output;
}

/**
 * Overrides theme_menu_link().
 *
 * @param array $variables
 * @return string
 */
function strapped_menu_link(array $variables) {
  $element = $variables['element'];
  $sub_menu = '';

  if ($element['#below']) {
    // Prevent dropdown functions from being added to management menu so it
    // does not affect the navbar module.
    if (($element['#original_link']['menu_name'] == 'management') && (module_exists('navbar'))) {
      $sub_menu = drupal_render($element['#below']);
    }
    elseif ((!empty($element['#original_link']['depth']))) {
      // Add our own wrapper.
      unset($element['#below']['#theme_wrappers']);
      $sub_menu = '<ul class="">' . drupal_render($element['#below']) . '</ul>';
      // Generate as standard dropdown.
      $element['#title'] .= ' <span class="caret"></span>';

      $element['#localized_options']['html'] = TRUE;

    }
  }
  // On primary navigation menu, class 'active' is not set on active menu item.
  // @see https://drupal.org/node/1896674
  if (($element['#href'] == $_GET['q'] || ($element['#href'] == '<front>' && drupal_is_front_page())) && (empty($element['#localized_options']['language']))) {
    $element['#attributes']['class'][] = 'active';
  }

  // Drupal adds two active classes sometime, don't know why but we'll fix it here.
  if (is_array($element['#attributes']['class'])) {
    $element['#attributes']['class'] = array_unique($element['#attributes']['class']);
  }

  // Strip double classes on links.
  if (isset($element['#localized_options']['attributes']['class'])) {
    $element['#localized_options']['attributes']['class'] = array_unique($element['#localized_options']['attributes']['class']);
  }

  // Add titles in if the user was too lazy to add them.
  if (!isset($element['#localized_options']['attributes']['title']) || $element['#localized_options']['attributes']['title'] == '') {
    $element['#localized_options']['attributes']['title'] = $element['#title'];
  }


  $element['#localized_options']['html'] = 'true';
  $output = l(t('<span>' . $element['#title'] . '</span>'), $element['#href'], $element['#localized_options']);
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}


/**
 * Overrides theme_menu_local_action().
 */
function strapped_menu_local_action($variables) {
  $link = $variables['element']['#link'];

  $options = isset($link['localized_options']) ? $link['localized_options'] : array();

  // If the title is not HTML, sanitize it.
  if (empty($options['html'])) {
    $link['title'] = check_plain($link['title']);
  }

  $icon = _bootstrap_iconize_button($link['title']);

  // Format the action link.
  $output = '<li>';
  if (isset($link['href'])) {
    // Turn link into a mini-button and colorize based on title.

      if (!isset($options['attributes']['class'])) {
        $options['attributes']['class'] = array();
      }
      $string = is_string($options['attributes']['class']);
      if ($string) {
        $options['attributes']['class'] = explode(' ', $options['attributes']['class']);
      }
      $options['attributes']['class'][] = 'btn';
      $options['attributes']['class'][] = 'btn-xs';
      $options['attributes']['class'][] = 'btn-primary';
      if ($string) {
        $options['attributes']['class'] = implode(' ', $options['attributes']['class']);
      }

    // Force HTML so we can add the icon rendering element.
    $options['html'] = TRUE;
    $output .= l($link['title'] . ($icon ? ' ' . $icon  : ''), $link['href'], $options);
  }
  else {
    $output .= $icon . $link['title'];
  }
  $output .= "</li>\n";

  return $output;
}
