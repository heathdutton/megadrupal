<?php
class openlayer_layer_assistant_render_wms extends openlayer_layer_assistant_render {
  protected  $layerType;
  public $title;
  public $description;
  
  public function render(&$form){
    if(!array_key_exists('server_url', $form)){
      $form['server_url'] = array();
    }
    $form['server_url'] += array(
      '#title' => t('Base URL'),
      '#description' => t('URL for automatic configuration. Fill this out before proceeding with the field below with the WMS service\'s URI, a GetCapabilities URI or a sample request for a map image.'),
      '#type' => 'textfield',
      '#default_value' => ''
    );
    $form['load_layers'] = array(
      '#type' => 'submit',
      '#value' => t('Load Layers'),
      '#ajax' => array(
        'path' => 'admin/structure/openlayers/layers/assistant/render/callback/load_layers',
      ),
      // Container for layer select box that is filled by AJAX
      '#suffix' => '<div id="load_layers_replacement"></div>'
    );
    $requestedLayerFound = false;
    if(!empty($form['server_url']['#default_value'])){
      // Remove layer select box from form that was only there to transport the default value.
      // If it persists, it affects the form field order because the addition sequence of keys defines their order in PHP.
      $tempLayerSelectBox = $form['layer_name'];
      unset($form['layer_name']);
      $server_url = $form['server_url']['#default_value'];
      $getCapabilitiesXPath = $this->getGetCapabilitiesXPath($server_url);
      $form['layer_name'] = $tempLayerSelectBox;
      $form['layer_name'] += $this->getLayerSelectBox($getCapabilitiesXPath);
      if(count($form['layer_name']['#options'])===0){
        drupal_set_message(t('The server does not offer any compatible layers currently.'), 'error');
      }
      
      unset($form['load_layers']['#suffix']);
      $form['layer_name']['#prefix'] = '<div id="load_layers_replacement">';
      $form['layer_name']['#suffix'] = '</div>';
      
      $requestedLayerFound = array_key_exists($form['layer_name']['#default_value'], $form['layer_name']['#options']);
      if(!$requestedLayerFound){
        drupal_set_message(t('The requested layer is not available in a supported projection. You might wish to chose another layer instead.'), 'error');
      }
    }
    $form['fill_form'] = array(
      '#type' => 'button',
      '#value' => t('Fill out and overwrite forms below'),
      '#ajax' => array(
        'path' => 'admin/structure/openlayers/layers/assistant/render/callback/fill_form',
      ),
      // Disable button until user selected a layer
      '#disabled' => !array_key_exists('layer_name', $form) || count($form['layer_name']['#options'])===0
    );
    
    $this->mergeBasicFields($form);
    
    $this->mergeLayerTypeFields($form);
    
    // Hide fields that can be set automatically by reading GetCapabilities document and provide easier to understand hints
    $specificFieldsKey = get_class($this->layerType);
    $form[$specificFieldsKey]['base_url']['#type'] = 'hidden';
    $form['basics']['isBaseLayer']['#default_value'] = 'false';
    
    // Hide projection list because all available projection get selected automatically to avoid distracting the user.
    $form['basics']['projection']['#prefix'] = '<div style="display:none">';
    $form['basics']['projection']['#suffix'] = '</div>';
    
    $form[$specificFieldsKey]['params']['buffer']['#type'] = 'hidden';
    $form[$specificFieldsKey]['params']['ratio']['#type'] = 'hidden';
    $form['basics']['projection']['#description'] = t('Flattening operation to get the world to display on a screen. Choose EPSG:900913 if the layer should be used in combination with map providers such as Google, Bing or MapQuest.');
    $form[$specificFieldsKey]['options']['TRANSPARENT']['#type'] = 'hidden';
    $form[$specificFieldsKey]['options']['exceptions']['#type'] = 'hidden';
    $form[$specificFieldsKey]['options']['layers']['#type'] = 'hidden';
    $form[$specificFieldsKey]['options']['styles']['#type'] = 'hidden';
    $form[$specificFieldsKey]['options']['styles']['#prefix'] = '<div class="form-item-openlayers-layer-type-wms-options-styles">';
    $form[$specificFieldsKey]['options']['styles']['#suffix'] = '</div>';
    $form[$specificFieldsKey]['options']['styles']['#description'] = t('Select theme to use for styling the layer.');
    
    if($requestedLayerFound){
      $this->fillForm($getCapabilitiesXPath, $form['layer_name']['#default_value'], $form);
    }
    
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t("Create WMS Layer")
    );
    return $form;
  }
  
  /**
   * Downloads and parses the GetCapabilities document.
   * @param String $server_url
   * @return DomXPath Query object for the GetCapabilities document
   */
  private function getGetCapabilitiesXPath($server_url){
    if (empty($server_url)){
      throw new Exception(t('Server URL must not be empty.'));
    }
    // Merge given URI with parameters for GetCapabilities
    $urlParts = parse_url($server_url);
    if(array_key_exists('query', $urlParts)){
      $queryParameters = array();
      parse_str($urlParts['query'], $queryParameters);
      $queryParameters['SERVICE'] = 'WMS';
      $queryParameters['REQUEST'] = 'GetCapabilities';
      $queryStringAccumulator = array();
      foreach($queryParameters as $parameterName => $parameterValue) {
        $queryStringAccumulator[] = rawurlencode($parameterName).'='.rawurlencode($parameterValue);
      }
      $queryString = implode('&', $queryStringAccumulator);
    } else {
      $queryString = 'SERVICE=WMS&REQUEST=GetCapabilities';
    }
    $getCapabilitiesURI = 'http';
    if(array_key_exists('scheme', $urlParts)){
      $getCapabilitiesURI = $urlParts['scheme'];
    }
    $getCapabilitiesURI .= '://';
    if(array_key_exists('user', $urlParts)){
      $getCapabilitiesURI .= $urlParts['user'];
    }
    if(array_key_exists('pass', $urlParts)){
      $getCapabilitiesURI .= ':'.$urlParts['pass'];
    }
    if(array_key_exists('user', $urlParts)){
      $getCapabilitiesURI .= '@';
    }
    if(array_key_exists('host', $urlParts)){
      $getCapabilitiesURI .= $urlParts['host'];
    }
    if(array_key_exists('path', $urlParts)){
      $getCapabilitiesURI .= $urlParts['path'];
    }
    $getCapabilitiesURI .= '?'.$queryString;
      
    $getCapabilitiesDocument = new DomDocument();
    try {
      $loadedCapabilitiesDocument = @$getCapabilitiesDocument->load($getCapabilitiesURI);
    } catch(Exception $e){
      throw new Exception(t("Cannot load GetCapabilities document: !message", array(
        '!message' => $e->getMessage
      )));
    }
    if($loadedCapabilitiesDocument===FALSE){
      $libxmlError = libxml_get_last_error();
      throw new Exception(t("Cannot load GetCapabilities document: !message", array(
        '!message' => $libxmlError->message
      )));
    }
    $getCapabilitiesXPath = new DomXPath($getCapabilitiesDocument);
    $getCapabilitiesXPath->registerNamespace('wms', 'http://www.opengis.net/wms');
    $getCapabilitiesXPath->registerNamespace('xlink', 'http://www.w3.org/1999/xlink');
    return $getCapabilitiesXPath;
  }
  
  /**
   * Generates a select box with layer names
   * @return array Drupal form field
   */
  private function getLayerSelectBox($getCapabilitiesXPath){
    $layerNames = $getCapabilitiesXPath->query('//wms:Layer[wms:CRS['.$this->getXPathCrsPredicate().']] | //Layer[SRS['.$this->getXPathCrsPredicate().']]');
    $layerNamesArray = array();
    foreach($layerNames as $layerName) {
      $layerNameKey = $getCapabilitiesXPath->evaluate('string((wms:Name | Name))', $layerName);
      $layerTitle = $getCapabilitiesXPath->evaluate('string((wms:Title | Title))', $layerName);
      $layerNamesArray[$layerNameKey] = $layerTitle;
    }
    return array(
      '#name' => 'layer_name',
      '#type' => 'select',
      '#options' => $layerNamesArray,
      '#description' => t('Choose a layer from the service. Only layers usable within Drupal are shown.')
    );
  }
  
  /**
   * @return string Filter to limit text content to any of the supported projections.
   */
  private function getXPathCrsPredicate(){
    $conditions = array();
    foreach(openlayers_get_all_projections() as $projection){
      $conditions[] = 'text()="'.$projection->identifier.'"';
    }
    return implode(' or ', $conditions);
  }


  /**
   * Adjusts form default values to match data of GetCapabilities request.
   * @param DomXPath $getCapabilitiesXPath Queries on GetCapabilities document
   * @param String $selectedLayer
   * @param array $form Drupal form
   */
  private function fillForm(DomXPath $getCapabilitiesXPath, $selectedLayer, array &$form){
    $specificFieldsKey = get_class($this->layerType);
    
    // Each WMS service must define the base URI for request generation.
    // In practice only a few servers send correct URI so the the source of the GetCapabilities document is used instead.
    //$form[$specificFieldsKey]['base_url']['#default_value'] = $getCapabilitiesXPath->evaluate('string(/wms:WMS_Capabilities/wms:Service/wms:OnlineResource/@xlink:href | /WMT_MS_Capabilities/Service/OnlineResource/@xlink:href)');
    $getCapabilitiesURI = $getCapabilitiesXPath->evaluate('/*')->item(0)->ownerDocument->documentURI;
    $base_url = mb_eregi_replace('\?.*$', '', $getCapabilitiesURI).'?';
    $form[$specificFieldsKey]['base_url']['#default_value'] = $base_url;
    
    $selectedLayerNode = $getCapabilitiesXPath->query('//wms:Layer[wms:Name="'.$selectedLayer.'"] | //Layer[Name="'.$selectedLayer.'"]')->item(0);
    
    $supportsImageExceptions = $getCapabilitiesXPath->evaluate('boolean(/wms:WMS_Capabilities/wms:Capability/wms:Exception/wms:Format[text()="application/vnd.ogc.se_inimage"] | /WMT_MS_Capabilities/Capability/Exception/Format[text()="application/vnd.ogc.se_inimage"])');
    if($supportsImageExceptions){
      $exceptions = "application/vnd.ogc.se_inimage";
    } else {
      $exceptions = "application/vnd.ogc.se_xml";
    }
    $form[$specificFieldsKey]['options']['exceptions']['#default_value'] = $exceptions;
    
    $title = $getCapabilitiesXPath->evaluate('string(wms:Title | Title)', $selectedLayerNode);
    $form['basics']['title']['#default_value'] = $title;
    $description = $getCapabilitiesXPath->evaluate('string(wms:Abstract | Abstract)', $selectedLayerNode);
    $form['basics']['description']['#default_value'] = $description;
    
    $supportedCRS = $getCapabilitiesXPath->query('wms:CRS['.$this->getXPathCrsPredicate().']/text() | SRS['.$this->getXPathCrsPredicate().']/text()', $selectedLayerNode);
    $crsOptionsList = array();
    // Put projections into list but place EPSG:900913 first because it is probably the most used
    foreach($supportedCRS as $crs){
      if($crs->nodeValue==="EPSG:900913"){
        array_unshift($crsOptionsList, $crs->nodeValue);
      } else {
        $crsOptionsList[] = $crs->nodeValue;
      }
    }
    $crsOptions = array_combine($crsOptionsList, $crsOptionsList);
    $crsSelectBox = &$form['basics']['projection'];
    // Drupal ignores #multiple flag if not moved into #attributes
    $crsSelectBox['#attributes']['multiple'] = $crsSelectBox['#multiple'];
    $crsSelectBox['#name'] = 'projection[]';
    $crsSelectBox['#options'] = $crsOptions;
    $crsSelectBox['#value'] = array_values($crsOptions);
    
    $isBaseLayerRaw = $getCapabilitiesXPath->evaluate('string(@opaque)', $selectedLayerNode);
    if($isBaseLayerRaw===FALSE){
      throw new Exception(t('Attribute opaque not readable in layer description.'));
    }
    $isBaseLayer = !empty($isBaseLayerRaw);
    $form[$specificFieldsKey]['params']['isBaseLayer']['#default_value'] = $isBaseLayer;
    
    $styles = $getCapabilitiesXPath->query('wms:Style | Style', $selectedLayerNode);
    $styleOptions = array();
    foreach($styles as $style) {
      $styleName = $getCapabilitiesXPath->evaluate('string(wms:Name | Name)', $style);
      $styleTitle = $getCapabilitiesXPath->evaluate('string(wms:Title | Title)', $style);
      $styleOptions[$styleName] = $styleTitle;
    }
    $stylesSelectBox = &$form[$specificFieldsKey]['options']['styles'];
    $stylesSelectBox['#name'] = $specificFieldsKey.'[options][styles]';
    $stylesSelectBox['#type'] = 'select';
    $stylesSelectBox['#options'] = $styleOptions;
  }
  
  public function callback(array $input, $callbackArguments){
    $clickedButton = $callbackArguments[0];
    $specificFieldsKey = get_class($this->layerType);
    $server_url = $this->getFieldValue($input, array('server_url'));
    try {
      $getCapabilitiesXPath = $this->getGetCapabilitiesXPath($server_url);
    } catch (Exception $e){
      return array(
        ajax_command_alert($e->getMessage())
      );
    }
    
    switch ($clickedButton) {
      /**
       * Fills the layer list
       */
      case "load_layers":
        $select = $this->getLayerSelectBox($getCapabilitiesXPath);
        $ajaxCommands = array(
          ajax_command_html('#load_layers_replacement', drupal_render($select)),
        );
        if(count($select['#options'])===0){
          drupal_set_message(t('The server does not offer any compatible layers currently.'), 'error');
          $ajaxCommands[] = ajax_command_invoke('#edit-fill-form', 'attr', array('disabled', 'disabled'));
          $ajaxCommands[] = ajax_command_invoke('#edit-fill-form', 'addClass', array('form-button-disabled'));
        } else {
          $ajaxCommands[] = ajax_command_invoke('#edit-fill-form', 'removeAttr', array('disabled'));
          $ajaxCommands[] = ajax_command_invoke('#edit-fill-form', 'removeClass', array('form-button-disabled'));
        }
        
        return $ajaxCommands;
      /**
       * Populate form with assumed to be correct and sensible defaults based on GetCapabilities
       */
      case "fill_form":
        $selectedLayer = $this->getFieldValue($input, array('layer_name'));
        list($form, $form_state) = ajax_get_form();
        $this->fillForm($getCapabilitiesXPath, $selectedLayer, $form);
        
        $selectedLayerNode = $getCapabilitiesXPath->query('//wms:Layer[wms:Name="'.$selectedLayer.'"]')->item(0);

        $exceptions = $form[$specificFieldsKey]['options']['exceptions']['#default_value'];
        
        $title = $form['basics']['title']['#default_value'];
        $description = $form['basics']['description']['#default_value'];
        
        $crsSelectBox = $form['basics']['projection'];
        
        $isBaseLayer = $form['basics']['isBaseLayer'];
        
        $stylesSelectBox = &$form[$specificFieldsKey]['options']['styles'];
        
        return array(
          $this->getSetFieldValueCommand(array('title'), $title),
          $this->getSetFieldValueCommand(array('description'), $description),
          
          $this->getSetFieldValueCommand(array($specificFieldsKey, 'base_url'), $form[$specificFieldsKey]['base_url']['#default_value']),
          ajax_command_invoke('#edit-openlayers-layer-type-wms-params-isbaselayer', 'attr', array('checked', $isBaseLayer)),
          ajax_command_html('.form-item-projection', drupal_render($crsSelectBox)),
          $this->getSetFieldValueCommand(array($specificFieldsKey, 'options', 'TRANSPARENT'), !$isBaseLayer),
          $this->getSetFieldValueCommand(array($specificFieldsKey, 'options', 'exceptions'), $exceptions),
          ajax_command_html('.form-item-openlayers-layer-type-wms-options-styles', drupal_render($stylesSelectBox)),
        );
    }
  }
  
  public function beforeSave(&$form_state){
    $specificFieldsKey = get_class($this->layerType);
    
    // Assume all layers are transparent that are not base layers
    $isBaseLayer = $this->getFieldValue($form_state['input'], array('isBaseLayer'));
    $form_state['input']['isBaseLayer'] = ($isBaseLayer===true || $isBaseLayer==='1');
    $form_state['input'][$specificFieldsKey]['options']['TRANSPARENT'] = !$form_state['input']['isBaseLayer'];
    
    // Set the selected layer in the field where the OpenLayers module expects it
    $layers = $this->getFieldValue($form_state['input'], array('layer_name'));
    $form_state['input'][$specificFieldsKey]['options']['layers'] = explode("\r\n", $layers);
  }
}