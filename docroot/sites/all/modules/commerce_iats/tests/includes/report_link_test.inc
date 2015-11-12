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

class ReportLinkTestService {
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
   * Gets an array of successful ACH/EFT transaction data for use in tests.
   *
   * @return array
   *   Test transaction data.
   *   array(
   *     $order_id => array(
   *       $remote_id => array()
   *     )
   *   )
   */
  public function getTestACHEFTSuccessTransactionData() {
    $data = array(
      1 => array(
        'A0000001' => array(
          array(
            'TNTYP' => 'ACHEFT',
            'TNID' => 'A0000001',
            'AGT' => 'TEST88',
            'CST' => array(
              'CSTC' => '',
              'FN' => 'Test',
              'LN' => 'User',
              'ADD' => '123 Any St.',
              'CTY' => 'Anytown',
              'ST' => 'AK',
              'CNT' => 'US',
              'ZC' => '11111',
            ),
            'ACH' => array(
              'ACN' => '************0001',
              'ACTYP' => 'Checking',
            ),
            'INV' => 'INVOICE-1',
            'AMT' => '10.00',
            'RST' => 'OK:BankAccept',
          ),
        ),
      ),
      2 => array(
        'A0000002|CCC000001' => array(
          array(
            'TNTYP' => 'ACHEFT',
            'TNID' => 'A0000002',
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
            'ACH' => array(
              'ACN' => '************0001',
              'ACTYP' => 'Checking',
            ),
            'INV' => 'INVOICE-2',
            'AMT' => '10.00',
            'RST' => 'OK:BankAccept',
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
   *       $remote_id => array()
   *     )
   *   )
   */
  public function getTestACHEFTRejectTransactionData() {
    $data = array(
      3 => array(
        'A0000003' => array(
          array(
            'TNTYP' => 'ACHEFT',
            'TNID' => 'A0000003',
            'AGT' => 'TEST88',
            'CST' => array(
              'CSTC' => '',
              'FN' => 'Test',
              'LN' => 'User',
              'ADD' => '123 Any St.',
              'CTY' => 'Anytown',
              'ST' => 'AK',
              'CNT' => 'US',
              'ZC' => '11111',
            ),
            'ACH' => array(
              'ACN' => '************0001',
              'ACTYP' => 'Checking',
            ),
            'INV' => 'INVOICE-3',
            'AMT' => '10.00',
            'RST' => 'OK:Reject',
          ),
        ),
      ),
      4 => array(
        'A0000555' => array(
          array(
            'TNTYP' => 'ACHEFT',
            'TNID' => 'A0000555',
            'AGT' => 'TEST88',
            'CST' => array(
              'CSTC' => '',
              'FN' => 'Test',
              'LN' => 'User',
              'ADD' => '123 Any St.',
              'CTY' => 'Anytown',
              'ST' => 'AK',
              'CNT' => 'US',
              'ZC' => '11111',
            ),
            'ACH' => array(
              'ACN' => '************0001',
              'ACTYP' => 'Checking',
            ),
            'INV' => 'INVOICE-4',
            'AMT' => '10.00',
            'RST' => 'OK:Reject',
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
