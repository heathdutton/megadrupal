/**
 * @file
 * This script generates a window to select another city, the user,
 * in case you have correctly identified. Urban data sent from
 * gb.module and list based on the established units and cities for them.
 *
 */
(function($) {
  Drupal.behaviors.gb = {
    attach: function(context, settings) {
      /**
       * Set parameters from module
       *
       * cities - List of uniq cities used in module.
       * user_city - The city which is defined by the first pass, or obtained from cookies.
       * text - The text that is displayed in front of the city in the top toolbar.
       */
      var cities = Drupal.settings.geo_block_cities;
      var user_city = Drupal.settings.geo_block_user_city;
      var text = Drupal.settings.geo_block_text;

      /**
       * This is to ensure that after the selection of the city did not write Object.
       */
      if ($.isPlainObject(user_city)) {
        user_city = user_city[0];
      }

      /**
       * Configurations for "TOP TOOLBAR"
       */
      if (Drupal.settings.geo_block_top_toolbar_enabled == 1) {
        var topToolbarCookie = Drupal.settings.geo_block_top_toolbar_usecookie;

        topToolbarContent = "<div id='gb_city_select_1_close'></div>";
        topToolbarContent += "<div id='gb_city_select_1_container'>";
        topToolbarContent += text + " <a href='#' id='gb_city_select_1_link'>" + user_city + "</a>";
        topToolbarContent += "</div>";

        if ($.cookie("geo_block_city_select_1_disabled") === 0 || $.cookie("geo_block_city_select_1_disabled") === null) {
          $('<div/>', {
            id: 'gb_city_select_1_wrapper',
            html: topToolbarContent
          }).appendTo('body');

          $('body').addClass('gb_placeholder');

          $('#gb_city_select_1_close').click(function() {
            if (topToolbarCookie == 1) {
              $.cookie("geo_block_city_select_1_disabled", 1);
            }

            $('#gb_city_select_1_wrapper').animate({
              height: "0px"
            }, 400, function() {
              $('#gb_city_select_1_wrapper').remove();
            });

            $('body').animate({
              marginTop: "0px"
            }), 1000;
          });

          cities_list_window = "<div id='cities_list_window_wrapper'>";
          cities_list_window += "<div id='cities_list_window_content'><div id='cities_list_window_close'></div>";
          cities_list_window += "<ul>";
          for (i = 0; i < cities.length; i++) {
            if (cities[i] == $('#gb_city_select_1_link').text()) {
              cities_list_window += "<li>" + cities[i] + "</li>";
            }
            else {
              cities_list_window += "<li><a href='#'>" + cities[i] + "</a></li>";
            }
          }
          cities_list_window += "</ul></div></div>";

          $('#gb_city_select_1_link').click(function() {
            $(cities_list_window).appendTo('body');

            $('#cities_list_window_wrapper').click(function() {
              $('#cities_list_window_wrapper').remove();
            });

            $('#cities_list_window_content > ul a').click(function() {
              $.cookie("geo_block_user_city", encodeURIComponent(this.innerHTML));
              window.location.reload();
            });
          });
        }
      }

      /**
       * Configurations for "Placeholder".
       */
      if (Drupal.settings.geo_block_placeholder_enabled == 1) {
        // Prepare all elements
        placeholder_html_select = "<a name='city_select' id='gb_city_change_link'>" + user_city + "</a>";

        // If true, then used new method on page.
        if ($('body *').is('city-change')) {
          placeholder = 'city-change';
        }
        else {
          placeholder = '#gb_placeholder';
        }

        // Append element to placeholder.
        $(placeholder).append(placeholder_html_select);

        placeholder_html_popup = "<ul class='gb_city_change_embeded_popup'>";
        for (i = 0; i < cities.length; i++) {
          if (cities[i] == $('#gb_city_change_link').text()) {
            placeholder_html_popup += "<li>" + cities[i] + "</li>";
          }
          else {
            placeholder_html_popup += "<li><a name='" + cities[i] + "'>" + cities[i] + "</a></li>";
          }
        }
        placeholder_html_popup += "</ul>";

        // Click on city name
        $(placeholder + ' > a').click(function() {
          if ($('body *').is('.gb_city_change_embeded_popup')) {
            $('body * .gb_city_change_embeded_popup').remove();
          }
          else {
            $(placeholder_html_popup).appendTo(placeholder);

            $(placeholder + ' ul > li > a').click(function() {
              $.cookie("geo_block_user_city", encodeURIComponent(this.innerHTML));
              window.location.reload();
            });

          }
        });

        // Styles
        $(placeholder + ' a').css({
          'cursor': 'pointer'
        });
      }
    }
  }
})(jQuery);
