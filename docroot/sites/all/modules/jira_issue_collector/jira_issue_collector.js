(function ($) {

Drupal.behaviors.JiraIssueCollector = {
  attach: function(context, settings) {
    // Use ajax() instead of getScript() as this allows cache to be enabled.
    // This is preferable for performance reasons. The JIRA Issue Collector
    // script should not change much.
    if (typeof settings.jiraIssueCollector != 'undefined') {
      jQuery.ajax({
        url: settings.jiraIssueCollector.url,
        type: "get",
        dataType: "script",
        cache: true,
        success: function() {
          // If we use a issue collector located at the top of the page
          // ("Prominent" style) and show a toolbar, then the toolbar will be
          // positioned over the issue collector button. Adjust the position so
          // it shows below the toolbar.
          // We cannot set attributes directly on the element as we do not know
          // when it will be available.
          // .atlwdg-TOP class ensures we affect only issue collectors with the
          // prominent style.
          $('<style>')
            .attr("type", "text/css")
            .html('body.toolbar .atlwdg-TOP { top: ' + Drupal.toolbar.height() + 'px; }')
            .appendTo('head');
        }
      });
    }
  }
};

})(jQuery);
