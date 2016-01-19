<?php

/**
 * @file
 * A Feeds Fetcher plugin that uses wsclient to fetch data from a web service.
 *
 * author dman dan@coders.co.nz
 * @version 2013-03
 */

/**
 * Feeds plugin. Uses wsclient to fetch data from a web service.
 */
class FeedsWSClientFetcher extends FeedsFetcher {

  /**
   * List the fields we expect to find on the config form.
   * 
   * ...and therefore which values get saved as settings on the fetcher.
   *
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    return array(
      'service_operation' => FALSE,
      'operation_arguments' => array(),
      'raw' => FALSE,
    );
  }


  /**
   * Provides the configuration form.
   * 
   * Choose what Webservice operation to invoke
   * and what default parameters to send it.
   *
   * TODO - this ALMOST workd with AJAX to update the parameters dynamically,
   * but it gets a little screwed.
   *
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    // Retrieve a list of all services.
    $type_info = wsclient_get_types();
    if (empty($type_info)) {
      drupal_set_message(t('No service types were found, please enable a module that provides a service type.'), 'warning');
    }

    $all_wsclients = wsclient_feeds_fetch_instances();
    if (empty($all_wsclients)) {
      drupal_set_message(t('No web service descriptions were found. Please set that up first in the Web Services admin.'), 'warning');
    }
    $options = array();
    foreach ($all_wsclients as $wsclient) {
      $operation_options = array();
      foreach ($wsclient->operations as $operation_id => $operation) {
        $operation_options[$wsclient->name . ':' . $operation_id] = $operation['label'];
      }
      $options[$wsclient->label] = $operation_options;
    }

    $wsclient_url = url(WSCLIENT_UI_PATH . '/manage/');
    $form['service_help'] = array(
      '#markup' => t('
        To use the Web Service fetcher, you must first
        <a href="!wsclient_url">add and configure the Web Service Description</a>
        under [Administration : Configuration : Web Services].
        The rest of the parameters (arguments and data structure) will be
        deduced from there.
      ', array('!wsclient_url', $wsclient_url)),
    );

    $form['service_operation'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#title' => t('Operation'),
      '#default_value' => $this->config['service_operation'],

      // If this changes, update the form.
      // Something in feeds_form management screws with this.
      // Data doesn't save
      // because the 'object' being saved - the fetcher
      // has become detached from the overall rule (feedsSource)
      // @see configformsubmit for a hack-around.
      '#description' => "Please save once to refresh the form with new options if you change this (I can't get the AJAX to work here yet)",
      '##ajax' => array(
        'callback' => 'wsclient_feeds_operation_arguments_callback_form',
        'wrapper' => 'operation-arguments-wrapper',
        'method' => 'replace',
        'progress' => array(
          'type' => 'throbber',
          'message' => t('Checking parameters for this operation.'),
        ),
      ),
    );

    // Now display the appropriate parameters.
    // Use the submitted form if possible, otherwise use the last saved val.
    // This element is used by the ajax callback.
    //
    // If the value has just been changed in the form selector, use that
    // in the next bit of logic.
    $service_operation = isset($form_state['values']['service_operation']) ?
      $form_state['values']['service_operation'] :
      $this->config['service_operation'];
    $this->config['service_operation'] = $service_operation;

    $form['result_context'] = array(
      '#markup' => '
        Additional rules for how to interpret the response recieved from this
        operation are configured in the "Parser" config screen.
      ',
    );

    $this->sourceForm($this->config);

    $form['operation_arguments'] = $this->sourceForm($this->config);

    // Add the placeholder div that the AJAX replace will target.
    $form['operation_arguments']['#title'] = 'Parameters sent to the web service';
    $form['operation_arguments']['#prefix'] = '<div id="operation-arguments-wrapper">';
    $form['operation_arguments']['#suffix'] = '</div>';
    $form['operation_arguments']['help'] = array(
      '#type' => 'markup',
      '#markup' => "Request parameters can also be changed when importing",
    );

    $form['raw'] = array(
      '#type' => 'checkbox',
      '#title' => t('Retain raw XML'),
      '#default_value' => isset($this->config['raw']) ? $this->config['raw'] : FALSE,
      '#description' => t('
        If you want to parse with another consumer, such as XPathParser,
        enable this and the unparsed XML will be available to that module.
      '),
    );
    return $form;
  }

  /**
   * Capture a settings change.
   *
   * TODO This still doesn't behaves as expected. Data captured by an AJAX
   * refresh doesn't get saved right.
   */
  public function configFormSubmit(&$values) {

    // For unknown magic reasons, possibly caching, this object is no longer
    // associated with its parent container.
    // We need to squish our data into that object so when the parent saves
    // it saves right.
    feeds_importer($this->id)->fetcher->config = $this->config;

    // This was HORRIBLE to find, and still makes no sense.
    // It's not needed if not using AJAX, but is if you are.
    parent::configFormSubmit($values);
  }


