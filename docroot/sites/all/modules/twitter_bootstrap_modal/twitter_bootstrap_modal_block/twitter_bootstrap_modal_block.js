(function ($) {

/**
* @file
* Javascript support files.
*
*/

Drupal.behaviors.twitter_bootstrap_modal_block = {
  attach: function (context, settings) {

    var TBBtrigger = Drupal.settings.twitter_bootstrap_modal_block.trigger;
    var link_type = Drupal.settings.twitter_bootstrap_modal_block.link_type;
    
    // This triggers block as modal.
    $(TBBtrigger).once('TBB', function () {
      var title = $(this).children('h2').text();
      $(this).after('<section id="btn-' + $(this).attr('id') + '" class="sideral-corner after"><button class="btn ' + link_type + ' btn-lg" data-target="#' + $(this).attr('id') + '" data-toggle="modal">' + title + '</button></section>');
      $(this).addClass("modal fade");
      $(this).attr("tabindex", "-1");
      $(this).attr("role", "dialog");
      $(this).attr("aria-hidden", "true");
      $(this).children('h2').wrap('<div class="modal-header" />');
      $(this).children('.modal-header').prepend('<button type="button" class="close" data-dismiss="modal" aria-hidden="true">×</button>');
      $(this).children('.block-content').addClass('modal-body');
      $(this).append('<div class="modal-footer"><button class="btn" data-dismiss="modal" aria-hidden="true">' + Drupal.t('Close') + '</button></div>');
      $(this).children('div').wrapAll('<div class="modal-content" />');
      $(this).children('.modal-content').wrap('<div class="modal-dialog" />');
      $(this).appendTo('body');
    });
  }
}


}(jQuery));

