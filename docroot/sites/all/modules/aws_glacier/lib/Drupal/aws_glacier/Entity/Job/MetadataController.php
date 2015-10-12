<?php
/**
 * @file
 * Defines \Drupal\aws_glacier\Entity\Job\MetadataController
 */
namespace Drupal\aws_glacier\Entity\Job;

/**
 * Class MetadataController
 * @package Drupal\aws_glacier\Entity\Job
 */
class MetadataController extends \EntityDefaultMetadataController {
  /**
   * {@inheritDoc}
   */
  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];
    $properties['jobId'] = array(
      'label' => t('Job Id'),
      'description' => t('The external unique JobId of a job.'),
      'setter permission' => Job::$permission,
      'setter callback' => 'entity_property_verbatim_set',
      'schema field' => 'jobId',
    );
    $properties['Format'] = array(
      'label' => t('Format'),
      'description' => t('When initiating a job to retrieve a vault inventory, you can optionally add this parameter to your request to specify the output format. If you are initiating an inventory job and do not specify a Format field, JSON is the default format. Valid Values are "CSV" and "JSON".'),
      'setter permission' => Job::$permission,
      'setter callback' => 'entity_property_verbatim_set',
      'schema field' => 'Format',
    );
    $properties['Type'] = array(
      'label' => t('Type'),
      'description' => t('The job type. You can initiate a job to retrieve an archive or get an inventory of a vault. Valid Values are "archive-retrieval" and "inventory-retrieval".'),
      'setter permission' => Job::$permission,
      'setter callback' => 'entity_property_verbatim_set',
      'schema field' => 'Type',
    );
    $properties['Description'] = array(
      'label' => t('Description'),
      'description' => t('The optional description for the job. The description must be less than or equal to 1,024 bytes. The allowable characters are 7-bit ASCII without control codes-specifically, ASCII values 32-126 decimal or 0x20-0x7E hexadecimal.'),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => Job::$permission,
      'schema field' => 'Description',
    );
    return $info;
  }
}
