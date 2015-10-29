<?php
/**
 * @file
 * Field template functions.
 */

/**
 * Implements hook_preprocess_field().
 */
function center_preprocess_field(&$vars) {
  // Set shortcut variables. Hooray for less typing!
  $name = $vars['element']['#field_name'];
  $bundle = $vars['element']['#bundle'];
  $mode = $vars['element']['#view_mode'];

  // Create a development print variable.
  $vars['dev_deets'] = theme_get_setting('dev_mode') ?
    '<!-- Name: ' . $name . ' | Bundle: ' . $bundle . ' | View Mode: ' . $mode . ' -->' : '';
}

/**
 * Overrides theme_field().
 *
 * Remove the hard coded classes so we can add them in preprocess functions.
 */
function center_field($variables) {
  $output = $variables['dev_deets'];

  // If we're nuking fields, return the rendered fields with no markup.
  if (theme_get_setting('nuke_fields')) {
    foreach ($variables['items'] as $delta => $item) {
      $output .= drupal_render($item);
    }
    return $output;
  }

  // Render the label, if it's not hidden.
  if (!$variables['label_hidden']) {
    $output .= '<div ' . $variables['title_attributes'] . '>' . $variables['label'] . ':&nbsp;</div>';
  }

  // Render the items.
  $output .= '<div ' . $variables['content_attributes'] . '>';
  foreach ($variables['items'] as $delta => $item) {
    $output .= '<div ' . $variables['item_attributes'][$delta] . '>' . drupal_render($item) . '</div>';
  }
  $output .= '</div>';

  // Render the top-level DIV.
  $output = '<div class="' . $variables['classes'] . '"' . $variables['attributes'] . '>' . $output . '</div>';

  return $output;
}

/**
 * Overrides theme_ds_field_minimal().
 *
 * Still allow the normal drupal field classes to apply.
 */
function center_field__minimal($variables) {
  $output = '';
  $config = $variables['ds-config'];

  // Add a simple wrapper.
  $output .= '<div class="' . $variables['classes'] . '">';

  // Render the label.
  if (!$variables['label_hidden']) {
    $output .= '<div' . $variables['title_attributes'] . '>' . $variables['label'];
    if (!isset($config['lb-col'])) {
      $output .= ':&nbsp;';
    }
    $output .= '</div>';
  }

  // Render the items.
  foreach ($variables['items'] as $delta => $item) {
    $output .= drupal_render($item);
  }
  $output .= "</div>";

  return $output;
}

/**
 * Overrides theme_ds_field_minimal().
 *
 * Still allow the normal drupal field classes to apply.
 */
function center_field__raw($variables) {
  $output = '';

  // Render the items.
  foreach ($variables['items'] as $delta => $item) {
    $output .= drupal_render($item);
  }

  return $output;
}

/**
 * Custom implementation of theme_field().
 *
 * Turns multivalued fields into a comma separated list.
 * USAGE: $vars['theme_hook_suggestions'][] = 'field__custom_separated';
 */
function center_field__custom_separated($variables) {
  $output = '';

  // Render the label, if it's not hidden.
  if (!$variables['label_hidden']) {
    $output .= '<label ' . $variables['title_attributes'] . '>' . $variables['label'] . ':&nbsp;</label>';
  }

  // Render the items.
  $count = 1;
  foreach ($variables['items'] as $delta => $item) {
    $output .= '<span ' . $variables['item_attributes'][$delta] . '>' . drupal_render($item) . '</span>';
    if ($count < count($variables['items'])) {
      $output .= ', ';
    }
    $count++;
  }

  // Render the top-level DIV.
  $output = '<div class="' . $variables['classes'] . '"' . $variables['attributes'] . '>' . $output . '</div>';

  return $output;
}

/**
 * Custom implementation of theme_field().
 *
 * Takes a url and renders it as a download link.
 * USAGE: $vars['theme_hook_suggestions'][] = 'field__custom_download';
 */
function center_field__custom_download($variables) {
  $output = '';

  // Render the label, if it's not hidden.
  if (!$variables['label_hidden']) {
    $output .= '<label ' . $variables['title_attributes'] . '>' . $variables['label'] . ':&nbsp;</label>';
  }

  // Render the items.
  $count = 1;
  foreach ($variables['element']['#items'] as $delta => $item) {
    $output .= l(
      t('Download'),
      file_create_url($item['uri']),
      array(
        'attributes' => array(
          'class' => array('link--download'),
        ),
      )
    );
  }

  // Render the top-level DIV.
  $output = '<div class="' . $variables['classes'] . '"' . $variables['attributes'] . '>' . $output . '</div>';

  return $output;
}

/**
 * Custom implementation of theme_field().
 *
 * Wraps the field in an h3 and nukes the label an outer wrapper.
 * USAGE: $vars['theme_hook_suggestions'][] = 'field__custom_h2';
 */
function center_field__custom_h2($variables) {
  $output = '';
  foreach ($variables['items'] as $delta => $item) {
    $output .= '<h2 ' . $variables['item_attributes'][$delta] . '>' . drupal_render($item) . '</h2>';
  }

  return $output;
}

/**
 * Custom implementation of theme_field().
 *
 * Wraps the field in an h3 and nukes the label an outer wrapper.
 * USAGE: $vars['theme_hook_suggestions'][] = 'field__custom_h3';
 */
function center_field__custom_h3($variables) {
  $output = '';
  foreach ($variables['items'] as $delta => $item) {
    $output .= '<h3 ' . $variables['item_attributes'][$delta] . '>' . drupal_render($item) . '</h3>';
  }

  return $output;
}
