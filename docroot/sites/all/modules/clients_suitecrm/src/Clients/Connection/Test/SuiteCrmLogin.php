<?php
/**
 * @file
 * Login testing class.
 */

namespace Drupal\clients_suitecrm\Clients\Connection\Test;

/**
 * Login testing class.
 */
class SuiteCrmLogin implements \ClientsConnectionTestingInterface {

  /**
   * {@inheritdoc}
   */
  public function testLabels() {
    return array(
      'label' => t('Test Login'),
      'description' => t('Test the login witht he configured credentials.'),
      'button' => t('Login'),
    );
  }

  /**
   * {@inheritdoc}
   */
  public function test($connection, &$button_form_values) {
    try {
      // Call the connect method.
      $logged_in = $connection->login();
      // Ensure this is a one time session.
      $connection->disconnect();
    }
    catch (Exception $e) {
      drupal_set_message(t('Could not login to the remote site, got error message "@message".', array(
        '@message' => $e->getMessage(),
      )), 'warning');
      return;
    }

    if ($logged_in) {
      drupal_set_message(t('Sucessfully logged into SuiteCRM.'));
    }
    else {
      drupal_set_message(t('Could not login into SuiteCRM.'), 'warning');
      $logged_in = $connection->getLastError();
    }

    return $logged_in;
  }
}
