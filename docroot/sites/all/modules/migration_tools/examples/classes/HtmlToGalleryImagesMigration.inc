<?php
/**
 * @file
 * Gallery Images migration classes.
 */

/**
 * Class HtmlToGalleryImagesMigration
 *
 * Migrates field collection items for Gallery nodes.
 */
abstract class HtmlToGalleryImagesMigration extends FileSourceMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments, $source_dirs, $options = array()) {

    if (!empty($arguments["gallery_migration"])) {
      $this->dependencies = array($arguments["gallery_migration"]);
    }
    else {
      throw new Exception("the 'gallery_migration' argument is required by HtmlToGalleryImagesMigration");
    }

    // Define the fields that will be derived from the static files.
    $source_fields = array(
      'field_gallery_image_title' => t('Title'),
      'field_gallery_image_body' => t('Body'),
      'field_gallery_image_image' => t('Image'),
      'field_gallery_image_credits' => t('Credits'),
    );

    // Match .htm, .html files only.
    $regex = '/.*\.htm(l)?$/';

    $parser = new GalleryChunkParser();

    // Calling parent constructor will set $this->source and $this->map.
    parent::__construct($arguments, $source_fields, $source_dirs, $regex, $options, $parser);

    $this->description = t('Migrates gallery images (field collections).');

    // A map of source HTML filename -> destination node id.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'id' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationFieldCollection::getKeySchema()
    );

    $this->destination = new MigrateDestinationFieldCollection(
      'field_gallery_images',
      array('host_entity_type' => 'node')
    );

    // The gallery_id is the file name of the source gallery.
    $this->addFieldMapping('host_entity_id', 'gallery_id')
      ->sourceMigration($arguments["gallery_migration"]);

    // Map fields where source field name is same as destination field name.
    $this->addSimpleMappings(array(
      'field_gallery_image_title',
      'field_gallery_image_body',
      'field_gallery_image_credits',
    ));

    $this->addFieldMapping('field_gallery_image_image:file_class')
      ->defaultValue('MigrateFileUri');

    // Map the file name of the image to the image field.
    $this->addFieldMapping('field_gallery_image_image', 'image_filename');

    // Define the source directory for the images.
    $this->addFieldMapping('field_gallery_image_image:source_dir', 'gallery_source_dir');

    // Map title and alt tags.
    $this->addFieldMapping('field_gallery_image_image:alt', 'image_alt');
    $this->addFieldMapping('field_gallery_image_image:title', 'image_alt');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);

    // Row ids are in the form of /file_name.html-?MIGRATECHUNK?-chunk-id.
    // We determine the original gallery id by stripping out only the file name
    // portion of this string.
    $row->gallery_id = substr($row->id, 0, strpos($row->id, '-?MIGRATECHUNK?-'));

    // Create a new SourceParser to handle HTML content.
    try {
      $html = $row->filedata;
      // BUG: There is a problem created here because we are taking content
      // that was gathered from a queryPath object and passing it to a new
      // queryPath object.  We have never been able to figure out why double
      // queryPathing leads to bad characters being created from harmless
      // html entities.
      $source_parser = new SourceParser($row->url_path, $html);

      $image_url = $source_parser->queryPath->find('.download a')->attr('href');

      $row->gallery_source_dir = $this->getGallerySourceDir($row->id);

      $file_name = basename($image_url);
      $row->image_filename = $file_name;

      $row->image_alt = $source_parser->queryPath->find('img')->attr('alt');
      $row->image_alt = mb_strimwidth($row->image_alt, 0, 512, "...");

      $row->field_gallery_image_title = $source_parser->queryPath->find('.image-title')->text();
      $row->field_gallery_image_credits = $source_parser->queryPath->find('.image-credit')->text();
      $row->field_gallery_image_body = $source_parser->queryPath->find('.image-desc')->text();
    }
    catch (Exception $e) {
      watchdog('migration_tools', 'Image migration failed for Gallery Image @row_id. File will be skipped. Error message: @error', array('@row_id' => $row->id, '@error' => $e->getMessage()), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  /**
   * Given the id of the image chunck return the containin source directory.
   */
  abstract protected function getGallerySourceDir($row_id);
}


/**
 * Parses a single gallery into a list gallery images.
 *
 * @package migration_tools
 * @subpackage core
 * @see GalleryImagesMigration
 */
class GalleryChunkParser extends MTChunkParser {

  /**
   * Sets $this->chunks.
   */
  public function setChunks() {
    // Each <li> in ul#thumbs is an image.
    $this->chunks = $this->sourceParser->queryPath->find('#thumbs li');
  }

  /**
   * {@inheritdoc}
   */
  public function getChunkIDs() {
    $chunks = $this->getChunks();

    $ids = array();

    foreach ($chunks as $chunk) {
      // The ID of each gallery image will be the href of the image file.
      $id = $chunk->find('.download a')->attr('href');
      // Filter out empty strings.
      if ($id) {
        $ids[] = $id;
      }
    }

    return $ids;
  }

  /**
   * {@inheritdoc}
   */
  public function getChunk($chunk_id) {
    $chunks = $this->getChunks();

    $chunk = $chunks->find('a[href="' . $chunk_id . '"]')->parent('li')->html();
    if (!$chunk) {
      $chunk = $chunks->find('a[name="' . $chunk_id . '"]')->parent()->html();
    }
    if (!$chunk) {
      $chunk = 'Data could not be migrated';
    }

    return $chunk;
  }
}
