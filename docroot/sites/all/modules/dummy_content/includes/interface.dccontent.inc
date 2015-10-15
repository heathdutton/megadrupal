<?php

/**
 * Basic content interface.
 */
interface dcContentInterface
{
  /**
   * Constructor. Sets the content subject.
   * 
   * @param $subject The content subject.
   */
  public function __construct($subject, $options = array(), $content_data = array());
  
  /**
   * Generates the content.
   *
   * @param string $quantity  The number of items to generate.
   * @param string $options   Additional options.
   */
  public function generate($quantity, $options = array());
}