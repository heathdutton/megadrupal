<?php
/**
 * Provides a DOM to work on HTML fragments.
 * 
 * Warning: PHP's DOM does work like an XML DOM and the HTML is adjusted so that is appears well-
 *     formed according to XML 1.0. Since this uses PHP's DOM internally, HTML specific DOM
 *     extensions are not available.
 */
class openlayers_layer_assistant_html_fragment {
    /**
     * @var DomDocument DOM that contains the HTML fragment
     */
    private $document = NULL;
    
    /** @var DOMXPath */
    private $xpath = NULL;
    
    /**
     * @param type $htmlFragment Raw HTML fragment
     */
    public function __construct($htmlFragment){
        $this->document = new DOMDocument('1.0', 'UTF-8');
        $this->document->loadHTML(
            '<html><head>'
                // Give enloading as meta tag otherwise PHP falsely assumes ISO-8859-1
                .'<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">'
            .'</head><body><div>'
            .$htmlFragment
            .'</div></body></html>'
        );
        $this->xpath = new DOMXPath($this->document);
    }
    
    /**
     * @return DOMElement The div element which contains the HTML fragment
     */
    private function getContentRoot(){
        $body = $this->document->getElementsByTagName('body')->item(0);
        return $body->getElementsByTagName('div')->item(0);
    }
    
    /**
     * Queries the DOM with a XPath
     * @param string $xpath XPath expression
     * @param DOMNode $context Base for relative xpaths. Defaults to fragment nodes' parent.
     * @return DOMNodeList References to matched elements. Changes made with this references will be
     *     reflected in the DOM.
     */
    public function query($xpath, $context = NULL){
        if($context===NULL){
            $context = $this->getContentRoot();
        }
        return $this->xpath->query($xpath, $context);
    }
    
    /**
     * Gets DOM content.
     * @return DOMNodeList List containing nodes of HTML fragment.
     */
    public function getFragmentList(){
        $root = $this->getContentRoot();
        return $root->childNodes;
    }
    
    /**
     * Gets DOM content.
     * @return string HTML fragment for DOM contents.
     */
    public function getHtml(){
        // Take nodes out of root into fragment in order to prevent serialization of root node
        //     itself; only its contents shall be serialized
        $fragment = $this->document->createDocumentFragment();
        $root = $this->getContentRoot();
        while($root->hasChildNodes()){
            $fragment->appendChild($root->firstChild);
        }
        
        $dirty = $this->getNodeHtml($fragment);
        
        // Put contents of root node back
        $root->appendChild($fragment);
        
        return $dirty;
    }
    
    /**
     * Gets partial DOM content.
     * @param DOMNode $fragment Node to serialize
     * @return string HTML fragment for node
     */
    public function getNodeHtml($fragment){
        $dirty = $this->document->saveXML($fragment, LIBXML_NOEMPTYTAG);
        // Remove those end tags that must not be present in HTML
        foreach(array('img', 'br', 'input') as $unclosableTagName){
            $dirty = mb_ereg_replace('</'.$unclosableTagName.'>', '', $dirty);
        }
        return $dirty;
    }    
}