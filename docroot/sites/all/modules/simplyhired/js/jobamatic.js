/**
 * @file
 * Main javascript file to add ajax search capabilities to SimplyHired Jobamatic module.
 */
(function($){
  Drupal.behaviors.jobamatic = {
    attach: function(context, settings) {
      var jobamatic_search_url = Drupal.settings.basePath + 'content/simply_hired_jobamatic/search';
      var wrap = $('#jobamatic-search-results');
      var form = $('#simply-hired-jobamatic-custom-search-form');

      if (form.length > 0) {
        form.submit(function(event){
          var f = event.target;
          var data = {query: $('#edit-jsq').val(), sb: $('#edit-sort-by').val(), ws: $('#edit-window-size option:selected').val()};
          var location = $.trim($('#edit-location').val());

          if (location != '') {
            data.l = location;
            data.mi = $('#edit-miles').val();
          }

          doJobSearch(jobamatic_search_url, data);
          // Stop the actual form submission.
          return false;
        });
      }

      doJobSearch(jobamatic_search_url, null);
    }
  }
})(jQuery);

/**
 * Executes ajax job search.
 *
 * @param string url - Ajax url to call.
 * @param data data - Data payload for the ajax call.
 */
function doJobSearch(url, data) {
  if (url == undefined || url == null) {
    return;
  }
  var wrap = jQuery('#jobamatic-search-results');
  wrap.html('<p class="jobamatic-wait">Searching jobs....</p>');

  jQuery.get(url, data, function(data, textStatus, jqXHR){
    data = jQuery(data);
    // Add click handlers to the pager items.
    data.find('.pager-item a, .pager-first a, .pager-previous a, .pager-next a, .pager-last a').click(function(){
      doJobSearch(jQuery(this).attr('href'));
      return false;
    });

    wrap.html('');
    wrap.append(data);
  }, 'html');
}
