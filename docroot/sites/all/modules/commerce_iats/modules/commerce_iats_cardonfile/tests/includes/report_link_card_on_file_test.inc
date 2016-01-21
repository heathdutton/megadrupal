<?php
/**
 * @file
 * iATS API ReportLink service used for testing.
 */

namespace iATS;

/**
 * @file
 * Test version of the iATS Payments ReportLink API.
 * Provides report data for test cases.
 */

class ReportLinkCardOnFileTestService {

  /**
   * Gets successful credit card transactions data.
   *
   * @see iATS\ReportLink::getCreditCardJournal()
   */
  public function getCreditCardJournal() {
    return $this->getTestCreditCardSuccessTransactionData();
  }

  /**
   * Gets rejected credit card transactions data.
   *
   * @see iATS\ReportLink::getCreditCardReject()
   */
  public function getCreditCardReject() {
    return $this->getTestCreditCardRejectTransactionData();
  }

  /**
   * Gets successful ACH/EFT transactions data.
   *
   * @see iATS\ReportLink::getACHEFTJournal()
   */
  public function getACHEFTJournal() {
    return $this->getTestACHEFTSuccessTransactionData();
  }

  /**
   * Gets rejected ACH/EFT transactions data.
   *
   * @see iATS\ReportLink::getACHEFTReject()
   */
  public function getACHEFTReject() {
    return $this->getTestACHEFTRejectTransactionData();
  }

  /**
   * Gets an array of successful credit card transaction data for use in tests.
   *
   * @return array
   *   Test transaction data.
   *   array(
   *     $order_id => array(
   *       $customer_code_creation_remote_id => array()
   *     )
   *   )
   */
  public function getTestCreditCardSuccessTransactionData() {
    $data = array(
      1 => array(
        COMMERCE_IATS_RECURRING_PAYMENT_TRANSACTION_ID . '|CCC000001' => array(
          array(
            'TNTYP' => 'CreditCard',
            'TNID' => 'CCT000001',
            'AGT' => 'TEST88',
            'CST' => array(
              'CSTC' => 'CCC000001',
              'FN' => 'Test',
              'LN' => 'User',
              'ADD' => '123 Any St.',
              'CTY' => 'Anytown',
              'ST' => 'AK',
              'CNT' => 'US',
              'ZC' => '11111',
            ),
            'CC' => array(
              'CCN' => '************2220',
              'MP' => 'VISA',
              'EXP' => '12/15',
            ),
            'AMT' => '10.00',
            'RST' => 'OK:678594:',
          ),
          array(
            'TNTYP' => 'CreditCard',
            'TNID' => 'CCT000002',
            'AGT' => 'TEST88',
            'CST' => array(
              'CSTC' => 'CCC000001',
              'FN' => 'Test',
              'LN' => 'User',
              'ADD' => '123 Any St.',
              'CTY' => 'Anytown',
              'ST' => 'AK',
              'CNT' => 'US',
              'ZC' => '11111',
            ),
            'CC' => array(
              'CCN' => '************2220',
              'MP' => 'VISA',
              'EXP' => '12/15',
            ),
            'AMT' => '10.00',
            'RST' => 'OK:678594:',
          ),
        ),
      ),
    );

    return $this->convertTestTransactionDataToReportFormat($data);
  }

  /**
   * Gets an array of rejected credit card transaction data for use in tests.
   *
   * @return array
   *   Test transaction data.
   *   array(
   *     $order_id => array(
   *       $customer_code_creation_remote_id => array()
   *     )
   *   )
   */
  public function getTestCreditCardRejectTransactionData() {
    $data = array(
      1 => array(
        COMMERCE_IATS_RECURRING_PAYMENT_TRANSACTION_ID . '|CCC000001' => array(
          array(
            'TNTYP' => 'CreditCard',
            'TNID' => 'CCT000003',
            'AGT' => 'TEST88',
            'CST' => array(
              'CSTC' => 'CCC000001',
              'FN' => 'Test',
              'LN' => 'User',
              'ADD' => '123 Any St.',
              'CTY' => 'Anytown',
              'ST' => 'AK',
              'CNT' => 'US',
              'ZC' => '11111',
            ),
            'CC' => array(
              'CCN' => '************2220',
              'MP' => 'VISA',
              'EXP' => '12/15',
            ),
            'AMT' => '10.00',
            'RST' => 'TIMEOUT:Timeout',
          ),
        ),
      ),
    );

    return $this->convertTestTransactionDataToReportFormat($data);
  }

