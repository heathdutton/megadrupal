/**
 * @file
 * GeoComplete handler.
 */

(function ($) {

  /**
   * Update the data of an addressfield from an autocompleted input field.
   * @param input
     */
  var webformAddressfieldExtraUpdate = function(input) {

    var data = {};
    var result = input.data('result');
    var addressfield = input.data('addressfield');
    var widget = $('.webform-addressfield-extra-wrapper', addressfield);

    /*
     * Load all of the address details obtained by the geocomplete
     * into a data array to be used later.
     */
    $.each(result.address_components, function (index, object) {
      var name = object.types[0];
      data[name] = object.long_name;
      data[name + "_short"] = object.short_name;
    });
    if (result.geometry.location !== undefined) {
      data['lat'] = result.geometry.location.lat();
      data['lng'] = result.geometry.location.lng();
    }
    if (result.name !== undefined) {
      data['organisation_name'] = result.name;
    }

    /*
     * Clear all data before commencing the update
     */
    widget.find('input,select.state').val('');

    /*
     * Check that the country in the result is the same as widget
     * If not we need to trigger the change.
     */
    var country = widget.find('select.country:first');
    if (data.country_short !== country.val()) {
      country.val(data.country_short);
      country.trigger('change', [{
        update: true
      }]);
    }

    /*
     * If the change has been triggered because the addressfield
     * module uses ajax to change the widget we will need to come
     * in this function again to update the rest of the values
     * as the postcode and county field are not always in the
     * widget. To update we loop through all fields inside the
     * widget that have an attribute with data-geo. We then explode
     * the data-geo value on a space because we sometimes wish to
     * add multiple values to the same field. We then replace the
     * value with the data value.
     */
    $.each(widget.find('[data-geo]'), function () {
      var object = $(this);
      var geo = object.data('geo').split(" ");
      for (var i = 0; i < geo.length; i++) {
        // If no data in result then continue
        if (data[geo[i]] === undefined) {
          continue;
        }
        // Handle subpremise
        if (geo[i] === 'subpremise' && geo[(i + 1)] === 'street_number') {
          $(this).val(data[geo[i]] + '/' + data[geo[(i + 1)]]);
          i++;
        }
        // Hanlde rest of thoroughfare
        else if (geo[i] === 'street_number' || geo[i] === 'route') {
          $(this).val() ? $(this).val($(this).val() + " " + data[geo[i]]) : $(this).val(data[geo[i]]);
        }
        else {
          $(this).val(data[geo[i]]);
          // If select and value is not empty
          if ($(this).is('select') && $(this).val().length > 0) {
            break;
          }
        }
      }
    });
  };

  Drupal.behaviors.webform_addressfield_extra = {
    attach: function(context, settings) {

      $('div.webform-component-addressfield').each(function () {
        var $addressfield = $(this);
        var $inputfield = $addressfield.find('.webform-addressfield-autocomplete-input');
        var $autcompletion_block = $addressfield.find('div[class*="autocompletion-block"]');
        var $toogler = $addressfield.find(".webform-addressfield-extra-wrapper--toogle");

        // Ensure the page we're on actually contains an addressfield
        // component with autocompletion on.
        if ($autcompletion_block.length == 0) {
          return;
        }

        // Store the corresponding details_block in the widget field.
        $inputfield.data('addressfield', $addressfield);

        $inputfield.geocomplete({
          //details: $($details_block, $addressfield),
          detailsAttribute: "data-geo"
        }).bind("geocode:result", function (event, result) {
          /*
           * Reveal the standard addressfield widget.
           * Update the widget, we have to do it this way instead
           * of the method provided by geocomplete because the
           * addressfield widget has a select for country.
           */
          $(this).data('result', result);
          webformAddressfieldExtraUpdate($inputfield, $addressfield);
        });

        /**
         * Reveal addressfield on clicking for 'manual' inputs.
         */
        $toogler.once().click(function () {
          $('.webform-addressfield-extra-wrapper', $addressfield).toggle("slow", function () {
            // Animation complete.
            $(this).data('visible', true);
          });
          //$(this).hide();
        });
      });

      /*
       * Methods to update the address after an ajax callback.
       */
      if (context.length) {
        /*
         * When we trigger a change on the addressfield country
         * select this will be returned. As new fields may have
         * been added we need to update the address.
         */
        if (context[0] !== undefined && context.has('[id^="addressfield-wrapper"]')) {
          var input = context.prevAll('.form-item').find('.webform-addressfield-autocomplete-input');
          if (input[0] !== undefined && input.data('result') !== undefined) {
            webformAddressfieldExtraUpdate(input);
          }
        }
      }
    }
  };
})(jQuery);
