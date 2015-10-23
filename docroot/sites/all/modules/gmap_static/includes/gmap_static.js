
/**
 * @file
 * Replace iframe map to static image map. And return it according to settings.
 */

(function($) {
  /**
   * Implements Drupal.behaviors event.
   */
  Drupal.behaviors.gmap_static = {
    attach : function(context, settings) {
      var view_mode = Drupal.settings['gmap_static_view_mode'][current_device_is()];
      $('iframe').once(function() {
        var iframesrc = $(this).attr('src');
        if (is_google_map_frame(iframesrc)) {
          create_static_map($(this), view_mode);
        }
      });
    }
  };

  /**
   * Checks to see current device type.
   *
   * @return string computer|mobile
   */
  function current_device_is() {
    var mobile = ['iPhone', 'Android', 'webOS', 'BlackBerry', 'iPod', 'iPad'];
    for (var agent in mobile) {
     if (navigator.userAgent.indexOf(mobile[agent]) >= 0) {
         return 'mobile';
      }
    }
   return 'computer';
  }

  /**
   * Shows interactive map in a pop-up.
   *
   * @param Object @iframe
   *   GoogleMap iframe jQuery object.
   */
  function popup_map($iframe) {
    $('body').prepend($('<div/>', {'class' : 'gmap_static_bigmap', }));
    $('div.gmap_static_bigmap').prepend($('<iframe/>', {
      'class' : 'gmap_static_iframe_fullscreen',
      'src' : $iframe.attr('src'),
    }));
    $('div.gmap_static_bigmap').append($('<div/>', {
      'class' : 'gmap_static_collapse',
      click : function() {
        $('.gmap_static_bigmap').remove();
      },
    }));
  }

  /**
   * Changes back the static map to the interactive map.
   *
   * @param Object @iframe
   *   GoogleMap iframe jQuery object.
   */
  function change_condition($iframe) {
    $image = $('img', $iframe.parent());
    $image.hide();
    $iframe.show();
    $iframe.attr('src', $iframe.attr('src'));
  }

  /**
   * Shows interactive map in new window.
   *
   * @param Object @iframe
   *   GoogleMap iframe jQuery object.
   */
  function in_new_window($iframe) {
    var window_map = window.open();
    var html = $iframe.clone().wrap("<iframe/>").parent().html();
    html = html.replace('display: none;', 'display: block;');
    $(window_map.document.body).html(html);
  }

  /**
   * Converts a Google map coordinates to the parametres array.
   *
   * @param string src
   *   String with source of GoogleMap iframe.
   * 
   * @return array
   *   Array contents parametres to create static image:
   *   - zoom: Map Scale.
   *   - markers: Map markers, if exist.
   *   - center: 'X,Y' map coordinates.
   *   - maptype: Visual type of map. Satellite or Roadmap.
   */
  function map_iframe_source_to_params(src) {
    var attributes = new Array();
    var keys_and_values = new Array();
    var param = new Array();
    var map_type = src.substring(src.lastIndexOf('/'), src.indexOf('?'));
    if (map_type == '/embed') {
      return pb_coordinates(src);
    }
    attributes = (src.substr(0)).split('&');
    for (var j = 1; j < attributes.length; j++) {
      values = attributes[j].split('='); 
      param[values[0]] = values[1];
    }
    if (param['q']) {
      param['center'] = param['q'];
      delete param['q'];
    }
    if (map_type == '/place') {
      param['markers'] = param['center'];
    }
    delete param['key'];
    return param;
  }

  /**
   * Creates DOM-element image with Google Map Static params.
   *
   * @param array param
   *   Map parametres array.
   * @param int height
   *   HTML element height attribute.
   * @param int width
   *   HTML element width attribute.
   *
   * @return Object $static_map
   *   jQuery DOM object of Static Map image.
   */
  function get_map_image(param, height, width) {
    var map_source = "https://maps.googleapis.com/maps/api/staticmap?";
    for (var key in param) {
      map_source += key + '=' + param[key] + "&";
    }
    map_source += 'size=' + width +'x'+ height + '&';
    map_source += 'scale=2';
    $static_map = $('<img/>', {
      'src' : map_source,
      'width' : width,
      'height' : height,
    });
    return $static_map;
  }

  /**
   * Checks to see if this iframe it is a Google Map iframe.
   *
   * @param string iframesrc
   *   Source attribute of iframe element.
   *
   * @return bool
   */
  function is_google_map_frame(iframesrc) {
    var google_maps_link = 'google.com/maps/embed';
    if (iframesrc.indexOf(google_maps_link) != -1) {
      return true;
    }
    else {
      return false;
    }
  }

  /**
   * Creates a Static Map block wrapping, image and extend button, hide iframe.
   *
   * @param Object $iframe_map
   *   jQuery DOM object of iframe with Google Map embed.
   * @param string view_mode
   *   Current setting to show back map changes.
   */
  function create_static_map($iframe_map, view_mode) {
    var height = $iframe_map.attr('height');
    var width = $iframe_map.attr('width');
    var param = map_iframe_source_to_params($iframe_map.attr('src'));
    $iframe_map.wrap($('<div/>', {'class' : 'gmap_static_map_block'}));
    $static_map = get_map_image(param, height, width);
    $iframe_map.after($static_map);
    $static_map.after($('<div/>', {
      'class' : 'gmap_static_extend',
      click: function() {
        switch (view_mode) {
          case 'popup_map':
            popup_map($iframe_map);
            break;
          case 'change_condition':
            $(this).hide();
            change_condition($iframe_map);
            break;
          case 'in_new_window':
            in_new_window($iframe_map);
            break;
        }
      }
    }));
    $iframe_map.hide();
  }
  
  /**
   * Converts a Google map pb-coordinates to the parametres array.
   *
   * @param string src
   *   String with source of pb-coordinates type GoogleMap iframe.
   * 
   * @return array
   *   Array contents parametres to create static image:
   *   - zoom: Map Scale.
   *   - markers: Map markers, if exist.
   *   - center: 'X,Y' map coordinates.
   *   - maptype: Visual type of map. Satellite or Roadmap.
   */
  function pb_coordinates(src) {
    var param = new Array();
    var coordinates = new Array(); 
    var maptype, coordinate_index, zoom;
    for (var i=1; i<=3; i++) {
      coordinate_index = src.indexOf('!' + i + 'd') + 3;
      coordinates[i] = src.substring(coordinate_index, src.indexOf('!', coordinate_index));
    }
    maptype = src.substr(src.indexOf('!5e')+3, 1);
    zoom = coordinates[1];
    zoom = (zoom / 4.355) / 1.645;
    zoom = Math.log(zoom) / Math.log(2);
    marker = src.indexOf('!2s') + 3;
    param['zoom'] = 24 - ~~zoom; 
    param['markers'] = src.substring(marker, src.indexOf('!', marker));
    param['center'] = coordinates[3] + ',' + coordinates[2];
    param['maptype'] = (maptype == 1) ? 'satellite' : 'roadmap';
    return param;
  }
})(jQuery);
