<?php

/**
 * @file
 * Extension of the Views Plugin Style for Leaflet Map
 * Adapted from the GeoField Map views module and the OpenLayers Views module.
 */
class leaflet_views_ajax_popup_plugin_style extends leaflet_views_plugin_style {

  /**
   * If this view is displaying an entity, save the entity type and info.
   */
  function init(&$view, &$display, $options = NULL) {
    parent::init($view, $display, $options);

    if (empty($this->entity_type) && strpos($this->view->base_table, 'search_api_index_') === 0) {
      $index_name = str_replace('search_api_index_', '', $this->view->base_table);
      $index = search_api_index_load($index_name);
      if ($index->item_type) {
        $info = entity_get_info($index->item_type);
        if ($info) {
          $this->entity_type = $index->item_type;
          $this->entity_info = $info;
        }
      }
    }
  }

  /**
   * Options form.
   */
  function options_form(&$form, &$form_state) {
    if ($this->entity_type) {
      parent::options_form($form, $form_state);

      // We accept rendered entity only as popup.
      unset($form['description_field']);
      unset($form['view_mode']['#states']);

      $form['view_mode']['#title'] = t('Popup view mode');
      $form['view_mode']['#description'] = t('Select what view mode should be used to render an entity when displaying popup.');
    }
    else {
      $form = array(
        '#markup' => '<p>' . t('Entity type is not defined for this view. Please consider using another display style.') . '</p>',
      );
    }
  }

  /**
   * Renders view.
   */
  function render() {
    if (!empty($this->view->live_preview)) {
      return t('No preview available');
    }
    $data = array();
    $map = leaflet_map_get_info($this->options['map']);
    // Is there a geofield selected?
    if ($this->options['data_source']) {
      $this->render_fields($this->view->result);
      foreach ($this->view->result as $id => $result) {
        $geofield = $this->get_field_value($id, $this->options['data_source']);

        if (!empty($geofield)) {
          // Render the entity with the selected view mode:
          $entity_id = isset($result->{$this->entity_info['entity keys']['id']})
            ? $result->{$this->entity_info['entity keys']['id']}
            : ((isset($result->entity) && is_numeric($result->entity)) ? $result->entity : 0);
          $description = leaflet_views_ajax_popup_markup($this->entity_type, $entity_id, $this->options['view_mode']);
          $points = leaflet_process_geofield($geofield);
          // Attach pop-ups if we have rendered into $description:
          foreach ($points as &$point) {
            $point['popup'] = $description;
          }
          // Attach also titles & entities, they might be used later on.
          if ($this->options['name_field']) {
            foreach ($points as &$point) {
              $point['label'] = $this->rendered_fields[$id][$this->options['name_field']];
            }
          }
          if (isset($this->options['icon']['iconType']) && $this->options['icon']['iconType'] == 'html') {
            foreach ($points as &$point) {
              $target_field = $this->options['icon']['html'];
              $point['rendered_html'] = isset($this->rendered_fields[$id][$target_field]) ? $this->rendered_fields[$id][$target_field] : '';
            }
          }
          // Let modules modify the points data.
          drupal_alter('leaflet_views_alter_points_data', $result, $points);
          // Merge these points into the $data array for map rendering:
          $data = array_merge($data, $points);
        }
      }
      leaflet_apply_map_settings($map, $data, $this->options, $this->entity_type);
      if (empty($data) && !empty($this->options['hide_empty'])) {
        return '';
      }
      $build = leaflet_build_map($map, $data, $this->options['height'] . 'px');
      $build['#attached']['js'][] = array(
        'type' => 'file',
        'data' => drupal_get_path('module', 'leaflet_views_ajax_popup') . '/leaflet_views_ajax_popup.js',
      );
      $build['#attached']['css'][] = drupal_get_path('module', 'leaflet_views_ajax_popup') . '/leaflet_views_ajax_popup.css';
      return $build;

    }
    return '';
  }
}
