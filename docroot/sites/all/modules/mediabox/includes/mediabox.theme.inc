<?php

/**
 * @file
 * Mediabox entity theme.
 */

/**
 * Preprocess variables for mediabox.tpl.php
 */
function template_preprocess_mediabox(&$variables) {
  $variables['view_mode'] = $variables['elements']['#view_mode'];
  // Provide a distinct $teaser boolean.
  $variables['mediabox'] = $variables['elements']['#mediabox'];
  $mediabox = $variables['mediabox'];

  // Flatten the mediabox object's member fields.
  $variables = array_merge((array) $mediabox, $variables);

  // Helpful $content variable for templates.
  $variables += array('content' => array());
  foreach (element_children($variables['elements']) as $key) {
    $variables['content'][$key] = $variables['elements'][$key];
  }

  // Make the field variables available with the appropriate language.
  field_attach_preprocess('mediabox', $mediabox, $variables['content'], $variables);
}

/**
 * Preprocess variables for mediabox-type-detail.tpl.php
 */
function template_preprocess_mediabox_type_detail(&$variables) {
  $variables['machine_name'] = t('Machine name: %type', array('%type' => $variables['type']));
}

/**
 * Preprocess variables for mediabox-meta.tpl.php
 */
function template_preprocess_mediabox_meta(&$variables) {
  $mediabox = $variables['mediabox'];

  $properties = array(
    'title' => t('Title'),
    'alternate_text' => t('Description'),
    'caption' => t('Caption'),
    'dimensions' => t('Dimensions'),
    'description' => t('Description'),
  );

  $metas = array();
  foreach ($properties as $key => $property) {
    $metas[$key] = array(
      'label' => $property,
      'value' => !empty($mediabox->{$key}) ? $mediabox->{$key} : '',
    );
  }

  $variables['metas'] = $metas;
}
