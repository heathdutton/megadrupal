/**
 * @file
 * supercookie.js
 */

(function ($) {

  Drupal.behaviors.supercookie = {
    attach : function(context, settings) {

      Drupal.behaviors.supercookie.writeCookie(context, settings);

    },
    writeCookie : function(context, settings) {

      if ($.isEmptyObject(navigator)) {
        return false;
      }

      // Loop navigator object and copy key/val pairs to $data.
      var $data = {};
      for (var $member in navigator) {
        switch (typeof navigator[$member]) {
          case 'object':
          case 'string':
          case 'boolean':
            $data[$member] = navigator[$member];
            break;

          case 'function':
            //navigator[$member]();
            break;
        }
      }

      // Do deep recursion on data collected.
      // @see https://github.com/Canop/JSON.prune
      $data = JSON.prune($data);
      // Set hash of data string.
      // @see https://code.google.com/p/crypto-js/#MD5
      $hash = CryptoJS.MD5($data);
      // Get local date/time.
      // TODO: NOTE SAFARI AND IE'S MISHANDLING OF ECMA DATE OBJECT FORMAT!!!!
      $date = new Date($.now());
      $date = $date.toLocaleString();
      $date = encodeURIComponent($date);

      // Get server response.
      $.ajax({
        type : 'GET',
        url : settings.supercookie.json_path + '?client=' + $hash + '&date=' + $date,
        beforeSend : function(xhr) {
          xhr.setRequestHeader(settings.supercookie.name_header, settings.supercookie.scid);
        },
        complete : function(xhr) {
          if (xhr.status === 200) {
            // Set client-side cookie.
            var response = JSON.parse(xhr.responseText);
            var expires = new Date(response.expires * 1000);
            document.cookie = settings.supercookie.name_server + '=""; expires=-1; path=/';
            document.cookie = settings.supercookie.name_server + '=' + response.scid + '; expires=' + expires.toGMTString() + '; path=/';
          }
        }
      });

      return false;
    }
  };

})(jQuery);
