<?php

libraries_load('aws-sdk-php');
use Aws\S3\S3Client;
use Aws\S3\Exception\S3Exception;

/**
 * @file
 * Functions to handle the dropbox backup destination.
 */

/**
 * A destination for sending database backups to a Dropbox account.
 *
 * @ingroup backup_migrate_destinations
 */
class backup_migrate_destination_s3_compatible extends backup_migrate_destination_remote {

  var $supported_ops = array(
    'scheduled backup',
    'manual backup',
    'remote backup',
    'restore',
    'list files',
    'configure',
    'delete',
  );

  var $default_values = array(
    'settings' => array(
      'file_directory' => '',
      's3_timeout' => '',
      's3_proxy' => '',
      's3_min_part_size' => '',
    ),
  );

  var $s3 = NULL;

  /**
   * S3 getters
   */
  function s3_host() {
    return $this->dest_url['scheme'] . '://' . $this->dest_url['host'];
  }
  function s3_key() {
    return $this->dest_url['user'];
  }
  function s3_secret() {
    return $this->dest_url['pass'];
  }
  function s3_bucket() {
    return $this->dest_url['path'];
  }

  /**
   * Generate a filepath with the correct prefix.
   */
  function s3_path($filename) {
    $path = '';
    if (empty($filename)) {
      $path .= '/';
    }
    if (!empty($this->settings['file_directory'])) {
      $path .= $this->settings['file_directory'];
    }
    if (!empty($filename)) {
      $path .= '/';
    }
    $path .= $filename;

    return $path;
  }

  /**
   * S3 Init.
   **/
  function s3_init() {
    // Establish connection with DreamObjects with an S3 client.
    $config = array(
      'base_url' => $this->s3_host(),
      'key'      => $this->s3_key(),
      'secret'   => $this->s3_secret(),
    );

    if (!empty($this->settings['s3_proxy'])) {
      $config['request.options'] = array(
        'proxy' => $this->settings['s3_proxy'],
      );
    }

    if (!empty($this->settings['s3_timeout'])) {
      $config['curl.options'] = array(
        CURLOPT_TIMEOUT => $this->settings['s3_timeout'],
      );
    }

    $this->s3 = S3Client::factory($config);
  }

  /**
   * Save to to the s3 destination.
   */
  function _save_file($file, $settings) {
    $this->s3_init();

    $options = array();

    if (!empty($this->settings['s3_min_part_size'])) {
      $options['min_part_size'] = $this->settings['s3_min_part_size'] * 1024 * 1024;
    }

    try {
      $this->s3->upload(
        $this->s3_bucket(),
        $this->s3_path($file->filename()),
        fopen($file->filepath(), 'r'),
        'private',
        $options
      );
    }
    catch (S3Exception $e) {
      $e_msg = 'S3 error saving %file - %code %error';
      $e_args = array('%file' => $file->filename(), '%error' => $e->getMessage(), '%code' => $e->getAwsErrorCode());
      drupal_set_message(t($e_msg, $e_args), 'error');
      watchdog('backup_migrate_s3', $e_msg, $e_args, WATCHDOG_ERROR);
    }

    return $file;
  }

  /**
   * Load from the s3 destination.
   */
  function load_file($file_id) {
    backup_migrate_include('files');
    $file = new backup_file(array('filename' => $file_id));
    $this->s3_init();
    try {
      $this->s3->getObject(array(
        'Bucket' => $this->s3_bucket(),
        'Key'    => $this->s3_path($file->filename()),
        'SaveAs' => $file->filepath(),
      ));
    }
    catch (S3Exception $e) {
      $e_msg = 'S3 error loading %file - %code %error';
      $e_args = array('%file' => $file_id, '%error' => $e->getMessage(), '%code' => $e->getAwsErrorCode());
      drupal_set_message(t($e_msg, $e_args), 'error');
      watchdog('backup_migrate_s3', $e_msg, $e_args, WATCHDOG_ERROR);
    }

    return $file;
  }

  /**
   * List all files from the s3 destination.
   */
  function _list_files() {
    $this->s3_init();
    $files = array();
    backup_migrate_include('files');

    try {
      $o_iter = $this->s3->getIterator('ListObjects', array(
        'Bucket' => $this->s3_bucket(),
        'Prefix' => $this->settings['file_directory'] . '/',
      ));
      foreach ($o_iter as $o) {
        $info = array(
          'filename' => basename($o['Key']),
          'filesize' => $o['Size'],
          'filetime' => $o['LastModified'],
        );
        $files[$info['filename']] = new backup_file($info);
      }
    }
    catch (S3Exception $e) {
      $e_msg = 'S3 error listing files - %code %error';
      $e_args = array('%error' => $e->getMessage(), '%code' => $e->getAwsErrorCode());
      drupal_set_message(t($e_msg, $e_args), 'error');
      watchdog('backup_migrate_s3', $e_msg, $e_args, WATCHDOG_ERROR);
    }

    return $files;
  }

  /**
   * Delete from the s3 destination.
   */
  function _delete_file($file_id) {
    $this->s3_init();
    try {
      $this->s3->deleteObject(array(
        'Bucket' => $this->s3_bucket(),
        'Key'    => $this->s3_path($file_id),
      ));
    }
    catch (S3Exception $e) {
      drupal_set_message(t('There was an error deleting the remote file.'));
    }
  }

