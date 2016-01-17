<?php
/**
 * @file
 * Classes for Community Exchange Network Intertrading Protocol.
 */

/**
 * Extension of CesBank for CEN protocol.
 */
class CesInteropCENIPBank extends CesBank {
  /**
   * Creates a transaction object.
   *
   * If the record specifies this is a CENIP transaction, it returns a new
   * CesInteropCENIPTransaction object, otherwise it calls the parent and
   * returns a regular transaction.
   *
   * @return CesBankTransactionInterface
   *   The new transaction.
   */
  protected function newTransaction(array &$record) {
    if (!empty($record['cenip'])) {
      if (isset($record['toaccount']) || isset($record['toaccountname'])) {
        return $this->newOutgoingTransaction($record);
      }
      else {
        return $this->newIncomingTransaction($record);
      }
    }
    else {
      return parent::newTransaction($record);
    }
  }
  /**
   * Create remote outgoing transaction.
   */
  protected function newOutgoingTransaction(array &$record) {
    if (isset($record['toaccount'])) {
      $toaccount = $this->getObject('CesBankLocalAccount', $record['toaccount']);
    }
    else {
      $s = new IcesSerializer('CesBankLocalAccount');
      $toaccount = $s->loadFromUniqueKey('name', $record['toaccountname']);
    }
    $toexchange = $toaccount->getExchange();
    $remoteexchange = new CesInteropCENIPRemoteExchange($record['nid']);
    $basic = array(
      'fromaccount' => $toexchange->getVirtualAccount($remoteexchange)->id,
      'toaccount' => $toaccount->id,
      'amount' => $record['amount'],
      'concept' => $record['concept'],
      'user' => $record['user'],
      'state' => CesBankTransactionInterface::STATE_NEW,
      'data' => array(),
    );
    $cenip = array(
      'nid' => $record['nid'],
      'buyer_id' => $record['buyer_id'],
    );
    return new CesInteropCENIPOutgoingTransaction(new CesBankBasicTransaction($basic), $cenip);
  }
  /**
   * Create remote incoming transaction.
   */
  protected function newIncomingTransaction(array &$record) {
    $fromaccount = $this->getObject('CesBankLocalAccount', $record['fromaccount']);
    $fromexchange = $fromaccount->getExchange();
    $remoteexchange = new CesInteropCENIPRemoteExchange($record['nid']);
    $basic = array(
      'toaccount' => $fromexchange->getVirtualAccount($remoteexchange)->id,
      'fromaccount' => $fromaccount->id,
      'amount' => $record['amount'],
      'concept' => $record['concept'],
      'user' => $record['user'],
      'state' => CesBankTransactionInterface::STATE_NEW,
      'data' => array(),
    );
    $cenip = array(
      'txid' => $record['txid'],
      'nid' => $record['nid'],
      'seller_id' => $record['seller_id'],
      'seller_name' => $record['seller_name'],
      'seller_email' => $record['seller_email'],
      'seller_xname' => $record['seller_xname'],
      'seller_country' => $record['seller_country'],
    );
    return new CesInteropCENIPIncomingTransaction(new CesBankBasicTransaction($basic), $cenip);
  }
  /**
   * Get echange given its NID.
   *
   * This is an expensive operation since exchange are not indexed by nid, so
   * its recomendable to save this result into a persistent cache system.
   *
   * @return array|bool
   *   The requested exchange record or FALSE if it doesn't exist.
   */
  public function getExchangeByNid($nid) {
    $exchanges = $this->getAllExchanges();
    foreach ($exchanges as $exchange) {
      if (isset($exchange['data']['cenip_user']) && $exchange['data']['cenip_user'] == $nid) {
        return $exchange;
      }
    }
    return FALSE;
  }
  /**
   * Get transaction given its CEN id.
   *
   * @return array|bool
   *   The requested transaction record or FALSE if it doesn't exist.
   */
  public function getTransactionByTxid($txid) {
    $si = new IcesSerializer('CesInteropCENIPTransactionId');
    $txid = $si->loadFromUniqueKey('centxid', $txid);
    if ($txid !== FALSE) {
      return $this->getTransaction($txid->id);
    }
    else {
      return FALSE;
    }
  }
}
/**
 * Represents a CENIP remote exchange. It is a stub in fact.
 */
