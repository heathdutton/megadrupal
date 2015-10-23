<?php
/**
 * @file
 * Contains DataUriCreator.
 */

/**
 * Implements a helper class with utility methods involving Data URIs.
 */
final class DataUriCreator {

  /**
   * Hides the DataUriCreator constructor.
   *
   * By combining the "protected" keyword (that is specified on the
   * constructor) with the "final" keyword (that is specified on the class
   * definition), this class is effectively turned into a sealed static class
   * that can never be instantiated or derived from.  To use the class, one
   * may only call existing public static methods from externally.
   *
   * Furthermore, without the ability to derive from this class, it reduces
   * the benefits of using late static bindings, such that PHP 5.3.0 is no
   * longer a requirement (instead requiring only basic OOP features of PHP 5).
   */
  protected function __construct() {
    // This is not supposed to execute ever.
  }

  /**
   * Determines whether the specified URL uses the Data URI scheme.
   *
   * @param string $url
   *   The URL to be inspected.
   *
   * @return bool
   *   TRUE if the URL appears to be a Data URI; otherwise FALSE.
   */
  public static function isDataUri($url) {
    return DataUriCreatorString::startsWith($url, 'data:');
  }

  /**
   * Ensures that the provided URL is a fully-qualified URL or a Data URI.
   *
   * @param string $url
   *   The source URL to be modified or returned as-is.
   * @param bool $is_stream_allowed
   *   TRUE to indicate that the specified path may be a stream URI;
   *   otherwise FALSE.
   *
   * @return string|false
   *   The full URL corresponding with the provided URI or URL.
   */
  public static function getDataUriOrQualifiedUrl(
    $url, $is_stream_allowed = FALSE) {

    // Note: Data URI's are allowed to pass through just as-is
    if ((!empty($url)) && (!self::isDataUri($url))) {
      if ($is_stream_allowed) {
        $url = file_create_url($url);
      }
      else {
        $url = url($url, array('absolute' => TRUE));
      }
    }

    return $url;
  }

  /**
   * Creates a Data URI for the specified data buffer.
   *
   * @param string $data_content
   *   The data bytes to be encoded.
   * @param string $media_type
   *   The full media-type of the specified content.  This may consist of
   *   only the MIME type, such as "image/png", or be combined with additional
   *   parameters, for example, "text/plain;charset=utf-8".
   * @param bool $use_base64_encoding
   *   (optional) TRUE to indicate that base64 encoding must be used to
   *   encode the data content in the URI (this is the default); otherwise
   *   FALSE to indicate that percent encoding (RFC 3986) must be used.
   *
   * @return string
   *   The content data bytes represented as a Data URI.
   */
  public static function encode(
    $data_content, $media_type, $use_base64_encoding = TRUE) {

    // Sanitize the media-type parameter.
    $append_parameters = TRUE;
    $include_defaults = FALSE;
    $defines_base64 = NULL;
    $media_type = self::parseMediaType($media_type, $defines_base64, $append_parameters, $include_defaults);
    $use_base64_encoding = ($use_base64_encoding || $defines_base64);

    // Build the Data URI.
    $data_uri = ('data:' . $media_type);
    if ($use_base64_encoding) {
      $data_uri .= (';base64,' . base64_encode($data_content));
    }
    else {
      $data_uri .= (',' . rawurlencode($data_content));
    }

    return $data_uri;
  }

  /**
   * Creates a Data URI for the specified file's content.
   *
   * @param string $data_filename
   *   The filename of the content to be encoded.
   * @param string|null $media_type
   *   (optional) The full media-type of the file's content.  This may
   *   consist of only the MIME type, such as "image/png", or be combined with
   *   additional parameters, for example, "text/plain;charset=utf-8".  However,
   *   if the media type is not specified (that is NULL), the parameter will be
   *   treated as an output variable and will be set to the file's MIME type.
   * @param bool $use_base64_encoding
   *   (optional) TRUE to indicate that base64 encoding must be used to
   *   encode the data content in the URI (this is the default); otherwise
   *   FALSE to indicate that percent encoding (RFC 3986) must be used.
   *
   * @return string
   *   The content of the file represented as a Data URI.
   */
  public static function encodeFile(
    $data_filename, &$media_type = NULL, $use_base64_encoding = TRUE) {

    if (!isset($media_type)) {
      $media_type = file_get_mimetype($data_filename);
    }

    return self::encode(file_get_contents($data_filename), $media_type, $use_base64_encoding);
  }

