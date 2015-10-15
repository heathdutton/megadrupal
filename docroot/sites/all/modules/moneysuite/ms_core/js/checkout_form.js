(function ($) {
  Drupal.behaviors.msCoreCheckout = {
    attach: function (context, settings) {
      // Disable submit buttons while AJAX is running.
      $('#ms-core-checkout-form').ajaxStart(function(){
        $("#ms-core-checkout-form input[type=submit]").attr("disabled", "disabled");
      });
      $('#ms-core-checkout-form').ajaxComplete(function(){
        $("#ms-core-checkout-form input[type=submit]").removeAttr("disabled");
      });

      $(".ms_checkout_cc_number").blur(updateCards);
      $(".ms_checkout_cc_number").keyup(updateCards);
      $('.ms_checkout_cc_type').hide();

      $('.ms_core_cc_cvv a.ms_core_cvv_link').click(function(e){
        e.preventDefault();
        $(".ms_core_cc_cvv .ms_core_cvv_dialog").dialog({
          modal: true,
          width: 465
        });
      });

      function updateCards(e) {
        var value = e.target.value,
          valid = isValidNumber(value),
          type = getCreditCardType(value);

        if (valid) {
          $('.ms_checkout_cc_number').addClass('ms_core_check');
        }
        else {
          $('.ms_checkout_cc_number').removeClass('ms_core_check');
        }

        $('.ms_core_cc_image_combo').removeClass('ms_core_cc_image_active');
        $('.ms_checkout_cc_type').val("");
        if (type) {
          // Mark the image as active.
          $('.ms_cc_image-' + type).addClass('ms_core_cc_image_active');
          // Set the input element value.
          $('.ms_checkout_cc_type').val(type);
        }
      }

      /**
       * Verifies credit card with Luhn algorithm.
       *
       * @param cc_num
       *   The credit card number.
       *
       * @return bool
       *   Whether or not it is valid.
       */
      function isValidNumber(cc_num) {
        var sum = 0,
          alt = false,
          i = cc_num.length-1,
          num;

        if (cc_num.length < 13 || cc_num.length > 19) {
          return false;
        }

        while (i >= 0) {
          num = parseInt(cc_num.charAt(i), 10);
          if (isNaN(num)){
            return false;
          }
          if (alt) {
            num *= 2;
            if (num > 9){
              num = (num % 10) + 1;
            }
          }
          alt = !alt;
          sum += num;
          i--;
        }

        return (sum % 10 == 0);
      }

      /**
       * Checks what credit card type is being typed.
       *
       * @param string cc_num
       *   The credit card number.
       *
       * @return string
       *   The credit card type.
       */
      function getCreditCardType(cc_num) {
        if (/^5[1-5]/.test(cc_num)) {
          return "mc";
        }
        else if (/^4/.test(cc_num)) {
          return "visa";
        }
        else if (/^3(?:0[0-5]|[68])/.test(cc_num)) {
          return "diners";
        }
        else if (/^6(?:011|5)/.test(cc_num)) {
          return "discover";
        }
        else if (/^(?:2131|1800|35)/.test(cc_num)) {
          return "jcb";
        }
        else if (/^6(304|706|771|709)/.test(cc_num)) {
          return "laser";
        }
        else if (/^(5018|5020|5038|5893|6304|6759|6761|6762|6763|0604)/.test(cc_num)) {
          return "maestro";
        }
        else if (/^6(334|767)/.test(cc_num)) {
          return "solo";
        }
        else if (/^(4903|4905|4911|4936|564182|633110|6333|6759)/.test(cc_num)) {
          return "switch";
        }
        else if (/^3[47]/.test(cc_num)) {
          return "amex";
        }
        else {
          return "";
        }
      }
    }
  };

})(jQuery);
