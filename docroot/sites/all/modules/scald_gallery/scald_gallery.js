/**
 * @file
 * Scald Gallery JS integration.
 */

(function($) {

Drupal.behaviors.scald_gallery = {
  attach: function(context, settings) {
    // Use gallery modal settings for gallery atoms.
    // Do not not use $('...', context) or the add gallery button does not work
    // in the first place.
    $('.dnd-library-wrapper')
      .find('.meta.type-gallery .edit a, .add-buttons .add-gallery a')
      .removeClass('ctools-modal-custom-style')
      .addClass('ctools-modal-scald_gallery');
  }
};

}) (jQuery);