  /**
   * Extracts the data content and MIME type from a Data URI.
   *
   * This method can also be used to determine if a specified URL is a Data URI
   * with valid content.  To do so, check that the return value is not NULL.
   *
   * @param string $data_uri
   *   The Data URI to be decoded.
   * @param string|null $mime_type
   *   (optional) For a valid Data URI, this output variable will be set to
   *   the MIME type of the content upon returning.  However, one can also
   *   request that the full media-type be returned instead, such that the
   *   media parameters will be appended to the MIME type.
   * @param bool|null $use_base64_encoding
   *   (optional) An output variable to be set to a boolean value that
   *   indicates the type of encoding that was used for the data content in
   *   the URI; a value of TRUE indicates that base64 encoding was used, while
   *   FALSE indicates that percent encoding (RFC 3986) was used, but the
   *   value may also be NULL when the Data URI is invalid.
   * @param bool $append_parameters
   *   (optional) A boolean value that can be set to TRUE to indicate that the
   *   returned $mime_type output variable must be set to the full media-type,
   *   including the media parameters (such as character encoding).  Else, as
   *   indicated by the default value of FALSE, only the MIME type is included.
   * @param bool $include_defaults
   *   (optional) A boolean value that can be set to TRUE, that is the default
   *   value, to indicate that the returned $mime_type output variable must be
   *   set to a default value (as defined by the Data URI specification,
   *   RFC 2397) if the Data URI does not explicitly specify the media type.
   *   Specify FALSE to indicate that no default values should be included.
   * @param array|null $media_type_parts
   *   (optional) An output variable to be set to an array that corresponds
   *   with the full media-type of the Data URI in decomposed form.  The first
   *   item in the array is the MIME type, while subsequent items represent
   *   parameters (that is strings consisting of attribute-value pairs that are
   *   separated by an equals sign).  Note that no assumptions should be made
   *   about the keys of the array; although they may be numerical, the numbers
   *   may not be sequential or zero-based.  Furthermore, the value of the
   *   $include_defaults parameter can affect the values of this output array.
   *
   * @return string|null
   *   For an invalid Data URI, NULL is returned; otherwise a string is
   *   returned, which contains the decoded data from the specified Data URI.
   */
  public static function decode(
    $data_uri, &$mime_type = NULL, &$use_base64_encoding = NULL, $append_parameters = FALSE, $include_defaults = TRUE, &$media_type_parts = NULL) {

    $data_content = NULL;
    $mime_type = NULL;
    $use_base64_encoding = NULL;
    $media_type_parts = NULL;
    if (self::isDataUri($data_uri)) {
      // The specified argument appears to be a Data URI.
      $data_index = strpos($data_uri, ',', 5);
      if ($data_index !== FALSE) {
        // The specified Data URI seems to be formatted correctly with data.
        $media_type = substr($data_uri, 5, ($data_index - 5));
        $data_value = ((strlen($data_uri) <= ($data_index + 1)) ? '' : substr($data_uri, ($data_index + 1)));

        // Extract the media/MIME type and the data bytes from the Data URI.
        $mime_type = self::parseMediaType($media_type, $use_base64_encoding, $append_parameters, $include_defaults, $media_type_parts);
        $data_content = ($use_base64_encoding ? base64_decode($data_value, TRUE) : rawurldecode($data_value));
        if ($data_content === FALSE) {
          $data_content = NULL;
        }
      }
    }

    return $data_content;
  }

