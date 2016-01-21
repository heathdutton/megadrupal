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
   * @When /^I attach(?:| a(?:|n)) (?:|")([^"]+)(?:|") translation(?:|s) of this "([^"]+)" ([^"]+)$/
   */
  public function iAttachATranslationOfThisEntity($targetLangs, $sourceLang, $entity) {
    $baseUrl = $this->getMinkParameter('base_url');
    $path = $this->getMinkParameter('files_path');
    $session = $this->getSession();
    $url = $session->getCurrentUrl();
    $pathPart = $this->entityPathPartMap[$entity];
    $targetLangs = explode(',', $targetLangs);
    $files = array();

    if (preg_match('/' . preg_quote($pathPart, '/') . '\/(\d+)/', $url, $matches)) {
      $id = $matches[1];

      // Iterate through all provided languages.
      foreach ($targetLangs as $targetLang) {
        $targetLang = trim($targetLang);
        $randomFile = mt_rand(0, 10000) . '-' . $targetLang . '.xlf';

        // Load the XLIFF file for this node.
        $this->getSession()
          ->visit($baseUrl . "/$pathPart/$id/as.xlf?targetLang=$targetLang");
        $xliff = $session->getPage()->getContent();

        // "Translate" the file.
        $translation = str_replace($sourceLang, $targetLang, $xliff);
        $fullPath = $path . DIRECTORY_SEPARATOR . $randomFile;

        // Write the file to the configured path.
        if (file_put_contents($fullPath, $translation)) {
          $files[$targetLang] = $fullPath;
        }
        else {
          throw new Exception('Unable to write translation to disk.');
        }
      }

      // Go back and attach the file(s) to the expected import field(s).
      $session->visit($baseUrl . "/$pathPart/$id/xliff");
      foreach ($files as $lang => $path) {
        $page = $session->getPage();
        $importFileField = $page->find('css', 'input[name="files[import-' . $lang . ']"]');
        $importFileField->attachFile($path);
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
        $entityId = $this->getEntityIdForTitle($entity, $title);
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

    // Create and reference related field collections.
    $this->attachSomeFieldCollectionsTo($node, $n_many, $title);

    // Resave the original node.
    node_save($node);
    $this->nodes[] = $node;

    // Set internal page on the node.
    $this->getSession()->visit($this->locatePath('/node/' . $node->nid));
  }

  /**
   * @Given this :hostentity has :n_many (additional) field collection(s)
   */
  public function thisNodeHasSomeNumberOfFieldCollections($hostentity, $n_many) {
    $session = $this->getSession();
    $url = $session->getCurrentUrl();
    $pathPart = $this->entityPathPartMap[$hostentity];

    if (preg_match('/' . preg_quote($pathPart, '/') . '\/(\d+)/', $url, $matches)) {
      $hostId = $matches[1];

      try {
        $hostToBeSaved = entity_metadata_wrapper($hostentity, $hostId);
        $host = $hostToBeSaved->value();
        $this->attachSomeFieldCollectionsTo($host, $n_many, NULL, $host->language);
        $hostToBeSaved->set($host);
        $hostToBeSaved->save();
      }
      catch (Exception $e) {
        throw new Exception('Unable to attach ' . $n_many . ' collections to this ' . $hostentity . ' because ' . $e->getMessage());
      }
    }
    else {
      throw new Exception('Unable to determine what "this ' . $hostentity . '" is referring to.');
    }
  }

  /**
   * @Given I am viewing a :type content with paragraphs and the title :title
   */
  public function iAmViewingAParagraphEnabledNode($type, $title) {
    // See if a node with this title already exists...
    if ($nid = $this->getEntityIdForTitle('node', $title)) {
      $node = node_load($nid);
    }
    else {
      // If not, create one.
      $node = (object) array(
        'title' => $title,
        'type' => $type,
        'uid' => 1,
      );
      $node = $this->getDriver()->createNode($node);
    }

    // Create and reference paragraphs.
    $this->createParagraph('node', $node, 'bundle_1', array(
      'field_long_text' => array(LANGUAGE_NONE => array(array(
        'format' => 'plain_text',
        'value' => $title . " paragraph 1",
      ))),
    ));
    $paragraphHost = $this->createParagraph('node', $node, 'bundle_2');
    $this->createParagraph('paragraph_item', $paragraphHost, 'bundle_1', array(
      'field_long_text' => array(LANGUAGE_NONE => array(array(
        'format' => 'plain_text',
        'value' => $title . " paragraph 2",
      ))),
    ));

    // Resave the original node.
    node_save($node);
    $this->nodes[] = $node;

    // Set internal page on the node.
    $this->getSession()->visit($this->locatePath('/node/' . $node->nid));
  }

  /**
   * Instantiates and saves a Paragraph item and attaches it to the given host.
   * @param string $hostType
   * @param object $host
   * @param string $bundle
   * @param array $data
   * @return \ParagraphsItemEntity
   */
  protected function createParagraph($hostType, $host, $bundle, $data = array()) {
    $paragraph = entity_create('paragraphs_item', array('field_name' => 'field_paragraphs', 'bundle' => $bundle));
    $paragraph->setHostEntity($hostType, $host);
    foreach ($data as $field => $values) {
      $paragraph->{$field} = $values;
    }
    $paragraph->save();
    return $paragraph;
  }

  /**
   * Attaches the given number of field collections to the given node.
   * @param object $node
   * @param int $n_many
   */
  protected function attachSomeFieldCollectionsTo($node, $n_many, $title = '', $language = LANGUAGE_NONE) {
    if (empty($title)) {
      $title = $node->title;
    }

    // Create and reference related nodes.
    for ($n = 1; $n <= $n_many; $n++) {
      $fieldCollectionItem = entity_create('field_collection_item', array('field_name' => 'field_field_collection'));
      $fieldCollectionItem->setHostEntity('node', $node, $language);
      $fieldCollectionItem->field_long_text[LANGUAGE_NONE][0] = array(
        'format' => 'plain_text',
        'value' => $title . " field collection $n",
      );
      $fieldCollectionItem->save();
    }
  }

  /**
   * Given an entity type and title, returns an entity ID.
   * @param string $entityType
   * @param string $title
   * @return int
   */
  protected function getEntityIdForTitle($entityType, $title) {
    $efq = new EntityFieldQuery();
    $efq->entityCondition('entity_type', $entityType);
    $efq->propertyCondition('title', $title);
    $entities = $efq->execute();
    return isset($entities[$entityType]) ? (int) key($entities[$entityType]) : 0;
  }
}
