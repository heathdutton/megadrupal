<div id="moip-ct-js-form">

  <input type="hidden" id="moip-user-phone" value="<?php print $moip_user_phone ?>">

  <div id="MoipWidget" data-token="<?php print $moip_payment_token ?>"
       callback-method-success="Drupal.Moip.CT.sendSuccesfull"
       callback-method-error="Drupal.Moip.CT.sendFailed"></div>

  <div id="moip-safety">
    <p><?php print t('Your safety is assured by Moip, the payment service from IG. <a href="https://site.moip.com.br/seguranca-para-compradores/" target="_blank">Want to read more about</a>?') ?></p>
    <div class="payment-icon-moip-safety"></div>
  </div>

  <div style="display:none" class="ajax-progress ajax-progress-throbber"><div class="throbber"><?php print t('Processing your payment...') ?></div></div>

  <div class='payment-way banktransfer'>
    <div class="fields">
      <div class="form-item form-type-radio field-wrapper bradesco">
        <input type="radio" id="bradesco" name="paymentway" value="Bradesco"/>
        <label class="option" for="bradesco">Bradesco</label>
      </div>
      <div class="form-item form-type-radio field-wrapper itau">
        <input type="radio" id="itau" name="paymentway" value="Itau"/>
        <label class="option" for="itau">Itaú</label>
      </div>
      <div class="form-item form-type-radio field-wrapper bancodobrasil">
        <input type="radio" id="bancodobrasil" name="paymentway" value="BancoDoBrasil"/>
        <label class="option" for="bancodobrasil">Banco do Brasil</label>
      </div>
      <div class="form-item form-type-radio field-wrapper banrisul">
        <input type="radio" id="banrisul" name="paymentway" value="Banrisul"/>
        <label class="option" for="banrisul">Banrisul</label>
      </div>
    </div>
  </div>
  
  <div id="moip-ct-messages"></div>
  
  <div class='payment-way creditcard'>
    <div class="fields">
      <fieldset>
        <legend><?php print t('Cardholder information') ?> </legend>
        <div class="fieldset-wrapper">
          <div class="form-item form-type-textfield field-wrapper name">
            <label class="option"><?php print t('Cardholder name') ?> </label>
            <input type="text" value="<?php print $moip_user_name ?>"/>
            <div class="help"></div>
          </div>
          <div class="form-item form-type-textfield field-wrapper birthday">
            <label class="option"><?php print t('Birthday') ?> </label>
            <input type="text" value="<?php print $moip_user_birthday ?>"/>
            <div class="help">ex: 30/12/1987</div>
          </div>
          <div class="form-item form-type-textfield field-wrapper cpf">
            <label class="option">CPF</label>
            <input type="text" value="<?php print $moip_user_cpf ?>"/>
            <div class="help">ex: 99999999999</div>
          </div>
        </div>
      </fieldset>
      <fieldset>
        <legend><?php print t('Card information') ?> </legend>
        <div class="fieldset-wrapper">
          <div class="form-item form-type-textfield field-wrapper number">
            <label class="option"><?php print t('Number') ?> </label>
            <input type="text"/>
            <div class="help"></div>
          </div>
          <div class="form-item form-type-textfield field-wrapper securitycode">
            <label class="option"><?php print t('Security code') ?> </label>
            <input type="text" size="4"/>
            <div class="help"></div>
          </div>
          <div class="form-item form-type-textfield field-wrapper expirationdate">
            <label class="option"><?php print t('Expiration date') ?> </label>
            <input type="text"/>
            <div class="help">ex: 12/2015</div>
          </div>
        </div>
      </fieldset>
    </div>
  </div>

  <div id="moip-ct-buttons" style="display: none">
    <div>
      <button id="moip-ct-submit" onclick="Drupal.Moip.CT.send(<?php print $order_id ?>); return false;">
        <?php print t('Next') ?> 
      </button>
    </div>
  </div>

</div>