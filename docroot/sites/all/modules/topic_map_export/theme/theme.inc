<?php
/**
 * @file
 * Theme preprocessor functions are declared here.
 */


/**
 * Process variables for topic.tpl.php.
 *
 * @param array $vars
 *   An associative array containing:
 *   - topic: Topic datastore object.
 *   - si: Subject identifier string of the topic itself.
 *   - topic_id: XML compatible string identifier for the topic.
 *   - name: Readable name of the topic.
 *   - instance_of: An array of topic type strings, that the topic is instance
 *   of.
 *   - occurences: An array of rendered occurences of the topic.
 *   - variants: An array of rendered variant names of the topic.
 *
 * @see topic.tpl.php
 * @see theme_topic_map_topic()
 *
 * @ingroup themeable
 */
function template_preprocess_topic_map_topic_entity(&$vars) {

  $topic = $vars['topic'];

  $info = entity_get_info($topic->type);
  $entity = entity_load($topic->type, array($topic->entity_id));
  $entity = reset($entity);

  $bundle = '';
  if (empty($info['entity keys']['bundle'])) {
    $bundle = $topic->type;
  }
  else {
    $bundle_key = $info['entity keys']['bundle'];
    $bundle = $entity->$bundle_key;
  }

  $fields = field_info_instances($topic->type, $bundle);

  $occurences = array();
  $default_lang = language_default();
  $lang_scope = $default_lang->language;
  if (isset($entity->language) && $entity->language != LANGUAGE_NONE) {
    $lang_scope = $entity->language;
  }

  // Create occurences out of entity fields.
  foreach ($fields as $field_key => $value) {

    $field_instance = field_info_instance($topic->type, $field_key, $bundle);
    $display_settings = field_get_display($field_instance, 'default', $entity);
    $display_settings['label'] = 'hidden';

    $field_render = "";
    $field_items = field_get_items($topic->type, $entity, $field_key);

    if ($field_items) {
      $item_count = count($field_items);

      foreach ($field_items as $field_item) {

        if (TOPIC_MAP_EXPORT_PRESERVE_FIELD_MARKUP) {
          $field_render .= render(field_view_value($topic->type, $entity, $field_key, $field_item));
        }
        else {
          $item_render = field_view_value($topic->type, $entity, $field_key, $field_item);
          $field_render .= check_plain(strip_tags(text_summary(render($item_render), 'plain_text')));
          $item_count--;
          if ($item_count) {
            $field_render .= ", ";
          }
        }

      }
    }

    if (TOPIC_MAP_EXPORT_PRESERVE_FIELD_MARKUP) {
      $field_render = _topic_map_export_encode_xml_entities($field_render);
    }

    $occurences[] = theme('topic_map_occurrence', array(
      'type' => 'field-type-' . $field_key . '-' . $topic->type . '-' . $bundle,
      'data' =>  $field_render,
      'scope' => TOPIC_MAP_EXPORT_VARIANT_LANG_ID . '_' . $lang_scope,
    ));
  }

  $variants = array();
  $variants[] = theme('topic_map_variant', array(
    'scopes' => array(TOPIC_MAP_EXPORT_VARIANT_LANG_ID . '_' . $lang_scope, TOPIC_MAP_EXPORT_DISPLAY_TOPIC_ID),
    'data' => $topic->basename,
  ));

  // Subject identifier (Not necessarily a working address, just needs to
  // be unique).
  $si = '';

  if (TOPIC_MAP_EXPORT_USE_ENTITY_URI_FOR_SI) {
    if (isset($info['uri callback'])) {
      $uri = entity_uri($topic->type, $entity);
      if ($uri) {
        $uri['options']['absolute'] = TRUE;
        $si = url($uri['path'], $uri['options']);
      }
    }
  }

  if (empty($si)) {
    $si = url($topic->type . '/' . $topic->entity_id, array('absolute' => TRUE));
  }
  $vars['si'] = array($si);
  $vars['topic_id'] = $topic->topic_id;
  $vars['name'] = $topic->basename . ' (' . $topic->type . '/' . $topic->entity_id . ')';

  $vars['instance_of'] = array_keys($topic->classes);
  if (!is_array($vars['instance_of'])) {
    $vars['instance_of'] = array($vars['instance_of']);
  }

  $vars['occurrences'] = $occurences;
  if (!empty($vars['occurences']) && is_array($vars['occurences'])) {
    $vars['occurences'] = implode('', $vars['occurences']);
  }

  $vars['variants'] = $variants;

}

