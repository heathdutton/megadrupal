<?php
/**
 * @file
 * Admin pages for Image Editor Inline module.
 */

/**
 * Configuration form for editing Image Editor Inline settings.
 */
function imageeditor_inline_settings_form() {
  imageeditor_admin_css();

  $form['editors'] = array(
    '#type' => 'item',
    '#title' => t('Enabled Image Editors'),
    '#description' => t('Choose enabled image editors for inline image editing and their order.'),
  );

  $position = 0;
  foreach (imageeditor_info('editor') as $codename => $editor) {
    $position++;
    $form['imageeditor_inline_' . $codename . '_enabled'] = array(
      '#type' => 'checkbox',
      '#title' =>  theme('imageeditor_admin_item', array('name' => $editor['name'], 'class' => $editor['class'], 'codename' => $codename))
        . $editor['name'] . ' (<a href="' . $editor['site'] . '" target="_blank">' . t('site') . '</a>)',
      '#default_value' => variable_get('imageeditor_inline_' . $codename . '_enabled', 0),
    );
    $form['imageeditor_inline_' . $codename . '_description'] = array(
      '#type' => 'markup',
      '#markup' => $editor['description'],
    );
    $form['imageeditor_inline_' . $codename . '_api_key'] = array(
      '#type' => 'markup',
      '#markup' => $editor['api_key'] ? l(variable_get($editor['api_key_codename']) ? t('Already set'): t('Required'), 'admin/config/media/imageeditor') : t('Not needed'),
    );
    $form['imageeditor_inline_' . $codename . '_position'] = array(
      '#type' => 'textfield',
      '#default_value' => variable_get('imageeditor_inline_' . $codename . '_position', 0) ? variable_get('imageeditor_inline_' . $codename . '_position', 0) : $position,
      '#size' => 3,
      '#maxlenth' => 4,
      '#attributes' => array('class' => array('imageeditor-position')),
    );
  }

  $form['uploaders'] = array(
    '#type' => 'item',
    '#title' => t('Enabled Upload services'),
    '#description' => t('Choose enabled upload services to upload images to if your images are not available from external network.'),
  );

  $position = 0;
  foreach (imageeditor_info('uploader') as $codename => $uploader) {
    $position++;
    $form['imageeditor_inline_' . $codename . '_enabled'] = array(
      '#type' => 'checkbox',
      '#title' =>  theme('imageeditor_admin_item', array('name' => $uploader['name'], 'class' => $uploader['class'], 'codename' => $codename))
        . $uploader['name'] . ' (<a href="' . $uploader['site'] . '" target="_blank">' . t('site') . '</a>)',
      '#default_value' => variable_get('imageeditor_inline_' . $codename . '_enabled', 0),
    );
    $form['imageeditor_inline_' . $codename . '_description'] = array(
      '#type' => 'markup',
      '#markup' => $uploader['description'],
    );
    $form['imageeditor_inline_' . $codename . '_api_key'] = array(
      '#type' => 'markup',
      '#markup' => $uploader['api_key'] ? l(variable_get($uploader['api_key_codename']) ? t('Already set'): t('Required'), 'admin/config/media/imageeditor') : t('Not needed'),
    );
    $form['imageeditor_inline_' . $codename . '_position'] = array(
      '#type' => 'textfield',
      '#default_value' => variable_get('imageeditor_inline_' . $codename . '_position', 0) ? variable_get('imageeditor_inline_' . $codename . '_position', 0) : $position,
      '#size' => 3,
      '#maxlenth' => 4,
      '#attributes' => array('class' => array('imageuploader-position')),
    );
  }

  $form['imageeditor_inline_min_dimention'] = array(
    '#type' => 'select',
    '#title' => t('Minimal image dimention'),
    '#description' => t('Minimal image height or width to activate Image Editor Inline.'),
    '#options' => array(10 => 10, 20 => 20, 30 => 30, 50 => 50, 100 => 100, 150 => 150, 200 => 200, 300 => 300, 500 => 500),
    '#default_value' => variable_get('imageeditor_inline_min_dimention', 150),
    '#weight' => 10,
  );
  $form['imageeditor_inline_ignore'] = array(
    '#type' => 'textfield',
    '#title' => t('Ignore/skip images having these CSS classes'),
    '#description' => 'jQuery selector expression to pass to the .not() method.',
    '#default_value' => variable_get('imageeditor_inline_ignore', '.admin-menu-icon, .stack-image, .imageeditor-inline-revert'),
    '#weight' => 15,
  );
  $form['imageeditor_inline_icons_position'] = array(
    '#type' => 'radios',
    '#title' => t('Icons position'),
    '#description' => t('Whether to show editing icons under the image or as a popup when you hover your mouse over the image.'),
    '#options' => array(t('Under the image'), t('Popup on the image when you hover over it')),
    '#default_value' => variable_get('imageeditor_inline_icons_position', 1),
    '#weight' => 20,
  );
  $form['imageeditor_inline_file_ownership'] = array(
    '#type' => 'radios',
    '#title' => t('Image file ownership'),
    '#description' => t('Whether to change edited image ownership to the user editing the image.'),
    '#options' => array(t('Do not change, keep the original image uploader'), t('Change to the user editing/reverting the image')),
    '#default_value' => variable_get('imageeditor_inline_file_ownership', 0),
    '#weight' => 30,
  );
  foreach (image_styles() as $key => $value) {
    $options[$key] = $key;
  }
  $form['imageeditor_inline_revert_image_style'] = array(
    '#type' => 'select',
    '#title' => t('Previews image style'),
    '#description' => t('Choose image style to use for Image Editor Inline Revert previews.'),
    '#options' => $options,
    '#default_value' => variable_get('imageeditor_inline_revert_image_style', 'medium'),
    '#weight' => 40,
  );
  $form['imageeditor_inline_access_check'] = array(
    '#type' => 'radios',
    '#title' => t('Access check'),
    '#description' => t('Access check feature allows to restrict image editing access.'),
    '#options' => array(
      t('Disable - all users with "@use_imageeditor_inline" permission will be able to edit any image', array('@use_imageeditor_inline' => t('Use Image Editor Inline'))),
      t('Enable - checks for image editing access using "@edit_all_images" and "@edit_own_images" permissions', array('@edit_all_images' => t('Edit any image on the page'), '@edit_own_images' => t('Edit only own images')))
    ),
    '#default_value' => variable_get('imageeditor_inline_access_check', 0),
    '#weight' => 50,
  );

  $form['#theme'] = 'imageeditor_inline_settings_form';
  return system_settings_form($form);
}