  /**
   * Gets an array of successful ACH/EFT transaction data for use in tests.
   *
   * @return array
   *   Test transaction data.
   *   array(
   *     $order_id => array(
   *       $customer_code_creation_remote_id => array()
   *     )
   *   )
   */
  public function getTestACHEFTSuccessTransactionData() {
    $data = array(
      2 => array(
        COMMERCE_IATS_RECURRING_PAYMENT_TRANSACTION_ID . '|ACHC00001' => array(
          array(
            'TNTYP' => 'ACHEFT',
            'TNID' => 'ACHT00001',
            'AGT' => 'TEST88',
            'CST' => array(
              'CSTC' => 'ACHC00001',
              'FN' => 'Test',
              'LN' => 'User',
              'ADD' => '123 Any St.',
              'CTY' => 'Anytown',
              'ST' => 'AK',
              'CNT' => 'US',
              'ZC' => '11111',
            ),
            'ACH' => array(
              'ACN' => '************2220',
              'ACTYP' => 'Checking',
            ),
            'AMT' => '10.00',
            'RST' => 'OK:678594:',
          ),
          array(
            'TNTYP' => 'ACHEFT',
            'TNID' => 'ACHT00002',
            'AGT' => 'TEST88',
            'CST' => array(
              'CSTC' => 'ACHC00001',
              'FN' => 'Test',
              'LN' => 'User',
              'ADD' => '123 Any St.',
              'CTY' => 'Anytown',
              'ST' => 'AK',
              'CNT' => 'US',
              'ZC' => '11111',
            ),
            'ACH' => array(
              'ACN' => '************2220',
              'ACTYP' => 'Checking',
            ),
            'AMT' => '10.00',
            'RST' => 'OK:678594:',
          ),
        ),
      ),
    );

    return $this->convertTestTransactionDataToReportFormat($data);
  }

  /**
   * Gets an array of rejected ACH/EFT transaction data for use in tests.
   *
   * @return array
   *   Test transaction data.
   *   array(
   *     $order_id => array(
   *       $customer_code_creation_remote_id => array()
   *     )
   *   )
   */
  public function getTestACHEFTRejectTransactionData() {
    $data = array(
      2 => array(
        COMMERCE_IATS_RECURRING_PAYMENT_TRANSACTION_ID . '|ACHC00001' => array(
          array(
            'TNTYP' => 'ACHEFT',
            'TNID' => 'ACHT00003',
            'AGT' => 'TEST88',
            'CST' => array(
              'CSTC' => 'ACHC00001',
              'FN' => 'Test',
              'LN' => 'User',
              'ADD' => '123 Any St.',
              'CTY' => 'Anytown',
              'ST' => 'AK',
              'CNT' => 'US',
              'ZC' => '11111',
            ),
            'ACH' => array(
              'ACN' => '************2220',
              'ACTYP' => 'Checking',
            ),
            'AMT' => '10.00',
            'RST' => 'TIMEOUT:Timeout',
          ),
        ),
      ),
    );

    return $this->convertTestTransactionDataToReportFormat($data);
  }

  /**
   * Converts transaction test data to match the format returned by iATS API.
   *
   * @param array $data
   *   Transaction test data to convert.
   *
   * @see iATS\ReportLinkTestService::getTestCreditCardSuccessTransactionData()
   * @see iATS\ReportLinkTestService::getTestCreditCardRejectTransactionData()
   *
   * @return array
   *   Array of transactions.
   */
  protected function convertTestTransactionDataToReportFormat($data) {
    $report_data = array();

    foreach ($data as $order_data) {
      foreach ($order_data as $transactions_data) {
        foreach ($transactions_data as $transaction) {
          $report_data[] = $transaction;
        }
      }
    }

    return $report_data;
  }
}
