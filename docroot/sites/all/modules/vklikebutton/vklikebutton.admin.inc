<?php

/**
 * @file
 * Admin functions for vklikebutton.
 */

/**
 * Settings for the dynamic VK button in full node viewmode and teaser viewmode.
 */
function vklikebutton_common_settings() {
  $form['vklikebutton_appid'] = array(
    '#type' => 'textfield',
    '#title' => t('VK app ID'),
    '#default_value' => variable_get('vklikebutton_appid', ''),
    '#description' => t('Enter valid ID of your VK app.'),
    '#required' => TRUE,
  );
  return system_settings_form($form);
}

/**
 * Settings for the dynamic VK button in full node viewmode and teaser viewmode.
 */
function vklikebutton_dynamic_settings() {
  $vklikebutton_node_options = node_type_get_names();
  $form['vklikebutton_dynamic_description'] = array(
    '#markup' => '<p>' . t("Configure the dynamic Like button. This Like button will like the URL you're visiting. You can set the content types on which the button displays, choose to display it in the content block or it's own block and set the appearance.") . '</p>',
  );
  $form['vklikebutton_dynamic_visibility'] = array(
    '#type' => 'fieldset',
    '#title' => t('Visibility settings'),
    '#collapsible' => FALSE,
  );
  $form['vklikebutton_dynamic_visibility']['vklikebutton_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Display the Like button on these content types:'),
    '#options' => $vklikebutton_node_options,
    '#default_value' => variable_get('vklikebutton_node_types', array('article')),
    '#description' => t('Each of these content types will have the "like" button automatically added to them.'),
  );
  $form['vklikebutton_dynamic_visibility']['vklikebutton_full_node_display'] = array(
    '#type' => 'radios',
    '#title' => t('Where do you want to show the Like button (full node view)?'),
    '#options' => array(t('Content area'), t('Own block'), t('Links area')),
    '#default_value' => variable_get('vklikebutton_full_node_display', 0),
    '#description' => t("If <em>Content area</em> is selected, the button will appear in the same area as the node content. If <em>Own block</em> is selected the Like button gets it's own block, which you can position at the <a href='@url'>block page</a>. When you select <em>Links area</em> the Like button will be visible in the links area, usually at the bottom of the node (When you select this last option you may want to adjust the Appearance settings).", array('@url' => url('admin/structure/block'))),
  );
  $form['vklikebutton_dynamic_visibility']['vklikebutton_teaser_display'] = array(
    '#type' => 'radios',
    '#title' => t('Where do you want to show the Like button on teasers?'),
    '#options' => array(
      t("Don't show on teasers"),
      t('Content area'),
      t('Links area'),
    ),
    '#default_value' => variable_get('vklikebutton_teaser_display', 0),
    '#description' => t('If you want to show the like button on teasers you can select the display area.'),
  );
  $form['vklikebutton_dynamic_appearance'] = array(
    '#type' => 'fieldset',
    '#title' => t('Appearance settings'),
    '#collapsible' => FALSE,
  );
  $form['vklikebutton_dynamic_appearance']['vklikebutton_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width of the iframe (px)'),
    '#default_value' => variable_get('vklikebutton_width', 350),
    '#description' => t('Width of the iframe, in pixels (>200). Default is 350. <em>Note: this value affects only full button type (button with text counter).</em>'),
  );
  $form['vklikebutton_dynamic_appearance']['vklikebutton_height'] = array(
    '#type' => 'select',
    '#options' => array(
      '18' => t('18 px'),
      '20' => t('20 px'),
      '22' => t('22 px'),
      '24' => t('24 px'),
    ),
    '#title' => t('Height of the iframe (px)'),
    '#default_value' => variable_get('vklikebutton_height', 22),
    '#description' => t('Height of the iframe, in pixels. Default is 22. <em>Note: this value affects only full button type (button with text counter).</em>'),
  );
  $form['vklikebutton_dynamic_appearance']['vklikebutton_type'] = array(
    '#type' => 'select',
    '#title' => t('Layout style'),
    '#options' => array(
      'full' => t('Button with text counter'),
      'button' => t('Button with mini counter'),
      'mini' => t('Mini button'),
      'vertical' => t('Mini button with top counter'),
    ),
    '#default_value' => variable_get('vklikebutton_type', 'full'),
    '#description' => t('Determines the type of button.'),
  );
  $form['vklikebutton_dynamic_appearance']['vklikebutton_verb'] = array(
    '#type' => 'select',
    '#title' => t('Verb to display'),
    '#options' => array('0' => t('Мне нравится'), '1' => t('Это интересно')),
    '#default_value' => variable_get('vklikebutton_verb', '0'),
    '#description' => t('The verbiage to display inside the button itself.'),
  );

  $form['vklikebutton_dynamic_appearance']['vklikebutton_weight'] = array(
    '#type' => 'textfield',
    '#title' => t('Weight'),
    '#default_value' => variable_get('vklikebutton_weight', 50),
    '#description' => t('The weight determines where, at the content block, the like button will appear. The larger the weight, the lower it will appear on the node. For example, if you want the button to appear more toward the top of the node, choose <em>-40</em> as opposed to <em>-39, -38, 0, 1,</em> or <em>50,</em> etc. To position the Like button in its own block, go to the <a href="@url">block page</a>.', array('@url' => url('admin/structure/block'))),
  );

  $form['#validate'] = array('vklikebutton_config_form_validate');
  return system_settings_form($form);
}

/**
 * Settings for the static VK Like button block.
 */
