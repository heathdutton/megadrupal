(function ($) {

  Drupal.behaviors.externanaliFrame = {
    attach: function(context, settings) {
      // Set iframe height 
        function resizeframe() { 
        var buffer = $('#external-container').height();
        var newHeight = $(window).height();
        var newFrameHeight = newHeight - buffer;
        $('#external-site-container').css('height', newFrameHeight);
      }
      $(window).resize(function() {
        resizeframe();
      });
      $(window).load(function() {
        resizeframe();  
      });
      

      // Rewrite external links
      var appendUrl = Drupal.settings.basePath + 'external?url=';
      var http_host = location.hostname.split('.');
      if (http_host.length == 3) {
        var host = http_host[1] + '.' + http_host[2];
      } else if (http_host.length == 2) {
        var host = http_host[0] + '.' + http_host[1];
      } else if (http_host.length == 1) {
        var host = http_host[0];
      }
      
      // Declare Variables
      var parent = Drupal.settings.externaliframe.parent;
      var classes = Drupal.settings.externaliframe.classes;
      var negate = Drupal.settings.externaliframe.negate;
      var excluded = Drupal.settings.externaliframe.exclude;
      var selectors = new Array();
      var i = 0;
      if (parent.length > 0) {
        parent += ' ';
      }
           
      // Rewrite all external links
      if (classes.length > 0 && classes.indexOf('*') == -1 && negate == 0) {
        for (i=0;i<classes.length;i++) {
          selectors[i] = parent + 'a[href^=http:]' + classes[i] + ':not(.external-nofollow)';
        }
      }
      else if (classes.length > 0 && classes.indexOf('*') == -1 && negate == 1) {
        for (i=0;i<classes.length;i++) {
          selectors[i] = parent + 'a[href^=http:]:not(' + classes[i] + ', .external-nofollow)';
        }
      }
      else if (classes.indexOf('*') >= 0) {
        for (i=0;i<classes.length;i++) { 
          selectors[i] = parent + 'a[href^=http:]:not(.external-nofollow)';
        }      
      }
      // Concat array items to string
      var selector = selectors.join(',');
      $(selector).each(
        function(){
          if(this.href.indexOf(host) == -1 && location.pathname.indexOf('external') == -1) { 
              var currentUrl = this.href;
              var questionMarkCount = currentUrl.length - currentUrl.replace(/\?/gi, '').length;
              // If currentUrl has two sets of parameters, ignore it ... see http://drupal.org/node/1534486
              if (questionMarkCount < 2) {
                var newUrl = appendUrl + currentUrl;
                for (i=0;i<excluded.length;i++) {
                  var str = excluded[i].replace(/(\r\n|\n|\r)/gm,"");;
                  if (currentUrl.indexOf(str) >= 0) {
                    var stop = true;
                  }
                }
                if (typeof stop == 'undefined') {
                  $(this).attr('href', newUrl);
                }
              }               
          }
        });
       // End of module code
    }
  };

})(jQuery);
