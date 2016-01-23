<?php

/**
 * @file
 * Provides autoload mecanism for smalot/magento-client API.
 */

// Include interfaces.
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/ActionInterface.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/MultiCallQueueInterface.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/RemoteAdapterInterface.php';
// Include exceptions.
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/RemoteAdapterException.php';
// Include abstracts.
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/MagentoModuleAbstract.php';
// Include core classes.
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/Action.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/MultiCallQueue.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/RemoteAdapter.php';
// Include action classes.
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/Cart/CartCoupon.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/Cart/CartCustomer.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/Cart/CartPayment.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/Cart/Cart.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/Cart/CartProduct.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/Cart/CartShipping.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/Catalog/CategoryAttributes.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/Catalog/Category.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/CatalogInventory/StockItem.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/Catalog/ProductAttribute.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/Catalog/ProductAttributeSet.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/Catalog/ProductCustomOption.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/Catalog/ProductCustomOptionValue.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/Catalog/ProductDownloadableLink.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/Catalog/ProductLink.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/Catalog/ProductMedia.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/Catalog/Product.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/Catalog/ProductTag.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/Catalog/ProductTierPrice.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/Catalog/ProductTypes.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/Core/Magento.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/CustomerBalance/StoreCredit.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/CustomerBalance/StoreCreditQuote.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/Customer/CustomerAddress.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/Customer/CustomerGroup.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/Customer/Customer.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/Directory/Directory.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/GiftMessage/GiftMessage.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/Order/OrderCreditMemo.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/Order/OrderInvoice.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/Order/Order.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/Order/OrderShipment.php';
require_once dirname(__FILE__) . '/lib/src/Smalot/Magento/Store/Store.php';

/**
 * Api function to get provider list.
 *
 * return array
 *   List of providers.
 */
function magento_api_provider_list() {
  return variable_get('magento_providers', array());
}

/**
 * Api function to get provider.
 *
 * @param string $code
 *   Code of the provider.
 * @param bool   $debug
 *   Enable debug mode.
 *
 * @return mixed
 *   Provider object.
 */
function magento_api_provider_get($code = 'default', $debug = FALSE) {
  static $providers;

  if (!isset($providers[$code])) {
    $magento_providers = magento_api_provider_list();

    if (isset($magento_providers[$code]) && $magento_provider = $magento_providers[$code]) {
      try {
        if ($debug) {
          // Debug oriented adapter.
          $provider = new MagentoDrupalRemoteAdapter(
            $magento_provider['url'],
            $magento_provider['api_user'],
            $magento_provider['api_key']
          );
        }
        else {
          // Standard adapter.
          $provider = new \Smalot\Magento\RemoteAdapter(
            $magento_provider['url'],
            $magento_provider['api_user'],
            $magento_provider['api_key']
          );
        }
      }
      catch (Exception $e) {
        watchdog('Magento', $e->getMessage(), array(), WATCHDOG_CRITICAL);
        $provider = NULL;
      }

      $providers[$code] = $provider;
    }
    else {
      watchdog('Magento', 'Missing provider.', array(), WATCHDOG_CRITICAL);
    }
  }

  return $providers[$code];
}

/**
 * Drupal override to add debug features.
 */
class MagentoDrupalRemoteAdapter extends \Smalot\Magento\RemoteAdapter {
  /**
   * Login method.
   *
   * @param string $api_user
   *   ApiUser on Magento.
   * @param string $api_key
   *   ApiKey on Magento.
   *
   * @return bool
   *   Login success.
   * @throws \Exception
   */
  public function login($api_user = NULL, $api_key = NULL) {
    $t        = microtime(TRUE);
    $result   = parent::login($api_user, $api_key);
    $duration = microtime(TRUE) - $t;
    watchdog(
      'Magento',
      '[Perfs] Login call duration: !duration ms',
      array('!duration' => round($duration * 1000)),
      WATCHDOG_DEBUG
    );

    return $result;
  }

  /**
   * Logout method.
   *
   * @return bool
   *   Logout success.
   */
  public function logout() {
    $t        = microtime(TRUE);
    $result   = parent::logout();
    $duration = microtime(TRUE) - $t;
    watchdog(
      'Magento',
      '[Perfs] Logout call duration: !duration ms',
      array('!duration' => round($duration * 1000)),
      WATCHDOG_DEBUG
    );

    return $result;
  }

  /**
   * Single call method.
   *
   * @param \Smalot\Magento\ActionInterface $action
   *   Action.
   * @param bool                            $throws_exception
   *   Throws exceptions.
   *
   * @return array|false
   *   Result.
   * @throws Exception
   */
  public function call(\Smalot\Magento\ActionInterface $action, $throws_exception = TRUE) {
    $t        = microtime(TRUE);
    $result   = parent::call($action, $throws_exception);
    $duration = microtime(TRUE) - $t;
    watchdog(
      'Magento',
      '[Perfs] Single call duration: !duration ms, for "@name" method',
      array('@name' => $action->getMethod(), '!duration' => round($duration * 1000)),
      WATCHDOG_DEBUG
    );

    return $result;
  }

  /**
   * Multi call method.
   *
   * @param \Smalot\Magento\MultiCallQueueInterface $queue
   *   Queue containing actions.
   * @param bool                                    $throws_exception
   *   Throws exception.
   *
   * @return array|false
   *   Result.
   * @throws \Exception
   */
  public function multiCall(\Smalot\Magento\MultiCallQueueInterface $queue, $throws_exception = TRUE) {
    $t        = microtime(TRUE);
    $result   = parent::multiCall($queue, $throws_exception);
    $duration = microtime(TRUE) - $t;
    watchdog(
      'Magento',
      '[Magento] Multi call duration: !duration ms, for !count elements',
      array('!count' => $queue->count(), '!duration' => round($duration * 1000)),
      WATCHDOG_DEBUG
    );

    return $result;
  }
}
