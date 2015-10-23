<?php

/**
 * Class good_relations_regular_content_types
 *
 * The difference between this class and the good_relations_entity_reference
 * class (from 'products'), is that this mapping (class) will be used only for
 * commerce_product references and not all entity_references (commerce_product
 * and field_collection_items).
 */
class good_relations_commerce_product implements good_relations_interface {

  protected $type;
  protected $bundle;
  protected $mapping_fields = array();

  function __construct($type = NULL, $bundle = NULL, $mapping_fields = NULL) {
    //revise here when implementing entity support
    $this->type = 'commerce_product';
    $this->bundle = $bundle;
    $this->mapping_fields = $mapping_fields;
  }

  // Extra (Getters and Setters).
  public function setType($type = NULL) {
    $this->type = $type;
  }

  public function getType() {
    return $this->type;
  }

  public function setBundle($bundle = NULL) {
    $this->bundle = $bundle;
  }

  public function getBundle() {
    return $this->bundle;
  }

  public function setMappingFields($mapping_fields = NULL) {
    $this->mapping_fields = $mapping_fields;
  }

  public function getMappingFields() {
    return $this->mapping_fields;
  }

  //---------------------------------------------------------------------------

  public function resultMessage() {
    return t('The following ' . $this->type . ' type has been mapped: ' . $this->bundle);
  }

  /**
   * @return array
   */
  public function namespaces() {
    return array(
      'gr' => 'http://purl.org/goodrelations/v1#',
      // Google's namespace for their custom vocabularies.
      'v' => 'http://rdf.data-vocabulary.org/#',
    );
  }

  /**
   * The RDF MAPPING! (node)
   *
   * @return array
   */
  public function map() {
    $rdf_mapping = array();

    // Good Relations offering set up.
    $rdf_mapping = array_merge($rdf_mapping, array('rdf_type' => array('gr:Offering')));
    // The title does not appear a normal field.
    $rdf_mapping = array_merge($rdf_mapping, array('title' => array('predicates' => array('gr:name'))));
    foreach ($this->mapping_fields as $id => $value) {
      $rdf_mapping = array_merge($rdf_mapping, array($id => array('predicates' => array($value))));
    }

    $product_mapping = array(
      'type' => $this->type,
      'bundle' => $this->bundle,
      'mapping' => $rdf_mapping,
    );

    return $product_mapping;
  }
}
