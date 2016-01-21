<?php

/**
 * @file
 * Provide additional Views fields for entity content.
 */

/**
 * Implements hook_views_data_alter().
 */
function profanity_views_data_alter(&$data) {
  $entity_info = entity_get_info();

  foreach ($entity_info as $entity_type => $info) {
    if (empty($info['entity keys']['label'])) {
      continue;
    }

    $title_property = $info['entity keys']['label'];

    if (empty($data[$entity_type][$title_property])) {
      continue;
    }

    $data[$entity_type]["profanity_{$title_property}"] = $data[$entity_type][$title_property];
    $data[$entity_type]["profanity_{$title_property}"]['title'] = t('@property - Profanity filtered', array('@property' => $data[$entity_type][$title_property]['title']));
    $data[$entity_type]["profanity_{$title_property}"]['help'] = t('Run the entity title through profanity filters, allows you to select which filters.');
    $data[$entity_type]["profanity_{$title_property}"]['field']['handler'] = 'profanity_views_handler_field';
    $data[$entity_type]["profanity_{$title_property}"]['field']['type'] = $entity_type;
    $data[$entity_type]["profanity_{$title_property}"]['field']['base_table'] = $info['base table'];
    $data[$entity_type]["profanity_{$title_property}"]['field']['id'] = $info['entity keys']['id'];
  }

  // If present, integrate with Search API Views.
  if (module_exists('search_api_views')) {
    try {
      $entity_types = entity_get_info();
      foreach (search_api_index_load_multiple(FALSE) as $index) {
        $key = 'search_api_index_' . $index->machine_name;
        $data[$key]['profanity_search_api_excerpt'] = array(
          'group' => 'Search',
          'title' => t('Excerpt - Profanity filtered'),
          'help' => t('The search result excerpted to show found search terms filtered by the Profanity word lists.'),
          'field' => array(
            'type' => 'text',
            'handler' => 'profanity_views_handler_search_excerpt',
          ),
        );
      }
    }
    catch (Exception $e) {
      watchdog_exception('search_api_views', $e);
    }
  }
}

/**
 * Field handler to provide simple renderer that allows linking to a entity.
 * Definition terms:
 * - link_to_node default: Should this field have the checkbox "link to entity" enabled by default.
 *
 * @ingroup views_field_handlers
 * @todo, i18n work!
 */
class profanity_views_handler_field extends views_handler_field {
  public $entity_type;
  public $entity_id_key;

  function init(&$view, &$options) {
    parent::init($view, $options);

    // Don't add the additional fields to groupby.
    if (!empty($this->options['link_to_entity'])) {
      $this->additional_fields['entity_info'] = array('table' => $this->definition['base_table'], 'field' => $this->definition['id']);
      /*if (module_exists('translation')) {
        $this->additional_fields['language'] = array('table' => 'node', 'field' => 'language');
      }*/
    }

    // Grab and save some entity based info.
    module_load_include('inc', 'entity', 'includes/entity.property');
    $this->entity_type = entity_property_extract_innermost_type($this->definition['type']);
    $this->entity_id_key = $this->definition['id'];
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_entity'] = array(
      'default' => FALSE,
      'bool' => TRUE,
    );
    $options['profanity_lists'] = array(
      'default' => array(),
    );
    return $options;
  }

  /**
   * Provide link to node option
   */
  function options_form(&$form, &$form_state) {
    $form['link_to_entity'] = array(
      '#title' => t('Link this field to the original piece of content'),
      '#description' => t("Enable to override this field's links."),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_entity']),
    );

    $form['profanity_lists'] = array(
      '#title' => t('Profanity lists to run'),
      '#type' => 'checkboxes',
      '#options' => profanity_get_lists_flat(),
      '#default_value' => !empty($this->options['profanity_lists']) ? $this->options['profanity_lists'] : array(),
      '#description' => t('Select which lists will be applied to the entity title.'),
    );

    parent::options_form($form, $form_state);
  }

  /**
   * Render whatever the data is as a link to the node.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_entity']) && !empty($this->additional_fields['entity_info'])) {
      if ($data !== NULL && $data !== '') {
        // This isn't nice but no choice here as there isn't a path component
        // to entity_info. Submit an issue to have other types added here.
        // Recognised entity types which have known paths should bypass
        // the long-ass entity load process.
        if (in_array($this->entity_type, array('node', 'taxonomy_term', 'user', 'harmony_thread', 'harmony_post'))) {
          switch ($this->entity_type) {
            case 'node':
              $path = 'node';
              break;
            case 'taxonomy_term':
              $path = 'taxonomy/term';
              break;
            case 'user':
              $path = 'user';
              break;
            case 'harmony_thread':
              $path = 'thread';
              break;
            case 'harmony_post':
              $path = 'post';
              break;
          }

          $this->options['alter']['path'] = $path . '/' . $values->{$this->entity_id_key};
          $this->options['alter']['make_link'] = TRUE;
        }
        else {
          $entity = entity_load_single($this->entity_type, $this->get_value($values, $this->entity_id_key));

          if ($entity) {
            $url = entity_uri($this->entity_type, $entity);
            $this->options['alter']['path'] = $url['path'];
            $this->options['alter']['make_link'] = TRUE;
          }
        }
      }
      else {
        $this->options['alter']['make_link'] = FALSE;
      }
    }
    return $data;
  }

  function render($values) {
    $value = $this->get_value($values);
    $sanitised = $this->sanitize_value($value);

    // Run through profanity lists.
    if (!empty($this->options['profanity_lists'])) {
      foreach ($this->options['profanity_lists'] as $list_name) {
        $sanitised = profanity_list_execute($list_name, $sanitised);
      }
    }

    return $this->render_link($sanitised, $values);
  }
}

class profanity_views_handler_search_excerpt extends profanity_views_handler_field {
  function construct() {
    parent::construct();
    $this->real_field = 'search_api_excerpt';
    //$this->additional_fields = array('vid');
  }

  function options_form(&$form, &$form_state) {
    $form['profanity_lists'] = array(
      '#title' => t('Profanity lists to run'),
      '#type' => 'checkboxes',
      '#options' => profanity_get_lists_flat(),
      '#default_value' => !empty($this->options['profanity_lists']) ? $this->options['profanity_lists'] : array(),
      '#description' => t('Select which lists will be applied to the entity title.'),
    );

    parent::options_form($form, $form_state);
  }
}
