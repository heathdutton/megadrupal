(function ($) {

  Drupal.behaviors.fakturaCalc = {
    attach: function(context, settings) {
      
      $('.field-name-field-invoice-item-unit-price input').keydown(function () {
         var row = $(this).closest('tr');
         var qty = row.find('.field-name-field-invoice-item-qty input');
         if(!qty.val()) {
           qty.val(1);
         }
      });
      
      $('.field-name-field-invoice-item-unit-price input').keyup(function () {
         var row = $(this).closest('tr');
         calcProductItemFields(row);
         calcSubtotal();
      });
      
      $('.field-name-field-invoice-item-qty input').keyup(function () {
         var row = $(this).closest('tr');
         calcProductItemFields(row);
         calcSubtotal();
      });
      
      // Tax field
      $('.field-name-field-invoice-item-tax select').change(function () {
        // $('#field-invoice-tax-values select').val($(this).val());
        calcTax();
      });
      
      function calcTax() {
        var taxes = [];
        $('#edit-field-invoice-item table tbody tr').each(function () {
          if(jQuery.inArray($(this).val(), taxes)) {
            var msg = 'Arrays: ';
            taxes.each(function () {
              msq += $(this);
            });
            alert(msg);
          } else {
            taxes.push($(this).val());
          }
        });
      }
      
      function calcProductItemFields(row) {
        var unitprice = row.find('.field-name-field-invoice-item-unit-price input');
        var qty = row.find('.field-name-field-invoice-item-qty input');
        var total = row.find('.field-name-field-invoice-item-total input');
        var calc = unitprice.val() * qty.val().replace( /,/,"." );
        total.val(calc.toString().replace( ".","," ));
      }
            
      function calcSubtotal() {
        var subtotal = 0;
        $('#edit-field-invoice-item table tbody tr').each(function () {
          var total = $(this).find('.field-name-field-invoice-item-total input');
          subtotal += parseFloat(total.val().replace( /,/,"." ));
        });
        $('#edit-field-invoice-subtotal-und-0-value').val(subtotal.toString().replace( ".","," ));
      }
    }
  };

})(jQuery);