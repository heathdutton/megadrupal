<?php
drupal_add_js('jQuery(function () {jQuery("#edit-layout").fieldset_tabs();});', 'inline');
drupal_add_js($arctica_theme_path . "/features/arctica-layoutkit/layoutkit-theme-settings.js", 'file');

$form['arctica_settings']['layout'] = array(
  '#title' => t('Layout'),
  '#type' => 'fieldset',
);


$form['arctica_settings']['layout']['responsive_enable'] = array(
  '#type'          => 'checkbox',
  '#title'         => t("Enable responsive design for LayoutKit. If you currently don't do !responsive !design you can switch this off. This will simplify settings (removing media query stuff).", array('!responsive' => l(t('responsive'), 'http://www.alistapart.com/articles/responsive-web-design/'), '!design' => l(t('design'), 'http://mediaqueri.es/'))),
  '#default_value' => theme_get_setting('responsive_enable'),
);

if (theme_get_setting('responsive_enable')) {
  $media = array();
  $media_queries = theme_get_setting('media_queries');
  if ($media_queries && is_numeric($media_queries)) {
    for ($i = 1; $i <= $media_queries; $i++) {
      $media[] = 'medium' . $i;
    }
  }
}
else {
  $media = array(t('Default'));
  $media_queries = 1;
}

for ($media_count = 1; $media_count <= $media_queries; $media_count++) {
  $medium = $media[$media_count-1];

  $form['arctica_settings']['layout']["medium{$media_count}"] = array(
    '#title' => t('@medium', array('@medium' => $medium)),
    '#type' => 'fieldset',
  );

  $form['arctica_settings']['layout']["medium{$media_count}"]["layout_type{$media_count}"] = array(
    '#type' => 'radios',
    '#title' => t('Sidebar positions'),
    '#default_value' => (theme_get_setting("layout_type{$media_count}")) ? theme_get_setting("layout_type{$media_count}") : theme_get_setting("layout_type1"),
    '#options' => array(
      1 => t('Sidebars on either sides of main content.'),
      2 => t('Both sidebars on the right.'),
      3 => t('Both sidebars on the left.'),
      4 => t('One column, sidebars stacked below main content'),
      5 => t('Two columns, One sidebar on the right and the other one underneath main content'),
    ),
    '#required' => TRUE,
  );

  $form['arctica_settings']['layout']["medium{$media_count}"]["layout_query{$media_count}"] = array(
    '#type' => 'textarea',
    '#rows' => 2,
    '#title' => t('Media query for this layout'),
    '#default_value' => (theme_get_setting("layout_query{$media_count}")) ? theme_get_setting("layout_query{$media_count}") : theme_get_setting("layout_query1"),
    '#description' => t("Adjust the design breakpoint in the query or construct up your own @media query."),
    '#required' => TRUE,
  );

  $form['arctica_settings']['layout']["medium{$media_count}"]["layout_width{$media_count}"] = array(
    '#type' => 'textfield',
    '#title' => t('Layout width'),
    '#default_value' => (theme_get_setting("layout_width{$media_count}")) ? theme_get_setting("layout_width{$media_count}") : theme_get_setting("layout_width1"),
    '#size' => 9,
    '#maxlength' => 9,
    '#description' => t('Enter a valid CSS width. For example: 960px, 80% or 20em. If you set a flexible width you can also set a max width.'),
    '#element_validate' => array('_arctica_is_width'),
  );

  $form['arctica_settings']['layout']["medium{$media_count}"]["layout_max_width{$media_count}"] = array(
    '#type' => 'textfield',
    '#title' => t('Layout Max-width'),
    '#default_value' => (theme_get_setting("layout_max_width{$media_count}")) ? theme_get_setting("layout_max_width{$media_count}") : theme_get_setting("layout_max_width1"),
    '#size' => 9,
    '#maxlength' => 9,
    '#description' => t('Enter a valid CSS max-width setting. For example "1400px" or "none"'),
    '#element_validate' => array('_arctica_is_fixed_width'),
  );


  $form['arctica_settings']['layout']["medium{$media_count}"]["sidebar_first_width{$media_count}"] = array(
    '#type' => 'select',
    '#title' => t('Sidebar first width'),
    '#default_value' => (theme_get_setting("sidebar_first_width{$media_count}")) ? theme_get_setting("sidebar_first_width{$media_count}") : theme_get_setting("sidebar_first_width1"),
    '#options' => array(
     '12.125' => t('1/8'),
     '14.143' => t('1/7'),
     '16.667' => t('1/6'),
     '20' => t('1/5'),
     '25' => t('1/4'),
     '33.333' => t('1/3'),
     '50' => t('1/2'),
     '75' => t('3/4'),
     '66.667' => t('2/3'),
     '100' => t('100%'),
     '61.803' => t('1/phi (61.8%)'),
     '38.196' => t('1/phi^2 (38.2%)'),
     '23.606' => t('1/phi^3 (23.6%)'),
    ),
  );

  $form['arctica_settings']['layout']["medium{$media_count}"]["sidebar_second_width{$media_count}"] = array(
    '#type' => 'select',
    '#title' => t('Sidebar second width'),
    '#default_value' => (theme_get_setting("sidebar_second_width{$media_count}")) ? theme_get_setting("sidebar_second_width{$media_count}") : theme_get_setting("sidebar_second_width1"),
    '#options' => array(
     '12.125' => t('1/8'),
     '14.143' => t('1/7'),
     '16.667' => t('1/6'),
     '20' => t('1/5'),
     '25' => t('1/4'),
     '33.333' => t('1/3'),
     '50' => t('1/2'),
     '75' => t('3/4'),
     '66.667' => t('2/3'),
     '100' => t('100%'),
     '61.803' => t('1/phi (61.8%)'),
     '38.196' => t('1/phi^2 (38.2%)'),
     '23.606' => t('1/phi^3 (23.6%)'),
    ),
  );

  $form['arctica_settings']['layout']["medium{$media_count}"]["gutter_vertical{$media_count}"] = array(
    '#type' => 'textfield',
    '#title' => t('Vertical gutter size'),
    '#default_value' => (theme_get_setting("gutter_vertical{$media_count}")) ? theme_get_setting("gutter_vertical{$media_count}") : theme_get_setting("gutter_vertical1"),
    '#size' => 9,
    '#maxlength' => 9,
    '#element_validate' => array('_arctica_is_fixed_width'),
  );

  $form['arctica_settings']['layout']["medium{$media_count}"]["gutter_horizontal{$media_count}"] = array(
    '#type' => 'textfield',
    '#title' => t('Horizontal gutter size'),
    '#default_value' => (theme_get_setting("gutter_horizontal{$media_count}")) ? theme_get_setting("gutter_horizontal{$media_count}") : theme_get_setting("gutter_horizontal1"),
    '#size' => 9,
    '#maxlength' => 9,
    '#element_validate' => array('_arctica_is_fixed_width'),
  );
}

