/**
 * @file
 *  Updates the Media Nivo Slider fieldset summary based on the status of the nivo slider block on the current media_gallery node.
 */

(function ($) {

Drupal.behaviors.mediaNivoSliderForm = {
  attach: function (context) {
    $('fieldset.media-nivo-slider-form', context).drupalSetSummary(function (context) {
      if ($('#edit-media-nivo-slider-block-und', context).attr('checked')) {
        return Drupal.t('Enabled');
      }
      else {
        return Drupal.t('Not enabled');
      }
    });

    // Toggle the box and slice specific fields based on the selected effect.
    // Once http://drupal.org/node/735528 is resolved in core handle this functionality with form states.
    Drupal.behaviors.mediaNivoSliderForm.showOrHideEffectOptions($('#edit-media-nivo-slider-effect-und').val());
  

    $('#edit-media-nivo-slider-effect-und').change(function() {
      Drupal.behaviors.mediaNivoSliderForm.showOrHideEffectOptions($(this).val());
    });
  },

  showOrHideEffectOptions: function(effect) {
    switch(effect) {
      // If the chosen effect is a slice effect show the slices field.
      case 'sliceDown':
      case 'sliceDownLeft':
      case 'sliceUp':
      case 'sliceUpLeft':
      case 'sliceUpDown':
      case 'sliceUpDownLeft':
        $('#edit-media-nivo-slider-slices').show();
        $('#edit-media-nivo-slider-box-cols').hide();
        $('#edit-media-nivo-slider-box-rows').hide();
        break;

      // If the chosen effect is a box effect show the box fields.
      case 'boxRandom':
      case 'boxRain':
      case 'boxRainReverse':
      case 'boxRainGrow':
      case 'boxRainGrowReverse':
        $('#edit-media-nivo-slider-box-cols').show();
        $('#edit-media-nivo-slider-box-rows').show();
        $('#edit-media-nivo-slider-slices').hide();
        break;

      default:
        $('#edit-media-nivo-slider-slices').hide();
        $('#edit-media-nivo-slider-box-cols').hide();
        $('#edit-media-nivo-slider-box-rows').hide();
        break;
    }
  }
};



})(jQuery);
