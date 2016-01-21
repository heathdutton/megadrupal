<?php

/**
 * @file
 * Converts HTML tags into their corresponding XLIFF tags.
 */

class CloudwordsConverter {
  protected $elementMap = array(
    'b' => 'bold',
    'br' => 'lb',
    'caption' => 'caption',
    'fieldset' => 'groupbox',
    'footer' => 'footer',
    'form' => 'dialog',
    'frame' => 'frame',
    'head' => 'header',
    'i' => 'italic',
    'img' => 'image',
    'li' => 'listitem',
    'menu' => 'menu',
    'table' => 'table',
    'td' => 'cell',
    'tfoot' => 'footer',
    'tr' => 'row',
    'u' => 'underlined',
  );

  protected $inlineTags = array(
    'a' => TRUE,
    'abbr' => TRUE,
    'acronym' => TRUE,
    'address' => TRUE,
    'applet' => TRUE,
    'area' => TRUE,
    'audio' => TRUE,
    'b' => TRUE,
    'bdo' => TRUE,
    'big' => TRUE,
    'blink' => TRUE,
    'br' => TRUE,
    'button' => TRUE,
    'cite' => TRUE,
    'code' => TRUE,
    'command' => TRUE,
    'datalist' => TRUE,
    'del' => TRUE,
    'details' => TRUE,
    'dfn' => TRUE,
    'em' => TRUE,
    'embed' => TRUE,
    'face' => TRUE,
    // 'font' => TRUE,
    'i' => TRUE,
    'iframe' => TRUE,
    'img' => TRUE,
    'input' => TRUE,
    'ins' => TRUE,
    'kbd' => TRUE,
    'label' => TRUE,
    'legend' => TRUE,
    'link' => TRUE,
    'map' => TRUE,
    'mark' => TRUE,
    'meter' => TRUE,
    'nav' => TRUE,
    'nobr' => TRUE,
    'object' => TRUE,
    'optgroup' => TRUE,
    'option' => TRUE,
    'param' => TRUE,
    'q' => TRUE,
    'rb' => TRUE,
    'rbc' => TRUE,
    'rp' => TRUE,
    'rt' => TRUE,
    'rtc' => TRUE,
    'ruby' => TRUE,
    's' => TRUE,
    'samp' => TRUE,
    'select' => TRUE,
    'small' => TRUE,
    'source' => TRUE,
    'span' => TRUE,
    'spacer' => TRUE,
    'strike' => TRUE,
    'strong' => TRUE,
    'sub' => TRUE,
    'summary' => TRUE,
    'sup' => TRUE,
    'symbol' => TRUE,
    'textarea' => TRUE,
    'time' => TRUE,
    'tt' => TRUE,
    'u' => TRUE,
    'var' => TRUE,
    'wbr' => TRUE,
  );

  protected $selfClosingTags = array(
    'area' => TRUE,
    'base' => TRUE,
    'basefont' => TRUE,
    'br' => TRUE,
    'col' => TRUE,
    'frame' => TRUE,
    'hr' => TRUE,
    'img' => TRUE,
    'input' => TRUE,
    'link' => TRUE,
    'meta' => TRUE,
    'param' => TRUE,
  );

  protected $inTransUnit = FALSE;

  public function __construct($html, $langcode) {
    $this->doc = new DOMDocument();
    $this->doc->strictErrorChecking = FALSE;
    $this->langcode = $langcode;
    $error = $this->errorStart();

    // Setting meta below is a hack to get our DomDocument into utf-8. All other
    // methods tried didn't work.
    $success = $this->doc->loadHTML('<meta http-equiv="content-type" content="text/html; charset=utf-8"><div id="cloudwords-dont-ever-use-this-id">' . $html . '</div>');
    $this->errorStop($error);

    if (!$success) {
      throw new Exception('Invalid HTML');
    }
  }

  /**
   * Converts HTML to the corresponding XLIFF representation.
   *
   * @return string
   *   The source HTML converted to XLIFF.
   */
  public function toXLIFF($pretty_print = FALSE) {
    $this->doc->formatOutput = $pretty_print;

    // Do not use getElementById to comply with older versions of libxml.
    // getElementById doesn't work properly on libxml 2.7.6 (CentOS)
    $xpath = new DOMXPath($this->doc);
    $wrapper_div = $xpath->query("//*[@id='cloudwords-dont-ever-use-this-id']")->item(0);

    $out = $this->doc->createDocumentFragment();

    $domNodeList = array();
    for ($i = 0; $i < $wrapper_div->childNodes->length; ++$i) {
      $domNodeList[] = $wrapper_div->childNodes->item($i);
    }

    $this->sanitizeMixedDomNodeList($this->doc, $domNodeList);

    foreach($domNodeList as $domNode){
      if ($output = $this->convert($domNode)) {
        $out->appendChild($output);
      }
    }

    return $this->doc->saveXML($out);
  }

