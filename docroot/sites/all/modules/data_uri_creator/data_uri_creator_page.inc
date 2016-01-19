<?php
/**
 * @file
 * Implements the form for the main Data URI Creator page.
 */

// Define constants used by this form page.
define('_DATA_URI_CREATOR_FORM_FIELD_DATA_FILE', 'data_upload_file');
define('_DATA_URI_CREATOR_FORM_FIELD_DATA_URI', 'uploaded_data_uri');
define('_DATA_URI_CREATOR_FORM_BUTTON_UPLOAD', 'data_upload_button');
define('_DATA_URI_CREATOR_FORM_BUTTON_IMAGE', 'image_launch');
define('_DATA_URI_CREATOR_FORM_CHECKBOX_BASE64', 'use_base64_encoding');
define('_DATA_URI_CREATOR_AJAX_CONTENT_UPLOAD', 'data_uri_result');
define('_DATA_URI_CREATOR_AJAX_WRAPPER_UPLOAD', 'data_uri_result_wrapper');

/**
 * Form constructor for the main Data URI Creator page.
 *
 * @param array $form
 *   The form's current render-array.
 * @param array $form_state
 *   Reference to the current form state.
 *
 * @return array
 *   An array representing the form definition.
 *
 * @ingroup forms
 * @see data_uri_creator_form_validate()
 * @see data_uri_creator_form_submit()
 * @see data_uri_creator_form_ajax()
 */
function data_uri_creator_form($form, &$form_state) {
  $form['intro'] = array('#markup' => ('<p>' . t('Use the tool below to create a Data URI representation of a file. For example, images can be embedded directly in HTML using this data representation.') . '</p>'));
  $form[_DATA_URI_CREATOR_FORM_FIELD_DATA_FILE] = array(
    '#title' => t('Data File'),
    '#type' => 'file',
    '#description' => t('Upload the file that you want to convert into a <a href="http://en.wikipedia.org/wiki/Data_URI_scheme" target="_blank">Data URI</a> (<a href="http://tools.ietf.org/html/rfc2397" target="_blank">RFC 2397</a>).'),
    '#size' => 40,
    '#attributes' => array('name' => _DATA_URI_CREATOR_FORM_FIELD_DATA_FILE),
    '#attached' => array(
      'js' => array(
        array(
          'data' => array(
            'dataUriCreator' => array(
              'ajaxButtonName' => _DATA_URI_CREATOR_FORM_BUTTON_UPLOAD,
              'dataUriName' => _DATA_URI_CREATOR_FORM_FIELD_DATA_URI,
              'resultWrapperId' => _DATA_URI_CREATOR_AJAX_WRAPPER_UPLOAD,
              'imageButtonName' => _DATA_URI_CREATOR_FORM_BUTTON_IMAGE,
            ),
          ),
          'type' => 'setting',
        ),
        (drupal_get_path('module', 'data_uri_creator') . '/data_uri_creator_page.js'),
      ),
    ),
  );
  $max_file_size = (integer) DataUriCreatorSettings::getMaximumFileSize();
  if ($max_file_size > 0) {
    $form[_DATA_URI_CREATOR_FORM_FIELD_DATA_FILE]['#description'] .= ('<br />' . t('Note: The maximum allowed file size is %maxsize.', array('%maxsize' => format_size($max_file_size))));
    $form['MAX_FILE_SIZE'] = array(
      '#attributes' => array('name' => 'MAX_FILE_SIZE'),
      '#type' => 'hidden',
      '#value' => $max_file_size,
    );
  }

  $form[_DATA_URI_CREATOR_FORM_CHECKBOX_BASE64] = array(
    '#title' => t('Use base64 encoding'),
    '#type' => 'checkbox',
    '#default_value' => 1,
  );

  $form[_DATA_URI_CREATOR_FORM_BUTTON_UPLOAD] = array(
    '#type' => 'submit',
    '#name' => _DATA_URI_CREATOR_FORM_BUTTON_UPLOAD,
    '#value' => t('Create Data URI'),
    '#attributes' => array(
      'title' => t('Upload the file to generate the Data URI.'),
    ),
    '#ajax' => array(
      'callback' => 'data_uri_creator_form_ajax',
      'wrapper' => _DATA_URI_CREATOR_AJAX_WRAPPER_UPLOAD,
      'progress' => array(
        'type' => 'throbber',
        'message' => 'Uploading and converting file...',
      ),
    ),
  );

  $file_result = array(
    '#prefix' => '<div id="' . _DATA_URI_CREATOR_AJAX_WRAPPER_UPLOAD . '">',
    '#suffix' => '</div>',
  );
  $form[_DATA_URI_CREATOR_AJAX_CONTENT_UPLOAD] = &$file_result;
  $uploaded_file = &_data_uri_creator_uploaded_file();
  $uploaded_mime_type = &_data_uri_creator_uploaded_mime_type();
  if ((!empty($uploaded_file)) && (!empty($uploaded_mime_type))) {
    $use_base64_encoding = (isset($form_state['values'][_DATA_URI_CREATOR_FORM_CHECKBOX_BASE64]) ? (bool) $form_state['values'][_DATA_URI_CREATOR_FORM_CHECKBOX_BASE64] : TRUE);
    $data_uri = DataUriCreator::encodeFile($uploaded_file, $uploaded_mime_type, $use_base64_encoding);
    $file_result[_DATA_URI_CREATOR_FORM_FIELD_DATA_URI] = array(
      '#attributes' => array('name' => _DATA_URI_CREATOR_FORM_FIELD_DATA_URI),
      '#title' => t('Data URI'),
      '#description' => t('The uploaded data can be represented as a Data URI using this text. To use it, <em>Select All</em> text in the field, <em>Copy</em>, and then <em>Paste</em> the value where needed.'),
      '#type' => 'textarea',
      '#disabled' => FALSE,
      '#value' => $data_uri,
    );
    $file_result['data_uri_launch'] = array(
      '#type' => 'button',
      '#value' => t('Browse Data'),
      '#attributes' => array(
        'title' => t('Preview the Data URI in the web browser (opens in a new tab or window; not supported by all browsers).'),
        'onclick' => ('window.open(jQuery("[name=' . _DATA_URI_CREATOR_FORM_FIELD_DATA_URI . ']").val(), "_blank"); return false;'),
      ),
    );
    if (DataUriCreatorString::startsWith($uploaded_mime_type, 'image/')) {
      $file_result[_DATA_URI_CREATOR_FORM_BUTTON_IMAGE] = array(
        '#type' => 'button',
        '#value' => t('View Image'),
        '#attributes' => array(
          'name' => _DATA_URI_CREATOR_FORM_BUTTON_IMAGE,
          'title' => t('Display the Data URI as an image below.'),
          'onclick' => ('jQuery("#uploaded_image").each(function() { this.src = jQuery("[name=' . _DATA_URI_CREATOR_FORM_FIELD_DATA_URI . ']").val(); }); jQuery("#image_container").show(); return false;'),
          'style' => 'display:none;',
        ),
        '#suffix' => '<div id="image_container" style="display:none;"><img title="Uploaded image" id="uploaded_image" /></div>',
      );
    }
  }

  return $form;
}

