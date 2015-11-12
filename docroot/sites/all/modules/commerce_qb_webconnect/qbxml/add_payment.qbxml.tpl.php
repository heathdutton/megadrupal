<ReceivePaymentAddRq>
  <ReceivePaymentAdd>
    <CustomerRef>
      <FullName><?php print $customer_name ?></FullName>
    </CustomerRef>
    <TxnDate><?php print $date ?></TxnDate>
    <RefNumber><?php print $ref_number ?></RefNumber>
    <TotalAmount><?php print $amount ?></TotalAmount>
    <PaymentMethodRef>
      <FullName><?php print $payment_method ?></FullName>
    </PaymentMethodRef>
    <IsAutoApply>true</IsAutoApply>        
  </ReceivePaymentAdd>
</ReceivePaymentAddRq>