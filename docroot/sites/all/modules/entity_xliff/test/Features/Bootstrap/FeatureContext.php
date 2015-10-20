<?php

use Drupal\DrupalExtension\Context\RawDrupalContext;
use Behat\Behat\Context\SnippetAcceptingContext;
use Behat\Gherkin\Node\PyStringNode;
use Behat\Gherkin\Node\TableNode;

/**
 * Defines application features from the specific context.
 */
class FeatureContext extends RawDrupalContext implements SnippetAcceptingContext {

  /**
   * Defines a path part prefix for each entity type tested.
   * @var array
   */
  protected $entityPathPartMap = array(
    'node' => 'node',
    'user' => 'user',
    'taxonomy_term' => 'taxonomy/term',
    'comment' => 'comment',
  );

  /**
   * Initializes context.
   *
   * Every scenario gets its own context instance.
   * You can also pass arbitrary arguments to the
   * context constructor through behat.yml.
   */
  public function __construct() {
  }

  /**
   * @When I attach a(n) :targetlang translation of this :sourcelang :entity
   */
  public function iAttachATranslationOfThisEntity($targetLang, $sourceLang, $entity) {
    $baseUrl = $this->getMinkParameter('base_url');
    $path = $this->getMinkParameter('files_path');
    $session = $this->getSession();
    $url = $session->getCurrentUrl();
    $pathPart = $this->entityPathPartMap[$entity];

    if (preg_match('/' . preg_quote($pathPart, '/') . '\/(\d+)/', $url, $matches)) {
      $id = $matches[1];
      $randomFile = mt_rand(0, 10000) . '-' . $targetLang . '.xlf';

      // Load the XLIFF file for this node.
      $this->getSession()->visit($baseUrl . "/$pathPart/$id/as.xlf?targetLang=$targetLang");
      $xliff = $session->getPage()->getContent();

      // "Translate" the file.
      $translation = str_replace($sourceLang, $targetLang, $xliff);
      $fullPath = $path . DIRECTORY_SEPARATOR . $randomFile;

      // Write the file to the configured path.
      if (file_put_contents($fullPath, $translation)) {
        // Go back and attach the file to the expected import field.
        $session->visit($baseUrl . "/$pathPart/$id/xliff");
        $page = $session->getPage();
        $importFileField = $page->find('css', 'input[name="files[import-' . $targetLang . ']"]');
        $importFileField->attachFile($fullPath);
      }
      else {
        throw new Exception('Unable to write translation to disk.');
      }
    }
    else {
      throw new Exception('Unable to determine what "this ' . $entity . '" is referring to.');
    }
  }

  /**
   * @Given this :entity has an image attached with alt text :alt
   */
  public function thisHasAnImageAttachedWithAltText($entity, $alt) {
    $session = $this->getSession();
    $url = $session->getCurrentUrl();
    $pathPart = $this->entityPathPartMap[$entity];

    if (preg_match('/' . preg_quote($pathPart, '/') . '\/(\d+)/', $url, $matches)) {
      $id = $matches[1];
      $randomFile = mt_rand(0, 10000) . '-test_file.jpg';

      $file = array(
        'uid' => 1,
        'filename' => $randomFile,
        'uri' => 'public://field/image/' . $randomFile,
        'filemime' => 'image/jpeg',
        'filesize' => 146157,
        'status' => 1,
        'timestamp' => REQUEST_TIME,
      );
      if (drupal_write_record('file_managed', $file)) {
        $fileUsage = array(
          'fid' => $file['fid'],
          'module' => 'file',
          'type' => $entity,
          'id' => $id,
          'count' => 1,
        );
        drupal_write_record('file_usage', $fileUsage);

        $wrapper = entity_metadata_wrapper($entity, $id);
        $wrapper->field_image->set($file + array(
          'alt' => $alt,
        ));
        $wrapper->save();
      }
      else {
        throw new Exception('Unable to "attach" image to ' . $entity . '.');
      }
    }
    else {
      throw new Exception('Unable to determine what "this ' . $entity . '" is referring to.');
    }
  }

  /**
   * @Given this :hostentity references the :title :entity
   */
  public function thisNodeReferencesTheNode($hostentity, $title, $entity) {
    $session = $this->getSession();
    $url = $session->getCurrentUrl();
    $pathPart = $this->entityPathPartMap[$hostentity];

    if (preg_match('/' . preg_quote($pathPart, '/') . '\/(\d+)/', $url, $matches)) {
      $hostId = $matches[1];

      try {
        $efq = new EntityFieldQuery();
        $efq->entityCondition('entity_type', $entity);
        $efq->propertyCondition('title', $title);
        $entities = $efq->execute();
        $entityId = (int) key($entities[$entity]);

        $host = entity_metadata_wrapper($hostentity, $hostId);
        $host->field_reference->set($entityId);
        $host->save();
      }
      catch (Exception $e) {
        throw new Exception('Unable to relate ' . $title . ' to this ' . $hostentity . ' because ' . $e->getMessage());
      }
    }
    else {
      throw new Exception('Unable to determine what "this ' . $hostentity . '" is referring to.');
    }
  }

  /**
   * @When I switch to the :langcode translation of this :entity
   */
  public function iSwitchToTheTranslationOfThisUser($langcode, $entity) {
    $baseUrl = $this->getMinkParameter('base_url');
    $session = $this->getSession();
    $url = $session->getCurrentUrl();
    $pathPart = $this->entityPathPartMap[$entity];

    if (preg_match('/' . preg_quote($pathPart, '/') . '\/(\d+)/', $url, $matches)) {
      $id = $matches[1];
      $session->visit($baseUrl . "/$langcode/$pathPart/$id");
    }
    else {
      throw new Exception('Unable to determine what "this ' . $entity . '" is referring to.');
    }
  }

  /**
   * @When I am viewing a(n) :n_many complex :type content with the title :title
   */
  public function iAmViewingAComplexNode($n_many, $type, $title) {
    // First, create a node.
    $node = (object) array(
      'title' => $title,
      'type' => $type,
      'uid' => 1,
    );
    $node = $this->getDriver()->createNode($node);

    // Create and reference related nodes.
    for ($n = 1; $n <= $n_many; $n++) {
      $fieldCollectionItem = entity_create('field_collection_item', array('field_name' => 'field_field_collection'));
      $fieldCollectionItem->setHostEntity('node', $node);
      $fieldCollectionItem->field_long_text[LANGUAGE_NONE][0] = array(
        'format' => 'plain_text',
        'value' => $title . " field collection $n",
      );
      $fieldCollectionItem->save();
    }

    // Resave the original node.
    node_save($node);
    $this->nodes[] = $node;

    // Set internal page on the node.
    $this->getSession()->visit($this->locatePath('/node/' . $node->nid));
  }

}
