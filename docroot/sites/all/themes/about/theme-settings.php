<?php

/**
 * implementation of hook_form_system_theme_settings_alter()
 */
function about_form_system_theme_settings_alter(&$form, $form_state) {
  // get ready to use the farbtastic color picker
  drupal_add_css('misc/farbtastic/farbtastic.css');
  drupal_add_js('misc/farbtastic/farbtastic.js');
  
  // Get the admin theme so we can set a class for styling this form.
  $admin = drupal_html_class(variable_get('admin_theme', $GLOBALS['theme']));
  $form['#prefix'] = '<div class="admin-theme-' . $admin . '">';
  $form['#suffix'] = '</div>';
  
  $form['about'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => -10,
  );
  
  // move the drupal system theme settings into a vertical tab.
  $drupal_theme_elements = array('theme_settings', 'logo', 'favicon');
  $drupal_tab_needed = false;
  foreach ($drupal_theme_elements as $drupal_theme_element) {
    if (isset($form[$drupal_theme_element])) {
      $drupal_tab_needed = true;
    }
  }
  if ($drupal_tab_needed) {
    $form['about']['drupal'] = array(
      '#type' => 'fieldset',
      '#title' => t('Drupal settings'),
      '#attributes' => array(
        'class' => array('about-drupal'),
      ),
      '#weight' => 99,
    );
  }
  foreach ($drupal_theme_elements as $drupal_theme_element) {
    $form['about']['drupal'][$drupal_theme_element] = $form[$drupal_theme_element];
    unset($form[$drupal_theme_element]);
  }
  
  $form['about']['name'] = array(
    '#type' => 'fieldset',
    '#title' => t('Headline information'),
    '#attributes' => array(
      'class' => array('about-name'),
    ),
  );
  $form['about']['name']['about_name'] = array(
    '#type'  => 'textfield',
    '#title' => t('Name'),
    '#default_value' => theme_get_setting('about_name'),
    '#description' => t("The name that is displayed on the about landing page."),
  );
  $google_fonts = about_get_default_google_fonts();
  array_walk($google_fonts, '_about_replace_space', '+');
  $google_fonts_link = 'http://fonts.googleapis.com/css?family=' . implode('|', $google_fonts);
  $opts['type'] = 'external';
  drupal_add_css($google_fonts_link, $opts);
  $google_fonts = about_get_default_google_fonts();
  foreach ($google_fonts as $k => $font) {
    $font_options[$font] = "<span style=\"font-family:'$font'\">$font</span>";
  }
  $form['about']['name']['about_name_font'] = array(
    '#type' => 'radios',
    '#title' => t('Name Font'),
    '#description' => t('Choose from these default Google webfonts, or select one of your own from the list at <a href="http://www.google.com/webfonts" target="_blank">http://www.google.com/webfonts</a>.'),
    '#options' => $font_options,
    '#theme' => 'about_font_radios',
    '#default_value' => theme_get_setting('about_name_font'),
  );
  $form['about']['name']['about_name_font_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Name Font Size'),
    '#description' => t('How large the name should be in pixels'),
    '#size' => 8,
    '#maxlength' => 3,
    '#field_suffix' => 'px',
    '#default_value' => theme_get_setting('about_name_font_size'),
  );
  $form['about']['name']['about_name_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Page Title Color'),
    '#default_value' => theme_get_setting('about_name_color'),
    '#description' => '<div id="about-name-color-colorpicker"></div>',
    '#size' => 9,
    '#maxlength' => 7,
    '#suffix' => "<script type='text/javascript'>
      (function($) {
        jQuery('#about-name-color-colorpicker').farbtastic('#edit-about-name-color');
      })(jQuery); </script>"
  );
    
  $form['about']['story'] = array(
    '#type' => 'fieldset',
    '#title' => t('Story to tell'),
    '#attributes' => array(
      'class' => array('about-story'),
    ),
  );
  $about_story = theme_get_setting('about_story');
  $form['about']['story']['about_story'] = array(
    '#type'  => 'text_format',
    '#format' =>  isset($about_story['format']) ? $about_story['format'] : null,
    '#title' => t('Story To Tell'),
    '#default_value' => isset($about_story['value']) ? $about_story['value'] : '',
    '#description' => t("The verbiage that is displayed on the about landing page."),
  );
  $form['about']['story']['about_story_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Story Text Color'),
    '#default_value' => theme_get_setting('about_story_color'),
    '#description' => '<div id="about-story-color-colorpicker"></div>',
    '#size' => 9,
    '#maxlength' => 7,
    '#suffix' => "<script type='text/javascript'>
      (function($) {
        jQuery('#about-story-color-colorpicker').farbtastic('#edit-about-story-color');
      })(jQuery); </script>"
  );
  $form['about']['story']['about_story_bg_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Story Background Color'),
    '#default_value' => theme_get_setting('about_story_bg_color'),
    '#description' => '<div id="about-story-bg-color-colorpicker"></div>',
    '#size' => 9,
    '#maxlength' => 7,
    '#suffix' => "<script type='text/javascript'>
      (function($) {
        jQuery('#about-story-bg-color-colorpicker').farbtastic('#edit-about-story-bg-color');
      })(jQuery); </script>"
  );
  
  $form['about']['bg'] = array(
    '#type' => 'fieldset',
    '#title' => t('Background image'),
    '#attributes' => array(
      'class' => array('about-bg'),
    ),
  );
  $form['about']['bg']['about_bg_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to Background Picture'),
    '#default_value' => theme_get_setting('about_bg_path'),
  );
  $form['about']['bg']['about_bg'] = array(
    '#type'  => 'file',
    '#title' => t('Background Picture'),
    '#default_value' => theme_get_setting('about_bg'),
    '#description' => t("The picture to be the background on the about landing page."),
    '#element_validate' => array('_about_bg_upload'),
  );
  $form['about']['bg']['about_bg_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Underlay Color'),
    '#default_value' => theme_get_setting('about_bg_color'),
    '#description' => t('Used if the background image can not scale to fill the whole window.') . '<div id="about-bg-color-colorpicker"></div>',
    '#size' => 9,
    '#maxlength' => 7,
    '#suffix' => "<script type='text/javascript'>
      (function($) {
        jQuery('#about-bg-color-colorpicker').farbtastic('#edit-about-bg-color');
      })(jQuery); </script>"
  );
  
  $form['about']['links'] = array(
    '#type' => 'fieldset',
    '#title' => t('Social media links'),
    '#attributes' => array(
      'class' => array('about-links'),
    ),
  );
  $form['about']['links']['about_facebook'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook'),
    '#description' => t('The Facebook username page to link to'),
    '#field_prefix' => 'facebook.com/',
    '#default_value' => theme_get_setting('about_facebook'),
  );
  $form['about']['links']['about_linkedin'] = array(
    '#type' => 'textfield',
    '#title' => t('LinkedIn'),
    '#description' => t('The LinkedIn username page to link to'),
    '#field_prefix' => 'linkedin.com/in/',
    '#default_value' => theme_get_setting('about_linkedin'),
  );
  $form['about']['links']['about_twitter'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter'),
    '#description' => t('The twitter handle to link to'),
    '#field_prefix' => '@',
    '#default_value' => theme_get_setting('about_twitter'),
  );
  $form['about']['links']['about_flickr'] = array(
    '#type' => 'textfield',
    '#title' => t('Flickr'),
    '#description' => t('The Flickr username page to link to'),
    '#field_prefix' => 'flickr.com/photos/',
    '#default_value' => theme_get_setting('about_flickr'),
  );
  about_include_hook_link_settings($form['about']['links']);
  // todo: add
  //   - drupal profile
  //   - youtube
  //   - pinterest
}

