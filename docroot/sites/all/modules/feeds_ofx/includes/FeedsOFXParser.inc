<?php
/**
 * @file
 * Feeds OFX parser class
 */

/**
 * Feeds OFX parser class
 */
class FeedsOFXParser extends FeedsParser {

  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {

    // Begin a list of items to return.
    $items = array();

    // Get the raw output of the fetcher to use as the input for our parser.
    $content = $fetcher_result->getRaw();

    // Remove everything before the opening <OFX> tag.
    $content = explode('<OFX>', $content);
    $content = '<OFX>' . $content[1];

    // Fix all unclosed tags.
    $content = preg_replace('/<([A-Z\.]+)>([\w0-9\.\-_:\+\,\]\[ ])+/i', '$0</$1>', $content);

    // Load it as a SimpleXMLElement.
    $ofx = simplexml_load_string('<?xml version="1.0"?>' . $content);

    // If the XML couldn't be parsed, error.
    if (!$ofx) {
      drupal_set_message('The was an error reading the OFX file.', 'error');
    }

    // If there are transactions, loop through them.
    if (!empty($ofx->BANKMSGSRSV1->STMTTRNRS->STMTRS->BANKTRANLIST->STMTTRN)) {
      foreach ($ofx->BANKMSGSRSV1->STMTTRNRS->STMTRS->BANKTRANLIST->STMTTRN as $trn) {

        // Convert the SimpleXMLElement to an array.
        $transaction = (array) $trn;

        // Add the transaction as an item.
        $item = array(
          'trntype' => $transaction['TRNTYPE'],
          'dtposted' => $transaction['DTPOSTED'],
          'trnamt' => $transaction['TRNAMT'],
          'fitid' => $transaction['FITID'],
          'memo' => $transaction['MEMO'],
        );

        // If a transaction name is set (as is the case in QFX and QBO files), add it.
        if (!empty($transaction['NAME'])) {
          $item['name'] = $transaction['NAME'];
        }

        // Add the new item to the array.
        $items[] = $item;
      }
    }

    // Return a parser result.
    $result = new FeedsParserResult($items);
    return $result;
  }

  public function getMappingSources() {
    return array(
      'trntype' => array(
        'name' => t('Transaction type'),
        'description' => t('Transaction type.'),
      ),
      'dtposted' => array(
        'name' => t('Date posted'),
        'description' => t('Date posted.'),
      ),
      'trnamt' => array(
        'name' => t('Transaction amount'),
        'description' => t('Transaction amount.'),
      ),
      'fitid' => array(
        'name' => t('FITID'),
        'description' => t('Unique ID.'),
      ),
      'name' => array(
        'name' => t('Transaction name'),
        'description' => t('Transaction name.'),
      ),
      'memo' => array(
        'name' => t('Transaction memo'),
        'description' => t('Transaction memo.'),
      ),
    );
  }
}