/**
 * Preprocess theme function to print a list of Drupal entities and bundles.
 *
 * @param array $vars
 *   An associative array containing:
 *   - occurrences: An array of datasets describing occurences of a topic.
 *
 * @see topic-map-occurence-types.tpl.php
 *
 * @ingroup themeable
 */
function template_preprocess_topic_map_occurence_types(&$vars) {

  $default_lang = language_default();
  $lang_scope = $default_lang->language;

  foreach ($vars['occurrences'] as $key => $occurence_set) {

    $vars['occurrences'][$key]['variant_name'] = theme('topic_map_variant', array(
      'scopes' => array(TOPIC_MAP_EXPORT_VARIANT_LANG_ID . '_' . $lang_scope, TOPIC_MAP_EXPORT_DISPLAY_TOPIC_ID),
      'data' => $occurence_set['label'],
    ));

  }

}

/**
 * Preprocess theme function to print a list of Drupal entities and bundles.
 *
 * @param array $vars
 *   An associative array containing:
 *   - entities: An array of entity type datastores.
 *
 * @see topic-map-entities-and-bundles.tpl.php
 * @ingroup themeable
 */
function template_preprocess_topic_map_entities_and_bundles(&$vars) {

  foreach ($vars['entities'] as $entity_id => $entity) {

    $vars['entities'][$entity_id]['entity_si'] = url('entity/' . $entity_id, array('absolute' => TRUE));
    $vars['entities'][$entity_id]['desc_si'] = url('entity/' . $entity_id . '/bundle/description', array('absolute' => TRUE));

    foreach ($entity['bundles'] as $bundle_key => $bundle) {

      $vars['entities'][$entity_id]['bundles'][$bundle_key]['bundle_si'] = url('entity/' . $entity_id . '/bundle/' . $bundle_key, array('absolute' => TRUE));
      $vars['entities'][$entity_id]['bundles'][$bundle_key]['bundle_desc_si'] = url('entity/' . $entity_id . '/bundle-' . $bundle_key . '/description', array('absolute' => TRUE));

      $bundle_desc = '';

      switch ($entity_id) {
        case 'node':
          $full_bundle = node_type_load($bundle_key);
          $bundle_desc = $full_bundle->description;
          break;

        case 'taxonomy_term':
          $full_bundle = taxonomy_vocabulary_machine_name_load($bundle_key);
          $bundle_desc = $full_bundle->description;
          break;

      }

      if (!empty($bundle_desc)) {
        $bundle_desc = check_plain(strip_tags($bundle_desc));
      }

      $vars['entities'][$entity_id]['bundles'][$bundle_key]['bundle_of'] = t('Bundle of @entity_name', array('@entity_name' => $entity['label']));
      $vars['entities'][$entity_id]['bundles'][$bundle_key]['bundle_desc'] = $bundle_desc;

    }

  }
}

/**
 * Helper function for encoding htmlentities as XML compatible.
 *
 * @param string $string
 *   String possibly containing HTML elements and XML incompatible characters.
 *
 * @return string
 *   XML safe encoded string.
 */
function _topic_map_export_encode_xml_entities($string) {

  $string = htmlentities($string, ENT_COMPAT, 'UTF-8', FALSE);

  $arr_xml_special_char = array("&quot;","&amp;","&apos;","&lt;","&gt;");
  $arr_xml_special_char_regex = "(?";
  foreach ($arr_xml_special_char as $key => $value) {
    $arr_xml_special_char_regex .= "(?!$value)";
  }
  $arr_xml_special_char_regex .= ")";
  $pattern = "/$arr_xml_special_char_regex&([a-zA-Z0-9]+;)/";

  $replacement = '&amp;${1}';
  $string = preg_replace($pattern, $replacement, $string);

  return $string;
}
