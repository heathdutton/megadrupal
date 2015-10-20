<?php
/**
 * @file
 * Provides custom exception classes for the Videopublishing module.
 */


/**
 * A generic exception class for the Videopublishing module.
 */
class VideopublishingException extends Exception {}

/**
 * A custom exception class for the API component of the Videopublishing module.
 */
class VideopublishingApiException extends VideopublishingException {}