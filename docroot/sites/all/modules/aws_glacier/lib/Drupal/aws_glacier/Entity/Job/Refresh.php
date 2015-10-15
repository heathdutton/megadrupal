<?php


namespace Drupal\aws_glacier\Entity\Job;

use Drupal\aws_glacier\Command;

/**
 * Class Refresh
 * @package Drupal\aws_glacier\Entity\Job
 */
class Refresh extends Command{

  /**
   * @var string
   * The name of the vault.
   */
  protected $vaultName;

  /**
   * Sets the required vault name for the command.
   *
   * @param string $vaultName
   *
   * @return $this
   */
  public function setVaultName($vaultName) {
    $this->vaultName = $vaultName;
    $this->setArgs(array('vaultName' => $vaultName));
    return $this;
  }

  /**
   * @var string
   * An opaque string used for pagination. This value specifies the job
   * at which the listing of jobs should begin. Get the marker value
   * from a previous List Jobs response. You need only include the marker if you
   * are continuing the pagination of results started in a previous List Jobs request.
   */
  protected $marker;

  /**
   * @param string $marker
   *
   * @return $this
   */
  private function setMarker($marker) {
    $this->marker = $marker;
    $this->setArgs(array('marker' => $marker));
    return $this;
  }

  /**
   * Has a marker. Means the Vault list returns more than 1000 vaults.
   * @return bool
   */
  private function hasMarker() {
    return !empty($this->marker) ? TRUE : FALSE;
  }

  /**
   * @var string
   * Specifies that the response be limited to the specified number of items or fewer. If not specified, the List Jobs operation returns up to 1,000 jobs.
   */
  protected $limit;

  /**
   * Sets the response limit.
   *
   * @param string $limit
   *
   * @return $this
   */
  public function setLimit($limit) {
    if ((int) $limit > 0) {
      $this->limit = $limit;
    }
    $this->setArgs(array('limit' => $limit));
    return $this;
  }

  /**
   * @return string
   */
  public function getLimit() {
    return $this->limit;
  }

  /**
   * @return bool
   */
  public function hasLimit() {
    return !empty($this->limit) ? TRUE : FALSE;
  }

  /**
   * @var string
   * Specifies the type of job status to return. You can specify the following values: "InProgress", "Succeeded", or "Failed".
   */
  protected $statuscode;

  /**
   * @param string $statuscode
   *
   * @return $this
   */
  public function setStatuscode($statuscode) {
    $this->statuscode = $statuscode;
    $this->setArgs(array('statuscode' => $statuscode));
    return $this;
  }

  /**
   * @return string
   */
  public function getStatuscode() {
    return $this->statuscode;
  }

  /**
   * @var string
   * Specifies the state of the jobs to return. You can specify true or false.
   */
  protected $completed;

  /**
   * @param string $completed
   *
   * @return $this
   */
  public function setCompleted($completed) {
    $this->completed = $completed;
    $this->setArgs(array('completed' => $completed));
    return $this;
  }

  /**
   * @return string
   */
  public function getCompleted() {
    return $this->completed;
  }

  /**
   * @var array
   * List of jobs.
   * @link http://docs.aws.amazon.com/aws-sdk-php/latest/class-Aws.Glacier.GlacierClient.html#_listJobs @endlink
   */
  private $JobList = array();

  /**
   * {@inheritDoc}
   * @return $this
   */
  function __construct() {
    parent::__construct('ListJobs');
    return $this;
  }

  /**
   * {@inheritDoc}
   * @return $this
   */
  public function run() {
    parent::run();
    $this->batchRefresh();
    return $this;
  }

  /**
   * @return $this
   */
  public function batchRefresh() {
    $data = $this->getData();
    $this->setMarker($data['Marker']);
    if (!empty($data['JobList'])) {
      $this->JobList = $data['JobList'];
    }
    $operations = $this->getOperations();
    if (!empty($operations)) {
      $batch = array(
        'title' => t('Refreshing'),
        'operations' => $operations,
        'finished' => 'aws_glacier_ui_vault_import_finished',
      );
      batch_set($batch);
      batch_process('<front>');
    }
    return $this;
  }

  /**
   * Returns an array of batch operations.
   *
   * @return array
   */
  private function getOperations() {
    $operations = array();
    foreach ($this->JobList as $data) {
      $type = 'archive-retrieval';
      if ($data['Action'] == 'InventoryRetrieval') {
        $type = 'inventory-retrieval';
      }
      /** @var Job $Job */
      $Job = entity_create('glacier_job', array(
        'jobId' => $data['JobId'],
        'vaultName' => $this->vaultName,
        'Format' => !empty($data['InventoryRetrievalParameters']) ? $data['InventoryRetrievalParameters']['Format'] : 'JSON',
        'Type' => $type,
        'ArchiveId' => $data['ArchiveId'],
      ));
      $Job = $Job->loadByUniqueProperty();
      $Job->setMetadata($data);
      $operations[] = array('\Drupal\aws_glacier\Entity\Job\Refresh::jobRefresh', array($Job));
    }
    if ($this->hasMarker() && !$this->hasLimit()) {
      $this->run();
    }
    return $operations;
  }

  /**
   * @param \Drupal\aws_glacier\Entity\Job\Job $job
   * @param $context
   */
  static public function jobRefresh(Job $job, &$context) {
    #dpm($job, 'pre' . __METHOD__);
    $job->save();
   # dpm($job, 'post' . __METHOD__);
    $context['results'][] = $job;
    $context['message'] = check_plain($job->jobId);
  }
}