/**
 * Theme function for admin settings form.
 */
function theme_imageeditor_inline_settings_form($variables) {
  $form = $variables['form'];
  $output = drupal_render($form['editors']);

  $header = array(t('Name'), t('Description'), t('API key'), t('Position'));
  $rows = array();
  $imageeditors = imageeditor_info('editor');
  $imageeditors_sort = array();
  foreach ($imageeditors as $codename => $editor) {
    $imageeditors_sort[$codename] = $form['imageeditor_inline_' . $codename . '_position'];
  }
  asort($imageeditors_sort);

  foreach ($imageeditors_sort as $codename => $position) {
    $row = array();
    $row[] = drupal_render($form['imageeditor_inline_' . $codename . '_enabled']);
    $row[] = drupal_render($form['imageeditor_inline_' . $codename . '_description']);
    $row[] = drupal_render($form['imageeditor_inline_' . $codename . '_api_key']);
    $row[] = drupal_render($form['imageeditor_inline_' . $codename . '_position']);
    $rows[] = array('data' => $row, 'class' => array('draggable'));
  }
  drupal_add_tabledrag('imageeditors-table', 'order', 'sibling', 'imageeditor-position');
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'imageeditors-table')));

  $output .= drupal_render($form['uploaders']);
  $rows = array();
  $imageuploaders = imageeditor_info('uploader');
  $imageuploaders_sort = array();
  foreach ($imageuploaders as $codename => $uploader) {
    $imageuploaders_sort[$codename] = $form['imageeditor_inline_' . $codename . '_position'];
  }
  asort($imageuploaders_sort);

  foreach ($imageuploaders_sort as $codename => $position) {
    $row = array();
    $row[] = drupal_render($form['imageeditor_inline_' . $codename . '_enabled']);
    $row[] = drupal_render($form['imageeditor_inline_' . $codename . '_description']);
    $row[] = drupal_render($form['imageeditor_inline_' . $codename . '_api_key']);
    $row[] = drupal_render($form['imageeditor_inline_' . $codename . '_position']);
    $rows[] = array('data' => $row, 'class' => array('draggable'));
  }
  drupal_add_tabledrag('imageuploaders-table', 'order', 'sibling', 'imageuploader-position');
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'imageuploaders-table')));

  $output .= drupal_render_children($form);

  return $output;
}
