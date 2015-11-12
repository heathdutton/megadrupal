<?php
/**
 * @file
 * OAI parser interface.
 *
 * It defines the functions the OAI parsers should implement
 *
 * @author Király Péter <kirunews@gmail.com>
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

interface IOAIParser {

  /** Get the HTTP response code */
  public function getHttpCode();

  /** Has errors in OAI response? */
  public function hasErrors();

  /** Get all errors */
  public function listErrors();

  /** Has records? */
  public function hasRecords();

  /** Get number of records */
  public function getRecordCount();

  /** Get next record */
  public function getNextRecord();

  /** Has resumptionToken? */
  public function hasResumptionToken();

  /** Get the resumptionToken */
  public function getResumptionToken();

  /** Get the statistics */
  public function getStatistics();

  /** Get the errors regarding to XML malformadness */
  public function getParserErrors();
}