class CesInteropCENIPRemoteExchange extends CesBankExchange {

  /**
   * Build new instance from the name.
   */
  public function __construct($code) {
    $this->code = $code;
  }
}
/**
 * Abstract parent class fo incomming and outgoing remote transactions.
 */
class CesInteropCENIPTransaction extends CesBankDecoratedTransaction {
  /**
   * @var string
   *   The remote network id.
   */
  public $nid;
  /**
   * @var string
   *   The unique remote transaction id.
   *
   * This field is saved in a separate table so its efficiently selected.
   */
  protected $txid;
  /**
   * Delete associated transaction id record.
   */
  public function deleteExtraData() {
    if (isset($this->txid)) {
      $s = new IcesSerializer('CesInteropCENIPTransactionId');
      $txid = $s->load('id', $this->getParent()->getId());
      $s->delete($txid);
    }
  }
  /**
   * Save associated transaction id record.
   */
  public function saveExtraData() {
    $id = $this->getParent()->getId();
    if (isset($this->txid) && !empty($id)) {
      $s = new IcesSerializer('CesInteropCENIPTransactionId');
      $txid = new CesInteropCENIPTransactionId(array(
        'id' => $id,
        'centxid' => $this->txid,
      ));
      if ($s->load($id) === FALSE) {
        // Force database insert.
        $txid->is_new = TRUE;
      }
      $s->save($txid);
    }
  }
  /**
   * Implements loadRecord by adding the $txid to the object.
   */
  public function loadRecord($record) {
    parent::loadRecord($record);
    if (isset($record['txid'])) {
      $this->txid = $record['txid'];
    }
    else {
      $s = new IcesSerializer('CesInteropCENIPTransactionId');
      $id = $this->getParent()->getId();
      if (!empty($id)) {
        $txid = $s->load($id);
        if (!empty($txid)) {
          $this->txid = $txid->centxid;
        }
      }
    }
  }
}
/**
 * Remote Transaction identifier.
 *
 * This class exists only as a reflection of the database table with the tx ids
 * for an efficient select.
 */
class CesInteropCENIPTransactionId extends IcesDBObject {
  /**
   * Table name.
   */
  public static $table = 'ces_transaction_centxid';
  /**
   * @var int
   *   Local transaction id.
   */
  public $id;
  /**
   * @var string
   *   Remote CEN transaction id.
   */
  public $centxid;
}
/**
 * Class for CENIP remote transactions where the local account is the seller.
 */
class CesInteropCENIPOutgoingTransaction extends CesInteropCENIPTransaction {
  // We are breaking the drupal naming conventions to match the variable names
  // to the protocol argument names.
  /**
   * @var string
   *   The remote account id.
   */
  public $buyer_id;
  /**
   * @var string
   *   The remote account user name.
   */
  public $buyer_name;
  /**
   * @var string
   *   The remote account user mail.
   */
  public $buyer_email;
  /**
   * @var string
   *   The name of the remote exchange.
   */
  public $buyer_xname;
  /**
   * @var string
   *   The country code of the remote exchange.
   */
  public $buyer_country;

