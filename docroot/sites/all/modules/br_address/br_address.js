(function ($) {

  Drupal.behaviors.br_address = {
    attach: function (context, settings) {

      $('.field-type-addressfield .br-address-cidade-estado input.locality', context).each(function () {
        $(this).bind('change', function() {
          var cityAndState = $('#autocomplete li.selected').text();
          var city = cityAndState.substr(0, cityAndState.length - 4);
          var state = cityAndState.substr(cityAndState.length - 2);
          $(this).val(city).parent().parent().find('.state').val(state);
        })
      })
    }
  };

})(jQuery);
