<?php

/**
 * Import Private Messages to the privatemsg module
 *
 * @author DarkSnow.net
 * @file
 * Import PHPBB private messages into the Drupal privatemsg system
 */
class PhpbbPrivateMessageMigration extends PhpbbMigration {
 static $phpbb_type = 'PrivateMessage';
 public function __construct($settings_machine_name) {
    parent::__construct($settings_machine_name);
    $this->description = t('Migrate private messages to the privatemsg module.');
    $this->dependencies = array($this->getMigrationName('User'));

    $this->source = new MigrateSourceSQL($this->query());

    $this->destination = new MigrateDestinationPrivateMsg(
      array('text_format' => $this->importFormat)
    );

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'msg_id' => array('type' => 'int', 'unsigned' => TRUE, 'not null' => TRUE)
      ),
      MigrateDestinationPrivateMsg::getKeySchema(), $this->migration_settings['machine_name']
    );

    $this->addFieldMapping('subject', 'message_subject');
    $this->addFieldMapping('body', 'message_text');
    $this->addFieldMapping('format')->defaultValue($this->importFormat);
    $this->addFieldMapping('recipients', 'to_address')
      ->separator(PHPBB_LIST_SEPARATOR)
      ->sourceMigration($this->getMigrationName('User'));
    $this->addFieldMapping('timestamp', 'message_time');
    $this->addFieldMapping('author', 'author_id')
      ->sourceMigration($this->getMigrationName('User'))->defaultValue(1);
    $this->addFieldMapping('is_new')->defaultValue(1);

    $this->addFieldMapping(NULL, 'bbcode_uid')->issueGroup(t('DNM')); // Used to santise body text

    // Unmapped destination fields
    $this->addUnmigratedDestinations(array('path'));
  }

  /**
   * Method to return the query used in the import. 
   * Can be called by sub classes and extended if needed.
   */
  protected function query() {
   return Database::getConnection('default', $this->migration_settings['machine_name'])
   ->select('{privmsgs}', 'p')
   ->fields('p', array(
     'msg_id', 'author_id', 'message_time',
     //'message_edit_time', 'message_edit_user',
     'message_subject', 'message_text',
     'bbcode_uid', 'to_address'
   ))
   ->orderBy('message_time');
  }
  
  function prepareRow($message) {
    // Populate to_address with all recipients of this message
    $recipients = array();
    $query = Database::getConnection('default', $this->migration_settings['machine_name'])
      ->select('{privmsgs_to}', 'pt')
      ->fields('pt', array('user_id'))
      ->condition('msg_id', $message->msg_id);
    
    foreach ($query->execute() as $to) {
      if ($to->user_id != $message->author_id) {
        array_push($recipients, $to->user_id);
      }
    }
    
    
    if (count($recipients) == 0 && !empty($message->to_address)) {
      // If there's no entry in the pm_to table, use the to_address field instead
      array_push($recipients, drupal_substr($message->to_address, 2));
    }
   
    if (count($recipients) == 0 || $recipients[0] == NULL) {
      // If there's still nobody to send it to, it's an invalid message
      return FALSE;
    }
    $message->to_address = implode(PHPBB_LIST_SEPARATOR, $recipients);
    
    
    
    // remove the :bbcode_uid from message_text
    if (!empty($message->bbcode_uid)) {
      $message->message_text = preg_replace("/:$message->bbcode_uid/", '', $message->message_text);
    }
    $message->message_text = PhpbbContentHelpers::stripBbcode($message->message_text);
    $message->message_text = PhpbbContentHelpers::textSanitise($message->message_text);
    $message->message_subject = PhpbbContentHelpers::textSanitise($message->message_subject);
  }
  
  function prepare(&$message,&$row) {
    //if all recipents are deleted then send to annon user
    if(!sizeof($message->recipients)) {
      $message->recipients[] = 0;
    }
  }
}
