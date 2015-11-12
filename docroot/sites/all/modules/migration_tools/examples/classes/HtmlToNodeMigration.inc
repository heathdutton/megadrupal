<?php
/**
 * @file
 * Defines HtmlToNodeMigration class.
 */

/**
 * Class HtmlToNodeMigration.
 *
 * Parent class for all migrations from static HTML to any content type.
 *
 * @package migration_tools
 */
abstract class HtmlToNodeMigration extends FileSourceMigration {

  /**
   * @var string $sourceParserClass
   *   The class name of the source parser. Used to instantiate $sourceParser.
   */
  protected $sourceParserClass;

  /**
   * @var NGSourceParser $source_parser
   *   The source parser object for a given row.
   */
  protected $sourceParser;

  /**
   * Define $this->source, $this->map, and simple mappings.
   *
   * @param array $arguments
   *   Arguments provided by migrate module.
   * @param array $source_fields
   *   An associative array of fields contained in the source data, in the form
   *   of array('machine_name' => t('Human Readable')).
   * @param array $source_dirs
   *   A flat array of the source directories containing html files.
   * @param string $regex
   *   The file mask. Only file names matching this regex will be migrated.
   *   This will default to '/.*\.htm(l)?$/' if NULL is passed in.
   * @param array $options
   *   Options that will be passed on to file_scan_directory(). See docs of that
   *   core Drupal function for more information.
   * @param MigrateContentParser $parser
   *   A parser class that will provide file chunk ids. Defaults to
   *   MigrateSimpleContentParser in parent constructor.
   */
  public function __construct($arguments, $source_fields, $source_dirs, $regex = NULL, $options = array(), MigrateContentParser $parser = NULL) {
    if ($regex === NULL) {
      $regex = '/.*\.htm(l)?$/i';
    }

    // Calling parent constructor will set $this->source and $this->map.
    parent::__construct($arguments, $source_fields, $source_dirs, $regex, $options, $parser);

    // A child class might provide it's own source parser that offer specific
    // cleanup.
    if (array_key_exists('source_parser_class', $arguments)) {
      $this->sourceParserClass = $arguments['source_parser_class'];
    }
    else {
      $this->sourceParserClass = "NGNodeSourceParser";
    }

    // Map fields where source field name is same as destination field name.
    $this->addSimpleMappings(array(
      'uid',
      'title',
    ));

    // A map of source HTML filename -> destination node id.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'fileid' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    // Define non-simple field mappings.
    $this->addFieldMapping('workbench_moderation_state_new')->defaultValue('published');

    // Create redirects using relative, legacy path of static file.
    $this->addFieldMapping('migrate_redirects', 'legacy_path')->separator('|');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    // Sets $row->legacy_path and $row->url_path.
    if (empty($row->legacy_path) || empty($row->url_path)) {
      $this->generateLegacyPath($row);
    }

    // Check whether a redirect to this page already exists, if so, do not
    // migrate the "duplicate" content.
    if ($this->isDuplicateByRedirect($row)) {
      return FALSE;
    }

    // Set to admin for now.
    $row->uid = 1;

    // Get the source parser ready.
    if (!empty($row->filedata)) {
      $this->initializeSourceParser($row);

      // Get the properties from the source parser.
      $this->setProperties($row);
    }
  }

  /**
   * Create a new SourceParser to handle HTML content.
   */
  protected function initializeSourceParser($row) {
    $source_parser = new $this->sourceParserClass($row->url_path, $row->filedata);

    $obtainers_info = $this->getArgument("obtainers_info");
    if (isset($obtainers_info) && is_array($obtainers_info)) {
      foreach ($obtainers_info as $oi) {
        $source_parser->addObtainerInfo($oi);
      }
    }
    $this->sourceParser = $source_parser;
  }

  /**
   * Use the source parser to set properties in whatever order.
   */
  protected function setProperties($row) {
    $row->content_type = $this->sourceParser->getContentType();
    $row->title = $this->sourceParser->getTitle();
    $row->body = $this->sourceParser->getBody();
  }

  /**
   * {@inheritdoc}
   */
  public function prepare($entity, $row) {
    migration_tools_prepare_workbench_settings($entity, $row);

    // Sidestep language to get redirects and aliases working for all users.
    parent::languageSideStep($entity);
  }

  /**
   * {@inheritdoc}
   */
  public function complete($entity, $row) {
    // Change the language back if it was sidestepped in prepare().
    // Must come before workbench settings or revision ends up unpublished.
    parent::languageReturnStep($entity);

    if (isset($entity->nid) && $entity->nid) {
      migration_tools_complete_workbench_settings($entity, $row);
    }
  }
}
