(function($) {
  
  Drupal.behaviors.webform_marketing = {
    attach: function(context) {
      //Get array of url params
      var prefixes = [];
      
      function init_prefexis() {
        var i = 0;
        for(var key in Drupal.settings.webform_marketing.url_params) {
          prefixes.push(key);
        }
        console.log(prefixes);
      }
      
      function testUrlParam(param) {
        var result = false;
        if (prefixes === '*') return true;
        for(var i = 0; i < prefixes.length; i++) {
          if (param.indexOf(prefixes[i]) === 0) {
            result = true;
            break;
          }
        }
        return result;
      }
      
      function setCookie(c_name, value){
        if (value !== "" && value != undefined && value != null && testUrlParam(c_name)) {
          var d = new Date();
          d.setTime(d.getTime() + (Drupal.settings.webform_marketing.hours * 60 * 60 * 1000));
          var expires = "expires=" + d.toGMTString();
          document.cookie = c_name + "=" + escape(value) + ";path=/;" + expires;
        }
      }

      function getCookie(name) {
        var matches = document.cookie.match(new RegExp(
          "(?:^|; )" + name.replace(/([\.$?*|{}\(\)\[\]\\\/\+^])/g, '\\$1') + "=([^;]*)"
        ));
        return matches ? decodeURIComponent(matches[1]) : null;
      }


      function fillForm(params) {
        for(var key in params) {
          $('input[url_param="' + key + '"]').val(params[key]);
        }
      }

      function getParameterByName(name) {
        name = name.replace(/[\[]/, "\\\[").replace(/[\]]/, "\\\]");
        var regex = new RegExp("[\\?&]" + name + "=([^&#]*)"),
        results = regex.exec(location.search);
        return results === null ? "" : decodeURIComponent(results[1].replace(/\+/g, " "));
      }

      function getUrlParams() {
        var params = {},
            params_string = location.search.substring(1).split('&');
        $.each(params_string, function(i, value){
          if (value.indexOf('=')) {
            var t = value.split('=');
            params[t[0]] = getParameterByName(t[0]);
          }
        });
        return params;
      }

      function setCookies(params) {
        for (var key in params) {
          setCookie(key, params[key]);
        } 
        if (document.referrer.indexOf(location.hostname) == -1) {
          setCookie('sitereferrer', document.referrer);
        }
      }

      function getCookies() {
        var params = {};
        $('input[is_wmarketing="1"]').each(function(i, value){
          var key = $(this).attr('url_param');
          params[key] = getCookie(key);
        });    
        return params;
      }
      
      init_prefexis();
      setCookies(getUrlParams());
      fillForm(getCookies());
    }
  };
})(jQuery);


