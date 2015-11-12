<?php

/**
 * @file
 * Includes hooks for extending Commerce Marketplace Funds functionality.
 */

/**
 * Implements hook_commerce_marketplace_funds_processor_info().
 */
function hook_commerce_marketplace_funds_processor_info() {
  $processor = array();
  $processor['processor'] = array(
    'name' => t('Human readable name of the funds processor'),
    'description' => t('Description of the funds processor.'),
    // Processor class, must implement CommerceMarketplaceFundsProcessorInterface.
    'class' => 'SomeFundsProcessorClass',
    // Absolute path to the file containing the processor class.
    'file' => '/absolute/path/to/file/containing/class',
    // Include the next two functions in your main module file.
    // This function is called when the processor is set as default.
    'enable callback' => 'some_function_name',
    // This function is called when the processor is deselected as the default processor.
    'disable callback' => 'another_function_name',
  );
  return $processor;
}

/**
 * Implements hook_commerce_marketplace_funds_processor_info_alter().
 */
function hook_commerce_marketplace_funds_processor_info_alter(&$processors) {
  $processors['local_funds']['class'] = 'AnotherFundsProcessorClass';
}
