<?php
/**
 * @file
 * Hooks provided by the Pirobox module.
 */

/**
 * Alter the form elements for a formatter's settings.
 *
 * @param $element
 *   The associative array contains the settings form elements to alter.
 * @param $vars
 *   The associative array contains various parameters to help to alter.
 *     - entity_type: Boolean or a string contains the entity type.
 *                    E.g. FALSE, node, comment or taxonomy_term
 *     - field: The field structure being configured.
 *     - instance: The instance structure being configured.
 *     - view_mode: The view mode for which a settings summary is requested.
 *     - form: The (entire) configuration form array, which will
 *             usually have no use here.
 *     - display: The display settings to use, as found in the 'display'
 *                entry of instance definitions. The array notably contains
 *                the following keys and values:
 *                  - type: The name of the formatter to use.
 *                  - settings: The array of formatter settings.
 *     - image_styles: Avalaible styles that can be used for resizing
 *                     or adjusting images on display.
 *     - extend_styles: Array of additional options to extend the image styles.
 *
 * @ingroup hooks
 */
function hook_pirobox_field_formatter_settings_form_get_element_alter(&$element, $vars) {
  // Entity type taxonomy_term not supported.
  if ($vars['entity_type'] == FALSE || $vars['entity_type'] == 'taxonomy_term') {
    return;
  }

  $settings = $vars['display']['settings'];

  $description = t('Limiting the number of images that are displayed. If used, only users with the permission <em>View Pirobox limited galleries</em> can see full galleries.');
  $description .= '<br />' . t('Please choose the number of displayed images.');
  $element['pirobox_gallery_limit'] = array(
    '#type' => 'select',
    '#title' => t('Gallery limitation'),
    '#description' => $description,
    '#default_value' => $settings['pirobox_gallery_limit'],
    '#options' => _pirobox_limit_gallery_limit_options(),
    '#weight' => -1
  );
}

/**
 * Alter the short summary for the current formatter settings of an instance.
 *
 * @param $summary
 *   The array contains the settings HTML content to alter.
 * @param $vars
 *   The associative array contains various parameters to help to alter.
 *     - field: The field structure being configured.
 *     - instance: The instance structure being configured.
 *     - view_mode: The view mode for which a settings summary is requested.
 *     - display: The display settings to use, as found in the 'display'
 *                entry of instance definitions. The array notably contains
 *                the following keys and values:
 *                  - type: The name of the formatter to use.
 *                  - settings: The array of formatter settings.
 *     - settings: Settings from display value;
 *     - image_styles: Avalaible styles that can be used for resizing
 *                     or adjusting images on display.
 *
 * @ingroup hooks
 */
function hook_pirobox_field_formatter_settings_summary_get_summary_alter(&$summary, $vars) {
  $settings = $vars['settings'];

  if (isset($settings['pirobox_gallery_limit'])) {
    $gallery_limit_options = _pirobox_limit_gallery_limit_options();

    $summary[] = t('Pirobox gallery limitation: @type', array('@type' => $gallery_limit_options[$settings['pirobox_gallery_limit']]));
  }
  if (isset($settings['pirobox_gallery_limit_bypass'])) {
    $gallery_limit_bypass_options = _pirobox_limit_gallery_limit_bypass_options();

    $summary[] = t('Pirobox gallery limitation bypass: @type', array('@type' => $gallery_limit_bypass_options[$settings['pirobox_gallery_limit_bypass']]));
  }
}

/**
 * Alter the items for a Pirobox field.
 *
 * @param $items
 *   The associative array contains the items to alter.
 * @param $vars
 *   The associative array contains various parameters to help to alter.
 *     - entity_type: A string contains the entity type.
 *     - entity: The full entity object.
 *     - field: The field structure being configured.
 *     - instance: The instance structure being configured.
 *     - langcode:
 *     - items: Array of values for this field.
 *     - display: The display settings to use, as found in the 'display'
 *                entry of instance definitions. The array notably contains
 *                the following keys and values:
 *                  - type: The name of the formatter to use.
 *                  - settings: The array of formatter settings.
 *
 * @return
 *   The array contains the items.
 *
 * @ingroup hooks
 */
function hook_pirobox_field_formatter_view_alter(&$items, $vars) {
  global $user;

  $settings = $vars['display']['settings'];
  $entity = $vars['entity'];
  $owner_access = FALSE;

  if ($user->uid == $entity->uid && (bool) $settings['pirobox_gallery_limit_bypass']) {
    $owner_access = TRUE;
  }

  if ($settings['pirobox_gallery_limit'] == 0 || $owner_access) {
    return;
  }
  if (user_access('view pirobox limited galleries', $user)) {
    return;
  }

  switch(variable_get('pirobox_limit_extra_style', FALSE)) {
    case 0:
      if (count($items)) {
        array_splice($items, $settings['pirobox_gallery_limit']);
      }
      break;

    case 1:
      if (count($items)) {
        array_splice($items, $settings['pirobox_gallery_limit'] + variable_get('pirobox_limit_limited_images', 1));
      }
      break;
  }
}

/**
 * Alter a renderable array for a Pirobox field value.
 *
 * @param $element
 *   A renderable array for a field value.
 * @param $vars
 *   The associative array contains various parameters to help to alter.
 *     - entity_type: A string contains the entity type.
 *     - entity:
 *     - field: The field structure being configured.
 *     - instance: The instance structure being configured.
 *     - langcode:
 *     - items: Array of values for this field.
 *     - display: The display settings to use, as found in the 'display'
 *                entry of instance definitions. The array notably contains
 *                the following keys and values:
 *                  - type: The name of the formatter to use.
 *                  - settings: The array of formatter settings.
 *
 * @return
 *   The array contains the field value.
 *
 * @ingroup hooks
 */
