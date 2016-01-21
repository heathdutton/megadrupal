(function($) {
  var History = window.History;
  if (!History.enabled) {
    return false;
  }

  /**
   * Redirect if page loads with hash data
   */
  if (location.hash.substr(0, 3) == '#./') {
    location.href = location.hash.substr(3);
  }

  Drupal.behaviors.ajaxify_pages = {};
  Drupal.behaviors.ajaxify_pages.attach = function(context, settings) {
    $(Drupal.settings.ajaxify_pages.links_selector, context).each(function() {
      var href = $(this).addClass('ajaxify-pages-processed').attr('href');
      if (typeof href != 'undefined') {
        $(this).click(function() {
          History.pushState(null, null, href);
          return false;
        });
      }
    });

    if (typeof Drupal.settings.ajaxify_pages.processed != 'undefined') {
      return false;
    }
    Drupal.settings.ajaxify_pages.processed = 'processed';
    Drupal.ajaxify_pages = {cache: {}};
    
    History.Adapter.bind(window, 'statechange', function() {
      loadPage(History.getState().url);
    });
    Drupal.ajaxify_pages.cache[History.getState().url] = {
      title: $('title').html(),
      content: $(Drupal.settings.ajaxify_pages.content_selector)
    };
  };

  /**
   * Get the page from the page cache or server; and process it
   */
  var loadPage = function(url) {
    if (Drupal.ajaxify_pages.cache[url]) {
      processPage(Drupal.ajaxify_pages.cache[url], url);
    }
    else {
      $.ajax({
        url: url,
        success: function(data, textStatus, jqXHR) {
          var title = data.match(/<title>([^<]+)<\/title>/)[1];
          var content = $(data).filter(Drupal.settings.ajaxify_pages.content_selector);
          Drupal.ajaxify_pages.cache[url] = {
            title: title,
            content: content
          };
          processPage(Drupal.ajaxify_pages.cache[url], url);
        },
        error: function(jqXHR, textStatus, errorThrown) {
          document.location.href = url;
          return false;
        }
      });
    }
  };

  /**
   * Process page - Replace content, set page title, process analytics
   */
  var processPage = function(page, url) {
    // Change page title
    document.title = page.title;
    //$(document).attr('title', page.title);
    // Replace content
    $(Drupal.settings.ajaxify_pages.content_selector).replaceWith(page.content);
    
    // Change meta tags
    
    // Attach behaviors
    Drupal.attachBehaviors($(Drupal.settings.ajaxify_pages.content_selector));
    
    // Track Google Analytics pageview
    /*if (typeof _gaq != 'undefined') {
      _gaq.push(['_trackPageview', url]);
    }*/
  };
  
})(jQuery);
