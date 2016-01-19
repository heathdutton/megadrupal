<?php

namespace Drupal\aws_glacier;

use Drupal\aws_glacier\Entity\Archive\Archive;
use Aws\Glacier\Model\MultipartUpload\UploadPartGenerator;
use Guzzle\Http\EntityBody;
use Guzzle\Http\ReadLimitEntityBody;

/**
 * Class MultipartUpload
 * @package Drupal\aws_glacier
 */
class MultipartUpload extends Upload {

  /**
   * @var string
   */
  protected $partSize;

  /**
   * @param int $partSize
   * In MB. The size of each part except the last, in bytes. The last part can be smaller than this part size.
   *
   * @return $this
   */
  public function setPartSize($partSize) {
    $mb = DRUPAL_KILOBYTE * DRUPAL_KILOBYTE;
    $pow = pow(2, $partSize / $mb);
    $this->partSize = $pow * $mb;
    $this->setArgs(array('partSize' => (string) $this->partSize));
    return $this;
  }

  /**
   * @var string
   * The upload ID of the multipart upload.
   */
  protected $uploadId;

  /**
   * @param string $uploadId
   *
   * @return $this
   */
  public function setUploadId($uploadId) {
    $this->uploadId = $uploadId;
    $this->setArgs(array('uploadId' => $uploadId));
    return $this;
  }

  /**
   * @var string
   * Identifies the range of bytes in the assembled archive that will be uploaded in this part.
   * Amazon Glacier uses this information to assemble the archive in the proper sequence.
   * The format of this header follows RFC 2616. An example header is Content-Range:bytes 0-4194303/*.
   */
  protected $range;

  /**
   * @param string $range
   *
   * @return $this
   */
  public function setRange($range) {
    $this->range = $range;
    $this->setArgs(array('range' => $range));
    return $this;
  }

  /**
   * @var string
   * The SHA256 tree hash of the entire archive.
   * It is the tree hash of SHA256 tree hash of the individual parts.
   * If the value you specify in the request does not match the SHA256 tree hash
   * of the final assembled archive as computed by Amazon Glacier,
   * Amazon Glacier returns an error and the request fails.
   */
  protected $checksum;

  /**
   * @param string $checksum
   *
   * @return $this
   */
  public function setChecksum($checksum) {
    $this->checksum = $checksum;
    $this->setArgs(array('checksum' => $checksum));
    return $this;
  }

  /**
   * @var string
   * The total size, in bytes, of the entire archive.
   * This value should be the sum of all the sizes of the individual parts that you uploaded.
   */
  protected $archiveSize;

  /**
   * @param string $archiveSize
   *
   * @return $this
   */
  public function setArchiveSize($archiveSize) {
    $this->archiveSize = $archiveSize;
    $this->setArgs(array('archiveSize' => $archiveSize));
    return $this;
  }

  /**
   * {@inheritDoc}
   *
   * @return $this
   */
  function __construct() {
    $this->setPartSize($this->maxUploadSize);
    $this->setCommand('InitiateMultipartUpload');
    return $this;
  }



  /**
   * @param \Drupal\aws_glacier\Entity\Archive\Archive $archive
   * @param $item
   * @param \DrupalQueueInterface  $queue
   *
   * @return \Drupal\aws_glacier\MultipartUpload
   */
  static function generateParts(Archive $archive, $item, \DrupalQueueInterface $queue) {
    /** @var \Drupal\aws_glacier\MultipartUpload $MultiUpload */
    $MultiUpload = new static();
    /** @var \DrupalQueueInterface $multiqueue */
    $multiqueue = \DrupalQueue::get('aws_glacier_multiupload:' . $archive->file_id);
    if ($multiqueue->numberOfItems()) {
      $MultiUpload->uploadParts($multiqueue, $archive, $item, $queue);
      return $MultiUpload;
    }

    $MultiUpload->setVaultName($archive->vaultName)
      ->setPartSize($MultiUpload->partSize)
      ->setArchiveDescription($MultiUpload->getGeneratedArchiveDescription($archive));

    $data = $MultiUpload->run()->getData();
    if (empty($data['uploadId'])) {
      $queue->releaseItem($item);
      return $MultiUpload;
    }
    $MultiUpload->setUploadId($data['uploadId']);
    $content = file_get_contents($archive->file_uri);
    try{
      $generator = UploadPartGenerator::factory($content, (int) $MultiUpload->partSize);
    }
    catch (\Exception $e) {
      watchdog_exception('aws_glacier', $e);
      return $MultiUpload;
    }
    foreach ($generator as $part) {
      /** @var \Aws\Glacier\Model\MultipartUpload\UploadPart $part */
      $multiqueue->createItem(array(
        'Size' => $part->getSize(),
        'Offset' => $part->getOffset(),
        'uploadId' => $data['uploadId'],
        'range' => $part->getFormattedRange(),
        'checksum' => $generator->getRootChecksum(),
        'archiveSize' => $generator->getArchiveSize(),
      ));
    }
    return $MultiUpload;
  }

  /**
   * @param \DrupalQueueInterface $multiqueue
   * @param \Drupal\aws_glacier\Entity\Archive\Archive $archive
   * @param $item
   * @param \DrupalQueueInterface $queue
   */
  function uploadParts( \DrupalQueueInterface $multiqueue, Archive $archive, $item, \DrupalQueueInterface $queue) {
    $file_content = file_get_contents($archive->file_uri);
    $content = EntityBody::factory($file_content);
    $end = time() + 60;
    while (time() < $end && ($multiitem = $multiqueue->claimItem())) {
      $this->setCommand('UploadMultipartPart')
        ->setVaultName($archive->vaultName)
        ->setUploadId($multiitem->data['uploadId'])
        ->setBody(new ReadLimitEntityBody($content, $multiitem->data['Size'], $multiitem->data['Offset']))
        ->setRange($multiitem->data['range'])
        ->run();
      $this->setChecksum($multiitem->data['checksum']);
      $this->setArchiveSize($multiitem->data['archiveSize']);
      $data = $this->getData();
      if (!empty($data['checksum'])) {
        $multiqueue->deleteItem($multiitem);
      }
      else{
        $multiqueue->releaseItem($multiitem);
      }
    }
    if (!$multiqueue->numberOfItems()) {
      $this->setCommand('CompleteMultipartUpload')
        ->setVaultName($archive->vaultName)
        ->setUploadId($this->uploadId)
        ->setChecksum($this->checksum)
        ->setArchiveSize($this->archiveSize)
        ->run();
      $data = $this->getData();
      if (!empty($data['archiveId'])) {
        $archive->archiveId = $data['archiveId'];
        $archive->save();
        $queue->deleteItem($item);
        $multiqueue->deleteQueue();
      }
    }
  }
}