  /**
   * Actually call the service now, with the given params.
   * 
   * Sort out the response.
   *
   * Implementation of FeedsFetcher::fetch().
   */
  public function fetch(FeedsSource $source) {
    // The user can use the config form
    // - where they choose the endpoint,
    // - Which doesn't change between invocations,
    // and the 'source' form,
    // - where they choose the parameters
    // - which may change between runs.
    $config = $this->config;
    $source_config = $source->getConfigFor($this);
    // Merge the arguments.
    $operation_arguments = (array) $source_config['operation_arguments'] + (array) $config['operation_arguments'];

    $service_operation = $config['service_operation'];
    list($service, $operation) = wsclient_feeds_get_service_operation($service_operation);

    // The WSDL may have told me about the data that will come back,
    // Eg is it a value, a data structure or a list of results.
    $expected_response_type_id = $operation['result']['type'];

    // Eg $expected_response_type_id = 'ListIndividualsResponse';
    $expected_response_datatype = $service->datatypes[$expected_response_type_id];

    if (empty($expected_response_datatype)) {
      drupal_set_message("I'm confused, I though I expected a '$expected_response_type_id' in response to the request, but the service seems to not know what this datatype is. ", 'error');
    }

    // It looks like WSDLs like to wrap their results in response packets
    // a lot.
    // The actual return value may be inside that. Hm
    // May need to dig until we find a list, eg the
    // ListIndividualsResponse > ListIndividualsResult 
    // > IndividualList > Individuals[n] .
    // That's a problem for the Parser.
    try {

      // If we need to capture the raw XML, the process has more steps.
      if (!$config['raw']) {
        // Fetch it the easy way. wsclient service does all the work.
        $response = $service->invoke($operation['label'], $operation_arguments);
        // That's all there is to it!
        $feeds_result = new FeedsWSClientFetcherResult($response);
        $feeds_result->setData($response);
      }

      else {
        // Extracting the raw data from inside the wsclient is a bit awkward,
        // so it's an optional feature, off by default.
        // If I want debug or raw data, then I must set
        // $service->settings['options']
        $service->settings['options']['trace'] = TRUE;
        // Which get passed in to the SOAPClient.
        $response = $service->invoke($operation['label'], $operation_arguments);
        // This normally returns a parsed object (not exactly 'raw'),
        /*
          To conform with other Feeds processors, set the 'raw' value as the
          reponse string, even though we internally have the parsed data object
          already.
        */
        
        /*
        ## Invoking the wsclient service means that
        ## WSClientServiceDescription runs ->endpoint()->call
        ## where ->endpoint() returns a WSClientSOAPEndpoint
        ## which wraps a SOAP object that it calls ->client.
        ## which is the thing that does the work.
        ## $service->endpoint()->client()
        ## When you invoke() the service, you eventually call()
        ## that $client. The soapCall just returns 'response', already parsed,
        ## not raw.
        ##    O_o
        */

        $client = $service->endpoint()->client();
        $raw_response = $client->__getLastResponse();

        // Creating the FeedsFetcherResult with the data to the __construct call
        // just sets the raw data.
        $feeds_result = new FeedsWSClientFetcherResult($raw_response);
        // setData() is our extension.
        $feeds_result->setData($response);
      }

      return $feeds_result;
    }
    catch (SoapFault $fault) {
      drupal_set_message("Web Service Client Fault: (faultcode: {$fault->faultcode}, faultstring: {$fault->faultstring})", 'error');
      return;
    }
  }


  /**
   * Source form.
   *
   * Shows all of the arguments this operation expects, Used on the
   * fetcher config form, and also shown on per-import pages.
   *
   * @return array
   *   FAPI form array.
   */
  public function sourceForm($source_config) {
    $form = array();
    $config = $this->getConfig();
    list($service, $operation) = wsclient_feeds_get_service_operation($this->config['service_operation']);

    if (empty($operation)) {
      return $form;
    }

    $form['operation_arguments']['#tree'] = TRUE;

    module_load_include('inc', 'wsclient_tester', 'wsclient_tester');
    $datatypes = $service->datatypes;

    // Steal the data entry form from wsclient_tester, as it's clever.
    module_load_include('inc', 'wsclient_tester', 'wsclient_tester');
    // Check it loaded (that module may still be in the dev version only today)
    // TODO require wsclient > 7.x-1.0-beta2
    if (function_exists('wsclient_tester_data_entry_form')) {
      foreach ((array) $operation['parameter'] as $param => $info) {
        $form['operation_arguments'][$param] = wsclient_tester_data_entry_form($param, $info['type'], @$config['operation_arguments'][$param], $datatypes);
      }
    }

    // This notice doesn't show if the operation takes *useless* parameters,
    // such as an array of nothing - which some WSDLs seem to do,
    if (empty($operation['parameter'])) {
      $form['operation_arguments']['#description'] = t('
        This operation takes no parameters.
        To set global parameters that apply to all operations, see the
        wsclient settings.
      ');
    }

    /*
      What PART of the result they are interested in is also selectable,
      though that is defined in the PARSER settings.
      The WSDL will give us a data structure describing valid values for 
      that also.
    */
    return $form;
  }

}


/**
 * A slightly extended result of FeedsHTTPFetcher::fetch().
 *
 * Most Feeds processors expect the ->raw data returned by
 * fetchers to be strings.
 * When using wsclient, it's already parsed object structures once
 * we've fetched it using wsclient invoke().
 *
 * For conformity, the $result->raw will be the raw string, but we'll
 * internally keep a handle on the $result->data also.
 *
 * This will allow you to swap out the Parser with XPathParser or others if
 * you so desired.
 */
class FeedsWSClientFetcherResult extends FeedsFetcherResult {
  protected $data;

  /**
   * Set the data array.
   */
  public function setData($data) {
    $this->data = $data;
  }
  /**
   * Return the data array.
   */
  public function getData() {
    return $this->data;
  }
}