  /**
   * Test for mixed dom node types (DOMText and DOMElement)
   * Wraps sibling DOMText and DOMElement inline tags into single DOMElement with a text attribute
   *
   * @return string
   *   The source HTML converted to XLIFF.
   */  
  protected function sanitizeMixedDomNodeList($doc, &$list){

    $NewElement = $doc->createElement('text');
    
    $test = array(XML_ELEMENT_NODE=> FALSE, XML_TEXT_NODE=>FALSE);
    foreach($list as $node){
      $test[$node->nodeType] = TRUE;
    }

    //text only so exit
    if($test[XML_TEXT_NODE] == TRUE && $test[XML_ELEMENT_NODE] == FALSE){
      return;
    }

    //mixed group logic
    $newList = array();
    foreach($list as $k => $node){
  		if (array_key_exists($node->nodeName, $this->inlineTags)||
  		array_key_exists($node->nodeName, array('#text' => true))){
          unset($list[$k]);
          $NewElement->appendChild($node->cloneNode(true));
        if (!isset($node->nextSibling->nodeName)){
          $newList[$k] = $NewElement;
        }else if(array_key_exists($node->nextSibling->nodeName, $this->inlineTags)||
		array_key_exists($node->nextSibling->nodeName, array('#text' => true))){
          //do nothing
        }else{
          $newList[$k] = $NewElement;
        }
      }else{
        $newList[$k] = $node;
        $NewElement = $doc->createElement('text');
      }
    }
    $list = $newList;
  }
  
  protected function convert(DOMNode $node) {

    switch ($node->nodeType) {
      case XML_ELEMENT_NODE:
        return $this->convertElement($node);

      case XML_TEXT_NODE:
        //if (!trim($node->nodeValue)) {
        //  break;
        //}

        return $this->addText($node);

      // case XML_CDATA_SECTION_NODE:
      //   $out->appendChild($this->addText($node));
      //   return $out;

    }

    return FALSE;
  }

  protected function convertElement(DOMElement $element) {
    $translated_element = $this->convertElementTag($element);

    foreach ($this->xliffAttrs($element) as $attr) {
      $translated_element->setAttributeNode($attr);
    }

    //Correction for inline tags created as trans-units
    if ((array_key_exists($element->nodeName, $this->inlineTags)) && $translated_element->tagName == 'trans-unit'){
      $tmpEl = $element;
      $element = $this->doc->createElement('text');
      $translated_element = $this->convertElementTag($element);
      foreach ($this->xliffAttrs($element) as $attr) {
        $translated_element->setAttributeNode($attr);
      }
      $element->appendchild(clone $tmpEl);
    }

    if ($translated_element->tagName == 'trans-unit') {
      $out = $this->createSource();
      $translated_element->appendChild($out);
      $target = $this->createTarget();
      $translated_element->appendChild($target);
      $this->inTransUnit = TRUE;
    }
    else {
      $out = $translated_element;
    }

    if ($element->hasChildNodes()) {
      foreach ($element->childNodes as $child) {
        if ($converted = $this->convert(clone $child)) {
          $out->appendChild($converted);
          if ($out->tagName == 'source') {
            $target->appendChild(clone $converted);
          }
        }
      }
    }

    if ($translated_element->tagName == 'trans-unit') {
      $this->inTransUnit = FALSE;
    }

    return $translated_element;
  }

  protected function addText(DOMText $text) {
    // $text->nodeValue = htmlentities($text->nodeValue);
    if (!$this->inTransUnit) {
      $trans = $this->doc->createElement('trans-unit');
      $trans->setAttribute('id', uniqid('text-'));
      $source = $this->createSource('en');
      $target = $this->createTarget('fr');
      $trans->appendChild($source);
      $trans->appendChild($target);
      $source->appendChild($text);
      $target->appendChild(clone $text);
      return $trans;
    }

    return $text;
  }

  protected function convertElementTag(DOMElement $element) {

    if ($this->isBlockElement($element) && $this->hasBlockChild($element)) {
      return $this->doc->createElement('group');
    }
    if (isset($this->selfClosingTags[$element->tagName]) && $this->inTransUnit) {
      $out = $this->doc->createElement('x');
    }
    elseif (isset($this->inlineTags[$element->tagName]) && $this->inTransUnit) {
      $out = $this->doc->createElement('g');
    }
    else {
      $out = $this->doc->createElement('trans-unit');
    }
    $out->setAttribute('id', uniqid($element->tagName . '-'));
    return $out;
  }

  protected function isBlockElement(DOMElement $element) {
    return !isset($this->inlineTags[$element->tagName]);
  }

