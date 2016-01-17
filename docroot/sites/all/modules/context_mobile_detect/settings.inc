<?php

/**
 * @file
 * This file should be included in your settings.php
 * in order to make Drupal page cache works correctly.
 */

require_once dirname(__FILE__) . '/context_mobile_detect.module';

context_mobile_detect_preboot();