$form['arctica_settings']['layout']['media_queries'] = array(
  '#type' => 'textfield',
  '#title' => t('How many media queries?'),
  '#default_value' => theme_get_setting("media_queries"),
  '#size' => 1,
  '#maxlength' => 1,
  '#description' => t('Enter how many media different queries you want to use. Save the form to update the configurator with the new value.'),
  '#element_validate' => array('_arctica_is_number'),
);

$form['arctica_settings']['layout']['meta'] = array(
  '#type' => 'textarea',
  '#rows' => 3,
  '#title' => t('UX Tweaks'),
  '#default_value' => theme_get_setting("meta") == 'RESET' ? theme_get_setting("meta1") . "\r\n" . theme_get_setting("meta2") . "\r\n" . theme_get_setting("meta3") . "\r\n" . theme_get_setting("meta4") . "\r\n" . theme_get_setting("meta5") . "\r\n" . theme_get_setting("meta6") : theme_get_setting("meta"),
  '#description' => t("Enter meta tags for improved browsing experience. These tags are loaded just after the title tag, before your other head contents. Enter RESET to reset this value to the defaults (from the .info file)"),
  '#required' => FALSE,
);

$form['arctica_settings']['layout']['hide_address_bar'] = array(
  '#type' => 'checkbox',
  '#title' => t('Hide address bar'),
  '#default_value' => theme_get_setting('hide_address_bar'),
  '#description' => t('Hides the address bar in Mobile Safari and various other mobile browsers.'),
);

$form['arctica_settings']['layout']['ori_scale'] = array(
  '#type' => 'checkbox',
  '#title' => t('Keep normal font size'),
  '#default_value' => theme_get_setting('ori_scale'),
  '#description' => t('Bugfix for Mobile Safari with orientation switching. Mobile Safari increases font size in landscape mode, which improves readability for most fixed-width designs, while causing excessively large font-size in fluid / responsive designs (because the latter aren\'t zoomed out.)'),
);