  protected function hasBlockChild(DOMElement $element) {

    if ($element->hasChildNodes()) {
      $filter = new CloudwordsDOMElementFilter(new CloudwordsRecursiveDOMIterator($element));
      $recursive = new RecursiveIteratorIterator($filter, RecursiveIteratorIterator::SELF_FIRST);

      foreach ($recursive as $element) {
        if ($this->isBlockElement($element)) {
          return TRUE;
        }
      }
    }

    return FALSE;
  }

  protected function xliffAttrs(DOMElement $element) {
    $attrs = array();

    if (isset($this->inlineTags[$element->tagName])) {
      $attrs[] = new DOMAttr('ctype', $this->mapHTMLTagToXLIFF($element));
    }
    else if($element->tagName != 'text'){
      $attrs[] = new DOMAttr('restype', $this->mapHTMLTagToXLIFF($element));
    }

    foreach ($element->attributes as $attr) {
      switch ($attr->name) {
        case 'style':
          $name = 'css-style';
          break;

        default:
          $name = 'html:' . $attr->name;
          break;
      }
      $attrs[] = new DOMAttr($name, _cloudwords_filter_xml_control_characters($attr->value));
    }

    return $attrs;
  }

  protected function createSource() {
    $element = $this->doc->createElement('source');
    $element->setAttribute('xml:lang', 'en');
    return $element;
  }

  protected function createTarget() {
    $element = $this->doc->createElement('target');
    $element->setAttribute('xml:lang', $this->langcode);
    return $element;
  }

  protected function mapHTMLTagToXLIFF(DOMElement $element) {
    if (isset($this->elementMap[$element->tagName])) {
      return $this->elementMap[$element->tagName];
    }

    return 'x-html-' . $element->tagName;
  }

  /**
   * Start custom error handling.
   *
   * @return bool
   *   The previous value of use_errors.
   */
  protected function errorStart() {
    return libxml_use_internal_errors(TRUE);
  }

  /**
   * Stop custom error handling.
   *
   * @param bool $use
   *   The previous value of use_errors.
   * @param bool $print
   *   (Optional) Whether to print errors to the screen. Defaults to TRUE.
   */
  protected function errorStop($use, $print = FALSE) {
    if ($print) {
      foreach (libxml_get_errors() as $error) {

        // Invalid tag. Skip this as DOMDocument does not supprt HTML5.
        if ($error->code == 801) {
          continue;
        }

        switch ($error->level) {
          case LIBXML_ERR_WARNING:
          case LIBXML_ERR_ERROR:
            $type = 'warning';
            break;
          case LIBXML_ERR_FATAL:
            throw new Exception('Fatal error');
            break;

        }
        $message = sprintf('%s on line %d. Error code: %d', trim($error->message), $error->line, $error->code);
        // drupal_set_message($message, $type, FALSE);
        print $message . "\n";
      }
    }
    libxml_clear_errors();
    libxml_use_internal_errors($use);
  }

}

class CloudwordsConverterToHTML extends CloudwordsConverter {
  public function __construct($xml) {
    $this->doc = new DOMDocument('1.0', 'UTF-8');
    $this->doc->strictErrorChecking = TRUE;
    $error = $this->errorStart();

    // Setting meta below is a hack to get our DomDocument into utf-8. All other
    // methods tried didn't work.
    $success = $this->doc->loadXML('<xliff version="1.2" xmlns:html="http://www.w3.org/1999/xhtml" xmlns="urn:oasis:names:tc:xliff:document:1.2" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="urn:oasis:names:tc:xliff:document:1.2 xliff-core-1.2-strict.xsd">' . $xml . '</xliff>');
    $this->errorStop($error);
    $this->elementMap = array_flip($this->elementMap);

    if (!$success) {
      throw new Exception('Invalid XML');
    }

    $this->xpath = new DOMXPath($this->doc);
    $this->xpath->registerNamespace('html', 'http://www.w3.org/1999/xhtml');
    $this->xpath->registerNamespace('xliff', 'urn:oasis:names:tc:xliff:document:1.2');
  }

  /**
   * Converts XML to the corresponding HTML representation.
   *
   * @return string
   *   The source XML converted to HTML.
   */
  public function toHTML($pretty_print = TRUE) {

    $this->out = new DOMDocument('1.0', 'UTF-8');
    $this->out->formatOutput = $pretty_print;
    $field = $this->doc->getElementsByTagName('group')->item(0);

    foreach ($field->childNodes as $child) {
      if ($output = $this->convert($child)) {
        $this->out->appendChild($output);
      }
    }

    return html_entity_decode($this->out->saveHTML(), ENT_QUOTES, 'UTF-8');
  }

