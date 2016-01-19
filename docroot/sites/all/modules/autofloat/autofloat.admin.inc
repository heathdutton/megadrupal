<?php
/**
 * @file
 * The admin settings for the AutoFloat module.
 */

/**
 * Implements hook_settings().
 */
function autofloat_admin_settings() {
  if (module_exists('image')) {
    // Make sure resized default images exist.
    $sample_image_medium = variable_get('image_style_preview_image', drupal_get_path('module', 'image') . '/sample.png');
    $styled_file = image_style_path('medium', $sample_image_medium);
    if (!file_exists($styled_file)) {
      image_style_create_derivative(image_style_load('medium'), $sample_image_medium, $styled_file);
    }
    $sample_image_medium = '/' . variable_get('file_public_path', conf_path() . '/files') . str_replace('public:/', '', $styled_file);
    $sample_image_thumb = variable_get('image_style_preview_image', drupal_get_path('module', 'image') . '/sample.png');
    $styled_file = image_style_path('thumbnail', $sample_image_thumb);
    if (!file_exists($styled_file)) {
      image_style_create_derivative(image_style_load('thumbnail'), $sample_image_thumb, $styled_file);
    }
    $sample_image_thumb = '/' . variable_get('file_public_path', conf_path() . '/files') . str_replace('public:/', '', $styled_file);
    // A preview area.
    $form['autofloat_preview'] = array(
      '#type' => 'fieldset',
      '#title' => t('Preview'),
      '#collapsible' => TRUE,
      '#collapsed' => variable_get('autofloat_preview_collapsed', TRUE),
    );
    // Form submit resulted in an uncollapsed preview. Set it back.
    variable_set('autofloat_preview_collapsed', TRUE);
    $markup = variable_get('autofloat_preview_html', array(
      'value' => '<h3>Justified.</h3><p style="text-align: justify;"><img src="' . $sample_image_medium . '" />Lectus purus ut urna! Et velit et vel sociis mid lundium habitasse, porttitor habitasse et, proin! Et vel porttitor, parturient sagittis lorem ac. Lorem! Nisi ultricies cum diam lundium turpis enim platea. Enim proin etiam platea non a diam integer platea scelerisque? Pulvinar et dis? Sit nunc purus, tortor montes egestas. Cras magna eu? Integer dignissim. Velit turpis pellentesque arcu integer cursus aenean quis natoque! Turpis sed dolor platea, tincidunt.</p><h3>Non-justified. Image within paragraph.</h3><p>Augue lectus magna? Porttitor amet pulvinar dapibus natoque pulvinar. Tristique nec, placerat elementum pulvinar amet. Tincidunt non velit diam, placerat mus, vut pid proin hac lacus lacus cursus penatibus nisi rhoncus! <img src="' . $sample_image_thumb . '" />Tristique amet natoque mattis augue velit tortor in, cum aliquam integer proin porta habitasse vel nisi, non non duis facilisis! Integer ultricies non magna eros lacus nisi etiam! Sagittis! Ac ut quis. Tristique. Lacus aliquam mus, pulvinar dignissim. Integer, habitasse urna cursus augue, odio enim mauris porta integer risus placerat dolor, et! In, integer nunc sociis ridiculus ac egestas. Purus pulvinar, sed pulvinar, integer purus! Phasellus, enim mauris montes, mauris montes massa pellentesque vut et ut dictumst turpis natoque cum cras cras? Nunc duis aliquam enim sit egestas elementum cursus? Nisi nisi tristique turpis. Mauris placerat pid, rhoncus, tristique, nisi, placerat penatibus adipiscing elementum, placerat rhoncus est ac. Scelerisque, eu! In mauris mauris ut turpis? Porttitor nec auctor! Magna elementum porttitor. Nunc ridiculus vel nunc augue natoque placerat phasellus dignissim, scelerisque turpis.</p>',
      'format' => 'full_html',
    ));
    // Reset to the default preview template if it is found empty.
    $trimmed = trim($markup['value']);
    $markup = empty($trimmed) ? variable_set('autofloat_preview_html', array(
      'value' => '<h3>Justified.</h3><p style="text-align: justify;"><img src="' . $sample_image_medium . '" />Lectus purus ut urna! Et velit et vel sociis mid lundium habitasse, porttitor habitasse et, proin! Et vel porttitor, parturient sagittis lorem ac. Lorem! Nisi ultricies cum diam lundium turpis enim platea. Enim proin etiam platea non a diam integer platea scelerisque? Pulvinar et dis? Sit nunc purus, tortor montes egestas. Cras magna eu? Integer dignissim. Velit turpis pellentesque arcu integer cursus aenean quis natoque! Turpis sed dolor platea, tincidunt.</p><h3>Non-justified. Image within paragraph.</h3><p>Augue lectus magna? Porttitor amet pulvinar dapibus natoque pulvinar. Tristique nec, placerat elementum pulvinar amet. Tincidunt non velit diam, placerat mus, vut pid proin hac lacus lacus cursus penatibus nisi rhoncus! <img src="' . $sample_image_thumb . '" />Tristique amet natoque mattis augue velit tortor in, cum aliquam integer proin porta habitasse vel nisi, non non duis facilisis! Integer ultricies non magna eros lacus nisi etiam! Sagittis! Ac ut quis. Tristique. Lacus aliquam mus, pulvinar dignissim. Integer, habitasse urna cursus augue, odio enim mauris porta integer risus placerat dolor, et! In, integer nunc sociis ridiculus ac egestas. Purus pulvinar, sed pulvinar, integer purus! Phasellus, enim mauris montes, mauris montes massa pellentesque vut et ut dictumst turpis natoque cum cras cras? Nunc duis aliquam enim sit egestas elementum cursus? Nisi nisi tristique turpis. Mauris placerat pid, rhoncus, tristique, nisi, placerat penatibus adipiscing elementum, placerat rhoncus est ac. Scelerisque, eu! In mauris mauris ut turpis? Porttitor nec auctor! Magna elementum porttitor. Nunc ridiculus vel nunc augue natoque placerat phasellus dignissim, scelerisque turpis.</p>',
      'format' => 'full_html',
    )) : $markup;
    // Use the current user's default format if the stored one isn't available.
    $format_id = filter_format_load($markup['format']) ? $markup['format'] : filter_default_format();
    $form['autofloat_preview']['autofloat_preview_markup'] = array(
      '#markup' => '<div class="style-settings-preview">' . check_markup($markup['value'], $format_id, '', $cache = FALSE) . '</div>',
    );
    $form['autofloat_preview']['autofloat_preview_details'] = array(
      '#type' => 'fieldset',
      '#title' => t('Template'),
      '#description' => t('Wrapped in <code>&lt;div class="style-settings-preview"> .. &lt;/div></code>.'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['autofloat_preview']['autofloat_preview_details']['autofloat_preview_html'] = array(
      '#type' => 'text_format',
      '#description' => t('Changes are visible after form submit. Empty the text area to reset to default.'),
      '#default_value' => $markup['value'],
      '#format' => $format_id,
      '#access' => 'use text format ' . $format_id,
    );
  }
  else {
    $image_module = l(t('Image module'), 'admin/modules', array(
      'attributes' => array(
        'title' => t('Module list'),
      ),
    ));
    // A preview area.
    $form['autofloat_preview'] = array(
      '#type' => 'fieldset',
      '#title' => t('Preview'),
      '#description' => t("Enable the Drupal core !image_module to get a preview area exposed here.", array('!image_module' => $image_module)),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
  }
  $form['autofloat_start'] = array(
    '#type' => 'radios',
    '#title' => t('Start with the first image on the..'),
    '#options' => array(
      0 => t('right'),
      1 => t('left (swaps "odd" and "even" classes)'),
    ),
    '#default_value' => variable_get('autofloat_start', 0),
    '#description' => t('Re-save existing content to apply changes.'),
  );
  $form['autofloat_css'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use autofloat.css'),
    '#default_value' => variable_get('autofloat_css', 1),
    '#description' => t('Uncheck to take care of the floating and margins yourself in custom css.'),
  );
  $form['target_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Selector / rejector settings'),
    '#description' => t('Images will float unless they have the class "nofloat". Re-save existing content to apply changes. Avoid adding classes manually by defining classes here added by other modules/filters. Use your browser inspector to find them.'),
  );
  $form['target_settings']['autofloat_span'] = array(
    '#type' => 'textfield',
    '#title' => t('Additional span classes to float'),
    '#default_value' => variable_get('autofloat_span', 'flickr-wrap'),
    '#description' => t('A "span" with the class "float" will float all containing content, e.g. the image with a caption under it. Optionally define others. Maximum two, divided by a comma. Example: "flickr-wrap".'),
  );
  $form['target_settings']['autofloat_div'] = array(
    '#type' => 'textfield',
    '#title' => t('Additional "div" classes to ignore'),
    '#default_value' => variable_get('autofloat_div', 'flickr-photoset'),
    '#description' => t('Images nested within any element with the class "nofloat" will NOT float, e.g. a set of thumbnails. Optionally define others. Maximum two, divided by a comma. Example: "flickr-photoset".'),
  );

  $style_settings_module = l(t('Style (CSS) Settings module'), 'https://drupal.org/project/style_settings', array(
      'attributes' => array(
        'title' => t('Style (CSS) Settings | Drupal.org'),
        'target' => '_blank',
      ),
  ));
  // Put CSS variables together in a fieldset.
  $form['css_variables'] = array(
    '#type' => 'fieldset',
    '#title' => t('CSS variables'),
    '#description' => t('The margins between an auto floated image and the surrounding text.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  if (module_exists('style_settings')) {
    $form['css_variables']['#collapsed'] = TRUE;
    // A SELECTABLE MEASUREMENT UNIT (px, em). It goes together with a submit
    // handler inside the 'function autofloat_admin_settings_submit()'.
    $form['css_variables']['autofloat_left'] = array(
      '#type' => 'fieldset',
      '#title' => t('Left margin for right floating images'),
      // Make containing fields align horizontally.
      '#attributes' => array('class' => array('container-inline')),
      // Number field input help (min, max, step) will be appended.
    );
    // Number field without a '#field_suffix'.
    $form['css_variables']['autofloat_left']['autofloat_left_value'] = array(
      '#type' => 'style_settings_number',
      '#default_value' => variable_get('autofloat_left_value', '5'),
    );
    // A measurement unit select field.
    $form['css_variables']['autofloat_left']['autofloat_left_unit'] = array(
      '#type' => 'select',
      '#options' => array(
        'px' => t('px'),
        'em' => t('em'),
      ),
      '#default_value' => variable_get('autofloat_left_unit', 'px'),
      '#required' => TRUE,
    );
    $form['css_variables']['autofloat_right'] = array(
      '#type' => 'fieldset',
      '#title' => t('Right margin for left floating images'),
      // Make containing fields align horizontally.
      '#attributes' => array('class' => array('container-inline')),
      // Number field input help (min, max, step) will be appended.
    );
    // Number field without a '#field_suffix'.
    $form['css_variables']['autofloat_right']['autofloat_right_value'] = array(
      '#type' => 'style_settings_number',
      '#default_value' => variable_get('autofloat_right_value', '5'),
    );
    // A measurement unit select field.
    $form['css_variables']['autofloat_right']['autofloat_right_unit'] = array(
      '#type' => 'select',
      '#options' => array(
        'px' => t('px'),
        'em' => t('em'),
      ),
      '#default_value' => variable_get('autofloat_right_unit', 'px'),
      '#required' => TRUE,
    );
    $form['css_variables']['autofloat_top'] = array(
      '#type' => 'fieldset',
      '#title' => t('Top margin for floating images'),
      // Make containing fields align horizontally.
      '#attributes' => array('class' => array('container-inline')),
      // Number field input help (min, max, step) will be appended.
    );
    // Number field without a '#field_suffix'.
    $form['css_variables']['autofloat_top']['autofloat_top_value'] = array(
      '#type' => 'style_settings_number',
      '#default_value' => variable_get('autofloat_top_value', '2'),
    );
    // A measurement unit select field.
    $form['css_variables']['autofloat_top']['autofloat_top_unit'] = array(
      '#type' => 'select',
      '#options' => array(
        'px' => t('px'),
        'em' => t('em'),
      ),
      '#default_value' => variable_get('autofloat_top_unit', 'px'),
      '#required' => TRUE,
    );
    $form['css_variables']['autofloat_bottom'] = array(
      '#type' => 'fieldset',
      '#title' => t('Bottom margin for floating images'),
      // Make containing fields align horizontally.
      '#attributes' => array('class' => array('container-inline')),
      // Number field input help (min, max, step) will be appended.
    );
    // Number field without a '#field_suffix'.
    $form['css_variables']['autofloat_bottom']['autofloat_bottom_value'] = array(
      '#type' => 'style_settings_number',
      '#default_value' => variable_get('autofloat_bottom_value', '2'),
    );
    // A measurement unit select field.
    $form['css_variables']['autofloat_bottom']['autofloat_bottom_unit'] = array(
      '#type' => 'select',
      '#options' => array(
        'px' => t('px'),
        'em' => t('em'),
      ),
      '#default_value' => variable_get('autofloat_bottom_unit', 'px'),
      '#required' => TRUE,
    );
  }

  // If the Style Settings module is not enabled, provide some instructions.
  else {
    $form['css_variables']['#description'] = NULL;
    $form['css_variables']['autofloat_note'] = array(
      '#markup' => t("Enable the !style_settings_module to get style options exposed here. They consist of:<ul>
          <li>A left margin for right floating images.</li>
          <li>A right margin for left floating images.</li>
          <li>A top and bottom margin for all auto floated images.</li>
        </ul>", array('!style_settings_module' => $style_settings_module)),
    );
  }

  // Call submit_function() on form submission.
  $form['#submit'][] = 'autofloat_admin_settings_submit';

  return system_settings_form($form);
}

/**
 * Validate autofloat settings submission.
 */
function autofloat_admin_settings_validate($form, &$form_state) {
  // Accept maximum two class value for the selector field.
  $limit = $form_state['values']['autofloat_span'];
  if ((substr_count($limit, ',') > 1)) {
    form_set_error('autofloat_span', t('Not more than two values.'));
  }
  // Accept maximum two class value for the rejector field.
  $limit = $form_state['values']['autofloat_div'];
  if ((substr_count($limit, ',') > 1)) {
    form_set_error('autofloat_div', t('Not more than two values.'));
  }
}

/**
 * Submit form data.
 */
function autofloat_admin_settings_submit($form, &$form_state) {
  if (module_exists('image')) {
    // Uncollapse the preview. Likely we want to see the changes we just made.
    variable_set('autofloat_preview_collapsed', FALSE);
    // Make sure resized default images exist.
    $sample_image_medium = variable_get('image_style_preview_image', drupal_get_path('module', 'image') . '/sample.png');
    $styled_file = image_style_path('medium', $sample_image_medium);
    if (!file_exists($styled_file)) {
      image_style_create_derivative(image_style_load('medium'), $sample_image_medium, $styled_file);
    }
    $sample_image_medium = '/' . variable_get('file_public_path', conf_path() . '/files') . str_replace('public:/', '', $styled_file);
    $sample_image_thumb = variable_get('image_style_preview_image', drupal_get_path('module', 'image') . '/sample.png');
    $styled_file = image_style_path('thumbnail', $sample_image_thumb);
    if (!file_exists($styled_file)) {
      image_style_create_derivative(image_style_load('thumbnail'), $sample_image_thumb, $styled_file);
    }
    $sample_image_thumb = '/' . variable_get('file_public_path', conf_path() . '/files') . str_replace('public:/', '', $styled_file);
    // Reset to the default preview template if it is found empty.
    $trimmed = trim($form_state['values']['autofloat_preview_html']['value']);
    $form_state['values']['autofloat_preview_html'] = empty($trimmed) ? variable_set('autofloat_preview_html', array(
      'value' => '<h3>Justified.</h3><p style="text-align: justify;"><img src="' . $sample_image_medium . '" />Lectus purus ut urna! Et velit et vel sociis mid lundium habitasse, porttitor habitasse et, proin! Et vel porttitor, parturient sagittis lorem ac. Lorem! Nisi ultricies cum diam lundium turpis enim platea. Enim proin etiam platea non a diam integer platea scelerisque? Pulvinar et dis? Sit nunc purus, tortor montes egestas. Cras magna eu? Integer dignissim. Velit turpis pellentesque arcu integer cursus aenean quis natoque! Turpis sed dolor platea, tincidunt.</p><h3>Non-justified. Image within paragraph.</h3><p>Augue lectus magna? Porttitor amet pulvinar dapibus natoque pulvinar. Tristique nec, placerat elementum pulvinar amet. Tincidunt non velit diam, placerat mus, vut pid proin hac lacus lacus cursus penatibus nisi rhoncus! <img src="' . $sample_image_thumb . '" />Tristique amet natoque mattis augue velit tortor in, cum aliquam integer proin porta habitasse vel nisi, non non duis facilisis! Integer ultricies non magna eros lacus nisi etiam! Sagittis! Ac ut quis. Tristique. Lacus aliquam mus, pulvinar dignissim. Integer, habitasse urna cursus augue, odio enim mauris porta integer risus placerat dolor, et! In, integer nunc sociis ridiculus ac egestas. Purus pulvinar, sed pulvinar, integer purus! Phasellus, enim mauris montes, mauris montes massa pellentesque vut et ut dictumst turpis natoque cum cras cras? Nunc duis aliquam enim sit egestas elementum cursus? Nisi nisi tristique turpis. Mauris placerat pid, rhoncus, tristique, nisi, placerat penatibus adipiscing elementum, placerat rhoncus est ac. Scelerisque, eu! In mauris mauris ut turpis? Porttitor nec auctor! Magna elementum porttitor. Nunc ridiculus vel nunc augue natoque placerat phasellus dignissim, scelerisque turpis.</p>',
      'format' => 'full_html',
    )) : $form_state['values']['autofloat_preview_html'];
  }
  if (module_exists('style_settings')) {

    // SELECTABLE MEASUREMENT UNIT: concatenate the value and unit in a new
    // variable (the one that will be used in the CSS).
    variable_set('autofloat_left', $form_state['values']['autofloat_left_value'] . $form_state['values']['autofloat_left_unit']);
    variable_set('autofloat_right', $form_state['values']['autofloat_right_value'] . $form_state['values']['autofloat_right_unit']);
    variable_set('autofloat_top', $form_state['values']['autofloat_top_value'] . $form_state['values']['autofloat_top_unit']);
    variable_set('autofloat_bottom', $form_state['values']['autofloat_bottom_value'] . $form_state['values']['autofloat_bottom_unit']);

    // Make sure changes are visible right after saving the settings.
    _drupal_flush_css_js();
  }
}