  /**
   * Validate the edit form for the item.
   */
  function edit_form_validate($form, &$form_state) {
    parent::edit_form_validate($form, $form_state);

    if (!class_exists('Aws\S3\S3Client')) {
      $requirements = backup_migrate_s3_requirements('runtime');
      $e_msg = t('Library not found or cannot be loaded.');
      if ($requirements['backup_migrate_s3']['severity'] == REQUIREMENT_OK) {
        $e_msg .= ' ' . t('You might have to <a href="!cache">clear cache</a> if you installed the library after enabling this module.', array('!cache' => url('admin/config/development/performance')));
      }
      else {
        $e_msg .= ' ' . $requirements['backup_migrate_s3']['description'];
      }
      form_set_error('', $e_msg);
    }
    else {
      // Do not attempt validation of there are erros in the form.
      if (form_get_errors()) return;

      try {
        $config = array(
          'base_url' => $form_state['values']['scheme'] . '://' . $form_state['values']['host'],
          'key'      => $form_state['values']['user'],
          'secret'   => empty($form_state['values']['pass']) ? $form_state['values']['old_password'] : $form_state['values']['pass'],
        );

       if (!empty($form_state['values']['s3_proxy'])) {
          $config['request.options'] = array(
            'proxy' => $form_state['values']['s3_proxy'],
          );
        }

        if (!empty($form_state['values']['s3_timeout'])) {
          $config['curl.options'] = array(
            CURLOPT_TIMEOUT => $form_state['values']['s3_timeout'],
          );
        }

        $this->s3 = S3Client::factory($config);

        $o_iter = $this->s3->getIterator('ListObjects', array(
          'Bucket' => $form_state['values']['path'],
          'Prefix' => $form_state['values']['file_directory'] . '/',
        ));
        foreach ($o_iter as $o) {
        }
      }
      catch (Exception $e) {
        $e_msg = 'There was an S3 error attempting to validate the settings below - %code %error';
        $e_args = array('%error' => $e->getMessage(), '%code' => $e->getCode());
        if (method_exists($e, 'getAwsErrorCode')) {
          $e_args['%code'] = $e->getAwsErrorCode();
        }
        form_set_error('', t($e_msg, $e_args));
      }
    }

  }

  /**
   * Validate the edit form for the item.
   */
  function edit_form_submit($form, &$form_state) {
    $this->settings['file_directory'] = $form_state['values']['file_directory'];
    $this->settings['s3_timeout'] = $form_state['values']['s3_timeout'];
    $this->settings['s3_proxy'] = $form_state['values']['s3_proxy'];
    $this->settings['s3_min_part_size'] = $form_state['values']['s3_min_part_size'];

    parent::edit_form_submit($form, $form_state);
  }

  /**
   * Element validate callback for the file destination field.
   *
   * Remove slashes from the beginning and end of the destination value and ensure
   * that the file directory path is not included at the beginning of the value.
   *
   * @see _file_generic_settings_file_directory_validate().
   */
  function file_directory_validate($element, &$form_state) {
    // Strip slashes from the beginning and end of $widget['file_directory'].
    $value = trim($element['#value'], '\\/');
    form_set_value($element, $value, $form_state);
  }

  function numeric_validate($element, &$form_state) {
    // Strip slashes from the beginning and end of $widget['file_directory'].
    if ($element['#value'] && !is_numeric($element['#value'])) {
      form_error($element, t('@fieldname must be numeric.', array('@fieldname' => $element['#title'])));
    }
  }

  /**
   * Get the form for the settings for this filter.
   */
  function edit_form() {
    $form = parent::edit_form();
    $form['scheme']['#type'] = 'value';
    $form['scheme']['#value'] = 'https';

    $form['host']['#default_value'] = $form['host']['#default_value'] == 'localhost' ? '' : $form['host']['#default_value'];
    $form['host']['#description'] = t('Enter the S3 compatible host, i.e. s3.amazonaws.com, objects.dreamhost.com, etc.');

    $form['path']['#title'] = 'S3 Bucket';
    //$form['path']['#default_value'] = $this->get_bucket();
    $form['path']['#description'] = 'This bucket must already exist. It will not be created for you.';

    $form['user']['#title'] = 'Access Key ID';
    $form['pass']['#title'] = 'Secret Access Key';

    $form['pass']['#required'] = empty($this->dest_url);

    $form['file_directory'] = array(
      '#type' => 'textfield',
      '#title' => t('File directory'),
      '#default_value' => $this->settings['file_directory'],
      '#description' => t('Optional subdirectory within the s3 bucket where files will be stored. Do not include preceding or trailing slashes.'),
      '#element_validate' => array(array($this, 'file_directory_validate')),
      '#weight' => 25,
    );

    $form['s3_advanced'] = array(
      '#type' => 'fieldset',
      '#title' => t('Advanced'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => 50,
    );
    $form['s3_advanced']['s3_timeout'] = array(
      '#type' => 'textfield',
      '#title' => t('Timeout'),
      '#default_value' => $this->settings['s3_timeout'],
      '#description' => t('Optional timeout for S3 requests, useful if you are getting timeout exceptions. If empty, the internal default is used, which is normally PHP configuration <em>default_socket_timeout</em>, which is currently set to %value seconds.', array('%value' => ini_get('default_socket_timeout'))),
      '#element_validate' => array(array($this, 'numeric_validate')),
    );
    $form['s3_advanced']['s3_min_part_size'] = array(
      '#type' => 'textfield',
      '#title' => t('Minimum part size'),
      '#default_value' => $this->settings['s3_min_part_size'],
      '#description' => t('Minimum size to allow for each uploaded part when performing a multipart upload, in MegaBytes.'),
      '#element_validate' => array(array($this, 'numeric_validate')),
    );
    $form['s3_advanced']['s3_proxy'] = array(
      '#type' => 'textfield',
      '#title' => t('Proxy'),
      '#default_value' => $this->settings['s3_proxy'],
      '#description' => t('Optional proxy to pass through to the S3 Client library. It should be specified in the <em><b>HOST:PORT</b></em> format.'),
    );

    return $form;
  }}
