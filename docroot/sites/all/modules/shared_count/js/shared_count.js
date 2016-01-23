(function ($) {
  Drupal.behaviors.shared_count = {
    attach: function (context, settings) {
      $.sharedCount = function(url, fn) {
        url = encodeURIComponent(url || location.href);
        var domain = settings.setting.shared_count_request_domain;
        var apikey = settings.setting.shared_count_api_key;
        var arg = {
          data: {
            url : url,
            apikey : apikey
          },
          url: domain,
          cache: true,
          dataType: "json"
        };
        if ('withCredentials' in new XMLHttpRequest()) {
          arg.success = fn;
        }
        else {
          var cb = "sc_" + url.replace(/\W/g, '');
          window[cb] = fn;
          arg.jsonpCallback = cb;
          arg.dataType += "p";
        }
        return jQuery.ajax(arg);
      };
      if ($('.shared-count-block .shared-count').length) {
        $.sharedCount(window.location, function(data) {
          var count  = data.Facebook.share_count;
          count += data.GooglePlusOne;
          count += data.Twitter;
          count += data.LinkedIn;
          count += data.Pinterest;
          count += data.StumbleUpon;
          $('.shared-count-block .shared-count').text(count);
        });
      }
    }
  };
  // Expand
  $(document).ready(function() {
    $('.shared-count-more-social-networks').click(function(){
      $('.shared-count-secondary-shares').toggleClass('show-element');
      $('.shared-count-more-social-networks').toggleClass('shared-count-more-social-networks-collapse');
    });
  });

})(jQuery);
