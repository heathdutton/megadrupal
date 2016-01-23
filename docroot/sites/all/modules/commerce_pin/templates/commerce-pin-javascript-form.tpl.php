<?php
/**
 * @file
 * Contains default theming of JavaScript form elements.
 *
 * Note that we've mimiced much of the default Commerce credit card markup with
 * one notable ommision - none of the form fields have *name* attributes.
 *
 * This ensures that the credit-card number, date and CVC are never sent to the
 * server as POST values.
 *
 * If you need to change the way the form is themed, do not change any of the
 * id attributes, or the JavaScript may not work as expected.
 *
 * @see js/commerce-pin.js
 */
?>
<div id="commerce-pin-errors" class="messages--error messages error element-hidden">
  <strong></strong>
  <ul></ul>
</div>
<div class="form-item form-type-textfield form-item-commerce-payment-payment-details-credit-card-number">
  <label for="edit-commerce-payment-payment-details-credit-card-number">Card number
    <span class="form-required" title="This field is required.">*</span></label>
  <input autocomplete="off" id="cc-number" value="" size="20" maxlength="19" class="form-text required" type="text">
</div>
<div class="commerce-credit-card-expiration container-inline">
  <div class="form-item form-type-select form-item-commerce-payment-payment-details-credit-card-exp-month">
    <label for="edit-commerce-payment-payment-details-credit-card-exp-month">Expiration
      <span class="form-required" title="This field is required.">*</span></label>
    <select id="cc-expiry-month" class="form-select required">
      <option value="01">01</option>
      <option value="02">02</option>
      <option value="03">03</option>
      <option value="04">04</option>
      <option value="05">05</option>
      <option value="06">06</option>
      <option value="07">07</option>
      <option value="08">08</option>
      <option value="09">09</option>
      <option value="10">10</option>
      <option value="11">11</option>
      <option value="12">12</option>
    </select>
  </div>
  <span class="commerce-month-year-divider">/</span>

  <div class="form-item form-type-select form-item-commerce-payment-payment-details-credit-card-exp-year">
    <select id="cc-expiry-year" class="form-select">
      <?php foreach (range(date('Y'), (int) date('Y') + 19) as $year) : ?>
        <option value="<?php print $year; ?>"><?php print $year; ?></option>
      <?php endforeach; ?>
    </select>
  </div>
</div>
<div class="form-item form-type-textfield form-item-commerce-payment-payment-details-credit-card-code">
  <label for="edit-commerce-payment-payment-details-credit-card-code">Security code
    <span class="form-required" title="This field is required.">*</span></label>
  <input autocomplete="off" id="cc-cvc" value="" size="4" maxlength="4" class="form-text required" type="text">
</div>
