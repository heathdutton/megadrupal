<?php

/**
 * @file
 * Handles test for MaPS Import Mapper classes.
 */

use Drupal\maps_import\Mapping\Mapper\Mapper;
use Drupal\maps_import\Mapping\Mapper\Object as ObjectMapper;
use Drupal\maps_import\Mapping\Mapper\Media as MediaMapper;

/**
 * Handles tests for configuration import operation.
 */
abstract class MapsImportMapperMock extends Mapper {}

class MapsImportObjectMapperMock extends ObjectMapper {}

class MapsImportMediaMapperMock extends MediaMapper {}
