<?php

/**
 * @file
 * Widgets for facets rendered as tabs.
 */


class FacetapiWidgetTabLinks extends FacetapiWidgetLinks {

  /**
   * Renders the links.
   */
  public function execute() {
    $element = &$this->build[$this->facet['field alias']];

    $total_count = 0;
    foreach ($element as &$e) {
      // Tally up the total hits for the "All" tab.
      $total_count += $e['#count'];

      if ($e['#active']) {
        $active = $e;
      }

      // For each link, drop any active facet that isn't defined by the
      // link itself. In effect, only the filter defined by the current
      // link is active.
      if (isset($e['#query']['f']) && is_array($e['#query']['f'])) {
        foreach ($e['#query']['f'] as $key => $value) {
          $temp = explode(':', $value);
          if ($temp[0] == $this->facet['field alias'] && $temp[1] != $e['#indexed_value']) {
            unset($e['#query']['f'][$key]);
          }
        }
      }
    }

    if ($this->settings->settings['show_all']) {
      // Create the "All" tab using the last looped filter as the base.
      $all = isset($active) ? $active : $e;
      $all['#markup'] = t('All');
      $all['#count'] = $total_count;
      $all['#active'] = isset($active) ? FALSE : TRUE;
      $all['#path'] = isset($active) ? $all['#path'] : current_path();

      if (isset($all['#query']['f']) && is_array($all['#query']['f'])) {
        foreach ($all['#query']['f'] as $key => $value) {
          $temp = explode(':', $value);
          if ($temp[0] == $this->facet['field alias']) {
            unset($all['#query']['f'][$key]);
          }
        }
      }

      // Add our new tab to the beginning of the list.
      array_unshift($element, $all);
    }

    // Sets each item's theme hook, builds item list.
    $this->setThemeHooks($element);

    $attributes = $this->build['#attributes'];
    $attributes['class'][] = 'tabs';

    $element = array(
      '#theme' => 'item_list',
      '#items' => $this->buildListItems($element),
      '#attributes' => $attributes,
    );
  }

  /**
   * Converts the render array for passing to theme_item_list().
   *
   * @param array $build
   *   The facet's render array.
   *
   * @return array
   *   The "items" parameter for theme_item_list().
   */
  public function buildListItems($build) {
    $items = array();

    foreach ($build as $value => $item) {
      $row = array('class' => array());

      // Initializes variables passed to theme hook.
      $variables = array(
        'text' => $item['#markup'],
        'path' => $item['#path'],
        'options' => array(
          'attributes' => array('class' => $this->getItemClasses()),
          'html' => $item['#html'],
          'query' => $item['#query'],
        ),
      );

      $display_count = $this->settings->settings['show_count'];

      if (!empty($display_count)) {
        $variables['count'] = $item['#count'];
      }

      // If the item has no children, it is a leaf.
      if (empty($item['#item_children'])) {
        $row['class'][] = 'leaf';
      }
      else {
        // If the item is active or the "show_expanded" setting is selected,
        // show this item as expanded so we see its children.
        if ($item['#active'] || !empty($this->settings->settings['show_expanded'])) {
          $row['class'][] = 'expanded';
          $row['children'] = $this->buildListItems($item['#item_children']);
        }
        else {
          $row['class'][] = 'collapsed';
        }
      }

      // Gets theme hook, adds last minute classes.
      if ($item['#active']) {
        $class = 'facetapi-tabs-active active';
        $item['#theme'] = 'facetapi_tabs_active';
        $row['class'][] = 'active';
      }
      else {
        $class = 'facetapi-tabs-inactive inactive';
        $item['#theme'] = 'facetapi_tabs_inactive';
      }

      $variables['options']['attributes']['class'][] = $class;

      // Themes the link, adds row to items.
      $row['data'] = theme($item['#theme'], $variables);

      $items[] = $row;
    }

    return $items;
  }

  /**
   * Overrides FacetapiWidgetLinks::settingsForm().
   */
  function settingsForm(&$form, &$form_state) {
    parent::settingsForm($form, $form_state);

    // @see http://drupal.org/node/1370342
    $form['widget']['widget_settings']['links'][$this->id]['show_all'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show "all" facet'),
      '#default_value' => !empty($this->settings->settings['show_all']),
      '#description' => t('Whether to display the "all" facet or not'),
      '#states' => array(
        'visible' => array(
          'select[name="widget"]' => array('value' => $this->id),
        ),
      ),
    );
    $form['widget']['widget_settings']['links'][$this->id]['show_count'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show count in facet'),
      '#default_value' => !empty($this->settings->settings['show_count']),
      '#description' => t('Whether to display count in facet or not'),
      '#states' => array(
        'visible' => array(
          'select[name="widget"]' => array('value' => $this->id),
        ),
      ),
    );
  }

  /**
   * Overrides FacetapiWidgetLinks::getDefaultSettings().
   */
  function getDefaultSettings() {
    $settings = parent::getDefaultSettings();
    $settings['show_all'] = 1;
    $settings['show_count'] = 1;
    return $settings;
  }
}
