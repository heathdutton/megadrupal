(function ($) {

/**
 * Grab the available videos and attach them to the proper elements.
 */
Drupal.tutorials = Drupal.tutorials || {};
Drupal.tutorials.overlay = '<div class="tutorials-border tutorials-border-top"></div><div class="tutorials-border tutorials-border-right"></div><div class="tutorials-border tutorials-border-bottom"></div><div class="tutorials-border tutorials-border-left"></div>';
Drupal.tutorials.contexts = [];

/**
 * Get the context id for a given context. If one doesn't exist yet, set up the page and add it to the array.
 */
Drupal.tutorials.getContextID = function(context) {
  var id = Drupal.tutorials.contexts.indexOf(context);
  // If uninitiated, initial the context links.
  if (id == -1) {
    id = Drupal.tutorials.contexts.length;
    Drupal.tutorials.contexts[id] = context;
    $('body').append('<div id="tutorials-' + id + '" class="tutorials"><div class="tutorials-wrapper"><a class="tutorials-toggler" onclick="javascript:Drupal.tutorials.toggleVis(\'tutorials-links-' + id + '\'); return false;">View Tutorials</a><ul id="tutorials-links-' + id + '" class="tutorials-links inline"></ul></div></div>')
  }
  return id;
}

/**
 * toggles visibility on an element
 */
Drupal.tutorials.toggleVis = function(element) {
  var element = $('#' + element);
  if (element.is(":hidden")) {
    element.slideDown('fast');
  } else {
    element.hide();
  }
  $('a.tutorials-toggler', element.parent()).toggleClass('tutorials-toggler-active');
}

/**
 * Pop up the video in a popup window.
 */
Drupal.tutorials.openPopup = function(nid, serverid) {
  $.ajax({
    'url': Drupal.settings.tutorials.servers[serverid] + '/tutorial/tutorial_view_js/' + nid + '?callback=?',
    'dataType': 'jsonp',
    'jsonpCallback': 'getPopup',
    'cache': true,
    'success': function(json) {
      $('#tutorials-popup-title').html('<h2>' + json.data.title + '</h2>');
      $('#tutorials-popup-body').html(json.data.body);
      $('#tutorials-popup').show();    
      $('#tutorials-popup').css('margin-left', '-' + ($('#tutorials-popup').width())/2 + 'px');
    }
  });
}

/**
 * Query tutr.tv for list of videos and add context links.
 */
Drupal.behaviors.tutorials  = {
  attach: function() {
    // If this is a popup, set up the div for the popup.
    if (Drupal.settings.tutorials.target == 'popup') {
      $('body').append('<div id="tutorials-popup"><div id="tutorials-popup-title"></div><div id="tutorials-popup-close">X</div><div id="tutorials-popup-body"></div></div>')
      $('#tutorials-popup-close').click(function() {
        $('#tutorials-popup').hide();
      });
    }
    for(var i in Drupal.settings.tutorials.query_urls) {
      var url = Drupal.settings.tutorials.query_urls[i];
      $.ajax({
        'url': url,
        'dataType': 'jsonp',
        'jsonpCallback': 'getLinks',
        'cache': true,
        'success': function(json) {
          var links = [];
          // Create the contextual links on the page.
          $.each(json.data, function(index, link) {
            // Add default links to default
            if (link.selector == "") {
              if ($(Drupal.settings.tutorials.default_selector).length > 0) {
                link.selector = Drupal.settings.tutorials.default_selector;
              }
              else {
                link.selector = 'body';
              }
            }
            if (Drupal.settings.tutorials.target == 'popup') {
              js_code = 'javascript:Drupal.tutorials.openPopup(\'' + link.tid + '\', \'' + i + '\'); return false;';
            }
            else if (Drupal.settings.tutorials.target == 'new_window') {
              js_code = 'javascript:window.open(\'' + Drupal.settings.tutorials.servers[i] + '/node/' + link.nid + '\'); return false;';
            }
            $('#tutorials-links-' + Drupal.tutorials.getContextID(link.selector)).append('<li><a href="#" title="' + link.summary + '" alt="' + link.summary + '" onclick="' + js_code + '" >' + link.title + '</a></li>');
          });
          // Set hover event for each context.
          $.each(Drupal.tutorials.contexts, function(index) {
            var $links = $('.tutorials-links');
            $(Drupal.tutorials.contexts[index]).hover(function(event) {
              // Get the right actions from the closure region
              var classes = ($(this).attr("class"));
              var identifier = '#tutorials-'+ index;
              $(this).prepend($(identifier));
              $(this).css({'position': 'relative'});

              // hide parent actions
              $('.tutorials').css('padding-left', 100);
              $links.hide();
              $('.tutorials').css('display', 'none');
              $('.tutorials-border').remove();
              $('a.tutorials-toggler').removeClass('tutorials-toggler-active');

              // Show current actions
              $('.tutorials:first', this).css('display', 'block').append(Drupal.tutorials.overlay);
            },
            function() {
              $('.tutorials', this).css('display', 'none');
              $('.tutorials-border', this).remove();
            });
          });
        }
      });
    }
  }
}

})(jQuery);
