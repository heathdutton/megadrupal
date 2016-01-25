/*jslint nomen: true, plusplus: true, todo: true, white: true,
  browser: true, indent: 2 */

(function() {
  var v = "1.8.3";

  // Install jQuery and initiate bookmarklet
  if (window.jQuery === undefined || window.jQuery.fn.jquery < v) {
    var done = false;
    var script = document.createElement("script");
    script.src = window.simpleBookmarkletProtocol +
                 "://ajax.googleapis.com/ajax/libs/jquery/" +
                 v +
                 "/jquery.min.js";
    script.onload = script.onreadystatechange = function() {
      if (!done &&
         (!this.readyState ||
          this.readyState == "loaded" ||
          this.readyState == "complete")) {
        done = true;
        initSimpleBookmarklet();
      }
    };
    document.getElementsByTagName("head")[0].appendChild(script);
  }
  else {
    initSimpleBookmarklet();
  }

  // Initiate bookmarklet
  function initSimpleBookmarklet() {
    (window.simpleBookmarklet = function() {

      var SimpleBookmarklet = {};

      // Function to get information about the page
      function getPageInformation(key) {
        switch(key) {
          case 'url':
            return document.location.href;
            break;
          case 'title':
            return document.title;
            break;
          case 'selected':
            return getSelText();
            break;
        }
        return "";
      }

      // Function to get text selected by the user
      function getSelText() {
        var s = '';
        if (window.getSelection) {
          s = window.getSelection();
        } else if (document.getSelection) {
          s = document.getSelection();
        } else if (document.selection) {
          s = document.selection.createRange().text;
        }
        if (s == "") {
          return document.title;
        }
        else {
          return s;
        }
      }

      // Function that creates the bookmarklet
      function createBookmarklet() {
        var s = getSelText(),
            url = SimpleBookmarklet.settings.url;

        $.each(SimpleBookmarklet.settings.prepopulate, function(field, v) {
          v = encodeURIComponent(getPageInformation(v)).replace(/'/g, '"');
          url += '&simple_bookmarklet_prepopulate[' + field + ']=' + v;
        });

        var css = '<link type="text/css" rel="stylesheet" href="' +
                  SimpleBookmarklet.settings.default_stylesheet +
                  '?' + parseInt(Math.random()*10000000) + '" media="all" />';
        $("head").append(css);

        if (SimpleBookmarklet.settings.stylesheet != '') {
          var css = '<link type="text/css" rel="stylesheet" href="' +
                    SimpleBookmarklet.settings.stylesheet +
                    '?' + parseInt(Math.random()*10000000) + '" media="all" />';
          $("head").append(css);
        }

        if (SimpleBookmarklet.settings.javascript != '') {
          var js = '<script type="text/javascript" src="' +
                   SimpleBookmarklet.settings.javascript +
                   '?' + parseInt(Math.random()*10000000000) + '"></script>';
          $("body").append(js);
        }

        var js = '<div id="simple_bookmarklet_cont">' +
                 '  <a id="simple_bookmarklet_close"><span>[X]</span></a>' +
                 '  <iframe src="' +
                 url +
                 '" id="simple_bookmarklet_frame" seamless></iframe>' +
                 '</div><div id="simple_bookmarklet_mask"></div>';
        $("body").append(js);

        $('#simple_bookmarklet_close').live('click', function() {
          $('#simple_bookmarklet_cont, #simple_bookmarklet_mask').fadeOut(500);
          $('body, html').css('overflow', 'auto');
        });

        $('body, html').scrollTop(0);
        $('body, html').css('overflow', 'hidden');
      }

      // First time, create bookmarklet window
      if ($("#simple_bookmarklet_cont").length == 0) {

        // Load settings
        $.getJSON(window.simpleBookmarkletURL, function(json) {
          SimpleBookmarklet.settings = json;
          createBookmarklet();
        });
      }

      // After the first time, just show and hide the bookmarklet
      else {
        $("#simple_bookmarklet_cont, #simple_bookmarklet_mask").fadeIn(500);
        $('body, html').scrollTop(0);
        $('body, html').css('overflow', 'hidden');
      }

    })();
  }
})();
