(function ($) {
/**
 * States.
 */
Drupal.tests.testswarm_forms_states = {
  getInfo: function() {
    return {
      name: 'States',
      description: 'Tests for States.',
      group: 'System'
    };
  },
  tests: {
    optional_checked: function ($, Drupal) {
      return function() {
        expect(3);
        // Check if the required marker is present on page load.
        ok($('label[for="edit-text1"] span.form-required').length, Drupal.t('Required marker found'));
        // Check the checkbox to make text1 optional.
        $(':input[name="optionaltext1"]').click().trigger('change');
        // Check if the required marker was removed
        ok(!$('label[for="edit-text1"] span.form-required').length, Drupal.t('Required marker removed'));
        // Uncheck the checkbox to make text1 required.
        $(':input[name="optionaltext1"]').click().trigger('change');
        // Check if the required marker was added
        ok($('label[for="edit-text1"] span.form-required').length, Drupal.t('Required marker found'));
      }
    },
    disabled_checked: function ($, Drupal) {
      return function() {
        expect(3);
        // Check if the textfield is enabled on page load.
        ok(!$('#edit-text1').attr('disabled'), Drupal.t('Textfield is not disbabled'));
        // Check the checkbox to make text1 disabled.
        $(':input[name="disabletext1"]').click().trigger('change');
        // Check if the textfield is disabled
        ok($('#edit-text1').attr('disabled'), Drupal.t('Textfield is disabled'));
        // Uncheck the checkbox to make text1 enabled.
        $(':input[name="disabletext1"]').click().trigger('change');
        // Check if the textfield is enabled
        ok(!$('#edit-text1').attr('disabled'), Drupal.t('Textfield is not disbabled'));
      }
    },
    invisible_checked: function ($, Drupal) {
      return function() {
        expect(3);
        // Check if the textfield is visible on page load.
        ok($('#edit-text1').is(':visible'), Drupal.t('Textfield is visible'));
        // Check the checkbox to make text1 invisible.
        $(':input[name="hidetext1"]').click().trigger('change');
        // Check if the textfield is hidden
        ok(!$('#edit-text1').is(':visible'), Drupal.t('Textfield is invisible'));
        // Uncheck the checkbox to make text1 visible.
        $(':input[name="hidetext1"]').click().trigger('change');
        // Check if the textfield is visible
        ok($('#edit-text1').is(':visible'), Drupal.t('Textfield is visible'));
      }
    },
    visible_empty: function ($, Drupal) {
      return function() {
        expect(3);
        // Check if the text is visible on page load.
        ok($('#edit-item1').is(':visible'), Drupal.t('Text is visible'));
        // Enter something in the textfield to make the text invisible.
        $('#edit-text1').val(Drupal.t('Not empty')).trigger('keyup');
        // Check if the text is hidden
        ok(!$('#edit-item1').is(':visible'), Drupal.t('Text is invisible'));
        // Remove the data from the textfield to make the text visible.
        $('#edit-text1').val('').trigger('keyup');
        // Check if the text is visible
        ok($('#edit-item1').is(':visible'), Drupal.t('Text is visible'));
      }
    },
    visible_filled: function ($, Drupal) {
      return function() {
        expect(3);
        // Check if the text is invisible on page load.
        ok(!$('#edit-item2').is(':visible'), Drupal.t('Text is invisible'));
        // Enter something in the textfield to make the text visible.
        $('#edit-text1').val(Drupal.t('Not empty')).trigger('keyup');
        // Check if the text is visible
        ok($('#edit-item2').is(':visible'), Drupal.t('Text is visible'));
        // Remove the data from the textfield to make the text invisible.
        $('#edit-text1').val('').trigger('keyup');
        // Check if the text is invisible
        ok(!$('#edit-item2').is(':visible'), Drupal.t('Text is invisible'));
      }
    },
    visible_checked: function ($, Drupal) {
      return function() {
        expect(3);
        // Check if the textfield is invisible on page load.
        ok(!$('#edit-text2').is(':visible'), Drupal.t('Textfield is invisible'));
        // Check the checkbox to make text2 visible.
        $(':input[name="showtext2"]').click().trigger('change');
        // Check if the textfield is visible
        ok($('#edit-text2').is(':visible'), Drupal.t('Textfield is visible'));
        // Uncheck the checkbox to make text2 invisible.
        $(':input[name="showtext2"]').click().trigger('change');
        // Check if the textfield is visible
        ok(!$('#edit-text2').is(':visible'), Drupal.t('Textfield is invisible'));
      }
    },
    enabled_checked: function ($, Drupal) {
      return function() {
        expect(3);
        // Check the checkbox to make text2 visible, makes it easier to work with.
        $(':input[name="showtext2"]').click().trigger('change');
        // Check if the textfield is disabled on page load.
        ok($('#edit-text2').attr('disabled'), Drupal.t('Textfield is disbabled'));
        // Check the checkbox to make text1 enabled.
        $(':input[name="enabletext2"]').click().trigger('change');
        // Check if the textfield is enabled
        ok(!$('#edit-text2').attr('disabled'), Drupal.t('Textfield is enabled'));
        // Uncheck the checkbox to make text1 enabled.
        $(':input[name="enabletext2"]').click().trigger('change');
        // Check if the textfield is enabled
        ok($('#edit-text2').attr('disabled'), Drupal.t('Textfield is disbabled'));
        // Uncheck the checkbox to make text2 invisible again.
        $(':input[name="showtext2"]').click().trigger('change');
      }
    },
    required_checked: function ($, Drupal) {
      return function() {
        expect(3);
        // Check the checkbox to make text2 visible, makes it easier to work with.
        $(':input[name="showtext2"]').click().trigger('change');
        // Check if the required marker is present on page load.
        ok(!$('label[for="edit-text2"] span.form-required').length, Drupal.t('Required marker not found'));
        // Check the checkbox to make text1 required.
        $(':input[name="requiredtext2"]').click().trigger('change');
        // Check if the required marker was added
        ok($('label[for="edit-text2"] span.form-required').length, Drupal.t('Required marker found'));
        // Uncheck the checkbox to make text1 optional.
        $(':input[name="requiredtext2"]').click().trigger('change');
        // Check if the required marker was removed
        ok(!$('label[for="edit-text2"] span.form-required').length, Drupal.t('Required marker not found'));
        // Uncheck the checkbox to make text2 invisible again.
        $(':input[name="showtext2"]').click().trigger('change');
      }
    },
    checked_filled: function ($, Drupal) {
      return function() {
        expect(3);
        // Check if the checkbox is unchecked on page load.
        ok(!$('#edit-checkbox1').attr('checked'), Drupal.t('Checkbox not checked'));
        // Enter something in the textfield to check the checkbox.
        $(':input[name="checkcheckbox1"]').val(Drupal.t('This is some text')).trigger('keyup');
        // Check if the checkbox is checked
        ok($('#edit-checkbox1').attr('checked'), Drupal.t('Checkbox checked'));
        // Empty the textfield to uncheck the checkbox
        $(':input[name="checkcheckbox1"]').val('').trigger('keyup');
        // Check if the checkbox is unchecked
        ok(!$('#edit-checkbox1').attr('checked'), Drupal.t('Checkbox not checked'));
      }
    },
    unchecked_empty: function ($, Drupal) {
      return function() {
        expect(3);
        // Check if the checkbox is checked on page load.
        ok($('#edit-checkbox2').attr('checked'), Drupal.t('Checkbox checked'));
        // Empty the textfield to uncheck the checkbox
        $(':input[name="uncheckcheckbox2"]').val('').trigger('keyup');
        // Check if the checkbox is unchecked
        ok(!$('#edit-checkbox2').attr('checked'), Drupal.t('Checkbox not checked'));
        // Enter something in the textfield to check the checkbox.
        $(':input[name="uncheckcheckbox2"]').val(Drupal.t('This is some text')).trigger('keyup');
        // Check if the checkbox is checked
        ok($('#edit-checkbox2').attr('checked'), Drupal.t('Checkbox checked'));
      }
    },
    collapsed_value: function ($, Drupal) {
      return function() {
        expect(4);
        var collapseDelay = 1000;
        // Check if the fieldset is expanded on page load.
        ok($('#edit-fieldset1').find('div.fieldset-wrapper').is(':visible'), Drupal.t('Fieldset is initially expanded.'));
        // Enter 'collapse' in to the textfield to collapse the fieldset
        $(':input[name="collapsefieldset1"]').val('collapse').trigger('keyup');
        stop();
        setTimeout(function() {
          // Check if the fieldset is collapsed
          ok($('#edit-fieldset1').find('div.fieldset-wrapper').is(':hidden'), Drupal.t('Fieldset is collapsed.'));
          // Enter something else in to the textfield to expand the fieldset
          $(':input[name="collapsefieldset1"]').val(Drupal.t('This is some text')).trigger('keyup');
          setTimeout(function() {
            // Check if the fieldset is expanded
            ok($('#edit-fieldset1').find('div.fieldset-wrapper').is(':visible'), Drupal.t('Fieldset is expanded.'));
            // Empty the textfield and check if the fieldset is still expanded
            $(':input[name="collapsefieldset1"]').val('').trigger('keyup');
            setTimeout(function() {
              // Check if the fieldset is expanded
              ok($('#edit-fieldset1').find('div.fieldset-wrapper').is(':visible'), Drupal.t('Fieldset is expanded.'));
              start();
            }, collapseDelay);
          }, collapseDelay);
        }, collapseDelay);
      }
    },
    expanded_value: function ($, Drupal) {
      return function() {
        expect(4);
        var collapseDelay = 1000;
        // Check if the fieldset is collapsed on page load.
        ok($('#edit-fieldset2').find('div.fieldset-wrapper').is(':hidden'), Drupal.t('Fieldset is initially collapsed.'));
        // Enter 'expand' in to the textfield to expand the fieldset
        $(':input[name="expandfieldset2"]').val('expand').trigger('keyup');
        stop();
        setTimeout(function() {
          // Check if the fieldset is expanded
          ok($('#edit-fieldset2').find('div.fieldset-wrapper').is(':visible'), Drupal.t('Fieldset is expanded.'));
          // Enter something else in to the textfield to collapse the fieldset
          $(':input[name="expandfieldset2"]').val(Drupal.t('This is some text')).trigger('keyup');
          setTimeout(function() {
            // Check if the fieldset is collapsed
            ok($('#edit-fieldset2').find('div.fieldset-wrapper').is(':hidden'), Drupal.t('Fieldset is collapsed.'));
            // Empty the textfield and check if the fieldset is still collapsed
            $(':input[name="expandfieldset2"]').val('').trigger('keyup');
            setTimeout(function() {
              // Check if the fieldset is expanded
              ok($('#edit-fieldset2').find('div.fieldset-wrapper').is(':hidden'), Drupal.t('Fieldset is collapsed.'));
              start();
            }, collapseDelay);
          }, collapseDelay);
        }, collapseDelay);
      }
    }
  }
};
})(jQuery);
