<?php
/**
 * Override sitemap writers for mobile sitemaps
 */
/**
 * Extended class for writing XML sitemap files.
 */
class MobileDomainSitemapWriter extends XMLSitemapWriter {
    /**
   * Write an full XML sitemap element tag.
   *
   * @param $name
   *   The element name.
   * @param $element
   *   An array of the elements properties and values.
   */
  public function writeSitemapElement($name, array &$element) {
    if ($name == 'url') {
      $element['mobile:mobile'] = '';
    }
    return parent::writeSitemapElement($name, $element);
  }
  public function getRootAttributes() {
    $attributes = parent::getRootAttributes();
    $attributes['xmlns:mobile'] = "http://www.google.com/schemas/sitemap-mobile/1.0";
    return $attributes;
  }
}


class MobileDomainSitemapIndexWriter extends XMLSitemapIndexWriter {
  public function getRootAttributes() {
    $attributes = parent::getRootAttributes();
    $attributes['xmlns:mobile'] = "http://www.google.com/schemas/sitemap-mobile/1.0";
    return $attributes;
  }
}