/**
 * @file
 * Main JS file for the Internet Defense League module.
 */

(function (w, d, $, undefined) {
  // We use behaviors here to make sure things are loaded properly
  Drupal.behaviors.internetDefenseLeague = {
    attach: function (context, settings) {
      // Check that our settings are loaded first
      if (typeof settings.internetDefenseLeague == 'undefined') {
        return;
      }
      
      // Only handle once
      $('body', context).once('internetDefenseLeague', function () {
        // Extend defaults
        var defaults = {
          'variant': 'modal', // modal, banner
          'campaign': null // null, idl_launch
        }
        var config = $.extend(defaults, settings.internetDefenseLeague);

        w._idl = {};
        _idl = config;
        (function() {
          var idl = d.createElement('script');
          idl.type = 'text/javascript';
          idl.async = true;
          idl.src = ('https:' == d.location.protocol ? 'https://' : 'http://') + 
            'members.internetdefenseleague.org/include/?url=' + 
            (_idl.url || '') + '&campaign=' + (_idl.campaign || '') + 
            '&variant=' + (_idl.variant || 'banner');
          d.getElementsByTagName('body')[0].appendChild(idl);
        })();
      });
    }
  };
})(window, document, jQuery); 