function hook_pirobox_field_formatter_view_element_alter(&$element, $vars) {
  global $user;

  $settings = $vars['display']['settings'];
  $items = $vars['items'];
  $entity = $vars['entity'];
  $owner_access = FALSE;

  if ($user->uid == $entity->uid && (bool) $settings['pirobox_gallery_limit_bypass']) {
    $owner_access = TRUE;
  }

  if ($settings['pirobox_gallery_limit'] == 0 || $owner_access) {
    return;
  }
  if (user_access('view pirobox limited galleries', $user)) {
    return;
  }

  $count = count($items);
  $pirobox_limit_entity_style = variable_get('pirobox_limit_entity_style', 'none');
  $pirobox_limit_image_style = variable_get('pirobox_limit_image_style', 'none');

  switch (variable_get('pirobox_limit_limited_images', 1)) {
    case 1:
      if ($count == $settings['pirobox_gallery_limit'] + 1) {
        if ($pirobox_limit_entity_style != 'none') {
          // Change entity image style for the limited content image.
          $element[$count -1]['#display_settings']['pirobox_entity_style'] = $pirobox_limit_entity_style;
        }

        if ($pirobox_limit_image_style != 'none') {
          // Change image style for the limited Pirobox image.
          $element[$count -1]['#display_settings']['pirobox_image_style'] = $pirobox_limit_image_style;
          // Initiate to change caption text for the last image.
          $element[$count -1]['#image_style_extra'] = TRUE;
        }
      }
      break;
  }
}

/**
 * Alter the settings are used by field formatter info.
 *
 * @param $settings
 *   The associative array contains the formatter settings to alter.
 *
 * @return
 *   An associative array contains field formatter settings.
 *
 * @ingroup hooks
 */
function hook_pirobox_field_formatter_get_settings_alter(&$settings) {
  $settings['pirobox_gallery_limit'] = FALSE;
  // Allow full gallery view access for the content owner.
  $settings['pirobox_gallery_limit_bypass'] = FALSE;
}

/**
 * Alter a string for the Pirobox caption.
 *
 * @param $caption
 *   HTML caption text to alter.
 * @param $variables
 *   The associative array contains the formatter settings to alter.
 *     - item: An array of image data.
 *     - entity_type: The entity type. E.g. node, comment or taxonomy_term.
 *     - entity: The full entity object.
 *     - field: The array contains field values.
 *     - display_settings: The array contains formatter setting values.
 *     - cover_class: The string contains the CSS class to use
 *                    the gallery covering.
 *     - random: Boolean value to use image random.
 *     - entity_style_extra: Boolean value to use different content image style.
 *     - image_style_extra: Boolean value to use different Pirobox image style.
 *
 * @return
 *   A string can contain HTML code. The string are not sanitized.
 *
 * @ingroup hooks
 */
function hook_pirobox_formatter_caption_alter(&$caption, $variables) {
  global $user;

  $pirobox_limit_extra_caption = variable_get('pirobox_limit_extra_caption', array('value' => '', 'format' => NULL));

  if (empty($pirobox_limit_extra_caption['value'])) {
    return;
  }

  // Only a limited gallery image uses the extra caption text.
  // The Pirobox jQuery plugin JS needs the caption text
  // without p tag on begin and end.
  if ($variables['image_style_extra'] == TRUE) {
    $caption = preg_replace("/^<p>|<\/p>$/", '', $pirobox_limit_extra_caption['value']);
  }
}

/**
 * Alter form elements on Pirobox image fields on content edit forms.
 *
 * @param $element
 *   The associative array contains elements to alter.
 * @param $vars
 *   The associative array contains the form elements to alter.
 *     - form_state: The form state of the (entire) configuration form.
 *     - form: The (entire) configuration form array, which will
 *             usually have no use here.
 *     - instance: The instance structure being configured.
 *
 * @ingroup hooks
 */
function hook_pirobox_imagefield_widget_process_alter(&$element, $vars) {
  $instance = $vars['instance'];
  $displays = array();

  $gallery_limit = FALSE;
  foreach (element_children($instance['display']) as $display) {
    if (isset($instance['display'][$display]['settings']['pirobox_gallery_limit'])) {
      $gallery_limit = TRUE;
      $displays[$display] = $instance['display'][$display]['settings']['pirobox_gallery_limit'];
    }
  }

  if ($gallery_limit == TRUE && isset($element['filename'])) {
    $filename = $element['filename']['#markup'];

    $message = t('Pirobox <em>image limitation</em> is active. Not all images displayed for all visitors.');
    $message .= _pirobox_limit_get_limit_info($displays);
    $message .= t('The number of used images is counted from the first image in this list.');

    $element['filename']['#markup'] = '<div class="description pirobox-limit-description">' . $message . '</div>' . $filename;
  }
}

/**
 * Returns an URL.
 *
 * @param $uri
 *   An string containing the URI for which we need an URL.
 *
 * @return
 *   An string containing an URL.
 *
 * @see pirobox.theme.inc|theme_pirobox_formatter()
 *
 * @ingroup hooks
 */
function hook_pirobox_image_path_alter($uri) {
  global $base_url;

  // See http://drupal.org/node/1149910

  $scheme = file_uri_scheme($uri);

  if (variable_get('pirobox_relative_paths', FALSE) == TRUE && ($scheme == 'public' || $scheme == 'private')) {
    $base = $base_url . '/';
    $matches = array();

    if (preg_match('/^[\w\d]+:\/\/.*?\//', $base, $matches)) {
      $base = drupal_substr($matches[0], 0, -1);
      $uri = str_replace($base, '', file_create_url($uri));
    }
  }
}
