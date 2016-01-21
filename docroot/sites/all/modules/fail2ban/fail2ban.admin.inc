<?php
/**
 * @file
 */

/**
 * Return the settings form.
 */
function fail2ban_admin_settings() {
  $form = array();

  $settings = variable_get('fail2ban', fail2ban_defaults());

  $form['fail2ban'] = array('#tree' => TRUE);

  $form['fail2ban']['logstring'] = array(
    '#title' => 'Log String',
    '#type' => 'textfield',
    '#default_value' => $settings['logstring'],
    '#description' => t('Enter the log message that the fail2ban module should write to syslog. Your fail2ban utility should be configured to look and act upon a message in this format. The originating IP address is inserted via the !address placeholder.'),
  );

  $form['fail2ban']['whitelist'] = array(
    '#title' => 'Address Whitelist',
    '#type' => 'textarea',
    '#default_value' => $settings['whitelist'],
    '#description' => t('Add any IP addresses or netmasks that should be whitelisted, one per line. It is a <em>really</em> good idea to add your own IP address or network to this field.'),
  );

  $form['fail2ban']['logopt'] = array(
    '#type' => 'fieldset',
    '#title' => 'Logging Options',
    '#description' => t('Set various syslog() options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['fail2ban']['logopt']['identifier'] = array(
    '#title' => 'Identifier',
    '#type' => 'textfield',
    '#default_value' => $settings['logopt']['identifier'],
    '#description' => t('Specify the identifier to use for the syslog message.'),
  );

  $options = array(
    LOG_CONS => 'LOG_CONS: if there is an error while sending data to the system logger, write directly to the system console',
    LOG_NDELAY => 'LOG_NDELAY: open the connection to the logger immediately',
    LOG_ODELAY => 'LOG_ODELAY: delay opening the connection until the first message is logged',
    LOG_PERROR => 'LOG_PERROR: print log message also to standard error',
    LOG_PID => 'LOG_PID: include PID with each message',
  );
  $form['fail2ban']['logopt']['options'] = array(
    '#title' => 'Options',
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => $settings['logopt']['options'],
    '#description' => t('Set logging options to use for the syslog message.'),
  );

  // Windows only has a single facility and I doubt very much it has fail2ban, but
  // lets not make it whine about lacking syslog facilities, just in case.
  if( strtoupper (substr(PHP_OS, 0,3)) == 'WIN' ) {
    $facilities = array(LOG_USER => 'LOG_USER: generic user-level messages');
  }
  else {
    // According to http://php.net/manual/en/function.openlog.php LOG_AUTHPRIV should be
    // used in preference over LOG_AUTH whenever it is available. So let's check.
    //
    if (defined('LOG_AUTHPRIV')) {
      $facilities = array(LOG_AUTHPRIV => 'LOG_AUTHPRIV: security/authorization messages (private)');
    }
    else {
      $facilities = array(LOG_AUTH => 'LOG_AUTH: security/authorization messages');
    }

    // Add the rest.
    //
    $facilities += array(
      // LOG_CRON => 'LOG_CRON: clock daemon (cron and at)',
      LOG_DAEMON => 'LOG_DAEMON: other system daemons',
      // LOG_KERN => 'LOG_KERN: kernel messages',
      LOG_LOCAL0 => 'LOG_LOCAL0: reserved for local use, not available in Windows',
      LOG_LOCAL1 => 'LOG_LOCAL1: reserved for local use, not available in Windows',
      LOG_LOCAL2 => 'LOG_LOCAL2: reserved for local use, not available in Windows',
      LOG_LOCAL3 => 'LOG_LOCAL3: reserved for local use, not available in Windows',
      LOG_LOCAL4 => 'LOG_LOCAL4: reserved for local use, not available in Windows',
      LOG_LOCAL5 => 'LOG_LOCAL5: reserved for local use, not available in Windows',
      LOG_LOCAL6 => 'LOG_LOCAL6: reserved for local use, not available in Windows',
      LOG_LOCAL7 => 'LOG_LOCAL7: reserved for local use, not available in Windows',
      // LOG_LPR => 'LOG_LPR: line printer subsystem',
      // LOG_MAIL => 'LOG_MAIL: mail subsystem',
      // LOG_NEWS => 'LOG_NEWS: USENET news subsystem',
      LOG_SYSLOG => 'LOG_SYSLOG: messages generated internally by syslogd',
      LOG_USER => 'LOG_USER: generic user-level messages',
      // LOG_UUCP => 'LOG_UUCP: UUCP subsystem',
    );
  }

  $form['fail2ban']['logopt']['facility'] = array(
    '#title' => 'Facility',
    '#type' => 'select',
    '#options' => $facilities,
    '#default_value' => $settings['logopt']['facility'],
    '#description' => t('Select the facility that should used for syslog message written by the fail2ban module. This can help you filter your system logs.'),
  );

  $priorities = array(
    LOG_EMERG => 'LOG_EMERG',
    LOG_ALERT => 'LOG_ALERT',
    LOG_CRIT => 'LOG_CRIT',
    LOG_ERR => 'LOG_ERR',
    LOG_WARNING => 'LOG_WARNING',
    LOG_NOTICE => 'LOG_NOTICE',
    LOG_INFO => 'LOG_INFO',
    LOG_DEBUG => 'LOG_DEBUG'
  );
  $form['fail2ban']['logopt']['priority'] = array(
    '#title' => 'Priority',
    '#type' => 'checkboxes',
    '#options' => $priorities,
    '#default_value' => $settings['logopt']['priority'],
    '#description' => t('Select all priorities that should be assigned to the syslog message written by the fail2ban module. This can help you filter your system logs.'),
  );

  return system_settings_form($form);
}
