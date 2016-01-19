<?php
/**
 * @file
 *
 * Provides the basic object definitions used by pickem scoring plugins.
 */

/**
 * Declare the interface 'pickem_scoring_engine_interface'.
 */
interface pickem_scoring_engine_interface {
  public function update_totals(&$totals=FALSE, &$row=FALSE);
  public function update_totals_final(&$totals=FALSE);
}
