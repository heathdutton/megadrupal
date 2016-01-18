/**
 * @file
 * JavaScript for the ballot paper form in the election_stv module.
 */
(function ($) {

  Drupal.behaviors.election_vote = {

    attach: function (context, settings) {

      var select_elems = $('.election-candidate-preference-select', context),
        disableOptions = function (triggering_elem, others) {
          var val = triggering_elem.val(), prev = triggering_elem.data('previous_value');
          if (prev !== undefined) {
            others.find('option[value="' + prev + '"][disabled]').removeAttr('disabled');
          }
          if (val !== '' && val !== 'NONE') {
            others.find('option[value="' + val + '"]').not(':selected').attr('disabled', 'disabled');
          }
          triggering_elem.data('previous_value', val);
        };

      select_elems.each(function () {
        var this_elem = $(this), others = select_elems.not(this_elem), allow_equal = this_elem.closest('form').hasClass('allow-equal');
        if (!allow_equal) {
          disableOptions(this_elem, others);
          this_elem.change(function () {
            disableOptions(this_elem, others);
          });
        }
      });
    }

  };

}(jQuery));
