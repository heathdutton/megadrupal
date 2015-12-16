<?php

/**
 * @file
 * All the constants of Plus Gallery.
 */

/**
 * Minimum library version that is required.
 */
define("PLUS_GALLERY_LIBRARY_MIN_VERSION", "0.9.2");

/**
 * Gallery sources.
 */
define('PLUS_GALLERY_NONE', 0);
define('PLUS_GALLERY_FACEBOOK', 1);
define('PLUS_GALLERY_GOOGLEPLUS', 2);
define('PLUS_GALLERY_FLICKR', 3);
define('PLUS_GALLERY_INSTAGRAM', 4);
define('PLUS_GALLERY_LOCAL', 5);

/**
 * Gallery modes.
 */
define('PLUS_GALLERY_MODE_GALLERY_OVERVIEW', 0);
define('PLUS_GALLERY_MODE_ALBUM_MODE', 1);

/**
 * Which galleries have to be shown? All, defined only or all except defined?
 */
define('PLUS_GALLERY_BEHAVIOUR_ALL', 0);
define('PLUS_GALLERY_BEHAVIOUR_INCLUDE', 1);
define('PLUS_GALLERY_BEHAVIOUR_EXCLUDE', 2);

/**
 * The maximum length of the form fields.
 */
define('PLUS_GALLERY_FIELD_LENGTH', 63);
define('PLUS_GALLERY_ACCESS_TOKEN_LENGTH', 255);

/**
 * The maximum number of images loaded per album.
 */
define('PLUS_GALLERY_FLICKR_MAX_IMAGES', 30);