(function($) {

// I don't know, what am I do. If I will,
// see: http://sachachua.com/blog/2011/08/drupal-overriding-drupal-autocompletion-to-pass-more-parameters/'

//  Drupal.ACDB.prototype.customSearch = function (searchString) {
//      searchString = searchString + "/" + $("#otherfield").val();
//      return this.search(searchString);
//  };
//
//  Drupal.jsAC.prototype.populatePopup = function () {
//    // Show popup
//    if (this.popup) {
//      $(this.popup).remove();
//    }
//    this.selected = false;
//    this.popup = document.createElement('div');
//    this.popup.id = 'autocomplete';
//    this.popup.owner = this;
//    $(this.popup).css({
//      marginTop: this.input.offsetHeight +'px',
//      width: (this.input.offsetWidth - 4) +'px',
//      display: 'none'
//    });
//    $(this.input).before(this.popup);
//
//    // Do search
//    this.db.owner = this;
//    if (this.input.id == 'edit-your-search-field') {
//      this.db.customSearch(this.input.value);
//    } else {
//      this.db.search(this.input.value);
//    }
//  }
//
//  Drupal.behaviors.rebindAutocomplete = function(context) {
//    // Unbind the behaviors to prevent multiple search handlers
//    $("#edit-your-search-field").unbind('keydown').unbind('keyup').unbind('blur').removeClass('autocomplete-processed');
//    // Rebind autocompletion with the new code
//    Drupal.behaviors.autocomplete(context);
//  }

  Drupal.behaviors.addressfield_hu_admin = {
    attach: function (context, settings) {
      var counter = Drupal.settings.addressfield_hu.counter;

      for (var i = 1; i <= counter; i++) {
        var county_id = '#address-hu-county-' + i;
        var postal_code = '#address-hu-postal-code-' + i;
        //var postal_code_options = 'address-hu-postal-code-options' + counter[i];
        $(county_id + ' select').change(function(){
          // Tmp disabled the hidden postal code field.
          // @todo: remove it, if not necessary
          //$(postal_code).hide();
        });
      }
//      for (var i = 0; i < addressfield_hu_fields.length; i++) {
//        var target = '*[name="' + addressfield_hu_fields[i] + '"]';
//        $(target).change (function(){
//          var url = Drupal.settings.addressfield_hu.autocomplete_url + $(this).val();
//          $(this)
//            .closest('.addressfield-hu-processed')
//            .find('.autocomplete-processed')
//            .attr('value', url)
//            .removeClass('autocomplete-processed')
//            .unbind('keydown')
//            .unbind('keyup')
//            .unbind('blur')
//          //Drupal.behaviors.autocomplete(context);

////          Drupal.behaviors.autocomplete.attach(context, settings);
//        });
//      }
    }
  }
})(jQuery);