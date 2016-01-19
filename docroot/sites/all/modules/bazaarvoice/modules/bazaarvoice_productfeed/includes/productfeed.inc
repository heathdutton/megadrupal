<?php
/**
 * @file
 * Contains functions around building the product feed.
 */

/**
 * Rebuild the productfeed.
 */
function bazaarvoice_productfeed_rebuild_productfeed() {
  // Is the productfeed triggered to be rebuilt?
  if (variable_get('bazaarvoice_rebuild_productfeed', FALSE)) {
    // Have products to build?
    if ($products = bazaarvoice_productfeed_get_products()) {
      // Build XML Object.
      $xml = new BazaarVoiceSimpleXMLElement('<?xml version="1.0" encoding="utf-8"?><Feed></Feed>');
      // Build structured feed array.
      $feed_element = bazaarvoice_productfeed_build_feed_element($products);
      // Build the XML feed from feed array.
      bazaarvoice_productfeed_build_feed_xml($xml, $feed_element);
      // Get XML string.
      $xml = $xml->asXML();
      // Bazaarvoice does not like CDATA.
      $xml = str_replace(array("<![CDATA[", "]]>"), "", $xml);
      // Generate .gz file.
      $gzdata = gzencode($xml);
      // Save feed file.
      if (bazaarvoice_productfeed_save_feed($gzdata)) {
        // Set rebuild flag to false.
        variable_set('bazaarvoice_rebuild_productfeed', FALSE);
        return TRUE;
      }
    }
    // @TODO: No products, build empty feed?
  }
  return FALSE;
}

/**
 * Return filepath to productfeed file.
 *
 * @return bool|null
 *   Path to productfeed file or FALSE.
 */
function bazaarvoice_productfeed_get_feed() {
  $file = variable_get('bazaarvoice_productfeed_file', '');
  // Check that the file exists.
  if (!$file || !file_exists($file)) {
    $file = FALSE;
  }
  return $file;
}

/**
 * Saves the feedfile to the filesystem.
 *
 * @param string $feedfile
 *   The feedfile.
 *
 * @return bool
 *   If the feed was saved or not.
 */
function bazaarvoice_productfeed_save_feed($feedfile) {
  $status = FALSE;
  // Get the directory to save to.
  $directory = file_default_scheme() . '://' . variable_get('bazaarvoice_feed_location', '');
  // Create filename.
  $filename = 'productFeed' . date('Y_m_d') . '.xml.gz';
  // Attempt to save the file.
  if (file_save_data($feedfile, $directory . '/' . $filename, FILE_EXISTS_REPLACE)) {
    // Store the file location.
    variable_set('bazaarvoice_productfeed_file', $directory . '/' . $filename);
    $status = TRUE;
  }
  // Return status of saving the file.
  return $status;
}

/**
 * Attempt to FTP file to Bazaarvoice.
 *
 * @param string $feed_file
 *   Path to feed file to FTP.
 *
 * @return bool
 *   Boolean TRUE or FALSE if successful.
 */
function bazaarvoice_productfeed_ftp_feed($feed_file) {
  // Frequency for FTPing file? 0 means never FTP.
  if ($frequency = variable_get('bazaarvoice_ftp_frequency', 0)) {
    // Get last time file FTP'd.
    $last_ftp = variable_get('bazaarvoice_ftp_last', 0);
    // Has Frequency lapsed?
    if ((mktime() - $last_ftp) > $frequency) {
      // Instance of productfeed object.
      $product_feed = new BazaarvoiceProductFeed();
      // Attempt to FTP the file.
      if ($product_feed->ftpProductFeed($feed_file)) {
        // Set time.
        variable_set('bazaarvoice_ftp_last', mktime());
        // @TODO: alter other modules?
        // @TODO: delete feedfile?
        return TRUE;
      }
    }
  }
  return FALSE;
}

/**
 * Create a structured array for building XML product feed.
 *
 * @param array $products
 *   Array of products with attributes and values.
 *
 * @return array
 *   Structured array used to build XML feed.
 */
