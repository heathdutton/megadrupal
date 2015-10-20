<?php

/**
 * @file
 * Handle links mapping operation.
 */

namespace Drupal\maps_links\Mapping\Mapper;

use Drupal\maps_import\Profile\Profile;
use Drupal\maps_import\Fetcher\Fetcher;
use Drupal\maps_import\Mapping\Mapper\Mapper;
use Drupal\maps_import\Mapping\Mapping;
use Drupal\maps_links\Mapping\Link as LinkMapping;
use Drupal\maps_links\Mapping\Source\MapsSystem\Link as MapsSystemLink;
use Drupal\maps_suite\Log\Logger;
use Drupal\maps_suite\Log\Context\Context as LogContext;

/**
 * Base class for all mapping operations.
 */
class Link extends Mapper {

  /**
   * @inheritdoc
   */
  public function getTitle() {
    return t('Links mapping');
  }

  /**
   * @inheritdoc
   */
  public function getDescription() {
    return t('The links mapping operation is related to the profile %profile.', array('%profile' => $this->getProfile()->getTitle()));
  }

  /**
   * @inheritdoc
   */
  public function getType() {
    return 'link_mapping';
  }

  /**
   * @inheritdoc
   */
  protected function getConverterClass() {
    return "Drupal\\maps_links\\Converter\\Link";
  }

  /**
   * @inheritdoc
   */
  protected function getEntityTable() {
    return Fetcher::DB_LINKS_TABLE;
  }

  /**
   * @inheritdoc
   */
  protected function createEntity(array $entity) {
    return new MapsSystemLink($entity);
  }

  /**
   * Get the current list of object entities to process
   * and add related medias to objects.
   */
  protected function getCurrentLinks($start, $maxEntities) {
	  $query = db_select($this->getEntityTable(), 'e')
      ->fields('e')
      ->condition('inserted', 0, '>')
      ->condition('pid', $this->getProfile()->getPid());

    if ($maxEntities > 0) {
      $query->range($start, $maxEntities);
    }

    return $query->execute()
      ->fetchAllAssoc('id', \PDO::FETCH_ASSOC);
  }

  /**
   * @inheritdoc
   */
  protected function getConverters() {
    $this->converters = new \ArrayObject();

    foreach (maps_links_get_converters($this->getProfile()->getPid()) as $converter) {
      $this->converters->append($converter);
    }

    return $this->converters;
  }

  /**
   * Get total count of links to process.
   *
   * @return int
   */
  public function getTotal() {
    return db_query('SELECT COUNT(*) FROM {' . $this->getEntityTable() . '} WHERE pid = :pid AND inserted > 0', array(':pid' => $this->getProfile()->getPid()))->fetchField();
  }

  /**
   * @inheritdoc
   */
  protected function processRun($start = 0, $maxEntities = 0) {
    if ($this->isBatch() && $start == 0) {
      // Message displayed under the progressbar.
      $this->context['message'] = t('Mapping links for profile "@title"', array('@title' => $this->getProfile()->getTitle()));

      $this->context['results']['count_total'] = $this->getTotal();
      $this->context['results']['count_processed'] = 0;
      $this->context['results']['link_uids'] = array();
    }

    self::$currentLog = $this->getlog();

    Logger::getLog($this->getType())->setCurrentOperation($start);
    Logger::getLog($this->getType())->setTotalOperations($this->getTotal());

    if (variable_get('maps_import_differential', 0)) {
      $this->context['results']['object_ids'] = array();
    }

    $link_ids = array();
  	if ($entities = $this->getCurrentLinks($start, $maxEntities)) {
      if ($this->isBatch()) {
        $this->context['results']['count_processed'] += count($entities);
      }

      foreach ($entities as $id => $data) {
        $entity = Mapping::getEntityFromMapsId('object', $data['source_id']);
        $entity = reset($entity);
        $uid = "{$entity['uid']}:{$data['target_id']}:{$data['type_id']}";

        $link_ids[] = $id;

        // Skip already processed links (some are related to others).
        // @todo Cleaner way to manage this.
        if (isset($this->context['results']['link_uids']) && in_array($uid, array_keys($this->context['results']['link_uids']))) {
          $row = db_select(LinkMapping::DB_ENTITIES_TABLE, 'e')
            ->fields('e')
            ->condition('link_id', $this->context['results']['link_uids'][$uid])
            ->execute()
            ->fetch();

          if ($row) {
            $row->link_id = $id;
            db_insert(LinkMapping::DB_ENTITIES_TABLE)
              ->fields((array) $row)
              ->execute();

            continue;
          }
        }

        $this->context['results']['link_uids'][$uid] = $id;

        Logger::getLog($this->getType())->incrementCurrentOperation();
      	$entity = $this->createEntity($data);

      	foreach ($this->getConverters() as $converter) {
      		if ($entity->getLinkType() == $converter->getLinkType()) {
            Logger::getLog($converter->getMapping()->getType())
              ->moveToContentRoot()
              ->addContext(new LogContext(strtolower($converter->getType()), array('id' => $data['source_id'] . " : " . $converter->getBundle())), 'child');

            if (variable_get('maps_import_differential', 0)) {
              if (empty($this->context['results']['object_ids'][$entity->getSource()])) {
                $this->context['results']['object_ids'][$entity->getSource()] = TRUE;
              }
            }

            $converter->getMapping()->process($entity);

            continue 2;
      		}
      	}
      }

      db_update($this->getEntityTable())
        ->fields(array('inserted' => 0))
        ->condition('pid', $this->getProfile()->getPid())
        ->condition('id', $link_ids)
        ->execute();

      if ($this->isBatch()) {
        $finished = $this->context['results']['count_processed'] / $this->context['results']['count_total'];
        $this->context['finished'] = $finished <= 1 ? $finished : 0.9;
      }

    } elseif ($this->isBatch()) {
      $this->context['finished'] = 1;
    }


    Logger::getLog($this->getType())->save();
  }

  /**
   * Delete all mapped relations created from this profile.
   */
  public static function deleteMappedRelations(Profile $profile, $object_id = NULL) {
    // Get all entity ids.
    $select_query = db_select(LinkMapping::DB_ENTITIES_TABLE, 'e')
      ->fields('e', array('relation_id'))
      ->condition('e.pid', $profile->getPid());

    $select_query->innerJoin('maps_import_links', 'l', 'l.id = e.link_id');
    if(!is_null($object_id)) {
      $select_query->condition('l.source_id', $object_id);
    }

    $ids = $select_query->execute()
      ->fetchAllAssoc('relation_id', \PDO::FETCH_ASSOC);

    if (!empty($ids)) {
      $ids = array_keys($ids);
      relation_delete_multiple($ids);

      $delete_query = db_delete(LinkMapping::DB_ENTITIES_TABLE)
        ->condition('pid', $profile->getPid())
        ->condition('relation_id', $ids);

      $delete_query->execute();
    }
  }

}
