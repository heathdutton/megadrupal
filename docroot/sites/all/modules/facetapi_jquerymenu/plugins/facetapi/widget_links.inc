<?php

/**
 * @file
 * Widgets for facets rendered in a JQuery Menu.
 */

/**
 * Widget that renders facets as links in a JQuery Menu.
 */
class FacetapiJQueryMenuWidgetLinks extends FacetapiWidgetLinks {

  /**
   * Implements FacetapiWidget::execute().
   *
   * Transforms the render array into something that can be themed by
   * theme_facetapi_jquerymenu_menu().
   *
   * @see FacetapiJQueryMenuWidgetLinks::setThemeHooks()
   * @see FacetapiJQueryMenuWidgetLinks::buildListItems()
   */
  public function execute() {
    $element = &$this->build[$this->facet['field alias']];
    $this->setThemeHooks($element);

    $tree = $this->buildListItems($element);
    $trail = array();

    // Sets each item's theme hook, builds item list.
    $this->setThemeHooks($element);
    $element = array(
      '#theme' => 'facetapi_jquerymenu_menu',
      '#tree' => $tree,
      '#trail' => $trail,
      '#attributes' => $this->build['#attributes'],
    );

    drupal_add_js(drupal_get_path('module', 'facetapi_jquerymenu') . '/facetapi_jquerymenu.js');
  }

  /**
   * Recursive function that sets each item's theme hook.
   *
   * The individual items will be rendered by different theme hooks depending on
   * whether or not they are active.
   *
   * @param array &$build
   *   A render array containing the facet items.
   *
   * @return FacetapiWidget
   *   An instance of this class.
   *
   * @see theme_facetapi_link_active()
   * @see theme_facetapi_link_inactive()
   */
  protected function setThemeHooks(array &$build) {
    foreach ($build as $value => &$item) {
      $item['#theme'] = ($item['#active']) ? 'facetapi_jquerymenu_item_active' : 'facetapi_jquerymenu_item_inactive';
      if (!empty($item['#item_children'])) {
        $this->setThemeHooks($item['#item_children']);
      }
    }
    return $this;
  }

  /**
   * Transforms the render array for use with theme_item_list().
   *
   * The recursion allows this function to act on the various levels of a
   * hierarchical data set.
   *
   * @param array $build
   *   The items in the facet's render array being transformed.
   *
   * @return array
   *   The "items" parameter for theme_item_list().
   */
  function buildListItems($build) {
    $settings = $this->settings->settings;

    // Initializes links attributes, adds rel="nofollow" if configured.
    $attributes = ($settings['nofollow']) ? array('rel' => 'nofollow') : array();
    $attributes += array('class' => $this->getItemClasses());

    // Builds rows.
    $tree = array();

    foreach ($build as $value => $item) {
      // Initializes variables passed to theme hook.
      $variables = array(
        'text' => $item['#markup'],
        'options' => array(
          //'attributes' => $attributes,
          'html' => $item['#html'],
        ),
      );

      if ($settings['display_count']) {
        $variables['count'] = $item['#count'];
      }

      // Adds the facetapi-zero-results class to items that have no results.
      if (!$item['#count']) {
        $attributes['class'][] = 'facetapi-zero-results';
      }

      // Add an ID to identify this link.
      $attributes['id'] = drupal_html_id('facetapi-link');

      // Gets theme hook, adds last minute classes.
      $class = ($item['#active']) ? 'facetapi-active' : 'facetapi-inactive';
      $attributes['class'][] = $class;

      // Themes the link, adds row to items.
      $text = theme($item['#theme'], $variables);

      $has_children = count($item['#item_children']) > 0;

      $tree[] = array(
        'link' => array(
          'href' => $item['#path'],
          'title' => $text,
          'has_children' => $has_children,
          'expanded' => $item['#active'],
          'options' => array(
            'query' => $item['#query'],
            'html' => TRUE,
            'attributes' => $attributes,
          ),
          'hidden' => 0,
        ),
        'below' => $has_children ? $this->buildListItems($item['#item_children']) : array(),
      );
    }

    return $tree;
  }
}


/**
 * Widget that renders facets as links in JQuery Menu with clickable checkboxes.
 *
 * This widget renders facets in the same way as FacetapiJQueryMenuWidgetLinks
 * but uses JavaScript to transform the links into checkboxes followed by the
 * facet.
 */
class FacetapiJQueryMenuWidgetCheckboxLinks extends FacetapiJQueryMenuWidgetLinks {

  /**
   * Overrides FacetapiWidgetLinks::init().
   *
   * Adds additional JavaScript settings and CSS.
   */
  public function init() {
    parent::init();
    $this->jsSettings['makeCheckboxes'] = 1;
    drupal_add_css(drupal_get_path('module', 'facetapi') . '/facetapi.css');
  }

  /**
   * Overrides FacetapiWidgetLinks::getItemClasses().
   *
   * Sets the base class for checkbox facet items.
   */
  public function getItemClasses() {
    return array('facetapi-checkbox');
  }
}
