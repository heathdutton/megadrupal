<?php

class good_relations_entity_reference implements good_relations_interface {

  protected $entity_type;
  protected $entity_bundle;
  protected $entity_mapped_fields = array();

  function __construct($entity_type = NULL, $entity_bundle = NULL, $entity_mapped_fields = NULL) {
    $this->entity_type = $entity_type;
    $this->entity_bundle = $entity_bundle;
    $this->entity_mapped_fields = $entity_mapped_fields;
  }

  // Extra (Getters and Setters).
  public function setEntityType($entity_type = NULL) {
    $this->entity_type = $entity_type;
  }

  public function getEntityType() {
    return $this->entity_type;
  }

  public function getEntity_bundle() {
    return $this->entity_bundle;
  }

  public function setEntity_bundle($entity_bundle) {
    $this->entity_bundle = $entity_bundle;
  }

  public function setEntityMappedFields($entity_mapped_fields = NULL) {
    $this->entity_mapped_fields = $entity_mapped_fields;
  }

  public function getEntityMappedFields() {
    return $this->entity_mapped_fields;
  }

  //---------------------------------------------------------------------------

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
   * THE RDF MAPPING! (entity reference)
   *
   * @return array
   */
  public function map() {
    $rdf_mapping = array();

    // Good Relations offering set up.
    $rdf_mapping = array_merge($rdf_mapping, array('rdf_type' => array('gr:Offering')));
    // The title does not appear a normal field.
    $rdf_mapping = array_merge($rdf_mapping, array('title' => array('predicates' => array('gr:name'))));
    foreach ($this->entity_mapped_fields as $id => $value) {
      $rdf_mapping = array_merge($rdf_mapping, array($id => array('predicates' => array($value))));
    }

    $product_mapping = array(
      'type' => $this->entity_type,
      'bundle' => $this->entity_bundle,
      'mapping' => $rdf_mapping,
    );

    return $product_mapping;
  }

}
