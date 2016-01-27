<?php

/**
 * Abstract class, provides implementation of configuration options and source
 * element handling that is common to all Micriodata Import parsers.
 */
abstract class MicrodataImportParser extends FeedsParser {
  protected $crawl_log = array();
  protected $md_properties = array();

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    return array(
      'example_source' => '',
    ) + parent::configDefaults();
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $form = parent::configForm($form_state);
    $form['example_source'] = array(
      '#type' => 'textfield',
      '#title' => t('Example source'),
      '#description' => t('Use an example source to get available properties <strong>Note:</strong> Only properties that are available on this example page will be available for mapping.'),
      '#default_value' => $this->config['example_source'],
    );

    return $form;
  }

  /**
   * Overrides parent::getMappingSources().
   */
  public function getMappingSources() {
    $sources = parent::getMappingSources();
    $properties = array();

    microdata_import_include_library();
    if (!empty($this->config['example_source'])) {
      $properties = array();
      $md = new MicrodataPhp($this->config['example_source']);
      $data = $md->obj();
      $items = $data->items;
      // For now, we only show properties from the first top-level item.
      $properties = $this->getMicrodataPropertyList($items[0], '', 2);
    }
    foreach ($properties as $property) {
      $sources['microdata:' . $property] = array(
        'name' => check_plain($property),
      );
    }

    return $sources;
  }

  /**
   * 
   */
  public function getSourceElement(FeedsSource $source, FeedsParserResult $result, $element_key) {
    $item = $result->currentItem();
    $keys = explode(':', $element_key);

    // If this is a microdata field.
    if ($keys[0] == 'microdata') {
      unset($keys[0]);
      // Use the first element in the microdata array because we only map the
      // first top level item for now. This would change if we introduce
      // support for itemtype selection.
      $first_item = $item['microdata'][0];

      // If the property is the itemid, return that.
      if ($keys[1] == 'itemid') {
        return $first_item->id;
      }

      // Otherwise, find the property in the tree.
      $properties = $first_item->properties;
      $values = array();
      while (count($keys) > 1) {
        $p = array();
        $key = array_shift($keys);
        // Here we merge the arrays. This is why this method only works for
        // properties on the root item, not properties of subitems. To properly
        // handle subitems, use the mapping with itemid method.
        foreach ($properties[$key] as $item) {
          if ($key == 'itemid') {
            continue;
          }
          $p = array_merge_recursive($p, $item->properties);
          $p['itemid'][] = $item->id;
        }
        $properties = $p;
      }

      $key = array_shift($keys);
      return $properties[$key];
    }
    else {
      return parent::getSourceElement($source, $result, $element_key);
    }
  }

  protected function getMicrodataPropertyList($item, $parents, $levels) {
    if (isset($item->id)) {
      $this->md_properties[] = 'itemid';
    }
    foreach ($item->properties as $name => $property) {
      if (is_object($property[0]) && $levels != 0) {
        $parents .= $name . ':';
        $this->getMicrodataPropertyList($property[0], $parents, $levels--);
        $this->md_properties[] = $parents . 'itemid';
        $parents = '';
      }
      else {
        $this->md_properties[] = $parents . $name;
      }
    }

    return $this->md_properties;
  }
}
