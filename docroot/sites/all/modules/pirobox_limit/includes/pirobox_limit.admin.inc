<?php
/**
 * @file
 * Administrative page callbacks for the Pirobox Limit module.
 */

function pirobox_limit_general_settings_form() {
  switch (variable_get('pirobox_module_compression_type', 'min')) {
    case 'min':
      drupal_add_js(drupal_get_path('module', 'pirobox_limit') . '/js/pirobox_limit.admin.min.js', array('scope' => 'footer'));
      break;
    case 'none':
      drupal_add_js(drupal_get_path('module', 'pirobox_limit') . '/js/pirobox_limit.admin.js', array('scope' => 'footer'));
      break;
  }

  $form = array();
  $image_styles = pirobox_image_style_options(FALSE);
  $pirobox_limit_extra_caption = variable_get('pirobox_limit_extra_caption', array('value' => '', 'format' => NULL));

  $form['pirobox_limit_custom_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Styles and options'),
    '#description' => '',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE
  );
  $form['pirobox_limit_custom_settings']['pirobox_limit_extra_style'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use different image styles'),
    '#description' => '<div id="edit-pirobox-limit-extra-style-description">' . t('The use of different image styles allows the use of Pirobox Limit in the extended variant. If this option is enabled, it is possible for all limited galleries to extend the configured limit and access options.') . '</div>',
    '#default_value' => variable_get('pirobox_limit_extra_style', FALSE)
  );

  // Number of limited images.
  $form['pirobox_limit_custom_settings']['pirobox_limit_limited_images'] = array(
    '#type' => 'select',
    '#title' => t('Limited images'),
    '#description' => t('How much limited images to be displayed.'),
    '#default_value' => variable_get('pirobox_limit_limited_images', 1),
    '#options' => drupal_map_assoc(array(1, 2, 3)),
    '#states' => array(
      'visible' => array(
        ':input[name="pirobox_limit_extra_style"]' => array('checked' => TRUE)
      )
    )
  );

  // Content image different style.
  $form['pirobox_limit_custom_settings']['pirobox_limit_entity_style'] = array(
    '#type' => 'select',
    '#title' => t('Content image style'),
    '#description' => t('Select which different image style to use for displaying limited images in the content  - if gallery covering not used.'),
    '#default_value' => variable_get('pirobox_limit_entity_style', FALSE),
    '#empty_option' => t('None (original image)'),
    '#options' => array_merge(array('none' => t('No additional style')), $image_styles),
    '#states' => array(
      'visible' => array(
        ':input[name="pirobox_limit_extra_style"]' => array('checked' => TRUE)
      )
    )
  );

  // Pirobox image different style.
  $form['pirobox_limit_custom_settings']['pirobox_limit_image_style'] = array(
    '#type' => 'select',
    '#title' => t('Pirobox image style'),
    '#description' => t('Select which different image style to use for displaying limited images in Pirobox galleries.'),
    '#default_value' => variable_get('pirobox_limit_image_style', FALSE),
    '#empty_option' => t('None (original image)'),
    '#options' => $image_styles,
    '#states' => array(
      'visible' => array(
        ':input[name="pirobox_limit_extra_style"]' => array('checked' => TRUE)
      )
    )
  );

  // Pirobox different caption.
  $description['pirobox_limit_extra_caption'] = '<strong>' . t('Usage different caption') . '</strong>';
  $description['pirobox_limit_extra_caption'] .= '<br />' . t('Enter a Pirobox caption text which are used for the limited images. As example, this text can informs the page visitors to obtain access to see full galleries.');
  $description['pirobox_limit_extra_caption'] .= '<br />' . t('Leave blank if you do not wish to use a different caption.');
  $form['pirobox_limit_custom_settings']['pirobox_limit_extra_caption'] = array(
    '#type' => 'text_format',
    '#title' => t('Different caption'),
    '#description' => $description['pirobox_limit_extra_caption'],
    '#default_value' => $pirobox_limit_extra_caption['value'],
    '#format' => isset($pirobox_limit_extra_caption['format']) ? $pirobox_limit_extra_caption['format'] : NULL,
    '#states' => array(
      'visible' => array(
        ':input[name="pirobox_limit_extra_style"]' => array('checked' => TRUE)
      )
    )
  );

  return system_settings_form($form);
}
