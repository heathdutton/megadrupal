/**
 * @file
 * Main user AJAX calls to enable personalization with site caching.
 * 
 */
var pzs_loader = '';

(function($) {

  var personalization = {
    $block: null,
    $listing: null,

    init: function() {
      var data = {
        referer: document.referrer,
        path: window.location.href,
        nid: null
      };

      if (typeof Drupal.settings.personalization.currentNid != undefined) {
        data.nid = Drupal.settings.personalization.currentNid;
      }

      if (pzs_loader == '') {
        pzs_loader = Drupal.settings.basePath + Drupal.settings.personalization.module_path + '/ajax-loader.gif';
      }
      pzs_loader = '<img src="' + pzs_loader + '" alt="loading" />';

      $.ajax({
        type: 'POST',
        url: Drupal.settings.basePath + '?q=personalization/ajax/init',
        data: data,
        dataType: 'json',
        success: function(response){
          if (response.track) {
            if (response.get_location) {
              personalization.get_location();
            }

            personalization.$block = $('#block-personalization-personalized-content');
            if (personalization.$block.length) {
              personalization.get_block();
            }

            personalization.$listing = $('#personalized-content-list');
            if (personalization.$listing.length) {
              personalization.get_list();
            }
          }
        }
      });
    },

    get_location: function() {
      if (navigator.geolocation) {
        navigator.geolocation.getCurrentPosition(
          personalization.send_location,
          function() {},//No need to capture failure
          {
            enableHighAccuracy : true,
            timeout : 10 * 1000 * 1000,
            maximumAge : 0
          }
        );
      }
    },

    send_location: function(position) {
      $.ajax({
        type: "POST",
        url: Drupal.settings.basePath + '?q=personalization/ajax/user/location',
        data: {latitude: position.coords.latitude, longitude: position.coords.longitude}
      });
    },

    get_block: function() {
      personalization.$block.html(pzs_loader);
      $.ajax({
        url: Drupal.settings.basePath + '?q=personalization/ajax/get-block',
        dataType: 'text',
        success: function(response) {
          personalization.$block.replaceWith(response);
        }
      });
    },

    get_list: function() {
      var limit = personalization.$listing.find('ul:not(.pager) li').length;
      var page = decodeURI((RegExp('page=' + '(.+?)(&|$)').exec(location.search)||[,0])[1]);

      personalization.$listing.html(pzs_loader);
      $.ajax({
        type: 'GET',
        data: {page: page},
        url: Drupal.settings.basePath + '?q=personalization/ajax/get-list/' + limit,
        dataType: 'text',
        success: function(response) {
          personalization.$listing.replaceWith(response);
        }
      });
    }
  };

  $(function() {
    personalization.init();
  });

})(jQuery);
