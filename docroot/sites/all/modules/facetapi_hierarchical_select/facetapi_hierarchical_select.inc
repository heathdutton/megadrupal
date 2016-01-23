<?php

class FacetapiWidgetHierarchicalSelect extends FacetapiWidgetLinks {
  public function execute() {
    $build = $this->getBuild();
    $searcher = $build['#settings']->searcher;
    $searcher_info = explode('@', $searcher);
    $index_name = $searcher_info[1];
    $form_id = 'facetapi_hierarchical_select_form_' . $index_name . '_' . $this->getKey();
    $element = &$this->build[$this->facet['field alias']];
    $global_settings = $this->facet->getAdapter()
      ->getFacetSettingsGlobal($this->facet->getFacet())->settings;
    $element = drupal_get_form($form_id, $this, $element, $global_settings);
  }

  public function getSettings() {
    return $this->settings;
  }

  public function settingsForm(&$form, &$form_state) {
    $states = array(
      'visible' => array(
        'select[name="widget"]' => array('value' => $this->id),
      ),
    );

    $form['widget']['widget_settings'][$this->id]['facetapi_hide_facet_count'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hide the facet count'),
      '#default_value' => !empty($this->settings->settings['facetapi_hide_facet_count']) ? $this->settings->settings['facetapi_hide_facet_count'] : FALSE,
      '#states' => $states,
    );
  }

  public function getDefaultSettings() {
    return array(
      'facetapi_hide_facet_count' => 0,
    );
  }
}