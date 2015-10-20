// Non jquery function.
function pmb_block_get_page(id, url){
  var $pmb = jQuery.noConflict();

  $pmb('#' + id + '-page').fadeTo('fast', 0.4);
  $pmb('#' + id + '-page').load(Drupal.settings.basePath + '?q=' + url, function () {
    $pmb('#' + id + '-page').fadeTo('slow', 1);
  });
  return false;
};