function bazaarvoice_productfeed_build_feed_element(array $products) {
  // Get default locale.
  $default_locale = bazaarvoice_get_default_locale();
  // Add root attributes.
  $element = array(
    '#attributes' => array(
      'xlmns' => 'http://www.bazaarvoice.com/xs/PRR/ProductFeed/' . Bazaarvoice::apiVersion(),
      'name' => variable_get('bazaarvoice_site_name', variable_get('site_name', '')),
      'incremental' => 'false',
      'extractDate' => date('Y-m-d') . 'T' . date('H:i:s'),
    ),
  );
  // Build brands section.
  $element[] = bazaarvoice_productfeed_build_feed_brands($products, $default_locale);
  // Build categories section.
  $element[] = bazaarvoice_productfeed_build_feed_categories($products, $default_locale);
  // Build products section.
  $element[] = bazaarvoice_productfeed_build_feed_products($products, $default_locale);
  // Allow other modules to alter.
  drupal_alter('bazaarvoice_productfeed_feed_element', $element, $default_locale);
  // Return array.
  return $element;
}

/**
 * Recursive function to build an XML document from a structured array.
 *
 * @param object &$parent_xml
 *   Parent XML object.
 * @param array $element
 *   Element array to turn into XML as part of parent.
 */
function bazaarvoice_productfeed_build_feed_xml(&$parent_xml, array $element) {
  $element_xml = FALSE;
  // See if element has children.
  $children = element_children($element);
  // Are there children?
  if (count($children)) {
    // Does element have a name?
    if (isset($element['#name'])) {
      // Create an xml element.
      $element_xml = $parent_xml->addChild($element['#name']);
    }
    // No element name.
    else {
      // Set the parent xml as the element xml.
      $element_xml = $parent_xml;
    }
    // Loop through the children.
    foreach ($children as $child) {
      // Recursively build children XML.
      bazaarvoice_productfeed_build_feed_xml($element_xml, $element[$child]);
    }
  }
  // No children, has a value?
  elseif (isset($element['#value']) && $element['#value']) {
    // Add element to parent with value.
    $element_xml = $parent_xml->addChild($element['#name'], $element['#value']);
  }
  // Element created and has any additional attributes?
  if ($element_xml && isset($element['#attributes'])) {
    // Add attributes to element XML.
    foreach ($element['#attributes'] as $attribute => $value) {
      $element_xml->addAttribute($attribute, $value);
    }
  }
}

/**
 * Build the brands section of the productfeed array.
 *
 * @param array &$products
 *   Array of products to include in the array.
 * @param string $default_locale
 *   The default language locale.
 *
 * @return array
 *   Structured array for building productfeed XML.
 */
function bazaarvoice_productfeed_build_feed_brands(array &$products, $default_locale) {
  $processed_brands = array();
  // Set section name.
  $brands = array('#name' => 'Brands');
  // Loop through each product.
  foreach ($products as &$product) {
    // Product have a brand external Id?
    if (isset($product['BrandExternalId']) && !in_array($product['BrandExternalId'], $processed_brands)) {
      // Build brand array.
      $brand = array('#name' => 'Brand');
      // Set brand id.
      $brand[] = array(
        '#name' => 'ExternalId',
        '#value' => $product['BrandExternalId'],
      );
      // Add brand name.
      $brand[] = array(
        '#name' => 'Name',
        '#value' => $product['Brand'][$default_locale],
      );
      // Multiple values?
      if (count($product['Brand']) > 1) {
        $names = array('#name' => 'Names');
        foreach ($product['Brand'] as $locale => $name) {
          $names[] = array(
            '#name' => 'Name',
            '#value' => $name,
            '#attributes' => array(
              'locale' => $locale,
            ),
          );
        }
        // Add names array.
        $brand[] = $names;
      }
      // Add brand to brands.
      $brands[] = $brand;
      // Add external ID to processed array.
      $processed_brands[] = $product['BrandExternalId'];
    }
    // Remove Brand, since wont be processed as part of product.
    unset($product['Brand']);
  }
  // Allow other modules to alter.
  drupal_alter('bazaarvoice_productfeed_feed_brands', $brands, $products, $default_locale);
  // Return brands array.
  return $brands;
}

/**
 * Build the categories section of the productfeed array.
 *
 * @param array &$products
 *   Array of products to include in the array.
 * @param string $default_locale
 *   The default language locale.
 *
 * @return array
 *   Structured array for building productfeed XML.
 */
