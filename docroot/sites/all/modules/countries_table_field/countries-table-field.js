
(function ($) {

Drupal.behaviors.countriesTableFieldContinents = {
  attach: function(context) {
    $('.countries-table-per-continent').once('countries-table-per-continent', function() {
      var countries_table = $(this);
      $('thead .form-checkbox', countries_table).change(function () {
        if (this.checked) {
          $('tbody .form-checkbox', countries_table).attr('checked','checked');
        }
        else {
          $('tbody .form-checkbox', countries_table).removeAttr('checked');
        }
      });
    });
  }
};

Drupal.behaviors.countriesTableFieldCountries = {
  attach: function(context) {
    $('.countries-table-per-country').once('countries-table-per-country', function() {
      var countries_table = $(this);
      $('thead .form-checkbox', countries_table).change(function () {
        var target = 'tbody .' + $(this).val() + '-marker';
        if (this.checked) {
          $(target, countries_table).attr('checked','checked');
        }
        else {
          $(target, countries_table).removeAttr('checked');
        }
      });
    });    
  }
};

})(jQuery);
