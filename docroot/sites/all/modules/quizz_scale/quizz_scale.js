(function ($, Drupal) {

  Drupal.behaviors.quizzScaleQuestionForm = {
    attach: function (context, settings) {
      $('.form-select[name="presets"]', context).change(function () {
        var alternatives = settings.quizz_scale_alternatives.alternatives[$(this).val()];
        var max = settings.quizz_scale_alternatives.max_alternatives;

        // Clears all the alternatives on the scale question form
        for (var i = 0; i < max; i++)
          $('#edit-alternative' + (i)).val('');

        // Apply collection items
        var i = 0;
        for (var key in alternatives)
          $('#edit-alternative' + (i++)).val(alternatives[key]);
      });
    }
  };

})(jQuery, Drupal);
