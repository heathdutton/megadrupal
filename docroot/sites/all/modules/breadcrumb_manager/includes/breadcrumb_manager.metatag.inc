<?php

/**
 * Title meta tag controller.
 *
 * This extends DrupalTitleMetaTag as we need to alter variables in
 * template_preprocess_html() rather output a normal meta tag.
 */
class BreadcrumbManagerTitleMetaTag extends DrupalTitleMetaTag {

  public function getElement(array $options = array()) {
    $element = array();
    $value = $this->getValue($options);

    if ($title = breadcrumb_manager_is_title_required()) {
      list(, $site_name) = explode('|', $value);
      $value = implode(' | ', array($title, trim($site_name)));
    }

    $element['#attached']['metatag_set_preprocess_variable'][] = array('html', 'head_title', $value);
    $element['#attached']['metatag_set_preprocess_variable'][] = array('html', 'head_array', array('title' => $value));
    return $element;
  }
}