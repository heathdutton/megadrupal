Drupal.ajax_regions.after = function(link, response) {
  jQuery('#page-title').html(response.node_title);
  jQuery(".tabs").html('');
}
