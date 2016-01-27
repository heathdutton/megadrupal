<?php

/**
 * @file
 * The facetapi_links and facetapi_checkbox_links widget plugin classes.
 */

/**
 * Widget that renders facets as a list of clickable links.
 *
 * Links make it easy for users to narrow down their search results by clicking
 * on them. The render arrays use theme_item_list() to generate the HTML markup.
 */
class RestWsSearchApiWidgetRaw extends FacetapiWidget {

  /**
   * Overrides FacetapiWidget::__construct().
   *
   * For links, it is better to use the machine name of the facet as opposed to
   * the alias for the key. Alias are usually, but not necessarily, unique. It
   * doesn't make sense to group links in the same element as they are usually
   * rendered in blocks with are separate from one another.
   */
  public function __construct($id, array $realm, FacetapiFacet $facet, stdClass $settings) {
    parent::__construct($id, $realm, $facet, $settings);
    $this->key = $facet['name'];
  }

  /**
   * Implements FacetapiWidget::execute().
   *
   * Transforms the render array into something that can be themed by
   * theme_item_list().
   *
   * @see FacetapiWidgetLinks::setThemeHooks()
   * @see FacetapiWidgetLinks::buildListItems()
   */
  public function execute() {
    $element = &$this->build[$this->facet['field alias']];

    // Sets each item's theme hook, builds item list.
    $this->setThemeHooks($element);
    $element = array(
      'items' => $this->buildListItems($element),
    );
  }

  /**
   * Recursive function that sets each item's theme hook.
   *
   * The indivual items will be rendered by different theme hooks depending on
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
      $item['#theme'] = ($item['#active']) ? 'facetapi_link_active' : 'facetapi_link_inactive';
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

    // Builds rows.
    $items = array();
    foreach ($build as $value => $item) {
      $row = array();

      // Initializes variables passed to theme hook.
      $variables = array(
        'text' => $item['#markup'],
        'path' => url($item['#path'],  array(
          'html' => $item['#html'],
          'query' => $item['#query'],
        )),
        'active' => $item['#active'],
        'count' => $item['#count'],
      );

      // Adds the facetapi-zero-results class to items that have no results.
      if (!$item['#count']) {
        $variables['options']['attributes']['class'][] = 'facetapi-zero-results';
      }

      // If the item has no children, it is a leaf.
      if (!empty($item['#item_children'])) {
        // If the item is active or the "show_expanded" setting is selected,
        // show this item as expanded so we see its children.
        if ($item['#active'] || !empty($settings['show_expanded'])) {
          $row['children'] = $this->buildListItems($item['#item_children']);
        }
      }

      // Themes the link, adds row to items.
      $row['data'] = $variables;
      $items[] = $variables;
    }

    return $items;
  }

  /**
   * Gets the base class array for a facet item.
   *
   * Classes that extend FacetapiWidgetLinks will often overide this method to
   * alter the link displays via CSS without having to touch the render array.
   *
   * @return array
   *   An array of classes.
   */
  function getItemClasses() {
    return array();
  }

  /**
   * Overrides FacetapiWidget::settingsForm().
   */
  function settingsForm(&$form, &$form_state) {
    // @see http://drupal.org/node/735528 for supporting multiple values in the
    // FAPI #states. The following hack adds multiple form elements and uses CSS
    // and JavaScript to ensure only one is displayed at a time. Only the last
    // form element actually passes the value.
    $form['widget']['widget_settings']['links'][$this->id]['soft_limit'] = array(
      '#type' => 'select',
      '#title' => t('Soft limit'),
      '#default_value' => $this->settings->settings['soft_limit'],
      '#options' => array(0 => t('No limit')) + drupal_map_assoc(array(50, 40, 30, 20, 15, 10, 5, 3)),
      '#description' => t('Limits the number of displayed facets via JavaScript.'),
      '#states' => array(
        'visible' => array(
          'select[name="widget"]' => array('value' => $this->id),
        ),
      ),
    );

    // @see http://drupal.org/node/735528
    if ($this->facet['hierarchy callback']) {
      $form['widget']['widget_settings']['links'][$this->id]['show_expanded'] = array(
        '#type' => 'checkbox',
        '#title' => t('Expand hierarchy'),
        '#default_value' => !empty($this->settings->settings['show_expanded']),
        '#description' => t('Show the entire tree regardless of whether the parent items are active.'),
        '#states' => array(
          'visible' => array(
            'select[name="widget"]' => array('value' => $this->id),
          ),
        ),
      );
    }

    // Hides all but the last element. The #states system will override this,
    // however it is necessary if JavaScript is disabled so multiple elements
    // aren't displayed to the user.
    $last = end($form['widget']['widget_settings']['links']);
    foreach ($form['widget']['widget_settings']['links'] as $id => $element) {
      if ($last != $element) {
        $form['widget']['widget_settings']['links'][$id]['#attributes']['style'] = 'display: none;';
      }
    }
  }

  /**
   * Overrides FacetapiWidget::getDefaultSettings().
   */
  function getDefaultSettings() {
    return array(
      'soft_limit' => 20,
      'nofollow' => 1,
      'show_expanded' => 0,
    );
  }
}