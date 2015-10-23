/**
 * @file
 * Provides necessary realtime recalculations reacting on fields editing in line item manager widget.
 */

(function ($) {

  Drupal.behaviors.attach_line_items_profits_form_handlers = {
    attach: function (context, settings) {
      var currency_symbol = settings.currency_symbol;
      $('#line-item-manager .field-widget-commerce-price-full input, #line-item-manager input.field-widget-field-cost, #line-item-manager .form-item input[name$="[quantity]"]', context).keyup(function () {
        var textfield = $(this);
        var trimmed_value = textfield.val().replace(' ', '');
        if (trimmed_value !== textfield.val()) {
          textfield.val(trimmed_value);
        }

        var row = textfield.parents('#line-item-manager tr');

        var price = row.find('td:nth-child(7) input').val();
        var quantity = row.find('td:nth-child(8) input').val();

        var cost = 0;
        if (row.find('input.field-widget-field-cost').length > 0) {
          cost = row.find('input.field-widget-field-cost').val();
        } else {
          var cost_string = row.children('td:nth-child(4)').html();
          cost = cost_string ? cost_string.replace(/[^0-9]/g, '') : 0;
        }
        var margin = cost > 0 ? Math.round(100 * (price - cost) / cost) : '';
        row.children('td:nth-child(5)').text(margin);
        row.children('td:nth-child(6)').text(currency_symbol.before + Math.round(price - cost) + ' ' + currency_symbol.after);
        row.children('td:nth-child(9)').text(currency_symbol.before + (price * quantity) + ' ' + currency_symbol.after);

        calculateOrderTotal(currency_symbol, context);
      });

      calculateOrderTotal(currency_symbol, context);
    }
  };

  function calculateOrderTotal(currency_symbol, context) {
    var total = 0;
    $('#line-item-manager tbody tr', context).each(function () {
      var row = $(this);
      var quantity = row.find('.form-item input[name$="[quantity]"]').val();
      var price = row.find('.field-widget-commerce-price-full input').val();
      if (quantity !== undefined && price !== undefined) {
        total += price * quantity;
      }
    });
    $('#order-edit-order-total .value', context).text(currency_symbol.before + total + ' ' + currency_symbol.after);
  }

})(jQuery);
