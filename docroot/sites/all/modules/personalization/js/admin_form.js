/**
 * @file
 * JavaScript enhancements for the admin settings for.
 * 
 */
(function ($) {
  var pz_admin = {
    $vocabs: null,
    $vocab_guides: null,
    $geo_opts: null,
    $geo_weights: null,

    init: function() {
      pz_admin.$vocabs = $('#edit-pz-vocabularies input[type=checkbox]');
      pz_admin.$geo_opts = $('#edit-geographic-based input[type=checkbox]');
      pz_admin.$vocab_guides = $('#edit-pz-vocab-weight-title, #edit-pilez-vocab-weight-desc, #edit-pz-vocab-weight-desc');
      pz_admin.$geo_weights = $('.form-item-pz-geo-user-weight, .form-item-pz-geo-vocab-weight');

      $('#edit-taxonomony-based div.form-type-textfield').hide();
      pz_admin.$vocab_guides.hide();
      pz_admin.$geo_weights.hide();

      pz_admin.$vocabs.bind('change', pz_admin.vocab_weight_visibility);
      pz_admin.$vocabs.each(pz_admin.vocab_weight_visibility);

      pz_admin.$geo_opts.bind('change', pz_admin.geo_weight_visibility);
      pz_admin.$geo_opts.each(pz_admin.geo_weight_visibility);
    },

    vocab_weight_visibility: function() {
      if (this.checked) {
        $('input[type=text]#' + $(this).attr('id').replace('vocabularies', 'vocab-weight')).parent().show();
        pz_admin.$vocab_guides.show();
      }
      else {
        $('input[type=text]#' + $(this).attr('id').replace('vocabularies', 'vocab-weight')).parent().hide();
        if (!pz_admin.$vocabs.filter(':checked').length) {pz_admin.$vocab_guides.hide();}
      }
    },

    geo_weight_visibility: function() {
      if (this.checked) {
        pz_admin.$geo_weights.show();
      }
      else {
        if (!pz_admin.$geo_opts.filter(':checked').length) {pz_admin.$geo_weights.hide();}
      }
    }
  };

  $(function() {
    pz_admin.init();
  });

})(jQuery);
