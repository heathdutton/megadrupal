<?php

/**
 * @file
 * Provides the parsing and mapping rules for consuming WSClient-supplied
 * data within Feeds.
 *
 * Code structure and some methods taken from feeds_xpath_parser.
 * The intent is for these plugins to be compatible with the xpathParser
 * UI, so you can use xpath to extract data if you don't want the WSDL fields.
 *
 * author dman dan@coders.co.nz
 * @version 2013-03
 */

/**
 * Class for parsing WSClient data structures.
 */
class FeedsWSClientParser extends FeedsParser {

  protected $wsclient = NULL;

  /**
   * feeds_tamper didn't like me shoe-horning an xpath sort of pattern into
   * the 'key' of the mapping sources - I can't use '/' as part of a key.
   * Boo.
   *
   * Make this configurable to see if I can get around that. Less elegant,
   * but less  likely to confuse URLs.
   * Originally it was like 'Team/stadium/Name' and XPathy
   * but now swapped the / to '|' and get 'Team|stadium|Name'.
   */
  protected $keyDivider = '|';

  /**
   * Shown on the Parser settings, choose how to handle the data we are given.
   *
   * @see FeedsConfigurable::configForm()
   */
  public function configForm(&$form_state) {
    $form = array();

    // Load context info.
    $importer = feeds_importer($this->id);
    $importer_config = $importer->getConfig();

    if (empty($source_config)) {
      $source_config = $this->getConfig();
    }

    // Show nothing on the parser settings form if allow_override is off.
    if (isset($source_config['allow_override']) &&
        !$source_config['allow_override'] &&
        empty($source_config['config'])) {
      return;
    }

    // Deal with the rules for parsing the response.
    // The response packet may be deep or structured, eg
    // OperationResponse > OperationResults > DataObject(s)
    // so need to select this also.
    // Prepare a way to present this selection in the UI.
    // The WSDL datatypes will tell us what to expect.
    //
    // Load the wsclient definitions.
    $service_operation = $importer_config['fetcher']['config']['service_operation'];
    list($service, $operation) = wsclient_feeds_get_service_operation($service_operation);

    $response_type = $operation['result']['type'];
    // Build an options list by recursing the response schema.
    $options = $this->buildResponseTypeOptions('', $response_type, $service->datatypes, $this->keyDivider, TRUE);

    $wsclient_url = url(WSCLIENT_UI_PATH . '/manage/' . $service->name . '/operation/' . $operation['label']);
    // The name 'context' is deliberately the same as that used by xpathparser,
    // And has the same syntax.
    $form['context'] = array(
      '#type' => 'select',
      '#title' => t('Result elements to parse'),
      '#options' => $options,
      '#default_value' => $this->config['context'],
      '#description' => t("
        <p>
        Response packets from a web service may be structured or deep.
        You need to select the right *context* element that will be
        used to create multiple items on import.
        EG, if the response packet looked like an RSS feed, you need
        to select the RSS/feed/item so that all 'items' are loaded.
        </p><p>
        Shown here is a rough representation of the data structure we expect
        back from the service (according to the
          <a href='!wsclient_ur'>web service description rules</a>).
        You want to <b>choose the array</b> (prefixed with [*])
        <b> of elements that contain your data</b>.
        </p>
      ", array('!wsclient_url' => $wsclient_url)),
    );

    return $form;
  }


  /**
   * Implements FeedsParser::parse().
   *
   * This parser does not do the mapping, it creates a data structure
   * that uses the SOURCE keys for the input fields, and puts data there.
   *
   * TODO - is it my job to iterate through a pager?
   * or should the fetcher. Probably the fetcher as it's a new request.
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {

    // I expect the result to be of type FeedsWSClientFetcherResult
    // Otherwise I have to unpack the XML myself - which is possible. Later.
    $expected_result_type = 'FeedsWSClientFetcherResult';
    if (!is_a($fetcher_result, $expected_result_type)) {
      trigger_error(t('%this_type can only parse input of type %result_type', array('%this_type' => get_class($this), '%result_type' => $expected_result_type)));
      return NULL;
    }
    // TODO - parse it and get some data anyway.
    // $data = new SimpleXMLElement($fetcher_result->getRaw());
    $data = $fetcher_result->getData();

    // The context is a config property of this, the parser.
    $config = $this->getConfig();

    // $source_config is settings that may change per-run (unused).
    $source_config = $source->getConfigFor($this);
    if (empty($source_config)) {
      $source_config = $this->getConfig();
    }

    $parser_result = new FeedsParserResult();
    $parser_result->title = "WSClient response";
    $parser_result->description = "WSClient response";
    $parser_result->link = "";

    // Get the items from the data that match the path pattern.
    $context_list = explode($this->keyDivider, $config['context']);
    array_shift($context_list);

    $items = $this->extractItemByPath($data, $context_list);

    // Usually $items is an array, but it's also valid for the web service
    // to return just one result. In those cases, the result is probably
    // an object. So make a list of one item out of that result.
    // Also if the response was a string or something, I guess that also
    // can be a list of one thing.
    if (!is_array($items)) {
      $items = array($items);
    }

    // Note that I don't DO the mapping during parsing, but
    // I do need to know what will be mapped.
    $mappings = $this->getOwnMappings();

    // I'll just extract everything so that later things can see it.
    foreach ($items as $item) {
      $parsed_item = array();
      foreach ($mappings as $mapping) {
        // Parse the item.
        $query_list = explode($this->keyDivider, $mapping['source']);
        $value = $this->extractItemByPath($item, $query_list);

        if (isset($value)) {
          // $target_field = $mapping['target'];
          // $parsed_item[$target_field] = $value;
          // It's not MY job to do the mapping!
          // I add the keyed data to the item using the source keys.
          // FeedsProcessor::map() does the mapping later.
          $parsed_item[$mapping['source']] = $value;
        }
      }
      if (!empty($parsed_item)) {
        $parser_result->items[] = $parsed_item;
      }
      else {
        watchdog('FeedsWSClientParser', 'Scanned an item but ended up with no data at all. This is probably invalid or unexpected.', array(), WATCHDOG_NOTICE);
      }
    }
    return $parser_result;
  }


  /**
   * Configurations that appear on each import page.
   *
   * Override parent::configForm().
   */
  public function sourceForm($source_config) {
    $form['help'] = array(
      '#markup' => '
        Settings for parsing rules are in the Parser:settings
      ',
    );

    // Dunno why, but the source form REQUIRES at least one
    // parameter to be set, else the FeedsConfigurable complains
    // about having NULL settings to merge with.
    $form['hidden'] = array(
      '#type' => 'hidden',
    );
    return $form;
  }


  /**
   * Define defaults.
   */
  public function sourceDefaults() {
    return array();
  }

  /**
   * Define defaults.
   */
  public function configDefaults() {
    return array(
      'context' => FALSE,
      'sources' => array(),
    );
  }


  /**
   * List the data sources we can anticipate. 
   * 
   * This appears as a list in the processor MAPPING configuration page.
   *
   * The wsclient config will be able to tell us what sort of data to
   * expect as a response.
   *
   * Override parent::getMappingSources().
   */
  public function getMappingSources() {

    $importer = feeds_importer($this->id);
    $fetcher = $importer->fetcher;
    // The fetcher may have a handle on the wsclient,
    // and thus know the expected response fields.
    $parser = $importer->parser;
    // The parser defines the extraction context, so it's required
    // to figure out what fields are valid inside that context.
    //
    // As long as the TYPE (plugin_key) of the FETCHER is a WSClientFetcher,
    // then the fetcher->config will define the wsclient info
    // and operation the we are reading.
    if (!is_a($fetcher, 'FeedsWSClientFetcher')) {
      drupal_set_message('Invalid use of FeedsWSClientParser. I can only map values for a WSClient');
      return NULL;
    }

    // Load the $service.
    list($service, $operation) = wsclient_feeds_get_service_operation($fetcher->config['service_operation']);

    // Based on the datatype the operation is expected to return,
    // we can deduce what fields will be present in the response,
    // and therefore what can be mapped from.
    $response_type = $operation['result']['type'];
    $datatypes = &$service->datatypes;

    // The context (parent_path) of this data query is usually selected
    // in the parser settings,given, so chop that off the beginning.
    // We only process the datatype of the response data selected by context.
    $context_list = explode($this->keyDivider, $parser->config['context']);
    array_shift($context_list);
    // This helper function finds that for us.
    $contextual_response_type = $this->extractResponseTypeWithinContext($context_list, $response_type, $datatypes);

    // Now we know the type of the data we expect to parse,
    // List all the fields within it, including deep recursion.
    $contextual_options = $this->buildResponseTypeOptions('', $contextual_response_type, $datatypes);

    return $contextual_options + parent::getMappingSources();
  }

  /**
   * Get the mappings that belong to us.
   * 
   * i.e. mappings that have paths known to us from inspecting the WSDL.
   *
   * @return array
   *   An array of mappings keyed source => target.
   */
  protected function getOwnMappings() {
    $importer_config = feeds_importer($this->id)->getConfig();
    return $this->filterMappings($importer_config['processor']['config']['mappings']);
  }

  /**
   * Filters mappings, returning the ones that belong to us.
   *
   * Checks the list of user-defined field mapping rules that the processor
   * wants to use against the WSDL provided list of fields that the parser
   * knows we can handle.
   *
   * Used by getOwnMappings().
   *
   * @param array $mappings
   *   A mapping array from a processor.
   *
   * @return array
   *   An array of mappings keyed source => target.
   */
  protected function filterMappings($mappings) {
    $our_mappings = array();
    $valid_mappings = $this->getMappingSources();
    foreach ($mappings as $mapping) {
      if (isset($valid_mappings[$mapping['source']])) {
        $our_mappings[] = $mapping;
      }
      // Else this is now an invalid mapping. Is this a problem?
    }
    return $our_mappings;
  }


  /**
   * Unpacks datatypes into a representation we can use as a form selector.
   *
   * Given a responseType, which is probably a structured datatype,
   * recurse into it to extract the labels.
   *
   * The keys it returns will probably look like XPath, this is intentional.
   * ...Though had to adjust the '/' keyDivider into something else.
   *
   * @param string $response_type
   * @param array $datatypes
   *   list of known datatypes for this service wsclient style.
   * @param Bool $compact 
   *   flag to use shortnames in the option labels, by
   *   chopping off the root of the tree and NOT using name and description
   *   values.
   */
  protected function buildResponseTypeOptions($response_id, $response_type, $datatypes, $prefix = '', $compact = FALSE) {
    $options = array();
    $id = $prefix . $response_id;
    $new_prefix = $id;

    $prefix_label = preg_replace('/' . preg_quote($this->keyDivider) . '/', ' / ', $prefix);

    if ($compact) {
      // Really scrunch up the selectors.
      // Hm. It looks better in the dropdown, but has less information.
      // $prefix_label = preg_replace('/[^\/ ]+/', ' ', $prefix_label) . ' ';
    }

    $label = $prefix_label . $response_id . " ($response_type)";

    // Treat lists special.
    if (preg_match('/^list\<(.*)\>$/', $response_type, $matches)) {
      // Strange notation, type="list<MyType>" means a list of those things.
      // @see wsclient_soap_type_mapper()
      // This becomes a numerically indexed array.
      // Present it in the form as a nested list.
      $response_type = $matches[1];

      // Not quite sure if this is right, (adding the responsetype here)
      // but it fits with the example I'm working today and produces the
      // XPath that makes sense.
      $new_prefix .= $this->keyDivider . $response_type;
      $id = $new_prefix;
      $label = $prefix_label . '[*] ' . $response_id . " (List of $response_type)";
    }

    if ($compact) {
      $options[$id] = $label;
    }
    else {
      // Feeds_UI adds descriptions to the Legend.
      $options[$id] = array('name' => $label, 'description' => $prefix . $response_id . " ($response_type)");
    }

    // Recurse if the nested type is another known complextype.
    if (isset($datatypes[$response_type]) && $struct_definition = $datatypes[$response_type]) {
      // Don't force the divider slash - we don't need it at the beginning
      // all the time.
      if (!empty($new_prefix) && $new_prefix != $this->keyDivider) {
        $new_prefix .= $this->keyDivider;
      }
      foreach ($struct_definition['property info'] as $child_item_id => $child_def) {
        $child_type = $child_def['type'];
        $options += $this->buildResponseTypeOptions($child_item_id, $child_type, $datatypes, $new_prefix, $compact);
      }
    }
    return $options;
  }

  /**
   * Extract XPath-defined data from an array data structure.
   * 
   * Given a data structure and an XPath-like key to its contents,
   * return the items that match that key.
   *
   * A recursive func, as it digs.
   *
   *
   *  Given a structure like
   *
   *  ShoppingCart[
   *               boxes[
   *                     box[
   *                         0 => Apples
   *                         1 => Oranges
   *                        ]
   *                     box[
   *                         0 => Grapes
   *                        ]
   *                    ]
   *              ]
   *
   *  then the search_path /ShoppingCart/boxes/box
   *  will return the array [Apples, Oranges, Grapes]
   *
   * @param array $data
   * @param string $path_list
   *   Similar to XPath.
   */
  protected function extractItemByPath($data, $path_list) {
    if (empty($path_list)) {
      return $data;
    }
    $key = array_shift($path_list);

    // TODO if $key == '*'.
    //
    if (is_array($data) && isset($data[$key])) {
      $found = $data[$key];
    }
    if (is_object($data) && isset($data->$key)) {
      $found = $data->$key;
    }
    if (!empty($found)) {
      // Keep digging.
      return $this->extractItemByPath($found, $path_list);
    }
  }

  /**
   * Find the expected datatype of an element.
   * 
   * Given a description of known service datatypes,
   * and a context to dig for, find the expected DATATYPE of the thing(s)
   * referred to at that context level.
   *
   * In the extractItemsUsingContext() example, the response type of that
   * contextual lookup is probably 'fruit' because each thing the lookup returns
   * is a fruit.
   *
   * The recursing has to happen again here because the schema works with
   * datatype IDs, while the context string works with field ids.
   * It's possible that the schema uses the same label for fieldname and
   * datatype, but not required.
   *
   * in
   *   $contactDetails->Address
   * 'Address' is the fieldname, that also corresponds with the datatype
   * 'Address'. The same, and a common convention.
   * However in
   *   $contactDetails->AltAddress
   * the datatype of AltAddress is 'Address'. Different from the field label.
   *
   * This means we have to revisit the schema, and find the TYPES of things
   * that the context fieldnames are identifying.
   *
   * @param array $context_list
   *   An array representing the path of IDs to travel down.
   * @param string $response_type
   * @param array $datatypes
   */
  protected function extractResponseTypeWithinContext($context_list, $response_type, &$datatypes) {
    // The $response_id is expected to be of type $response_type .
    if (empty($context_list)) {
      return $response_type;
    }

    if (preg_match('/^list\<(.*)\>$/', $response_type, $matches)) {
      $response_type = $matches[1];
      if (end($context_list) == $response_type) {
        return $response_type;
      }
    }

    $response_id = array_shift($context_list);

    $type_def = $datatypes[$response_type];
    $child_type_def_id = $type_def['property info'][$response_id]['type'];

    // Recurse to find any structs inside this one.
    return $this->extractResponseTypeWithinContext($context_list, $child_type_def_id, $datatypes);
  }

}
