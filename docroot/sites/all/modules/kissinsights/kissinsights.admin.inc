<?php

/**
 * Menu callback for the kissinsights module settings form.
 */
function kissinsights_admin_form($form_state) {
  $form = array();

  $form['layout'] = array(
    '#type' => 'fieldset',
    '#title' => t('Layout'),
    '#description' => t('Decide where to place the KISSinsights box on page.'),
  );

  $form['layout']['kissinsights_bottom'] = array(
    '#type' => 'textfield',
    '#title' => t('Distance from bottom'),
    '#description' => t('Change the distance (in pixels) form the bottom of page. This is helpful if you have a toolbar or footer at the bottom of the page.'),
    '#default_value' => variable_get('kissinsights_bottom', 0),
    '#size' => 6,
  );

  $form['layout']['kissinsights_position'] = array(
    '#type' => 'select',
    '#title' => t('Anchor at side of page'),
    '#description' => t('Anchor the survey on the left or right side of the page.'),
    '#options' => array('left' => t('Left'), 'right' => t('Right')),
    '#default_value' => variable_get('kissinsights_position', 'right'),
  );

  $form['system'] = array(
    '#type' => 'fieldset',
    '#title' => t('KISSinsights Settings'),
    '#description' => t('Settings from the KISSinsights dashboard page.'),
  );

  if (user_access('enable kissinsights')) {
    $form['system']['kissinsights_enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('KISSinsights box enabled'),
      '#description' => t('Remove the checkbox if you want to disable the KISSinsights box on the website.'),
      '#default_value' => variable_get('kissinsights_enabled', 0),
    );
  }

  $form['system']['kissinsights_user'] = array(
    '#type' => 'textfield',
    '#title' => t('User ID'),
    '#description' => t('This value can be found on the <a href="http://kissinsights.com/dashboard">KISSinsights Dashboard</a> when pressing the \'View code\' button.<br /><br /> The User ID is the \'XXXX\' value from //s3.amazonaws.com/ki.js/XXXX/YY.js.'),
    '#size' => 10,
    '#default_value' => variable_get('kissinsights_user', FALSE),
  );

  $form['system']['kissinsights_script_identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('Javascript identifier'),
    '#description' => t('This value can be found on the <a href="http://kissinsights.com/dashboard">KISSinsights Dashboard</a> when pressing the \'View code\' button.<br /><br /> The Javascript identifier is the \'YY\' value from http://s3.amazonaws.com/ki.js/XXXX/YY.js.'),
    '#size' => 10,
    '#default_value' => variable_get('kissinsights_script_identifier', FALSE),
  );

  $form['page_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Visibility Settings'),
  );

  $options = array(t('Add to every page except the listed pages.'), t('Add to the listed pages only.'));
  $visibility = variable_get('kissinsights_visibility', 0);
  $pages = variable_get('kissinsights_pages', KISSINSIGHTS_PAGES);
  $description = t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));

  $form['page_settings']['kissinsights_visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Add tracking to specific pages'),
    '#options' => $options,
    '#default_value' => $visibility,
  );

  $form['page_settings']['kissinsights_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Pages'),
    '#default_value' => $pages,
    '#description' => $description,
    '#wysiwyg' => FALSE,
    '#rows' => 10,
  );

  return system_settings_form($form);
}

