/**
 * @file
 * Handles the dynamic functionality for the ercd-related forms.
 *
 * @ingroup ercd
 */
(function($) {
  Drupal.behaviors.ercd = {

    attach : function(context, settings) {
      // Set the initial state for the settings reference direction fieldset.
      var $radios = $('#edit-ercd-reference-direction .form-radio');
      $radios.each(function() {
        if ($(this).attr('checked')) {
          $(this).parent().addClass('selected');
          ercd_set_direction_state($('#edit-ercd-reference-direction-fieldset'), $(this));
        }
      });

      // Update the settings reference direction fieldset state.
      $('#edit-ercd-reference-direction .form-radio').change(function() {
        $('#edit-ercd-reference-direction .form-radio').parent().removeClass('selected');
        $(this).parent().addClass('selected');
        ercd_set_direction_state($('#edit-ercd-reference-direction-fieldset'), $(this));
      });

      // Set the initial state for the option fieldsets.
      var $fieldsets = $('.ercd-row fieldset');
      $fieldsets.each(function() {
        ercd_set_options_state($(this));
      });

      // Toggle child containers.
      $('.ercd-trigger').change(function() {
        if ($(this).attr('checked')) {
          $(this).parent().next('.ercd-child').fadeIn();
        } else {
          $(this).parent().next('.ercd-child').css('display', 'none');
        }
      });

      // Toggle checkboxes on the settings page.
      $('.ercd-setting').change(function() {
        var checked = $(this).attr('checked');

        // Update all child checkboxes if not checked.
        if (!checked) {
          // Get child container.
          var $ercdContainer = $(this).parent().next('.ercd-child');
          if ($ercdContainer.length) {
            ercd_toggle_checkboxes($ercdContainer, checked);
          }
        }

        // Set the parent state.
        var $ercdParentContainer = ercd_get_parent_container($(this));
        if (!checked || $ercdParentContainer.attr('tagName').toLowerCase() == 'fieldset') {
          ercd_set_options_state($ercdParentContainer);
        }
      });

      // Toggle checkboxes on the delete confirm page.
      $('.ercd-option').change(function() {
        // Get child container.
        var $ercdContainer = $(this).parent().next('.ercd-child');

        // Set the checkboxes for the child container if there is one.
        if ($ercdContainer.length) {
          var checked = $(this).attr('checked');
          ercd_toggle_checkboxes($ercdContainer, checked);
        }

        // Set the parent state.
        var $ercdParentContainer = ercd_get_parent_container($(this));
        ercd_set_options_state($ercdParentContainer);
      });

      // Toggle ercd options wrapper on an off for the user cancel page.
      $('#edit-user-cancel-method .form-radio').change(function() {
        var $ercdFieldsets = $('#ercd-delete-message').find('fieldset');
        var $ercdFieldset = $ercdFieldsets[0];

        if ($(this).val() == 'user_cancel_delete') {
          ercd_toggle_checkboxes($ercdFieldset, true);
          $('#ercd-delete-message').fadeIn();
        } else {
          ercd_toggle_checkboxes($ercdFieldset, false);
          $('#ercd-delete-message').css('display', 'none');
        }
      });

      // Select all button action.
      $('.ercd-select-all').click(function() {
        var $ercdContainer = $(this).closest('.ercd-child');
        ercd_toggle_checkboxes($ercdContainer, true);
      });

      // Unselect all button action.
      $('.ercd-unselect-all').click(function() {
        var $ercdContainer = $(this).closest('.ercd-child');
        ercd_toggle_checkboxes($ercdContainer, false);
      });
    }
  }

  /**
   * Gets an appropriate parent container for a checkbox.
   *
   * Will retrieve a parent fieldset if one exists, and will
   * retrieve a parent row if not.
   *
   * @param object $ercdCheckbox
   *   The checkbox for whom we wish to find a parent.
   *
   * @return object
   *   the jQuery container of the checkbox.
   */
  function ercd_get_parent_container($ercdCheckbox) {
    var $ercdContainer = $ercdCheckbox.closest('fieldset');

    // If the returned object is not a fieldset (i.e. the top level),
    // get the parent row instead.
    if ($ercdContainer.attr('tagName') != 'fieldset' && $ercdContainer.attr('tagName') != 'FIELDSET') {
      $ercdContainer = $ercdCheckbox.closest('.ercd-row');
    }

    return $ercdContainer;
  }

  /**
   * Toggle all checkboxes within a fieldset.
   *
   * Cycles through all checkboxes of a given fieldset
   * and toggles them based on the supplied toggle value.
   *
   * @param object $container
   *   A container that includes checkboxes.
   * @param bool toggleValue
   *   A boolean value that will determine if a checkbox
   *   should be selected of not.
   */
  function ercd_toggle_checkboxes($container, toggleValue) {
    // In this case, we are dealing with div.ercd-child,
    // so update all fieldsets.
    if ($container.attr('tagName').toLowerCase() != 'fieldset') {
      $fieldsets = $container.find('fieldset');

      // Loop through in reverse order so that we filter up and set the
      // correct user message.
      $($fieldsets.get().reverse()).each(function() {
        var ercdUserRole = $(this).hasClass('ercd-user-roles');
        if (!(ercdUserRole && toggleValue)) {
          if (ercdUserRole) {
            $(this).children('.fieldset-wrapper').children('.form-checkboxes').children('.form-type-checkbox').children('.form-checkbox').attr('checked', toggleValue);
          } else {
            $(this).children('.fieldset-wrapper').children('.form-type-checkbox').children('.form-checkbox').attr('checked', toggleValue);
          }
        }
        ercd_set_options_state($(this));
      })
    } else {
      $container.find('.form-checkbox').attr('checked', toggleValue);
      ercd_set_options_state($container);
    }
  }

  /**
   * Check if any checkboxes for a given container have been checked.
   *
   * If a checkbox has been check, if there is a child container, show it
   * and check all checkboxes within it. Otherwise, hide the child
   * container and unselect all of the checkboxes within it.
   *
   * @param object container
   *   A container that includes checkboxes.
   */
  function ercd_set_options_state($container) {
    var options = false;

    var $checkboxes = $container.find('.form-checkbox');
    $checkboxes.each(function() {
      var $ercdChild = $(this).parent().next('.ercd-child');
      if ($(this).attr('checked')) {
        options = true;
        $ercdChild.find('.ercd-child').css('display', 'block');
        $ercdChild.fadeIn();
      } else {
        var $fieldset = $ercdChild.find('fieldset');

        // This is borrowed from misc/collapse.js
        $fieldset.addClass('collapsed').find('> legend span.fieldset-legend-prefix').html(Drupal.t('Show'));

        $ercdChild.css('display', 'none');
      }
    });

    if ($container.attr('tagName').toLowerCase() == 'fieldset') {
      if ($container.hasClass('ercd-user-roles')) {
        if (options) {
          ercd_set_message($container, 'Roles Selected');
        } else {
          ercd_set_message($container, 'No roles Selected');
        }
      } else {
        if (options) {
          ercd_set_message($container, 'Entities Selected');
        } else {
          ercd_set_message($container, 'No Entities Selected');
        }
      }
    }
  }

  /**
   * Set the direction of the references and set the message.
   *
   * Change the message to the user to indicate the direction
   * of the references we will work with.
   *
   * @param object $fieldset
   *   A fieldset with radio buttons.
   * @param object $selectedRadio
   *   A radio button object.
   */
  function ercd_set_direction_state($fieldset, $selectedRadio) {
    var direction = $selectedRadio.val();

    switch(direction) {
      case 'desc':
        ercd_set_message($fieldset, 'Parent referencing children');
        break;

      case 'asc':
        ercd_set_message($fieldset, 'Children referencing parent');
        break;

      default:
        ercd_set_message($fieldset, 'No choice made');
    }
  }

  /**
   * Sets a user message in the fieldset label.
   *
   * If the user message span does not exist, we will append one.
   *
   * @param object $fieldset
   *   The fieldset whose message we need to set.
   * @param string message
   *   The message to be set.
   */
  function ercd_set_message($fieldset, message) {
    var legend = $fieldset.children('legend');
    var $fieldsetLegend = legend.find('.fieldset-legend');
    var $ercdMessage = $fieldsetLegend.find('.ercd-message');

    // Add a span for our messages if needed.
    if (!$ercdMessage.length) {
      $fieldsetTitle = $fieldsetLegend.find('.fieldset-title');
      if (!$fieldsetTitle.length) {
        // In case we get here BEFORE the fieldset has been altered by collapse.js.
        $fieldsetLegend.append('<span class="ercd-message ercd-alert"></span>');
        $ercdMessage = $fieldsetLegend.find('.ercd-message');
      } else {
        // In case we get here AFTER the fieldset has been altered by collapse.js.
        $ercdMessage = $fieldsetTitle.find('.ercd-message');
        if (!$ercdMessage.length) {
          $fieldsetTitle.append('<span class="ercd-message ercd-alert"></span>');
          $ercdMessage = $fieldsetTitle.find('.ercd-message');
        }
      }
    }

    $ercdMessage.html(Drupal.t('(@message)', {
      '@message' : message
    }));
  }

})(jQuery);
