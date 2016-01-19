<?php
/**
 * @file
 * Connection testing class.
 */

namespace Drupal\clients_suitecrm\Clients\Connection\Test;

/**
 * Connection testing class.
 */
class SuiteCrmConnection implements \ClientsConnectionTestingInterface {

  /**
   * {@inheritdoc}
   */
  public function testLabels() {
    return array(
      'label' => t('Test connection'),
      'description' => t('Test the basic connection to the SuiteCRM by calling get_server_info(). No credentials needed.'),
      'button' => t('Connect'),
    );
  }

  /**
   * {@inheritdoc}
   */
  public function test($connection, &$button_form_values) {
    try {
      // Call the connect method.
      $connect = $connection->get_server_info();
    }
    catch (Exception $e) {
      drupal_set_message(t('Could not connect to the remote site, got error message "@message".', array(
        '@message' => $e->getMessage(),
      )), 'warning');
      return;
    }

    if (is_object($connect) && isset($connect->version)) {
      drupal_set_message(t('Sucessfully connected to the remote site.'));
    }
    else {
      drupal_set_message(t('Could not connect to the remote site.'), 'warning');
      $connect = $connection->getLastError();
    }

    return $connect;
  }

}
