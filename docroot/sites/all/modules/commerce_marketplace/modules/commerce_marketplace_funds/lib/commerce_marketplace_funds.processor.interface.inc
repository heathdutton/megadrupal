<?php

/**
 * @file
 * Defines the interface that other modules need to implement for providing
 * additional funds processors.
 */

/**
 * Interface CommerceMarketplaceFundsProcessorInterface.
 * Classes implementing this interface will be used by Commerce Marketplace Funds
 * entity controllers to handle remote funds processing.
 */
interface CommerceMarketplaceFundsProcessorInterface {
  /**
   * Prepare funds account values before creating the account object.
   */
  public function createAccount(&$values);

  /**
   * Act upon saving an account.
   */
  public function saveAccount($account);

  /**
   * Prepare funds transaction values before creating the transaction object.
   */
  public function createTransaction(&$values);

  /**
   * Act upon saving a transaction.
   */
  public function saveTransaction($transaction);

  /**
   * Prepare funds withdraw method values before creating the method object.
   */
  public function createWithdrawMethod(&$values);

  /**
   * Act upon saving a withdraw method.
   */
  public function saveWithdrawMethod($method);
}
