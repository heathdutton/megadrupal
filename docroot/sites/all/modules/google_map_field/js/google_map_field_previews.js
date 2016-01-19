
/**
 * @file
 * This file contains the javascript functions used by the google map field
 * widget
 */

/**
 * Add code to generate the maps on page load.
 */
(function ($) {

  Drupal.behaviors.google_map_field = {
    attach: function (context) {

      googleMapFieldPreviews();

      $('.google-map-field-clear').bind('click', function(event) {
        event.preventDefault();
        var data_delta = $(this).attr('data-delta');
        var data_field_id = $(this).attr('data-field-id');
        $('input[data-name-delta="'+data_delta+'"][data-name-field-id="'+data_field_id+'"]').prop('value', '').attr('value', '');
        $('input[data-lat-delta="'+data_delta+'"][data-lat-field-id="'+data_field_id+'"]').prop('value', '').attr('value', '');
        $('input[data-lng-delta="'+data_delta+'"][data-lng-field-id="'+data_field_id+'"]').prop('value', '').attr('value', '');
        $('input[data-zoom-delta="'+data_delta+'"][data-zoom-field-id="'+data_field_id+'"]').prop('value', '').attr('value', '');
        googleMapFieldPreviews(data_delta);
      });

      $('.google-map-field-defaults').bind('click', function(event) {
        event.preventDefault();
        var data_delta = $(this).attr('data-delta');
        var data_field_id = $(this).attr('data-field-id');
        $('input[data-name-delta="'+data_delta+'"][data-name-field-id="'+data_field_id+'"]').prop('value', $(this).attr('data-default-name')).attr('value', $(this).attr('data-default-name'));
        $('input[data-lat-delta="'+data_delta+'"][data-lat-field-id="'+data_field_id+'"]').prop('value', $(this).attr('data-default-lat')).attr('value', $(this).attr('data-default-lat'));
        $('input[data-lng-delta="'+data_delta+'"][data-lng-field-id="'+data_field_id+'"]').prop('value', $(this).attr('data-default-lon')).attr('value', $(this).attr('data-default-lon'));
        $('input[data-zoom-delta="'+data_delta+'"][data-zoom-field-id="'+data_field_id+'"]').prop('value', $(this).attr('data-default-zoom')).attr('value', $(this).attr('data-default-zoom'));
        googleMapFieldPreviews(data_delta);
      });

      $('.google-map-field-watch-change').change(function(event) {
        var data_delta = $(this).attr('data-lat-delta') || $(this).attr('data-lng-delta') || $(this).attr('data-zoom-delta');
        var data_field_id = $(this).attr('data-lat-field-id') || $(this).attr('data-lng-field-id') || $(this).attr('data-zoom-field-id');
        googleMapFieldPreviews(data_delta);
      });

    }
  };

})(jQuery);
