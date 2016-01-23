<?php
/**
 * @TransferInitiatorDetails.tpl.php
 *
 * Initiates a paymnet order, see step II-2 of chapter 7 "EPS Ablauf".
 *
 * @link http://www.stuzza.at/en/download/294-eps-pflichtenheft-inter-scheme-v1-0-01-09-2014.html
 */
?>
<epsp:EpsProtocolDetails xmlns:epsp="http://www.stuzza.at/namespaces/eps/protocol/2013/02"
                         xmlns:atrul="http://www.stuzza.at/namespaces/eps/austrianrules/2013/02"
                         xmlns:epi="http://www.stuzza.at/namespaces/eps/epi/2013/02"
                         xmlns:eps="http://www.stuzza.at/namespaces/eps/payment/2013/02"
                         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                         xsi:schemaLocation="http://www.stuzza.at/namespaces/eps/protocol/2013/02/EPSProtocol-V25.xsd"
                         SessionLanguage="DE">
  <epsp:TransferInitiatorDetails>
    <eps:PaymentInitiatorDetails>
      <epi:EpiDetails>
        <epi:IdentificationDetails>
          <epi:Date><?php print $date; ?></epi:Date>
          <epi:ReferenceIdentifier><?php print $reference_id; ?></epi:ReferenceIdentifier>
        </epi:IdentificationDetails>
        <epi:PartyDetails>
          <epi:BfiPartyDetails>
            <epi:BfiBicIdentifier><?php print $bfi_bic_id; ?></epi:BfiBicIdentifier>
          </epi:BfiPartyDetails>
          <epi:BeneficiaryPartyDetails>
            <epi:BeneficiaryNameAddressText><?php print $ben_name_address; ?></epi:BeneficiaryNameAddressText>
            <epi:BeneficiaryAccountIdentifier><?php print $ben_account_id; ?></epi:BeneficiaryAccountIdentifier>
          </epi:BeneficiaryPartyDetails>
        </epi:PartyDetails>
        <epi:PaymentInstructionDetails>
          <epi:RemittanceIdentifier><?php print $remittance_id; ?></epi:RemittanceIdentifier>
          <epi:InstructedAmount AmountCurrencyIdentifier="<?php print $currency; ?>"><?php print $amount; ?></epi:InstructedAmount>
          <epi:ChargeCode>SHA</epi:ChargeCode>
          <epi:DateOptionDetails DateSpecificationCode="DBD">
            <epi:OptionDate><?php print $date; ?></epi:OptionDate>
          </epi:DateOptionDetails>
        </epi:PaymentInstructionDetails>
      </epi:EpiDetails>
    </eps:PaymentInitiatorDetails>
    <epsp:TransferMsgDetails>
      <epsp:ConfirmationUrl>
        <?php print $confirmation_url; ?>
      </epsp:ConfirmationUrl>
      <epsp:TransactionOkUrl TargetWindow="pay1">
        <?php print $transaction_ok_url; ?>
      </epsp:TransactionOkUrl>
      <epsp:TransactionNokUrl TargetWindow="pay1">
        <?php print $transaction_nok_url; ?>
      </epsp:TransactionNokUrl>
    </epsp:TransferMsgDetails>
    <?php if (!empty($articles)) : ?>
    <epsp:WebshopDetails>
      <?php foreach ($articles as $article) : ?>
        <epsp:WebshopArticle ArticleName="<?php print $article['name']; ?>" ArticleCount="<?php print $article['count']; ?>" ArticlePrice="<?php print $article['price']; ?>"/>
     <?php endforeach; ?>
    </epsp:WebshopDetails>
    <?php endif;?>
    <epsp:AuthenticationDetails>
      <epsp:UserId><?php print $user_id; ?></epsp:UserId>
      <epsp:MD5Fingerprint><?php print $md5_hash; ?></epsp:MD5Fingerprint>
    </epsp:AuthenticationDetails>
  </epsp:TransferInitiatorDetails>
</epsp:EpsProtocolDetails>