  /**
   * Implements apply().
   */
  public function apply() {
    $as = new IcesSerializer('CesBankLocalAccount');
    $toaccount = $as->loadFromUniqueKey('name', $this->getToAccountName());
    $es = new IcesSerializer('CesBankExchange');
    $toexchange = $es->load($toaccount->exchange);
    $toaccusers = $toaccount->getUsers();
    $toaccuser = reset($toaccusers);
    $touser = user_load($toaccuser->user);

    $data = array(
      'password' => $toexchange->data['cenip_pass'],
      'buyer_nid' => $this->nid,
      'buyer_id' => $this->buyer_id,
      'seller_nid' => $toexchange->data['cenip_user'],
      'seller_id' => $toaccount->name,
      'seller_xname' => $toexchange->name,
      'seller_name' => ces_user_get_name($touser),
      'seller_email' => $touser->mail,
      'seller_country' => $toexchange->country,
      'description' => $this->getConcept(),
      'amount' => $this->getAmount(),
    );
    $url = variable_get(CES_INTEROP_CEN_CLEARING_CENTRAL_URL, CES_INTEROP_CEN_CLEARING_CENTRAL_URL_DEFAULT);
    $url .= '/' . CES_INTEROP_CEN_CLEARING_CENTRAL_TXINPUT;
    $response = drupal_http_request($url, array(
      'data' => drupal_http_build_query($data),
      'method' => 'POST',
      'headers' => array(
        'Content-Type' => 'application/x-www-form-urlencoded',
      ),
    ));
    if (!empty($response->data)) {
      // The response is url-encoded.
      $response = drupal_get_query_array($response->data);
      // In the response attribute is the transaction status or error code.
      switch ($response['response']) {
        // Tansaction successfully applied.
        case 1:
          // Save return values for better review.
          if (isset($response['txid'])) {
            $this->txid = $response['txid'];
          }
          if (isset($response['buyer_name'])) {
            $this->buyer_name = $response['buyer_name'];
          }
          if (isset($response['buyer_email'])) {
            $this->buyer_email = $response['buyer_email'];
          }
          if (isset($response['buyer_xname'])) {
            $this->buyer_xname = $response['buyer_xname'];
          }
          if (isset($response['buyer_country'])) {
            $this->buyer_country = $response['buyer_country'];
          }
          // Update amount with the remote answer just in case there is any
          // change (in rounding for example).
          if (isset($response['amount'])) {
            $this->getParent()->amount = $response['amount'];
          }
          // Apply the local transaction between the virtual account and the
          // local account.
          parent::apply();
          break;

        // Error cases:
        case 2:
          $message = t('Buyer account does not exist.');
          break;

        case 3:
          $message = t('Network ID does not exist.');
          break;

        case 4:
          $message = t('Transaction denied by remote exchange.');
          break;

        case 5:
          $message = t('Faulty data.');
          break;

        case 6:
          $message = t('The transaction has repeated ID.');
          break;

        case 7:
          $message = t('URL error.');
          break;

        case 8:
          $message = t('Remote conversion rate not set.');
          break;

        case 9:
          $message = t('Remote server error.');
          break;

        case 10:
          $message = t('Password incorrect.');
          break;

        case 11:
          $message = t('Invalid IP of incoming server.');
          break;

        case 12:
          $message = t('No Transaction ID provided.');
          break;

        case 13:
          $message = t('Transaction ID does not exist.');
          break;

        case 14:
          $message = t('Unable to connect to remote server.');
          break;

        default:
          $message = t('Unknown error in remote transaction.');
      }
      if ($response['response'] != 1) {
        $this->setState(CesBankTransactionInterface::STATE_ERROR);
        $this->log(array($message));
      }
    }
    else {
      $this->setState(CesBankTransactionInterface::STATE_ERROR);
      $this->log(array(t('Network error in remote transaction.')));
    }
  }
  /**
   * Implements revoke().
   */
  public function revoke() {
    // TODO: remotely revoke.
    parent::revoke();
  }

}
/**
 * Class for CENIP remote transactions where the local account is the buyer.
 */
class CesInteropCENIPIncomingTransaction extends CesInteropCENIPTransaction {
  /**
   * @var string
   *   The remote account id.
   */
  public $seller_id;
  /**
   * @var string
   *   The remote account user name.
   */
  public $seller_name;
  /**
   * @var string
   *   The remote account user mail.
   */
  public $seller_email;
  /**
   * @var string
   *   The name of the remote exchange.
   */
  public $seller_xname;
  /**
   * @var string
   *   The country code of the remote exchange.
   */
  public $seller_country;
}