/**
 * Form validation handler for data_uri_creator_form().
 *
 * It executes when a file is uploaded through the main Data URI Creator page.
 *
 * @param array $form
 *   The form's current render-array.
 * @param array $form_state
 *   Reference to the current form state.
 *
 * @see data_uri_creator_form()
 * @see data_uri_creator_form_submit()
 * @see data_uri_creator_form_ajax()
 */
function data_uri_creator_form_validate($form, &$form_state) {
  $uploaded_file = &_data_uri_creator_uploaded_file();
  $uploaded_file = NULL;
  $uploaded_mime_type = &_data_uri_creator_uploaded_mime_type();
  $uploaded_mime_type = NULL;
  $error_message = NULL;
  _data_uri_creator_add_file_upload_error_messages();
  $data_file_upload = (isset($_FILES[_DATA_URI_CREATOR_FORM_FIELD_DATA_FILE]) ? $_FILES[_DATA_URI_CREATOR_FORM_FIELD_DATA_FILE] : NULL);
  if (isset($data_file_upload['error_message'])) {
    $error_message = t('@error_message', array('@error_message' => $data_file_upload['error_message']));
  }
  elseif (isset($data_file_upload['error']) && ($data_file_upload['error'] == UPLOAD_ERR_OK)) {
    $data_file = NULL;
    if (isset($data_file_upload['tmp_name']) && (!empty($data_file_upload['tmp_name']))) {
      $data_file = $data_file_upload['tmp_name'];
    }

    $max_file_size = (integer) DataUriCreatorSettings::getMaximumFileSize();
    if (($max_file_size > 0) && isset($data_file_upload['size']) && ($data_file_upload['size'] > $max_file_size)) {
      $error_message = t('The uploaded file size must be @max_file_size bytes or less!', array('@max_file_size' => (string) $max_file_size));
    }
    elseif (!empty($data_file)) {
      $uploaded_file = $data_file;
      $uploaded_mime_type = (isset($data_file_upload['type']) ? $data_file_upload['type'] : NULL);
    }
  }

  if (!empty($error_message)) {
    form_set_error(_DATA_URI_CREATOR_FORM_FIELD_DATA_FILE, check_plain($error_message));
  }
  elseif (empty($uploaded_mime_type)) {
    form_set_error(_DATA_URI_CREATOR_FORM_FIELD_DATA_FILE, t("The upload file's MIME type is not known!"));
  }
  elseif (!isset($uploaded_file)) {
    form_set_error(_DATA_URI_CREATOR_FORM_FIELD_DATA_FILE, t('Unhandled upload error!'));
  }
}

