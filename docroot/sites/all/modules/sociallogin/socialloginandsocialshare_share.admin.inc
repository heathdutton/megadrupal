<?php
/**
 * @file
 * Admin page callback file for the socialloginandsocialshare_share module.
 */
/**
 * Function that build an abstract representation of a HTML form for Social Sharing settings.
 *
 * @global string $base_url URL to home page.
 * @param array $form Nested array of form elements that comprise the form.
 * @param array $form_state A keyed array containing the current state of the form.
 * @return array A renderable form array for the respective request.
 */
function socialloginandsocialshare_sharebar_settings_form($form, &$form_state) {
  global $base_url;
  drupal_add_library('system', 'ui.sortable');
  drupal_add_js(drupal_get_path('module', 'socialloginandsocialshare_share') . '/js/socialloginandsocialshare_share.js',  array('type' => 'file', 'scope' => 'footer', 'weight' => 5));
  drupal_add_css(drupal_get_path('module', 'socialloginandsocialshare_share') . '/css/socialloginandsocialshare_share.css');
  drupal_add_js('jQuery(document).ready(function() {
var horizontal_image="' . variable_get("socialloginandsocialshare_share_horizontal_images") . '";
var vertical_image="' . variable_get("socialloginandsocialshare_share_vertical_images") . '";
if (horizontal_image == 8 || horizontal_image == 9) {
counter_horizontal_show();
}
else {
sharing_horizontal_show();
if (horizontal_image == 2 || horizontal_image == 3) {
sharing_simplehorizontal_show();
}
}
if (vertical_image == 6 || vertical_image == 7) {
counter_vertical_show();
}
else {
sharing_vertical_show();
}
var selected_theme = "' . variable_get("socialloginandsocialshare_share_selected_share_interface") . '";
if (selected_theme == "vertical"){
hidden_horizontal_widget();
}
else {
display_horizontal_widget();
}
});', array('type' => 'inline', 'scope' => 'header', 'weight' => 5));
  $my_path = drupal_get_path('module', 'socialloginandsocialshare_share');
  $share_group = 'share';
  $form[$share_group] = array(
    '#type' => 'fieldset',
    '#title' => t('Social Sharing Settings'),
  );
  $form[$share_group]['socialloginandsocialshare_share_horizontal'] = array(
    '#type' => 'item',
    '#prefix' => '<div><b>' . t('What social sharing widget theme do you want to use across your website?<div class="description">Horizontal and Vertical themes can be enabled simultaneously</div>') . '</b></div>',
    '#markup' => ' <div id="lr_tabs"><ul><li><a id="socialloginandsocialshare_share_horizontal" onclick="display_horizontal_widget();">Horizontal widget</a></li><li><a id="socialloginandsocialshare_share_veritical" onclick="hidden_horizontal_widget();">Vertical widget</a></li></ul>'
  );
  $form[$share_group]['socialloginandsocialshare_share_selected_share_interface'] = array(
    '#type' => 'hidden',
    '#title' => t('selected share interface'),
    '#default_value' => variable_get('socialloginandsocialshare_share_selected_share_interface'),
    '#suffix' => '<div id=lrsharing_divwhite></div><div id=lrsharing_divgrey></div><div id="show_horizontal_block">',
  );
  $form[$share_group]['socialloginandsocialshare_share_enable_horizontal_share'] = array(
    '#type' => 'radios',
    '#title' => t('Do you want to enable horizontal social sharing for your website?'),
    '#default_value' => variable_get('socialloginandsocialshare_share_enable_horizontal_share', 1),
    '#options' => array(
      1 => t('Yes'),
      0 => t('No'),
    )
  );
  $form[$share_group]['socialloginandsocialshare_share_enable_vertical_share'] = array(
    '#type' => 'radios',
    '#title' => t('Do you want to enable vertical social sharing for your website'),
    '#default_value' => variable_get('socialloginandsocialshare_share_enable_vertical_share', 0),
    '#options' => array(
      1 => t('Yes'),
      0 => t('No'),
    )
  );
  $form[$share_group]['socialshare_label_string'] = array(
    '#type' => 'textfield',
    '#title' => t('What text do you want to display above the social sharing widget?'),
    '#default_value' => variable_get('socialshare_label_string', 'Share it now!'),
    '#description' => t('Leave empty for no text.'),
  );
  $form[$share_group]['socialloginandsocialshare_share_horizontal_images'] = array(
    '#type' => 'radios',
    '#id' => 'socialloginandsocialshare_share_horizontal_images',
    '#default_value' => variable_get('socialloginandsocialshare_share_horizontal_images', 0),
    '#options' => array(
      0 => '<img src="' . $base_url . '/' . $my_path . '/images/horizonSharing32.png"></img>',
      1 => '<img src="' . $base_url . '/' . $my_path . '/images/horizonSharing16.png"></img>',
      10 => '<img src="' . $base_url . '/' . $my_path . '/images/responsive-icons.png"></img>',
      2 => '<img src="' . $base_url . '/' . $my_path . '/images/single-image-theme-large.png"></img>',
      3 => '<img src="' . $base_url . '/' . $my_path . '/images/single-image-theme-small.png"></img>',
      8 => '<img src="' . $base_url . '/' . $my_path . '/images/horizontalvertical.png"></img>',
      9 => '<img src="' . $base_url . '/' . $my_path . '/images/horizontal.png"></img>',
    ),
  );
  $form[$share_group]['socialloginandsocialshare_share_vertical_images'] = array(
    '#type' => 'radios',
    '#id' => 'socialloginandsocialshare_share_vertical_images',
    '#default_value' => variable_get('socialloginandsocialshare_share_vertical_images', 4),
    '#options' => array(
      4 => '<img id= "32VerticlewithBox" src="' . $base_url . '/' . $my_path . '/images/32VerticlewithBox.png"></img>',
      5 => '<img id="VerticlewithBox" src="' . $base_url . '/' . $my_path . '/images/16VerticlewithBox.png"></img>',
      6 => '<img id="hybrid-verticle-vertical" src="' . $base_url . '/' . $my_path . '/images/hybrid-verticle-vertical.png"></img>',
      7 => '<img id="hybrid-verticle-horizontal"  src="' . $base_url . '/' . $my_path . '/images/hybrid-verticle-horizontal.png"></img>',
    ),
  );
  $form[$share_group]['socialloginandsocialshare_share_show_horizotal'] = array(
    '#type' => 'hidden',
    '#suffix' => '<div id="socialloginandsocialshare_share_show_horizotal_widget">',
  );
  $counter_providers = variable_get('socialcounter_rearrange_providers_list');

  if (empty($counter_providers)) {
    variable_set('socialcounter_rearrange_providers_list', socialloginandsocialshare_share_default_sharing_networks('socialcounter_rearrange_providers_list'));
  }

  $form[$share_group]['socialshare_counter_show_providers_list'] = array(
    '#type' => 'item',
    '#id' => 'socialcounter_show_providers_list',
    '#class' => 'form-item form-type-checkboxes form-item-socialshare-counter-show-providers-list',
    '#title' => t('What sharing networks do you want to show in the sharing widget? (All other sharing networks will be shown as part of LoginRadius sharing icon)'),
    '#default_value' => variable_get('socialcounter_rearrange_providers_list'),
    '#suffix' => '<div id="socialcounter_hidden_field" style="display:none;">',
  );

  foreach (variable_get("socialcounter_rearrange_providers_list") as $provider) {
    if (!empty($provider)) {
      $raw = $provider;
      $provider = str_replace(array(
        ' ',
        '++',
        '+'
      ), array(
        '',
        'plusplus',
        'plus'
      ), $provider);
      $form[$share_group][$provider] = array(
        '#type' => 'item',
        '#attributes' => array('class' => array('lrshare_' . $provider)),
        '#markup' => '<input type = "hidden" id = "input-lrcounter-' . $provider . '" name = "socialcounter_rearrange_providers_list[]" value = "' . $raw . '" />',
      );
    }
  }
  $share_providers = variable_get("socialloginandsocialshare_share_rearrange_providers_list");

  if (empty($share_providers)) {
    variable_set('socialloginandsocialshare_share_rearrange_providers_list', socialloginandsocialshare_share_default_sharing_networks('socialloginandsocialshare_share_rearrange_providers_list'));
  }

  $form[$share_group]['socialshare_show_providers_list'] = array(
    '#type' => 'item',
    '#id' => 'socialshare_providers_list',
    '#title' => t('What sharing networks do you want to show in the sharing widget? (All other sharing networks will be shown as part of LoginRadius sharing icon)'),
    '#default_value' => variable_get('socialloginandsocialshare_share_rearrange_providers_list'),
    '#prefix' => '</div><div id="loginRadiusSharingLimit" style="color:red; display:none; margin-bottom: 5px;">' . t('You can select only 9 providers.') . '</div>',
    '#suffix' => '<div id="rearrange_sharing_text"><b>' . t('What sharing network order do you prefer for your sharing widget?(Drag around to set the order)') . '</b></div><ul id="socialloginandsocialshare_share_rearrange_providers" class="socialloginandsocialshare_share_rearrange_providers">',
  );

  foreach (variable_get("socialloginandsocialshare_share_rearrange_providers_list") as $provider) {
    if (!empty($provider)) {
      $form[$share_group]['lrshare_iconsprite32' . $provider] = array(
        '#type' => 'item',
        '#attributes' => array('class' => array('lrshare_' . $provider)),
        '#markup' => '<li id = "edit-lrshare-iconsprite32' . $provider . '" class = "lrshare_iconsprite32 lrshare_' . $provider . '" title = "' . $provider . '" >
      <input type = "hidden" id = "input-lrshare-' . $provider . '" name = "socialloginandsocialshare_share_rearrange_providers_list[]" value = "' . $provider . '" /></li>',
      );
    }
  }

  $form[$share_group]['socialloginandsocialshare_share_rearran_providers_list'] = array(
    '#type' => 'hidden',
    '#prefix' => '</ul>',
    '#suffix' => '</div>',
  );
  $form[$share_group]['socialloginandsocialshare_share_show_veritical'] = array(
    '#type' => 'hidden',
    '#suffix' => '<div id="socialloginandsocialshare_share_show_veritcal_widget">',
  );
  $vertical_counter_providers = variable_get('socialcounter_vertical_rearrange_providers_list');

  if (empty($vertical_counter_providers)) {
    variable_set('socialcounter_vertical_rearrange_providers_list', socialloginandsocialshare_share_default_sharing_networks('socialcounter_vertical_rearrange_providers_list'));
  }

  $form[$share_group]['socialshare_counter_vertical_show_providers_list'] = array(
    '#type' => 'item',
    '#id' => 'socialcounter_vertical_show_providers_list',
    '#title' => t('What sharing networks do you want to show in the sharing widget? (All other sharing networks will be shown as part of LoginRadius sharing icon)'),
    '#default_value' => variable_get('socialcounter_vertical_rearrange_providers_list'),
    '#suffix' => '<div id="socialcounter_vertical_hidden_field" style="display:none;">',
  );

  foreach (variable_get("socialcounter_vertical_rearrange_providers_list") as $provider) {
    if (!empty($provider)) {
      $raw = $provider;
      $provider = str_replace(array(
        ' ',
        '++',
        '+'
      ), array(
        '',
        'plusplus',
        'plus'
      ), $provider);
      $form[$share_group]['lrshare_vertical_' . $provider] = array(
        '#type' => 'item',
        '#attributes' => array('class' => array('lrshare_vertical_' . $provider)),
        '#markup' => '<input type = "hidden" id = "input-lrcounter-vertical-' . $provider . '" name = "socialcounter_vertical_rearrange_providers_list[]" value = "' . $raw . '" />',
      );
    }
  }

  $vertical_share_providers = variable_get("socialshare_vertical_rearrange_providers_list");

  if (empty($vertical_share_providers)) {
    variable_set('socialshare_vertical_rearrange_providers_list', socialloginandsocialshare_share_default_sharing_networks('socialshare_vertical_rearrange_providers_list'));
  }

  $form[$share_group]['socialshare_vetical_show_providers_list'] = array(
    '#type' => 'item',
    '#id' => 'socialshare_vetical_show_providers_list',
    '#title' => t('What sharing networks do you want to show in the sharing widget? (All other sharing networks will be shown as part of LoginRadius sharing icon)'),
    '#default_value' => variable_get('socialshare_vertical_rearrange_providers_list'),
    '#prefix' => '</div><div id="loginRadiusSharingLimit_vertical" style="color:red; display:none; margin-bottom: 5px;">' . t('You can select only 9 providers.') . '</div>',
    '#suffix' => '<div id="rearrange_sharing_text_vertical"><b>' . t('What sharing network order do you prefer for your sharing widget?(Drag around to set the order)') . '</b></div><ul id="socialshare_vertical_rearrange_providers" class="socialshare_vertical_rearrange_providers">',
  );

  foreach (variable_get("socialshare_vertical_rearrange_providers_list") as $provider) {
    if (!empty($provider)) {
      $form[$share_group]['lrshare_iconsprite32_vertical' . $provider] = array(
        '#type' => 'item',
        '#attributes' => array('class' => array('lrshare_' . $provider)),
        '#markup' => '<li id = "edit-lrshare-iconsprite32_vertical' . $provider . '" class = "lrshare_iconsprite32 lrshare_' . $provider . '" title = "' . $provider . '" >
      <input type = "hidden" id = "input-lrshare-vertical-' . $provider . '" name = "socialshare_vertical_rearrange_providers_list[]" value = "' . $provider . '" /></li>',
      );
    }
  }

  $form[$share_group]['socialshare_vertical_rearra_providers_list'] = array(
    '#type' => 'hidden',
    '#prefix' => '</ul>',
    '#suffix' => '</div>',
  );
  $form[$share_group]['socialloginandsocialshare_share_vertical_images']['socialshare_vertical_position'] = array(
    '#type' => 'radios',
    '#title' => t('Select the position of social sharing widget'),
    '#weight' => 50,
    '#default_value' => variable_get('socialshare_vertical_position', 0),
    '#options' => array(
      0 => t('Top Left'),
      1 => t('Top Right'),
      2 => t('Bottom Left'),
      3 => t('Bottom Right'),
    ),
    '#attributes' => array('style' => 'clear:both'),
  );
  $form[$share_group]['socialshare_position_top'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show at the top of content.'),
    '#default_value' => variable_get('socialshare_position_top', 1) ? 1 : 0,
    '#prefix' => '<div id="horizontal_sharing_show" style="clear:both;"> <b>Select the position of Social sharing interface</b>',
  );
  $form[$share_group]['socialshare_position_bottom'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show at the bottom of content.'),
    '#default_value' => variable_get('socialshare_position_bottom', 1) ? 1 : 0,
    '#suffix' => '</div>',
  );
  $form[$share_group]['socialshare_show_pages'] = array(
    '#type' => 'radios',
    '#title' => t('Show social share on specific pages'),
    '#default_value' => variable_get('socialshare_show_pages', 0),
    '#options' => array(
      0 => t('All pages except those listed'),
      1 => t('Only the listed pages'),
    ),
  );
  $form[$share_group]['socialshare_show_exceptpages'] = array(
    '#type' => 'textarea',
    '#default_value' => variable_get('socialshare_show_exceptpages', ''),
    '#description' => t('Enter a page title(you give on page creation) or node id (if url is http://example.com/node/1 then enter 1(node id)) with comma sepreted'),
    '#rows' => 5,
  );
  $form[$share_group]['socialshare_vertical_show_pages'] = array(
    '#type' => 'radios',
    '#title' => t('Show social share on specific pages'),
    '#default_value' => variable_get('socialshare_vertical_show_pages', 0),
    '#options' => array(
      0 => t('All pages except those listed'),
      1 => t('Only the listed pages'),
    ),
  );
  $form[$share_group]['socialshare_vertical_show_exceptpages'] = array(
    '#type' => 'textarea',
    '#default_value' => variable_get('socialshare_vertical_show_exceptpages', ''),
    '#description' => t('Enter a page title(you give on page creation) or node id (if url is http://example.com/node/1 then enter 1(node id)) with comma sepreted'),
    '#rows' => 5,
    '#suffix' => '</div>',
  );
  $form['#submit'][] = 'socialloginandsocialshare_sharebar_settings_submit';
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}

