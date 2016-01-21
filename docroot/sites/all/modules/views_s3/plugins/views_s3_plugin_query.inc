<?php

/**
 * @file
 *   Query plugin implementation file
 */

/**
 * Class for implementing a simple Views query backend plugin, that uses our custom class
 * to execute requests to get information about S3 files.
 *
 * Possible improvement: implementing pager functionality.
 * See the EntityFieldQuery Views Backend module for an example:
 * http://drupalcode.org/project/efq_views.git/blob/refs/heads/7.x-1.x:/efq_views_plugin_query.inc
 */
class views_s3_plugin_query extends views_plugin_query {

  /**
   * The S3APIRequest object.
   */
  var $request;

  /**
   * Constructor; Create the basic request object.
   */
  function init($base_table, $base_field, $options) {
    parent::init($base_table, $base_field, $options);
    $this->request = new S3APIRequest();
    try {
      if (!empty($this->options['bucket'])) {
        $this->request->setParam('bucket_name', $this->options['bucket']);
      }
      if (!empty($this->options['lifetime'])) {
        $this->request->setParam('lifetime', $this->options['lifetime']);
      }
      if (_is_cloudfront_active()) {
        if (!empty($this->options['distribution'])) {
          $this->request->setParam('distribution', $this->options['distribution']);
        }
      }
    } catch (Exception $e) {
      drupal_set_message($e->getMessage(), 'error');
    }
  }

  /**
   * Builds the necessary info to execute the query.
   */
  function build(&$view) {
    $view->build_info['views_s3_request'] = $this->request;

    try {
      // Adding arguments to the request.
      if (isset($view->query->request_arguments)) {
        foreach ($view->query->request_arguments as $arg_key => $arg_value) {
          $view->build_info['views_s3_request']->addArgument($arg_key, $arg_value);
        }
      }
      else {
        $view->build_info['views_s3_request']->clearAllArguments();
      }
    } catch (Exception $e) {
      drupal_set_message($e->getMessage(), 'error');
    }
  }
  
  /**
   * Add settings for the ui.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    libraries_load('awssdk');
    libraries_load('awssdk-s3');
    $form['bucket'] = array(
      '#type' => 'select',
      '#options' => _get_bucket_list(),
      '#title' => t('Bucket'),
      '#default_value' => $this->options['bucket'],
      '#description' => t('Only files in this bucket will be listed.'),
    );
    if (_is_cloudfront_active()) {
      $form['distribution'] = array(
        '#type' => 'select',
        '#options' => _get_distribution_list(),
        '#title' => t('Distribution'),
        '#default_value' => $this->options['distribution'],
        '#description' => t('Select the distribution you want to use to sign CDN urls. NOTE: You need to specify key-pair name and *.pem content in AWS settings.'),
      );
    }
    $form['lifetime'] = array(
      '#type' => 'textfield',
      '#title' => t('Lifetime'),
      '#description' => t('Lifetime in seconds for signed URL.'),
      '#default_value' => isset($this->options['lifetime']) ? $this->options['lifetime'] : 7200,
    );
  }

  function options_submit(&$form, &$form_state) {
    $this->request->setParam('bucket_name', $form_state['values']['query']['options']['bucket']);
    if (_is_cloudfront_active()) {
      $this->request->setParam('distribution', $form_state['values']['query']['options']['distribution']);
    }
    $this->request->setParam('lifetime', $form_state['values']['query']['options']['lifetime']);
  }

  function summary_title() {
    return empty($this->options['bucket']) ? t('Settings') : t('Using bucket: %bucketname', array('%bucketname' => $this->options['bucket']));
  }

  /**
   * Add an ORDER BY clause to the query.
   *
   * @param $table
   *   The table this field is part of. If a formula, enter NULL.
   *   If you want to orderby random use "rand" as table and nothing else.
   * @param $field
   *   The field or formula to sort on. If already a field, enter NULL
   *   and put in the alias.
   * @param $order
   *   Either ASC or DESC.
   * @param $alias
   *   The alias to add the field as. In SQL, all fields in the order by
   *   must also be in the SELECT portion. If an $alias isn't specified
   *   one will be generated for from the $field; however, if the
   *   $field is a formula, this alias will likely fail.
   * @param $params
   *   Any params that should be passed through to the add_field.
   */
  function add_orderby($table, $field = NULL, $order = 'ASC', $alias = '', $params = array()) {
    // Only ensure the table if it's not the special random key.
    // @TODO: Take care of $table == 'rand'.
    
    // Only fill out this aliasing if there is a table;
    // otherwise we assume it is a formula.
    if (!$alias) {
      $as = $field;
    }
    else {
      $as = $alias;
    }
    $item = array(
      'field' => $as,
      'direction' => strtoupper($order)
    );
    $item = array_merge($item, $params);
    $this->orderby[] = $item;
    $this->request->addOrderCriterion($item);
  }

  /**
   * Executes the request and fills the associated view object with according
   * values.
   */
  function execute(&$view) {
    $request = $view->build_info['views_s3_request'];

    $start = microtime(true);

    $view->result = array();
    try {
      $view->result = $request->execute();
    } catch (Exception $e) {
      drupal_set_message($e->getMessage(), 'error');
    }

    $view->execute_time = microtime(true) - $start;
  }
}