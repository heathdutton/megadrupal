<?php

/**
 * @file
 * Contains \Drupal\integration_producer\AbstractProducer.
 */

namespace Drupal\integration_producer;

use Drupal\integration\Backend\AbstractBackend;
use Drupal\integration\ConfigurablePluginInterface;
use Drupal\integration\Document\DocumentInterface;
use Drupal\integration\Configuration\AbstractConfiguration;
use Drupal\integration\Plugins\PluginManager;
use Drupal\integration\ResourceSchema\AbstractResourceSchema;

/**
 * Class AbstractProducer.
 *
 * @package Drupal\integration_producer
 */
abstract class AbstractProducer implements ProducerInterface, ConfigurablePluginInterface {

  /**
   * Configuration object.
   *
   * @var Configuration\ProducerConfiguration
   */
  private $configuration;

  /**
   * Entity wrapper.
   *
   * @var EntityWrapper\EntityWrapper
   */
  private $entityWrapper = NULL;

  /**
   * Document handler instance.
   *
   * @var DocumentInterface
   */
  private $document = NULL;

  /**
   * Backend instance.
   *
   * @var AbstractBackend
   */
  private $backend = NULL;

  /**
   * Resource schema instance.
   *
   * @var AbstractResourceSchema
   */
  private $resource = NULL;

  /**
   * List of field handler definitions keyed by field type.
   *
   * @see integration_producer_get_field_handlers()
   *
   * @var array[FieldHandlerInterface]
   */
  private $fieldHandlers = array();

  /**
   * AbstractProducer constructor.
   *
   * @param Configuration\ProducerConfiguration $configuration
   *    Configuration object.
   * @param EntityWrapper\EntityWrapper $entity_wrapper
   *    Entity object.
   * @param DocumentInterface $document
   *    Document object.
   * @param AbstractBackend $backend
   *    Backend object.
   * @param AbstractResourceSchema $resource
   *    Resource schema object.
   */
  public function __construct(Configuration\ProducerConfiguration $configuration, EntityWrapper\EntityWrapper $entity_wrapper, DocumentInterface $document, AbstractBackend $backend, AbstractResourceSchema $resource) {
    $manager = PluginManager::getInstance('producer');

    $this->setConfiguration($configuration);
    $this->entityWrapper = $entity_wrapper;
    $this->document = $document;
    $this->backend = $backend;
    $this->resource = $resource;
    $this->fieldHandlers = $manager->getComponentDefinitions('field_handler');
  }

  /**
   * {@inheritdoc}
   */
  public function getConfiguration() {
    return $this->configuration;
  }

  /**
   * {@inheritdoc}
   */
  public function setConfiguration(AbstractConfiguration $configuration) {
    $this->configuration = $configuration;
  }

  /**
   * {@inheritdoc}
   */
  public function getEntityWrapper() {
    return $this->entityWrapper;
  }

  /**
   * {@inheritdoc}
   */
  public function getDocument() {
    return $this->document;
  }

  /**
   * {@inheritdoc}
   */
  public function getProducerId() {
    return $this->getConfiguration()->getProducerId();
  }


  /**
   * Return field handler object given field name and language.
   *
   * @param string $field_name
   *    Field machine name.
   * @param string $language
   *    Field language.
   *
   * @return FieldHandlers\AbstractFieldHandler
   *    Field handler object.
   */
  protected function getFieldHandler($field_name, $language) {
    $field_info = field_info_field($field_name);
    $class = isset($this->fieldHandlers[$field_info['type']]) ? $this->fieldHandlers[$field_info['type']]['class'] : $this->fieldHandlers['default']['class'];
    return new $class($field_name, $language, $this->getEntityWrapper(), $this->getDocument());
  }

  /**
   * {@inheritdoc}
   */
  public function build($entity) {
    $this->getEntityWrapper()->set($entity);

    // Set document metadata.
    $this->getDocument()->setMetadata('type', $this->getDocumentType());
    $this->getDocument()->setMetadata('producer', $this->getProducerId());
    $this->getDocument()->setMetadata('producer_content_id', $this->getProducerContentId());
    $this->getDocument()->setMetadata('created', $this->getDocumentCreationDate());
    $this->getDocument()->setMetadata('updated', $this->getDocumentUpdateDate());

    // Set multilingual-related metadata.
    $this->getDocument()->setMetadata('languages', $this->getEntityWrapper()->getAvailableLanguages());
    $this->getDocument()->setMetadata('default_language', $this->getEntityWrapper()->getDefaultLanguage());

    // Set field values.
    foreach ($this->getEntityWrapper()->getAvailableLanguages() as $language) {
      foreach ($this->getEntityWrapper()->getFieldList() as $field_name) {
        $this->getFieldHandler($field_name, $language)->process();
      }
    }

    $entity_wrapper = $this->getEntityWrapper();
    $document = $this->getDocument();
    drupal_alter('integration_producer_document_build', $entity_wrapper, $document);
    return $document;
  }

}