/**
 * Submit Social Sharing settings.
 *
 * @param $form Nested array of form elements that comprise the form.
 * @param $form_state array A keyed array containing the current state of the form
 * @return bool
 */
function socialloginandsocialshare_sharebar_settings_submit($form, &$form_state) {
  form_state_values_clean($form_state);

  foreach ($form_state['values'] as $key => $value) {
    if (is_array($value) && isset($form_state['values']['array_filter'])) {
      $value = array_keys(array_filter($value));
    }
    variable_set($key, $value);
  }
  socialloginandsocialshare_share_save_network_providers($form_state);
  // Clean all values and save into database.
  foreach ($form_state['input'] as $key => $value) {
    if (is_array($value)) {
      variable_set($key, array_filter($value));
    }
  }
  drupal_set_message(t('The configuration options have been saved.'));
  // Clear the cached pages and blocks.
  cache_clear_all();
  menu_rebuild();
  return TRUE;
}

/**
 * Get sharing/Counter providers into database.
 *
 * @param array $form_state A keyed array containing the current state of the form
 */
function socialloginandsocialshare_share_save_network_providers(&$form_state) {
  // Set default value of sharing networks.
  $settings = array(
    'socialloginandsocialshare_share_rearrange_providers_list',
    'socialcounter_rearrange_providers_list',
    'socialshare_vertical_rearrange_providers_list',
    'socialcounter_vertical_rearrange_providers_list'
  );

  foreach ($settings as $value) {
    socialloginandsocialshare_share_save_network_providers_submit($form_state, $value);
  }
}

/**
 * Save the sharing/counter networks.
 *
 * @param array $form_state A keyed array containing the current state of the form
 * @param string $network Key of sharing/counter to get default networks.
 */
function socialloginandsocialshare_share_save_network_providers_submit(&$form_state, $network) {
  if (isset($form_state['input'][$network])) {
    $form_state['input'][$network] = array_filter($form_state['input'][$network]);
  }
  else {
    $form_state['input'][$network] = socialloginandsocialshare_share_default_sharing_networks($network);
  }
}

/**
 * Get default values of sharing/counter networks.
 *
 * @param string $key sharing/counter key
 * @return array default sharing/counter networks
 */
function socialloginandsocialshare_share_default_sharing_networks($key) {
  if ($key == 'socialloginandsocialshare_share_rearrange_providers_list' || $key == 'socialshare_vertical_rearrange_providers_list') {
    return array("facebook", "googleplus", "google", "pinterest", "twitter");
  }
  return array(
    "Facebook Like",
    "Google+ +1",
    "Pinterest Pin it",
    "LinkedIn Share"
  );
}
