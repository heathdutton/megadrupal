<?php
/**
 * @file
 * The admin settings for the FOO module.
 * The Style Settings module offers several form API elements to help
 * developers build the settings page. Cool UI widgets and built-in validation
 * of user input especially designed for CSS attributes.
 */

/**
 * Implements hook_settings().
 */
function FOO_admin_settings() {
  $style_settings_module = l(t('Style (CSS) Settings module'), 'https://drupal.org/project/style_settings', array(
      'attributes' => array(
        'title' => t('Style (CSS) Settings | Drupal.org'),
        'target' => '_blank',
      ),
  ));
  // Put CSS variables together in a fieldset. Remove if only one is given.
  $form['css_variables'] = array(
    '#type' => 'fieldset',
    '#title' => t('CSS variables'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t("This example settings form shows the form API elements offered by the !style_settings_module. They consist of:<ul>
      <li>An image URL with optional file upload.</li>
      <li>A slider (HTML5 range).</li>
      <li>A number.</li>
      <li>A combined value with unit example.</li>
      <li>A color picker.</li>
    </ul>
    All code is included in the demo of the module, ready to be copy/pasted to your own project and customized.", array('!style_settings_module' => $style_settings_module)),
  );
  // The condition below only exists for when code is copy/pasted to elsewhere.
  if (module_exists('style_settings')) {
    // Uncomment line below to collapse the fieldset but open after form submit.
//    $form['css_variables']['#collapsed'] = variable_get('FOO_preview_collapsed', TRUE);
    //
    // Make sure a medium sized default image exists.
    $sample_image = variable_get('image_style_preview_image', drupal_get_path('module', 'image') . '/sample.png');
    $styled_file = image_style_path('medium', $sample_image);
    if (!file_exists($styled_file)) {
      image_style_create_derivative(image_style_load('medium'), $sample_image, $styled_file);
    }
    $sample_image = '/' . variable_get('file_public_path', conf_path() . '/files') . str_replace('public:/', '', $styled_file);
    // A preview area.
    $form['css_variables']['FOO_preview'] = array(
      '#type' => 'fieldset',
      '#title' => t('Preview'),
      '#description' => t('After modifying the settings below, submit the form to verify the result of your changes here.'),
      '#collapsible' => TRUE,
      '#collapsed' => variable_get('FOO_preview_collapsed', TRUE),
    );
    // Form submit resulted in an uncollapsed preview. Set it back.
    variable_set('FOO_preview_collapsed', TRUE);
    $markup = variable_get('FOO_preview_html', array(
      'value' => '<div class="FOO-preview-swatch"><div class="FOO-preview-object"></div></div><div class="FOO-preview-caption">This is a caption.</div>',
      'format' => 'full_html',
    ));
    // Reset to the default preview template if it is found empty.
    $trimmed = trim($markup['value']);
    $markup = empty($trimmed) ? variable_set('FOO_preview_html', array(
      'value' => '<div class="FOO-preview-swatch"><div class="FOO-preview-object"></div></div><div class="FOO-preview-caption">This is a caption.</div>',
      'format' => 'full_html',
    )) : $markup;
    // Use the current user's default format if the stored one isn't available.
    $format_id = filter_format_load($markup['format']) ? $markup['format'] : filter_default_format();
    $form['css_variables']['FOO_preview']['FOO_preview_markup'] = array(
      '#markup' => '<div class="style-settings-preview">' . check_markup($markup['value'], $format_id, '', $cache = FALSE) . '</div>',
    );
    $form['css_variables']['FOO_preview']['FOO_preview_details'] = array(
      '#type' => 'fieldset',
      '#title' => t('Template'),
      '#description' => t('Wrapped in <code>&lt;div class="style-settings-preview"> .. &lt;/div></code>.'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['css_variables']['FOO_preview']['FOO_preview_details']['FOO_preview_html'] = array(
      '#type' => 'text_format',
      '#description' => t('Changes are visible after form submit. Empty the text area to reset to default.'),
      '#default_value' => $markup['value'],
      '#format' => $format_id,
      '#access' => 'use text format ' . $format_id,
    );

    // IMAGE URL field with an optional file UPLOAD. It goes together with a
    // submit handler inside the 'function FOO_admin_settings_submit()'.
    // If you use this code in your project you should either declare the core
    // 'image' module as a dependency in the '.info' file or wrap it inside:
    // if (module_exists('image')) { .. }
    // You could also renounce to the file upload and use a URL field only.
    $sample = l(t('https://unsplash.imgix.net/photo-1414450397866-85f90db48714?fit=crop&fm=jpg&h=165&q=75&w=220'), 'https://unsplash.imgix.net/photo-1414450397866-85f90db48714?fit=crop&fm=jpg&h=165&q=75&w=220', array(
        'attributes' => array(
          'title' => t('photo-1414450397866-85f90db48714 (400×300) | Milo McDowell | Unsplash'),
          'target' => '_blank',
        ),
    ));
    $form['css_variables']['FOO_image'] = array(
      '#type' => 'fieldset',
      '#title' => t('IMAGE SOURCE'),
      // Make containing fields align horizontally.
      '#attributes' => array('class' => array('container-inline')),
      '#description' => t('An absolute (external) or relative (local) image URL. A relative URL must be given from the base URL (<em>/sites/..</em>). Leave all empty to reset to the default image.<br />Try with: !sample', array('!sample' => $sample)),
    );
    // IMAGE UPLOAD example.
    $form['css_variables']['FOO_image']['FOO_imgupload'] = array(
      '#type' => 'managed_file',
      '#description' => t('Alternatively upload an image. Visible after form submit.'),
      '#default_value' => variable_get('FOO_imgupload', ''),
      '#size' => 40,
      '#upload_location' => 'public://FOO/',
      '#theme' => 'image_widget',
    );
    if (!variable_get('FOO_imgupload', '')) {
      variable_set('FOO_imgupload_url', '');
    }
    // IMAGE URL example.
    $uploaded_url = variable_get('FOO_imgupload_url', '');
    $default = empty($uploaded_url) ? variable_get('FOO_imgurl', $sample_image) : $uploaded_url;
    $form['css_variables']['FOO_image']['FOO_imgurl'] = array(
      '#type' => 'style_settings_imgurl',
      // If you use this for a 'theme', replace 'module' below.
      '#default_value' => $default,
      // In the submit handler below we reset an empty field to the default URL.
      // This way the user isn't required to know the URL of the default image.
    );

    // SLIDER WIDGET example.
    $form['css_variables']['FOO_img_opacity'] = array(
      '#type' => 'style_settings_slider',
      '#title' => t('IMAGE OPACITY'),
      '#description' => t('0 = transparent. 1 = opaque.'),
      // The variable default should include a measurement unit if applicable.
      // Wrapped in floatval() to turn it into a number. E.g. '2px' => '2'.
      '#default_value' => floatval(variable_get('FOO_img_opacity', 0.9)),
      // Parameters below could be omitted. It is already preset for opacity.
      '#step' => 0.01,
      '#min' => 0,
      '#max' => 1,
      // The suffix gets added to the input on submit if valid measurement unit.
      // Added for demonstration purpose only.
      'field_suffix' => NULL,
    );

    // A SELECTABLE MEASUREMENT UNIT (e.g. px, em, %) example. It goes together
    // with a submit handler inside the 'function FOO_admin_settings_submit()'.
    $form['css_variables']['FOO_fontsize'] = array(
      '#type' => 'fieldset',
      '#title' => t('CAPTION FONT-SIZE'),
      // Make containing fields align horizontally.
      '#attributes' => array('class' => array('container-inline')),
      // Add optionally a description to the fieldset. NOT to the member fields.
      // Number field input help (min, max, step) will optionally be appended.
      '#description' => t('Note: A minimum font-size setting of your browser might interfere.'),
    );
    // Number field without a '#field_suffix'.
    $form['css_variables']['FOO_fontsize']['FOO_fontsize_value'] = array(
      '#type' => 'style_settings_number',
      '#default_value' => variable_get('FOO_fontsize_value', '110'),
      // Uncomment the line below to NOT show min, max and step values as help.
//      '#input_help' => NULL,
    );
    // A measurement unit select field.
    $form['css_variables']['FOO_fontsize']['FOO_fontsize_unit'] = array(
      '#type' => 'select',
      '#options' => array(
        'px' => t('px'),
        'em' => t('em'),
        '%' => t('%'),
      ),
      '#default_value' => variable_get('FOO_fontsize_unit', '%'),
      '#required' => TRUE,
    );

    // Any normal form API type can be used.
    // A normal TEXTFIELD. It does not offer any validation by itself.
    $form['css_variables']['FOO_caption_align'] = array(
      '#type' => 'textfield',
      '#title' => t('CAPTION TEXT ALIGN'),
      '#default_value' => variable_get('FOO_caption_align', 'center'),
      '#description' => t('A normal TEXTFIELD. It does not offer any validation by itself.'),
      '#size' => 12,
    );

    // NUMBER example in this case with an appended measurement unit (optional).
    // E.g. user input: '2', field_suffix: 'px' => stored variable: '2px'.
    $form['css_variables']['FOO_borderwidth'] = array(
      '#type' => 'style_settings_number',
      '#title' => t('CAPTION BORDER WIDTH'),
      '#description' => t("NUMBER example, in this case with a measurement unit (gets also appended to the stored variable).<br />Automatically validated based on the passed form attributes '#min', '#max' and '#step'."),
      '#step' => 1, // In this case if forces an integer as input.
      '#min' => 0, // Could be omitted. Defaults to 0. Set to NULL to ignore.
      '#max' => 10, // Defaults to 1 if omitted.
      // The variable default should include a measurement unit if applicable.
      // Wrapped in floatval() to turn it into a number. E.g. '2px' => '2'.
      '#default_value' => floatval(variable_get('FOO_borderwidth', '5px')),
      // The suffix gets added to the input on submit if valid measurement unit.
      '#field_suffix' => 'px',
      // Uncomment the line below to NOT align the field input on the right.
//      '#attributes' => NULL,
      // Uncomment the line below to NOT show min, max and step values as help.
//      '#input_help' => NULL,
    );

    // COLOR PICKER example.
    $form['css_variables']['FOO_bordercolor'] = array(
      '#type' => 'style_settings_colorpicker',
      '#title' => t('CAPTION BORDER COLOR'),
      // Besides hex color value also color names are accepted.
      '#default_value' => variable_get('FOO_bordercolor', 'IndianRed'),
    );
  }

  // If the Style Settings module is not enabled, provide some instructions.
  else {
    $form['css_variables']['FOO_note'] = array(
      '#markup' => t("Enable the !style_settings_module to get style options exposed here. They consist of:<ul>
          <li> A caption font-size.</li>
          <li> ... </li>
          <li> ... </li>
        </ul>", array('!style_settings_module' => $style_settings_module)),
    );
  }

  // Call submit_function() on form submission.
  $form['#submit'][] = 'FOO_admin_settings_submit';
  return system_settings_form($form);
}

/**
 * Submit form data.
 */
function FOO_admin_settings_submit($form, &$form_state) {
  if (module_exists('style_settings')) {
    // Uncollapse the preview. Likely we want to see the changes we just made.
    variable_set('FOO_preview_collapsed', FALSE);
    // Make sure a medium sized sample default image exists.
    $sample_image = variable_get('image_style_preview_image', drupal_get_path('module', 'image') . '/sample.png');
    $styled_file = image_style_path('medium', $sample_image);
    if (!file_exists($styled_file)) {
      image_style_create_derivative(image_style_load('medium'), $sample_image, $styled_file);
    }
    $sample_image = '/' . variable_get('file_public_path', conf_path() . '/files') . str_replace('public:/', '', $styled_file);

    // Reset to the default preview template if it is found empty.
    $trimmed = trim($form_state['values']['FOO_preview_html']['value']);
    $form_state['values']['FOO_preview_html'] = empty($trimmed) ? variable_set('FOO_preview_html', array(
      'value' => '<div class="FOO-preview-swatch"><div class="FOO-preview-object"></div></div><div class="FOO-preview-caption">This is a caption.</div>',
      'format' => 'full_html',
    )) : $form_state['values']['FOO_preview_html'];

    // IMAGE UPLOAD.
    if (!empty($form_state['values']['FOO_imgupload'])) {
      $image = file_load($form_state['values']['FOO_imgupload']);
      $url = '/' . variable_get('file_public_path', conf_path() . '/files') . '/' . file_uri_target($image->uri);
      variable_set('FOO_imgupload_url', $url);
      // If we have an uploaded file, use the URL of it.
      $form_state['values']['FOO_imgurl'] = $url;
      $image->status = FILE_STATUS_PERMANENT;
      file_save($image);
      // Add a reference to prevent warnings.
      file_usage_add($image, 'style_settings', 'FOO_upload', $image->fid);
    }

    // IMAGE URL: Reset to default if empty. Does not work after a
    // hook_form_FORM_ID_alter(). In that case move it to the submit handler
    // after hook_settings() in the 'parent' form.
    if (trim($form_state['values']['FOO_imgurl']) == '') {
      $form_state['values']['FOO_imgurl'] = $sample_image;
      drupal_set_message(t('The image URL has been reset to the default.'), 'warning', FALSE);
    }

    // SELECTABLE MEASUREMENT UNIT: concatenate the value and unit in a new
    // variable (the one that will be used in the CSS).
    variable_set('FOO_fontsize', $form_state['values']['FOO_fontsize_value'] . $form_state['values']['FOO_fontsize_unit']);

    // Make sure changes are visible right after saving the settings.
    _drupal_flush_css_js();
    // If changes don't appear, try to uncomment also the line below.
//    cache_clear_all('style_settings_', 'cache', TRUE);
  }
}