  protected function convert(DOMNode $node) {

    if ($node->nodeType == XML_ELEMENT_NODE) {
      switch ($node->tagName) {
        case 'group':
          return $this->convertGroup($node);

        case 'trans-unit':
          return $this->convertTransUnit($node);

        case 'g':
        case 'x':
          return $this->convertXG($node);
      }
    }
    elseif ($node->nodeType == XML_TEXT_NODE) {
      if (trim($node->nodeValue)) {
        return $node->nodeValue;
      }
    }

    return FALSE;
  }

  protected function addChildren(DOMNode $node, DOMNode $elem) {
    foreach ($node->childNodes as $child) {
      if ($new_child = $this->convert($child)) {
        $elem->appendChild($new_child);
      }
    }
    return $elem;
  }

  protected function htmlTag(DOMElement $element) {
    switch ($element->tagName) {
      case 'group':
      case 'trans-unit':
        $attr = $element->getAttribute('restype');
        break;

      case 'g':
      case 'x':
        $attr = $element->getAttribute('ctype');
        break;

      default:
        // var_export( $element->nodeValue);
    }

    if (!$attr) {
      return $this->out->createDocumentFragment();
    }

    if (isset($this->elementMap[$attr])) {
      $html_element = $this->elementMap[$attr];
    }
    else {
      $html_element = substr($attr, 7);
    }

    $out = $this->out->createElement($html_element);
    $this->addAttrs($out, $element);
    return $out;
  }

  protected function convertGroup(DOMElement $node) {
    $elem = $this->htmlTag($node);
    return $this->addChildren($node, $elem);
  }

  protected function convertTransUnit(DOMElement $node) {
    $elem = $this->htmlTag($node);
    $target = $node->getElementsByTagName('target')->item(0);
    foreach ($target->childNodes as $child) {
      $elem->appendChild($this->convertTarget($child));
    }
    return $elem;
  }

  protected function convertTarget(DOMNode $node) {
    switch ($node->nodeType) {
      case XML_ELEMENT_NODE:
        $tag = $this->htmlTag($node);
        foreach ($node->childNodes as $child) {
          $tag->appendChild($this->convertTarget($child));
        }
        return $tag;

      case XML_TEXT_NODE:
        return $this->out->createTextNode($node->nodeValue);
    }

  }

  protected function convertXG(DOMElement $elem) {
    $html_element = substr($element->getAttribute('ctype')->nodeValue, 7);
    $out = $this->out->createElement($html_element);
    $this->addAttrs($out, $elem);
    return $this->addChildren($elem, $out);
  }

  protected function addAttrs($out, $elem) {
    foreach ($elem->attributes as $key => $attr) {
      if ($attr->prefix == 'html') {
        $out->setAttribute($key, $attr->nodeValue);
      }
      elseif ($key == 'css-style') {
        $out->setAttribute('style', $attr->nodeValue);
      }
    }
  }
}

/**
 * Iterates recursively over a DOMNodeList.
 */
class CloudwordsRecursiveDOMIterator implements RecursiveIterator {

  /**
   * Current Position in DOMNodeList.
   *
   *  @var integer
   */
  protected $position = 0;

  /**
   * The DOMNodeList with all children to iterate over.
   *
   * @var DOMNodeList
   */
  protected $nodeList;

  /**
   * Constructor.
   *
   * @param DOMNode $node
   *   A DOMNode to iterate over.
   */
  public function __construct(DOMNode $node) {
    $this->nodeList = $node->childNodes;
  }

  /**
   * Returns the current DOMNode.
   *
   * @return DOMNode
   */
  public function current() {
    return $this->nodeList->item($this->position);
  }

  /**
   * Returns an iterator for the current iterator entry.
   *
   * @return CloudwordsRecursiveDOMIterator
   */
  public function getChildren() {

    // Poor mans late static binding.
    $class = get_class($this);
    return new $class($this->current());
  }

  /**
   * Returns if an iterator can be created for the current entry.
   *
   * @return bool
   */
  public function hasChildren() {
    return $this->current()->hasChildNodes();
  }

  /**
   * Returns the current position.
   *
   * @return int
   */
  public function key() {
    return $this->position;
  }

  /**
   * Moves the current position to the next element.
   */
  public function next() {
    $this->position++;
  }

  /**
   * Rewinds the Iterator to the first element.
   */
  public function rewind() {
    $this->position = 0;
  }

  /**
   * Checks if current position is valid.
   *
   * @return bool
   */
  public function valid() {
    return $this->position < $this->nodeList->length;
  }

}

class CloudwordsDOMElementFilter extends RecursiveFilterIterator {

  public function accept() {
    return $this->current()->nodeType === XML_ELEMENT_NODE;
  }

}
