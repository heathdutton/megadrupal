<?php

/**
 * @file
 * SmartQueue Interactive module AJAX callbacks.
 */

/**
 * Page callback to handle processing AJAX callback.
 */
function smartqueue_int_ajax_process() {
  $response = new stdClass();

  $p = new SmartQueueInteractiveProcessor();
  $response->complete = $p->process();

  return $response;
}