/**
 * Form submission handler for data_uri_creator_form().
 *
 * It gets called once the file upload has been validated completely for the
 * main Data URI Creator page.  This function gets called only if the data
 * that was submitted for the file upload was valid.
 *
 * @param array $form
 *   The form's current render-array.
 * @param array $form_state
 *   Reference to the current form state.
 *
 * @see data_uri_creator_form()
 * @see data_uri_creator_form_validate()
 * @see data_uri_creator_form_ajax()
 */
function data_uri_creator_form_submit($form, &$form_state) {
  // Rebuild the form with the current file data.
  $form_state['rebuild'] = TRUE;
}

/**
 * AJAX-callback handler for data_uri_creator_form().
 *
 * It is called once the file-upload web-request completes for the Data URI
 * Creator page.  This function gets called for every AJAX file-upload request,
 * irrespective of success; however, this function is not called if the upload
 * request was made without JavaScript support (in which case the full form
 * from the rebuild step is returned).
 *
 * @param array $form
 *   The form's current render-array.
 * @param array $form_state
 *   Reference to the current form state.
 *
 * @return array
 *   An array representing the AJAX response, either a render-array fragment
 *   or an array of AJAX commands.
 *
 * @see data_uri_creator_form()
 * @see data_uri_creator_form_validate()
 * @see data_uri_creator_form_submit()
 */
function data_uri_creator_form_ajax($form, &$form_state) {
  // Select the region to be replaced by the AJAX response.
  $output = $form[_DATA_URI_CREATOR_AJAX_CONTENT_UPLOAD];

  // If any validation errors occurred, only handle those.
  $errors = form_get_errors();
  if (!empty($errors)) {
    // Include default error handling, so as to display error messages.
    return $output;
  }

  // Render the output for the uploaded file.
  $commands = array();
  $commands[] = ajax_command_insert(NULL, drupal_render($output));
  $commands[] = ajax_command_invoke(NULL, 'dataUriCreatorResultComplete');

  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * Obtains a descriptive error-message for the specified upload error code.
 *
 * @param int $error_code
 *   The error code associated with the file upload, such as UPLOAD_ERR_OK
 *   or UPLOAD_ERR_NO_FILE.
 * @param string|null $success_message
 *   (optional) A default value for the success code (for UPLOAD_ERR_OK),
 *   which defaults to NULL.
 *
 * @return string|null
 *   An descriptive string (not translated) that corresponds with the error
 *   code; or the success message that defaults to NULL.
 */
function _data_uri_creator_get_file_upload_error_message(
  $error_code, $success_message = NULL) {

  switch ($error_code) {
    case UPLOAD_ERR_OK:
      // Error code 0: There is no error; the file uploaded successfully.
      return $success_message;

    case UPLOAD_ERR_INI_SIZE:
      // Error code 1.
      $error_message = 'The uploaded file exceeds the upload_max_filesize directive in "php.ini"';
      break;

    case UPLOAD_ERR_FORM_SIZE:
      // Error code 2.
      $error_message = 'The uploaded file exceeds the MAX_FILE_SIZE directive that was specified in the HTML form';
      break;

    case UPLOAD_ERR_PARTIAL:
      // Error code 3.
      $error_message = 'The uploaded file was only partially uploaded';
      break;

    case UPLOAD_ERR_NO_FILE:
      // Error code 4.
      $error_message = 'No file was uploaded';
      break;

    case UPLOAD_ERR_NO_TMP_DIR:
      // Error code 6.
      $error_message = 'Missing a temporary folder';
      break;

    case UPLOAD_ERR_CANT_WRITE:
      // Error code 7.
      $error_message = 'Failed to write file to disk';
      break;

    case UPLOAD_ERR_EXTENSION:
      // Error code 8.
      $error_message = 'File upload stopped by a PHP extension';
      break;

    default:
      $error_message = 'Unknown file upload error';
      break;
  }

  return ($error_message . ' (' . $error_code . ')');
}

/**
 * Adds descriptive error messages to the files array for upload errors.
 */
function _data_uri_creator_add_file_upload_error_messages() {
  if (isset($_FILES)) {
    foreach ($_FILES as &$value) {
      $error_message = _data_uri_creator_get_file_upload_error_message($value['error']);
      if (isset($error_message)) {
        $value['error_message'] = $error_message;
      }
    }
  }
}

/**
 * Provides access to the form's variable that stores the uploaded file.
 */
function &_data_uri_creator_uploaded_file() {
  $uploaded_file = &drupal_static('data_uri_creator_uploaded_file');
  return $uploaded_file;
}

/**
 * Provides access to the form's variable storing the MIME-type of the upload.
 */
function &_data_uri_creator_uploaded_mime_type() {
  $uploaded_mime_type = &drupal_static('data_uri_creator_uploaded_mime_type');
  return $uploaded_mime_type;
}