function vklikebutton_static_settings() {
  global $base_url;
  $form['vklikebutton_static_block_description'] = array(
    '#markup' => t('<p>Set the settings for static like button. This Like button will like the given URL, no matter on which page it is displayed. To position this block go the <a href="@url">block page</a>.</p>', array('@url' => url('admin/structure/block'))),
  );
  $form['vklikebutton_static_block_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Button settings'),
    '#collapsible' => FALSE,
  );
  $form['vklikebutton_static_block_settings']['vklikebutton_page_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Page title'),
    '#default_value' => variable_get('vklikebutton_page_title', variable_get('site_name', '')),
    '#description' => t('Title of the page to like to display at preview of wall post'),
  );
  $form['vklikebutton_static_block_settings']['vklikebutton_page_description'] = array(
    '#type' => 'textfield',
    '#title' => t('Page description'),
    '#default_value' => variable_get('vklikebutton_page_description', variable_get('site_slogan', '')),
    '#description' => t('Description of the page to lik to display at preview of wall post'),
  );
  $form['vklikebutton_static_block_settings']['vklikebutton_page_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Page text'),
    '#default_value' => variable_get('vklikebutton_page_text', ''),
    '#description' => t('Text to publish at the wall. If leaved blank, page title will be used by default. Max length is 140 characters.'),
    '#maxlength' => 140,
  );
  $form['vklikebutton_static_block_settings']['vklikebutton_page_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Page URL'),
    '#default_value' => variable_get('vklikebutton_page_url', $base_url),
    '#description' => t('URL of the page to like'),
  );
  $form['vklikebutton_static_block_settings']['vklikebutton_page_image'] = array(
    '#type' => 'textfield',
    '#title' => t('Page image URL'),
    '#default_value' => variable_get('vklikebutton_page_image', ''),
    '#description' => t('URL of the image to display at preview of wall post'),
  );

  $form['vklikebutton_static_block_appearance'] = array(
    '#type' => 'fieldset',
    '#title' => t('Appearance settings'),
    '#collapsible' => FALSE,
  );
  $form['vklikebutton_static_block_appearance']['vklikebutton_bl_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width of the iframe (px)'),
    '#default_value' => variable_get('vklikebutton_width', 350),
    '#description' => t('Width of the iframe, in pixels (>200). Default is 350. <em>Note: this value affects only full button type (button with text counter).</em>'),
  );
  $form['vklikebutton_static_block_appearance']['vklikebutton_bl_height'] = array(
    '#type' => 'select',
    '#options' => array(
      '18' => t('18 px'),
      '20' => t('20 px'),
      '22' => t('22 px'),
      '24' => t('24 px'),
    ),
    '#title' => t('Height of the iframe (px)'),
    '#default_value' => variable_get('vklikebutton_height', 22),
    '#description' => t('Height of the iframe, in pixels. Default is 80. <em>Note: lower values may crop the output.</em>'),
  );
  $form['vklikebutton_static_block_appearance']['vklikebutton_bl_type'] = array(
    '#type' => 'select',
    '#title' => t('Layout style'),
    '#options' => array(
      'full' => t('Button with text counter'),
      'button' => t('Button with mini counter'),
      'mini' => t('Mini button'),
      'vertical' => t('Mini button with top counter'),
    ),
    '#default_value' => variable_get('vklikebutton_type', 'full'),
    '#description' => t('Determines the type of button.'),
  );
  $form['vklikebutton_static_block_appearance']['vklikebutton_bl_verb'] = array(
    '#type' => 'select',
    '#title' => t('Verb to display'),
    '#options' => array('0' => t('Мне нравится'), '1' => t('Это интересно')),
    '#default_value' => variable_get('vklikebutton_verb', '0'),
    '#description' => t('The verbiage to display inside the button itself.'),
  );
  $form['#validate'] = array('vklikebutton_config_form_validate');
  return system_settings_form($form);
}

/**
 * Validation handler for vklikebutton_dynamic_settings form.
 */
function vklikebutton_config_form_validate($form, &$form_state) {
  if (isset($form_state['values']['vklikebutton_width'])) {
    if ((!is_numeric($form_state['values']['vklikebutton_width'])) || ($form_state['values']['vklikebutton_width'] <= 0)) {
      form_set_error('vklikebutton_width', t('The width of the like button must be a positive number that is greater than 0 (examples: 201 or 450 or 1024).'));
    }
  }
  if (isset($form_state['values']['vklikebutton_height'])) {
    if ((!is_numeric($form_state['values']['vklikebutton_height'])) || ($form_state['values']['vklikebutton_height'] <= 0)) {
      form_set_error('vklikebutton_height', t('The height of the like button must be a positive number that is greater than 0 (examples: 201 or 450 or 1024).'));
    }
  }
  if (isset($form_state['values']['vklikebutton_bl_width'])) {
    if ((!is_numeric($form_state['values']['vklikebutton_bl_width'])) || ($form_state['values']['vklikebutton_bl_width'] <= 0)) {
      form_set_error('vklikebutton_bl_width', t('The width of the like button must be a positive number that is greater than 0 (examples: 201 or 450 or 1024).'));
    }
  }
  if (isset($form_state['values']['vklikebutton_bl_height'])) {
    if ((!is_numeric($form_state['values']['vklikebutton_bl_height'])) || ($form_state['values']['vklikebutton_bl_height'] <= 0)) {
      form_set_error('vklikebutton_bl_height', t('The height of the like button must be a positive number that is greater than 0 (examples: 201 or 450 or 1024).'));
    }
  }
  if (isset($form_state['values']['vklikebutton_weight'])) {
    if (!is_numeric($form_state['values']['vklikebutton_weight'])) {
      form_set_error('vklikebutton_weight', t('The weight of the like button must be a number (examples: 50 or -42 or 0).'));
    }
  }
}
