<?php
/**
 * @file
 * Defines mailhandler_media_nodes importer.
 */

/**
 * Implements hook_feeds_importer_default().
 */
function mail_media_feeds_importer_default() {
  $export = array();

// include generated code from the Feeds importer "export" function

//-- begin generated code --//

$feeds_importer = new stdClass;
$feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
$feeds_importer->api_version = 1;
$feeds_importer->id = 'mailhandler_media_nodes';
$feeds_importer->config = array(
  'name' => 'Mailhandler media nodes',
  'description' => 'Imports media nodes from a Mailhandler mailbox',
  'fetcher' => array(
    'plugin_key' => 'MailhandlerFetcher',
    'config' => array(
      'filter' => 'MailhandlerFiltersNodes',
    ),
  ),
  'parser' => array(
    'plugin_key' => 'MailhandlerParser',
    'config' => array(
      'if_auth_fails' => 'remove',
      'default_commands' => 'status: 1',
      'command_plugin' => array(
        'MailhandlerCommandsHeaders' => 'MailhandlerCommandsHeaders',
        'MailhandlerCommandsMailMediaLink' => 'MailhandlerCommandsMailMediaLink',
        'MailhandlerCommandsDefault' => 'MailhandlerCommandsDefault',
        'MailhandlerCommandsFiles' => 'MailhandlerCommandsFiles',
      ),
      'authenticate_plugin' => 'MailhandlerAuthenticateSendto',
      'available_commands' => 'status',
    ),
  ),
  'processor' => array(
    'plugin_key' => 'FeedsNodeProcessor',
    'config' => array(
      'content_type' => 'mail_media_import',
      'expire' => '-1',
      'author' => 0,
      'mappings' => array(
        0 => array(
          'source' => 'subject',
          'target' => 'title',
          'unique' => FALSE,
        ),
        1 => array(
          'source' => 'authenticated_uid',
          'target' => 'uid',
          'unique' => FALSE,
        ),
        2 => array(
          'source' => 'fromaddress',
          'target' => 'field_mail_media_from',
          'unique' => FALSE,
        ),
        3 => array(
          'source' => 'attachments',
          'target' => 'field_mail_media_attachment',
          'unique' => FALSE,
        ),
        4 => array(
          'source' => 'toaddress',
          'target' => 'field_mail_media_to',
          'unique' => FALSE,
        ),
        5 => array(
          'source' => 'mailmedia',
          'target' => 'field_mail_media_link',
          'unique' => FALSE,
        ),
        6 => array(
          'source' => 'body_text',
          'target' => 'body',
          'unique' => FALSE,
        ),
      ),
      'update_existing' => '0',
      'input_format' => 'plain_text',
    ),
  ),
  'content_type' => '',
  'update' => 0,
  'import_period' => '0',
  'expire_period' => 3600,
  'import_on_create' => 1,
  'process_in_background' => 0,
);

//--end generated code --//

  $export['mailhandler_media_nodes'] = $feeds_importer;

  return $export;
}
