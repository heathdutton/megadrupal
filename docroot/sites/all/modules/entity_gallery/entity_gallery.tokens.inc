<?php

/**
 * @file
 * Builds placeholder replacement tokens for entity-gallery-related data.
 */



/**
 * Implements hook_token_info().
 */
function entity_gallery_token_info() {
  $type = array(
    'name' => t('Entity galleries'),
    'description' => t('Tokens related to individual gallery items, or "entity galleries".'),
    'needs-data' => 'entity_gallery',
  );

  // Core tokens for entity galleries.
  $entity_gallery['egid'] = array(
    'name' => t("Entity gallery ID"),
    'description' => t('The unique ID of the gallery item, or "entity gallery".'),
  );
  $entity_gallery['vid'] = array(
    'name' => t("Revision ID"),
    'description' => t("The unique ID of the entity gallery's latest revision."),
  );
  $entity_gallery['tegid'] = array(
    'name' => t("Translation set ID"),
    'description' => t("The unique ID of the original-language version of this entity gallery, if one exists."),
  );
  $entity_gallery['type'] = array(
    'name' => t("Gallery type"),
    'description' => t("The type of the entity gallery."),
  );
  $entity_gallery['type-name'] = array(
    'name' => t("Gallery type name"),
    'description' => t("The human-readable name of the entity gallery type."),
  );
  $entity_gallery['title'] = array(
    'name' => t("Title"),
    'description' => t("The title of the entity gallery."),
  );
  $entity_gallery['description'] = array(
    'name' => t("Description"),
    'description' => t("The main description text of the entity gallery."),
  );
  $entity_gallery['summary'] = array(
    'name' => t("Summary"),
    'description' => t("The summary of the entity gallery's main description text."),
  );
  $entity_gallery['language'] = array(
    'name' => t("Language"),
    'description' => t("The language the entity gallery is written in."),
  );
  $entity_gallery['url'] = array(
    'name' => t("URL"),
    'description' => t("The URL of the entity gallery."),
  );
  $entity_gallery['edit-url'] = array(
    'name' => t("Edit URL"),
    'description' => t("The URL of the entity gallery's edit page."),
  );

  // Chained tokens for entity galleries.
  $entity_gallery['created'] = array(
    'name' => t("Date created"),
    'description' => t("The date the entity gallery was posted."),
    'type' => 'date',
  );
  $entity_gallery['changed'] = array(
    'name' => t("Date changed"),
    'description' => t("The date the entity gallery was most recently updated."),
    'type' => 'date',
  );
  $entity_gallery['author'] = array(
    'name' => t("Author"),
    'description' => t("The author of the entity gallery."),
    'type' => 'user',
  );

  return array(
    'types' => array('entity_gallery' => $type),
    'tokens' => array('entity_gallery' => $entity_gallery),
  );
}

/**
 * Implements hook_tokens().
 */
function entity_gallery_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $url_options = array('absolute' => TRUE);
  if (isset($options['language'])) {
    $url_options['language'] = $options['language'];
    $language_code = $options['language']->language;
  }
  else {
    $language_code = NULL;
  }
  $sanitize = !empty($options['sanitize']);

  $replacements = array();

  if ($type == 'entity_gallery' && !empty($data['entity_gallery'])) {
    $entity_gallery = $data['entity_gallery'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        // Simple key values on the entity gallery.
        case 'egid':
          $replacements[$original] = $entity_gallery->egid;
          break;

        case 'vid':
          $replacements[$original] = $entity_gallery->vid;
          break;

        case 'tegid':
          $replacements[$original] = $entity_gallery->tegid;
          break;

        case 'type':
          $replacements[$original] = $sanitize ? check_plain($entity_gallery->type) : $entity_gallery->type;
          break;

        case 'type-name':
          $type_name = entity_gallery_type_get_name($entity_gallery);
          $replacements[$original] = $sanitize ? check_plain($type_name) : $type_name;
          break;

        case 'title':
          $replacements[$original] = $sanitize ? check_plain($entity_gallery->title) : $entity_gallery->title;
          break;

        case 'description':
        case 'summary':
          if ($items = field_get_items('entity_gallery', $entity_gallery, 'entity_gallery_description', $language_code)) {
            $instance = field_info_instance('entity_gallery', 'entity_gallery_description', $entity_gallery->type);
            $field_langcode = field_language('entity_gallery', $entity_gallery, 'entity_gallery_description', $language_code);
            // If the summary was requested and is not empty, use it.
            if ($name == 'summary' && !empty($items[0]['summary'])) {
              $output = $sanitize ? _text_sanitize($instance, $field_langcode, $items[0], 'summary') : $items[0]['summary'];
            }
            // Attempt to provide a suitable version of the 'description' field.
            else {
              $output = $sanitize ? _text_sanitize($instance, $field_langcode, $items[0], 'value') : $items[0]['value'];
              // A summary was requested.
              if ($name == 'summary') {
                if (isset($instance['display']['teaser']['settings']['trim_length'])) {
                  $trim_length = $instance['display']['teaser']['settings']['trim_length'];
                }
                else {
                  // Use default value.
                  $trim_length = NULL;
                }
                // Generate an optionally trimmed summary of the description field.
                $output = text_summary($output, $instance['settings']['text_processing'] ? $items[0]['format'] : NULL, $trim_length);
              }
            }
            $replacements[$original] = $output;
          }
          break;

        case 'language':
          $langcode = entity_language('entity_gallery', $entity_gallery);
          $replacements[$original] = $sanitize ? check_plain($langcode) : $langcode;
          break;

        case 'url':
          $replacements[$original] = url('gallery/' . $entity_gallery->egid, $url_options);
          break;

        case 'edit-url':
          $replacements[$original] = url('gallery/' . $entity_gallery->egid . '/edit', $url_options);
          break;

        // Default values for the chained tokens handled below.
        case 'author':
          $account = user_load($entity_gallery->uid);
          $name = format_username($account);
          $replacements[$original] = $sanitize ? check_plain($name) : $name;
          break;

        case 'created':
          $replacements[$original] = format_date($entity_gallery->created, 'medium', '', NULL, $language_code);
          break;

        case 'changed':
          $replacements[$original] = format_date($entity_gallery->changed, 'medium', '', NULL, $language_code);
          break;
      }
    }

    if ($author_tokens = token_find_with_prefix($tokens, 'author')) {
      $author = user_load($entity_gallery->uid);
      $replacements += token_generate('user', $author_tokens, array('user' => $author), $options);
    }

    if ($created_tokens = token_find_with_prefix($tokens, 'created')) {
      $replacements += token_generate('date', $created_tokens, array('date' => $entity_gallery->created), $options);
    }

    if ($changed_tokens = token_find_with_prefix($tokens, 'changed')) {
      $replacements += token_generate('date', $changed_tokens, array('date' => $entity_gallery->changed), $options);
    }
  }

  return $replacements;
}
