/**
 * @file
 * Adds job search functionality to latest jobs block.
 */
(function($){
  Drupal.behaviors.jobamatic = {
    attach: function(context, settings) {
      doJobSearch(Drupal.settings.basePath + 'content/simply_hired_jobamatic/search', {ws: 5, pager:0, mode: 'block'});
    }
  }
})(jQuery);

/**
 * Executes ajax call to search for jobs using the default settings.
 */
function doJobSearch(url, data) {
  if (url == undefined || url == null) {
    return;
  }

  var wrap = jQuery('#jobamatic-latest-job-results');
  wrap.html('<p class="jobamatic-wait">Searching jobs....</p>');

  jQuery.get(url, data, function(data, textStatus, jqXHR){
    data = jQuery(data);

    wrap.html('');
    wrap.append(data);
  }, 'html');
}
