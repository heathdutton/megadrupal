
/**
 * @file
 * @author Bob Hutchinson http://drupal.org/user/52366
 * @copyright GNU GPL
 *
 * Javascript functions for getdirections_fields module settings
 * jquery gee whizzery
*/
(function ($) {
  Drupal.behaviors.getdirections_fields_formatter = {
    attach: function() {

      if ($("input[id$=-settings-edit-form-settings-default-use-advanced]").attr('checked')) {
        $("#wrap-waypoints").show();
      }
      else {
        $("#wrap-waypoints").hide();
      }
      $("input[id$=-settings-edit-form-settings-default-use-advanced]").change(function() {
        if ($(this).attr('checked')) {
          $("#wrap-waypoints").show();
        }
        else {
          $("#wrap-waypoints").hide();
        }
      });

      if ($("input[id$=-settings-edit-form-settings-default-travelmode-show]").attr('checked')) {
        $("#getdirections_transit_dates_wrapper").show();
      }
      else {
        $("#getdirections_transit_dates_wrapper").hide();
      }
      $("input[id$=-settings-edit-form-settings-default-travelmode-show]").change(function() {
        if ($(this).attr('checked')) {
          $("#getdirections_transit_dates_wrapper").show();
        }
        else {
          $("#getdirections_transit_dates_wrapper").hide();
        }
      });

      if ($("input[id$=-settings-edit-form-settings-misc-trafficinfo]").attr('checked')) {
        $("#wrap-getdirections-trafficinfo").show();
      }
      else {
        $("#wrap-getdirections-trafficinfo").hide();
      }
      $("input[id$=-settings-edit-form-settings-misc-trafficinfo]").change(function() {
        if ($(this).attr('checked')) {
          $("#wrap-getdirections-trafficinfo").show();
        }
        else {
          $("#wrap-getdirections-trafficinfo").hide();
        }
      });

      if ($("input[id$=-settings-edit-form-settings-misc-bicycleinfo]").attr('checked')) {
        $("#wrap-getdirections-bicycleinfo").show();
      }
      else {
        $("#wrap-getdirections-bicycleinfo").hide();
      }
      $("input[id$=-settings-edit-form-settings-misc-bicycleinfo]").change(function() {
        if ($(this).attr('checked')) {
          $("#wrap-getdirections-bicycleinfo").show();
        }
        else {
          $("#wrap-getdirections-bicycleinfo").hide();
        }
      });

      if ($("input[id$=-settings-edit-form-settings-misc-transitinfo]").attr('checked')) {
        $("#wrap-getdirections-transitinfo").show();
      }
      else {
        $("#wrap-getdirections-transitinfo").hide();
      }
      $("input[id$=-settings-edit-form-settings-misc-transitinfo]").change(function() {
        if ($(this).attr('checked')) {
          $("#wrap-getdirections-transitinfo").show();
        }
        else {
          $("#wrap-getdirections-transitinfo").hide();
        }
      });

      if ($("input[id$=-settings-edit-form-settings-misc-panoramio-show]").attr('checked')) {
        $("#wrap-getdirections-panoramio").show();
      }
      else {
        $("#wrap-getdirections-panoramio").hide();
      }
      $("input[id$=-settings-edit-form-settings-misc-panoramio-show]").change(function() {
        if ($(this).attr('checked')) {
          $("#wrap-getdirections-panoramio").show();
        }
        else {
          $("#wrap-getdirections-panoramio").hide();
        }
      });

      if ($("input[id$=-settings-edit-form-settings-misc-weather-show]").attr('checked')) {
        $("#wrap-getdirections-weather").show();
      }
      else {
        $("#wrap-getdirections-weather").hide();
      }
      $("input[id$=-settings-edit-form-settings-misc-weather-show]").change(function() {
        if ($(this).attr('checked')) {
          $("#wrap-getdirections-weather").show();
        }
        else {
          $("#wrap-getdirections-weather").hide();
        }
      });
      if ($("input[id$=-settings-edit-form-settings-misc-weather-cloud]").attr('checked')) {
        $("#wrap-getdirections-weather-cloud").show();
      }
      else {
        $("#wrap-getdirections-weather-cloud").hide();
      }
      $("input[id$=-settings-edit-form-settings-misc-weather-cloud]").change(function() {
        if ($(this).attr('checked')) {
          $("#wrap-getdirections-weather-cloud").show();
        }
        else {
          $("#wrap-getdirections-weather-cloud").hide();
        }
      });

      if ($("input[id$=-settings-edit-form-settings-misc-geolocation-enable]").attr('checked')) {
        $("#wrap-getdirections-geolocation-options").show();
      }
      else {
        $("#wrap-getdirections-geolocation-options").hide();
      }
      $("input[id$=-settings-edit-form-settings-misc-geolocation-enable]").change(function() {
        if ($(this).attr('checked')) {
          $("#wrap-getdirections-geolocation-options").show();
        }
        else {
          $("#wrap-getdirections-geolocation-options").hide();
        }
      });


    }
  };
}(jQuery));
