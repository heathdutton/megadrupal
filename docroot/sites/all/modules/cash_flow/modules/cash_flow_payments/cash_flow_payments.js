(function ($) {
 // TODO: Abstract the Amount remaining string a bit more
 var amountRemainingStr;
 var lastFill = '';
  // Automatically fill in appropriate amount
  Drupal.behaviors.populateAmount = {
    attach: function(context) {
      amountRemainingStr = Drupal.t('Amount remaining:');
      $('#edit-field-line-item-und, #edit-field-target-line-item-und').change(
        function() {
          var targetAmount = findAmount($('#edit-field-target-line-item-und option:selected').html());
          var sourceAmount = findAmount($('#edit-field-line-item-und option:selected').html());
          // If both aren't selected, bail.
          if (targetAmount == '' || sourceAmount == '') {
            return;
          }
          var $amount = $('#edit-field-amount-und-0-value');
          // If there was already a value, don't clobber it (we're probably
          // editing).
          if ($amount.val() == '' || $amount.val() == lastFill) {
            var fillAmount = sourceAmount;
            if ((parseFloat(sourceAmount) >= parseFloat(targetAmount))) {
              fillAmount = targetAmount;
            }
            lastFill = fillAmount;
            $amount.val(fillAmount).focus();
          }
        }
      );
    }
  }

  function findAmount(lineItemString) {
    // Do some string parsing to get the amount
    var arsLen = amountRemainingStr.length + 1;
    var startPos = lineItemString.indexOf(amountRemainingStr);
    var amount = lineItemString.substring(startPos + arsLen).trim();
    return amount;
  }
})(jQuery);
