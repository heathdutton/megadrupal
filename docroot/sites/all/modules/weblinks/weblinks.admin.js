/**
 * @file
 * jquery for weblinks admin pages.
 */

(function ($) {

function weblinks_collapse_handler(event) {
  if ($("input[name=weblinks_collapsible]:checked").val() == 1) {
    $("div.weblinks_collapse_hide").show();
    $("div.weblinks-link-title").hide();
  }
  else {
    $("div.weblinks_collapse_hide").hide();
    $("div.weblinks-link-title").show();
  }
}

function weblinks_link_display_handler(event) {
  // Node view "url" or "visit".
  if ($("input[name=weblinks_view_as]:checked").val() == 'url') {
    $("div.weblinks_trim_hide").show();
    $("div.weblinks_visit_hide").hide();
  }
  else {
    $("div.weblinks_trim_hide").hide();
    $("div.weblinks_visit_hide").show();
  }
}

function weblinks_setup() {
  // Collapse the fieldsets, bottom to top.
  Drupal.toggleFieldset('#weblinks-link-settings');
  Drupal.toggleFieldset('#weblinks-group-settings');
  Drupal.toggleFieldset('#weblinks-page-settings');
}

// Run the javascript on page load.
  $(document).ready(function () {
  // On page load, determine the default settings.
  weblinks_collapse_handler();
  weblinks_link_display_handler();

  // Bind the functions to click events.
  $("input[name=weblinks_collapsible]").bind("click", weblinks_collapse_handler);
  $("input[name=weblinks_view_as]").bind("click", weblinks_link_display_handler);
  
  weblinks_setup();
  });

})(jQuery);
