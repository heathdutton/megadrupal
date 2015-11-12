<?php
// @codingStandardsIgnoreFile
?>

<div id="<?php print $modal_id ?>">

  <div class="short-list">
  <ul>
    <li><a href="#" data-currency-code="USD" class="quick-link-swap-currency">$ USD</a></li>
    <li><a href="#" data-currency-code="AUD" class="quick-link-swap-currency">$ AUD</a></li>
    <li><a href="#" data-currency-code="GBP" class="quick-link-swap-currency">£ GBP</a></li>
    <li><a href="#" data-currency-code="EUR" class="quick-link-swap-currency">€ EUR</a></li>
    <li><a href="#" data-currency-code="CAD" class="quick-link-swap-currency">$ CAD</a></li>
  </ul>
  </div>

  <div class="currencies">
    <form action="">
      <h5>Other currencies</h5>
        <select class="swap-currency">
          <option value="USD">-- Choose currency --</option>
          <option value="USD">United States Dollars - USD</option>
          <option value="GBP">United Kingdom Pounds - GBP</option>
          <option value="CAD">Canada Dollars - CAD</option>
          <option value="AUD">Australia Dollars - AUD</option>
          <option value="NZD">New Zealand Dollars - NZD</option>
          <option value="CHF">Switzerland Francs - CHF</option>
          <option value="ZAR">South Africa Rand - ZAR</option>
          <option value="ARS">Argentina Pesos - ARS</option>
          <option value="AUD">Australia Dollars - AUD</option>
          <option value="BHD">Bahrain Dinars - BHD</option>
          <option value="BRL">Brazil Reais - BRL</option>
          <option value="CAD">Canada Dollars - CAD</option>
          <option value="CNY">RMB (China Yuan Renminbi) - CNY</option>
          <option value="DKK">Denmark Kroner - DKK</option>
          <option value="EUR">Euro - EUR</option>
          <option value="HKD">Hong Kong Dollars - HKD</option>
          <option value="INR">India Rupees - INR</option>
          <option value="IDR">Indonesia Rupiahs - IDR</option>
          <option value="JPY">Japan Yen - JPY</option>
          <option value="JOD">Jordan Dinars - JOD</option>
          <option value="KRW">Korea (South) Won - KRW</option>
          <option value="KWD">Kuwait Dinars - KWD</option>
          <option value="MYR">Malaysia Ringgits - MYR</option>
          <option value="NZD">New Zealand Dollars - NZD</option>
          <option value="NOK">Norway Kroner - NOK</option>
          <option value="PKR">Pakistan Rupees - PKR</option>
          <option value="PEN">Peru Nuevos Soles - PEN</option>
          <option value="PHP">Philippines Pesos - PHP</option>
          <option value="QAR">Qatar Riyals - QAR</option>
          <option value="RUB">Russia Rubles - RUB</option>
          <option value="SAR">Saudi Arabia Riyals - SAR</option>
          <option value="SGD">Singapore Dollars - SGD</option>
          <option value="ZAR">South Africa Rand - ZAR</option>
          <option value="LKR">Sri Lanka Rupees - LKR</option>
          <option value="SEK">Sweden Kronor - SEK</option>
          <option value="CHF">Switzerland Francs - CHF</option>
          <option value="TWD">Taiwan New Dollars - TWD</option>
          <option value="THB">Thailand Baht - THB</option>
          <option value="AED">United Arab Emirates Dirhams - AED</option>
      </select>
    </form>
  </div>

  <div class="pricing-disclaimer">
    <?php print $disclaimer; ?>
</div>