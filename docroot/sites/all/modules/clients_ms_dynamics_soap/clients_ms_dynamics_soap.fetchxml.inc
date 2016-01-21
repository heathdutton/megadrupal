<?php

/**
 * MS Dynamics SOAP FetchXML builder class.
 *
 */
class MSDynamicsSoapFetchXMLBuilder {

  /**
   * The DOMDocument for the FetchXML query.
   */
  public $dom;

  /**
   * Constructor
   */
  function __construct($entity_type, $remote_entity_type) {
    $this->entity_type = $entity_type;
    $this->remote_entity_type = $remote_entity_type;

    $this->dom = new DOMDocument();
    // We need this because otherwise linebreaks count as text nodes! WTF!
    // It also must be set before we load the XML. Double-WTF!
    $this->dom->preserveWhiteSpace = FALSE;
    $this->dom->formatOutput = TRUE;
  }

  /**
   * Set the initial FetchXML.
   *
   * This will typically have come from a marketing list.
   *
   * @param $fetch_xml
   *  A FetchXML string.
   */
  public function setFetchXML($fetch_xml) {
    $this->dom->loadXML($fetch_xml);

    // Set the root element, as we need this frequently.
    $this->root = $this->dom->documentElement;
  }

  /**
   * Retrieve the FetchXML to place into the query.
   *
   * @return
   *  The FetchXML string.
   */
  public function getFetchXML() {
    // Need to specify the root element, as otherwide DOMDocument adds an XML
    // header which we don't want.
    $xml = $this->dom->saveXML($this->root);
    return $xml;
  }

  /**
   * Set the pager on the query.
   *
   * @param $pager
   *  The pager array from MSDynamicsSoapSelectFetchExpressionQuery.
   */
  public function setPager($pager) {
    // Set defaults.
    $pager += array(
      'page' => 0,
      'paging_cookie' => '',
    );

    $this->root->setAttribute("paging-cookie", $pager['paging_cookie']);
    $this->root->setAttribute("page", $pager['page']);
    $this->root->setAttribute("count", $pager['limit']);
  }

  /**
   * Set the attributes on the query.
   *
   * The attributes are the fields to retrieve. The FetchXML query may not have
   * all the remote properties that are defined to the entity type.
   */
  public function setQueryAttributes() {
    $entity = $this->dom->getElementsByTagName('entity')->item(0);

    // First clean up: remove all the attributes.
    // WTF, PHP? We can't remove elements when iterating over them in a node
    // list! Have to do a stupid 2-pass approach.
    $attribute_elements = $this->dom->getElementsByTagName("attribute");
    $elements_to_remove = array();
    foreach ($attribute_elements as $attribute_element) {
      $elements_to_remove[] = $attribute_element;
    }
    foreach ($elements_to_remove as $attribute_element) {
      $entity->removeChild($attribute_element);
    }

    // Now add the attributes: we want everything defined in the property map.
    $fields = array();
    $entity_info = entity_get_info($this->entity_type);
    foreach ($entity_info['property map'] as $field_name) {
      $fields[] = $field_name;
    }

    // Reverse the array, as each successive item is inserted in first place,
    // which overall puts them in backwards.
    foreach (array_reverse($fields) as $field_name) {
      $new_attribute_node = $this->dom->createElement('attribute');
      $domAttribute = $this->dom->createAttribute('name');
      $domAttribute->value = strip_tags($field_name);
      $new_attribute_node->appendChild($domAttribute);
      // WTF PHP??
      $entity->insertBefore($new_attribute_node, $entity->firstChild);
    }
  }

}