/**
 * add additional elements to the array used in the theme settings form
 * as needed by modules that provide extra links via hook_about_links()
 * @param type $links 
 */
function about_include_hook_link_settings(&$form) {
  $hook_modules = array();
  foreach (module_implements('about_links') as $module) {
    $link_data_array = module_invoke($module, 'about_links');
    $hook_modules[$module] = $link_data_array;
    foreach($link_data_array as $link => $link_data) {
      if (isset($link_data['form elements']) && !empty($link_data['form elements'])) {
        $link_name = $module . '_' . $link;
        $form[$link_name] = $link_data['form elements'];
        if (!isset($form[$link_name]['#default_value'])) {
          $form[$link_name]['#default_value'] = theme_get_setting($link_name);
        }
      }
    }
  }
}


/**
 * Google Font handling
 * @see http://www.google.com/webfonts
 */
function about_get_default_google_fonts() {
  return array(
    'Sarina',
    'Codystar',
    'Sonsie One',
    'Croissant One',
    'Fredericka the Great',
    'Stalemate',
    'Londrina Sketch',
    'Snowburst One',
    'Yesteryear',
    'Elsie Swash Caps',
    'Lobster',
    'Engagement',
    'Ewert',
    'Kavoon',
    'Cinzel',
    'Montserrat Subrayada',
    'Warnes',
    'Arizonia',
    'Rock Salt',
    'Miltonian Tattoo',
    'Creepster',
  );
}

function _about_replace_space(&$item, $key, $replacement) {
  $item = str_replace(' ', $replacement, $item);
}

function _about_bg_upload($form, &$form_state) {
  // Check for a new uploaded file, and use that if available.
  $file_test = file_save_upload('about_bg');
  if ($file_test) {
    
    $file = file_save_upload('about_bg', array(
                                           'file_validate_is_image' => array(), // Validates file is really an image. 
                                           'file_validate_extensions' => array('png gif jpg jpeg'), // Validate extensions.
                                         ));

    if ($file) {
      // make permanent
      $file->status = FILE_STATUS_PERMANENT;
      // move from temporary:// to public://
      if ($public_file = file_move($file, 'public://' . $file->filename)) {
        $_POST['about_bg_path'] = $form_state['values']['about_bg_path'] = file_create_url($public_file->uri);
      }
    }
    else {
      form_set_error('about_bg', t('Please upload only .png, .gif, .jpg, or .jpeg files.'));
    }
    
  }
}