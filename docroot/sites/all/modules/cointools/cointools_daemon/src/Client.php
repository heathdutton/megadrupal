<?php

namespace Drupal\cointools_daemon;

use GuzzleHttp\Url as GuzzleUrl;
use GuzzleHttp\Exception\RequestException;
use Graze\GuzzleHttp\JsonRpc\Client as JsonRpcClient;
use Drupal\cointools\CoinTools;

class Client {

  private $client;

  public static function factory($report_error = TRUE) {
    $uri = new GuzzleUrl(variable_get('cointools_daemon.scheme'), variable_get('cointools_daemon.hostname'), variable_get('cointools_daemon.username'), variable_get('cointools_daemon.password'), variable_get('cointools_daemon.port'));
    $client = new Client(JsonRpcClient::factory($uri));

    try {
      $client->request('getinfo');
    }
    catch (RequestException $exception) {
      $message = t("Unable to connect to bitcoind. Check <a href=\"@url\">configuration</a>.", ['@url' => url('admin/config/cointools/daemon')]);
      if ($report_error) {
        drupal_set_message($message, 'error');
      }
      throw new \Exception($message);
    }

    return $client;
  }

  public function __construct(JsonRpcClient $client) {
    $this->client = $client;
  }

  public function request($method, array $params = []) {
    return $this->client->send($this->client->request(1, $method, $params))->getRpcResult();
  }

  public function balanceItem() {
    $confirmed = CoinTools::bitcoinToSatoshi($this->request('getbalance'));
    $unconfirmed = CoinTools::bitcoinToSatoshi($this->request('getbalance', ['', 0]));

    $items['confirmed'] = [
      '#type' => 'item',
      '#title' => "Confirmed balance",
      'amount' => [
        '#theme' => 'cointools_amount',
        '#amount' => $confirmed,
        '#fiat' => TRUE,
      ],
    ];

    if ($unconfirmed > $confirmed) {
      $items['unconfirmed'] = [
        '#type' => 'item',
        '#title' => "Unconfirmed balance",
        'amount' => [
          '#theme' => 'cointools_amount',
          '#amount' => $unconfirmed,
          '#fiat' => TRUE,
        ],
      ];
    }

    return $items;
  }

  public function timeSinceLastBlock() {
    $hash = $this->request('getbestblockhash');
    $block = $this->request('getblock', [$hash]);
    return REQUEST_TIME - $block['time'];
  }

  public function averageTimeUntilNextBlock() {
    $difficulty = $this->request('getdifficulty');
    $hashrate = $this->request('getnetworkhashps');
    return $difficulty * pow(2, 32) / $hashrate;
  }

  /**
   * Determines the fee necessary to get a 1kB transaction in a block within
   * nblocks.
   *
   * @param $nblocks
   *   Number of blocks it should take for the transaction to confirm.
   *
   * @return int
   *   Fee size in satoshis.
   */
  public function estimateFee($nblocks = 1) {
    return CoinTools::bitcoinToSatoshi($this->request('estimatefee', [$nblocks]));
  }

  /**
   * Determines the fee necessary to get a transaction in a block within
   * nblocks.
   *
   * @param $inputs
   *   Number of inputs.
   * @param $outputs
   *   Number of outputs.
   * @param $nblocks
   *   Number of blocks it should take for the transaction to confirm.
   *
   * @return int
   *   Fee size in satoshis.
   */
  public function transactionEstimateFee($inputs, $outputs = 1, $nblocks = 1) {
    // @see https://bitcoin.stackexchange.com/questions/1195/how-to-calculate-transaction-size-before-sending
    $size = 149 * $inputs + 34 * $outputs + 10;
    $fee_per_kb = $this->estimateFee($nblocks);
    return ceil($size * $fee_per_kb / 1000);
  }

  public function transactionLoad($txid) {
    $raw = $this->request('getrawtransaction', [$txid]);
    return $this->request('decoderawtransaction', [$raw]);
  }

  public function transactionSendNew(array $inputs, array $outputs) {
    $raw = $this->request('createrawtransaction', [$inputs, $outputs]);
    $raw = $this->request('signrawtransaction', [$raw]);
    return $this->request('sendrawtransaction', [$raw['hex']]);
  }

  /**
   * Forwards a bitcoin transaction to one or more addresses with defined
   * proportionality.
   *
   * @param $txid
   *   txid of transaction to forward.
   * @param array $addresses
   *   List of addresses that can be spent from.
   * @param array $outputs
   *   Where to forward the bitcoin to.
   *   Keys are destinations addresses.
   *   Values are proportional quantities.
   * @param bool $tx_confirm_target
   *   The fee should be calculated for the transaction to reach the blockchain
   *   after this many blocks. default = 1
   *
   * @return string
   *   txid of the forwarding transaction.
   */
  public function forwardTransaction($txid, array $addresses, array $outputs, $tx_confirm_target = 1) {
    $transaction_in = $this->transactionLoad($txid);
    // Find inputs and amount for new transaction.
    $inputs = [];
    $transaction_amount = 0;
    foreach ($transaction_in['vout'] as $vout) {
      // Is this output for one of our addresses?
      if (in_array($vout['scriptPubKey']['addresses'][0], $addresses)) {
        // Has this output been spent yet?
        try {
          $this->request('gettxout', [$txid, $vout['n']]);
          $inputs[] = [
            'txid' => $txid,
            'vout' => $vout['n'],
          ];
          $transaction_amount += CoinTools::bitcoinToSatoshi($vout['value']);
        }
        catch (\Exception $e) {}
      }
    }
    // Remove the miner fee from the transaction amount.
    $transaction_amount -= $this->transactionEstimateFee(count($inputs), count($outputs));
    // Divide up the pie according to the correct proportions.
    $ratio = $transaction_amount / array_sum($outputs);
    foreach ($outputs as $address => &$amount) {
      $amount *= $ratio;
      // Eliminate dust outputs.
      if ($amount < 546) {
        unset($outputs[$address]);
        continue;
      }
      $amount = CoinTools::satoshiToBitcoin($amount);
    }
    // Make sure there is something to send.
    if (empty($outputs)) {
      throw new \Exception("No bitcoin to send.");
    }
    // Send the transaction.
    return $this->transactionSendNew($inputs, $outputs);
  }

}
