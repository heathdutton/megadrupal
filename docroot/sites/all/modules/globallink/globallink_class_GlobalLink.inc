<?php

/**
 * @file
 * File contains class for GlobalLink.
 */

class GlobalLink {
  public $documentTicket;
  public $submissionTicket;
  public $submissionName;
  public $sourceLocale;
  public $targetLocale;
  public $sourceXML;
  public $targetTicket;
  public $targetXML;
  public $status;
  public $tptRowId;
  public $nid;
  public $vid;
  public $title;
  public $type;
  public $sourceFileName;
  public $dueDate;
  public $cancelled = FALSE;
  public $sourceDeleted = FALSE;
  public $otherObjectId;
  public $errorMessage;
  public $submissionInstructions;
  public $submitter;
  public $metadata;
}