function bazaarvoice_productfeed_build_feed_categories(array &$products, $default_locale) {
  $processed_categories = array();
  $categories = array('#name' => 'Categories');
  // Loop through each product.
  foreach ($products as &$product) {
    // Product have a category set?
    if (isset($product['CategoryExternalId']) && !in_array($product['CategoryExternalId'], $processed_categories)) {
      $category = array('#name' => 'Category');
      $category[] = array(
        '#name' => 'ExternalId',
        '#value' => $product['CategoryExternalId'],
      );
      // Add category name.
      $category[] = array(
        '#name' => 'Name',
        '#value' => $product['Category'],
      );
      // Multiple values?
      if (count($product['Category']) > 1) {
        $names = array('#name' => 'Names');
        foreach ($product['Category'] as $locale => $name) {
          $names[] = array(
            '#name' => 'Name',
            '#value' => $name,
            '#attributes' => array(
              'locale' => $locale,
            ),
          );
        }
        $category[] = $names;
      }
      $categories[] = $category;
      // Add external ID to processed array.
      $processed_categories[] = $product['CategoryExternalId'];
    }
    // Remove category, since wont be processed as part of product.
    unset($product['Category']);
  }
  // Allow other modules to alter.
  drupal_alter('bazaarvoice_productfeed_feed_categories', $categories, $products, $default_locale);
  // Return categories array.
  return $categories;
}

/**
 * Build structured array of products for productfeed xml.
 *
 * @param array $products
 *   Array of products with attributes and value.
 * @param string $default_locale
 *   The default locale.
 *
 * @return array
 *   Structured array for building XML elements.
 */
function bazaarvoice_productfeed_build_feed_products(array $products, $default_locale) {
  module_load_include('inc', 'bazaarvoice_productfeed', 'includes/attributes');
  // Get list of attributes.
  $attributes = bazaarvoice_productfeed_get_attributes();
  // Get list of custom attributes;
  $custom_attributes = bazaarvoice_productfeed_get_custom_attributes();
  // Create product element.
  $products_element = array('#name' => 'Products');
  // Process each product.
  foreach ($products as $product) {
    $product_element = array('#name' => 'Product');
    // Array for custom attributes.
    $custom = array('#name' => 'Attributes');
    // Loop through the attributes.
    foreach ($attributes as $attribute_name => $attribute_data) {
      // Product has attribute?
      if (isset($product[$attribute_name])) {
        // Custom Attribute?
        if (isset($custom_attributes[$attribute_name])) {
          $custom[] = array(
            '#name' => 'Attribute',
            '#attributes' => array(
              'id' => $attribute_name,
            ),
            array(
              '#name' => 'Value',
              '#value' => $product[$attribute_name],
            ),
          );
        }
        // A standard attribute.
        else {
          // Create the single attribute.
          $product_element[] = array(
            '#name' => $attribute_name,
            '#value' => $product[$attribute_name],
          );
          // Supports multiple values?
          if (isset($attribute_data['multiple']) && isset($product[$attribute_data['multiple']])) {
            $multiple = array(
              '#name' => $attribute_data['multiple'],
            );
            // Support translation?
            if (isset($attribute_data['translatable']) && $attribute_data['translatable']) {
              foreach ($product[$attribute_data['multiple']] as $locale => $value) {
                if ($value) {
                  // Set the locale attribute of the attribute.
                  $multiple[] = array(
                    '#name' => $attribute_name,
                    '#value' => $value,
                    '#attributes' => array(
                      'locale' => $locale,
                    ),
                  );
                }
              }
            }
            // Not translatable, do not care about the key.
            else {
              // Loop through each value and create an attribute.
              foreach ($product[$attribute_data['multiple']] as $value) {
                if ($value) {
                  $multiple[] = array(
                    '#name' => $attribute_name,
                    '#value' => $value,
                  );
                }
              }
            }
            $product_element[] = $multiple;
          }
        }
      }
    }
    // Add any custom attributes.
    if (count($custom) > 1) {
      $product_element[] = $custom;
    }
    // Add to products element.
    $products_element[] = $product_element;
  }
  // Allow to be altered.
  drupal_alter('bazaarvoice_productfeed_feed_products', $products_element, $products, $default_locale);
  // Return array of products.
  return $products_element;
}