  /**
   * Parses and sanitizes the specified media-type string.
   *
   * @param string $media_type
   *   The full media-type of the specified content.  This may consist of
   *   only the MIME type, such as "image/png", or be combined with additional
   *   parameters, for example, "text/plain;charset=utf-8".
   * @param bool|null $defines_base64
   *   (optional) An output variable to be set to a boolean value that indicates
   *   whether the "base64" marker was included in the specified media-type
   *   value.  Although this marker is strictly speaking not part of the media
   *   type, this method can detect and remove the marker from the media-type,
   *   such that it will never be included in any returned media-type values.
   * @param bool $append_parameters
   *   (optional) A boolean value that can be set to TRUE to indicate that the
   *   method's return value must be set to the full media-type, including the
   *   media parameters (such as character encoding).  Otherwise, as indicated
   *   by the default value of FALSE, only the MIME type is included.
   * @param bool $include_defaults
   *   (optional) A boolean value that can be set to TRUE, that is the default
   *   value, to indicate that the method's return value must be set to a
   *   default value (as defined by the Data URI specification, RFC 2397)
   *   if the Data URI does not explicitly specify the media type.  Specify
   *   FALSE to indicate that no default values should be included.
   * @param array|null $media_type_parts
   *   (optional) An output variable to be set to an array that corresponds
   *   with the full media-type of the Data URI in decomposed form.  The first
   *   item in the array is the MIME type, while subsequent items represent
   *   parameters (that is strings consisting of attribute-value pairs that are
   *   separated by an equals sign).  Note that no assumptions should be made
   *   about the keys of the array; although they may be numerical, the numbers
   *   may not be sequential or zero-based.  Furthermore, the value of the
   *   $include_defaults parameter can affect the values of this output array.
   *
   * @return string|null
   *   For a valid media-type, the MIME-type segment of the media-type is
   *   returned as a string, which is what the media-type string usually starts
   *   with.  However, one can also request that the full media-type be returned
   *   instead (using $append_parameters), such that the media parameters will
   *   be appended to the MIME type; this would effectively be a sanatized
   *   version of the media-type that was specified as input.  A default
   *   value may also be returned, if requested (using $include_defaults).
   *   For an unexpected media-type argument-data-type, NULL may be returned.
   */
  public static function parseMediaType(
    $media_type, &$defines_base64 = NULL, $append_parameters = FALSE, $include_defaults = TRUE, &$media_type_parts = NULL) {

    $mime_type = NULL;
    $defines_base64 = NULL;
    $media_type_parts = NULL;
    if ((!is_string($media_type)) && empty($media_type)) {
      $media_type = '';
    }

    if (is_string($media_type)) {
      // Split the media-type and parameters, and remove surrounding whitespace.
      $media_type_parts = array_map('trim', explode(';', $media_type));

      // Retrieve the first element of the array, which should be the MIME-type.
      if (empty($media_type_parts)) {
        $mime_type = '';
        $media_type_parts = array($mime_type);
      }
      else {
        reset($media_type_parts);
        $mime_type = current($media_type_parts);
      }

      if (isset($mime_type) && ($mime_type !== '') && (((strpos($mime_type, '/') === FALSE) && (0 !== strcasecmp($mime_type, 'base64'))) || (strpos($mime_type, '=') !== FALSE))) {
        // The first element does not appear to be a MIME-type string;
        // insert a first element to represent an unspecified MIME-type.
        $mime_type = '';
        array_splice($media_type_parts, 0, 0, array($mime_type));
      }

      // Check whether "base64" was included in the media-type's parameters.
      // Note that the first element is ignored, since that must be interpreted
      // as the MIME-type (even when it appears to be the "base64" marker),
      // because the marker must be prefixed by a semicolon according to
      // the RFC 2397 specification (exactly specified as ";base64") that
      // would make it not be representable by the first array-element.
      $parameters = array_slice($media_type_parts, 1);
      $defines_base64 = in_array('base64', $parameters, TRUE);
      if ($defines_base64) {
        // Remove "base64" that is strictly speaking not part of the media type,
        // but preserve the first element as the MIME-type (whatever it may be).
        $parameters = array_diff($parameters, array('base64'));
        array_splice($media_type_parts, 1, count($media_type_parts), $parameters);
      }

      // Define a default MIME-type, if requested.
      if ($include_defaults && empty($mime_type)) {
        // Note: The default MIME-type is defined by RFC 2397.
        $mime_type = 'text/plain';

        // Replace the first element of the array with the new media-type.
        array_splice($media_type_parts, 0, 1, array($mime_type));

        // Check whether a character encoding has been specified already.
        $defines_charset = FALSE;
        foreach ($parameters as $parameter) {
          $equals_index = strpos($parameter, '=');
          if (($equals_index !== FALSE) && (0 === strcasecmp('charset', trim(substr($parameter, 0, $equals_index))))) {
            $defines_charset = TRUE;
            break;
          }
        }

        // Add the default character-encoding, if missing.
        if (!$defines_charset) {
          // Insert an element after the first element (after the media type).
          // Note: The default encoding is defined by RFC 2397.
          array_splice($media_type_parts, 1, 0, array('charset=US-ASCII'));
        }
      }

      if ($append_parameters) {
        // Build the full media-type string to be returned.
        $mime_type = implode(';', $media_type_parts);
      }
    }

    return $mime_type;
  }
}
